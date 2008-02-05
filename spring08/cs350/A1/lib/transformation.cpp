///////////////////////////////////////////////////////////////////////
// $Id: transformation.cpp 1140 2008-01-11 23:49:58Z gherron $
//
// Operations for manipulating transformations
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"

Transformation::Transformation()
{
	curr = Matrix4x4();
	matrixStack.clear();
}

void Transformation::Push()
{
	matrixStack.push_back(curr);
}

void Transformation::Pop()
{
	curr = matrixStack.back();
	matrixStack.pop_back();
}

void Transformation::Identity()
{
	curr = Matrix4x4();
}

void Transformation::RotateX(float theta)
{
	theta = theta * PI / 180.0f;
	Matrix4x4 rotMatrix;
	rotMatrix[1][1] = cos(theta);
	rotMatrix[1][2] = -sin(theta);
	rotMatrix[2][1] = sin(theta);
	rotMatrix[2][2] = cos(theta);
	curr = curr * rotMatrix;
}

void Transformation::RotateY(float theta)
{
	theta = theta * PI / 180.0f;
	Matrix4x4 rotMatrix;
	rotMatrix[2][2] = cos(theta);
	rotMatrix[2][0] = -sin(theta);
	rotMatrix[0][2] = sin(theta);
	rotMatrix[0][0] = cos(theta);
	curr = curr * rotMatrix;
}

void Transformation::RotateZ(float theta)
{

	theta = theta * PI / 180.0f;
	Matrix4x4 rotMatrix;
	rotMatrix[0][0] = cos(theta);
	rotMatrix[0][1] = -sin(theta);
	rotMatrix[1][0] = sin(theta);
	rotMatrix[1][1] = cos(theta);
	curr = curr * rotMatrix;
}

void Transformation::Scale(float sx, float sy, float sz)
{
	Matrix4x4 scaleMatrix;
	scaleMatrix[0][0] = sx;
	scaleMatrix[1][1] = sy;
	scaleMatrix[2][2] = sz;
	curr =  curr * scaleMatrix;
}

void Transformation::Translate(float tx, float ty, float tz)
{
	Matrix4x4 transMatrix;
	transMatrix[0][3] = tx;
	transMatrix[1][3] = ty;
	transMatrix[2][3] = tz;
	curr =  curr * transMatrix;
}

void Transformation::Perspective(float sx, float sy, float front, float back)
{

	Matrix4x4 perspectiveMatrix;
	perspectiveMatrix[0][0] = 1 / sx;
	perspectiveMatrix[1][1] = 1 / sy;
	perspectiveMatrix[2][2] = - (back + front) / (back - front);
	perspectiveMatrix[2][3] = - 2 * back * front / (back - front);
	perspectiveMatrix[3][2] = -1;
	perspectiveMatrix[3][3] = 0;
	curr = curr * perspectiveMatrix;
}

Vector4D Transformation::InverseTransform(const Vector4D& V)
{
	return Vector4D(
	 inv[0][0]*V[0] + inv[0][1]*V[1] + inv[0][2]*V[2] + inv[0][3]*V[3],
	 inv[1][0]*V[0] + inv[1][1]*V[1] + inv[1][2]*V[2] + inv[1][3]*V[3],
	 inv[2][0]*V[0] + inv[2][1]*V[1] + inv[2][2]*V[2] + inv[2][3]*V[3],
	 inv[3][0]*V[0] + inv[3][1]*V[1] + inv[3][2]*V[2] + inv[3][3]*V[3]
	);
}

Vector4D Transformation::Transform(Vector4D& V)
{
	return Vector4D(
	 curr[0][0]*V[0] + curr[0][1]*V[1] + curr[0][2]*V[2] + curr[0][3]*V[3],
	 curr[1][0]*V[0] + curr[1][1]*V[1] + curr[1][2]*V[2] + curr[1][3]*V[3],
	 curr[2][0]*V[0] + curr[2][1]*V[1] + curr[2][2]*V[2] + curr[2][3]*V[3],
	 curr[3][0]*V[0] + curr[3][1]*V[1] + curr[3][2]*V[2] + curr[3][3]*V[3]
	);
}

Vector4D Transformation::InverseTransform(Vector4D& V)
{
	return Vector4D(
	 inv[0][0]*V[0] + inv[0][1]*V[1] + inv[0][2]*V[2] + inv[0][3]*V[3],
	 inv[1][0]*V[0] + inv[1][1]*V[1] + inv[1][2]*V[2] + inv[1][3]*V[3],
	 inv[2][0]*V[0] + inv[2][1]*V[1] + inv[2][2]*V[2] + inv[2][3]*V[3],
	 inv[3][0]*V[0] + inv[3][1]*V[1] + inv[3][2]*V[2] + inv[3][3]*V[3]
	);
}

