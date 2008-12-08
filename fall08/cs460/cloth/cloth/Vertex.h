#pragma once

#include <d3dx9.h>
#include <vector>

#define D3DFVF_COLOREDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_NORMAL)

class ColoredVertex
{
  public:
    ColoredVertex( const D3DXVECTOR3 _Pos  = D3DXVECTOR3( 0.f, 0.f, 0.f ), 
                   const D3DXVECTOR3 _Norm = D3DXVECTOR3( 0.f, 1.f, 0.f ), 
                   D3DCOLOR _Color = D3DCOLOR_XRGB( 0, 255, 0 ) )
      : Pos( _Pos )
      , Norm( _Norm )
      , Color( _Color )
    {}

    bool operator ==( const ColoredVertex &rhs )
    {
      return !memcmp( this, &rhs, sizeof( ColoredVertex ) );
    }

    ColoredVertex &operator ()( const D3DXVECTOR3 &vec )
    {
      return *this;
    }

    D3DXVECTOR3 Pos;
    D3DXVECTOR3 Norm;
    D3DCOLOR    Color;
    float       Mass;
};

typedef std::vector< ColoredVertex >  VertVec;
typedef VertVec::iterator             VertVecIt;
typedef VertVec::const_iterator       VertVecItC;
