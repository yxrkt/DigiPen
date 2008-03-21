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

// globals
NodeKd   *g_treeKd;
unsigned  g_depth = 0;
unsigned  g_nodes = 0;
TriangleVec g_tris;

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
  TriangleVec tris;

  // convert all polys to triangles
  size_t nObjects = scene.objects.size();
  //for ( size_t i = 0; i < nObjects; ++i )
  for ( size_t i = 0; i < 1; ++i )
  {
    std::vector<APolygon> triangles;
    Object &obj = scene.objects[i];
    size_t nPolys = obj.polygons.size();
    nPolys = 10;
    for ( size_t j = 0; j < nPolys; ++j )
    {
      APolygon poly = RemoveDupeVerts( obj.polygons[j] );
      size_t nVerts = poly.size();
      if ( nVerts < 3 ) continue;
      for ( size_t k = 1; k + 1 < nVerts; ++k )
      {
        APolygon triangle(3);
        triangle[0] = poly[0];
        triangle[1] = poly[k];
        triangle[2] = poly[k + 1];
        triangle[0].prevIndex = 2;
        triangle[1].prevIndex = 0;
        triangle[2].prevIndex = 1;
        triangles.push_back( triangle );
        tris.push_back( TriangleKd( triangle, obj ) );
      }
    }
    obj.polygons = triangles;
  }

  // build kd tree
  g_treeKd = MakeKdTree( tris, GetAABB( tris ), 0 );
  g_tris = tris;
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
		//DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
	}

	// Student rendering code goes here
  else
  {
    scene.EnableLighting ? glEnable( GL_LIGHTING ) : glDisable( GL_LIGHTING );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
    for ( unsigned i = 0;  i < scene.lights.size(); ++i )
    {
      Point3D& P3 = scene.lights[i].position;
      Vector4D P4( P3[0], P3[1], P3[2], 0 ); // OpenGL needs a 4 vector
      glLightfv( GL_LIGHT0 + i, GL_POSITION, P4.begin());
      glLightfv( GL_LIGHT0 + i, GL_DIFFUSE,  scene.lights[i].color.rgba );
      glLightfv( GL_LIGHT0 + i, GL_SPECULAR, scene.lights[i].color.rgba );
      glEnable( GL_LIGHT0 + i);
    }

    Point3D eye = scene.viewing.InverseTransform( Vector4D( 0.0, 0.0, 0.0, 1.0 ) ).Hdiv();

    // for each pixel
    unsigned tests = 0;
    for ( int y = 0; y < height; ++y )
    {
      for ( int x = 0; x < width; ++x )
      {
        float distSqr = (float)INFINITE;

        Vector4D S = Vector4D((float)(x + .5f) * 2.f / (float)width - 1.f, 
                              (float)(y + .5f)  * 2.f / (float)height - 1.f, 0.f);
        Point3D W  = scene.viewing.InverseTransform(scene.projection.InverseTransform(S)).Hdiv();
        
        Ray3D ray( W, W - eye );

        TriangleKd *tri = FindTriangle( ray, g_treeKd, tests );

        if ( tri != NULL )
        {
          glBegin( GL_POINTS );
          glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tri->ParentObj()->Kd.rgba );
          glMateriali( GL_FRONT, GL_SHININESS, tri->ParentObj()->n );
          glMaterialfv( GL_FRONT, GL_SPECULAR, tri->ParentObj()->Ks.rgba );
          glNormal3fv( force_cast<float *>( &tri->Verts()[0].N ) );
          glVertex2i( x, y );
          glEnd();
        }
      }
      //glutSwapBuffers();
    }

    float testRatio = (float)tests / (float)( width * height * g_tris.size() );
    std::stringstream ss;
    ss << testRatio;
    DrawMessage((char *)ss.str().c_str(), width, height);
	}
}
