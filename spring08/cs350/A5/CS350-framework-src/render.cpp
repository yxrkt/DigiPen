///////////////////////////////////////////////////////////////////////
// $Id: render.cpp 1210M 2008-02-06 07:54:38Z (local) $
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright Å© 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "common.h"

// globals
Node *g_tree;
unsigned g_depth = 0;
unsigned g_nodes = 0;

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
	PolygonList polys;
  size_t nObjects = scene.objects.size();
  for ( size_t i = 0; i < nObjects; ++i )
  {
    Object &obj = scene.objects[i];
    size_t nPolys = scene.objects[i].polygons.size();
    for ( size_t j = 0; j < nPolys; ++j )
    {
      APolygon &poly = obj.polygons[j]; std::vector<Vertex> temp( poly );
      APolygon safePoly = RemoveDupeVerts( poly );
      if ( safePoly.size() > 2 )
        polys.push_back( Polygon3D( safePoly, obj.Kd ) );
    }
  }

  MakeBSPT( polys, g_tree );
  g_depth = GetHeight( g_tree ) - 1;
  GetNodes( g_tree, g_nodes );
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene( Scene &scene, int width, int height )
{
	// Choose OpenGL or student rendering here.	 The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	// if (useOpenGLRendering || useFastRendering)	{
	if (scene.UseOpenGLRendering)
  {
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
	}

	// Student rendering code goes here
	else
  {
		// BSPTree project must disable depth test, other projects
		// may need to enable it.
    // BSPTree project must disable depth test, other projects
    // may need to enable it.
    glDisable(GL_DEPTH_TEST);

    // Communicate matrices to OpenGL
    float v[16], m[16];
    scene.viewing.ArrayForOpenGL(v);
    scene.projection.ArrayForOpenGL(m);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(v);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);

    // Set some lighting parameters for OpenGL
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    for (unsigned int li=0;  li<scene.lights.size();  li++) {
      Point3D& P3 = scene.lights[li].position;
      Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
      glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
      glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
      glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
      glEnable(GL_LIGHT0+li); }

    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    Point3D eye = scene.viewing.InverseTransform(Vector4D(0,0,0,1)).Hdiv();
    DrawBSPT( g_tree, eye );

    std::stringstream msg;
    msg << "Rendered by Alex Serio; Polys = " << g_nodes << "; Tree Depth = " << g_depth;
    DrawMessage( (char *)msg.str().c_str(), width, height );
	}
}