Vector4D Transformation::Transform(const Vector4D& V)
{
	return Vector4D(
	 curr[0][0]*V[0] + curr[0][1]*V[1] + curr[0][2]*V[2] + curr[0][3]*V[3],
	 curr[1][0]*V[0] + curr[1][1]*V[1] + curr[1][2]*V[2] + curr[1][3]*V[3],
	 curr[2][0]*V[0] + curr[2][1]*V[1] + curr[2][2]*V[2] + curr[2][3]*V[3],
	 curr[3][0]*V[0] + curr[3][1]*V[1] + curr[3][2]*V[2] + curr[3][3]*V[3]
	);
}

void Transformation::ArrayForOpenGL(float* array16)
{
	for (int i=0;  i<4;  i++)
		for (int j=0;  j<4;  j++)
			*(array16 + i+4*j) = curr[i][j];
}

////////////////////////////////////////////////////////////////////////
// Calculates the inverse matrix by performing the gaussian matrix
// reduction with partial pivoting followed by back/substitution with
// the loops manually unrolled.
//
// Taken from Mesa implementation of OpenGL:  http://mesa3d.sourceforge.net/
////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) ((*m)[r][c])
#define SWAP_ROWS(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }

int invert(Matrix4x4* mat, Matrix4x4* inv)
{
   double wtmp[4][8];
   double m0, m1, m2, m3, s;
   double *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(mat,0,0);
   r0[1] = MAT(mat,0,1);
   r0[2] = MAT(mat,0,2);
   r0[3] = MAT(mat,0,3);
   r0[4] = 1.0;
   r0[5] = r0[6] = r0[7] = 0.0;
   
   r1[0] = MAT(mat,1,0);
   r1[1] = MAT(mat,1,1);
   r1[2] = MAT(mat,1,2);
   r1[3] = MAT(mat,1,3);
   r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;

   r2[0] = MAT(mat,2,0);
   r2[1] = MAT(mat,2,1);
   r2[2] = MAT(mat,2,2);
   r2[3] = MAT(mat,2,3);
   r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;

   r3[0] = MAT(mat,3,0);
   r3[1] = MAT(mat,3,1);
   r3[2] = MAT(mat,3,2);
   r3[3] = MAT(mat,3,3);
   r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
   if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
   if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
   if (0.0 == r0[0])  return 0;

   /* eliminate first variable     */
   m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
   s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
   s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
   s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r0[5];
   if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r0[6];
   if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r0[7];
   if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
   if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
   if (0.0 == r1[1])  return 0;

   /* eliminate second variable */
   m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
   r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
   s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
   if (0.0 == r2[2])  return 0;

   /* eliminate third variable */
   m3 = r3[2]/r2[2];
   r3[3] -= m3 * r2[3];
   r3[4] -= m3 * r2[4];
   r3[5] -= m3 * r2[5];
   r3[6] -= m3 * r2[6];
   r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3]) return 0;

   s = 1.0F/r3[3];             /* now back substitute row 3 */
   r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

   m2 = r2[3];                 /* now back substitute row 2 */
   s  = 1.0F/r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2);
   r2[5] = s * (r2[5] - r3[5] * m2);
   r2[6] = s * (r2[6] - r3[6] * m2);
   r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1;
   r1[5] -= r3[5] * m1;
   r1[6] -= r3[6] * m1;
   r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0;
   r0[5] -= r3[5] * m0;
   r0[6] -= r3[6] * m0;
   r0[7] -= r3[7] * m0;

   m1 = r1[2];                 /* now back substitute row 1 */
   s  = 1.0F/r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1);
   r1[5] = s * (r1[5] - r2[5] * m1);
   r1[6] = s * (r1[6] - r2[6] * m1);
   r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0;
   r0[5] -= r2[5] * m0;
   r0[6] -= r2[6] * m0;
   r0[7] -= r2[7] * m0;

   m0 = r0[1];                 /* now back substitute row 0 */
   s  = 1.0F/r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0);
   r0[5] = s * (r0[5] - r1[5] * m0);
   r0[6] = s * (r0[6] - r1[6] * m0);
   r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(inv,0,0) = r0[4];
   MAT(inv,0,1) = r0[5],
   MAT(inv,0,2) = r0[6];
   MAT(inv,0,3) = r0[7],
   MAT(inv,1,0) = r1[4];
   MAT(inv,1,1) = r1[5],
   MAT(inv,1,2) = r1[6];
   MAT(inv,1,3) = r1[7],
   MAT(inv,2,0) = r2[4];
   MAT(inv,2,1) = r2[5],
   MAT(inv,2,2) = r2[6];
   MAT(inv,2,3) = r2[7],
   MAT(inv,3,0) = r3[4];
   MAT(inv,3,1) = r3[5],
   MAT(inv,3,2) = r3[6];
   MAT(inv,3,3) = r3[7];

   return 1;
}
