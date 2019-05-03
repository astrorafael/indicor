## Process this file with automake to produce Makefile.in

dnl ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -Wall
AM_CFLAGS   = -Wall

SUBDIRS = common log resources items factory driver

## ------------------------------------------------------------------------
## The common shared library for the main driver program and plug-ins
## The nodist_EXTRA_libindicor_la_SOURCES trick selects C++ linker
## ------------------------------------------------------------------------

pkginclude_HEADERS = api.h utils.h

lib_LTLIBRARIES    = libindicor.la

libindicor_la_CPPFLAGS= \
-I$(top_srcdir)/common \
-I$(top_srcdir)/log \
-I$(top_srcdir)/resources \
-I$(top_srcdir)/items \
-I$(top_srcdir)/factory 

libindicor_la_SOURCES  =
nodist_EXTRA_libindicor_la_SOURCES = dummy.cpp	
libindicor_la_LIBADD  = \
$(top_builddir)/common/libcommon.la \
$(top_builddir)/log/liblog.la \
$(top_builddir)/resources/libresources.la \
$(top_builddir)/items/libitems.la \
$(top_builddir)/factory/libfactory.la

libindicor_la_LDFLAGS = -no-undefined -version-info 0:0:0

## ------------------------------------------------------------------------
## The distributed XML configuration files
## ------------------------------------------------------------------------

dist_pkgdata_DATA = data/config.xml data/relationships.xml data/log.xml

## dist_pkgdata_DATA = $(top_builddir)/config.xml $(top_builddir)/log.xml data/relationships.xml 

## Preprocess the files to substitute for actual installation paths

do_subst = sed -e 's,[@]pkgdatadir[@],$(pkgdatadir),g' \
	-e 's,[@]libdir[@],$(libdir),g' 


install-data-hook:
	$(do_subst) < $(DESTDIR)$(pkgdatadir)/config.xml > /tmp/config.xml
	$(do_subst) < $(DESTDIR)$(pkgdatadir)/log.xml    > /tmp/log.xml
	cp  /tmp/config.xml  $(DESTDIR)$(pkgdatadir)/config.xml 
	cp  /tmp/log.xml     $(DESTDIR)$(pkgdatadir)/log.xml 


## ------------------------------------------------------------------------
## Builds the standard INDI programs
## 1) The standard indi superserver that forks drivers.
## 2) The skeleton driver program that hosts plug-ins in this architecture
## ------------------------------------------------------------------------

bin_PROGRAMS = indiserver pacorro

indiserver_SOURCES = indiserver.c
indiserver_CPPFLAGS= -I$(top_srcdir)/common
indiserver_LDFLAGS= -static
indiserver_LDADD   = $(top_builddir)/common/liblilxml.la

pacorro_SOURCES = drivermain.cpp
pacorro_CPPFLAGS= -DPKGDATADIR=\"$(pkgdatadir)\" \
-I$(top_srcdir)/common \
-I$(top_srcdir)/log \
-I$(top_srcdir)/resources \
-I$(top_srcdir)/items \
-I$(top_srcdir)/factory \
-I$(top_srcdir)/driver 

pacorro_LDADD   = libindicor.la $(top_builddir)/driver/libdriver.la -ldl


