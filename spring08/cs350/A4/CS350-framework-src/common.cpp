#include "common.h"

HOM BuildHOM( Scene &scene )
{
  return HOM( ):
}

unsigned RemoveOccludedObjects( Scene &scene )
{
  BuildHOM( objs );

  return 0;
}

void PrepareToDraw( Scene &scene )
{
  // Communicate matrices to OpenGL
  float v[16], m[16];
  scene.viewing.ArrayForOpenGL( v );
  scene.projection.ArrayForOpenGL( m );

  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

  glMatrixMode( GL_MODELVIEW );
  glLoadMatrixf( v );

  glMatrixMode( GL_PROJECTION );
  glLoadMatrixf( m );

  // Set some lighting parameters for OpenGL
  glEnable( GL_LIGHTING );
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba );
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
  for ( unsigned int i = 0; i < scene.lights.size(); ++i )
  {
    Point3D &p3 = scene.lights[i].position;
    Vector4D p4(p3[0], p3[1], p3[2], 0 ); // OpenGL needs a 4 vector
    glLightfv( GL_LIGHT0 + i, GL_POSITION, p4.begin() );
    glLightfv( GL_LIGHT0 + i, GL_DIFFUSE,  scene.lights[i].color.rgba );
    glLightfv( GL_LIGHT0 + i, GL_SPECULAR, scene.lights[i].color.rgba );
    glEnable(  GL_LIGHT0 + i );
  }

  glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
}

void DrawObjects( ObjectVec &objs )
{
  size_t nObjs = objs.size();
  for ( size_t i = 0; i < nObjs; ++i )
  {
    glPolygonMode( GL_FRONT_AND_BACK, objs[i].occluder ? GL_LINE : GL_FILL );
    size_t nPolys = objs[i].polygons.size();
    for ( size_t j = 0; j < nPolys; ++j )
    {
      APolygon &poly = objs[i].polygons[j];
      glNormal3fv( force_cast<float *>( &poly[0].N ) );
      glColor3fv( objs[i].Kd.rgba );
      glBegin( GL_POLYGON );
      size_t nVerts = poly.size();
      for ( size_t k = 0; k < nVerts; ++k )
        glVertex3fv( force_cast<float *>( &poly[k].V ) );
      glEnd();
    }
  }
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
