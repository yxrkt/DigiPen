///////////////////////////////////////////////////////////////////////
// $Id: scenelib.cpp 1016 2007-06-12 17:07:55Z gherron $
//
// Operations for building a scene full of objects, their geometry,
// colors, textures and such.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <algorithm>
using namespace std;

#include "math.h"
#include "framework.h"
#include "scenelib.h"

#if defined(MS_WINDOWS)
#include <io.h>
#endif

////////////////////////////////////////////////////////////////////////
//
// Some noise computations copied directly from some Perlin based web site
//
////////////////////////////////////////////////////////////////////////

#define B 0x100
#define BM 0xff
#define NC 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i,b0,b1,r0,r1)\
        t = vec[i] + NC;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

void noiseinit(void);
double noise1(double);
double noise2(double *);
double noise3(double *);
void normalize3(double *);
void normalize2(double *);
static int p[B + B + 2];
static double g3[B + B + 2][3];
static double g2[B + B + 2][2];
static double g1[B + B + 2];
static int start = 1;

double noise1(double arg)
{
   int bx0, bx1;
   double rx0, rx1, sx, t, u, v, vec[1];

   vec[0] = arg;
   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0,bx0,bx1,rx0,rx1);

   sx = s_curve(rx0);
   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return(lerp(sx, u, v));
}

double noise2(double vec[2])
{
   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);
   sy = s_curve(ry0);

   q = g2[ b00 ] ; u = at2(rx0,ry0);
   q = g2[ b10 ] ; v = at2(rx1,ry0);
   a = lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1);
   q = g2[ b11 ] ; v = at2(rx1,ry1);
   b = lerp(sx, u, v);

   return lerp(sy, a, b);
}

double noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      noiseinit();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);
   setup(2, bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = lerp(t, u, v);

   c = lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = lerp(t, u, v);

   d = lerp(sy, a, b);

   return lerp(sz, c, d);
}

void normalize2(double v[2])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
}

void normalize3(double v[3])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
   v[2] = v[2] / s;
}

void noiseinit(void)
{
   int i, j, k;

   for (i = 0 ; i < B ; i++) {
      p[i] = i;
      g1[i] = (double)((rand() % (B + B)) - B) / B;

      for (j = 0 ; j < 2 ; j++)
         g2[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize2(g2[i]);

      for (j = 0 ; j < 3 ; j++)
         g3[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = rand() % B];
      p[j] = k;
   }

   for (i = 0 ; i < B + 2 ; i++) {
      p[B + i] = p[i];
      g1[B + i] = g1[i];
      for (j = 0 ; j < 2 ; j++)
         g2[B + i][j] = g2[i][j];
      for (j = 0 ; j < 3 ; j++)
         g3[B + i][j] = g3[i][j];
   }
}



////////////////////////////////////////////////////////////////////////
//
// End of Perlin based web site stuff
//
////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	Clear();
}

void Scene::Clear()
{
	EnableLighting = false;
	EnableFlatShading = false;
	UseOpenGLRendering = true;
	UseFastRendering = false;
	UseAntiAliasing = true;

	objects.resize(0);
	lights.resize(0);
	
	currentNormal = Vector3D(0.0, 0.0, 1.0);
	currentKd = Color(1.0, 1.0, 1.0);
	currentKs = Color(0.0, 0.0, 0.0);
	currentExpn = 0;
	currentStartIndex = 0;
	currentTexture = NULL;
	currentBump = NULL;
	currentRefl = NULL;
	currentU = currentV = 0.0;
}

void Scene::SetAmbient(const Color color)
{
	ambient = color;
}

void Scene::AddLight(const Point3D position, const Color color)
{
	int nLights = lights.size();
	lights.resize(nLights+1);
	lights[nLights].position = position;
	lights[nLights].color = color;
}


void Scene::SetColor(Color Kd, Color Ks, int n)
{
	currentKd = Kd;
	currentKs = Ks;
	currentExpn = n;
}

