///////////////////////////////////////////////////////////////////////
// $Id: geomlib-advanced.cpp 1182 2008-01-21 22:50:54Z gherron $
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "common.h"
#include <vector>
#include <queue>

const double epsilon = 1e-3;

const float M_PI = 3.14159265f;

struct TimedPoint
{
  bool operator ==( const TimedPoint &rhs ) { return t == rhs.t; }

  float t;
  Point3D p;
};

bool TimedPointLess( const TimedPoint &lhs, const TimedPoint &rhs )
{
  return ( lhs.t < rhs.t );
}

typedef std::queue<TimedPoint> PointQueue;

// dot product for Vector3D
float operator *( const Vector3D &lhs, const Vector3D &rhs )
{
  return ( ( lhs[0] * rhs[0] ) + ( lhs[1] * rhs[1] ) + ( lhs[2] * rhs[2] ) );
}

// cross product for Vector3D
Vector3D operator ^( const Vector3D &lhs, const Vector3D &rhs )
{
  float x = lhs[1] * rhs[2] - lhs[2] * rhs[1];
  float y = lhs[2] * rhs[0] - lhs[0] * rhs[2];
  float z = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  
  return Vector3D( x, y, z );
}

// dot product for Vector2D
float operator *( const Vector2D &lhs, const Vector2D &rhs )
{
  return ( lhs[0] * rhs[0] + lhs[1] * rhs[1] );
}

// line vs line (or line segment)
bool vsLine2D(const Line2D& line1, const Line2D& line2, Point2D *rpoint = NULL, bool check = true)
{
  // check for parallel
  float mag1 = abs( line1.vector * line2.vector );
  float mag2 = abs( line1.vector.length() * line2.vector.length() );
  if ( abs( mag1 - mag2 ) < epsilon )
    return false;

  // get dist for each point
  Vector2D n( Vector2D( -line1.vector[1], line1.vector[0] ).normalized() );

  float first  = ( line2.point - line1.point ) * n;
  float second = ( ( line2.point + line2.vector ) - line1.point ) * n;

  // find t
  float t;

  if ( ( first > 0.f && second > 0.f ) || ( first < 0.f && second < 0.f ) )
  {
    if ( check ) return false;

    t = abs( first ) < abs( second )
        ? -abs( first / ( first - second ) )
        : abs( ( first + second ) / ( first - second ) );
  }

  else
  {
    t = abs( first ) / ( abs( first ) + abs( second ) );
  }

  if ( rpoint ) *rpoint = line2.point + t * line2.vector;

  return true;
}

// line (or segment) in 3D vs plane
bool vsLine3D(const Line3D& line, const Plane3D& plane, Point3D *rpoint = NULL, bool check = true)
{
  // check if line is parallel with plane
  if ( abs( line.vector * plane.normal() ) < epsilon )
    return false;

  Point3D p  = line.point, q = line.point + line.vector;
  float normalLen = plane.normal().length();

  // get dist (and side) from plane for each point
  float distP = plane.Evaluate( p ) / normalLen;
  float distQ = plane.Evaluate( q ) / normalLen;

  // find t
  float t;

  if ( ( distP < 0.f && distQ < 0.f ) || ( distP > 0.f && distQ > 0.f ) )
  {
    if ( check ) return false;

    t = abs( distP ) < abs( distQ )
        ? -abs( distP / ( distP - distQ ) )
        : abs( ( distP + distQ ) / ( distP - distQ ) );
  }

  else
  {
    t = abs( distP ) / ( abs( distP ) + abs( distQ ) );
  }

  if ( rpoint ) *rpoint = p + t * line.vector;

  return true;
}

