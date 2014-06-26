#include "Burstio_OutPort.h"
#include "bulkio.h"
#include "burstio.h"
/*
#include<log4cxx/logger.h>
#include<log4cxx/propertyconfigurator.h>
#include <log4cxx/logstring.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logmanager.h>
*/
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Burstio_OutPort );

class MyFloatPort : public burstio::BurstFloatOut {

public:

  MyFloatPort( std::string pname ):
    burstio::BurstFloatOut( pname ) {};

  //
  // over ride default behavior for pushPacket and pushSRI
  //
  void pushBursts(const burstio::BurstFloatOut::BurstSequenceType & bursts) {
    burstio::BurstFloatOut::pushBursts( bursts );
  }

};


void 
Burstio_OutPort::setUp()
{
   logger = rh_logger::Logger::getLogger("BurstioOutPort");
   logger->setLevel( rh_logger::Level::getError());
}


void 
Burstio_OutPort::tearDown()
{
}


template< typename T >
void  Burstio_OutPort::test_setget_api( T *port  ) 
{
  RH_INFO(logger, "BURSTIO-API-TEST Port:" << port->getName() );

  // try and assign logger to port
  port->setLogger(logger);

  BULKIO::UsesPortStatisticsSequence *stats = port->statistics();
  CPPUNIT_ASSERT( stats != NULL );
  delete stats;

  BULKIO::PortUsageType rt = port->state();
  CPPUNIT_ASSERT_MESSAGE( "BURSTIO_OUT_PORT_TEST Invalid Port State", rt == BULKIO::IDLE );

  // set to default queue threshold
  size_t tmp = port->getMaxBursts();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_OUT_PORT_TEST Get Max Bursts Failed", tmp == 100 );

  //
  // test max bursts state
  //
  tmp=22;
  port->setMaxBursts(tmp);
  tmp = port->getMaxBursts();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_OUT_PORT_TEST Get Max Bursts Failed", tmp == 22 );

  //
  // test byte threshold state
  //
  tmp=0xdeedbeef;
  port->setByteThreshold(tmp);
  tmp = port->getByteThreshold();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_OUT_PORT_TEST Get Byte Threshold Failed", tmp == 0xdeedbeef );


  //
  // test latency threshold
  //
  tmp=123456789;
  port->setLatencyThreshold(tmp);
  tmp = port->getLatencyThreshold();
  CPPUNIT_ASSERT_MESSAGE("BURSTIO_OUT_PORT_TEST Get Byte Threshold Failed", tmp == 123456789 );

  // test start/stop sequence
  port->start();
  port->stop();

  port->start();
  port->stop();


  port->start();
  port->flush();
  port->flush();
  port->flush();
  port->stop();

  //
  // try and grab a statistics object... should be null
  //
  stats = port->statistics();
  CPPUNIT_ASSERT( stats != NULL );
  delete stats;

}

struct conn_filter {
  std::string port_name;
  std::string connection_id;
  std::string stream_id;
  //  conn_filt( std::string &p, std::string &c, std::string &s ) : port_name(p), connection_id(c), stream_id(s) {};
  conn_filter( std::string &p, const char *c, const char *s ) : port_name(p), connection_id(c), stream_id(s) {};
};



template< typename T >
void  Burstio_OutPort::test_connection_filter( T *port  ) 
{

  RH_INFO(logger, "BURSTIO-API-TEST CONNECTION FILTER Port:" << port->getName() );
  
  std::string pname = port->getName();
  std::vector< conn_filter >  filter;
  filter.push_back( conn_filter( pname, "connection-1", "stream-1-1"));
  filter.push_back( conn_filter( pname, "connection-1", "stream-1-2"));
  filter.push_back( conn_filter( pname, "connection-2", "stream-2-1"));
  filter.push_back( conn_filter( pname, "connection-2", "stream-2-2"));
  filter.push_back( conn_filter( pname, "connection-3", "stream-3-1"));
  filter.push_back( conn_filter( pname, "connection-3", "stream-3-2"));
  filter.push_back( conn_filter( pname, "connection-4", "stream-4-1"));
  port->updateConnectionFilter( filter );

  // simple add remove sequence
  port->addConnectionFilter( "stream-4-2", "connection-4");
  port->addConnectionFilter( "stream-4-3", "connection-4");

  port->removeConnectionFilter( "stream-4-2", "connection-4");
  port->removeConnectionFilter( "stream-4-3", "connection-4");


  // simple add remove to create possible errant handling of ids
  port->addConnectionFilter( "stream-4-3", "connection-4");


  port->removeConnectionFilter( "stream-4-3", "connection-4");
  port->removeConnectionFilter( "stream-4-3", "connection-4");
  port->removeConnectionFilter( "stream-4-3", "connection-4");
  port->removeConnectionFilter( "stream-4-3", "connection-4");

}


