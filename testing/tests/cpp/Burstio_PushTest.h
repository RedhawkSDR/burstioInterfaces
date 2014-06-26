#ifndef BURSTIO_PUSHTEST_FIXTURE_H
#define BURSTIO_PUSHTEST_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>
#include<omniORB4/CORBA.h>
#include<bulkio.h>
#include<burstio.h>

template< typename OUT_PORT, typename IN_PORT >
class Burstio_PushBursts: public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Burstio_PushBursts );
  CPPUNIT_TEST( test_push_simple );
  CPPUNIT_TEST( test_fan_out );
  CPPUNIT_TEST( test_fan_in );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  virtual void test_push_simple();
  virtual void test_push_sequence();
  virtual void test_fan_out();
  virtual void test_fan_in();

  template < typename T > void test_port_api( T *port );

  rh_logger::LoggerPtr logger;

  CORBA::ORB_ptr orb;

  std::string  lname;

  IN_PORT *ip1; 
  IN_PORT *ip2;
  IN_PORT *ip3;
  IN_PORT *ip4;
  OUT_PORT *op1;
  OUT_PORT *op2;
  OUT_PORT *op3;
  OUT_PORT *op4;

  PortableServer::ObjectId_var ip1_oid;
  PortableServer::ObjectId_var ip2_oid;
  PortableServer::ObjectId_var ip3_oid; 
  PortableServer::ObjectId_var ip4_oid;
  PortableServer::ObjectId_var op1_oid;
  PortableServer::ObjectId_var op2_oid;
  PortableServer::ObjectId_var op3_oid;
  PortableServer::ObjectId_var op4_oid;

  std::vector< bulkio::connection_descriptor_struct >  desc_list;


};

typedef Burstio_PushBursts< burstio::BurstByteOut, burstio::BurstByteIn >   PushBurstsUInt8;
typedef Burstio_PushBursts< burstio::BurstShortOut, burstio::BurstShortIn >   PushBurstsInt16;
typedef Burstio_PushBursts< burstio::BurstUshortOut, burstio::BurstUshortIn >   PushBurstsUInt16;
typedef Burstio_PushBursts< burstio::BurstLongOut, burstio::BurstLongIn >   PushBurstsInt32;
typedef Burstio_PushBursts< burstio::BurstUlongOut, burstio::BurstUlongIn >   PushBurstsUInt32;
typedef Burstio_PushBursts< burstio::BurstLongLongOut, burstio::BurstLongLongIn >   PushBurstsInt64;
typedef Burstio_PushBursts< burstio::BurstUlongLongOut, burstio::BurstUlongLongIn >   PushBurstsUInt64;
typedef Burstio_PushBursts< burstio::BurstDoubleOut, burstio::BurstDoubleIn >   PushBurstsDouble;
typedef Burstio_PushBursts< burstio::BurstFloatOut, burstio::BurstFloatIn >   PushBurstsFloat;


#define DEF_TEST( IP, OP, TNAME, NAME ) class PushBursts##TNAME##_Fixture : public  Burstio_PushBursts< burstio::Burst##OP##Out, burstio::Burst##IP##In > \
{ \
  CPPUNIT_TEST_SUITE( PushBursts##TNAME##_Fixture ); \
  CPPUNIT_TEST( test_push_simple ); \
  CPPUNIT_TEST( test_push_sequence ); \
  CPPUNIT_TEST( test_fan_in ); \
  CPPUNIT_TEST( test_fan_out ); \
  CPPUNIT_TEST_SUITE_END(); \
public: \
\
 PushBursts##TNAME##_Fixture () : PushBursts##TNAME (){ \
    this->lname=#NAME; \
  }; \
};


DEF_TEST( Byte, Byte, UInt8, UINT8 );
DEF_TEST( Short, Short, Int16, INT16 );
DEF_TEST( Ushort, Ushort, UInt16, UINT16 );
DEF_TEST( Long, Long, Int32, INT32 );
DEF_TEST( Ulong, Ulong, UInt32, UINT32 );
DEF_TEST( LongLong, LongLong, Int64, INT64 );
DEF_TEST( UlongLong, UlongLong, UInt64, UINT64 );
DEF_TEST( Double, Double, Double, DOUBLE );
DEF_TEST( Float, Float, Float, FLOAT );


#endif  // BURSTIO_PushTest_FIXTURE_H
