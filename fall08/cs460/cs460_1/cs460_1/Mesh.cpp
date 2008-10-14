#include "Mesh.h"
#include "ASSERT.h"

// Static Mesh
StaticMesh::StaticMesh( LPDIRECT3DDEVICE9 _pDevice )
: pDevice( _pDevice )
, pData( NULL )
, Data( pData )
{
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::Load( const std::string &file )
{
  HRESULT hr;

  DWORD nMaterials;
  LPD3DXBUFFER pMtrlBuffer;

  hr = D3DXLoadMeshFromX( file.c_str(), D3DXMESH_SYSTEMMEM, pDevice, 
                          NULL, &pMtrlBuffer, NULL, &nMaterials, &pData );
  ASSERT( hr == S_OK, "Loading mesh failed." );

  D3DXMATERIAL *materialBuf = (D3DXMATERIAL *)pMtrlBuffer->GetBufferPointer();

  materials.reserve( nMaterials );
  textures.reserve( nMaterials );

  for ( DWORD i = 0; i < nMaterials; ++i )
  {
    materials.push_back( materialBuf[i].MatD3D );
    materials[i].Ambient = materials[i].Diffuse;

    textures.push_back( NULL );
    if ( materialBuf[i].pTextureFilename != NULL && strlen( materialBuf[i].pTextureFilename ) > 0 )
    {
      std::string texFile( std::string( ASSETS_DIR ) + materialBuf[i].pTextureFilename );
      hr = D3DXCreateTextureFromFileA( pDevice, texFile.c_str(), &textures[i] );
      if ( hr != S_OK )
        MESSAGEOK( "Creating texture failed." );
    }
  }

  pMtrlBuffer->Release();
}

void StaticMesh::Render() const
{
  HRESULT hr;

  size_t nMaterials = materials.size();
  for ( size_t i = 0; i < nMaterials; ++i )
  {
    hr = pDevice->SetMaterial( &materials[i] );
    ASSERT( hr == S_OK, "Setting material failed." );

    hr = pDevice->SetTexture( 0, textures[i] );
    ASSERT( hr == S_OK, "Setting texture failed." );

    hr = pData->DrawSubset( (DWORD)i );
    ASSERT( hr == S_OK, "DrawSubset failed." );
  }
}

// Animated Mesh
AnimatedMesh::AnimatedMesh( LPDIRECT3DDEVICE9 _pDevice )
: pDevice( _pDevice )
{
}

AnimatedMesh::~AnimatedMesh()
{
}

void AnimatedMesh::Load( const std::string &file )
{
  //HRESULT hr;

  //hr = D3DXLoadMeshHierarchyFromX( file.c_str(), 0, pDevice, 
}

void AnimatedMesh::FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld )
{
}

void AnimatedMesh::Render() const
{
}

void AnimatedMesh::SetAnimationSet( DWORD index )
{
}
