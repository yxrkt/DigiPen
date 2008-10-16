#pragma once

#include <d3dx9.h>
#include <string>

#include "Mesh.h"
#include "Macros.h"
#include "Vertex.h"



struct Camera
{
  D3DXVECTOR3 eye;
  D3DXVECTOR3 lookAt;
  D3DXVECTOR3 up;
};

class Graphics
{
  public:
    Graphics( void );
    ~Graphics( void );

    void Initialize( HWND hWndApp );
    void Update( void );
    void LoadStaticMesh( const std::string &file );
    void LoadAnimatedMesh( const std::string &file );
    void IncDecAnimSpeed( bool inc );
    void DrawLine( D3DXVECTOR3 p, D3DXVECTOR3 q );
    void SetupMatrices( void );
    void Cleanup( void );

      // Accessors
    Camera      &MainCam;
    const bool  &Ready;

  private:
      // Class cannot be copied
    Graphics( const Graphics &rhs );
    void operator =( const Graphics &rhs );

      // Data
    LPDIRECT3D9         pD3D;
    LPDIRECT3DDEVICE9   pDevice;
    Camera              mainCam;
    DWORD               bgColor;
    StaticMeshList      staticMeshes;
    AnimatedMeshList    animMeshes;
    HWND                hWnd;
    bool                ready;

  private:
    static void IncAnimSpeed( const AnimatedMesh &animMesh )
    {
      animMesh.AnimSpeed += ( ( animMesh.AnimSpeed >= 1.f ) ? 1.f : animMesh.AnimSpeed );
    }
    static void DecAnimSpeed( const AnimatedMesh &animMesh )
    {
      ( animMesh.AnimSpeed > 1.f ) ? animMesh.AnimSpeed -= 1.f : animMesh.AnimSpeed /= 2.f;
    }
};
