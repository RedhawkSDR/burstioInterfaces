#include "Burstio_PushTest.h"
#include "bulkio.h"
#include "burstio.h"


template < typename OUT_PORT, typename IN_PORT >
void Burstio_PushBursts< OUT_PORT, IN_PORT >::setUp()
{
  logger = rh_logger::Logger::getLogger("Burstio_PushBursts -" + lname );
  logger->setLevel( rh_logger::Level::getError());
  orb = ossie::corba::CorbaInit(0,NULL);

  RH_INFO(logger, "Setup - Burstio Create Ports Table " );

  ip1 = new IN_PORT("sink_1" );
  ip1_oid = ossie::corba::RootPOA()->activate_object(ip1);
  ip2 = new IN_PORT("sink_2" );
  ip2_oid = ossie::corba::RootPOA()->activate_object(ip2);
  ip3 = new IN_PORT("sink_3" );
  ip3_oid = ossie::corba::RootPOA()->activate_object(ip3);
  ip4 = new IN_PORT("sink_4" );
  ip4_oid = ossie::corba::RootPOA()->activate_object(ip4);
  op1 = new OUT_PORT("multiout_source" );
  op1_oid = ossie::corba::RootPOA()->activate_object(op1);
  op2 = new OUT_PORT("sink_2" );
  op2_oid = ossie::corba::RootPOA()->activate_object(op2);
  op3 = new OUT_PORT("sink_3" );
  op3_oid = ossie::corba::RootPOA()->activate_object(op3);
  op4 = new OUT_PORT("sink_4" );
  op4_oid = ossie::corba::RootPOA()->activate_object(op4);


  desc_list.clear();
  RH_INFO(logger, "Setup - Burstio Connection Table " );
  bulkio::connection_descriptor_struct desc;
  desc.connection_id = "connection_1";
  desc.stream_id = "stream-1-1";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_1";
  desc.stream_id = "stream-1-2";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_1";
  desc.stream_id = "stream-1-3";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_2";
  desc.stream_id = "stream-2-1";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_2";
  desc.stream_id = "stream-2-2";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_2";
  desc.stream_id = "stream-2-3";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_3";
  desc.stream_id = "stream-3-1";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_3";
  desc.stream_id = "stream-3-2";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_3";
  desc.stream_id = "stream-3-3";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);
  desc.connection_id = "connection_4";
  desc.stream_id = "stream-4-1";
  desc.port_name = "multiout_source";
  desc_list.push_back(desc);

}



template < typename OUT_PORT, typename IN_PORT >
void Burstio_PushBursts< OUT_PORT, IN_PORT >::tearDown()
{

  RH_INFO(logger, "TearDown - Deactivate Servants " );
  ossie::corba::RootPOA()->deactivate_object(ip1_oid);
  ossie::corba::RootPOA()->deactivate_object(ip2_oid);
  ossie::corba::RootPOA()->deactivate_object(ip3_oid);
  ossie::corba::RootPOA()->deactivate_object(ip4_oid);
  ossie::corba::RootPOA()->deactivate_object(op1_oid);
  ossie::corba::RootPOA()->deactivate_object(op2_oid);
  ossie::corba::RootPOA()->deactivate_object(op3_oid);
  ossie::corba::RootPOA()->deactivate_object(op4_oid);

  RH_INFO(logger, "TearDown - Shutdown the ORB " );
  //orb->shutdown(1);
}


//
// test_multiout_sri_filtered()
//

