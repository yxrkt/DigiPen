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
#include <map>

#include "framework.h"
#include "scenelib.h"

// =============================================================================
// My Interface
// =============================================================================


float *g_zBuf;          // depth buffer
int g_Width, g_Height;  // screen width & height
Scene *g_pScene;        // the 'scene'

struct Edge;

  // Edge containers
std::list<Edge> EdgeTable;
std::list<Edge> ActiveEdgeList;
typedef std::list<Edge>::iterator EdgeListIt;

  // Mipmaping stuff
typedef std::vector< Texture > MipmapVector;
std::map< Texture *, MipmapVector > MipmapMap;

  // Rounds a float to the nearest int
int FloatToInt( const float &f )
{
  if ( f >= 0 )
    return (int) ( f + .5f );
  return (int) ( f - .5f );
}

  // Edge data
struct Edge
{
  Edge( const Vertex &_v0, const Vertex &_v1 ) : v0( _v0 ), v1( _v1 )
  {
      // Convert vertices to pixel coords and store inverse w coord
    WorldToPixel( v0, i_w );
    WorldToPixel( v1, i_w1 );

      // Make sure v0 is the lower vertex
    SwapIf();

      // Set starting values
    x   = v0.V[0];
    z   = v0.V[2];
    N   = v0.N;
    u_w = v0.u * i_w;
    v_w = v0.v * i_w;

      // Set incremental values
    float dy = v1.V[1] - v0.V[1];

    dx    = ( v1.V[0]     - v0.V[0] ) / dy;
    dz    = ( v1.V[2]     - v0.V[2] ) / dy;
    dN    = ( v1.N        - v0.N    ) / dy;
    du_w  = ( v1.u * i_w1 - u_w     ) / dy;
    dv_w  = ( v1.v * i_w1 - v_w     ) / dy;
    di_w  = ( i_w1        - i_w     ) / dy;
  }

    // Increment values for each scanline
  void Inc()
  {
    x   += dx;
    z   += dz;
    N   += dN;
    u_w += du_w;
    v_w += dv_w;
    i_w += di_w;
  }

    // Convert a world coordinate vertex to pixel coordinates and store inverse w
  void WorldToPixel( Vertex &vtx, float &wi )
  {
      // convert to pixel coordinates
    Vector4D v4S = g_pScene->projection.Transform( g_pScene->viewing.Transform( vtx.V ) );
    wi           = 1.f / v4S[3];
    v4S          = Vector4D( v4S.Hdiv() );
    v4S[0]       = (float) FloatToInt( ( v4S[0] + 1.f ) * (float) g_Width  / 2.f );
    v4S[1]       = (float) FloatToInt( ( v4S[1] + 1.f ) * (float) g_Height / 2.f );
    vtx.V        = v4S;
    vtx.N.normalize();
  }

    // swap vertices if 2nd is lower than first
  void SwapIf()
  {
    if ( v0.V[1] > v1.V[1] )
    {
      std::swap( v0, v1 );
      std::swap( i_w, i_w1 );
    }
  }

    // For sorting edges in the AEL
  bool operator <( const Edge &rhs ) const
  {
    if ( x < rhs.x )
      return true;
    else if ( x > rhs.x )
      return false;
    else
      return ( dx < rhs.dx ) ? true : false;
  }

  Vertex v0, v1;              // start and end vertices

  float x, z;                 // current x & z values
  Vector3D N;                 // current normal
  float u_w, v_w, i_w, i_w1;  // current texture values

  float dx, dz;               // incremental values for x and z
  Vector3D dN;                // incremental value for N
  float du_w, dv_w, di_w;     // incremental texture values

}; // End Edge struct

  // Used for incrementing along the scanline
struct IncX
{
  void Inc()
  {
    z   += dzdx;
    u_w += dudx;
    v_w += dvdx;
    i_w += dwdx;
    N   += dNdx;
  }

  void operator *=( const float &rhs )
  {
    z   += ( dzdx * rhs );
    u_w += ( dudx * rhs );
    v_w += ( dvdx * rhs );
    i_w += ( dwdx * rhs );
    N   += ( dNdx * rhs );
  }

  void Clamp()
  {
    if ( u_w < 0.f )
      u_w = 0.f;
    if ( v_w < 0.f )
      v_w = 0.f;
    if ( i_w < 0.f )
      i_w = 0.f;
  }

