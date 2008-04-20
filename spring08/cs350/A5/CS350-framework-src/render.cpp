///////////////////////////////////////////////////////////////////////
// $Id: render.cpp 1210M 2008-02-06 07:54:38Z (local) $
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "common.h"

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
  for ( ObjectVecIt i = scene.objects.begin(); i != scene.objects.end(); ++i )
  {
    GenerateAABB( *i );
    i->start = Frustum::LEFT;
  }
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene( Scene &scene, int width, int height )
{
	// Choose OpenGL or student rendering here.	 The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	// if (useOpenGLRendering || useFastRendering)	{
	if ( scene.UseOpenGLRendering )
  {
    BeginDefaultDraw( width, height );
		DrawSceneWithOpenGL( scene, width, height );
		DrawMessage( "Rendered by OpenGL (toggle with 'g')", width, height );
	}

	// Student rendering code goes here
	else
  {
    Frustum frust( scene );

    unsigned nIsects = 0, nCulled = 0, nTotalTests = 0, nTotalTestsAlt = 0;
    BeginDraw( scene, width, height );
    for ( ObjectVecItC i = scene.objects.begin(); i != scene.objects.end(); ++i )
    {
      unsigned nTests = 0, nTestsAlt = 0;
      OBJ_STATE state = frust.Contains( *i, nTests, nTestsAlt );
      switch ( state )
      {
        case INSIDE:
          break;
        case INTERSECTING:
          nIsects++;
          break;
        case OUTSIDE:
          nCulled++;
          break;
      }

      nTotalTests    += nTests;
      nTotalTestsAlt += nTestsAlt;

      if ( state != OUTSIDE )
        DrawObject( *i );
    }

    size_t nObjects = scene.objects.size();
    static std::stringstream report;
    report.str( "" );
    report << "Out: " << nCulled << ", Isect: " << nIsects<< ", Tests w/: " << (float)nTotalTests / (float)nObjects
           << ", Tests w/o: " << (float)nTotalTestsAlt / (float)nObjects;
    DrawMessage( const_cast<char *>( report.str().c_str() ), width, height );
	}
}
