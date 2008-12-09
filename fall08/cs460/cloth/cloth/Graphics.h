#pragma once

#include <d3dx9.h>

#include "Vertex.h"
#include "Camera.h"
#include "Macros.h"
#include "Cloth.h"


#define GRAPHICS Graphics::Instance()

struct StringPars;

typedef std::vector< StringPars > StrParVec;

class Graphics
{
  public:
    static Graphics *Instance( void );

    bool Initialize( void );
    void Render( void );

    void AddLine( const ColoredVertex *verts );
    void AddTriangle( const ColoredVertex *verts );
    void AddQuad( const ColoredVertex *verts );
    void GetScreenWorldCoord( D3DXVECTOR3 &w, const D3DXVECTOR3 &s ) const;
    void IsectPlane( D3DXVECTOR3 *pOut, float x, float y, 
                     const D3DXPLANE &plane = D3DXPLANE( 0.f, 1.f, 0.f, 0.f ) ) const;
    void WriteText( const std::string &text, int x, int y, 
                    D3DCOLOR color = D3DCOLOR_XRGB( 4, 192, 255 ) );

    Camera &GetMainCam( void );

  private:
    void CreateSunLight( void );
    void DrawPrimitives( void );
    void RenderText( void );
    void SetupMatrices( void );
    void AddCloths( void );
    void AddSpheres( void );

    Graphics( void );
    Graphics( const Graphics &rhs );
    void operator =( const Graphics &rhs );
    ~Graphics( void );

  private:
    LPDIRECT3D9       pD3D;
    LPDIRECT3DDEVICE9 pD3DDevice;
    LPD3DXFONT        pD3DFont;
    D3DCOLOR          bgColor;

    VertVec           linePrimitives;
    VertVec           trianglePrimitives;
    Camera            mainCam;
    StrParVec         stringPars;
};

struct StringPars
{
  StringPars( const std::string &_text, int _x, int _y, D3DCOLOR _color )
    : text( _text ), x( _x ), y( _y ), color( _color ) {}

  std::string text;
  int         x, y;
  D3DCOLOR    color;
};