void Scene::NewObject(bool closed)
{
	int nObjs = objects.size();
	objects.resize(nObjs+1);
	objects[nObjs].closed = closed;
	objects[nObjs].polygons.clear();
	objects[nObjs].Kd = currentKd;
	objects[nObjs].Ks = currentKs;
	objects[nObjs].n = currentExpn;
	objects[nObjs].texture = currentTexture;
	objects[nObjs].bump = currentBump;
	objects[nObjs].refl = currentRefl;
	currentStartIndex = 0;
}

void Scene::NewPolygon()
{
	int nObjs = objects.size();
	Object& object = objects[nObjs-1];
	int nPoly = object.polygons.size();
	object.polygons.resize(nPoly+1);
	currentStartIndex = 0;
}

void Scene::Hole()
{
	int nObjs = objects.size();
	Object& object = objects[nObjs-1];
	int nPoly = object.polygons.size();
	currentStartIndex = object.polygons[nPoly-1].size();
}

void Scene::SetNormal(Vector3D N)
{
	currentNormal = N;
	currentNormal.normalize();
}

void Scene::SetTexture(Texture* texture, Texture* bump, Texture* refl)
{
	currentTexture = texture;
	currentBump = bump;
	currentRefl = refl;
}

void Scene::SetTexCoord(float u, float v)
{
	currentU = u;
	currentV = v;
}

void Scene::Vertex(Point3D P)
{
	Vector4D V4 = modeling.Transform(P);
	Vector4D N(currentNormal[0], currentNormal[1], currentNormal[2], 0.0);
	Vector4D N4 = modeling.Transform(N);

	int nObjs = objects.size();
	Object& object = objects[nObjs-1];
	int nPoly = object.polygons.size();
	int lPoly = object.polygons[nPoly-1].size();

	object.polygons[nPoly-1].resize(lPoly+1);
	object.polygons[nPoly-1][lPoly].V = V4;
	object.polygons[nPoly-1][lPoly].N = Vector3D(N4[0],N4[1],N4[2]);
	object.polygons[nPoly-1][lPoly].u = currentU;
	object.polygons[nPoly-1][lPoly].v = currentV;
	object.polygons[nPoly-1][lPoly].prevIndex = lPoly-1;

	object.polygons[nPoly-1][currentStartIndex].prevIndex = lPoly;
}

////////////////////////////////////////////////////////////////////////
// Various texture building methods
////////////////////////////////////////////////////////////////////////
Texture::Texture(int w, int h, int c) :
	width(w), height(h), channels(c), processed(NULL)
{
	texels = new float [w*h*c];
}

float* Texture::texel(int i, int j, int k)
{
	return texels + (channels*(width*j + i) + k);
}

void Texture::set(int i, int j, Color& c)
{
	assert (channels==3);
	float* ptr = texel(i,j,0);
	*ptr++ = c.rgba[0];
	*ptr++ = c.rgba[1];
	*ptr++ = c.rgba[2]; 
}

void Texture::set(int i, int j, float v)
{
	assert (channels==1);
	float* ptr = texel(i,j,0);
	*ptr = v;
}


ReflTexture::ReflTexture(): Texture(256, 256, 3)
{
	FILE* f = fopen("sphere.raw", "rb");
	unsigned char bytes[256*256*3];
	fread(bytes, 256*256, 3, f);
	fclose(f);

	float* ptr = texel(0,0,0);
	unsigned char* src = bytes;
	for (int i=0;  i<256;  i++)
		for (int j=0;  j<256;  j++)
			for (int k=0;  k<3;  k++)
				*ptr++ = *src++ /255.0f;
}

