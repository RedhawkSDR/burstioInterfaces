#ifndef BURSTIO_OUTPORT_FIXTURE_H
#define BURSTIO_OUTPORT_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>
#include <burstio.h>

class Burstio_OutPort : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Burstio_OutPort );
  CPPUNIT_TEST( test_create_int8 );
  CPPUNIT_TEST( test_int8 );
  CPPUNIT_TEST( test_create_int16 );
  CPPUNIT_TEST( test_int16 );
  CPPUNIT_TEST( test_create_int32);
  CPPUNIT_TEST( test_int32 );
  CPPUNIT_TEST( test_create_int64);
  CPPUNIT_TEST( test_int64 );
  CPPUNIT_TEST( test_create_uint8 );
  CPPUNIT_TEST( test_uint8 );
  CPPUNIT_TEST( test_create_uint16 );
  CPPUNIT_TEST( test_uint16 );
  CPPUNIT_TEST( test_create_uint32);
  CPPUNIT_TEST( test_uint32 );
  CPPUNIT_TEST( test_create_uint64);
  CPPUNIT_TEST( test_uint64 );
  CPPUNIT_TEST( test_create_float );
  CPPUNIT_TEST( test_float );
  CPPUNIT_TEST( test_create_double );
  CPPUNIT_TEST( test_double );
  //CPPUNIT_TEST( test_subclass );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_create_int8();
  void test_int8();
  void test_create_int16();
  void test_int16();
  void test_create_int32();
  void test_int32();
  void test_create_int64();
  void test_int64();
  void test_create_uint8();
  void test_uint8();
  void test_create_uint16();
  void test_uint16();
  void test_create_uint32();
  void test_uint32();
  void test_create_uint64();
  void test_uint64();
  void test_create_float();
  void test_float();
  void test_create_double();
  void test_double();
  //  void test_subclass();

  template < typename T > void test_setget_api( T *port );
  template < typename T > void test_connection_filter( T *port );
  //  template < typename T > void test_multiout_api( T *port );

  rh_logger::LoggerPtr logger;
};

#endif  // BURSTIO_OutPort_FIXTURE_H