void 
Burstio_OutPort::test_create_int8()
{
  burstio::BurstByteOut *port = new burstio::BurstByteOut("test_ctor_int8" );
  CPPUNIT_ASSERT( port != NULL );

  CPPUNIT_ASSERT_NO_THROW( port );
}


void 
Burstio_OutPort::test_int8()
{
  burstio::BurstByteOut *port = new burstio::BurstByteOut("test_api_int8" );
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  test_connection_filter( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_int16()
{
  burstio::BurstShortOut *port = new burstio::BurstShortOut("test_ctor_int16");
  CPPUNIT_ASSERT( port != NULL );

  CPPUNIT_ASSERT_NO_THROW( port );
}


void 
Burstio_OutPort::test_int16()
{
  burstio::BurstShortOut *port = new burstio::BurstShortOut("test_api_int16");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}

void 
Burstio_OutPort::test_create_int32()
{
  burstio::BurstLongOut *port = new burstio::BurstLongOut("test_ctor_int32");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_OutPort::test_int32()
{
  burstio::BurstLongOut *port = new burstio::BurstLongOut("test_api_int32");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_int64()
{
  burstio::BurstLongLongOut *port = new burstio::BurstLongLongOut("test_ctor_int64");
  CPPUNIT_ASSERT( port != NULL );
}


void 
Burstio_OutPort::test_int64()
{
  burstio::BurstLongLongOut *port = new burstio::BurstLongLongOut("test_api_int64");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_uint8()
{
  burstio::BurstUbyteOut *port = new burstio::BurstUbyteOut("test_api_int64");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_OutPort::test_uint8()
{
  burstio::BurstUbyteOut *port = new burstio::BurstUbyteOut("test_api_uint8");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}

void 
Burstio_OutPort::test_create_uint16()
{
  burstio::BurstUshortOut *port = new burstio::BurstUshortOut("test_ctor_uint16");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_OutPort::test_uint16()
{
  burstio::BurstUshortOut *port = new burstio::BurstUshortOut("test_api_uint16");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_uint32()
{
  burstio::BurstUlongOut *port = new burstio::BurstUlongOut("test_ctor_uint32");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_OutPort::test_uint32()
{
  burstio::BurstUlongOut *port = new burstio::BurstUlongOut("test_api_uint32");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_uint64()
{
  burstio::BurstUlongLongOut *port = new burstio::BurstUlongLongOut("test_ctor_uint64");
  CPPUNIT_ASSERT( port != NULL );
}

void 
Burstio_OutPort::test_uint64()
{
  burstio::BurstUlongLongOut *port = new burstio::BurstUlongLongOut("test_api_uint64");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}



void 
Burstio_OutPort::test_create_float()
{
  burstio::BurstFloatOut *port = new burstio::BurstFloatOut("test_ctor_float");
  CPPUNIT_ASSERT( port != NULL );
}


void 
Burstio_OutPort::test_float()
{
  burstio::BurstFloatOut *port = new burstio::BurstFloatOut("test_api_float");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}


void 
Burstio_OutPort::test_create_double()
{
  burstio::BurstDoubleOut *port = new burstio::BurstDoubleOut("test_ctor_float");
  CPPUNIT_ASSERT( port != NULL );
}



void 
Burstio_OutPort::test_double()
{
  burstio::BurstDoubleOut *port = new burstio::BurstDoubleOut("test_api_float");
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  //test_push_flush_sequence( port );

  CPPUNIT_ASSERT_NO_THROW( port );

}

#if 0
void 
Burstio_OutPort::test_subclass()
{
  burstio::BurstFloatOut *port = new MyFloatPort("test_api_subclass" );
  
  CPPUNIT_ASSERT( port != NULL );

  test_setget_api( port );

  CPPUNIT_ASSERT_NO_THROW( port );
}

#endif
