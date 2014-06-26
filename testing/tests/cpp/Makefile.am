
ACLOCAL_AMFLAGS = -I m4 -I ${OSSIEHOME}/share/aclocal/ossie -I acinclude

burstio_top=../../..
burstio_cpplib_top=$(burstio_top)/src/cpp
Burstio_SOURCES = Burstio.cpp Burstio_InPort.cpp Burstio_OutPort.cpp  Burstio_PushTest.cpp
Burstio_INCLUDES = -I$(burstio_cpplib_top)/include -I$(burstio_cpplib_top)/include/burstio  -I$(burstio_cpplib_top)/redhawk
Burstio_CXXFLAGS = $(CPPUNIT_CFLAGS) $(Burstio_INCLUDES) $(BOOST_CPPFLAGS) $(BULKIO_CFLAGS) $(RH_DEPS_CFLAGS)
Burstio_LDADD = -L$(burstio_cpplib_top)/.libs -lburstio -lburstioInterfaces $(BULKIO_LIBS) $(BOOST_SYSTEM_LIB) $(RH_DEPS_LIBS) -llog4cxx $(CPPUNIT_LIBS) -ldl
Burstio_LDFLAGS = $(CPPUNIT_LIBS)  


TESTS = Burstio
noinst_PROGRAMS=$(TESTS)
check_PROGRAMS=$(TESTS)

