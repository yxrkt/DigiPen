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
const float OPACITY_THRESHOLD       = .975f;
const float TRANSPARENCY_THRESHOLD  = .80f;

// classes and structs
struct HOM
{
  HOM( int _width = 0, int _height = 0, HOM *_prev = NULL )
    : width( _width )
    , height( _height )
    , map( new float[width * height] )
    , depth( new float[width * height] )
    , prev( _prev )
    , next( NULL ) {}

  ~HOM()
  {
    if ( this->next != NULL )
      delete this->next;
    delete [] map;
    delete [] depth;
  }

  inline float &Map( int x, int y )
  {
    return map[x + y * width];
  }

  inline float &Depth( int x, int y )
  {
    return depth[x + y * width];
  }
  const int width;
  const int height;

  float  *map;
  float  *depth;
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
inline Point3D GetScreenCrds( const Point3D &world, Scene &scene, int width, int height )
{
  Point3D ndc = ( scene.projection.Transform( scene.viewing.Transform( Vector4D( world ) ) ).Hdiv() );
  return Point3D( ( ndc[0] + 1.f ) * (float)( width / 2 ), ( ndc[1] + 1.f ) * (float)( height / 2 ), ndc[2] );
}

// other forward decls
HOM *BuildHOM( Scene &scene, int width, int height );
unsigned RemoveOccludedObjects( Scene &scene, int width, int height );
void BeginDraw( Scene &scene, int width, int height );
void DrawObject( Object &obj, bool filled = true );
void BeginDefaultDraw( int width, int height );
bool Occluded( const Box2D &bound, float depth, const HOM *hom, 
               int scnWidth, int scnHeight, bool autoFind = true );
float GetMaxDepth( const Object &obj, Scene &scene, int width, int height );

#endif
