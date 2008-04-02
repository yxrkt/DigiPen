///////////////////////////////////////////////////////////////////////
// $Id: scenelib.h 1145 2008-01-13 04:04:25Z gherron $
//
// Operations for building a scene full of objects, their geometry,
// colors, textures and such.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

using namespace std;
#include <assert.h>

////////////////////////////////////////////////////////////////////////
// A texture map has height, width, and channel sizes and a large
// float array.
class Texture {
public:
	int width;					// Width of texel array
	int height;					// Height of texel array
	int channels;				// Number of colors/values per texel

	float* texels;				// Array of width*height*channels floats
	void* processed;			// Unused -- for student use

	// Use this method to access a single float value in the Texture.
	// Parameters (i,j) index a texel, parameter k indexes a single
	// channel of that texel.
	float* texel(int i, int j, int k);
	// Acces hints:
	//   texture.texel(i,j,0) -- pointer to red channel value
	//   texture.texel(i,j,1) -- pointer to green channel value
	//   texture.texel(i,j,2) -- pointer to blue channel value
    //
	//   texture.texel(i,j,0) -- pointer to RGB triple
	//
	//   texture.texel(i,j,0) -- pointer to bump map single height value

	// Constructor and setters used by the framework to build a texture
	Texture(int w, int h, int c);
	void set(int i, int j, Color& c);
	void set(int i, int j, float v);

};

////////////////////////////////////////////////////////////////////////
// Various specializations of Texture for specific uses.
class ReflTexture: public Texture
{
public:
	ReflTexture();
};

class TileTexture: public Texture
{
public:
	TileTexture(int w, int h);
};

class TileBump: public Texture
{
public:
	double s(int k) { return pow(double(k), 0.25); }
	TileBump(int w, int h);
};

class WoodTexture: public Texture
{
public:
	WoodTexture(int w, int h);
};


////////////////////////////////////////////////////////////////////////
// Class Vertex reperesents one vertex on a polygon.
class Vertex
{
public:
	Vector4D V;					// Vertex's position
	Vector3D N;					// Vertex's normal
	float u,v;					// Vertex's texture coordinates
	int prevIndex;				// Index of edge's other end vertex
};

////////////////////////////////////////////////////////////////////////
// An APolygon is a list (of type std::vector<>) of vertex's (Named
// APolygon because MS compilers choke on things named Polygon).
typedef std::vector<Vertex> APolygon;

////////////////////////////////////////////////////////////////////////
// An Object contains a list of polygons and color and texture info
class Object {
public:
	std::vector<APolygon> polygons;	// List of polygons defining this object.
	Color Kd;					// Object's diffuse color
	Color Ks;					// Object's specular color
	int n;						// Object's Phong shininess exponent

	Texture* texture;			// Texture map -- may be NULL
	Texture* bump;				// Bump height map -- may be NULL
	Texture* refl;				// Reflection map -- may be NULL

	bool closed;				// Object is back-face cullable.
};

////////////////////////////////////////////////////////////////////////
// A light contains a Vector3D position and a Color.
class Light {
public:
	Point3D position;
	Color color;
};

////////////////////////////////////////////////////////////////////////
// A LightList is a list (of type std::vector<>) of Light's
typedef std::vector<Light> LightList;

////////////////////////////////////////////////////////////////////////
// Class Scene represents a whole scene as a list of polygons, and
// contains several state variables and methods used to build up a
// scene.
class Scene
{
public:
	bool EnableLighting;		// Compute (or not) Phong lighting
	bool EnableFlatShading;		// Do (or not) normal interpolation
	bool UseOpenGLRendering;	// Toggled via the 'g' key.
	bool UseFastRendering;		// Toggled when doing an interactive transform
	bool UseAntiAliasing;		// Toggled via the 'a' key.

	Color ambient;				// The scene's ambient color
	LightList lights;			// The scene's list of lights
	std::vector<Object> objects; // The scene's list of Objects
	Transformation viewing;		// The interactive viewing transformation
	Transformation projection;	// The interactive perspective transformation

	// The following parameters and methods are used by the scene
	// building code.  By the time the framework is running these
	// parameters have no useful values, and the methods will no
	// longer be called.  Just ignore them!
	Transformation modeling;
	Vector3D currentNormal;
	Color currentKd;
	Color currentKs;
	int currentExpn;
	int currentStartIndex;
	Texture* currentTexture;
	Texture* currentBump;
	Texture* currentRefl;
	float currentU, currentV;

	Scene();
	void Clear();
	void SetAmbient(const Color);
	void AddLight(const Point3D, const Color);
	void NewObject(bool closed=false);
	void NewPolygon();
	void Hole();
	void Vertex(Point3D);
	void SetNormal(Vector3D);
	void SetColor(Color Kd, Color Ks, int n);
	void SetTexture(Texture* texture, Texture* bump=NULL, Texture* refl=NULL);
	void SetTexCoord(float u, float v);
};

void CreateRectangularPrism(Scene& scene, double l, double w, double h);
void CreateCone(Scene& scene, double r, double l, double count);
void CreateCylinder(Scene& scene, double r, double l, double count);
void CreateSphereOctant(Scene& scene, double r, double count);
void CreateSphere(Scene& scene, double r, double count,
				  float theta=2*PI, float phi=PI);
void CreateLetterA(Scene& scene);
void Reflector(Scene& scene, int n, int m);
void CreateRect(Scene& scene, double l, double w);

////////////////////////////////////////////////////////////////////////
// The framework will call CreateScene once during startup to create
// the scene as a list of polygons.  It will then call PreprocessScene
// once to allow the user to execute any scene preprocessing code.
//
// Note: Each class project will come with a project specific version
// of CreateScene to create a scene appropriate to the project.
//
// Note: Each class project will also come with a scene specific
// procedure to draw the scene using OpenGL called
// DrawSceneWithOpenGL.  Student code can toggle between their own
// rendering code and DrawSceneWithOpenGL for comparisson purposes.
//
void CreateScene(Scene& scene);
void PreprocessScene(Scene& scene);
void DrawMessage(char* msg, int width, int height);
void DrawSceneWithOpenGL(Scene& scene, int width, int height);

////////////////////////////////////////////////////////////////////////
// The framework will call DrawScene whenever the scene needs to be
// redrawn, because either the window has been exposed, or a
// mouse/keyboard interaction requires it.
//
// Note:  Most student code will be implemented in DrawScene.
//

extern int sceneID;

void DrawScene(Scene& scene, int width, int height);
