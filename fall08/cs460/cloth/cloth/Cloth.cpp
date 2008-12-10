#include "Cloth.h"


Cloth::Cloth( float _width, float _height, int _resX, int _resY, D3DXVECTOR3 _pos )
: width( _width )
, height( _height )
, resX( _resX )
, resY( _resY )
, pos( _pos )
{
  float halfWidth  = width  / 2.f;
  float halfHeight = height / 2.f;

  int lastCol = resX - 1;
  int lastRow = resY - 1;

  float fResX = (float)resX, fLastRow = (float)lastRow;
  float fResY = (float)resY, fLastCol = (float)lastCol;

  float xStep = width  / fResX;
  float yStep = height / fResY;


  vertices.reserve( resX * resY );

  D3DXVECTOR3 colorD( 1.f, 0.f, 0.f );
  D3DXVECTOR3 colorC( 0.f, 0.f, 1.f );
  D3DXVECTOR3 colorB( 1.f, 1.f, 0.f );
  D3DXVECTOR3 colorA( 0.f, 1.f, 0.f );

  for ( int r = 0; r < resY; ++r )
  {
    for ( int c = 0; c < resX; ++c )
    {
      D3DXVECTOR3 localPos( -halfWidth + (float)c * xStep, 0.f, -halfHeight + (float)r * yStep );
      ColoredVertex vert( pos + localPos );
      float fr = (float)r, fc = (float)c;
      D3DXVECTOR3 fColor = 
        ( ( ( fLastRow - fr ) * ( fLastCol - fc ) ) / ( fLastRow * fLastCol ) ) * colorA +
        ( ( ( fLastRow - fr ) * ( fc            ) ) / ( fLastRow * fLastCol ) ) * colorB +
        ( ( ( fr            ) * ( fc            ) ) / ( fLastRow * fLastCol ) ) * colorC +
        ( ( ( fr            ) * ( fLastCol - fc ) ) / ( fLastRow * fLastCol ) ) * colorD;
      vert.Color = D3DCOLOR_COLORVALUE( fColor.x, fColor.y, fColor.z, 1.f );
      vertices.push_back( vert );
    }
  }
}

Cloth::~Cloth( void )
{
}

void Cloth::UpdateNormals( void )
{
  D3DXVECTOR3 *neighbors[4];
  int lastCol = resX - 1, lastRow = resY - 1;

  for ( int r = 0; r < resY; ++r )
  {
    for ( int c = 0; c < resX; ++c )
    {
      ColoredVertex &vert = GetVertex( r, c );
      neighbors[0] = c != 0       ? &GetVertex( r,     c - 1).Pos : NULL;
      neighbors[1] = r != 0       ? &GetVertex( r - 1, c    ).Pos : NULL;
      neighbors[2] = c != lastCol ? &GetVertex( r,     c + 1).Pos : NULL;
      neighbors[3] = r != lastRow ? &GetVertex( r + 1, c    ).Pos : NULL;

      int nNormals = 0;
      D3DXVECTOR3 normalBuf;
      vert.Norm = D3DXVECTOR3( 0.f, 0.f, 0.f );
      for ( int i = 0; i < 4; ++i )
      {
        int j = i != 3 ? i + 1 : 0;
        if ( neighbors[i] && neighbors[j] )
        {
          D3DXVECTOR3 u( vert.Pos - *neighbors[j] ), v( *neighbors[i] - vert.Pos );
          D3DXVec3Cross( &normalBuf, &v, &u );
          D3DXVec3Normalize( &normalBuf, &normalBuf );
          vert.Norm += normalBuf;
          nNormals++;
        }
      }
      vert.Norm /= (float)nNormals;
    }
  }
}

int Cloth::GetNumNodes( void ) const
{
  return resX * resY;
}

float Cloth::GetWidth( void ) const
{
  return width;
}

float Cloth::GetHeight( void ) const
{
  return height;
}

int Cloth::GetResX( void ) const
{
  return resX;
}

int Cloth::GetResY( void ) const
{
  return resY;
}

VertVec &Cloth::GetVertices( void )
{
  return vertices;
}

ColoredVertex &Cloth::GetVertex( int r, int c )
{
  return vertices[r * resX + c];
}

const D3DXVECTOR3 &Cloth::GetPos( void ) const
{
  return pos;
}
