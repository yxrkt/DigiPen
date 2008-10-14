#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>

#define ASSETS_DIR "../Assets/"

typedef std::vector< D3DMATERIAL9 >       MaterialVec;
typedef std::vector< LPDIRECT3DTEXTURE9 > TextureVec;

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
    AnimatedMesh( const LPDIRECT3DDEVICE9 _pDevice );
    ~AnimatedMesh();

    void Load( const std::string &file );
    void FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld );
    void Render() const;
    void SetAnimationSet( DWORD index );

  private:
    LPDIRECT3DDEVICE9 pDevice;
};
