########################################################################
# $Id: Makefile 1015 2007-06-12 16:23:20Z gherron $
#
# Makefile for Linux
#
# Copyright © 2005 DigiPen Institute of Technology
########################################################################

Framework-OBJS = framework.o geomlib.o render.o scenelib.o scene.o

Geometry-OBJS = Geometry.o geomlib.o
BSPTree-OBJS = framework.o geomlib.o BSPTree-render.o scenelib.o BSPTree-scene.o
RayCast-OBJS = framework.o geomlib.o RayCast-render.o scenelib.o RayCast-scene.o RayCast.o

ZBuffer-OBJS = framework.o geomlib.o render.o scenelib.o ZBuffer-scene.o
Phong-OBJS = framework.o geomlib.o render.o scenelib.o Phong-scene.o
Texture-OBJS = framework.o geomlib.o render.o scenelib.o Texture-scene.o
Bump-OBJS = framework.o geomlib.o render.o scenelib.o Bump-scene.o
Reflection-OBJS = framework.o geomlib.o render.o scenelib.o Reflection-scene.o

CXXFLAGS = -I. -g -I/usr/X11R6/include/GL/ -I/usr/include/GL/

EXES = Framework Geometry BSPTree RayCast ZBuffer Phong Texture Bump Reflection 

all: $(EXES)

run0: $(EXES)
	./Geometry
	./BSPTree
	./RayCast

run5: $(EXES)
	./ZBuffer
	./Phong
	./Texture
	./Bump
	./Reflection

Framework: $(Framework-OBJS)
	g++ $(Framework-OBJS) -g -o Framework -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Geometry: $(Geometry-OBJS)
	g++ $(Geometry-OBJS) -g -o Geometry

BSPTree: $(BSPTree-OBJS)
	g++ $(BSPTree-OBJS) -g -o BSPTree -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

RayCast: $(RayCast-OBJS)
	g++ $(RayCast-OBJS) -g -o RayCast -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

ZBuffer: $(ZBuffer-OBJS)
	g++ $(ZBuffer-OBJS) -g -o ZBuffer -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Phong: $(Phong-OBJS)
	g++ $(Phong-OBJS) -g -o Phong -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Texture: $(Texture-OBJS)
	g++ $(Texture-OBJS) -g -o Texture -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Bump: $(Bump-OBJS)
	g++ $(Bump-OBJS) -g -o Bump -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Reflection: $(Reflection-OBJS)
	g++ $(Reflection-OBJS) -g -o Reflection -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXmu

Bump-scene.o: Bump-scene.cpp framework.h geomlib.h scenelib.h
Geometry.o: Geometry.cpp geomlib.h
Phong-scene.o: Phong-scene.cpp framework.h geomlib.h scenelib.h
Reflection-scene.o: Reflection-scene.cpp framework.h geomlib.h scenelib.h
Texture-scene.o: Texture-scene.cpp framework.h geomlib.h scenelib.h
BSPTree-scene.o: BSPTree-scene.cpp framework.h geomlib.h scenelib.h
ZBuffer-scene.o: ZBuffer-scene.cpp framework.h geomlib.h scenelib.h
framework.o: framework.cpp framework.h geomlib.h scenelib.h
geomlib.o: geomlib.cpp geomlib.h
render.o: render.cpp framework.h geomlib.h scenelib.h
scenelib.o: scenelib.cpp framework.h geomlib.h scenelib.h
