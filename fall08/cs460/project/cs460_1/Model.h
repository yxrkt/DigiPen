#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include <list>
#include <set>

#include "CAllocateHierarchy.h"
#include "Vertex.h"
#include "XFileParser.h"
#include "VQS.h"

#define ASSETS_DIR      "../Assets/"


typedef std::vector< D3DMATERIAL9 >       MaterialVec;
typedef std::vector< LPDIRECT3DTEXTURE9 > TextureVec;
typedef std::vector< ColoredVertex >      VertVec;
typedef std::vector< std::string >        StringVec;
typedef std::vector< AnimationSet >       AnimationSetVec;

class AnimatedModel;

typedef void ( *CHANGEFRAMEFN )( LPFRAME );
typedef void ( AnimatedModel::*MCHANGEFRAMEFN )( LPFRAME );
typedef void ( *ANIMCALLBACKFN )( void );

class StaticModel
{
  public:
    StaticModel( const LPDIRECT3DDEVICE9 _pDevice );
    ~StaticModel( void );

    void Load( const std::string &file );
    void Render( void ) const;

    LPD3DXMESH  &Data;

    float       Scale;
    D3DXVECTOR3 Pos;

  private:
    LPDIRECT3DDEVICE9   pDevice;
    LPD3DXMESH          pData;
    MaterialVec         materials;
    TextureVec          textures;
};

class AnimatedModel
{
  public:
    enum RENDER_FLAG { RENDER_BONES, RENDER_MESHES, RENDER_ALL };

    struct Sphere
    {
      float       radius;
      D3DXVECTOR3 center;
    };

  public:
    AnimatedModel( const LPDIRECT3DDEVICE9 _pDevice );
    AnimatedModel( const AnimatedModel &rhs );
    ~AnimatedModel( void );

    void DrawBones( void ) const;
    void FrameMove( DWORD elapsedTime, const D3DXMATRIX &matWorld, ANIMCALLBACKFN callback = NULL );
    void Load( const std::string &file );
    void Render( RENDER_FLAG flag = RENDER_MESHES );
    void SetAnimationSet( DWORD index );
    void DrawFrameMeshes( LPFRAME frame );
    Sphere GetBS( void ) const;
    D3DXMATRIX GetWorldTrans( void ) const;
    LPFRAME GetFrameRoot( void );
    D3DXVECTOR3 GetOrientVec( void ) const;
    void ForEachFrame( LPFRAME pRoot, MCHANGEFRAMEFN fn );

    static void ForEachFrame( LPFRAME pRoot, CHANGEFRAMEFN fn );
    static void DrawLinesToChildren( LPFRAME pFrame );

    const DWORD           &AnimSet;
    const size_t          &KeyFrame;
    const AnimationSetVec &AnimSets;
          float           &AnimSpeed;
          D3DXMATRIX      &MatScale, &MatRot, &MatTrans;

  private:
    void MoveFrames( const LPFRAME pFrame, const D3DXMATRIX &matrix );
    void MoveMeshes( const LPFRAME pFrame );
    void ReadAnimData( const std::string &file );
    void SetFrameMatrix( LPFRAME pFrame );
    void SetKeyFrame( DWORD tick );
    bool CreateSkeleton( void ) const;

  private:
    LPDIRECT3DDEVICE9   pDevice;
    LPFRAME             pFrameRoot;
    AnimationSetVec     animSets;
    Sphere              bs;
    DWORD               curAnimSet;
    size_t              curKeyFrame;
    float               animSpeed;
    float               exactFrame;
    D3DXMATRIX          matScale, matRot, matTrans;

    mutable VertVec     boneLines;

    static AnimatedModel *s_pCurAnimModel;
};


typedef std::list< StaticModel >    StaticModelList;
typedef std::list< AnimatedModel >  AnimatedModelList;
