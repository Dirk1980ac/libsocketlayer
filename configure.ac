dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(socketlayer, 2.0.0)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE()
AM_MAINTAINER_MODE
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

LT_INIT([win32-dll])

AC_OUTPUT([
Makefile
src/socketlayer.pc
src/Makefile
])