  float z, dzdx, u_w, dudx, v_w, dvdx, i_w, dwdx;
  Vector3D N, dNdx;
};

  // Float range checker
bool IsInRange( float val, float low, float high )
{
  return ( val >= low && val < high ) ? true : false;
}

  // Set the x boundaries
void SetBounds( int &low, int &high, IncX &incX )
{
    // Trivial rejection (cull)
  if ( high < 0 || low > g_Width )
  {
    low = high;
    return;
  }

    // Clip left edge
  if ( low < 0 )
  {
    incX *= (float) -low;
    low = 0;
  }

    // Clip right edge
  if ( high > g_Width )
  {
    high = g_Width;
  }
}

  // Retrieve value in z buffer
float &ZBuf( int x, int y )
{
  return g_zBuf[ g_Width * y + x ];
}

  // Vector3D dot product
float operator*( const Vector3D &lhs, const Vector3D &rhs )
{
  return ( lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] );
}

  // Clip an edge against bottom and top
void ClipY( EdgeListIt EdgeIt )
{
    // bottom border
  if ( EdgeIt->v0.V[1] < 0.f )
  {
    EdgeIt->x   += ( -EdgeIt->v0.V[1] * EdgeIt->dx   );
    EdgeIt->z   += ( -EdgeIt->v0.V[1] * EdgeIt->dz   );
    EdgeIt->N   += ( -EdgeIt->v0.V[1] * EdgeIt->dN   );
    EdgeIt->u_w += ( -EdgeIt->v0.V[1] * EdgeIt->du_w );
    EdgeIt->v_w += ( -EdgeIt->v0.V[1] * EdgeIt->dv_w );
    EdgeIt->i_w += ( -EdgeIt->v0.V[1] * EdgeIt->di_w );
  }

    // top border
  else if ( EdgeIt->v1.V[1] > g_Height )
  {
    float fYMax = (float) ( g_Height - 1 );
    float fYDif = EdgeIt->v1.V[1] - fYMax;
    EdgeIt->v1.V[1]  = fYMax;
    EdgeIt->v1.V[0] -= ( fYDif * EdgeIt->dx );
    EdgeIt->v1.V[2] -= ( fYDif * EdgeIt->dz );
  }
}

 // finds log base 2 of a float
float lg( const float rhs )
{
  return ( log( rhs ) / log( 2.f ) );
}

  // Gets the average of a group of texels
void GetAverage( float texels[4][3], float *avg, unsigned nTexels = 4 )
{
  memset( avg, 0, 3 * sizeof( float ) );
  for ( unsigned i = 0; i < nTexels; ++i )
  {
    for ( unsigned j = 0; j < 3; ++j )
    {
      avg[j] += ( texels[i][j] / (float) nTexels );
    }
  }
}

  // Gets weighted average of a group of texels
void GetWAverage( float *wavg, float **colors, float *weights, unsigned nTexels = 4 )
{
  memset( wavg, 0, 3 * sizeof( float ) );
  for ( unsigned i = 0; i < nTexels; ++i )
  {
    for ( unsigned j = 0; j < 3; ++j )
    {
      wavg[j] += colors[i][j] * weights[i];
    }
  }
}

  // Gets the color at ( u, v ) on a texture
float *GetTexColor( Texture *tex, float u_w, float v_w, float i_w )
{
  int u = (int) floor( (float) tex->width  * u_w / i_w );
  int v = (int) floor( (float) tex->height * v_w / i_w );

  return tex->texel( u, v, 0 );
}

  // Generates mipmaps for a texture
