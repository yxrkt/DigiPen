///////////////////////////////////////////////////////////////////////
// $Id: render.cpp 1033 2007-07-14 20:43:26Z gherron $
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright �� 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include <list>

#include "framework.h"
#include "scenelib.h"

// =============================================================================
// My Interface

  
float *zBuf;    // depth buffer
int w, h;       // screen width & height

  // Edge data
struct Edge
{
  Edge(const Vector3D &_v0, const Vector3D &_v1) : v0(_v0), v1(_v1), x(_v0[0]), z(_v0[2])
  {
    if (_v1[1] - _v0[1])
    {
      dx   = (_v1[0] - _v0[0]) / (_v1[1] - _v0[1]);
      dz   = (_v1[2] - _v0[2]) / (_v1[1] - _v0[1]);
    }

    else
    {
      dx   = _v1[0] - _v0[0];
      dz   = _v1[2] - _v0[2];
    }
  }

    // Increment x & z values (used for each scanline)
  void Inc()
  {
    x += dx;
    z += dz;
  }

    // For sorting edges in the AEL
  bool operator <(const Edge &rhs) const
  {
    if (x < rhs.x)
      return true;
    else if (x > rhs.x)
      return false;
    else
      return (dx < rhs.dx) ? true : false;
  }

  Vector3D v0, v1;          // start and end vertices
  float dx, dz;             // incremental values for x and z
  float x, z;               // current x & z values
};

  // Generic range checker
bool IsInRange(float val, float low, float high)
{
  return (val >= low && val < high) ? true : false;
}

  // Set the x boundaries
void SetBounds(int &low, int &high)
{
  int max = w;

  if (low < 0)
    low = 0;

  else if (low > max)
  {
    low = max;
    high = low;
    return;
  }

  if (high > max)
    high = max;

  else if (high < 0)
  {
    high = 0;
    low = high;
    return;
  }
}

int FloatToInt(const float &f)
{
  if (f >= 0)
    return (int) (f + .5f);
  return (int) (f - .5f);
}

  // For casting point to vector
template <typename T, typename U>
T bybyte_cast(U u) { return (*(T *) &u); }

  // Retrieve value in z buffer
float &ZBuf(int x, int y)
{
  return zBuf[w * y + x];
}

  // Vector3D dot product
float operator*(const Vector3D &lhs, const Vector3D &rhs)
{
  return (lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]);
}

  // Set the polygon color
void SetColor(Vector3D vLightN, Vector3D vPolyN, Color colDif, float *rgba)
{
  float fScale = vLightN * vPolyN;
  memcpy(rgba, &colDif[0], 4 * sizeof(float));
  for (unsigned i = 0; i < 3; ++i)
    rgba[i] = rgba[i] * fScale;
}

  // Edge containers
std::list<Edge> EdgeTable;
std::list<Edge> ActiveEdgeList;

typedef std::list<Edge>::iterator EdgeListIt;

