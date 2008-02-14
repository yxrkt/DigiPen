///////////////////////////////////////////////////////////////////////
// $Id: Reflection-scene.cpp 969 2007-05-29 20:58:59Z gherron $
//
// Scene definition for the reflection mapping project.
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

	scene.EnableLighting = false;
	scene.EnableFlatShading = false;

	ReflTexture* refl = new ReflTexture();

	scene.SetAmbient(Color(0.4, 0.4, 0.4));
	scene.AddLight(Point3D(0,-100,80), Color(0.8, 0.8, 0.8));

	scene.SetTexture(NULL, NULL, refl);
	scene.modeling.Push();
	scene.SetColor(HSVColor(0.66, 1.0, 0.9), HSVColor(0.0, 0.0, 1.5), 120);
	Reflector(scene, 12*4, 12*4);
	scene.modeling.Pop();
}