void GenerateMipmaps( Texture *pTex )
{
  MipmapVector mipmaps;

  int nWidth      = pTex->width;
  int nHeight     = pTex->height;
  Texture *pPrev  = pTex;

  while ( nWidth != 1 && nHeight != 1 )
  {
    nWidth  /= 2;
    nHeight /= 2;

    Texture mipmap( nWidth, nHeight, 3 );

    for ( int x = 0; x < nWidth; ++x )
    {
      for ( int y = 0; y < nHeight; ++y )
      {
        int X = 2 * x;
        int Y = 2 * y;

        float texels[4][3];

        memcpy( texels[0], pPrev->texel( X, Y, 0 ),         3 * sizeof( float ) );
        memcpy( texels[1], pPrev->texel( X + 1, Y, 0 ),     3 * sizeof( float ) );
        memcpy( texels[2], pPrev->texel( X, Y + 1, 0 ),     3 * sizeof( float ) );
        memcpy( texels[3], pPrev->texel( X + 1, Y + 1, 0 ), 3 * sizeof( float ) );

        GetAverage( texels, mipmap.texel( x, y, 0 ) );
      }
    }

    mipmaps.push_back( mipmap );
    pPrev = &mipmaps.front();
  }

  MipmapMap[ pTex ] = mipmaps;
}

