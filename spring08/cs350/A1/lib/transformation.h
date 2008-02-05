///////////////////////////////////////////////////////////////////////
// $Id: scenelib.h 1016 2007-06-12 17:07:55Z gherron $
//
// Operations for manipulating transformations
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

int invert(Matrix4x4* mat, Matrix4x4* inv);

////////////////////////////////////////////////////////////////////////
// Class Transformation implements a Matrix4x4 along with the usual
// methods to build a transformation from primitives, plus the ability
// to push and pop the matrix onto a stack.
class Transformation
{
public:
	Transformation();			// Constructor -- yields an identity

	// Standard graphics pipeline matrix stack operations
	void Push();
	void Pop();

	// Standard graphics pipeline operations to build the transformation.
	void Identity();
	void RotateX(float theta);
	void RotateY(float theta);
	void RotateZ(float theta);
	void Scale(float sx, float sy, float sz);
	void Translate(float tx, float ty, float tz);
	void Perspective(float sx, float sy, float front, float back);

	// Called to compute the inverse transformation when the foreard
	// transformation is finsihed being built.
	void ComputeInverse() { invert(&curr, &inv); }

	// Methods to (forward) transform a point.
	Vector4D Transform(Vector4D& V);
	Vector4D Transform(const Vector4D& V);
	
	// Methods to back-transform a point.
	Vector4D InverseTransform(Vector4D& V);
	Vector4D InverseTransform(const Vector4D& V);

	// Get matrix as a float* for passing to OpenGL
	void ArrayForOpenGL(float* array16);

private:
	Matrix4x4 curr;				// The current transformation
	Matrix4x4 inv;				// Its inverse
	std::vector<Matrix4x4> matrixStack;	// A stack for Push/Pop methods.
	
};
