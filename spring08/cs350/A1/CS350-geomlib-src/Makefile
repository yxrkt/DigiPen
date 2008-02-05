########################################################################
# $Id: Makefile 1205 2008-01-30 21:47:57Z gherron $
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

VPATH = ../lib
CXXFLAGS = -I. -I../lib -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = 

libsrc = geomlib-main.cpp geomlib.cpp

root = CS350-geomlib
target = ../$(root).exe
bindir = ../$(root)-bin

src = geomlib-advanced.cpp

objects = $(patsubst %.cpp,$(bindir)/%.o,$(libsrc) $(src))

$(target): $(bindir) $(objects)
	g++ -g  -o $@  $(patsubst %,$(bindir)/%,$(objects)) $(LIBS)

$(bindir):
	mkdir $(bindir)

$(objects): $(bindir)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(bindir)/$@
