#include "common.h"

// Select and remove root
Polygon3D PullOutRoot( PolygonList &polys )
{
  int score = 0x7FFFFFFF, checks = 0;
  PolygonListIt best;

  for ( PolygonListIt i = polys.begin(); i != polys.end(); ++i )
  {
    int curScore = 0;
    for ( PolygonListIt j = polys.begin(); j != polys.end(); ++j )
      if ( j != i ) curScore += ( i->Plane().Evaluate( force_cast<Point3D>( j->Center() ) ) > 0 ) ? 1 : -1;
    if ( abs( curScore ) < score )
    {
      score = abs( curScore );
      best  = i;
    }
  }

  Polygon3D divisor = *best;
  polys.erase( best );
  return divisor;
}

// Split a polygon
void SplitPolygon( const Polygon3D &poly, Node *tree, PolygonList &front, PolygonList &back )
{
  Point3D p3DIsect[2];
  size_t  nVerts = poly.Verts().size(), isectIndex[2][2], j = 0;
  for ( size_t i = 0; i < nVerts; ++i )
  {
    size_t iPrev = poly.Verts()[i].prevIndex;
    Segment3D seg( force_cast<Point3D>( poly.Verts()[iPrev].V ), force_cast<Point3D>( poly.Verts()[i].V ) );
    if ( Intersects( seg, tree->root->Plane(), &p3DIsect[j] ) )
    {
      isectIndex[j][0] = iPrev;
      isectIndex[j][1] = i;
      if ( ++j == 2 ) break;
    }
  }

  if ( j == 1 )
  {
    if ( tree->root->Plane().Evaluate( force_cast<Point3D>( poly.Center() ) ) > 0.f ) front.push_back( poly );
    else back.push_back( poly );
    return;
  }

  size_t prev = 1;
  APolygon poly1, poly2;
  Vertex vtxIsect1, vtxIsect2;
  //float t;

  vtxIsect2.V = Vector4D( p3DIsect[1] );
  //t = TVal( poly.Verts()[isectIndex[1][0]], poly.Verts()[isectIndex[1][1]], p3DIsect[1] );
  //vtxIsect2.N = t * ( poly.Verts()[isectIndex[1][0]].N + poly.Verts()[isectIndex[1][1]].N );
  //vtxIsect2.u = t * ( poly.Verts()[isectIndex[1][0]].u + poly.Verts()[isectIndex[1][1]].u );
  //vtxIsect2.v = t * ( poly.Verts()[isectIndex[1][0]].v + poly.Verts()[isectIndex[1][1]].v );
  vtxIsect2.N = Vector3D( poly.Verts()[0].N );

  vtxIsect1.V = Vector4D( p3DIsect[0] );
  //t = TVal( poly.Verts()[isectIndex[0][0]], poly.Verts()[isectIndex[0][1]], p3DIsect[0] );
  //vtxIsect1.N = t * ( poly.Verts()[isectIndex[0][0]].N + poly.Verts()[isectIndex[0][1]].N );
  //vtxIsect1.u = t * ( poly.Verts()[isectIndex[0][0]].u + poly.Verts()[isectIndex[0][1]].u );
  //vtxIsect1.v = t * ( poly.Verts()[isectIndex[0][0]].v + poly.Verts()[isectIndex[0][1]].v );
  vtxIsect1.N = Vector3D( poly.Verts()[0].N );

  // create poly 1
  poly1.push_back( vtxIsect2 );
  vtxIsect1.prevIndex = 0;
  poly1.push_back( vtxIsect1 );

  for ( size_t i = isectIndex[0][1]; i != isectIndex[1][1]; ( i != nVerts - 1 ) ? ++i : i = 0 )
  {
    poly1.push_back( poly.Verts()[i] );
    poly1.back().prevIndex = prev++;
  }
  poly1.front().prevIndex = prev;

  // create poly 2
  poly2.push_back( vtxIsect1 );
  vtxIsect2.prevIndex = 0;
  poly2.push_back( vtxIsect2 );

  prev = 1;
  for ( size_t i = isectIndex[1][1]; i != isectIndex[0][1]; ( i != nVerts - 1 ) ? ++i : i = 0 )
  {
    poly2.push_back( poly.Verts()[i] );
    poly2.back().prevIndex = prev++;
  }
  poly2.front().prevIndex = prev;

  // put polys in tree
  bool flipped = ( tree->root->Plane().Evaluate( force_cast<Point3D>( poly1[2].V ) ) < 0.f );

  Polygon3D poly1_3D( poly1, poly.ColorRgba() ), poly2_3D( poly2, poly.ColorRgba() );

  poly2_3D.Plane() = poly1_3D.Plane();

  front.push_back( !flipped ? poly1_3D : poly2_3D );
  back.push_back( !flipped ? poly2_3D : poly1_3D );
}

