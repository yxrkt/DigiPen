///////////////////////////////////////////////////////////////////////
// $Id: geomlib.cpp 972 2007-05-29 23:42:53Z gherron $
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "geomlib.h"
#include <vector>

float ToRadians(const float& Degrees)
{
    const float ret = (PI*Degrees)/180.0f;
    return ret;
}

// Test any STL sequence for all elements zero
template<class Sequence> bool IsZero(const Sequence& s)
{
    for(typename Sequence::const_iterator i = s.begin(); i != s.end(); ++i)
        if(0 != *i)
            return false;
    return true;
}


////////////////////////////////////////////////////////////////////////
// Color methods and functions
////////////////////////////////////////////////////////////////////////
Color::Color(const float r, const float g,
			 const float b, const float a)
{
	Set(r,g,b,a);
}

void Color::Set(float r, float g, float b, float a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}
	
Color operator+(const Color& C, const Color& D)
{
	return Color(C[0] + D[0], C[1] + D[1], C[2] + D[2], C[3] + D[3]);
}

Color operator*(Color& C, float t)
{
	return Color(C[0]*t, C[1]*t, C[2]*t, C[3]*t);
}

Color operator*(float t, const Color& C)
{
	return Color(C[0]*t, C[1]*t, C[2]*t, C[3]*t);
}

Color operator*(float t, Color& C)
{
	return Color(C[0]*t, C[1]*t, C[2]*t, C[3]*t);
}

Color operator*(Color& C, Color& D)
{
	return Color(C[0]*D[0], C[1]*D[1], C[2]*D[2], C[3]*D[3]);
}

HSVColor::HSVColor(float h, float s, float v, float a)
{
	if (s == 0.0)
		Set(v,v,v,a);
	else {
		int i = (int)(h*6.0);
		float f = (h*6.0) - i;
		float p = v*(1.0 - s);
		float q = v*(1.0 - s*f);
		float t = v*(1.0 - s*(1.0-f));
		if (i%6 == 0)
			Set(v,t,p,a);
		else if (i == 1)
			Set(q,v,p,a);
		else if (i == 2)
			Set(p,v,t,a);
		else if (i == 3)
			Set(p,q,v,a);
		else if (i == 4)
			Set(t,p,v,a);
		else if (i == 5)
			Set(v,p,q,a); }
}

////////////////////////////////////////////////////////////////////////
// In-place operations
////////////////////////////////////////////////////////////////////////

// Point2d In-place operations
Point2D& Point2D::operator+=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::plus<float>());
    return *this;
}

Point2D& Point2D::operator-=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

// Vector2D In-place operations
Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::plus<float>());
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::minus<float>());
    return *this;
}

Vector2D& Vector2D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector2D& Vector2D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector2D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Point3D In-place operations
Point3D& Point3D::operator+=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::plus<float>());
    return *this;
}

Point3D& Point3D::operator-=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

// Vector3D In-place operations
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::plus<float>());
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

Vector3D& Vector3D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector3D& Vector3D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector3D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Vector4D In-place operations
Vector4D& Vector4D::operator+=(const Vector4D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::plus<float>());
    return *this;
}

