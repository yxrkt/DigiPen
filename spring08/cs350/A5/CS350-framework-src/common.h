//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------

#include <list>

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"


// =============================================================================
// Added during A2
// =============================================================================
const float EPSILON = .0001f;

template<typename T, typename U>
inline T force_cast( const U &data ) { return *(T *)&data; }

inline float TVal( const Point3D &p, const Point3D &q, const Point3D &i )
{
  return abs( ( i - p ).length() / ( q - p ).length() );
}

inline float TVal( const Vertex &p, const Vertex &q, const Point3D &i )
{
  return TVal( force_cast<Point3D>( p.V ), force_cast<Point3D>( q.V ), i );
}

// Polygon
class Polygon3D
{
  public:
    Polygon3D( const APolygon &poly, const Color &_color ) : verts( poly ), color( _color ), center()
    {
      size_t nVerts = verts.size();
      for ( size_t i = 0; i < nVerts; ++i ) center += force_cast<Vector3D>( verts[i].V );
      center /= (float)nVerts;
      Vector3D u( force_cast<Point3D>( verts[1].V ) - force_cast<Point3D>( verts[0].V ) );
      Vector3D v( force_cast<Point3D>( verts[2].V ) - force_cast<Point3D>( verts[1].V ) );
      Vector3D n( ( u ^ v ).normalized() );
      if ( n * verts[0].N < 0 ) n *= -1.f;
      plane = Plane3D( n[0], n[1], n[2], -( n[0] * verts[0].V[0] + n[1] * verts[0].V[1] + n[2] * verts[0].V[2] ) );
    }

    inline Plane3D &Plane() { return plane; }
    inline const Plane3D &Plane() const { return plane; }
    inline APolygon &Verts() { return verts; }
    inline const APolygon &Verts() const { return verts; }
    inline Color &ColorRgba() { return color; }
    inline const Color &ColorRgba() const { return color; }
    inline Vector3D &Center() { return center; }
    inline const Vector3D &Center() const { return center; }

  private:
    Plane3D   plane;
    APolygon  verts;
    Color     color;
    Vector3D  center;
};

// Node
struct Node
{
  Node( Polygon3D *_root = NULL )
    : root( _root )
    , front( NULL )
    , back( NULL ) {}
  ~Node()
  {
    if ( back ) this->~Node();
    if ( root ) delete root;
    if ( front ) this->~Node();
  }

  Polygon3D  *root;
  Node       *front;
  Node       *back;
};


// typedefs
typedef std::list<Polygon3D>    PolygonList;
typedef PolygonList::iterator   PolygonListIt;

// Prototypes
Polygon3D PullOutRoot( PolygonList &polys );
void SplitPolygon( const Polygon3D &poly, Node *tree, PolygonList &front, PolygonList &back );
void MakeBSPT( PolygonList polys, Node *&tree );
void DrawBSPT( Node *tree, const Point3D &eye );
void DrawPoly( const Polygon3D &poly );
unsigned GetHeight( const Node *tree );
void GetNodes( const Node *tree, unsigned &nPolys );
APolygon RemoveDupeVerts( const APolygon &poly );


#endif
