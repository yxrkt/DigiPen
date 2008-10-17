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
, curAnimSet( 0 )
, animSpeed( 1.f )
, BS( bs )
, AnimSet( curAnimSet )
, KeyFrame( curKeyFrame )
, AnimSpeed( animSpeed )
{
}

AnimatedMesh::AnimatedMesh( const AnimatedMesh &rhs )
: pDevice( rhs.pDevice )
, pFrameRoot( rhs.pFrameRoot )
, boneLines( rhs.boneLines )
, animSets( rhs.animSets )
, bs( rhs.bs )
, curAnimSet( rhs.curAnimSet )
, curKeyFrame( rhs.curKeyFrame )
, animSpeed( rhs.animSpeed )
, exactFrame( rhs.exactFrame )
, BS( bs )
, AnimSet( curAnimSet )
, KeyFrame( curKeyFrame )
, AnimSpeed( animSpeed )
{
}

AnimatedMesh::~AnimatedMesh()
{
}

void AnimatedMesh::Load( const std::string &file )
{
  HRESULT hr;

  CAllocateHierarchy allocHierarchy;
  hr = D3DXLoadMeshHierarchyFromX( file.c_str(), 0, pDevice, &allocHierarchy, NULL, 
                                   (LPD3DXFRAME *)&pFrameRoot, NULL );
  ASSERT( hr == S_OK, "Loading mesh hierarchy failed." );

  AddBones( pFrameRoot, pFrameRoot->TransformationMatrix );

  ReadAnimData( file );

  hr = D3DXFrameCalculateBoundingSphere( pFrameRoot, &bs.center, &bs.radius );
  ASSERT( hr == S_OK, "Calculating bounding sphere failed." );
}

void AnimatedMesh::ReadAnimData( const std::string &file )
{
  AnimationSet animSet;
  XFileParser xFile( file );
  while ( xFile.GetNextAnimationSet( animSet ) )
  {
    animSets.push_back( animSet );
  }
}

void AnimatedMesh::DrawBones() const
{
  HRESULT hr;

  hr = pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  ASSERT( hr == S_OK, "Turning off lighting failed." );

  hr = pDevice->SetFVF( D3DFVF_COLOREDVERTEX );
  ASSERT( hr == S_OK, "Setting FVF failed." );

  hr = pDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)boneLines.size() / 2,
                                 &boneLines[0], sizeof( ColoredVertex ) );
  ASSERT( hr == S_OK, "Drawing mesh bones failed." );

  hr = pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
  ASSERT( hr == S_OK, "Turning on lighting failed." );
}

void AnimatedMesh::AddBones( const LPFRAME pFrame, const D3DXMATRIX &matrix )
{
  if ( pFrame )
  {
    D3DXVECTOR3 startVert;
    D3DXVec3TransformCoord( &startVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &matrix );

    LPFRAME pCurChild = (LPFRAME)pFrame->pFrameFirstChild;
    while ( pCurChild )
    {
      D3DXMATRIX concat;
      D3DXMatrixMultiply( &concat, &pCurChild->TransformationMatrix, &matrix );
      D3DXVECTOR3 childVert;
      D3DXVec3TransformCoord( &childVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &concat );
      if ( startVert.x != 0 || startVert.y != 0 || startVert.z != 0 )
      { // avoid drawing kickstand
        boneLines.push_back( startVert );
        boneLines.push_back( childVert );
      }
      AddBones( pCurChild, concat );
      pCurChild = (LPFRAME)pCurChild->pFrameSibling;
    }
  }
}

void AnimatedMesh::MoveBones( const LPFRAME pFrame, const D3DXMATRIX &matrix, size_t keyFrame )
{
  if ( pFrame )
  {
    SetFrameMatrix( pFrame, keyFrame );

    D3DXVECTOR3 startVert;
    D3DXVec3TransformCoord( &startVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &matrix );

    LPFRAME pCurChild = (LPFRAME)pFrame->pFrameFirstChild;
    while ( pCurChild )
    {
      SetFrameMatrix( pCurChild, keyFrame );
      D3DXMATRIX concat;
      D3DXMatrixMultiply( &concat, &pCurChild->TransformationMatrix, &matrix );
      D3DXVECTOR3 childVert;
      D3DXVec3TransformCoord( &childVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &concat );
      if ( startVert.x != 0 || startVert.y != 0 || startVert.z != 0 )
      { // avoid drawing kickstand
        boneLines.push_back( startVert );
        boneLines.push_back( childVert );
      }
      MoveBones( pCurChild, concat, keyFrame );
      pCurChild = (LPFRAME)pCurChild->pFrameSibling;
    }
  }
}