TileTexture::TileTexture(int w, int h): Texture(w, h, 3)
{
	Color ceramic = HSVColor(0.083, 0.75, 0.8);
	Color grout   = HSVColor(0.125, 0.1, 0.8);
	
	int n = w/4;
	int b = 1;
	for (int i=0;  i<w;  i++)
		for (int j=0;  j<h;  j++) {
			if (i%n>b && i%n<n-1-b && j%n>b && j%n<n-1-b) {
				set(i,j,ceramic); }
			else
				set(i,j,grout); }
}

TileBump::TileBump(int w, int h): Texture(w, h, 1)
{
	int n = w/4;
	int b = 1;
	float c = 30.495901;
	for (int i=0;  i<w;  i++)
		for (int j=0;  j<h;  j++) {
			if (i%n>b && i%n<n-1-b && j%n>b && j%n<n-1-b) {
				int dx = min(i%n-b, n-1-b-i%n);
				int dy = min(j%n-b, n-1-b-j%n);
				float d = min(dx,dy);
				float v = s(i%n-b)*s(n-1-b-i%n)*s(j%n-b)*s(n-1-b-j%n)/c;
				set(i,j,v); }
			else
				set(i,j,0.0); }
}

WoodTexture::WoodTexture(int w, int h): Texture(w, h, 3)
{
	Color light = HSVColor(0.125, 0.7, 0.8);
	Color dark  = HSVColor(0.125, 0.7, 0.6);
	Vector3D C(0.0, 0.0, 1.0);
	Vector3D D(0.0, 1.0, 0.1);
	D.normalize();
	
	for (int i=0;  i<w;  i++) {
		float u = (2.0*i)/w - 1.0;
		for (int j=0;  j<h;  j++) {
			float v = (2.0*j)/h - 1.0;
			Vector3D P = Vector3D(u,v,0.0)-C;
			float d = Dot(P,P) - Dot(P,D)*Dot(P,D);
			d *= 10;
			float f = d - floor(d);
			if (f < 0.7)
				set(i,j,light);
			else
				set(i,j,dark); } }
}

////////////////////////////////////////////////////////////////////////
// Various geometry creation routines
////////////////////////////////////////////////////////////////////////

void CreateRect(Scene& scene, double l, double w)
{
	scene.NewObject();

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0,0,1));

	scene.SetTexCoord(0.0, 0.0);
	scene.Vertex(Point3D(-l/2, -w/2,  0.0));

	scene.SetTexCoord(1.0, 0.0);
	scene.Vertex(Point3D( l/2, -w/2,  0.0));

	scene.SetTexCoord(1.0, 1.0);
	scene.Vertex(Point3D( l/2,  w/2,  0.0));

	scene.SetTexCoord(0.0, 1.0);
	scene.Vertex(Point3D(-l/2,  w/2,  0.0));

}

void TorusGeometry(Scene& scene, float u, float v)
{
	float r2 = 0.4;
	float r1 = 1.0-r2;
	float theta = PI*2.0*u;
	float phi = PI*2.0*v;

	float s1 = sin(theta);
	float c1 = cos(theta);
	float s2 = r2*sin(phi);
	float c2 = r2*cos(phi);

	scene.SetNormal(Vector3D(s2*s1,s2*c1,c2));
	scene.Vertex(Point3D(r1*s1+s2*s1, r1*c1+s2*c1, c2));
}

void Reflector(Scene& scene, int n, int m)
{
	scene.NewObject(true);

	for (int i=0; i<n;  i++) {
		float u0 = float(i)/n;
		float u1 = float(i+1)/n;

		for (int j=0; j<m;  j++) {
			float v0 = float(j)/m;
			float v1 = float(j+1)/m;

			scene.NewPolygon();

			scene.SetTexCoord(u0,v0);
			TorusGeometry(scene, u0,v0);

			scene.SetTexCoord(u0,v1);
			TorusGeometry(scene, u0,v1);

			scene.SetTexCoord(u1,v1);
			TorusGeometry(scene, u1,v1);

			scene.SetTexCoord(u1,v0);
			TorusGeometry(scene, u1,v0);
		}
	}
}

