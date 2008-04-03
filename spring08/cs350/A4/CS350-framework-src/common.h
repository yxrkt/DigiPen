//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------

#include <list>

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

#define ERRCHECK(str)\
  while ( true )\
  {\
    GLenum err = glGetError();\
		if ( err == GL_NO_ERROR ) break;\
		printf( "ERROR: 0x%x at %s\n", err, str );\
  }

// constants
const float OPACITY_THRESHOLD       = .95f;
const float TRANSPARENCY_THRESHOLD  = .50f;

// classes and structs
struct HOM
{
  HOM( int _width = 0, int _height = 0, HOM *_prev = NULL )
    : width( _width ), height( _height ), map( new float[width * height] ), prev( _prev ), next( NULL ) {}

  ~HOM()
  {
    if ( this->next != NULL )
      delete this->next;
    delete [] map;
  }

  inline float &Map( int x, int y )
  {
    return map[x + y * width];
  }

  const int width;
  const int height;

  float  *map;
  HOM    *prev;
  HOM    *next;
};

struct Box2D
{
  inline float Area() const
  {
    return ( ( extent[0] - origin[0] ) * ( extent[1] - origin[1] ) );
  }

  Point2D origin;
  Point2D extent;
};

// typedefs
typedef std::vector< Object > ObjectVec;
typedef ObjectVec::iterator ObjectVecIt;

// useful inlines
template< typename T, typename U >
inline T force_cast( const U &data ) { return *(T *)&data; }
inline Point2D GetScreenCrds( const Point3D &world, Scene &scene )
{
  return force_cast< Point2D >( scene.projection.Transform( 
    scene.viewing.Transform( Vector4D( world ) ) ).Hdiv() );
}

inline float TVal( const Point3D &p, const Point3D &q, const Point3D &i )
{
  return abs( ( i - p ).length() / ( q - p ).length() );
}

inline float TVal( const Vertex &p, const Vertex &q, const Point3D &i )
{
  return TVal( force_cast<Point3D>( p.V ), force_cast<Point3D>( q.V ), i );
}

// other forward decls
HOM *BuildHOM( Scene &scene, int width, int height );
unsigned RemoveOccludedObjects( Scene &scene, int width, int height );
void BeginDraw( Scene &scene, int width, int height );
void DrawObject( Object &obj, bool filled = true );
void BeginDefaultDraw( int width, int height );
bool Occluded( const Box2D &bound, const HOM *hom, int scnWidth, int scnHeight, bool autoFind = true );


#endif
