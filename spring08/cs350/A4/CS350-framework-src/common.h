//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------

#include <list>

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

// classes and structs
struct HOM
{
  HOM( int _width = 0, int _height = 0 )
    : width( _width ), height( _height ), map( NULL ), prev( NULL ), next( NULL ) {}

  int     width;
  int     height;
  float  *map;
  HOM    *prev;
  HOM    *next;
};

// typedefs
typedef std::vector< Object > ObjectVec;
typedef ObjectVec::iterator ObjectVecIt;

// useful inlines
template< typename T, typename U >
inline T force_cast( const U &data ) { return *(T *)&data; }

inline float TVal( const Point3D &p, const Point3D &q, const Point3D &i )
{
  return abs( ( i - p ).length() / ( q - p ).length() );
}

inline float TVal( const Vertex &p, const Vertex &q, const Point3D &i )
{
  return TVal( force_cast<Point3D>( p.V ), force_cast<Point3D>( q.V ), i );
}

// other forward decls
HOM BuildHOM( ObjectVec &objs, int width, int height );
unsigned RemoveOccludedObjects( ObjectVec &objs, int width, int height );
void PrepareToDraw( Scene &scene );
void DrawObjects( ObjectVec &objs );

#endif
