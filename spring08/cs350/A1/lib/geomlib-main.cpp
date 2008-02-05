/////////////////////////////////////////////////////////////////////////
// $Id: Geometry.cpp 972 2007-05-29 23:42:53Z gherron $
//
// Unit tests of the geometry library.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <fstream>

#include "geomlib.h"

struct Unimplemented {};	   // Marks code to be implemented by students.

bool R = false;
std::ofstream* out = NULL;

//   *out("output");

using namespace std;

const double epsilon = 1e-3;

int failures = 0;
int completed = 0;
int missing = 0;
int expected = 128;

#define AssertHere _AssertHere(__FILE__,__LINE__)
#define AssertNotHere(msg) _AssertNotHere(msg, __FILE__,__LINE__)
#define AssertEq(A,B,msg) _AssertEq(msg, A, B, __FILE__, __LINE__)

bool _AssertHere(char* file, int line)
{
	completed++;
	//std::cout << "Passed "  << file << ":" << line << std::endl;
	return true;
}

bool _AssertNotHere(char* name, char* file, int line)
{
	completed++;
	if (!failures++) std::cout << "Failed tests:" <<  std::endl;
	std::cout << "  " << name << "  At line "  << file << ":" << line << std::endl;
	if (R) *out << "  " << name << ";    ";
    return false;
}

