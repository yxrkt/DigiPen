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


class StaticModel
{
  public:
    StaticModel( const LPDIRECT3DDEVICE9 _pDevice );
    ~StaticModel();

    void Load( const std::string &file );
    void Render() const;

    LPD3DXMESH  &Data;

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
    ~AnimatedModel();

    void DrawBones() const;
    void FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld );
    void Load( const std::string &file );
    void Render( RENDER_FLAG flag = RENDER_MESHES );
    void SetAnimationSet( DWORD index );
    void DrawFrameMeshes( LPFRAME frame );
    Sphere GetBS( void ) const;
    D3DXMATRIX GetWorldTrans( void ) const;

    const DWORD           &AnimSet;
    const size_t          &KeyFrame;
    const AnimationSetVec &AnimSets;
          float           &AnimSpeed;
          D3DXMATRIX      &MatScale, &MatRot, &MatTrans;

  private:
    void MoveBones( const LPFRAME pFrame, const D3DXMATRIX &matrix, size_t keyFrame );
    void MoveMeshes( const LPFRAME pFrame );
    void ReadAnimData( const std::string &file );
    void SetFrameMatrix( LPFRAME pFrame, size_t keyFrame, bool exact = true );
    void SetKeyFrame( DWORD tick );

  private:
    LPDIRECT3DDEVICE9   pDevice;
    LPFRAME             pFrameRoot;
    VertVec             boneLines;
    AnimationSetVec     animSets;
    Sphere              bs;
    DWORD               curAnimSet;
    size_t              curKeyFrame;
    float               animSpeed;
    float               exactFrame;
    D3DXMATRIX          matScale, matRot, matTrans;
};


typedef std::list< StaticModel >    StaticModelList;
typedef std::list< AnimatedModel >  AnimatedModelList;
