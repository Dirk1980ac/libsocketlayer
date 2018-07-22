dnl Process this file with autoconf to produce a configure script.

AC_INIT(socketlayer,[m4_esyscmd_s([git describe --long --always])], \
        Dirk Gottschalk <dirk.gottschalk1980@googlemail.com)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE()
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])
AC_PROG_CC

dnl ***************************************************************************
dnl Check for Windows
dnl ***************************************************************************
AC_CANONICAL_HOST

case $host_os in
  *mingw*)
    platform_win32=yes
    native_win32=yes
    ;;
  pw32* | *cygwin*)
    platform_win32=yes
    native_win32=no
    ;;
  *)
    platform_win32=no
    native_win32=no
    ;;
esac
AM_CONDITIONAL(PLATFORM_WIN32, test x"$platform_win32" = "xyes")
AM_CONDITIONAL(NATIVE_WIN32, test x"$native_win32" = "xyes")

AC_CHECK_HEADERS([arpa/inet.h windef.h netinet/in.h stdlib.h sys/socket.h unistd.h windows.h winsock2.h error.h sys/types.h netdb.h])

LT_INIT([win32-dll])

AC_CONFIG_FILES([Makefile
		src/socketlayer.pc
		src/Makefile])
AC_OUTPUT
