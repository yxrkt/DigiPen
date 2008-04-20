//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------

#include <list>

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

// Thanks microsoft
#undef near
#undef far
#undef NEAR
#undef FAR

// Used by frustum
inline void MakePlane( const Point3D &a, const Point3D &b, const Point3D &c, Plane3D &plane )
{
  Vector3D n( ( ( b - a ) ^ ( c - b ) ).normalized() );
  plane[0] = n[0];
  plane[1] = n[1];
  plane[2] = n[2];
  plane[3] = -( n[0] * a[0] + n[1] * a[1] + n[2] * a[2] );
}

// Types
enum OBJ_STATE { INSIDE, INTERSECTING, OUTSIDE };

class Frustum
{
  public:
    Frustum( Scene &scene )
    {
      // Get points of frustum in world space
      Point3D points[2][2][2];
      for ( int x = 0; x < 2; ++x )
      {
        for ( int y = 0; y < 2; ++y )
        {
          for ( int z = 0; z < 2; ++z )
          {
            Vector4D ndc( x == 0 ? -1.f : 1.f, y == 0 ? -1.f : 1.f, z == 0 ? -1.f : 1.f, 1.f );
            points[x][y][z] = scene.viewing.InverseTransform( scene.projection.InverseTransform( ndc ) ).Hdiv();
          }
        }
      }

      MakePlane( points[0][1][1], points[0][1][0], points[0][0][0], planes[LEFT]   );
      MakePlane( points[1][0][0], points[1][1][0], points[1][1][1], planes[RIGHT]  );
      MakePlane( points[0][0][0], points[0][1][0], points[1][1][0], planes[NEAR]   );
      MakePlane( points[1][1][1], points[0][1][1], points[0][0][1], planes[FAR]    );
      MakePlane( points[0][0][0], points[1][0][0], points[1][0][1], planes[BOTTOM] );
      MakePlane( points[1][1][1], points[1][1][0], points[0][1][0], planes[TOP]    );
    }

    OBJ_STATE Contains( const Object &obj, unsigned &tries, unsigned &triesAlt ) const
    {
      float result;

      for ( unsigned i = 0; i < NPLANES; ++i )
      {
        int cur = ( i + obj.start ) % NPLANES;
        Point3D min( obj.aabb.origin ), max( obj.aabb.extent );
        for ( unsigned j = 0; j < 3; ++j )
        {
          if ( planes[cur][j] < 0 )
          {
            min[j] = obj.aabb.extent[j];
            max[j] = obj.aabb.origin[j];
          }
        }

        result = planes[cur].Evaluate( max ); tries++;
        if ( result < 0.f )
        {
          triesAlt = ( i + obj.start ) * 2 + 1;
          obj.start = cur;
          return OUTSIDE;
        }

        result = planes[cur].Evaluate( min ); tries++;
        if ( result < 0.f )
        {
          triesAlt = ( i + obj.start ) * 2 + 2;
          obj.start = cur;
          return INTERSECTING;
        }
      }

      triesAlt = tries;
      return INSIDE;
    }

    enum { LEFT, RIGHT, NEAR, FAR, BOTTOM, TOP, NPLANES };
    Plane3D planes[NPLANES];
};

typedef std::vector< Object >       ObjectVec;
typedef ObjectVec::iterator         ObjectVecIt;
typedef ObjectVec::const_iterator   ObjectVecItC;
typedef std::vector< APolygon >     APolygonVec;
typedef APolygonVec::iterator       APolygonVecIt;
typedef APolygonVec::const_iterator APolygonVecItC;
typedef APolygon::iterator          VertexIt;
typedef APolygon::const_iterator    VertexItC;

// Constants
const float EPSILON = .0001f;

// Inlines
template<typename T, typename U>
inline T &force_cast( const U &data ) { return *(T *)&data; }

// Prototypes
void GenerateAABB( Object &obj );
void BeginDraw( Scene &scene, int width, int height );
void BeginDefaultDraw( int width, int height );
void DrawObject( const Object &obj );


#endif