void CreateRectangularPrism(Scene& scene,
							double l, double w, double h)
{
	scene.NewObject();

	scene.NewPolygon();
	scene.SetNormal(Vector3D(-1,0,0));
	scene.Vertex(Point3D(-l/2,  w/2, -h/2));
	scene.Vertex(Point3D(-l/2, -w/2, -h/2));
	scene.Vertex(Point3D(-l/2, -w/2,  h/2));
	scene.Vertex(Point3D(-l/2,  w/2,  h/2));

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0,-1,0));
	scene.Vertex(Point3D(-l/2, -w/2,  h/2));
	scene.Vertex(Point3D(-l/2, -w/2, -h/2));
	scene.Vertex(Point3D( l/2, -w/2, -h/2));
	scene.Vertex(Point3D( l/2, -w/2,  h/2));

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0,0,-1));
	scene.Vertex(Point3D( l/2, -w/2, -h/2));
	scene.Vertex(Point3D(-l/2, -w/2, -h/2));
	scene.Vertex(Point3D(-l/2,  w/2, -h/2));
	scene.Vertex(Point3D( l/2,  w/2, -h/2));

	scene.NewPolygon();
	scene.SetNormal(Vector3D(1,0,0));
	scene.Vertex(Point3D( l/2, -w/2, -h/2));
	scene.Vertex(Point3D( l/2,  w/2, -h/2));
	scene.Vertex(Point3D( l/2,  w/2,  h/2));
	scene.Vertex(Point3D( l/2, -w/2,  h/2));

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0,1,0));
	scene.Vertex(Point3D(-l/2,  w/2, -h/2));
	scene.Vertex(Point3D(-l/2,  w/2,  h/2));
	scene.Vertex(Point3D( l/2,  w/2,  h/2));
	scene.Vertex(Point3D( l/2,  w/2, -h/2));

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0,0,1));
	scene.Vertex(Point3D(-l/2, -w/2,  h/2));
	scene.Vertex(Point3D( l/2, -w/2,  h/2));
	scene.Vertex(Point3D( l/2,  w/2,  h/2));
	scene.Vertex(Point3D(-l/2,  w/2,  h/2));

}

void CreateCone(Scene& scene, double r, double l, double count)
{
	scene.NewObject();

	for(int i = 0; i < count;  i++)  {
		float theta0 = (i*2.0*PI)/count;
		float thetah = ((i + 0.5)*2.0*PI)/count;
		float theta1 = ((i + 1)*2.0*PI)/count;

		scene.NewPolygon();

		scene.SetNormal(Vector3D(r*cos(thetah), r*sin(thetah), l));
		scene.Vertex(Point3D(0.0, 0.0, l/2));
		scene.SetNormal(Vector3D(r*cos(theta0), r*sin(theta0), l));
		scene.Vertex(Point3D(r*cos(theta0), r*sin(theta0), -l/2));
		scene.SetNormal(Vector3D(r*cos(theta1), r*sin(theta1), l));
		scene.Vertex(Point3D(r*cos(theta1), r*sin(theta1), -l/2));

		scene.NewPolygon();

		scene.SetNormal(Vector3D(0.0, 0.0, -1.0));
		scene.Vertex(Point3D(0.0, 0.0, -l/2));
		scene.Vertex(Point3D(r*cos(theta1), r*sin(theta1), -l/2)); 
		scene.Vertex(Point3D(r*cos(theta0), r*sin(theta0), -l/2));
	}
}

