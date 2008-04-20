#include "common.h"


void GenerateAABB( Object &obj )
{
  obj.aabb.origin = obj.polygons[0][0].V.Hdiv();
  obj.aabb.extent = obj.polygons[0][0].V.Hdiv();
  for ( APolygonVecIt i = obj.polygons.begin(); i != obj.polygons.end(); ++i )
  {
    for ( VertexIt j = i->begin(); j != i->end(); ++j )
    {
      for ( size_t v = 0; v < 3; ++v )
      {
        if ( force_cast<Point3D>( j->V )[v] < obj.aabb.origin[v] )
          obj.aabb.origin[v] = force_cast<Point3D>( j->V )[v];
        else if ( force_cast<Point3D>( j->V )[v] > obj.aabb.extent[v] )
          obj.aabb.extent[v] = force_cast<Point3D>( j->V )[v];
      }
    }
  }
}

void BeginDraw( Scene &scene, int width, int height )
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

  glClearColor( 1.f, 1.f, 1.f, 1.f );
  glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
  glViewport( 0, 0, width, height );
}

void BeginDefaultDraw( int width, int height )
{
  glDisable( GL_COLOR_MATERIAL );
  glViewport( 0, 0, width, height );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable( GL_LIGHTING );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.f, 1.f, 1.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT );
}



void DrawObject( const Object &obj )
{
  for ( APolygonVecItC i = obj.polygons.begin(); i != obj.polygons.end(); ++i )
  {
    glNormal3fv( force_cast<float *>( &(*i)[0].N ) );
    glColor3fv( obj.Kd.rgba );
    glBegin( GL_POLYGON );
    size_t nVerts = i->size();
    for ( size_t j = 0; j < nVerts; ++j )
      glVertex3fv( force_cast<float *>( &(*i)[j].V ) );
    glEnd();
  }
}