// =============================================================================
// End My Interface
// =============================================================================

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene( Scene &scene )
{
  size_t nObjects = scene.objects.size();
  for ( size_t i = 0; i < nObjects; ++i )
    GenerateMipmaps( scene.objects[i].texture );
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene( Scene &scene, int width, int height )
{
	// Choose OpenGL or student rendering here.  The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	// if (useOpenGLRendering || useFastRendering)
	if ( scene.UseOpenGLRendering )
  {
		DrawSceneWithOpenGL( scene, width, height );
    return;
  }

  // ---------------------------------------------------------------------------
	// Student rendering code goes here
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glBegin( GL_POINTS );

  g_pScene  = &scene;
  g_Width   = width;
  g_Height  = height;
  unsigned nLength = (unsigned) ( width * height );

  g_zBuf = new float[nLength];

  for ( unsigned i = 0; i < nLength; ++i )
    g_zBuf[i] = 1.f;

    // for each object
  size_t nObjects = scene.objects.size();
  for ( size_t i = 0; i < nObjects; ++i )
  {
    Object &obj = scene.objects[i];

      // for each polygon
    size_t nPolys = obj.polygons.size();
    for ( size_t j = 0; j < nPolys; ++j )
    {
      EdgeTable.clear();
      APolygon &poly = obj.polygons[j];

        // add edges to edge table
      size_t nVerts = poly.size();
      for ( size_t k = 0; k < nVerts; ++k )
      {
        int nNext = poly[k].prevIndex;
        Edge edge( poly[k], poly[nNext] );

          // skip horizontal edges
        if ( edge.v0.V[1] != edge.v1.V[1] )
          EdgeTable.push_back( edge );
      }

      ActiveEdgeList.clear();
      EdgeListIt ETIt, AELIt;

        // Cull / clip edges
      ETIt = EdgeTable.begin();
      while ( ETIt != EdgeTable.end() )
      {
          // y culling
        if ( ETIt->v1.V[1] < 0.f || ETIt->v0.V[1] >= height )
          EdgeTable.erase( ETIt++ );

        else
        {
          ClipY( ETIt );
          ++ETIt;
        }
      }

        // Set values for scanline 0
      ETIt = EdgeTable.begin();
      while ( ETIt != EdgeTable.end() )
      {
        if ( (ETIt->v0.V[1] <= 0) && (ETIt->v0.V[1] != ETIt->v1.V[1]) && (ETIt->v1.V[1] != 0.f) )
        {
          ActiveEdgeList.push_back( *ETIt );
          EdgeTable.erase( ETIt++ );
        }
        else
          ++ETIt;
      }

      ActiveEdgeList.sort();

        // for each scanline
      for ( int y = 0; y < height; ++y )
      {
          // draw by pair
        for ( AELIt = ActiveEdgeList.begin(); AELIt != ActiveEdgeList.end(); ++AELIt )
        {
          EdgeListIt AELItPrev  = AELIt++;
          float dx  = ( AELIt->x - AELItPrev->x );
          IncX incX;

            // Set starting values for scanline
          incX.z    = AELItPrev->z;
          incX.N    = AELItPrev->N;
          incX.u_w  = AELItPrev->u_w;
          incX.v_w  = AELItPrev->v_w;
          incX.i_w  = AELItPrev->i_w;

            // Set incremental values for scanline
          incX.dzdx = ( AELIt->z   - AELItPrev->z   ) / dx;
          incX.dNdx = ( AELIt->N   - AELItPrev->N   ) / dx;
          incX.dudx = ( AELIt->u_w - AELItPrev->u_w ) / dx;
          incX.dvdx = ( AELIt->v_w - AELItPrev->v_w ) / dx;
          incX.dwdx = ( AELIt->i_w - AELItPrev->i_w ) / dx;

            // Clip edges along left and right borders
          int x0 = FloatToInt( AELItPrev->x ), x1 = FloatToInt( AELIt->x );
          SetBounds( x0, x1, incX );

            // Draw each pixel on scanline
          for ( int x = x0; x < x1; ++x )
          {
            if ( incX.z < ZBuf( x, y ) )
            {
              incX.Clamp();
              ZBuf( x, y ) = incX.z;

                // Approximate area
              float den = ( incX.i_w == 0.f ) ? .001f : incX.i_w;
              float Area = ( AELItPrev->dv_w * incX.dudx - AELItPrev->du_w * incX.dvdx ) / den;
              /*Area *= 2.f * (float) ( obj.texture->width * obj.texture->height );*/
              Area *= (float) ( width * height );
              float D = ( Area > 0.f ) ? .5f * lg( Area ) : -1.f;

                // magnification: use interpolation
              if ( D < 0 )
              {
                float u   = (float) obj.texture->width  * incX.u_w / incX.i_w;
                float v   = (float) obj.texture->height * incX.v_w / incX.i_w;
                float uf  = u - floor( u );
                float vf  = v - floor( v );
                float ufC = 1.f - uf;
                float vfC = 1.f - vf;

                float *colors[4], wavg[3];
                colors[0] = obj.texture->texel( floor(u), floor(v), 0 );
                colors[1] = obj.texture->texel( ceil(u),  floor(v), 0 );
                colors[2] = obj.texture->texel( floor(u), ceil(v),  0 );
                colors[3] = obj.texture->texel( ceil(u),  ceil(v),  0 );

                  // Make sure to not go off texture
                if ( (int) ceil(u) > obj.texture->width || (int) ceil(v) > obj.texture->height )
                {
                  glColor3fv( GetTexColor( obj.texture, incX.u_w, incX.v_w, incX.i_w ) );
                }

                else
                {
                  float weights[4] = { ufC * vfC, vfC * uf, ufC * vf, uf * vf };
                  GetWAverage( wavg, colors, weights );
                  glColor3fv( wavg );
                }
              }

                // minification: use mipmapping
              else
              {
                if ( D > 7.f )
                  bool break_here = true;

                int DFloor  = (int) D;
                int DCeil   = DFloor + 1;

                  // Get low and high bounds
                float *rgbL = GetTexColor( &MipmapMap[obj.texture][DFloor], incX.u_w, incX.v_w, incX.i_w );
                float *rgbH = GetTexColor( &MipmapMap[obj.texture][DCeil],  incX.u_w, incX.v_w, incX.i_w );

                  // Get weighted average
                float rgb[3];
                float DFrac  = D - floor( D );
                float DFracC = 1.f - DFrac;
                for ( unsigned k = 0; k < 3; ++k )
                  rgb[k] = rgbL[k] * DFracC + rgbH[k] * DFrac;

                glColor3fv( rgb );
              }

              glVertex2i( x, y );
            }

            incX.Inc();
          }
        }

          // insert edges into AEL
        bool bSort = false;
        ETIt = EdgeTable.begin();
        while ( ETIt != EdgeTable.end() )
        {
          if ( IsInRange( (float) y, ETIt->v0.V[1], ETIt->v1.V[1] ) )
          {
            ActiveEdgeList.push_back( *ETIt );
            EdgeTable.erase( ETIt++ );
            bSort = true;
          }
          else
            ++ETIt;
        }

          // increment edges on AEL & remove passed edges
        AELIt = ActiveEdgeList.begin();
        while ( AELIt != ActiveEdgeList.end() )
        {
          AELIt->Inc();

          if ( !IsInRange( (float) y, AELIt->v0.V[1], AELIt->v1.V[1] ) )
            ActiveEdgeList.erase( AELIt++ );
          else
            ++AELIt;
        }

          // sort the AEL
        if ( bSort ) ActiveEdgeList.sort();

      } // [END] for each scanline
    } // [END] for each polygon
  } // [END] for each object

  delete [] g_zBuf;

  glEnd();
}