Vector4D& Vector4D::operator-=(const Vector4D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

Vector4D& Vector4D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector4D& Vector4D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector4D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Matrix4x4 In-place operations
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	Matrix4x4 temp;

	const Vector4D& A = (*this)[0];
	const Vector4D& B = (*this)[1];
	const Vector4D& C = (*this)[2];
	const Vector4D& D = (*this)[3];

	temp[0][0] = rhs[0][0]*A[0]+rhs[1][0]*A[1]+rhs[2][0]*A[2]+rhs[3][0]*A[3];
	temp[1][0] = rhs[0][0]*B[0]+rhs[1][0]*B[1]+rhs[2][0]*B[2]+rhs[3][0]*B[3];
	temp[2][0] = rhs[0][0]*C[0]+rhs[1][0]*C[1]+rhs[2][0]*C[2]+rhs[3][0]*C[3];
	temp[3][0] = rhs[0][0]*D[0]+rhs[1][0]*D[1]+rhs[2][0]*D[2]+rhs[3][0]*D[3];

	temp[0][1] = rhs[0][1]*A[0]+rhs[1][1]*A[1]+rhs[2][1]*A[2]+rhs[3][1]*A[3];
	temp[1][1] = rhs[0][1]*B[0]+rhs[1][1]*B[1]+rhs[2][1]*B[2]+rhs[3][1]*B[3];
	temp[2][1] = rhs[0][1]*C[0]+rhs[1][1]*C[1]+rhs[2][1]*C[2]+rhs[3][1]*C[3];
	temp[3][1] = rhs[0][1]*D[0]+rhs[1][1]*D[1]+rhs[2][1]*D[2]+rhs[3][1]*D[3];

	temp[0][2] = rhs[0][2]*A[0]+rhs[1][2]*A[1]+rhs[2][2]*A[2]+rhs[3][2]*A[3];
	temp[1][2] = rhs[0][2]*B[0]+rhs[1][2]*B[1]+rhs[2][2]*B[2]+rhs[3][2]*B[3];
	temp[2][2] = rhs[0][2]*C[0]+rhs[1][2]*C[1]+rhs[2][2]*C[2]+rhs[3][2]*C[3];
	temp[3][2] = rhs[0][2]*D[0]+rhs[1][2]*D[1]+rhs[2][2]*D[2]+rhs[3][2]*D[3];

	temp[0][3] = rhs[0][3]*A[0]+rhs[1][3]*A[1]+rhs[2][3]*A[2]+rhs[3][3]*A[3];
	temp[1][3] = rhs[0][3]*B[0]+rhs[1][3]*B[1]+rhs[2][3]*B[2]+rhs[3][3]*B[3];
	temp[2][3] = rhs[0][3]*C[0]+rhs[1][3]*C[1]+rhs[2][3]*C[2]+rhs[3][3]*C[3];
	temp[3][3] = rhs[0][3]*D[0]+rhs[1][3]*D[1]+rhs[2][3]*D[2]+rhs[3][3]*D[3];
	
	return *this = temp;
}

void Matrix4x4::reset()
{
	*this = Matrix4x4();
    return;
}


////////////////////////////////////////////////////////////////////////
// Various operations
////////////////////////////////////////////////////////////////////////

// Vector* length
float Vector2D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

float Vector3D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

float Vector4D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

// Vector* normalized()
Vector2D Vector2D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector2D ret(*this);
    return ret /= len;
}

Vector3D Vector3D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector3D ret(*this);
    return ret /= len;
}

Vector4D Vector4D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector4D ret(*this);
    return ret /= len;
}

