///////////////////////////////////////////////////////////////////////
// $Id: scene.cpp 1177 2008-01-21 19:24:49Z gherron $
//
// Scene definition for the Phong lighting project.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

int sceneID = 1;

void CreateScene(Scene& scene)
{
	scene.Clear();

	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.2, 0.2, 0.2));
	scene.AddLight(Point3D(100,0,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(0,100,80), Color(0.5, 0.5, 0.5));

	scene.modeling.Push();
	scene.SetColor(HSVColor(0.00, 1.0, 0.7), HSVColor(0.0, 0.0, 0.8), 30);
	scene.modeling.Translate( 0.5, -0.5, 0);
	CreateSphere(scene, 0.5, 12);
	scene.modeling.Pop();

	scene.modeling.Push();
	scene.SetColor(HSVColor(0.15, 1.0, 0.9), HSVColor(0.0, 0.0, 0.8), 120);
	scene.modeling.Translate(-0.5, -0.5, 0);
	scene.modeling.RotateZ(-45);
	scene.modeling.RotateX(90);
	CreateCylinder(scene, 0.2, 1.0, 12);
	scene.modeling.Pop();

	scene.modeling.Push();
	scene.SetColor(HSVColor(0.66, 1.0, 0.9), HSVColor(0.0, 0.0, 1.5), 120);
	scene.modeling.Translate(-0.5,  0.5, 0);
	CreateRectangularPrism(scene, 0.75, 0.75, 0.75);
	scene.modeling.Pop();

	scene.modeling.Push();
	scene.SetColor(HSVColor(0.33, 1.0, 0.7), HSVColor(0.0, 0.0, 0.8), 30);
	scene.modeling.Translate( 0.5, 0.5, 0);
	CreateCone(scene, 0.5, 1.0, 12);
	scene.modeling.Pop();
}
