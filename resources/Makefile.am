## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/common -I$(top_srcdir)/log
AM_CXXFLAGS = -Wall
AM_CFLAGS   = -Wall

## Set of installable headers
pkginclude_HEADERS  = event.h corproto.h mux.h demux.h \
	timer.h alarm.h command.h macrocmd.h cmdqueue.h

## Convenient libraries around this package

noinst_LTLIBRARIES  = libresources.la
libresources_la_SOURCES = pipevent.h udpmux.cpp corproto.c \
	udpmux.h udpdemux.h \
	alarmimp.h adaptimer.h macrocmd.cpp corqueue.h corqueue.cpp \
	qcommand.h qcommand.cpp
libresources_la_LDFLAGS = -static
