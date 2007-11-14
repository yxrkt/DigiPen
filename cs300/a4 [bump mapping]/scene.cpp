///////////////////////////////////////////////////////////////////////
// $Id: Bump-scene.cpp 969 2007-05-29 20:58:59Z gherron $
//
// Scene definition for the bump mapping project.
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

	TileTexture* tile = new TileTexture(256,256);
	TileBump* bump = new TileBump(256,256);

	scene.SetAmbient(Color(0.4, 0.4, 0.4));
	scene.AddLight(Point3D(0,-100,80), Color(0.8, 0.8, 0.8));

	scene.SetTexture(tile, bump);
	scene.modeling.Push();
	scene.SetColor(HSVColor(0.083, 0.75, 0.8), HSVColor(0.0, 0.0, 1.0), 100);
	CreateRect(scene, 1.0, 1.0);
	scene.modeling.Pop();
}
