
dnl Process this file with autoconf to produce a configure script.
AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(Burstio_Test,0.1)
AM_PATH_CPPUNIT(1.9.6)
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PREFIX_DEFAULT(${OSSIEHOME})
AC_CORBA_ORB

OSSIE_CHECK_OSSIE
OSSIE_OSSIEHOME_AS_PREFIX
PKG_CHECK_MODULES([RH_DEPS], [ossie >= 1.7 omniORB4 >= 4.0.0])
PKG_CHECK_MODULES([BULKIO], [ bulkio >= 1.0 bulkioInterfaces >= 1.9 ])

AX_BOOST_BASE([1.41])
AX_BOOST_THREAD
AX_BOOST_SYSTEM

# set PKG_CONFIG_PATH to look at local xxx.pc files

export PKG_CONFIG_PATH="../../../..:../../..":${PKG_CONFIG_PATH}
PKG_CHECK_MODULES([BIO], [ burstio >= 1.0 burstioInterfaces >= 1.8 ])

AC_OUTPUT(Makefile)
