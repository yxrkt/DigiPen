#pragma once

#include <d3dx9.h>
#include <string>
#include <sstream>
#include <float.h>

#include "Model.h"
#include "Macros.h"
#include "Vertex.h"
#include "Joint.h"

#define Gfx Graphics::Instance()


#define CP_RADIUS 5.f
#define CP_HEIGHT .5f

#define CCD_MAX_ITERS 200
#define CCD_FAIL      0.f
#define CCD_SUCCESS   5.f


struct Camera
{
  D3DXVECTOR3 eye;
  D3DXVECTOR3 lookAt;
  D3DXVECTOR3 up;
};

class Graphics
{
  private:
    Graphics( void );
    ~Graphics( void );
    Graphics( const Graphics &rhs );
    void operator =( const Graphics &rhs );

  public:
    static Graphics *Instance();

    void Initialize( HWND hWndApp );
    void Update( void );
    void LoadStaticModel( const std::string &file, const D3DXVECTOR3 &pos = D3DXVECTOR3( 1.f, 1.f, 1.f ), 
                                                   float scale = 1.f );
    void LoadAnimatedModel( const std::string &file );
    void IncDecAnimSpeed( bool inc );
    void SetupMatrices( void );
    void DisplayInfo( void );
    void CreateSunLight( void );
    void Cleanup( void );
    void AddQuad( const ColoredVertex *verts );
    LPDIRECT3DDEVICE9 GetDevice( void ) const;
    D3DXVECTOR3 IsectGroundPlane( int x, int y );
    void DrawControlPoints( void );
    void AddLine( const ColoredVertex *verts );
    void AddLine( const ColoredVertex &begin, const ColoredVertex &end );
    void AddPolyline( const ColoredVertex *verts, size_t nVerts, bool closed = true );
    bool CCD( JointVec &joints, const D3DXVECTOR3 &dest ) const;
    void PauseAnims( void );
    void PlayAnims( void );
    bool IsPaused( void ) const;
    LPDIRECT3DDEVICE9 GetDevice( void );
    void WriteText( const std::string &text );

    Camera              &MainCam;
    const bool          &Ready;
    StaticModelList     &StaticModels;
    AnimatedModelList   &AnimatedModels;

    LPDIRECT3DTEXTURE9  pFloorTex;
    VertVec             controlPoints;

    LPD3DXFONT          pFont;

    ANIMCALLBACKFN      animCallback;

  private:
    LPDIRECT3D9         pD3D;
    LPDIRECT3DDEVICE9   pDevice;
    LPD3DXMATRIXSTACK   pMatrixStack;
    Camera              mainCam;
    DWORD               bgColor;
    StaticModelList     staticModels;
    AnimatedModelList   animModels;
    HWND                hWnd;
    bool                ready;
    bool                paused;
    VertVec             quadPrimitives;
    VertVec             linePrimitives;
    VertVec             polylinePrimitives;
    DWORD               curTime;
    std::stringstream   infoStrings;

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