// Add intersections to queue
void AddIsects( const Ray3D &ray, const Box3D &box, PointQueue &isects )
{
  const unsigned nPairs = 3;
  Point3D rayEnd( ray.origin + ray.direction );

  for ( unsigned i = 0; i < nPairs; ++i )
  {
    unsigned j = ( i + 1 ) % nPairs, k = ( i + 2 ) % nPairs;

    if ( rayEnd[i] != ray.origin[i] )
    {
      for ( unsigned m = 0; m < 2; ++m)
      {
        float t = (!m) ? ( box.origin[i] - ray.origin[i] ) / ( ray.direction[i] )
                       : ( box.extent[i] - ray.origin[i] ) / ( ray.direction[i] );

        if ( t >= -epsilon && t <= 1.f + epsilon )
        {
          Point3D isect = ray.origin + t * ray.direction;
          if ( ( isect[j] >= box.origin[j] && isect[j] <= box.extent[j] ) &&
               ( isect[k] >= box.origin[k] && isect[k] <= box.extent[k] ) )
          {
            TimedPoint point = { t, isect };
            if ( isects.c.end() == std::find( isects.c.begin(), isects.c.end(), point ) )
              isects.push( point );
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////
// Distance methods
////////////////////////////////////////////////////////////////////////

// Return the distance from a point to a line.
float Distance(const Point3D& point, const Line3D& line)
{
  Vector3D u = point - line.point;
  Vector3D normal = u ^ line.vector;

  try
  {
    normal = ( line.vector ^ normal ).normalized();
  }

  catch ( ZeroDivide )
  {
    return 0;
  }

  return ( u * normal );
}

// Return the distance from a point to a plane.
float Distance(const Point3D& point, const Plane3D& plane)
{
  return abs( plane.Evaluate( point ) / plane.normal().length() );
}

////////////////////////////////////////////////////////////////////////
// Containment methods
////////////////////////////////////////////////////////////////////////

// Determines if point (known to be on a line) is contained within a segment.
bool Segment3D::contains(const Point3D& point) const
{
  Vector3D seg( point2 - point1 );
  Vector3D ptSeg( point - point1 );

  if ( seg * ptSeg < -epsilon )
    return false;

  if ( ptSeg.length() > seg.length() + epsilon )
    return false;

  return true;
}

// Determines if point (known to be on a line) is contained within a ray.
bool Ray3D::contains(const Point3D& point, float *t) const
{
  Vector3D u( point - origin );

  if ( u * direction < -epsilon )
  {
    if ( t ) *t = -u.length() / direction.length();
    return false;
  }

  if ( u.length() > direction.length() + epsilon )
  {
    if ( t ) *t = u.length() / direction.length();
    return false;
  }

  if ( t ) *t = u.length() / direction.length();
  return true;
}

// Determines if point is contained within a box.
bool Box3D::contains(const Point3D& point) const
{
  if ( point[0] >= origin[0] && point[1] >= origin[1] && point[2] >= origin[2] )
  {
    if ( point[0] <= extent[0] && point[1] <= extent[1] && point[2] <= extent[2] )
      return true;
  }

  return false;
}

// Determines if point (known to be on a plane) is contained within a triangle.
bool Triangle3D::contains(const Point3D& point) const
{
  Vector3D n( ( points[1] - points[0] ) ^ ( points[2] - points[1] ) );

    // check if in plane
  if ( abs( n * ( point - points[0] ) ) > epsilon )
    return false;

  for ( unsigned i = 0; i < 3; ++i )
  {
    unsigned next = ( ( i == 2 ) ? 0 : i + 1 );

    Vector3D u = point - points[i];
    Vector3D v = points[next] - point;

    if ( ( u ^ v ) * n > 0.f )
      return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////
// Intersects functions
// In the following Intersects function these rules apply:
//
// * Most test are to determine if a *unique* solution exists. (Or in
//   some cases up to two intersection points exist.)  Parallel
//   objects have either zero or infinitely many solutions and so
//   return false.
//
// * If a unique solution exists, a function value of true is
//   returned.  (Or in the cases where several solutions can exist,
//   the number of intersection points is returned.)
//
// * If a unique solution does exist, the calling program may provide
//   a memory location into which the intersection point can be
//   returned.  Such pointer may be NULL to indicate that the
//   intersection point is not to be returned.
//
////////////////////////////////////////////////////////////////////////

// Determines if 2D segments have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Segment2D& seg1, const Segment2D& seg2, Point2D *rpoint)
{
  Vector2D u = seg1.point2 - seg1.point1;
  Vector2D v = seg2.point2 - seg2.point1;

  if ( !vsLine2D( Line2D( seg1.point1, u ), Line2D( seg2.point1, v ) ) )
    return false;

  if ( !vsLine2D( Line2D( seg2.point1, v ), Line2D( seg1.point1, u ), rpoint ) )
    return false;

  return true;
}

// Determines if 2D lines have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Line2D& line1, const Line2D& line2, Point2D *rpoint)
{
  return vsLine2D( line1, line2, rpoint, false );
}

// Determines if 3D line and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Line3D& line, const Plane3D& plane, Point3D *rpoint)
{
	return vsLine3D( line, plane, rpoint, false );
}

// Determines if 3D segment and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Plane3D& plane, Point3D *rpoint)
{
  return vsLine3D( Line3D( seg.point1, seg.point2 - seg.point1 ), plane, rpoint );
}

// Determines if 3D segment and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Triangle3D& tri, Point3D *rpoint)
{
  Vector3D n = ( tri[1] - tri[0] ) ^ ( tri[2] - tri[1] );

  float a = n[0], b = n[1], c = n[2];
  float d = -( a * tri[0][0] + b * tri[0][1] + c * tri[0][2] );

  if ( !Intersects( seg, Plane3D( a, b, c, d ), rpoint ) )
    return false;

  return tri.contains( *rpoint );
}

// Determines if 3D ray and sphere have 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Sphere3D& sphere, std::pair<Point3D, Point3D> *rpoints)
{
  // find pts of intersection of line
  float dist = Distance( sphere.center, Line3D( ray.origin, ray.direction ) );
  Vector3D u = sphere.center - ray.origin;
  float uLen = u.length();
  
  // trivial rejection 1
  if ( dist > sphere.radius + epsilon ) return 0;

  // trivial rejection 2
  if ( ray.direction * u < 0.f && uLen > sphere.radius ) return 0;

  // line has one intersection
  if ( dist >= sphere.radius - epsilon )
  {
    float t = sqrt( dist * dist - sphere.radius * sphere.radius ) / ray.direction.length();

    if ( t <= 1.f )
    {
      if ( rpoints ) rpoints->first = ray.origin + t * ray.direction;
      return 1;
    }

    return 0;
  }

  // line has two intersections (assuming ray origin not in sphere)
  float dist2   = dist * dist;
  float rayLen  = ray.direction.length();
  float m       = sqrt( uLen * uLen - dist2 );
  float h       = sqrt( sphere.radius * sphere.radius - dist2 );
  float t1      = ( m - h ) / rayLen;
  float t2      = ( m + h ) / rayLen;

  if ( t1 <= 1.f )
  {
    if ( rpoints ) rpoints->first = ray.origin + t1 * ray.direction;

    if ( t2 <= 1.f )
    {
      if ( rpoints ) rpoints->second = ray.origin + t2 * ray.direction;
      return 2;
    }

    return 1;
  }

  return 0;
}

// Determines if 3D ray and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Ray3D& ray, const Triangle3D& tri, Point3D *rpoint)
{
  Vector3D n( Vector3D( ( tri[1] - tri[0] ) ^ ( tri[2] - tri[1] ) ).normalized() );

  // check if coplanar
  if ( abs( n * ray.direction ) < epsilon ) return false;

  // check if ray is facing away from plane
  if ( ( tri[0] - ray.origin ) * ray.direction < 0.f ) return false;

  float d = -( n[0] * tri[0][0] + n[1] * tri[0][1] + n[2] * tri[0][2] );
  Point3D isect;

  //// if point intersects plane
  //if ( Intersects( Line3D( ray.origin, ray.direction ), Plane3D( n[0], n[1], n[2], d ), &isect ) )
  //{
  //  if ( tri.contains( isect ) )
  //  {
  //    if ( rpoint ) *rpoint = isect;
  //    return true;
  //  }
  //}

  //return false;


//=========================================================================================

  Plane3D plane(n[0], n[1], n[2], d);

  float den = ray.direction * plane.normal();
  float t = -(force_cast<Vector3D>(ray.origin) * plane.normal() + plane[3]) / den;

  if (t >= 0)
  {
    isect = ray.origin + t * ray.direction;
    bool contained = tri.contains(isect);
    if (rpoint) *rpoint = isect;
    return contained;
  }

  return false;
}

// Determines if 3D ray and AABB have a 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Box3D& box, std::pair<Point3D, Point3D> *rpoints)
{
  PointQueue isects;
  AddIsects( ray, box, isects );

  std::sort( isects.c.begin(), isects.c.end(), TimedPointLess );

  size_t nIsects = isects.size();

  if ( rpoints )
  {
    for ( size_t i = 0; i < nIsects; ++i )
    {
      if ( i == 0 ) rpoints->first = isects.c[i].p;
      else          rpoints->second = isects.c[i].p;
    }      
  }

  return nIsects;
}

// Determines if 3D triangles intersect.  
// If parallel, returns false. (This may be considered misleading.)
// If true and rpoint is not NULL, returns two edge/triangle intersections.
int Intersects(const Triangle3D& tri1, const Triangle3D& tri2, std::pair<Point3D, Point3D> *rpoints)
{
  // check if coplanar
  if ( abs( ( ( tri1[1] - tri1[0] ) ^ ( tri1[2] - tri1[1] ) ) * ( tri2[1] - tri2[0] ) ) > epsilon )
    return 0;

  std::vector<Point3D> points;

  // test first tri's edges against second tri
  for ( unsigned i = 0; i < 3; ++i )
  {
    unsigned j = ( i == 2 ) ? 0 : i + 1;
    Point3D isect;

    if ( Intersects( Segment3D( tri1[i], tri1[j] ), tri2, &isect ) )
      points.push_back( isect );
  }

  // test second tri's edges against first tri
  for ( unsigned i = 0; i < 3; ++i )
  {
    unsigned j = ( i == 2 ) ? 0 : i + 1;
    Point3D isect;

    if ( Intersects( Segment3D( tri2[i], tri2[j] ), tri1, &isect ) )
      points.push_back( isect );
  }

  if ( rpoints && !points.empty() )
  {
    rpoints->first  = points[0];
    rpoints->second = points[1];
  }

  return points.size();
}

////////////////////////////////////////////////////////////////////////
// Geometric relationships
////////////////////////////////////////////////////////////////////////

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line1, const Line3D& line2)
{
  return acos( line1.vector * line2.vector ) / ( line1.vector.length() * line2.vector.length() );
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line, const Plane3D& plane)
{
  Vector3D u = line.vector ^ plane.normal();
  Vector3D v = plane.normal() ^ u;

  float angle = acos( v * line.vector );

  return ( ( angle < 0.f ) ? ( angle + 2.f * M_PI ) : angle );
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Plane3D& plane1, const Plane3D& plane2)
{
  return acos( ( plane1.normal() * plane2.normal() ) / 
               ( plane1.normal().length() * plane2.normal().length() ) );
}

// Determine if two lines are coplanar
bool Coplanar(const Line3D& line1, const Line3D& line2)
{
  Vector3D n1 = ( line1.vector ^ ( line2.point - line1.point ) );
  Vector3D n2 = ( line1.vector ^ ( ( line2.point + line2.vector ) - line1.point ) );

  return ( abs( n1 * n2 - n1.length() * n2.length() ) <= epsilon );
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line1, const Line3D& line2)
{
  return ( abs( line1.vector * line2.vector - line1.vector.length() * line1.vector.length() ) <= epsilon );
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line, const Plane3D& plane)
{
  return ( abs( line.vector * plane.normal() ) <= epsilon );
}

// Determine if two geometric entities are parallel.
bool Parallel(const Plane3D& plane1, const Plane3D& plane2)
{
  return ( abs( plane1.normal() * plane2.normal() - 
                plane1.normal().length() * plane2.normal().length() ) <= epsilon );
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line1, const Line3D& line2)
{
  return ( abs( line1.vector * line2.vector ) <= epsilon );
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line, const Plane3D& plane)
{
  return ( abs( line.vector * plane.normal() - 
                line.vector.length() * plane.normal().length() ) <= epsilon );
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2)
{
  return ( plane1.normal() * plane2.normal() <= epsilon );
}
