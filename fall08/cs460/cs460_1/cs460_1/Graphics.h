#pragma once

#include <d3dx9.h>
#include <string>
#include <sstream>

#include "Model.h"
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
    void LoadStaticModel( const std::string &file );
    void LoadAnimatedModel( const std::string &file );
    void IncDecAnimSpeed( bool inc );
    void DrawLine( D3DXVECTOR3 p, D3DXVECTOR3 q );
    void SetupMatrices( void );
    void DisplayInfo( void );
    void CreateSunLight( void );
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
    LPD3DXFONT          pFont;
    LPD3DXMATRIXSTACK   pMatrixStack;
    Camera              mainCam;
    DWORD               bgColor;
    StaticModelList     staticModels;
    AnimatedModelList   animModels;
    HWND                hWnd;
    bool                ready;

  private:
    static void IncAnimSpeed( const AnimatedModel &animModel )
    {
      animModel.AnimSpeed += ( ( animModel.AnimSpeed >= 1.f ) ? 1.f : animModel.AnimSpeed );
    }
    static void DecAnimSpeed( const AnimatedModel &animModel )
    {
      ( animModel.AnimSpeed > 1.f ) ? animModel.AnimSpeed -= 1.f : animModel.AnimSpeed /= 2.f;
    }
    struct AddModelInfo
    {
      AddModelInfo( std::stringstream *_pInfo ) : pInfo( _pInfo ) {}
      void operator ()( const AnimatedModel &model )
      {
        *pInfo << "Animation Set: " << model.AnimSet + 1 << "/" << model.AnimSets.size() << std::endl
               << "Speed:         " << model.AnimSpeed << "x" << std::endl;
        *pInfo << std::endl;
      }

      std::stringstream *pInfo;
    };
};
