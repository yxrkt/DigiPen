#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>

#include "CAllocateHierarchy.h"
#include "Vertex.h"

#define ASSETS_DIR "../Assets/"

typedef std::vector< D3DMATERIAL9 >       MaterialVec;
typedef std::vector< LPDIRECT3DTEXTURE9 > TextureVec;
typedef std::vector< ColoredVertex >      VertVec;
typedef std::vector< std::string >        StringVec;

class StaticMesh
{
  public:
    StaticMesh( const LPDIRECT3DDEVICE9 _pDevice );
    ~StaticMesh();

    void Load( const std::string &file );
    void Render() const;

    LPD3DXMESH  &Data;

  private:
    LPDIRECT3DDEVICE9   pDevice;
    LPD3DXMESH          pData;
    MaterialVec         materials;
    TextureVec          textures;
};

class AnimatedMesh
{
  public:
    struct Sphere
    {
      float       radius;
      D3DXVECTOR3 center;
    };

  public:
    AnimatedMesh( const LPDIRECT3DDEVICE9 _pDevice );
    ~AnimatedMesh();

    void Load( const std::string &file );
    void FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld );
    void Render() const;
    void SetAnimationSet( DWORD index );
    void DrawBones() const;
    void AddBones( const LPFRAME pFrame, const D3DXMATRIX &matrix );

    const Sphere &GetBoundingSphere() const;
    //void SetupBoneMatrices( LPFRAME pFrame, LPD3DXMATRIX pParentMatrix )

  private:
    void ReadAnimData( const std::string &file );

    LPDIRECT3DDEVICE9   pDevice;
    LPFRAME             pFrameRoot;
    LPD3DXMATRIX        boneMatrices;
    Sphere              bs;

    StringVec           animSetNames;
    VertVec             boneLines;
};
