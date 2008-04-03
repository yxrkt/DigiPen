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

inline float randf()
{
 return ( (float)( rand() % 101 ) / 100.f );
}

void CreateScene(Scene& scene)
{
	scene.Clear();

	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.2, 0.2, 0.2));
	scene.AddLight(Point3D(100,0,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(0,100,80), Color(0.5, 0.5, 0.5));

  // load occluder
  scene.modeling.Push();
  scene.SetColor(HSVColor(randf(), randf(), randf()), HSVColor(0.f, 0.f, .5f), rand() % 120);
  scene.modeling.Translate(-2.75f, 0.f, 0.f);
  CreateRectangularPrism(scene, 5.0, .25, 1.0);
  scene.modeling.Pop();

  // load objects that can be occluded
  double scale = .25;
  int nRows = 10, nCols = 10;
  for ( int y = 0; y < nRows; ++y )
  {
    for ( int x = 0; x < nCols; ++x )
    {
      scene.modeling.Push();
      scene.SetColor(HSVColor(randf(), randf(), randf()), HSVColor(0.f, 0.f, .5f), rand() % 120);
      scene.modeling.Translate((float)(x - nCols) / 2.f, (float)(y - nRows) / 2.f, 0.f);
      CreateRectangularPrism(scene, scale, scale, scale);
      scene.modeling.Pop();
    }
  }
}
