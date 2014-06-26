#include "Burstio_InPort.h"
#include "burstio.h"
#include "bulkio.h"

/**
#include<log4cxx/logger.h>
#include<log4cxx/propertyconfigurator.h>
#include <log4cxx/logstring.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logmanager.h>
**/

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Burstio_InPort );

class MyFloatPort : public burstio::BurstFloatIn {

public:

  MyFloatPort( std::string pname ):
    burstio::BurstFloatIn( pname ) {};

  //
  // over ride default behavior for pushPacket and pushSRI
  //
  void pushBursts(const burstio::BurstFloatIn::BurstSequenceType & bursts) {
    burstio::BurstFloatIn::pushBursts( bursts );
  }

};


void 
Burstio_InPort::setUp()
{
   logger = rh_logger::Logger::getLogger("BurstioInPort");
   logger->setLevel( rh_logger::Level::getError());
}


void 
Burstio_InPort::tearDown()
{
}


template< typename T >
void  Burstio_InPort::test_port_api( T *port  ) 
{
  RH_INFO(logger, "BURSTIO-API-TEST Port:" << port->getName() );

  // try and assign logger to port
  port->setLogger(logger);

  BULKIO::PortStatistics *stats = port->statistics();
  CPPUNIT_ASSERT( stats != NULL );
  delete stats;

  BULKIO::PortUsageType rt = port->state();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST Invalid Port State", rt == BULKIO::IDLE );

  // set to default queue threshold
  size_t tmp = port->getQueueThreshold();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_IN_PORT_TEST Get Threshold Failed", tmp == 100 );

  // set to default queue threshold
  tmp = port->getQueueDepth();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_IN_PORT_TEST Queue Depth != 0 ", tmp == 0 );

  // try set-get operations
  port->setQueueThreshold(22);
  tmp = port->getQueueThreshold();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_IN_PORT_TEST QUEUE SET/GET Failed:",  tmp == 22 );

  // enable data flow into port
  port->start();

  // check that port queue is empty
  typename T::PacketType *pkt  = port->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts should return null pkt", NULL == pkt );

  // check other 
  typename T::BurstSequenceType *seq_pkt  = port->getBursts(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts returned bad sequence", seq_pkt != NULL );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts Sequence length invalid", seq_pkt->length() == 0 );
  delete seq_pkt;

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
  typename T::BurstSequenceType bursts;
  bursts.length(1);
  typename T::BurstType burst;
  burst.SRI = sri;
  burst.EOS = false;
  burst.T = burstio::utils::now();
  burst.data.length(50);
  bursts[0] = burst;

  port->pushBursts( bursts );

  // set to default queue threshold
  tmp = port->getQueueDepth();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getQueueDepth should be 1", tmp == 1);

  //
  // Grab off first entry..   check that pieces are not corrupt
  //
  pkt  = port->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts PKT != NULL ", NULL != pkt );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts EOS mismatch ", false == pkt->getEOS() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts Data Length mismatch ", 50 == pkt->getSize() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts Mode ", 0 == pkt->isComplex() );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI mismatch ", sid == pkt->getStreamID().c_str() );
  BURSTIO::BurstSRI asri = pkt->getSRI();
  CORBA::String_var t = asri.modulation;
  std::string  expected("mod");
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI.modulation", expected == t.in() );
  t = asri.fec;  
  expected = "vit";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI.fec", expected == t.in() );
  t = asri.fecrate;  
  expected = "7/8";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI.fec", expected == t.in() );
  t = asri.randomizer;  
  expected = "R20";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI.fec", expected == t.in() );
  t = asri.overhead;  
  expected = "unknown";
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts SRI.overhead", expected == t.in() );

  // disable data flow into port
  port->stop();

  port->pushBursts( bursts );

  // set to default queue threshold
  tmp = port->getQueueDepth();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getQueueDepth should be 1", tmp == 0);

  //
  // Try and grab a packet, should be empty
  //
  pkt  = port->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_IN_PORT_TEST getBursts PKT != NULL ", NULL == pkt );


  RH_INFO(logger, "BURSTIO_IN_PORT_TEST END ---- PORT:" << port->getName() );

}