// End My Interface
// =============================================================================

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene(Scene& scene)
{}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene& scene, int width, int height)
{
	// Choose OpenGL or student rendering here.  The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	// if (useOpenGLRendering || useFastRendering)
	if (scene.UseOpenGLRendering)
  {
		DrawSceneWithOpenGL(scene, width, height);
    return;
  }

  // ---------------------------------------------------------------------------
	// Student rendering code goes here
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_POINTS);

  w = width;
  h = height;
  unsigned nLength = (unsigned) (width * height);

  zBuf = new float[nLength];

  for (unsigned i = 0; i < nLength; ++i)
    zBuf[i] = 1.f;

    // for each object
  size_t nObjects = scene.objects.size();
  for (size_t i = 0; i < nObjects; ++i)
  {
    Object &obj = scene.objects[i];

      // for each polygon
    size_t nPolys = obj.polygons.size();
    for (size_t j = 0; j < nPolys; ++j)
    {
      EdgeTable.clear();
      APolygon &poly = obj.polygons[j];
      std::vector<Vector3D> vVertices;

        // Set polygon color
      float rgba[4];
      Vector3D v3Light = bybyte_cast<Vector3D>(scene.lights[0].position);
      v3Light.normalize();
      SetColor(v3Light, poly[0].N, obj.Kd, rgba);

        // make the A object blue both sides
      if (i == 2)
      {
        for (int c = 0; c < 3; ++c)
          rgba[c] = abs(rgba[c]);
      }

        // Get pixel coords for polygon & push edges
      size_t nVerts = poly.size();
      for (size_t k = 0; k < nVerts; ++k)
      {
          // current vertex
        Vector4D v4T = scene.viewing.Transform(poly[k].V);
        Vector3D v3S = bybyte_cast<Vector3D>(scene.projection.Transform(v4T).Hdiv());
        v3S[0] = (float) FloatToInt((v3S[0] + 1.f) * width / 2.f);
        v3S[1] = (float) FloatToInt((v3S[1] + 1.f) * height / 2.f);

        vVertices.push_back(v3S);
      }

        // put vertices in correct order
      for (size_t k = 0; k < nVerts; ++k)
      {
        unsigned nNext = poly[k].prevIndex;

          // skip horizontal edges
        if ((int) vVertices[k][1] == (int) vVertices[nNext][1])
          continue;

        if (vVertices[k][1] < vVertices[nNext][1])
        {
          Edge e(vVertices[k], vVertices[nNext]);
          EdgeTable.push_back(e);
        }

        else
        {
          Edge e(vVertices[nNext], vVertices[k]);
          EdgeTable.push_back(e);
        }
      }

      ActiveEdgeList.clear();
      EdgeListIt ETIt, AELIt;
      size_t nEdges = EdgeTable.size();

        // Cull / clip edges
      ETIt = EdgeTable.begin();
      while (ETIt != EdgeTable.end())
      {
          // y culling
        if (ETIt->v1[1] < 0.f || ETIt->v0[1] >= height)
          EdgeTable.erase(ETIt++);
        else
        {
            // y clipping
          if (ETIt->v0[1] < 0)
          {
            ETIt->x += (-ETIt->v0[1] * ETIt->dx);
            ETIt->z += (-ETIt->v0[1] * ETIt->dz);
          }
          else if (ETIt->v1[1] > height)
          {
            float fYMax = (float) (height - 1);
            float fYDif = ETIt->v1[1] - fYMax;
            ETIt->v1[1]  = fYMax;
            ETIt->v1[0] -= (fYDif * ETIt->dx);
            ETIt->v1[2] -= (fYDif * ETIt->dz);
          }
          ++ETIt;
        }
      }

        // Set values for scanline 0
      ETIt = EdgeTable.begin();
      while (ETIt != EdgeTable.end())
      {
        if ((ETIt->v0[1] <= 0) && (ETIt->v0[1] != ETIt->v1[1]) && (ETIt->v1[1] != 0.f))
        {
          ActiveEdgeList.push_back(*ETIt);
          EdgeTable.erase(ETIt++);
        }
        else
          ++ETIt;
      }

      ActiveEdgeList.sort();

        // for each scanline
      for (int y = 0; y < height; ++y)
      {
          // draw by pair
        for (AELIt = ActiveEdgeList.begin(); AELIt != ActiveEdgeList.end(); ++AELIt)
        {
          EdgeListIt AELItPrev = AELIt++;
          float z              = AELItPrev->z;
          float dzdx           = (AELIt->z - AELItPrev->z) / (AELIt->x - AELItPrev->x);

          int x0 = FloatToInt(AELItPrev->x), x1 = FloatToInt(AELIt->x);
          SetBounds(x0, x1);
          for (int x = x0; x < x1; ++x)
          {
            if (z < ZBuf(x, y))
            {
              ZBuf(x, y) = z;
              glColor4fv(rgba);
              glVertex2i(x, y);
            }
            z += dzdx;
          }
        }

          // insert edges into AEL
        bool bSort = false;
        ETIt = EdgeTable.begin();
        while (ETIt != EdgeTable.end())
        {
          if (IsInRange((float) y, ETIt->v0[1], ETIt->v1[1]))
          {
            ActiveEdgeList.push_back(*ETIt);
            EdgeTable.erase(ETIt++);
            bSort = true;
          }
          else
            ++ETIt;
        }

          // increment edges on AEL & remove passed edges
        AELIt = ActiveEdgeList.begin();
        while (AELIt != ActiveEdgeList.end())
        {
          AELIt->Inc();

          if (!IsInRange((float) y, AELIt->v0[1], AELIt->v1[1]))
            ActiveEdgeList.erase(AELIt++);
          else
            ++AELIt;
        }

          // sort the AEL
        if (bSort) ActiveEdgeList.sort();

      } // [END] for each scanline
    } // [END] for each polygon
  } // [END] for each object

  delete [] zBuf;

  glEnd();
}