void AnimatedMesh::SetFrameMatrix( LPFRAME pFrame, size_t keyFrame, bool exact )
{
  if ( pFrame->Name != NULL )
  {
    AnimationMap::iterator animIt = animSets[curAnimSet].animMap.find( pFrame->Name );
    if ( animIt != animSets[curAnimSet].animMap.end() )
    {
      if ( !exact )
      {
        pFrame->TransformationMatrix = animIt->second.animKey.keyFrames[keyFrame].matrix;
      }
      else
      {
        D3DXVECTOR3     scale0, scale1;
        D3DXVECTOR3     trans0, trans1;
        D3DXQUATERNION  rot0,   rot1;

        D3DXMatrixDecompose( &scale0, &rot0, &trans0, &animIt->second.animKey.keyFrames[(int)floor(exactFrame)].matrix );
        D3DXMatrixDecompose( &scale1, &rot1, &trans1, &animIt->second.animKey.keyFrames[(int)ceil(exactFrame)].matrix );

        VQS vqs0( trans0, rot0, scale0.x );
        VQS vqs1( trans1, rot1, scale1.x );

        VQS vqsFinal;
        VQS::Interpolate( vqsFinal, vqs0, vqs1, exactFrame - floor( exactFrame ) );

        D3DXMATRIX mtxFinal;
        for ( int r = 0; r < 4; ++r )
        {
          for ( int c = 0; c < 4; ++c )
          {
            if ( r == c && r != 3 )
              mtxFinal.m[c][r] = vqsFinal.S;
            else if ( r == 3 &&  c != 3 )
              mtxFinal.m[c][r] = vqsFinal.V[c];
            else
              mtxFinal.m[c][r] = 0.f;
          }
        }
        mtxFinal.m[3][3] = 1.f;
        //D3DXMATRIX mtxRot( 1 - 2y2 - 2z2, 2xy - 2wz, 2xz + 2wy, 0,
        //                   2xy + 2wz, 1 - 2x2 - 2z2, 2yz - 2wx, 0,
        //                   2xz - 2wy, 2yz + 2wx, 1 - 2x2 - 2y2, 0,
        //                   0, 0, 0, 1 );
      }
    }
  }
}

void AnimatedMesh::FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld )
{
  SetKeyFrame( elapsedTime );

  boneLines.clear();
  SetFrameMatrix( pFrameRoot, curKeyFrame );
  D3DXMatrixMultiply( &pFrameRoot->TransformationMatrix, &pFrameRoot->TransformationMatrix, &mtxWorld );
  MoveBones( pFrameRoot, pFrameRoot->TransformationMatrix, curKeyFrame );
}

void AnimatedMesh::SetKeyFrame( DWORD tick )
{
  KeyFrameVec &keyFrames0 = animSets[curAnimSet].animMap.begin()->second.animKey.keyFrames;
  size_t nKeyFrames = keyFrames0.size(), nLastFrame = nKeyFrames - 1;
  DWORD modVal = (DWORD)( (float)( keyFrames0[nLastFrame].tick + 1.f ) / animSpeed );
  tick = (DWORD)( animSpeed * (float)( tick % modVal ) );
  for ( size_t i = nLastFrame; i >= 0; --i )
  {
    if ( tick >= keyFrames0[i].tick )
    {
      float num   = (float)( tick - keyFrames0[i].tick );
      float den   = (float)( keyFrames0[(i + 1) % nKeyFrames].tick - keyFrames0[i].tick );
      exactFrame  = (float)i + num / den;
      curKeyFrame = i;
      break;
    }
  }
}

void AnimatedMesh::Render() const
{
  
}

void AnimatedMesh::SetAnimationSet( DWORD index )
{
  curAnimSet = min( index, animSets.size() - 1 );
}

