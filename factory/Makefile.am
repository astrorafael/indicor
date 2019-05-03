## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir) \
-I$(top_srcdir)/common \
-I$(top_srcdir)/log \
-I$(top_srcdir)/resources \
-I$(top_srcdir)/items


AM_CXXFLAGS = -Wall

## Set of installable headers
pkginclude_HEADERS = resfactory.h

## Convenient library around this package

noinst_LTLIBRARIES   = libfactory.la
libfactory_la_SOURCES = sysfactory.cpp sysfactory.h hubfactory.cpp hubfactory.h
libfactory_la_LDFLAGS = -static

