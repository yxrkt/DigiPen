#include "common.h"

HOM *BuildHOM( Scene &scene, int width, int height )
{
  HOM *hom = new HOM( width / 10, height / 10 );

  // get level 0 map
  glViewport( 0, 0, hom->width, hom->height );
  glColor3f( 1.f, 1.f, 1.f );
  glDisable( GL_LIGHTING );
  glDisable( GL_DEPTH_TEST );
  glClearColor( 0.f, 0.f, 0.f, 0.f );
  glClear( GL_COLOR_BUFFER_BIT );

  glBegin( GL_POLYGON );
  BeginDraw( scene, width, height );
  size_t nObjs = scene.objects.size();
  for ( size_t i = 0; i < nObjs; ++i )
  {
    if ( scene.objects[i].occluder )
      DrawObject( scene.objects[i] );
  }
  glEnd();

  glReadPixels( 0, 0, hom->width, hom->height, GL_RED, GL_FLOAT, hom->map );

  HOM *pCurHOM = hom;
  // build remaining maps
  while ( pCurHOM->width > 4 && pCurHOM->height > 4 )
  {
    pCurHOM->next = new HOM( pCurHOM->width / 2, pCurHOM->height / 2, pCurHOM );
    pCurHOM = pCurHOM->next;
    for ( int y = 0; y < pCurHOM->height; ++y )
    {
      int j = y * 2;
      for ( int x = 0; x < pCurHOM->width; ++x )
      {
        int i = x * 2;
        pCurHOM->Map( x, y ) = ( pCurHOM->prev->Map( i, j ) + pCurHOM->prev->Map( i + 1, j ) 
                               + pCurHOM->prev->Map( i, j + 1 ) + pCurHOM->prev->Map( i + 1, j + 1 ) ) / 4.f;
      }
    }
  }

  return hom;
}

unsigned RemoveOccludedObjects( Scene &scene, int width, int height )
{
  unsigned nOccluded = 0;

  HOM *head = BuildHOM( scene, width, height );

  size_t nObjs = scene.objects.size();
  std::vector< Box2D > projs( nObjs );
  for ( size_t i = 0; i < nObjs; ++i )
  {
    if ( !scene.objects[i].occluder )
    {
      // get bounding box from bounding volume
      std::vector< Point2D > projVerts( 8 );
      Object &obj  = scene.objects[i];
      Box3D &bound = obj.bound;

      projVerts[0] = GetScreenCrds( bound.origin, scene );
      projVerts[1] = GetScreenCrds( Point3D( bound.origin[0], bound.origin[1], bound.extent[2] ), scene );
      projVerts[2] = GetScreenCrds( Point3D( bound.origin[0], bound.extent[1], bound.extent[2] ), scene );
      projVerts[3] = GetScreenCrds( Point3D( bound.extent[0], bound.origin[1], bound.extent[2] ), scene );
      projVerts[4] = GetScreenCrds( Point3D( bound.extent[0], bound.extent[1], bound.origin[2] ), scene );
      projVerts[5] = GetScreenCrds( Point3D( bound.extent[0], bound.origin[1], bound.origin[2] ), scene );
      projVerts[6] = GetScreenCrds( Point3D( bound.origin[0], bound.extent[1], bound.origin[2] ), scene );
      projVerts[7] = GetScreenCrds( bound.extent, scene );

      projs[i].origin = projVerts[0];
      projs[i].extent = projVerts[7];

      for ( size_t j = 0; j < 8; ++j )
      {
        if ( projVerts[j][0] < projs[i].origin[0] )
          projs[i].origin[0] = projVerts[j][0];
        if ( projVerts[j][1] < projs[i].origin[1] )
          projs[i].origin[1] = projVerts[j][1];
      }

      obj.occluded = Occluded( projs[i], head, width, height );
      if ( obj.occluded )
        nOccluded++;
    }
  }

  delete head;

  return nOccluded;
}

void BeginDraw( Scene &scene, int width, int height )
{
  // Communicate matrices to OpenGL
  float v[16], m[16];
  scene.viewing.ArrayForOpenGL( v );
  scene.projection.ArrayForOpenGL( m );

  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

  glClearColor( 1.f, 1.f, 1.f, 1.f );

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
  glViewport( 0, 0, width, height );
}

void DrawObject( Object &obj, bool filled )
{
  if ( obj.occluded ) return;

  glPolygonMode( GL_FRONT_AND_BACK, filled ? GL_FILL : GL_LINE );
  size_t nPolys = obj.polygons.size();
  for ( size_t i = 0; i < nPolys; ++i )
  {
    Color col = ( obj.occluder && filled ) ? Color( 1.f, 1.f, 1.f ) : obj.Kd;
    APolygon &poly = obj.polygons[i];
    glNormal3fv( force_cast<float *>( &poly[0].N ) );
    glColor3fv( col.rgba );
    glBegin( GL_POLYGON );
    size_t nVerts = poly.size();
    for ( size_t j = 0; j < nVerts; ++j )
      glVertex3fv( force_cast<float *>( &poly[j].V ) );
    glEnd();
  }

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void BeginDefaultDraw( int width, int height )
{
  glViewport( 0, 0, width, height );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable( GL_LIGHTING );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1, 1, 1, 1 );
  glClear( GL_COLOR_BUFFER_BIT );
}

bool Occluded( const Box2D &bound, const HOM *hom, int scnWidth, int scnHeight, bool autoFind )
{
  // initiating case
  if ( autoFind )
  {
    // find best starting level
    float area    = bound.Area();
    float bestDif = (float)INFINITE;
    HOM *curLevel = const_cast<HOM *>( hom ); 
    while ( curLevel != NULL )
    {
      int pixArea  = ( ( scnWidth / curLevel->width ) * ( scnHeight / curLevel->height ) );
      float curDif = abs( (float)pixArea - area );
      if ( curDif < bestDif )
      {
        hom     = curLevel;
        bestDif = curDif;
      }
      curLevel = curLevel->prev;
    }
  }

  // test each pixel
  int nOpaques = 0;
  int yMin = (int)( bound.origin[1] + .5f ), yMax = (int)( bound.extent[1] + .5f );
  int xMin = (int)( bound.origin[0] + .5f ), xMax = (int)( bound.extent[0] + .5f );
  for ( int y = yMin; y < yMax; ++y )
  {
    for ( int x = xMin; x < xMax; ++x )
    {
      if ( const_cast<HOM *>( hom )->Map( x, y ) < TRANSPARENCY_THRESHOLD )
        return false;
      if ( const_cast<HOM *>( hom )->Map( x, y ) > OPACITY_THRESHOLD )
        nOpaques++;
    }
  }

  int iArea = ( ( xMax - xMin ) * ( yMax - yMin ) );
  if ( nOpaques < iArea )
  {
    if ( hom->prev != NULL )
      return Occluded( bound, hom->prev, scnWidth, scnHeight, false );
    return false;
  }
  return true;
}
