########################################################################
# $Id: Makefile 1177 2008-01-21 19:24:49Z gherron $
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

VPATH = ../lib
CXXFLAGS = -I. -I../lib -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/
LIBS = -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

libsrc = framework.cpp geomlib.cpp scenelib.cpp transformation.cpp common.cpp

root = CS350-framework
target = ../$(root).exe
bindir = ../$(root)-bin

src = scene.cpp render.cpp geomlib-advanced.cpp common.cpp

objects = $(patsubst %.cpp,$(bindir)/%.o,$(libsrc) $(src))

$(target): $(bindir) $(objects)
	g++ -g  -o $@  $(patsubst %,$(bindir)/%,$(objects)) $(LIBS)

$(bindir):
	mkdir $(bindir)

$(objects): $(bindir)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(bindir)/$@