void CreateCylinder(Scene& scene,
					double r, double l, double count)
{
	scene.NewObject();

	for(int i = 0; i < count;  i++)  {
		float theta0 = (i*2.0*PI)/count;
		float theta1 = ((i + 1)*2.0*PI)/count;

		scene.NewPolygon();

		scene.SetNormal(Vector3D(cos(theta0), sin(theta0), 0.0));
		scene.Vertex(Point3D(r*cos(theta0), r*sin(theta0), -l/2));
		scene.Vertex(Point3D(r*cos(theta0), r*sin(theta0),  l/2));

		scene.SetNormal(Vector3D(cos(theta1), sin(theta1), 0.0));
		scene.Vertex(Point3D(r*cos(theta1), r*sin(theta1),  l/2));
		scene.Vertex(Point3D(r*cos(theta1), r*sin(theta1), -l/2)); }

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0.0, 0.0, -1.0));
	for(int i = 0; i < count; i++)  {
		float theta = (i*2.0*PI)/count;
		scene.Vertex(Point3D(r*cos(theta), r*sin(theta), -l/2)); }

	scene.NewPolygon();
	scene.SetNormal(Vector3D(0.0, 0.0, 1.0));
	for(int i = 0; i < count; i++)  {
		float theta = (i*2.0*PI)/count;
		scene.Vertex(Point3D(r*cos(theta), r*sin(theta), l/2)); }
}

void CreateSphereOctant(Scene& scene, double r, double count)
{
	CreateSphere(scene, r, count, PI/2.0, PI/2.0);
}

void CreateSphere(Scene& scene, double r, double count,
				  float theta, float phi)
{
	scene.NewObject();

	for(int i=0;  i<count;  i++)  {
		float theta0 = (i*theta)/count;
		float theta1 = ((i + 1.0)*theta)/count;

		for (int j=0; j<count;  j++) {
			float phi0 = (j*phi)/count;
			float phi1 = ((j + 1.0)*phi)/count;

			scene.NewPolygon();

			{
				float x = sin(phi0)*cos(theta0);
				float y = sin(phi0)*sin(theta0);
				float z = cos(phi0);
				scene.SetNormal(Vector3D(x, y, z));
				scene.Vertex(Point3D(r*x, r*y, r*z));
			}

			{
				float x = sin(phi0)*cos(theta1);
				float y = sin(phi0)*sin(theta1);
				float z = cos(phi0);
				scene.SetNormal(Vector3D(x, y, z));
				scene.Vertex(Point3D(r*x, r*y, r*z));
			}

			{
				float x = sin(phi1)*cos(theta1);
				float y = sin(phi1)*sin(theta1);
				float z = cos(phi1);
				scene.SetNormal(Vector3D(x, y, z));
				scene.Vertex(Point3D(r*x, r*y, r*z));
			}

			{
				float x = sin(phi1)*cos(theta0);
				float y = sin(phi1)*sin(theta0);
				float z = cos(phi1);
				scene.SetNormal(Vector3D(x, y, z));
				scene.Vertex(Point3D(r*x, r*y, r*z));
			}
		}
	}
}

void CreateLetterA(Scene& scene)
{
	double theta = 20.0*PI/180.0;
	double c = cos(theta);
	double s = sin(theta);
	double w = 0.06;
	double m = 0.5;

	Point3D top(0,1,0);
	Point3D lBottom(-s/c,0,0);
	Point3D rBottom( s/c,0,0);
	Point3D lMid(-(1-m)*s/c, 1-m, 0);
	Point3D rMid( (1-m)*s/c, 1-m, 0);
	Vector3D dx(w/c,0,0);
	Vector3D dy(0,w/s,0);

	double ax = w*s/c;
	double ay = w;
	double mx = s*(1.0-m);
	double my = c*(1.0-m);

	scene.NewObject();
	scene.NewPolygon();
	scene.SetNormal(Vector3D(0.0, 0.0, 1.0));
	scene.Vertex(top-dx);
	scene.Vertex(top+dx);
	scene.Vertex(rBottom+dx);
	scene.Vertex(rBottom-dx);
	scene.Vertex(rMid-dx+Vector3D(ax,-ay,0));
	scene.Vertex(lMid+dx+Vector3D(-ax,-ay,0));
	scene.Vertex(lBottom+dx);
	scene.Vertex(lBottom-dx);

	scene.SetColor(HSVColor(0, 0, 0.5), HSVColor(0, 0, 0.5), 0);
	scene.Hole();
	scene.Vertex(top-dy);
	scene.Vertex(rMid-dx+Vector3D(-ax,ay,0));
	scene.Vertex(lMid+dx+Vector3D(ax,ay,0));
}

