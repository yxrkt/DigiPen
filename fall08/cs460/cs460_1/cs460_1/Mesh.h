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
#define FLOAT_EPSILON   0.00001f


typedef std::vector< D3DMATERIAL9 >       MaterialVec;
typedef std::vector< LPDIRECT3DTEXTURE9 > TextureVec;
typedef std::vector< ColoredVertex >      VertVec;
typedef std::vector< std::string >        StringVec;
typedef std::vector< AnimationSet >       AnimationSetVec;


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
    AnimatedMesh( const AnimatedMesh &rhs );
    ~AnimatedMesh();

    void DrawBones() const;
    void FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld );
    void Load( const std::string &file );
    void Render() const;
    void SetAnimationSet( DWORD index );

    const Sphere  &BS;
    const DWORD   &AnimSet;
    const size_t  &KeyFrame;
          float   &AnimSpeed;

  private:
    void AddBones( const LPFRAME pFrame, const D3DXMATRIX &matrix );
    void MoveBones( const LPFRAME pFrame, const D3DXMATRIX &matrix, size_t keyFrame );
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
};


typedef std::list< StaticMesh >           StaticMeshList;
typedef std::list< AnimatedMesh >         AnimatedMeshList;
