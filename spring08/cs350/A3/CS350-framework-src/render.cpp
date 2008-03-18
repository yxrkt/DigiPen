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
NodeKd   *g_treeKd;
unsigned  g_depth = 0;
unsigned  g_nodes = 0;

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
  TriangleVec tris;

  // convert all polys to triangles
  size_t nObjects = scene.objects.size();
  for ( size_t i = 0; i < nObjects; ++i )
  {
    std::vector<APolygon> triangles;
    Object &obj = scene.objects[i];
    size_t nPolys = obj.polygons.size();
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
  // make starting bounding box
  //Box3D vox( tris.front()
  for ( TriangleVecIt i = tris.begin(); i != tris.end(); ++i )
  {
    //if (i->Verts())
  }
  //g_treeKd = MakeKdTree( tris,  );
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
    for ( int y = 0; y < height; ++y )
    {
      for ( int x = 0; x < width; ++x )
      {
        float distSqr = 0xFFFFFFFF;

        Vector4D S = Vector4D((float)(x + .5f) * 2.f / (float)width - 1.f, 
                              (float)(y + .5f)  * 2.f / (float)height - 1.f, 0.f);
        Point3D W  = scene.viewing.InverseTransform(scene.projection.InverseTransform(S)).Hdiv();
        Vector3D V = W - eye;
        
        Ray3D ray( W, V );

        size_t nObjs = scene.objects.size();
        /*for ( size_t i = 0; i < nObjs; ++i )*/
        for ( size_t i = 2; i < 3; ++i )
        {
          Object &obj = scene.objects[i];
          size_t nTris = obj.polygons.size();
          for ( size_t j = 0; j < nTris; ++j )
          {
            Point3D isect;
            APolygon &tri = obj.polygons[j];
            if ( Intersects( ray, Triangle3D( force_cast<Point3D>( tri[0].V ), 
                                              force_cast<Point3D>( tri[1].V ), 
                                              force_cast<Point3D>( tri[2].V ) ), &isect ) )
            {
              Vector3D d( isect - eye );
              float newDistSqr = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
              if ( newDistSqr < distSqr )
              {
                distSqr = newDistSqr;

                glBegin( GL_POINTS );
                glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba );
                glMateriali( GL_FRONT, GL_SHININESS, obj.n );
                glMaterialfv( GL_FRONT, GL_SPECULAR, obj.Ks.rgba );
                glNormal3fv( force_cast<float *>( &tri[0].N ) );
                glVertex2i( x, y );
                glEnd();
              }
            }
          }
        }
      }
      glutSwapBuffers();
    }
	}
}
