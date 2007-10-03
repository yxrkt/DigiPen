///////////////////////////////////////////////////////////////////////
// $Id: ZBuffer-scene.cpp 1054 2007-09-14 04:54:58Z gherron $
//
// Scene definition for the z-buffer scanline project.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "scenelib.h"


void CreateScene(Scene& scene)
{
	scene.Clear();

	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.25, 0.25, 0.25));
	scene.AddLight(Point3D(1,2,3), Color(0.7, 0.7, 0.7));

 	scene.modeling.Push();
 	scene.SetColor(HSVColor(0.3, 1.0, 0.8), HSVColor(0.0, 1.0, 0.0), 0);
 	scene.modeling.Translate(-0.5,0,0);
 	CreateRectangularPrism(scene, 0.5,0.5,1);
 	scene.modeling.Pop();

 	scene.modeling.Push();
 	scene.SetColor(HSVColor(0.0, 1.0, 0.9), HSVColor(0.0, 1.0, 0.0), 0);
 	scene.modeling.Translate(0.5, 0, 0);
 	CreateCylinder(scene, 0.25, 1, 12);
 	scene.modeling.Pop();

 	scene.modeling.Push();
 	scene.modeling.Translate(0,0.5,-0.5);
 	scene.modeling.RotateX(90);
 	scene.SetColor(HSVColor(0.5, 1.0, 0.8), HSVColor(0.0, 1.0, 0.0), 0);
 	CreateLetterA(scene);
 	scene.modeling.Pop();

	scene.modeling.Push();
	scene.modeling.Translate(0,0,-0.5);
	scene.SetColor(HSVColor(0.0, 0.0, 0.5), HSVColor(0.0, 1.0, 0.0), 0);
	CreateRect(scene, 2, 4);
	scene.modeling.Pop();
}
