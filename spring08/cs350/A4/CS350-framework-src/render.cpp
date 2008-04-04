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
#include <sstream>

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
  size_t nObjs = scene.objects.size();
  for ( size_t i = 0; i < nObjs; ++i )
  {
    Object &obj = scene.objects[i];
    obj.occluder = ( i == 0 );
    obj.occluded = false;
    obj.bound.origin = obj.polygons[0][0].V.Hdiv();
    obj.bound.extent = obj.polygons[0][0].V.Hdiv();
    size_t nPolys = obj.polygons.size();
    for ( size_t j = 0; j < nPolys; ++j )
    {
      APolygon &poly = obj.polygons[j];
      size_t nVerts = poly.size();
      for ( size_t k = 0; k < nVerts; ++k )
      {
        for ( size_t v = 0; v < 3; ++v )
        {
          if ( poly[k].V.Hdiv()[v] < obj.bound.origin[v] )
            obj.bound.origin[v] = poly[k].V.Hdiv()[v];
          else if ( poly[k].V.Hdiv()[v] > obj.bound.extent[v] )
            obj.bound.extent[v] = poly[k].V.Hdiv()[v];
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene( Scene &scene, int width, int height )
{
	// Choose OpenGL or student rendering here.	 The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	if (scene.UseOpenGLRendering)
  {
    BeginDefaultDraw( width, height );
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
	}
  else
  {
    static std::stringstream cmsg;
    cmsg.str( "" );
    
    Scene sceneNew = scene;
    cmsg << "Occluded Objects: " << RemoveOccludedObjects( sceneNew, width, height );

    glEnable( GL_DEPTH_TEST );
    BeginDraw( scene, width, height );
    size_t nObjs = sceneNew.objects.size();
    for ( size_t i = 0; i < nObjs; ++i )
      DrawObject( sceneNew.objects[i], !sceneNew.objects[i].occluder );
    glDisable( GL_DEPTH_TEST );
    DrawMessage( (char *)cmsg.str().c_str(), width, height );

    ERRCHECK( "end (uncaught)" );
  }
}
