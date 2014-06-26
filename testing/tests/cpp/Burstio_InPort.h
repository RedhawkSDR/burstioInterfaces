#ifndef BURSTIO_INPORT_FIXTURE_H
#define BURSTIO_INPORT_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>
#include "ossie/debug.h"

class Burstio_InPort : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Burstio_InPort );
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
  CPPUNIT_TEST( test_subclass );
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
  void test_subclass();

  template < typename T > void test_port_api( T *port );
  template < typename T > void test_push_flush_sequence( T *port );

  rh_logger::LoggerPtr logger;
};

#endif  // BURSTIO_InPort_FIXTURE_H
