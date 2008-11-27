#pragma once

#include <d3dx9.h>
#include <string>
#include <sstream>
#include <float.h>

#include "Model.h"
#include "Macros.h"
#include "Vertex.h"


#define CP_RADIUS 5.f
#define CP_HEIGHT .5f

#define CCD_FAIL      1.f
#define CCD_SUCCESS   10.f

typedef std::vector< LPFRAME >  PFrameVec;
typedef std::vector< float >    FloatVec;


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
    bool CCD( PFrameVec *pFramesOut, const PFrameVec *pFramesIn, 
              const D3DXVECTOR3 &dest, const FloatVec *pFloats ) const;

    Camera              &MainCam;
    const bool          &Ready;
    StaticModelList     &StaticModels;
    AnimatedModelList   &AnimatedModels;

    LPDIRECT3DTEXTURE9  pFloorTex;
    VertVec             controlPoints;

    LPD3DXFONT          pFont;

  private:
      // Class cannot be copied
    Graphics( const Graphics &rhs );
    void operator =( const Graphics &rhs );

    LPDIRECT3D9         pD3D;
    LPDIRECT3DDEVICE9   pDevice;
    LPD3DXMATRIXSTACK   pMatrixStack;
    Camera              mainCam;
    DWORD               bgColor;
    StaticModelList     staticModels;
    AnimatedModelList   animModels;
    HWND                hWnd;
    bool                ready;
    VertVec             quadPrimitives;
    VertVec             linePrimitives;
    VertVec             polylinePrimitives;

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