void DrawSceneWithOpenGL(Scene& scene, int width, int height)
{
	while (true) {
		GLenum err = glGetError();
		if (err == GL_NO_ERROR) break;
		printf("ERROR: 0x%x\n", err); }

	// Draw some text on the screen
	char* msg = "OpenGL rendering (toggle with 'g')";
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 0.0);	// Set text color

	// Save the current transformations, and setup 2D transformation
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

    // Setup pixel coordinates
	gluOrtho2D(0,width,0,height); 

	// Set screen coords for text (in Pixel coordinates)
	glRasterPos2f(10,10);

	// Draw each character in msg string
	for (unsigned int i=0;  i<strlen(msg);  i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);

	// Restore original transformations
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Set OpenGL modes
	glEnable(GL_DEPTH_TEST);
	scene.EnableFlatShading ? glShadeModel(GL_FLAT) :  glShadeModel(GL_SMOOTH);
	
	// Communicate matrices to OpenGL
	float v[16], m[16];
	scene.viewing.ArrayForOpenGL(v);
	scene.projection.ArrayForOpenGL(m);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(v);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m);

	// Set some lighting parameters for OpenGL
	scene.EnableLighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	for (unsigned int li=0;  li<scene.lights.size();  li++) {
		Point3D& P3 = scene.lights[li].position;
		Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
		glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
		glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
		glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
		glEnable(GL_LIGHT0+li); }

	// Loop through the scene's objects
	for (unsigned int k=0;  k<scene.objects.size();  k++) {
		Object& obj = scene.objects[k];

		// Set this object's material properties
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba);
		glMateriali(GL_FRONT, GL_SHININESS, obj.n);
		glMaterialfv(GL_FRONT, GL_SPECULAR, obj.Ks.rgba);

		// If there's a texture (or reflection texture) set it up
		if (obj.refl) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, k);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
						 obj.refl->width, obj.refl->height,
						 0, GL_RGB, GL_FLOAT,
						 obj.refl->texels);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); 

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); } 

		else if (obj.texture) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, k);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			if (scene.UseAntiAliasing) {
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,
								  obj.texture->width, obj.texture->height,
								  GL_RGB, GL_FLOAT,
								  obj.texture->texels);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
								GL_LINEAR_MIPMAP_LINEAR); }
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
							 obj.texture->width, obj.texture->height, 0,
							 GL_RGB, GL_FLOAT,
							 obj.texture->texels);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
								GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
								GL_NEAREST); }

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);


			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); }

		else {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0); }
		
		// Loop through the object's polygons
		for (int j=0;  j< (int)obj.polygons.size();  j++) {
			APolygon& poly = obj.polygons[j];
			
			// Set OpenGL's polygon mode
			glBegin(GL_POLYGON);
			
			// Loop through each vertex.
			for (int i=0;  i< (int)poly.size(); i++)  {
				// If this vertex begins a hole, do something reasonable
				if (i && poly[i].prevIndex != i-1) {
					glEnd();
					glColor3i(0,0,0);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, Color(0,0,0).rgba);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(0.0,-100.0);
					glBegin(GL_POLYGON); }
				
				// Send the vertex to OpenGL
				glTexCoord2f(poly[i].u, poly[i].v);
				glNormal3fv(poly[i].N.begin());
				glVertex3fv(poly[i].V.begin());
			}
			
			// End openGL's polygon mode
			glEnd(); 
			glDisable(GL_POLYGON_OFFSET_FILL); }

		glDisable(GL_TEXTURE_2D); }

	glFlush();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
		
}

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
	matrixStack.clear();
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