// Cross product (Vector3D only)
Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
{
    Vector3D ret;

        ret[0] = v1[1]*v2[2] - v1[2]*v2[1];
        ret[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
        ret[2] = v1[0]*v2[1] - v1[1]*v2[0];

    return ret;
}

// Homogeneous division of a Vector4D to a Point3D
Point3D Vector4D::Hdiv() throw(ZeroDivide)
{
	if (!crds[3]) throw ZeroDivide();
	return Point3D(crds[0]/crds[3],
				   crds[1]/crds[3],
				   crds[2]/crds[3]);
}

////////////////////////////////////////////////////////////////////////
// Distance methods
////////////////////////////////////////////////////////////////////////

float Distance(const Point3D& p, const Line3D& l)
{
	Vector3D pVector(p - l.point);
	Vector3D vVector = l.vector.normalized();
	Vector3D projection = vVector * Dot(pVector, vVector);
	return (pVector - projection).length();
}

float Distance(const Point3D& p, const Plane3D& m)
{
	Vector3D normal(m[0], m[1], m[2]);
	float newD = m[3] * (1/normal.length());
	normal = normal.normalized();
    return fabs(Dot(normal, p - Point3D(0,0,0)) + newD);
}

////////////////////////////////////////////////////////////////////////
// Containment methods
////////////////////////////////////////////////////////////////////////

bool LineSeg3D::contains(const Point3D& p) const
{
	float* t = new float;
	Ray3D testRay(point1, point2-point1);

	if(testRay.contains(p, t) && *t <= 1)  {
		delete t;
		return true; }
	else  {
		delete t;
		return false; }
}

bool Ray3D::contains(const Point3D& p, float *t) const
{
	if(fabs(Distance(p, Line3D(origin, direction))) < .0001) {
		if(Dot((p - origin), direction)>= 0) {
			if(t)
				*t = (p - origin).length() / direction.length();
			return true; }
		else
			return false; }
	else
		return false;
}

bool Box3D::contains(const Point3D& p) const
{
    bool ret = false;

    if(origin[X] <= p[X] && p[X] <= extent[X]
        && origin[Y] <= p[Y] && p[Y] <= extent[Y]
        && origin[Z] <= p[Z] && p[Z] <= extent[Z]) {
        ret = true; }

    return ret;
}

bool Triangle3D::contains(const Point3D& p) const
{
	const Point3D& A = (*this)[0];
	const Point3D& B = (*this)[1];
	const Point3D& C = (*this)[2];

	Vector3D planeNormal = Cross(B-A,
								 C-A).normalized();
	Plane3D trianglePlane(planeNormal[0], planeNormal[1], planeNormal[2],
						  -Dot(planeNormal, A-Point3D(0,0,0)));
	if(fabs(Distance(p, trianglePlane)) < .0001) {
		Vector3D plane1Normal = Cross(B-A, Cross(B-A, C-A)).normalized();
		Vector3D plane2Normal = Cross(C-B, Cross(C-B, A-B)).normalized();
		Vector3D plane3Normal = Cross(A-C, Cross(A-C, B-C)).normalized();

		//test plane 1
		if(Dot(p - A, plane1Normal) < 0) {
			//test plane 2
			if(Dot(p - B, plane2Normal) < 0) {
				//test plane 3
				if(Dot(p - C, plane3Normal) < 0)
					return true;
				else
					return false; }
			else
				return false; }
		else
			return false; }
	else
		return false;
}

////////////////////////////////////////////////////////////////////////
// Intersects functions
////////////////////////////////////////////////////////////////////////
bool Intersects(const LineSeg2D& l1, const LineSeg2D& l2, Point2D *p)
{
    bool ret = false;
    /* convert line segments into lines so code can be leveraged */
    Line2D line1(l1.point1, l1.point1 - l1.point2);
    Line2D line2(l2.point1, l2.point1 - l2.point2);
    
    Point2D M;

    /* leverage code: */
    if(Intersects(line1, line2, &M))
    {
        /* containment tests */
        if(std::min(l1.point1[X], l1.point2[X]) <= M[X]
            && M[X] <= std::max(l1.point1[X], l1.point2[X])
            && std::min(l1.point1[Y], l1.point2[Y]) <= M[Y]
            && M[Y] <= std::max(l1.point1[Y], l1.point2[Y])
            && std::min(l2.point1[X], l2.point2[X]) <= M[X]
            && M[X] <= std::max(l2.point1[X], l2.point2[X])
            && std::min(l2.point1[Y], l2.point2[Y]) <= M[Y]
            && M[Y] <= std::max(l2.point1[Y], l2.point2[Y]))  {
            if(p)
                *p = M;
            ret = true; } }

    return ret;
}

bool Intersects(const Line2D& l1, const Line2D& l2, Point2D *p)
{
    bool ret = false;

    /* special case #1: parallel lines (i.e. cross product equals zero) */
    const float intersect = l2.vector[X]*l1.vector[Y]
        - l1.vector[X]*l2.vector[Y];

    /* intersection condition: p2q1 != p1q2 */
    if(intersect) {
        if(p)  {
            (*p)[X] = (l1.vector[X]*l2.vector[X]
                *(l2.point[Y] - l1.point[Y])
                + l2.vector[X]*l1.vector[Y]*l1.point[X]
                - l1.vector[X]*l2.vector[Y]*l2.point[X]) / intersect;
            (*p)[Y] = (l1.vector[Y]*l2.vector[Y]
                *(l1.point[X] - l2.point[X])
                + l2.vector[X]*l1.vector[Y]*l2.point[Y]
                - l1.vector[X]*l2.vector[Y]*l1.point[Y]) / intersect; }

        /* indicate the intersection */
        ret = true; }

    return ret;
}

bool Intersects(const Line3D& l, const Plane3D& p, Point3D *m)
{
	Vector3D pNormal(p[0], p[1], p[2]);
	if(Dot(pNormal, l.vector) == 0)
		return false;
	else  {
		float timeOfIntersect = (-p[3] - Dot(pNormal, l.point-Point3D(0,0,0)))
			/ Dot(pNormal, l.vector);
		if (m)
			*m = l.point + timeOfIntersect * l.vector;
		return true; }
}

bool Intersects(const LineSeg3D& l, const Plane3D& p, Point3D *m)
{
	Line3D testLine;
	testLine.vector = l.point2 - l.point1;
	testLine.point = l.point1;

	bool pointGiven;
	if(m)
		pointGiven = true;
	else {
		pointGiven = false;
		m = new Point3D;}

	if(Intersects(testLine, p, m)) {
		if(l.contains(*m)) {
			if(!pointGiven)
				delete m;
			return true; }
		else {
			if(!pointGiven)
				delete m;
			return false; } }
	else {
		if(!pointGiven)
			delete m;
		return false; }
}

bool Intersects(const LineSeg3D& l, const Triangle3D& t, Point3D *p)
{
	Ray3D testRay;
	testRay.direction = (l.point2 - l.point1).normalized();
	testRay.origin = l.point1;

	bool pointGiven;
	if(p)
		pointGiven = true;
	else  {
		pointGiven = false;
		p = new Point3D; }


	if(Intersects(testRay, t, p)) {
		if(l.contains(*p)) {
			if(!pointGiven)
				delete p;
			return true; }
		else {
			if(!pointGiven)
				delete p;
			return false; } }
	else {
		if(!pointGiven)
			delete p;
		return false; }
}

int Intersects(const Ray3D& r, const Sphere3D& s,
    std::pair<Point3D, Point3D> *p)
{
	float a = Dot(r.direction,  r.direction);

	if(fabs(a) < .0001f)
		return 0;

	Vector3D qVector = r.origin - s.center;
	float b = 2 * Dot(qVector, r.direction);
	float c = Dot(qVector, qVector) - s.radius*s.radius;

	float d = b*b - 4*a*c;
	if(d < 0)
		return 0;
	if(fabs(d) < .0001f) {
		if(p)
			p->first = r.origin + (-b / (2*a)) * r.direction;
		return 1; }
	else {
		if(p) {
			p->first = r.origin + ((-b + sqrt(d)) / (2*a)) * r.direction;
			p->second = r.origin + ((-b - sqrt(d)) / (2*a)) * r.direction; }
		return 2; }
}

bool Intersects(const Ray3D& r, const Triangle3D& t, Point3D *p)
{
	Vector3D triangleNormal = Cross(t[2]-t[0], t[1]-t[0]).normalized();
	Plane3D trianglePlane(triangleNormal[0],
						  triangleNormal[1],
						  triangleNormal[2],
						  -Dot(triangleNormal, t[0]-Point3D(0,0,0)));

	bool pointGiven;
	if(p)
		pointGiven = true;
	else {
		pointGiven = false;
		p = new Point3D; }

	if(Intersects(Line3D(r.origin, r.direction), trianglePlane, p)) {
		if(r.contains(*p)) {
			if(t.contains(*p)) {
				if(!pointGiven)
					delete p;
				return true; }
			else {
				if(!pointGiven)
					delete p;
				return false; } }
		else {
			if(!pointGiven)
				delete p;
			return false; } }
	else {
		if(!pointGiven)
			delete p;
		return false; }
}

int Intersects(const Ray3D& r, const Box3D& b, std::pair<Point3D, Point3D> *p)
{
	std::pair<float, float> xInterval(0,-1), yInterval(0,-1), zInterval(0,-1);

	//get extents
	std::pair<float, float> xRange(b.origin[0], b.origin[0] + b.extent[0]);
	std::pair<float, float> yRange(b.origin[1], b.origin[1] + b.extent[1]);
	std::pair<float, float> zRange(b.origin[2], b.origin[2] + b.extent[2]);

	//find t for when ray passes through x
	if(r.direction[0] != 0) {
		xInterval.first = (xRange.first - r.origin[0]) / r.direction[0];
		if(xInterval.first < 0)
			xInterval.first = 0;
		xInterval.second = (xRange.second - r.origin[0]) / r.direction[0];
		if(xInterval.second < 0)
			xInterval.second = 0;


		if(xInterval.first > xInterval.second)
			std::swap(xInterval.first, xInterval.second); }

	//find t for when ray passes through y
	if(r.direction[1] != 0) {
		yInterval.first = (yRange.first - r.origin[1]) / r.direction[1];
		if(yInterval.first < 0)
			yInterval.first = 0;
		yInterval.second = (yRange.second - r.origin[1]) / r.direction[1];
		if(yInterval.second < 0)
			yInterval.second = 0;

		if(yInterval.first > yInterval.second)
			std::swap(yInterval.first, yInterval.second); }

	//find t for when ray passes through z
	if(r.direction[2] != 0) {
		zInterval.first = (zRange.first - r.origin[2]) / r.direction[2];
		if(zInterval.first < 0)
			zInterval.first = 0;
		zInterval.second = (zRange.second - r.origin[2]) / r.direction[2];
		if(zInterval.second < 0)
			zInterval.second = 0;

		if(zInterval.first > zInterval.second)
			std::swap(zInterval.first, zInterval.second); }

	//get the intersection of the intervals
	std::pair<float, float> finalInterval = xInterval;
	if(finalInterval.first < yInterval.first)
		finalInterval.first = yInterval.first;
	if(finalInterval.second > yInterval.second)
		finalInterval.second = yInterval.second;

	if(finalInterval.first < zInterval.first)
		finalInterval.first = zInterval.first;
	if(finalInterval.second > zInterval.second)
		finalInterval.second = zInterval.second;

	if(finalInterval.first < finalInterval.second) {
		if(p) {
			if(finalInterval.first != 0)
				p->first = r.origin + finalInterval.first * r.direction;
			p->second = r.origin + finalInterval.second * r.direction; }

		if(finalInterval.first = 0)
			return 1;
		else
			return 2; }
	else
		return false;
}

int Intersects(const Triangle3D& t1, const Triangle3D& t2,
    std::pair<Point3D, Point3D> *p)
{
	Point3D* testPt = new Point3D(0,0,0);
	std::vector<Point3D> intersectionPts;
	Point3D zeroPt(0,0,0);

	for(unsigned currPt = 0; currPt < 3; ++currPt) {
		unsigned secondPt = (currPt == 2) ? 0 : currPt+1;
		if(Intersects(LineSeg3D(t1[currPt], t1[secondPt]), t2, testPt))
			intersectionPts.push_back(*testPt);
		*testPt = zeroPt; }

	for(unsigned currPt = 0; currPt < 3; ++currPt) {
		unsigned secondPt = (currPt == 2) ? 0 : currPt+1;
		if(Intersects(LineSeg3D(t2[currPt], t2[secondPt]), t1, testPt))
			intersectionPts.push_back(*testPt);
		*testPt = zeroPt; }

	delete testPt;

	if(intersectionPts.size() > 1) {
		if(p) {
			p->first = intersectionPts[0];
			p->second = intersectionPts[1]; }
		return 2; }
	else if(intersectionPts.size() > 0) {
		if(p)
			p->first = intersectionPts[0];
		return 1; }
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////
// Geometric relationships
////////////////////////////////////////////////////////////////////////

// AngleBetween relationships
float AngleBetween(const Line3D& l1, const Line3D& l2)
{
	Vector3D l1Vector(l1.vector);
	Vector3D l2Vector(l2.vector);

	l1Vector = l1Vector.normalized();
	l2Vector = l2Vector.normalized();

    return acos(Dot(l1Vector, l2Vector));
}

float AngleBetween(const Line3D& l, const Plane3D& p)
{
	Vector3D lVector(l.vector);
	Vector3D pNormal(p[0], p[1], p[2]);

	lVector = lVector.normalized();
	pNormal = pNormal.normalized();

    return acos(Dot(lVector, pNormal));
}

float AngleBetween(const Plane3D& p1, const Plane3D& p2)
{
	Vector3D p1Normal(p1[0], p1[1], p1[2]);
	Vector3D p2Normal(p2[0], p2[1], p2[2]);

	p1Normal = p1Normal.normalized();
	p2Normal = p2Normal.normalized();

    return acos(Dot(p1Normal,p2Normal));
}

// Coplanar relationships
bool Coplanar(const Line3D& l1, const Line3D& l2)
{
	return fabs(Dot(Cross(l1.vector, l2.vector), l2.point - l1.point)) < .0001;
}

// Parallel relationships
bool Parallel(const Line3D& l1, const Line3D& l2)
{
	return !Cross(l1.vector,l2.vector).length();
}

bool Parallel(const Line3D& l, const Plane3D& p)
{
	Vector3D normalVect(p[0], p[1], p[2]);
    return fabs(Dot(normalVect, l.vector)) < .0001f;
}

bool Parallel(const Plane3D& p1, const Plane3D& p2)
{
	Vector3D p1Normal(p1[0], p1[1], p1[2]);
	Vector3D p2Normal(p2[0], p2[1], p2[2]);

    return !Cross(p1Normal, p2Normal).length();
}

// Perpendicular relationships
bool Perpendicular(const Line3D& l1, const Line3D& l2)
{
    bool ret = false;

    const float dot = fabs(Dot(l1.vector, l2.vector));

    if(dot < 1e-6)
        ret = true;

    return ret;
}

bool Perpendicular(const Line3D& l, const Plane3D& p)
{
	Vector3D normalVect(p[0], p[1], p[2]);
    return fabs((l.vector.normalized() - normalVect.normalized()).length()) < .0001f;
}

bool Perpendicular(const Plane3D& p1, const Plane3D& p2)
{
	Vector3D p1Normal(p1[0], p1[1], p1[2]);
	Vector3D p2Normal(p2[0], p2[1], p2[2]);

    return !Dot(p1Normal, p2Normal);
}

////////////////////////////////////////////////////////////////////////
// Arithmetic operations
////////////////////////////////////////////////////////////////////////

// Vector2D operations
bool operator==(const Vector2D& lhs, const Vector2D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector2D& lhs, const Vector2D& rhs)
{
    const bool ret = !(lhs == rhs);
    return ret;
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D ret(lhs);
    ret += rhs;
    return ret;
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector2D operator*(const Vector2D& lhs, const float& rhs)
{
    Vector2D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector2D operator*(const float& lhs, const Vector2D& rhs)
{
    return rhs*lhs;
}

Vector2D operator/(const Vector2D& lhs, const float& rhs)
{
    Vector2D ret(lhs);
    ret /= rhs;
    return ret;
}

float Dot(const Vector2D& lhs, const Vector2D& rhs)
{
    const float ret = std::inner_product(lhs.begin(), lhs.end(), rhs.begin(),
        float(0));
    return ret;
}

// Point2D operations
bool operator==(const Point2D& lhs, const Point2D& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

bool operator!=(const Point2D& lhs, const Point2D& rhs)
{
    return !(lhs == rhs);
}

// Point2D/Vector2D operations
Point2D operator+(const Point2D& lhs, const Vector2D& rhs)
{
    return Point2D(lhs) += rhs;
}

Point2D operator+(const Vector2D& lhs, const Point2D& rhs)
{
    return rhs + lhs;
}

Point2D operator-(const Point2D& lhs, const Vector2D& rhs)
{
    return Point2D(lhs) -= rhs;
}

Vector2D operator-(const Point2D& lhs, const Point2D& rhs)
{
    Vector2D ret;
    std::transform(lhs.begin(), lhs.end(), rhs.begin(), ret.begin(),
        std::minus<float>());
    return ret;
}


// Point3D operations
bool operator==(const Point3D& lhs, const Point3D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Point3D& lhs, const Point3D& rhs)
{
    const bool ret = !(lhs == rhs);
    return ret;
}

Point3D operator+(const Point3D& lhs, const Vector3D& rhs)
{
    Point3D ret(lhs);
    ret += rhs;
    return ret;
}

Point3D operator+(const Vector3D& lhs, const Point3D& rhs)
{
    return rhs + lhs;
}

Point3D operator-(const Point3D& lhs, const Vector3D& rhs)
{
    Point3D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector3D operator-(const Point3D& lhs, const Point3D& rhs)
{
    return Vector3D(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
}


// Vector3D operations
bool operator==(const Vector3D& lhs, const Vector3D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector3D& lhs, const Vector3D& rhs)
{
    const bool ret = !(lhs == rhs);
    return ret;
}

Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D ret(lhs);
    ret += rhs;
    return ret;
}

Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector3D operator*(const Vector3D& lhs, const float& rhs)
{
    Vector3D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector3D operator*(const float& lhs, const Vector3D& rhs)
{
    return rhs*lhs;
}

Vector3D operator/(const Vector3D& lhs, const float& rhs)
{
    Vector3D ret(lhs);
    ret /= rhs;
    return ret;
}

float Dot(const Vector3D& lhs, const Vector3D& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}


// Vector4D operations
bool operator==(const Vector4D& lhs, const Vector4D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector4D& lhs, const Vector4D& rhs)
{
    const bool ret = !(lhs == rhs);

    return ret;
}

Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs)
{
    Vector4D ret(lhs);
    ret += rhs;
    return ret;
}

Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs)
{
    Vector4D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector4D operator*(const Vector4D& lhs, const float& rhs)
{
    Vector4D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector4D operator/(const Vector4D& lhs, const float& rhs)
{
    Vector4D ret(lhs);
    ret /= rhs;
    return ret;
}

Vector4D operator*(const float& lhs, const Vector4D& rhs)
{
    return rhs*lhs;
}

float Dot(const Vector4D& lhs, const Vector4D& rhs)
{
    const float ret = std::inner_product(lhs.begin(), lhs.end(), rhs.begin(),
        float(0));
    return ret;
}


// Matrix4x4 operations
Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 ret(lhs);
    ret *= rhs;
    return ret;
}

Vector4D operator*(const Matrix4x4& lhs, const Vector3D& rhs)
{
    Vector4D temp(rhs[X], rhs[Y], rhs[Z], 0);
    temp = lhs*temp;
    return temp;
}

Vector4D operator*(const Matrix4x4& lhs, const Point3D& rhs)
{
    Vector4D temp(rhs[X], rhs[Y], rhs[Z], 1);
    temp = lhs*temp;
    return temp;
}

Vector4D operator*(const Matrix4x4& lhs, const Vector4D& rhs)
{
    Vector4D ret;
	ret[0] = Dot(lhs[0], rhs);
	ret[1] = Dot(lhs[1], rhs);
	ret[2] = Dot(lhs[2], rhs);
	ret[3] = Dot(lhs[3], rhs);

    return ret;
}

Matrix4x4 LookAt(const Point3D& C, const Point3D& D, const Vector3D& U)
{
	//look z, y is up, x to the right

	Vector3D target = (D - C).normalized();
	Vector3D X = Cross(target, U).normalized();
	Vector3D newU = Cross(X, target);
	Vector3D CV = C - Point3D(0,0,0);

	Matrix4x4 ret;

	ret[0] = Vector4D(X[0], X[1], X[2], -Dot(X, CV));
	ret[1] = Vector4D(newU[0], newU[1], newU[2], -Dot(newU, CV));
	ret[2] = Vector4D(-target[0], -target[1], -target[2], Dot(target, CV));
	ret[3] = Vector4D(0        , 0       , 0   , 1       );

	return ret;
}

Matrix4x4 Perspective(const float w, const float h,
					  const float d, const float f)
{
    Matrix4x4 ret;

	ret[0] = Vector4D(d/w,   0,       0,          0);
	ret[1] = Vector4D(  0, d/h,       0,          0);
	ret[2] = Vector4D(  0,   0, f/(f-d), -d*f/(f-d));
	ret[3] = Vector4D(  0,   0,       1,          0);

    return ret;
}