bool _AssertEq(char* name, const double A, const double B,
			   char* file, int line)
{
	completed++;
	if (fabs(A-B) > epsilon) {
		if (!failures++) std::cout << "Failed tests:" <<  std::endl;
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Point2D A, const Point2D B,
			   char* file, int line)
{
	completed++;
	if (fabs(A[0]-B[0]) > epsilon ||
		fabs(A[1]-B[1]) > epsilon) {
		if (!failures++) std::cout << "Failed tests:" <<  std::endl;
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Point3D A, const Point3D B,
			   char* file, int line)
{
	completed++;
	if (fabs(A[0]-B[0]) > epsilon ||
		fabs(A[1]-B[1]) > epsilon ||
		fabs(A[2]-B[2]) > epsilon) {
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		failures++;
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Vector2D A, const Vector2D B,
			   char* file, int line)
{
	completed++;
	if (fabs(A[0]-B[0]) > epsilon ||
		fabs(A[1]-B[1]) > epsilon) {
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		failures++;
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Vector3D A, const Vector3D B,
			   char* file, int line)
{
	completed++;
	if (fabs(A[0]-B[0]) > epsilon ||
		fabs(A[1]-B[1]) > epsilon ||
		fabs(A[2]-B[2]) > epsilon) {
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		failures++;
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Vector4D A, const Vector4D B,
			   char* file, int line)
{
	completed++;
	if (fabs(A[0]-B[0]) > epsilon ||
		fabs(A[1]-B[1]) > epsilon ||
		fabs(A[2]-B[2]) > epsilon ||
		fabs(A[3]-B[3]) > epsilon) {
		std::cout << "  " << name << "  At line " << file << ":" << line << std::endl;
		if (R) *out << "  " << name << ";    ";
		failures++;
		return false; }
	else {
		//std::cout << "Passed " << file << ":" << line << std::endl;
		return true; }
}

bool _AssertEq(char* name, const Matrix4x4 A, const Matrix4x4 B,
			   char* file, int line)
{
	completed++;
	Matrix4x4::const_iterator i;
	Matrix4x4::const_iterator j;

	for (i=A.begin(), j=B.begin(); i != A.end(); ++i, ++j) {
		if (!_AssertEq(name, *i, *j, file, line)) {
			failures++;
			std::cout << A << std::endl;
			std::cout << B << std::endl;
			return false; } }
	return true;
}

void Points2()
{
	Point2D	 P(2.1, 2.2);
	Point2D	 Q(1.1, 2.1);
	Vector2D V(1.1, 1.2);
	AssertEq(P+V, Point2D(3.2, 3.4), "Point2D+Vector2D");
	AssertEq(P-V, Point2D(1.0, 1.0), "Point2D-Vector2D");
	AssertEq(Point2D(2.1, 2.2)+Vector2D(1.1, 1.2),
			 Point2D(3.2, 3.4), "Point2D+Vector2D");
	AssertEq(P-V, Point2D(1.0, 1.0), "Point2D-Vector2D");

	P += V;
	AssertEq(P, Point2D(3.2, 3.4), "Point2D+=Vector2D");
	P -= V;
	AssertEq(P, Point2D(2.1, 2.2), "Point2D-=Vector2D");

	AssertEq(P-Q, Vector2D(1.0, 0.1), "Point2D-Point2D");
}

void Points3()
{
	Point3D	 P(2.1, 2.2, 2.3);
	Point3D	 Q(1.1, 2.1, 3.1);
	Vector3D V(1.1, 1.2, 1.3);
	AssertEq(P+V, Point3D(3.2, 3.4, 3.6), "Point3D+Vector3D");
	AssertEq(P-V, Point3D(1.0, 1.0, 1.0), "Point3D-Vector3D");
	AssertEq(Point3D(2.1, 2.2, 2.3)+Vector3D(1.1, 1.2, 1.3),
			 Point3D(3.2, 3.4, 3.6), "Point3D+Vector3D");
	AssertEq(P-V, Point3D(1.0, 1.0, 1.0), "Point3D-Vector3D");

	P += V;
	AssertEq(P, Point3D(3.2, 3.4, 3.6), "Point3D+=Vector3D");
	P -= V;
	AssertEq(P, Point3D(2.1, 2.2, 2.3), "Point3D-=Vector3D");

	AssertEq(P-Q, Vector3D(1.0, 0.1, -0.8), "Point3D-Point3D");
}

void Vectors2()
{
	Vector2D U(2.1, 2.2);
	Vector2D V(1.1, 1.2);
	AssertEq(U+V, Vector2D(3.2, 3.4), "Vector2D+Vector2D");
	AssertEq(U-V, Vector2D(1.0, 1.0), "Vector2D-Vector2D");

	U += V;
	AssertEq(U, Vector2D(3.2, 3.4), "Vector2D+=Vector2D");
	U -= V;
	AssertEq(U, Vector2D(2.1, 2.2), "Vector2D-=Vector2D");

	AssertEq(U*2, Vector2D(2*2.1, 2*2.2), "scalar*Vector2D");
	AssertEq(2*U, Vector2D(2*2.1, 2*2.2), "Vector2D*scalar");
	AssertEq(U/2, Vector2D(2.1/2, 2.2/2), "Vector2D/scalar");

	U *= 2;
	AssertEq(U, Vector2D(2*2.1, 2*2.2), "Vector2D*=scalar");
	U /= 2;
	AssertEq(U, Vector2D(2.1, 2.2), "Vector2D/=scalar");
}

void Vectors3()
{
	Vector3D U(2.1, 2.2, 2.3);
	Vector3D V(1.1, 1.2, 1.3);
	AssertEq(U+V, Vector3D(3.2, 3.4, 3.6), "Vector3D+Vector3D");
	AssertEq(U-V, Vector3D(1.0, 1.0, 1.0), "Vector3D-Vector3D");

	U += V;
	AssertEq(U, Vector3D(3.2, 3.4, 3.6), "Vector3D+=Vector3D");
	U -= V;
	AssertEq(U, Vector3D(2.1, 2.2, 2.3), "Vector3D-=Vector3D");

	AssertEq(U*2, Vector3D(2*2.1, 2*2.2, 2*2.3), "scalar*Vector3D");
	AssertEq(2*U, Vector3D(2*2.1, 2*2.2, 2*2.3), "Vector3D*scalar");
	AssertEq(U/2, Vector3D(2.1/2, 2.2/2, 2.3/2), "Vector3D/scalar");

	U *= 2;
	AssertEq(U, Vector3D(2*2.1, 2*2.2, 2*2.3), "Vector3D*=scalar");
	U /= 2;
	AssertEq(U, Vector3D(2.1, 2.2, 2.3), "Vector3D/=scalar");
}

void Vectors4()
{
	Vector4D U(2.1, 2.2, 2.3);
	Vector4D V(1.1, 1.2, 1.3);
	AssertEq(U+V, Vector4D(3.2, 3.4, 3.6, 2), "Vector4D+Vector4D");
	AssertEq(U-V, Vector4D(1.0, 1.0, 1.0, 0), "Vector4D-Vector4D");

	U += V;
	AssertEq(U, Vector4D(3.2, 3.4, 3.6, 2), "Vector4D+=Vector4D");
	U -= V;
	AssertEq(U, Vector4D(2.1, 2.2, 2.3, 1), "Vector4D-=Vector4D");

	AssertEq(U*2, Vector4D(2*2.1,2*2.2,2*2.3,2), "scalar*Vector4D");
	AssertEq(2*U, Vector4D(2*2.1,2*2.2,2*2.3,2), "Vector4D*scalar");
	AssertEq(U/2, Vector4D(2.1/2,2.2/2,2.3/2,0.5),"Vector4D/scalar");

	U *= 2;
	AssertEq(U, Vector4D(2*2.1,2*2.2,2*2.3,2), "Vector4D*=scalar");
	U /= 2;
	AssertEq(U, Vector4D(2.1, 2.2, 2.3, 1), "Vector4D/=scalar");
}

void VectorLengthAndNormalize()
{
	const double a=3.1, b=4.2, c=5.3, d=6.4;

	{
		const double l = std::sqrt(a*a+b*b);
		Vector2D V(a,b);
		AssertEq(V.length(), l, "Vector2D.length()");
		
		AssertEq(V.normalized(), Vector2D(a/l,b/l),
			 "Vector2D.normalized()");
		
		try {
			const Vector2D zero;
			zero.normalized();
			AssertNotHere("Normal of zero Vector2D.");
		} catch(const ZeroDivide&) {
			AssertHere; }
	}

	{
		const double l = std::sqrt(a*a+b*b+c*c);
		Vector3D V(a,b,c);
		AssertEq(V.length(), l, "Vector3D.length()");
		
		AssertEq(V.normalized(), Vector3D(a/l,b/l,c/l),
				 "Vector3D.normalized()");
		
		try {
			const Vector3D zero;
			zero.normalized();
			AssertNotHere("Normal of zero Vector3D.");
		} catch(const ZeroDivide&) {
			AssertHere; }
	}

	{
		const double l = std::sqrt(a*a+b*b+c*c+d*d);
		Vector4D V(a,b,c,d);
		AssertEq(V.length(), l, "Vector4D.length()");
		
		AssertEq(V.normalized(), Vector4D(a/l,b/l,c/l,d/l),
				 "Vector4D.normalized()");
		
		try {
			const Vector4D zero(0,0,0,0);
			zero.normalized();
			AssertNotHere("Normal of zero Vector4D.");
		} catch(const ZeroDivide&) {
			AssertHere; }
	}
}

void VectorDot()
{
	const double a=3.1, b=4.2, c=5.3;
	const double d=6.1, e=7.2, f=8.3;
	{
		Vector2D U(a,b);
		Vector2D V(d,e);
		AssertEq(Dot(U,V), a*d+b*e, "Vector2D dot Vector2D");
	}
	{
		Vector3D U(a,b,c);
		Vector3D V(d,e,f);
		AssertEq(Dot(U,V), a*d+b*e+c*f, "Vector3D dot Vector3D");
	}
}

void VectorCross()
{
	const double a=3.1, b=4.2, c=5.3;
	const double d=6.1, e=7.2, f=8.3;
	Vector3D U(a,b,c);
	Vector3D V(d,e,f);

	Vector3D X = Cross(U,V);
	AssertEq(Dot(X,U), 0.0,
			 "Cross product perpendicular to first Vector3D.");
	AssertEq(Dot(X,V), 0.0,
			 "Cross product perpendicular to second Vector3D.");

	double cos_angle = Dot(U,V)/(U.length()*V.length());
	double sin_angle = std::sqrt(1-cos_angle*cos_angle);
	AssertEq(X.length(), U.length()*V.length()*sin_angle,
			 "Length of cross product");

	Vector3D E1(1,0,0);
	Vector3D E2(0,1,0);
	Vector3D E3(0,0,1);

	AssertEq(Cross(E1,E2), E3, "Right-handed Cross product.");
	AssertEq(Cross(E2,E3), E1, "Right-handed Cross product.");
	AssertEq(Cross(E3,E1), E2, "Right-handed Cross product.");
}

void PointLineDistance()
{

	Point3D	 P(9, 6, 5);
	Vector3D V(-83, 52, 344.65);
	Line3D line(P, V);
	Vector3D N = Cross(V, Vector3D(83, 52, 344.65)).normalized();

	AssertEq(Distance(P,line), 0,
			 "Distance of P to line<P,V>.");
	AssertEq(Distance(P+V,line), 0,
			 "Distance of P+V to line<P,V>");

	for (double d=1;	 d<=2;	d+=0.5) {
		Point3D Q0 = P + d*N;
		AssertEq(Distance(Q0,line), d,
				 "Distance of P+d*N to line<P,V>");
		Point3D Q1 = P + V - d*N;
		AssertEq(Distance(Q1,line), d,
				 "Distance of P+d*N to line<P,V>"); }
}

void PointPlaneDistance()
{
	Point3D P(121, -1290.2, 347.04);
	const double a=3.1, b=4.2, c=5.3;
	const double d=6.1, e=7.2, f=8.3;
	Vector3D U(a,b,c);
	Vector3D V(d,e,f);
	Vector3D N = Cross(U,V).normalized();
	Plane3D plane(N[0], N[1], N[2], -Dot(Vector3D(P[0],P[1],P[2]),N));

	AssertEq(Distance(P,plane), 0,
			 "Distance of P to plane containing P");
	AssertEq(Distance(P+U+V,plane), 0,
			 "Distance of P+U+V to plane containing P");

	for (double d=1;	 d<=2;	d+=0.5) {
		Point3D Q0 = P + U + V + d*N;
		AssertEq(Distance(Q0,plane), d,
				 "Distance of point from plane.");
		Point3D Q1 = P + U - V - d*N;
		AssertEq(Distance(Q1,plane), d,
				 "Distance of point from plane."); }
}

void AngleBetweenPlanes()
{
	const double a=3.1, b=4.2, c=5.3;
	const double d=6.1, e=7.2, f=8.3;
	Plane3D P(a, b, c, 12.0);
	Plane3D Q(d, e, f, 13.0);

	double cos0 = cos(AngleBetween(P,Q));
	double cos1 = Dot(P.normal().normalized(), Q.normal().normalized());

	AssertEq(cos0, cos1, "Angle between planes.");
}

void AngleBetweenLinePlane()
{
	Vector3D V(-37, 31, -23);
	Vector3D W(44, -345, 883);
	Plane3D P(V[0], V[1], V[2], 1.0);
	Line3D L(Point3D(255.45, -42.78, 363), W);

	double cos0 = cos(AngleBetween(L,P));
	double cos1 = Dot(V,W);
	double l = V.length()*W.length();

	AssertEq(cos0, cos1/l, "Angle between line and plane.");
}

void AngleBetweenLines()
{
	Vector3D V(-37, 31, -23);
	Vector3D W(44, -345, 883);
	Line3D M(Point3D(323, 45, -457), V);
	Line3D L(Point3D(255.45, -42.78, 363), W);

	double cos0 = cos(AngleBetween(M,L));
	double cos1 = Dot(V,W);
	double l = V.length()*W.length();

	AssertEq(cos0, cos1/l, "Angle between lines.");
}

void CoplanarLines()
{
	Point3D Base(1,2,3);
	Vector3D N(0.1, 0.2, 1.0);
	Vector3D V (1.0, 0.3, 0.2);
	Vector3D W=Cross(N,V);

	if (Coplanar(Line3D(Base,V),Line3D(Base,W))) {
		AssertHere; }
	else {
		AssertNotHere("Coplanar lines with same base."); }

	if (Coplanar(Line3D(Base,V),Line3D(Base+V,W))) {
		AssertHere; }
	else {
		AssertNotHere("Coplanar lines with different base."); }

	if (Coplanar(Line3D(Base,V),Line3D(Base+N,W))) {
		AssertNotHere("NON-Coplanar lines with different base."); }
	else {
		AssertHere; }
}

void ParallelPerpendicular()
{
	Point3D P0(1,1,0);
	Point3D P1(1,0,1);
	Vector3D V(1,2,3);
	Vector3D W = Cross(V, Vector3D(1,0,1));

	Line3D LW0 = Line3D(P0,W);
	Line3D LW1 = Line3D(P1,W);
	Line3D LV1 = Line3D(P1,V);

	Plane3D PW0(W[0], W[1], W[2], 0);
	Plane3D PW1(W[0], W[1], W[2], 1);
	Plane3D PV1(V[0], V[1], V[2], 1);

	AssertEq(true,  Parallel(LW0, LW1), "Parallel lines.");
	AssertEq(false, Parallel(LW0, LV1), "Not parallel lines.");
	AssertEq(false, Perpendicular(LW0, LW1), "Not perpendicular lines.");
	AssertEq(true,  Perpendicular(LW0, LV1), "Perpendicular lines.");

	AssertEq(true,  Parallel(PW0, PW1), "Parallel planes.");
	AssertEq(false, Parallel(PW0, PV1), "Not parallel planes.");
	AssertEq(false, Perpendicular(PW0, PW1), "Not perpendicular planes.");
	AssertEq(true,  Perpendicular(PW0, PV1), "Perpendicular planes.");

	AssertEq(false,  Parallel(LW0, PW1), "Not parallel line/plane.");
	AssertEq(true, Parallel(LW0, PV1), "Parallel line/plane.");
	AssertEq(true, Perpendicular(LW0, PW1), "Perpendicular line/plane.");
	AssertEq(false,  Perpendicular(LW0, PV1), "Not perpendicular line/plane.");
	   
}


void Line2DIntersection()
{
	Line2D first(Point2D(15, 6), Vector2D(9, 3));
	Line2D second(Point2D(3, 0), Vector2D(0, 9));

	Point2D point;
	if (Intersects(first, second, &point)) {
		AssertEq(point, Point2D(3,2),
				 "Line2D/Line2D intersection point."); }
	else {
		AssertNotHere("Line2D/Line2D intersection."); }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Line2D/Line2D intersection with NULL pointer."); }

	first.point = Point2D(0, 0);
	first.vector = Vector2D(9, 3);
	second.point = Point2D(3, 0);
	second.vector = Vector2D(9, 3);

	if (Intersects(first, second, &point)) {
		AssertNotHere("Line2D/Line2D non-intersection."); }
	else {
		AssertHere; }

}

void Segment2DIntersection()
{
	Segment2D first(Point2D(0, 0), Point2D(12, 12));
	Segment2D second(Point2D(0, 12), Point2D(12, 0));

	Point2D point;
	if (Intersects(first, second, &point)) {
		AssertEq(point, Point2D(6,6),
				 "Segment2D/Point2D intersection point."); }
	else {
		AssertNotHere("Segment2D/Point2D intersection."); }

	first.point1 = Point2D(0, 0);
	first.point2 = Point2D(12, 12);
	second.point1 = Point2D(48, 48);
	second.point2 = Point2D(13, 13);

	if (Intersects(first, second, &point)) {
		AssertNotHere("Segment2D/Point2D  non-intersection."); }
	else {
		AssertHere; }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Segment2D/Point2D intersection with NULL pointer."); }
}

void LinePlaneIntersection()
{
	Line3D first(Point3D(2.23, 123.0, 401), Vector3D(123, -13, 63.3));
	Plane3D second(32, 3.5, 21, -12);

	Point3D point;
	if (Intersects(first, second, &point))
	{
		AssertEq(point, Point3D(-2.0774658530978e+02,
								1.4519264722786e+02,
								2.9293887926740e+02),
				 "Line3D/Plane3D intersection point."); }
	else {
		AssertNotHere("Line3D/Plane3D intersection."); }

	first.point = Point3D(0, 0, 1);
	first.vector = Vector3D(0, 0, 1);
	second[0] = 1;
	second[1] = 0;
	second[2] = 0;
	second[3] = 0;

	if (Intersects(first, second, &point)) {
		AssertNotHere("Line3D/Plane3D non-intersection."); }
	else {
		AssertHere; }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Line3D/Plane3D intersection with NULL pointer."); }
}

void SegmentPlaneIntersection()
{
	Point3D base(2.23, 123.0, 401);
	Vector3D vec(-123, -13, 63.3);

	Segment3D seg(base, base+vec);
	Plane3D plane(3.2, 3.5, .21, -120);

	Point3D point;
	if (Intersects(seg, plane, &point))
	{
		AssertEq(0.0, Distance(point,plane),
				 "Segment3D/Plane3D intersection point on plane.");
		AssertEq(0.0, Distance(point,Line3D(base,vec)),
				 "Segment3D/Plane3D intersection point on line.");
		AssertEq(true, seg.contains(point),
				 "Segment3D/Plane3D intersection point on Segment.");
	}
	else {
		AssertNotHere("Segment3D/Plane3D intersection."); }

	seg.point1 = Point3D(0, 0, 1);
	seg.point2 = seg.point1+Vector3D(0, 0, 1);
	plane[0] = 1;
	plane[1] = 0;
	plane[2] = 0;
	plane[3] = 0;

	if (Intersects(seg, plane, &point)) {
		AssertNotHere("Segment3D/Plane3D non-intersection."); }
	else {
		AssertHere; }

	try {
		Intersects(seg, plane);
		AssertHere; }
	catch(...) {
		AssertNotHere("Segment3D/Plane3D intersection with NULL pointer."); }
}

void Ray3DPointContainment()
{ float t;

	{	Ray3D RAY(Point3D(), Vector3D(1, 1, 1));
		Point3D P(-2, -2, -2);
		if (RAY.contains(P, &t)) {
			AssertNotHere("Ray3D/Point3D non-containment"); }
		else {
			AssertHere; }
	}

	{  	Ray3D RAY(Point3D(1, 0, 2), Vector3D(4, 8, 6));
		Point3D P(3, 4, 5);

		if (RAY.contains(P, &t)) {
			AssertEq(t, 0.5, "Ray3D/Point3D containment parameter value."); }
		else {
			AssertNotHere("Ray3D/Point3D containment."); }

		try {
			RAY.contains(P);
			AssertHere; }
		catch(...) {
			AssertNotHere("Ray3D/Point3D containment with NULL pointer."); }
	}

}

void Box3DPointContainment()
{
	Box3D BOX(Point3D(), Point3D(1, 1, 1));
	
	Point3D points[] = {
		Point3D(),
		Point3D(1,1,1),
		Point3D(0.1, 0.1, 0.1)};

	for (int i=0;  i<sizeof(points)/sizeof(points[0]);  i++) {
		if (BOX.contains(points[i])) {
			AssertHere; }
		else {
			AssertNotHere("Box3D/Point3D containment."); }
		
		if (BOX.contains(points[i]+Vector3D(2,0,0))) {
			AssertNotHere("Box3D/Point3D non-containment"); }
		else {
			AssertHere; }
		
		if (BOX.contains(points[i]+Vector3D(-2,0,0))) {
			AssertNotHere("Box3D/Point3D non-containment"); }
		else {
			AssertHere; }
		
		if (BOX.contains(points[i]+Vector3D(0,2,0))) {
			AssertNotHere("Box3D/Point3D non-containment"); }
		else {
			AssertHere; }
		
		if (BOX.contains(points[i]+Vector3D(0,-2,0))) {
			AssertNotHere("Box3D/Point3D non-containment"); }
		else {
			AssertHere; } }
}

void Segment3DPointContainment()
{
	{  	Segment3D seg(Point3D(1, 0, 2), Point3D(5, 8, 8));
		Point3D P(3, 4, 5);

		if (seg.contains(P)) {
			AssertHere; }
		else {
			AssertNotHere("Segment3D/Point3D containment."); }
	}

	{	Segment3D seg(Point3D(), Point3D(1, 1, 1));
		Point3D P(2, 2, 2);
		if (seg.contains(P)) {
			AssertNotHere("Segment3D/Point3D non-containment"); }
		else {
			AssertHere; }
	}
}

void TrianglePointContainment()
{
	Triangle3D TRI(Point3D(4, -2, 4), Point3D(0, -2, 2),
		Point3D(3, -2, -1));
	Point3D P(3, -2, 3);

	if (TRI.contains(P)) {
		AssertHere; }
	else {
		AssertNotHere("Triangle3D/Point3D containment."); }

	TRI[0] = Point3D(1, 0, 0);
	TRI[1] = Point3D(-1, 1, 0);
	TRI[2] = Point3D(0, -1, 0);
	P = Point3D(0, 0, 1);

	if (TRI.contains(P)) {
		AssertNotHere("Triangle3D/Point3D non-containment."); }
	else {
		AssertHere; }
}

void Segment3DTriangleIntersection()
{
	Segment3D first(Point3D(3, 0, 0), Point3D(3, 3, 6));
	Triangle3D second(Point3D(0, 0, 6), Point3D(6, 0, 6),
					  Point3D(3, 3, 0));

	Point3D point;
	if (Intersects(first, second, &point)) {
		AssertEq(point, Point3D(3,1.5,3),
				 "Segment3D/Triangle3D intersection point."); }
	else {
		AssertNotHere("Segment3D/Triangle3D intersection."); }

	first.point1 = Point3D(10, 10, 10);
	first.point2 = Point3D(20, 20, 20);
	second[0] = Point3D(1, 0, 0);
	second[1] = Point3D(-1, 1, 0);
	second[2] = Point3D(0, -1, 0);

	if (Intersects(first, second, &point)) {
		AssertNotHere("Segment3D/Triangle3D non-intersection"); }
	else {
		AssertHere; }
}

void TriangleIntersection()
{
	Triangle3D first(Point3D(0, 0, 6), Point3D(6, 0, 6),
					 Point3D(3, 3, 0));
	Triangle3D second(Point3D(0, 3, 4), Point3D(6, 3, 4),
					  Point3D(3, 0, 3));

	std::pair <Point3D, Point3D> points;
	int intersection_count = Intersects(first, second, &points);

	if (intersection_count == 2)
	{
		if (points.first[X] > points.second[X]) {
			AssertEq(points.first,	Point3D(4.285714, 1.285714, 3.428571),
					 "Triangle3D/Triangle3D intersection -- first point.");
			AssertEq(points.second, Point3D(1.714285, 1.285714, 3.428571),
					 "Triangle3D/Triangle3D intersection -- second point."); }
		else {
			AssertEq(points.first,	Point3D(1.714285, 1.285714, 3.428571),
					 "Triangle3D/Triangle3D intersection -- first point.");
			AssertEq(points.second, Point3D(4.285714, 1.285714, 3.428571),
					 "Triangle3D/Triangle3D intersection -- second point."); } }
	else {
		AssertNotHere("Triangle3D/Triangle3D intersection."); }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Triangle3D/Triangle3D intersection with NULL pointer.");}
}

void RaySphereIntersection()
{
	Ray3D first(Point3D(0, 4, 6), Vector3D(5, 3, -5));
	Sphere3D second(Point3D(2.5, 5.5, 3.5), 1);

	std::pair< Point3D, Point3D > points;
	int intersection_count = Intersects(first, second, &points);

	if (intersection_count == 2) {
		if (points.first[X] > points.second[X]) {
			AssertEq(points.first,	Point3D(3.1509445,5.8905667,2.8490554),
					 "Ray3D/Sphere3D intersection -- first point.");
			AssertEq(points.second, Point3D(1.8490554,5.1094332,4.1509445),
					 "Ray3D/Sphere3D intersection -- second point."); }
		else {
			AssertEq(points.first,	Point3D(1.8490554,5.1094332,4.1509445),
					 "Ray3D/Sphere3D intersection -- first point.");
			AssertEq(points.second, Point3D(3.1509445,5.8905667,2.8490554),
					 "Ray3D/Sphere3D intersection -- second point."); } }
	else {
		AssertNotHere("Ray3D/Sphere3D intersection."); }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Ray3D/Sphere3D intersection with NULL pointer."); }
}

void RayBoxIntersection()
{
	Ray3D first(Point3D(0, 4, 6), Vector3D(5, 3, -5));
	Box3D second(Point3D(1.5, 4.5, 2.5), Point3D(3.5, 6.5, 4.5));

	std::pair< Point3D, Point3D > points;
	int intersection_count = Intersects(first, second, &points);

	if (intersection_count == 2) {
		if (points.first[X] > points.second[X]) {
			AssertEq(points.first,	Point3D(3.5, 6.1, 2.5),
					 "Incorrect Ray3D/Box3D intersection -- first point.");
			AssertEq(points.second, Point3D(1.5, 4.9, 4.5),
					 "Incorrect Ray3D/Box3D intersection -- second point."); }
		else {
			AssertEq(points.first,	Point3D(1.5, 4.9, 4.5),
					 "Incorrect Ray3D/Box3D intersection -- first point.");
			AssertEq(points.second, Point3D(3.5, 6.1, 2.5),
					 "Incorrect Ray3D/Box3D intersection -- second point."); } }
	else {
		AssertNotHere("Ray3D/Box3D intersection."); }

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Ray3D/Box3D intersection with NULL pointer."); }
}


void RayTriangleIntersection()
{
	Ray3D first(Point3D(), Vector3D(0, 0, 10));
	Triangle3D second(Point3D(0, 10, -4), Point3D(-10, -10, -4),
					  Point3D(10, -10, -4));

	Point3D point;

	//Test for the ray not intersecting the triangle
	AssertEq(Intersects(first, second, &point), false,
					 "Ray3D/Triangle3D npn-intersection.");

	//Test that the ray does intersect the triangle
	first = Ray3D(Point3D(0,3,0), Vector3D(0, -2, -6));
	second = Triangle3D(Point3D(0, 10, -6), Point3D(-10, -10, -6), Point3D(10, -10, -6));

	AssertEq(Intersects(first, second, &point), true,
			 "Valid Ray3D/Triangle3D intersection.");
	AssertEq(point, Point3D(0,1,-6),
			 "Incorrect Ray3D/Triangle3D intersection point.");

	try {
		Intersects(first, second);
		AssertHere; }
	catch(...) {
		AssertNotHere("Ray3D/Triangle3D intersection with NULL pointer."); }

}

Matrix4x4 Transpose(const Matrix4x4 M)
{
	return Matrix4x4(
		Vector4D(M[0][0], M[1][0], M[2][0], M[3][0]),
		Vector4D(M[0][1], M[1][1], M[2][1], M[3][1]),
		Vector4D(M[0][2], M[1][2], M[2][2], M[3][2]),
		Vector4D(M[0][3], M[1][3], M[2][3], M[3][3]));
}

void MatrixMultiplication()
{
	Vector4D A1(1.0, 0.0, 0.0, 0.0);
	Vector4D A2(0.0, 1.0, 0.0, 0.0);
	Vector4D A3(0.0, 0.0, 1.0, 0.0);
	Vector4D A4(0.0, 0.0, 0.0, 1.0);

	Vector4D B1(1.0, 2.0, 3.0,-4.0);
	Vector4D B2(2.0, 3.0, 4.0, 1.0);
	Vector4D B3(3.0, 4.0, 5.0, 3.0);
	Vector4D B4(4.0, 5.0, 6.0, 7.0);

	Vector4D C1 = 1.0*A1 +2.0*A2 -3.0*A3 +4.0*A4;
	Vector4D C2 = 2.0*A1 +8.0*A2 +1.0*A3 -4.0*A4;
	Vector4D C3 =-3.0*A1 -2.0*A2 -3.0*A3 +2.0*A4;
	Vector4D C4 = 4.0*A1 -8.0*A2 -4.0*A3 +6.0*A4;

	Vector4D D1 = 1.0*B1 +2.0*B2 -3.0*B3 +4.0*B4;
	Vector4D D2 = 2.0*B1 +8.0*B2 +1.0*B3 -4.0*B4;
	Vector4D D3 =-3.0*B1 -2.0*B2 -3.0*B3 +2.0*B4;
	Vector4D D4 = 4.0*B1 -8.0*B2 -4.0*B3 +6.0*B4;

	Matrix4x4 B(B1,B2,B3,B4);
	Matrix4x4 C(C1,C2,C3,C4);
	Matrix4x4 D(D1,D2,D3,D4);

	AssertEq(C*B,D, "Matrix4x4/Matrix4x4 product.");

	Matrix4x4 BT = Transpose(B);
	Matrix4x4 DT = Transpose(D);

	AssertEq(C*BT[0], DT[0], "Matrix4x4/Vector4D product -- column 0.");
	AssertEq(C*BT[1], DT[1], "Matrix4x4/Vector4D product -- column 1.");
	AssertEq(C*BT[2], DT[2], "Matrix4x4/Vector4D product -- column 2.");
	AssertEq(C*BT[3], DT[3], "Matrix4x4/Vector4D product -- column 3.");
}

void MatrixLookAt()
{
	Point3D center(0.0, 1.0, 0.0);
	Point3D target(0.0, 0.0, 1.0);
	Vector3D up(0.0, 1.0, 0.0);
	Matrix4x4 L(LookAt(center, target, up));

	double s = sqrt(2.0)/2;

	Matrix4x4 C(Vector4D(-1,0,0,0),
				Vector4D(0,s,s,-s),
				Vector4D(0,s,-s,-s),
				Vector4D(0,0,0,1));

	AssertEq(L,C, "LootAt matrix.");
}

void MatrixPerspective()
{
	double w = 10;
	double h = 8;
	double d = 1;
	double f = 100;
	Matrix4x4 P = Perspective(w, h, d, f);

	double s = P[0][0]/(d/w);

	Matrix4x4 C(s*Vector4D(d/w,0,0,0),
				s*Vector4D(0,d/h,0,0),
				s*Vector4D(0,0,f/(f-d), -d*f/(f-d)),
				s*Vector4D(0,0,1,0));
	AssertEq(P,C, "Perspective matrix.");
}

void MatrixReset()
{
	Matrix4x4 M(Vector4D(1, 2, 3, 4), Vector4D(5, 6, 7, 8),
				Vector4D(9, 10, 11, 12), Vector4D(13, 14, 15, 16));

	M.reset();

	Matrix4x4 I(Vector4D(1,0,0,0),
				Vector4D(0,1,0,0),
				Vector4D(0,0,1,0),
				Vector4D(0,0,0,1));

	AssertEq(M,I, "Matrix reset.");
}

#define TEST(name, fn)								\
	try { \
		fn(); \
	} catch(const Unimplemented&) { \
		if (!failures++) std::cout << "Failed tests:" <<  std::endl; \
		std::cout << "  " << name << " called unimplemented code." << std::endl; \
		if (R) *out << "  " << name << " called unimplemented code;    "; \
	}


int main()
{

	if (R) out = new std::ofstream("output");
	std::cout << std::endl;

	TEST("Points2", Points2);
	TEST("Points3", Points3);
	TEST("Vectors2", Vectors2);
	TEST("Vectors3", Vectors3);
	TEST("Vectors4", Vectors4);
	TEST("VectorLengthAndNormalize", VectorLengthAndNormalize);
	TEST("VectorDot", VectorDot);
	TEST("VectorCross", VectorCross);
	TEST("PointLineDistance", PointLineDistance);
	TEST("PointPlaneDistance", PointPlaneDistance);
	TEST("AngleBetweenPlanes", AngleBetweenPlanes);
	TEST("AngleBetweenLines", AngleBetweenLines);
	TEST("AngleBetweenLinePlane", AngleBetweenLinePlane);
	TEST("CoplanarLines", CoplanarLines);
	TEST("ParallelPerpendicular", ParallelPerpendicular);
	TEST("Line2DIntersection", Line2DIntersection);
	TEST("Segment2DIntersection", Segment2DIntersection);
	TEST("LinePlaneIntersection", LinePlaneIntersection);
	TEST("SegmentPlaneIntersection", SegmentPlaneIntersection);
	TEST("Ray3DPointContainment", Ray3DPointContainment);
	TEST("Box3DPointContainment", Box3DPointContainment);
	TEST("Segment3DPointContainment", Segment3DPointContainment);
	TEST("TrianglePointContainment", TrianglePointContainment);
	TEST("Segment3DTriangleIntersection", Segment3DTriangleIntersection);
	TEST("TriangleIntersection", TriangleIntersection);
	TEST("RaySphereIntersection", RaySphereIntersection);
	TEST("RayBoxIntersection", RayBoxIntersection);
	TEST("RayTriangleIntersection", RayTriangleIntersection);
	TEST("MatrixMultiplication", MatrixMultiplication);
	TEST("MatrixLookAt", MatrixLookAt);
	TEST("MatrixPerspective", MatrixPerspective);
	TEST("MatrixReset", MatrixReset);

	if (R) out->close();

	std::cout << std::endl;
	std::cout << completed << " tests completed." << std::endl;
	std::cout << "    " << completed-failures << " tests passed." << std::endl;
	std::cout << "    " << failures << " tests failed." << std::endl;
		

	std::cout << "Press RETURN to quit: ";
	char* x;
	scanf("%c", &x);
}
