## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = -Wall

noinst_LTLIBRARIES   = libdriver.la
libdriver_la_CPPFLAGS = \
-I$(top_srcdir) \
-I$(top_srcdir)/common \
-I$(top_srcdir)/log \
-I$(top_srcdir)/resources \
-I$(top_srcdir)/items \
-I$(top_srcdir)/factory 


libdriver_la_LDFLAGS = -static
libdriver_la_SOURCES = driver.cpp driver.h handler.cpp handler.h \
	devreader.cpp devreader.h manager.cpp manager.h \
	relreader.cpp relreader.h