template< typename T >
void  Burstio_InPort::test_push_flush_sequence( T *port  ) 
{
  RH_INFO(logger, "BURSTIO-PUSH/FLUSH START PORT:" << port->getName() );

  size_t tmp;
  typename T::PacketType *pkt;
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
  typename T::BurstSequenceType bursts;
  bursts.length(1);
  typename T::BurstType burst;

  int totalBursts=55;
  port->setQueueThreshold(totalBursts);

  //
  // Testing Push/Flush Sequence
  //

  // enable data flow into port
  port->start();

  int i,j;
  totalBursts=0;
  for ( i=0; i<10;i++ ) {
    // test out flush works and no issue occurrs
    bursts.length(i+1);
    for ( j=0; j<i+1;j++ ) {
      burst.SRI = sri;
      burst.EOS = false;
      burst.T = burstio::utils::now();
      burst.data.length(50);
      totalBursts++;
      bursts[j] = burst;
    }
    port->pushBursts( bursts );
  }

  // set to default queue threshold
  tmp = port->getQueueDepth();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO-PUSH/FLUSH  getQueueDepth should be 1", tmp == totalBursts);

  port->flush();
  
  // set to default queue threshold
  tmp = port->getQueueDepth();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO-PUSH/FLUSH getQueueDepth should be 1", tmp == 0);

  //
  // Try and grab a packet, should be empty
  //
  pkt  = port->getBurst(bulkio::Const::NON_BLOCKING );
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO-PUSH/FLUSH getBursts PKT != NULL ", NULL == pkt );

  RH_INFO(logger, "BURSTIO-PUSH/FLUSH END PORT:" << port->getName() );
}

void 
Burstio_InPort::test_create_int8()
{
  burstio::BurstByteIn *port = new burstio::BurstByteIn("test_ctor_int8" );
  CPPUNIT_ASSERT( port != NULL );

  CPPUNIT_ASSERT_NO_THROW( port );
}


void 
Burstio_InPort::test_int8()
{
  burstio::BurstByteIn *port = new burstio::BurstByteIn("test_api_int8" );
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_int16()
{
  burstio::BurstShortIn *port = new burstio::BurstShortIn("test_ctor_int16");
  CPPUNIT_ASSERT( port != NULL );

  CPPUNIT_ASSERT_NO_THROW( port );
}


void 
Burstio_InPort::test_int16()
{
  burstio::BurstShortIn *port = new burstio::BurstShortIn("test_api_int16");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}

void 
Burstio_InPort::test_create_int32()
{
  burstio::BurstLongIn *port = new burstio::BurstLongIn("test_ctor_int32");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_InPort::test_int32()
{
  burstio::BurstLongIn *port = new burstio::BurstLongIn("test_api_int32");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_int64()
{
  burstio::BurstLongLongIn *port = new burstio::BurstLongLongIn("test_ctor_int64");
  CPPUNIT_ASSERT( port != NULL );
}


void 
Burstio_InPort::test_int64()
{
  burstio::BurstLongLongIn *port = new burstio::BurstLongLongIn("test_api_int64");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_uint8()
{
  burstio::BurstUbyteIn *port = new burstio::BurstUbyteIn("test_api_int64");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_InPort::test_uint8()
{
  burstio::BurstUbyteIn *port = new burstio::BurstUbyteIn("test_api_uint8");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}

void 
Burstio_InPort::test_create_uint16()
{
  burstio::BurstUshortIn *port = new burstio::BurstUshortIn("test_ctor_uint16");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_InPort::test_uint16()
{
  burstio::BurstUshortIn *port = new burstio::BurstUshortIn("test_api_uint16");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_uint32()
{
  burstio::BurstUlongIn *port = new burstio::BurstUlongIn("test_ctor_uint32");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_InPort::test_uint32()
{
  burstio::BurstUlongIn *port = new burstio::BurstUlongIn("test_api_uint32");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_uint64()
{
  burstio::BurstUlongLongIn *port = new burstio::BurstUlongLongIn("test_ctor_uint64");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_InPort::test_uint64()
{
  burstio::BurstUlongLongIn *port = new burstio::BurstUlongLongIn("test_api_uint64");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}



void 
Burstio_InPort::test_create_float()
{
  burstio::BurstFloatIn *port = new burstio::BurstFloatIn("test_ctor_float");
  CPPUNIT_ASSERT( port != NULL );
}


void 
Burstio_InPort::test_float()
{
  burstio::BurstFloatIn *port = new burstio::BurstFloatIn("test_api_float");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_InPort::test_create_double()
{
  burstio::BurstDoubleIn *port = new burstio::BurstDoubleIn("test_ctor_float");
  CPPUNIT_ASSERT( port != NULL );
}



void 
Burstio_InPort::test_double()
{
  burstio::BurstDoubleIn *port = new burstio::BurstDoubleIn("test_api_float");
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}




void 
Burstio_InPort::test_subclass()
{
  burstio::BurstFloatIn *port = new MyFloatPort("test_api_subclass" );
  
  CPPUNIT_ASSERT( port != NULL );

  test_port_api( port );

  CPPUNIT_ASSERT_NO_THROW( port );
}

