#pragma once

#include <d3dx9.h>
#include <vector>

#define D3DFVF_COLOREDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class ColoredVertex
{
  public:
    ColoredVertex( float _x = 0, float _y = 0, float _z = 0, 
                   DWORD _color = D3DCOLOR_XRGB( 0, 255, 0 ), float _u = 0.f, float _v = 0.f )
      : x( _x ), y( _y ), z( _z ), color( _color ), u( _u ), v( _v ) {}
    ColoredVertex( const D3DXVECTOR3 &rhs )
      : x( rhs.x ), y( rhs.y ), z( rhs.z ), color( D3DCOLOR_XRGB( 0, 255, 0 ) ), u( 0.f ), v( 0.f ) {}

    bool operator ==( const ColoredVertex &rhs )
    {
      return !memcmp( this, &rhs, sizeof( ColoredVertex ) );
    }

    LPD3DXVECTOR3 ToLPD3DXVECTOR3() const
    {
      return (LPD3DXVECTOR3)this;
    };

    float     x, y, z;
    D3DCOLOR  color;
    float     u, v;
};

typedef std::vector< ColoredVertex >  VertVec;
typedef VertVec::iterator             VertVecIt;
typedef VertVec::const_iterator       VertVecItC;
