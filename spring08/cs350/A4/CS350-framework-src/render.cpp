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
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene( Scene &scene, int width, int height )
{
	// Choose OpenGL or student rendering here.	 The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	if (scene.UseOpenGLRendering)
  {
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
    cmsg << "Occluded Objects: " << RemoveOccludedObjects( sceneNew.objects );
    glEnable( GL_DEPTH_TEST );
    DrawSceneWithOpenGL( sceneNew, width, height );
    glDisable( GL_DEPTH_TEST );
    DrawMessage( (char *)cmsg.str().c_str(), width, height );
  }
}
