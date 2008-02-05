///////////////////////////////////////////////////////////////////////
// $Id: geomlib.cpp 1147 2008-01-13 18:40:16Z gherron $
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
    if(!len)
      throw ZeroDivide();
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
        ret[1] = v1[2]*v2[0] - v1[0]*v2[2];
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

bool Coplanar(const Point3D& A,const Point3D& B,
			  const Point3D& C, const Point3D& D)
{
	// Generated with maxima as the determinant of the 4x4:
	// | A[0] A[1] A[2] 1 |
    // |                  |
	// | B[0] B[1] B[2] 1 |
    // |                  |
	// | C[0] C[1] C[2] 1 |
    // |                  |
	// | D[0] D[1] D[2] 1 |
	float det = 
		- B[0]*C[1]*D[2] + A[0]*C[1]*D[2] + B[1]*C[0]*D[2] - A[1]*C[0]*D[2] 
		- A[0]*B[1]*D[2] + A[1]*B[0]*D[2] + B[0]*C[2]*D[1] - A[0]*C[2]*D[1] 
		- B[2]*C[0]*D[1] + A[2]*C[0]*D[1] + A[0]*B[2]*D[1] - A[2]*B[0]*D[1] 
		- B[1]*C[2]*D[0] + A[1]*C[2]*D[0] + B[2]*C[1]*D[0] - A[2]*C[1]*D[0] 
		- A[1]*B[2]*D[0] + A[2]*B[1]*D[0] + A[0]*B[1]*C[2] - A[1]*B[0]*C[2] 
		- A[0]*B[2]*C[1] + A[2]*B[0]*C[1] + A[1]*B[2]*C[0] - A[2]*B[1]*C[0];

	return fabs(det) < 1.0e-3;
}