// Recursively builds BSP tree
void MakeBSPT( PolygonList polys, Node *&tree )
{
  PolygonList front, back;

  if ( polys.empty() ) { tree = NULL; return; }

  tree = new Node( new Polygon3D( PullOutRoot( polys ) ) );

  for ( PolygonListIt i = polys.begin(); i != polys.end(); ++i ) // for each poly
  {
    // check which side each vertex is on
    size_t sumFront = 0, sumBack = 0, nVerts = i->Verts().size();
    for ( size_t j = 0; j < nVerts; ++j ) // for each vertex
    {
      float dist = tree->root->Plane().Evaluate( force_cast<Point3D>( i->Verts()[j].V ) );
      if ( dist > -EPSILON ) sumFront++;
      if ( dist <  EPSILON ) sumBack++;
    }

    if ( sumFront == nVerts ) front.push_back( *i );    // poly contained in FRONT space
    else if ( sumBack == nVerts ) back.push_back( *i ); // poly contained in BACK space
    else SplitPolygon( *i, tree, front, back );         // poly needs to be split
  }

  MakeBSPT( front, tree->front );
  MakeBSPT( back, tree->back );
}

// Draw polys in tree
void DrawBSPT( Node *tree, const Point3D &eye )
{
  if ( tree->root->Plane().Evaluate( eye ) > 0.f )
  {
    if ( tree->back ) DrawBSPT( tree->back, eye );
    DrawPoly( *tree->root );
    if ( tree->front ) DrawBSPT( tree->front, eye );
  }
  else
  {
    if ( tree->front ) DrawBSPT( tree->front, eye );
    DrawPoly( *tree->root );
    if ( tree->back ) DrawBSPT( tree->back, eye );
  }
}

// Draw a polygon
void DrawPoly( const Polygon3D &poly )  // poly has a color, normal, and list of vertices
{
  glNormal3fv( force_cast<float *>( &poly.Verts()[0].N ) );
  glColor3fv( poly.ColorRgba().rgba );
  glBegin( GL_POLYGON );
  unsigned nVerts = (unsigned)poly.Verts().size();
  for ( unsigned i = 0; i < nVerts; ++i )
    glVertex3fv( force_cast<float *>( &poly.Verts()[i].V ) );
  glEnd();
}

// Get Height + 1 of tree
unsigned GetHeight( const Node *tree )
{
  unsigned bheight = 1, fheight = 1;

  if ( tree == NULL ) return 0;
  if ( tree->back == NULL && tree->front == NULL ) return 1;

  bheight += GetHeight( tree->back );
  fheight += GetHeight( tree->front );

  return std::max( bheight, fheight );
}

// Get number of nodes or polys
void GetNodes( const Node *tree, unsigned &nPolys )
{
  if ( tree->back ) GetNodes( tree->back, nPolys );
  nPolys++;
  if ( tree->front) GetNodes( tree->front, nPolys );
}

APolygon RemoveDupeVerts( const APolygon &poly )
{
  int nVerts = (int)poly.size(), j = -1;
  APolygon newPoly;
  for ( int i = 0; i < nVerts; ++i )
  {
    if ( std::find( newPoly.begin(), newPoly.end(), poly[i] ) == newPoly.end() )
    {
      newPoly.push_back( poly[i] );
      newPoly.back().prevIndex = j++;
    }
  }
  newPoly.front().prevIndex = (int)newPoly.size() - 1;
  return newPoly;
}

bool operator ==( const Vertex &lhs, const Vertex &rhs )
{
  if ( abs( lhs.V[0] - rhs.V[0] ) > EPSILON ) return false;
  if ( abs( lhs.V[1] - rhs.V[1] ) > EPSILON ) return false;
  if ( abs( lhs.V[2] - rhs.V[2] ) > EPSILON ) return false;
  return true;
}
