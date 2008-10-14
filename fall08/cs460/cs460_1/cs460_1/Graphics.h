#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include <list>

#include "Mesh.h"

#define SAFE_RELEASE( p ) if ( p ) p->Release(); p = NULL;

struct ColoredVertex
{
  FLOAT x, y, z;
  DWORD color;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_COLOREDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct Camera
{
  D3DXVECTOR3 eye;
  D3DXVECTOR3 lookAt;
  D3DXVECTOR3 up;
};

class Graphics
{
  public:
    typedef std::list< StaticMesh >   StaticMeshList;
    typedef std::list< AnimatedMesh > AnimatedMeshList;

  public:
    Graphics();
    ~Graphics();

    void Initialize( HWND hWndApp );
    void Update();
    void LoadStaticMesh( const std::string &file );
    void LoadAnimatedMesh( const std::string &file );
    void SetupMatrices();
    void Cleanup();

    // temp shiz
    void DrawBone( D3DXVECTOR3 p, D3DXVECTOR3 q );

    Camera      &MainCam;
    const bool  &Ready;

  private:
    Graphics( const Graphics &rhs );
    void operator =( const Graphics &rhs );

    LPDIRECT3D9         pD3D;
    LPDIRECT3DDEVICE9   pDevice;
    Camera              mainCam;
    DWORD               bgColor;
    StaticMeshList      staticMeshes;
    AnimatedMeshList    animMeshes;

    HWND hWnd;
    bool ready;
};
