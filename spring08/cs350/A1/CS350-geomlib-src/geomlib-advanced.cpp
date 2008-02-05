///////////////////////////////////////////////////////////////////////
// $Id: geomlib-advanced.cpp 1182 2008-01-21 22:50:54Z gherron $
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include <vector>

const double epsilon = 1e-3;

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
  if ( n * ( point - points[0] ) != 0.f )
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
bool Intersects(const Segment2D& seg1, const Segment2D& seg2,
				Point2D *rpoint)
{
  Vector2D u = seg1.point2 - seg1.point1;
  Vector2D v = seg2.point2 - seg2.point1;

  if ( !Intersects( Line2D( seg1.point1, u ), Line2D( seg2.point1, v ) ) )
    return false;

  if ( !Intersects( Line2D( seg2.point1, v ), Line2D( seg1.point1, u ), rpoint ) )
    return false;
  
  return true;
}

// Determines if 2D lines have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Line2D& line1, const Line2D& line2,
				Point2D *rpoint)
{
  // check for parallel
  float mag1 = abs( line1.vector * line2.vector );
  float mag2 = abs( line1.vector.length() * line2.vector.length() );
  if ( abs( mag1 - mag2 ) < epsilon )
    return false;

  Vector2D n( Vector2D( -line1.vector[1], line1.vector[0] ).normalized() );

  float first  = ( line2.point - line1.point ) * n;
  float second = ( ( line2.point + line2.vector ) - line1.point ) * n;

  if ( ( first > 0.f && second > 0.f ) || ( first < 0.f && second < 0.f ) )
    return false;

  first = abs( first );
  second = abs( second );

  float t = ( first <= second )
            ? first / ( first + second )
            : 1.f - second / ( first + second );

  if ( rpoint ) *rpoint = line2.point + t * line2.vector;

  return true;
}

// Determines if 3D line and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Line3D& line, const Plane3D& plane, Point3D *rpoint)
{
  // check if line is parallel with plane
  if ( abs( line.vector * plane.normal() ) < epsilon )
    return false;

	throw Unimplemented();
}

// Determines if 3D segment and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Plane3D& plane, Point3D *rpoint)
{
	throw Unimplemented();
}

// Determines if 3D segment and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Triangle3D& tri, Point3D *rpoint)
{
	throw Unimplemented();
}

// Determines if 3D ray and sphere have 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Sphere3D& sphere,
			   std::pair<Point3D, Point3D> *rpoints)
{
	throw Unimplemented();
}

// Determines if 3D ray and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Ray3D& ray, const Triangle3D& tri, Point3D *rpoint)
{
	throw Unimplemented();
}

// Determines if 3D ray and AABB have a 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Box3D& box,
			   std::pair<Point3D, Point3D> *rpoints)
{
	throw Unimplemented();
}

// Determines if 3D triangles intersect.  
// If parallel, returns false. (This may be considered misleading.)
// If true and rpoint is not NULL, returns two edge/triangle intersections.
int Intersects(const Triangle3D& tri1, const Triangle3D& tri2,
		   std::pair<Point3D, Point3D> *rpoints)
{
	throw Unimplemented();
}

////////////////////////////////////////////////////////////////////////
// Geometric relationships
////////////////////////////////////////////////////////////////////////

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line1, const Line3D& line2)
{
	throw Unimplemented();
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line, const Plane3D& plane)
{
	throw Unimplemented();
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Plane3D& plane1, const Plane3D& plane2)
{
	throw Unimplemented();
}

// Determine if two lines are coplanar
bool Coplanar(const Line3D& line1, const Line3D& line2)
{
	throw Unimplemented();
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line1, const Line3D& line2)
{
	throw Unimplemented();
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line, const Plane3D& plane)
{
	throw Unimplemented();
}

// Determine if two geometric entities are parallel.
bool Parallel(const Plane3D& plane1, const Plane3D& plane2)
{
	throw Unimplemented();
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line1, const Line3D& line2)
{
	throw Unimplemented();
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line, const Plane3D& plane)
{
	throw Unimplemented();
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2)
{
	throw Unimplemented();
}