template < typename OUT_PORT, typename IN_PORT >
void  Burstio_PushBursts< OUT_PORT, IN_PORT >::test_push_simple( ) {

  RH_INFO(logger, "Burstio Flow - SIMPLE FLOW - BEGIN " );

  ExtendedCF::UsesConnectionSequence *clist = op1->connections();
  CPPUNIT_ASSERT( clist != NULL );
  delete clist;

  std::string cid("connection_1");

  RH_INFO(logger, "Burstio Flow - Create Connections and Filter list " );
  op1->connectPort( ip1->_this(), cid.c_str());

  // need to allow for flow of data
  op1->start();
  ip1->start();

  BULKIO::PrecisionUTCTime TS = burstio::utils::now();
  BURSTIO::BurstSRI sri;
  std::string sid = "test_port_api";
  std::string id = "id-1";
  sri.streamID = sid.c_str();
  sri.id = id.c_str();
  sri.xdelta = 1.0;
  sri.mode = 0;       /* 0-Scalar, 1-Complex */
  sri.flags = 0;
  sri.tau = 1.1;
  sri.theta = 1.2;
  sri.gain = 1.3;
  sri.uwlength = 128;
  sri.bursttype = 2;
  sri.burstLength = 1024;
  sri.CHAN_RF = 1e6;
  sri.baudestimate = 2.0;
  sri.carrieroffset = 2.1;
  sri.SNR = 2.3;
  sri.modulation = "mod";
  sri.baudrate = 56000.0;
  sri.fec = "vit";       /* fec type */
  sri.fecrate = "7/8";   /* not a number for TPC rates like (64,57)x(64,57) */
  sri.randomizer="R20";
  sri.overhead="unknown";
  sri.expectedStartOfBurstTime=TS;


  // push Bursts
  typename OUT_PORT::BurstSequenceType bursts;
  bursts.length(1);
  typename OUT_PORT::BurstType burst;
  burst.SRI = sri;
  burst.EOS = false;
  burst.T = burstio::utils::now();
  burst.data.length(50);
  bursts[0] = burst;

  op1->pushBursts( bursts);

  typename IN_PORT::PacketType *pkt;
  RH_INFO(logger, "Burstio Flow - Simple Flow -- GetPacket " );
  pkt  = ip1->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts PKT == NULL ", NULL != pkt );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts EOS mismatch ", false == pkt->getEOS() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts Data Length mismatch ", 50 == pkt->getSize() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts Mode ", 0 == pkt->isComplex() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI mismatch ", sid == pkt->getStreamID().c_str() );
  RH_INFO(logger, "Burstio Flow - Simple Flow -- Passed basic PKT examination " );
  BURSTIO::BurstSRI asri = pkt->getSRI();
  CORBA::String_var t = asri.modulation;
  std::string  expected("mod");
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI.modulation", expected == t.in() );
  t = asri.fec;  
  expected = "vit";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI.fec", expected == t.in() );
  t = asri.fecrate;  
  expected = "7/8";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI.fec", expected == t.in() );
  t = asri.randomizer;  
  expected = "R20";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI.fec", expected == t.in() );
  t = asri.overhead;  
  expected = "unknown";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts SRI.overhead", expected == t.in() );
  RH_INFO(logger, "Burstio Flow - Simple Flow -- Passed basic SRI examination " );

  RH_INFO(logger, "Burstio Flow - Simple Flow -- GetPacket " );
  pkt  = ip1->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_SIMPLE_FLOW getBursts PKT != NULL ", NULL == pkt );

  op1->disconnectPort(cid.c_str());
  RH_INFO(logger, "Burstio Flow - Simple Flow -- Disconnected" );

  RH_INFO(logger, "Burstio Flow - SIMPLE FLOW - END " );

}



