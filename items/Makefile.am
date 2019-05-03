## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir) \
-I$(top_srcdir)/common \
-I$(top_srcdir)/log \
-I$(top_srcdir)/resources \
-I$(top_srcdir)/factory 

AM_CXXFLAGS = -Wall

## Set of installable headers
pkginclude_HEADERS  = item.h itembase.h compoitem.h \
	proputils.h property.h propvector.h \
	device.h plugin.h plugbase.h

## Convenient library around the composite pattern for properties

noinst_LTLIBRARIES  = libitems.la
libitems_la_LDFLAGS = -static
libitems_la_SOURCES = item.cpp itembase.cpp compoitem.cpp \
	proputils.cpp property.cpp propvector.cpp device.cpp plugbase.cpp
