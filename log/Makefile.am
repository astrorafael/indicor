## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/common
AM_CXXFLAGS = -Wall

## Set of installable headers
pkginclude_HEADERS  = log.h logfact.h

## Convenient library around this package

noinst_LTLIBRARIES  = liblog.la
liblog_la_LDFLAGS = -static
liblog_la_SOURCES = log.cpp logfact.cpp xlogfact.cpp  xlogfact.h \
	logbase.h logbase.cpp log0.h log1.h log2.h log3.h log4.h
