///////////////////////////////////////////////////////////////////////
// $Id: Phong-scene.cpp 969 2007-05-29 20:58:59Z gherron $
//
// Scene definition for the Phong lighting project.
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
	scene.EnableFlatShading = false;

	float r = 0.5;
	float count = 12;

	scene.SetAmbient(Color(0.2, 0.2, 0.2));
	scene.AddLight(Point3D(100,0,30), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(0,100,80), Color(0.5, 0.5, 0.5));

	// Red octant: Moderate specular, moderate shininess
	scene.modeling.Push();
	scene.SetColor(HSVColor(0.00, 1.0, 0.7), HSVColor(0.0, 0.0, 0.8), 30);
	scene.modeling.Translate( 0.5, -0.5, 0);
	CreateSphereOctant(scene, r, count);
	scene.modeling.Pop();

	// Blue octant: High specular, high shininess
	scene.modeling.Push();
	scene.SetColor(HSVColor(0.66, 1.0, 0.9), HSVColor(0.0, 0.0, 1.5), 120);
	scene.modeling.Translate(-0.5,  0.5, 0);
	CreateSphereOctant(scene, r, count);
	scene.modeling.Pop();

	// Green octant:  No specular
	scene.modeling.Push();
	scene.SetColor(HSVColor(0.33, 1.0, 0.7), HSVColor(0.0, 0.0, 0.0), 0);
	scene.modeling.Translate(-0.5, -0.5, 0);
	CreateSphereOctant(scene, r, count);
	scene.modeling.Pop();
}
