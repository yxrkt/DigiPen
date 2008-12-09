#pragma once

#include "AutoList.h"
#include "Vertex.h"

class Cloth : public AutoList< Cloth >
{
  public:
    Cloth( float _width, float _height, int resX, int resY, 
           D3DXVECTOR3 pos = D3DXVECTOR3( 0.f, 0.f, 0.f ) );
    ~Cloth( void );

    float   GetWidth( void ) const;
    float   GetHeight( void ) const;
    int     GetResX( void ) const;
    int     GetResY( void ) const;
    VertVec &GetVertices( void );
    void    UpdateNormals( void );
    int     GetNumNodes( void ) const;

    ColoredVertex &GetVertex( int r, int c );

  private:
    VertVec     vertices;
    float       width, height;
    int         resX, resY;
    D3DXVECTOR3 pos;
};
