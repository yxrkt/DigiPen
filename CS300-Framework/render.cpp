///////////////////////////////////////////////////////////////////////
// $Id: render.cpp 1033 2007-07-14 20:43:26Z gherron $
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright Å© 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include <map>
#include <list>

#include "framework.h"
#include "scenelib.h"

// =============================================================================
// My Interface
float *zBuf;
int w, h;

struct Edge
{
  Edge() : bastard(true) {}
  Edge(const Vector3D &_v0, const Vector3D &_v1) : v0(_v0), v1(_v1), x(_v0[0]), z(_v0[2]), bastard(false)
  {
    dx   = (_v1[0] - _v0[0]) / (_v1[1] - _v0[1]);
    dz   = (_v1[2] - _v0[2]) / (_v1[1] - _v0[1]);
  }

  void Inc()
  {
    x += dx;
    z += dz;
  }

  Vector3D v0, v1;
  float dx, dz;
  float x, z;
  bool bastard;
};

template <typename T>
bool IsInRange(T val, T low, T high)
{
  return (val >= low && val <= high) ? true : false;
}

void SetBounds(int &low, int &high)
{
  int max = w - 1;

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

float &ZBuf(unsigned x, unsigned y)
{
  return zBuf[w * y + x];
}

std::list<Edge> EdgeTable;
std::map<float, Edge> ActiveEdgeList;

typedef std::map<float, Edge>::iterator EdgeMapIt;
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

  glBegin(GL_POINTS);

  w = width;
  h = height;
  unsigned nLength = (unsigned) (width * height);

  zBuf = new float[nLength];

  for (unsigned i = 0; i < nLength; ++i)
    zBuf[i] = 1.f;

    // for each object
  size_t nObjects = scene.objects.size();
  for (size_t i = 0; i < 1/*nObjects*/; ++i)
  {
    Object &obj = scene.objects[i];

      // for each polygon
    size_t nPolys = obj.polygons.size();
    for (size_t j = 0; j < 1/*nPolys*/; ++j)
    {
      EdgeTable.clear();
      APolygon &poly = obj.polygons[j];
      std::vector<Vector3D> vVertices;

        // Get pixel coords for polygon
      size_t nVerts = poly.size();
      for (size_t k = 0; k < nVerts; ++k)
      {
        Vector4D v4T = scene.viewing.Transform(poly[k].V);
        Vector3D v3S = *(Vector3D *) &scene.projection.Transform(v4T).Hdiv();
        v3S[0] = (v3S[0] + 1.f) * width / 2.f;
        v3S[1] = (v3S[1] + 1.f) * height / 2.f;

        vVertices.push_back(v3S);
      }

        // put vertices in correct order
      for (size_t k = 0; k < nVerts; ++k)
      {
        unsigned nNext = (k + 1) % nVerts;

          // skip horizontal edges
        if (vVertices[k][1] == vVertices[nNext][1])
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
      EdgeMapIt mIt;
      EdgeListIt lIt;
      size_t nEdges = EdgeTable.size();

        // for each scanline
      for (int y = 0; y < height; ++y)
      {
        if (!(ActiveEdgeList.size() % 2)) // TEMP SOLUTION FOR ODD NO. OF EDGES IN AEL
        {
            // draw by pair
          for (mIt = ActiveEdgeList.begin(); mIt != ActiveEdgeList.end(); ++mIt)
          {
            EdgeMapIt mItPrev = mIt++;
            float z           = mItPrev->second.z;
            float dzdx        = (mIt->second.z - mItPrev->second.z) / (mIt->second.x - mItPrev->second.x);

            int x0 = (unsigned) (mItPrev->second.x), x1 = (unsigned) mIt->second.x;
            SetBounds(x0, x1);
            for (int x = x0; x < x1; ++x)
            {
              if (z < ZBuf(x, y))
              {
                ZBuf(x, y) = z;
                glColor3f(.5f, .5f, .5f);
                glVertex2i(x, y);
              }
              z += dzdx;
            }
          }
        } // [END] TEMP SOLUTION

        else
        {
          int LOLDAVID = 0;
        }

          // insert edges into AEL
        lIt = EdgeTable.begin();
        while (lIt != EdgeTable.end())
        {
          if (IsInRange((float) y, lIt->v0[1], lIt->v1[1]))
          {
            ActiveEdgeList[lIt->x] = *lIt;
            EdgeTable.erase(lIt++);
          }
          else
            ++lIt;
        }

          // increment edges on AEL & remove passed edges
        mIt = ActiveEdgeList.begin();
        while (mIt != ActiveEdgeList.end())
        {
          mIt->second.Inc();

          if (!IsInRange((float) y, mIt->second.v0[1], mIt->second.v1[1]))
            ActiveEdgeList.erase(mIt++);
          else
            ++mIt;
        }

      } // [END] for each scanline
    } // [END] for each polygon
  } // [END] for each object

  delete [] zBuf;

  glEnd();
}
