## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)

AM_CXXFLAGS = -Wall

## Set of installable headers
pkginclude_HEADERS  = indicom.h timestamp.h

## Convenient libraries around this package
## The XML parser is built isolated so we can statically link it to indiserver

noinst_LTLIBRARIES   = liblilxml.la libcommon.la
liblilxml_la_LDFLAGS = -static
liblilxml_la_SOURCES = lilxml.c
libcommon_la_SOURCES = eventloop.c eventloop.h indicom.c timestamp.c lilxml.h
libcommon_la_LDFLAGS = -static
libcommon_la_LIBADD  = liblilxml.la
