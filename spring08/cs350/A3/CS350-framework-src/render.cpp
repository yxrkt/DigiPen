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
//Node *g_tree;
unsigned g_depth = 0;
unsigned g_nodes = 0;

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
    // convert all polys to triangles
  size_t nObjects = scene.objects.size();
  for ( size_t i = 0; i < nObjects; ++i )
  {
    std::vector<APolygon> triangles;
    Object &obj = scene.objects[i];
    size_t nPolys = obj.polygons.size();
    for ( size_t j = 0; j < nPolys; ++j )
    {
      APolygon poly = obj.polygons[j];
      size_t nVerts = poly.size();
      for ( size_t k = 1; k < nVerts; ++k )
      {
        APolygon triangle(3);
        triangle[0] = poly[0];
        triangle[1] = poly[k];
        triangle[2] = poly[(k + 1 == nVerts) ? 0 : k + 1];
        triangle[0].prevIndex = 2;
        triangle[1].prevIndex = 0;
        triangle[2].prevIndex = 1;
        triangles.push_back( triangle );
      }
    }
    obj.polygons = triangles;
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
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
	}

	// Student rendering code goes here
	else
  {
    GLdouble proj[16];
    GLdouble model[16];
    GLint    view[4];

    glGetDoublev( GL_PROJECTION_MATRIX, proj );
    glGetDoublev( GL_MODELVIEW_MATRIX, model );
    glGetIntegerv( GL_VIEWPORT, view );

    // for each pixel
    for ( int y = 0; y < height; ++y )
    {
      for ( int x = 0; x < width; ++x )
      {
        Point3D wp;

        Vector4D S = Vector4D((float) x * 2.f / (float) width - 1.0, (float) y * 2.f / (float) height - 1.0, z);
        Vector4D T = scene.projection.InverseTransform(S);
        Point3D eye = scene.viewing.InverseTransform( Vector4D( 0.0, 0.0, 0.0, 1.0 ) ).Hdiv();
        Vector3D V( Vector3D( eye - scene.viewing.InverseTransform(T).Hdiv() ) );

        gluUnProject( (GLdouble)x, (GLdouble)y, 0.0, model, proj, view, (GLdouble *)&wp[0], (GLdouble *)&wp[1], (GLdouble *)&wp[2] );
        Ray3D ray( wp, view );

        size_t nObjs = scene.objects.size();
        for ( size_t i = 0; i < nObjs; ++i )
        {
          Object &obj = scene.objects[i];
          size_t nTris = obj.size();
          for ( size_t j = 0; j < nTris; ++j )
          {
            APolygon &poly = obj.polygons[j];
            if ( Intersects( ray, Triangle3D( force_cast<Point3D>(poly[0].V ), force_cast<Point3D>(poly[1].V ), V ) ) )
            {
              float black[] = { 0.f, 0.f, 0.f };
              glColor3fv( black );
              glVertex2i( x, y );
            } 
          }
        }
      }
    }
	}
}
