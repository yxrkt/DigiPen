//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------

#include <list>

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"


// constants
const float EPSILON = .0001f;
const unsigned CB   = 1;
const unsigned CI   = 5;

// useful inlines
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

// Triangle
class TriangleKd
{
  public:
    TriangleKd( const APolygon &poly, const Object &obj ) : verts( poly ), parentObj( (Object *)&obj ), temp( poly ) {}
    inline const APolygon &Verts() const { return verts; }
    inline const Object *ParentObj() const { return parentObj; }
  private:
    Object   *parentObj;
    APolygon  verts, temp;
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
typedef std::list<Polygon3D>        PolygonList;
typedef PolygonList::iterator       PolygonListIt;
typedef std::vector<TriangleKd>     TriangleVec;
typedef TriangleVec::iterator       TriangleVecIt;
typedef TriangleVec::const_iterator TriangleVecItC;

enum PLANE_TYPE { PLANE_X = 0, PLANE_Y, PLANE_Z };

// Node for Kd tree
struct NodeKd
{
  NodeKd() : left( NULL ), right( NULL ) {}
  NodeKd( const Box3D &_vox, float _divVal, PLANE_TYPE _divType, NodeKd *_left = NULL, NodeKd *_right = NULL )
    : vox( _vox ), divVal( _divVal ), divType( _divType ), left( _left ), right( _right ) {}
  NodeKd( const TriangleVec &_tris, const Box3D &_vox, float _divVal, PLANE_TYPE _divType )
    : tris( _tris ), vox( _vox ), divVal( _divVal ), divType( _divType ), left( NULL ), right( NULL ) {}

  float         divVal;
  PLANE_TYPE    divType;
  Box3D         vox;
  NodeKd       *left;
  NodeKd       *right;
  TriangleVec   tris;
};

// Functors and other stl helpers
template <PLANE_TYPE T>
inline bool VertMin( const Vertex &lhs, const Vertex &rhs )
{
  if ( T == PLANE_X ) return ( lhs.V[0] < rhs.V[0] );
  if ( T == PLANE_Y ) return ( lhs.V[1] < rhs.V[1] );
  return ( lhs.V[2] < rhs.V[2] );
}

template <PLANE_TYPE T>
inline bool VertMax( const Vertex &lhs, const Vertex &rhs )
{
  if ( T == PLANE_X ) return ( lhs.V[0] > rhs.V[0] );
  if ( T == PLANE_Y ) return ( lhs.V[1] > rhs.V[1] );
  return ( lhs.V[2] > rhs.V[2] );
}


// inlines
inline bool OnFace( const Point3D &p, const Box3D &box )
{
  return ( p[0] == box.origin[0] || p[0] == box.extent[0] ||
           p[1] == box.origin[1] || p[1] == box.extent[1] ||
           p[2] == box.origin[2] || p[2] == box.extent[2] );
}

// Prototypes
Polygon3D PullOutRoot( PolygonList &polys );
void SplitPolygon( const Polygon3D &poly, Node *tree, PolygonList &front, PolygonList &back );
void MakeBSPT( PolygonList polys, Node *&tree );
void DrawBSPT( Node *tree, const Point3D &eye );
void DrawPoly( const Polygon3D &poly );
unsigned GetHeight( const Node *tree );
void GetNodes( const Node *tree, unsigned &nPolys );
APolygon RemoveDupeVerts( const APolygon &poly );

// Added during A3
TriangleKd *FindTriangle( const Ray3D &view, NodeKd *node, unsigned &tests );
Box3D GetAABB( const TriangleVec &tris );
float Cost( float val, PLANE_TYPE planeType, const Box3D &vox, const TriangleVec &tris );
void BuildTriVecs( const TriangleVec &tris, const Box3D &voxL, 
                   const Box3D &voxR, TriangleVec &trisL, TriangleVec &trisR );
float GetPlaneVal( const TriangleVec &tris, const Box3D &vox, unsigned depth, 
                   Box3D &voxL, Box3D &voxR, float &cost );
NodeKd *MakeKdTree( const TriangleVec &tris, const Box3D &vox, unsigned depth );


#endif
