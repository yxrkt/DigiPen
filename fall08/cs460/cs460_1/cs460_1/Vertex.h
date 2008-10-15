#pragma once

#include <d3dx9.h>

class ColoredVertex
{
  public:
    ColoredVertex( float _x = 0, float _y = 0, float _z = 0, DWORD _color = D3DCOLOR_XRGB( 0, 255, 0 ) )
      : x( _x ), y( _y ), z( _z ), color( _color ) {}
    ColoredVertex( const D3DXVECTOR3 &v )
      : x( v.x ), y( v.y ), z( v.z ), color( D3DCOLOR_XRGB( 0, 255, 0 ) ) {}

    LPD3DXVECTOR3 ToLPD3DXVECTOR3() const
    {
      return ( LPD3DXVECTOR3 )this;
    };

    float x, y, z;
    DWORD color;
};

#define D3DFVF_COLOREDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
