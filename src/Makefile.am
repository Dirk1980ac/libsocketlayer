## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS = \
	 -Wall \
	 -g3

lib_LTLIBRARIES = libsocketlayer.la


libsocketlayer_la_SOURCES = \
	lib.c

libsocketlayer_la_LDFLAGS = 

libsocketlayer_la_LIBADD = 


if PLATFORM_WIN32
libsocketlayer_la_LDFLAGS += -lws2_32 -no-undefined
endif

if NATIVE_WIN32
libsocketlayer_la_LDFLAGS += -lws2_32 -export-dynamic -avoid-version
endif

include_HEADERS = \
	socketlayer.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = socketlayer.pc

EXTRA_DIST = \
	socketlayer.pc.in
