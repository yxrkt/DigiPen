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

	scene.SetAmbient(Color(0.3, 0.3, 0.3));
	scene.AddLight(Point3D(100,0,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(-100,0,80), Color(0.5, 0.5, 0.5));

	int n = 5;
	float w = 3.0/(4*n);
	int count = 0;

	for (int i=0;  i<=2*n;  i++) {
		for (int j=0;  j<=2*n;  j++) {
			float xpos = (i-n)/float(n) + .01*i/10.0 + .03*j/10.0;
			float ypos = (j-n)/float(n) + .02*i/10.0 + .01*j/10.0;
			float zpos =                  .03*i/10.0 + .02*j/10.0;

			float hue = (atan2f(ypos,xpos)+PI)/(2*PI);
			float sat = max(abs(xpos), abs(ypos));
			
			scene.modeling.Push();
			scene.SetColor(HSVColor(hue,sat,0.9), HSVColor(0.0,0.0,1.5), 120);
			scene.modeling.Translate(xpos, ypos, 0);
			CreateRectangularPrism(scene, w, w, 0.3-sat/4.);
			scene.modeling.Pop();
			count++;
		}
	}
	printf("Count: %d\n", count*6);

}