template < typename OUT_PORT, typename IN_PORT >
void  Burstio_PushBursts< OUT_PORT, IN_PORT >::test_push_sequence( ) {

  RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE - BEGIN " );

  ExtendedCF::UsesConnectionSequence *clist = op1->connections();
  CPPUNIT_ASSERT( clist != NULL );
  delete clist;

  std::string cid("connection_1");

  RH_INFO(logger, "Burstio Flow - Create Connections and Filter list " );
  op1->connectPort( ip1->_this(), cid.c_str());

  // need to allow for flow of data
  op1->start();
  ip1->start();

  BULKIO::PrecisionUTCTime TS = burstio::utils::now();
  BURSTIO::BurstSRI sri;
  std::string sid = "test_port_api";
  std::string id = "id-1";
  sri.streamID = sid.c_str();
  sri.id = id.c_str();
  sri.xdelta = 1.0;
  sri.mode = 0;       /* 0-Scalar, 1-Complex */
  sri.flags = 0;
  sri.tau = 1.1;
  sri.theta = 1.2;
  sri.gain = 1.3;
  sri.uwlength = 128;
  sri.bursttype = 2;
  sri.burstLength = 1024;
  sri.CHAN_RF = 1e6;
  sri.baudestimate = 2.0;
  sri.carrieroffset = 2.1;
  sri.SNR = 2.3;
  sri.modulation = "mod";
  sri.baudrate = 56000.0;
  sri.fec = "vit";       /* fec type */
  sri.fecrate = "7/8";   /* not a number for TPC rates like (64,57)x(64,57) */
  sri.randomizer="R20";
  sri.overhead="unknown";
  sri.expectedStartOfBurstTime=TS;


  // push Bursts
  typename OUT_PORT::BurstSequenceType bursts;
  int i,j;
  int nseq=10;
  int burst_count[nseq];
  int totalBursts=0;
  for ( i=0; i<nseq;i++ ) {
    // test out flush works and no issue occurrs
    int nbursts=i+1;
    burst_count[i] = nbursts;
    bursts.length( nbursts );
    for ( j=0; j< nbursts;j++ ) {
      typename OUT_PORT::BurstType burst;
      burst.SRI = sri;
      burst.EOS = false;
      burst.T = burstio::utils::now();
      burst.data.length(50);
      totalBursts++;
      bursts[j] = burst;
    }
    //RH_INFO(logger, "Burstio Flow - OP1 PushBurts " << bursts.length()  << " == " << burst_count[i] );
    op1->pushBursts( bursts );
  }

  RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE -- SENT NSEQ:" << nseq << " TOTAL BURSTS:" << totalBursts );

  typename IN_PORT::BurstSequenceVar in_bursts;
  for ( i=0; i<nseq;i++ ) {  
    RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE -- GET BURST SEQ:" << i );
    in_bursts  = ip1->getBursts(bulkio::Const::NON_BLOCKING );
    CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts Sequence is empty ",  in_bursts->length() != 0  );
    CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts Sequence Len mismatch ", in_bursts->length() == burst_count[i]);
  
    RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE -- IN BURSTS:" << in_bursts->length() );
    for ( j=0; j < in_bursts->length(); j++ ) {
      RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE -- GET BURST:" << j);
      typename IN_PORT::BurstType burst=in_bursts[j];
      //burst = in_bursts[j];
    
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts EOS mismatch ", false == burst.EOS );
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts Data Length mismatch ", 50 == burst.data.length() );
      RH_INFO(logger, "Burstio Flow - Push Sequence -- Passed basic BURST examination " );
      BURSTIO::BurstSRI asri = burst.SRI;
      CORBA::String_var t;

      t = burst.SRI.streamID;
      std::string  expected(sid);
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts StreamID mismatch ", expected == t.in() );

      t = asri.modulation;
      expected = "mod";
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts SRI.modulation", expected == t.in() );
      t = asri.fec;  
      expected = "vit";
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts SRI.fec", expected == t.in() );
      t = asri.fecrate;  
      expected = "7/8";
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts SRI.fec", expected == t.in() );
      t = asri.randomizer;  
      expected = "R20";
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts SRI.fec", expected == t.in() );
      t = asri.overhead;  
      expected = "unknown";
      CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts SRI.overhead", expected == t.in() );
      RH_INFO(logger, "Burstio Flow - Push Sequence -- Passed basic SRI examination " );
    }

  }

  RH_INFO(logger, "Burstio Flow - Push Seqeunce-- GetPacket " );
  typename IN_PORT::PacketType *pkt  = ip1->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_PUSH_SEQUENCE getBursts PKT != NULL ", NULL == pkt );

  op1->disconnectPort(cid.c_str());
  RH_INFO(logger, "Burstio Flow - Push Sequence -- Disconnected" );

  RH_INFO(logger, "Burstio Flow - PUSH SEQUENCE - END " );

}



template < typename OUT_PORT, typename IN_PORT >
void  Burstio_PushBursts< OUT_PORT, IN_PORT >::test_fan_in( ) {

  RH_INFO(logger, "Burstio Flow - FAN IN - BEGIN " );
  RH_INFO(logger, "Burstio Flow - FAN IN - END " );

}

template < typename OUT_PORT, typename IN_PORT >
void  Burstio_PushBursts< OUT_PORT, IN_PORT >::test_fan_out( ) {

  RH_INFO(logger, "Burstio Flow - FAN OUT - BEGIN " );
  RH_INFO(logger, "Burstio Flow - FAN OUT - END " );

}



// Registers the fixture into the 'registry'
// this also worked sans type name in output CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsUInt8 );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsUInt8_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsInt16_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsUInt16_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsInt32_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsUInt32_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsInt64_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsUInt64_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsDouble_Fixture );
CPPUNIT_TEST_SUITE_REGISTRATION( PushBurstsFloat_Fixture );

//template class PushBursts< burstio::BurstCharPort, burstio::BurstInt8Port >;
template class Burstio_PushBursts< burstio::BurstByteOut, burstio::BurstByteIn >;
template class Burstio_PushBursts< burstio::BurstShortOut, burstio::BurstShortIn >;
template class Burstio_PushBursts< burstio::BurstUshortOut, burstio::BurstUshortIn >;
template class Burstio_PushBursts< burstio::BurstLongOut, burstio::BurstLongIn >;
template class Burstio_PushBursts< burstio::BurstUlongOut, burstio::BurstUlongIn >;
template class Burstio_PushBursts< burstio::BurstLongLongOut, burstio::BurstLongLongIn >;
template class Burstio_PushBursts< burstio::BurstUlongLongOut, burstio::BurstUlongLongIn >;
template class Burstio_PushBursts< burstio::BurstDoubleOut, burstio::BurstDoubleIn >;
template class Burstio_PushBursts< burstio::BurstFloatOut, burstio::BurstFloatIn >;
