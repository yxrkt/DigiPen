#include "Model.h"
#include "ASSERT.h"


// Static Model
StaticModel::StaticModel( LPDIRECT3DDEVICE9 _pDevice )
: pDevice( _pDevice )
, pData( NULL )
, Data( pData )
{
}

StaticModel::~StaticModel()
{
}

void StaticModel::Load( const std::string &file )
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
    materials[i].Ambient = .5f * materials[i].Diffuse;

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

void StaticModel::Render() const
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

// Animated Model
AnimatedModel::AnimatedModel( LPDIRECT3DDEVICE9 _pDevice )
: pDevice( _pDevice )
, curAnimSet( 0 )
, animSpeed( 1.f )
, exactFrame( 0.f )
, curKeyFrame( 0 )
, AnimSet( curAnimSet )
, KeyFrame( curKeyFrame )
, AnimSets( animSets )
, AnimSpeed( animSpeed )
, MatScale( matScale )
, MatRot( matRot )
, MatTrans( matTrans )
{
  D3DXMatrixIdentity( &matScale );
  D3DXMatrixIdentity( &matRot   );
  D3DXMatrixIdentity( &matTrans );
}

AnimatedModel::AnimatedModel( const AnimatedModel &rhs )
: pDevice( rhs.pDevice )
, pFrameRoot( rhs.pFrameRoot )
, boneLines( rhs.boneLines )
, animSets( rhs.animSets )
, bs( rhs.bs )
, curAnimSet( rhs.curAnimSet )
, curKeyFrame( rhs.curKeyFrame )
, animSpeed( rhs.animSpeed )
, exactFrame( rhs.exactFrame )
, AnimSet( curAnimSet )
, KeyFrame( curKeyFrame )
, AnimSets( animSets )
, AnimSpeed( animSpeed )
, MatScale( matScale )
, MatRot( matRot )
, MatTrans( matTrans )
{
  D3DXMatrixIdentity( &matScale );
  D3DXMatrixIdentity( &matRot   );
  D3DXMatrixIdentity( &matTrans );
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::Load( const std::string &file )
{
  HRESULT hr;

  CAllocateHierarchy allocHierarchy;
  hr = D3DXLoadMeshHierarchyFromX( file.c_str(), 0, pDevice, &allocHierarchy, NULL, 
                                   (LPD3DXFRAME *)&pFrameRoot, NULL );
  ASSERT( hr == S_OK, "Loading mesh hierarchy failed." );

  ReadAnimData( file );

  hr = D3DXFrameCalculateBoundingSphere( pFrameRoot, &bs.center, &bs.radius );
  ASSERT( hr == S_OK, "Calculating bounding sphere failed." );
}

void AnimatedModel::ReadAnimData( const std::string &file )
{
  AnimationSet animSet;
  XFileParser xFile( file );
  while ( xFile.GetNextAnimationSet( animSet ) )
  {
    animSets.push_back( animSet );
  }
}

void AnimatedModel::DrawBones() const
{
  pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  pDevice->SetFVF( D3DFVF_COLOREDVERTEX );
  pDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)boneLines.size() / 2,
                            &boneLines[0], sizeof( ColoredVertex ) );
  pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void AnimatedModel::MoveBones( const LPFRAME pFrame, const D3DXMATRIX &matrix, size_t keyFrame )
{
  if ( pFrame )
  {
    if ( pFrame == pFrameRoot )
    {
      boneLines.clear();
    }

    SetFrameMatrix( pFrame, keyFrame );

    D3DXVECTOR3 startVert;
    D3DXVec3TransformCoord( &startVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &matrix );

    LPFRAME pCurFrame = (LPFRAME)pFrame->pFrameFirstChild;
    while ( pCurFrame )
    {
      SetFrameMatrix( pCurFrame, keyFrame );
      pCurFrame->matCombined;
      D3DXMatrixMultiply( &pCurFrame->matCombined, &pCurFrame->TransformationMatrix, &matrix );

      D3DXVECTOR3 childVert;
      D3DXVec3TransformCoord( &childVert, &D3DXVECTOR3( 0.f, 0.f, 0.f ), &pCurFrame->matCombined );
      boneLines.push_back( startVert );
      boneLines.push_back( childVert );

      MoveBones( pCurFrame, pCurFrame->matCombined, keyFrame );

      pCurFrame = (LPFRAME)pCurFrame->pFrameSibling;
    }
  }
}

void AnimatedModel::MoveMeshes( const LPFRAME pFrame )
{
  if ( pFrame )
  {
    LPMESHCONTAINER pMesh = (LPMESHCONTAINER)pFrame->pMeshContainer;
    if ( pMesh )
    {
      if ( pMesh->pSkinInfo )
      {
        if ( !pMesh->pSkinMesh )
        {
          HRESULT hr;
          hr = pMesh->MeshData.pMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMesh->MeshData.pMesh->GetFVF(), 
                                                    pDevice, &pMesh->pSkinMesh );
          ASSERT( SUCCEEDED( hr ), "Cloning mesh failed" );
        }

        void *srcPtr, *destPtr;
        pMesh->MeshData.pMesh->LockVertexBuffer( D3DLOCK_READONLY, &srcPtr );
        pMesh->pSkinMesh->LockVertexBuffer( 0, &destPtr );

        DWORD nBones = pMesh->pSkinInfo->GetNumBones();
        std::vector< D3DXMATRIX > boneMatrices( nBones );
        for ( DWORD i = 0; i < nBones; ++i )
        {
          D3DXMATRIX out;

          LPFRAME pTempFrame = (LPFRAME)D3DXFrameFind( pFrameRoot, pMesh->pSkinInfo->GetBoneName( i ) );
          pMesh->ppFrameMatrices[i] = &pTempFrame->matCombined;

          D3DXMatrixMultiply( &out, &pMesh->pBoneOffsets[i], pMesh->ppFrameMatrices[i] );
          boneMatrices[i] = out;
        }

        pMesh->pSkinInfo->UpdateSkinnedMesh( &boneMatrices[0], NULL, srcPtr, destPtr );

        pMesh->MeshData.pMesh->UnlockVertexBuffer();
        pMesh->pSkinMesh->UnlockVertexBuffer();
      }
    }

    MoveMeshes( (LPFRAME)pFrame->pFrameSibling );
    MoveMeshes( (LPFRAME)pFrame->pFrameFirstChild );
  }
}

void AnimatedModel::SetFrameMatrix( LPFRAME pFrame, size_t keyFrame, bool exact )
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
      else if ( !boneLines.empty() )
      {
        D3DXVECTOR3     scale0, scale1;
        D3DXVECTOR3     trans0, trans1;
        D3DXQUATERNION  rot0,   rot1;

        D3DXMatrixDecompose( &scale0, &rot0, &trans0, &animIt->second.animKey.keyFrames[(int)floor(exactFrame)].matrix );
        D3DXMatrixDecompose( &scale1, &rot1, &trans1, &animIt->second.animKey.keyFrames[(int)ceil(exactFrame)].matrix );

        VQS vqs0( trans0, rot0, scale0.x );
        VQS vqs1( trans1, rot1, scale1.x );

        VQS vqs;
        VQS::Interpolate( vqs, vqs0, vqs1, exactFrame - floor( exactFrame ) );

        D3DXMATRIX mtxFinal;
        vqs.GetMatrix( mtxFinal );

        pFrame->TransformationMatrix = mtxFinal;
      }
    }
  }
}

void AnimatedModel::FrameMove( DWORD elapsedTime, const D3DXMATRIX &mtxWorld )
{
  SetKeyFrame( elapsedTime );
  
  D3DXMATRIX mtxRootWorld;

  SetFrameMatrix( pFrameRoot, curKeyFrame );
  D3DXMatrixMultiply( &mtxRootWorld, &pFrameRoot->TransformationMatrix, &mtxWorld );
  MoveBones( pFrameRoot, mtxRootWorld, curKeyFrame );
  MoveMeshes( pFrameRoot );
}

void AnimatedModel::SetKeyFrame( DWORD tick )
{
  if ( animSpeed == 0.f ) return;

  KeyFrameVec &keyFrames0 = animSets[curAnimSet].animMap.begin()->second.animKey.keyFrames;
  size_t nKeyFrames = keyFrames0.size(), nLastFrame = nKeyFrames - 1;

  static DWORD realTick = 0;
  static DWORD lastTick = tick;
  DWORD tickDif = (DWORD)( animSpeed * (float)( tick - lastTick ) );
  realTick += tickDif;
  DWORD curTick = ( realTick % keyFrames0[nLastFrame].tick );

  for ( size_t i = nLastFrame; i >= 0; --i )
  {
    if ( curTick >= keyFrames0[i].tick )
    {
      float num   = (float)( curTick - keyFrames0[i].tick );
      float den   = (float)( keyFrames0[(i + 1) % nKeyFrames].tick - keyFrames0[i].tick );
      exactFrame  = (float)i + num / den;
      curKeyFrame = i;
      break;
    }
  }

  lastTick = tick;
}

void AnimatedModel::Render( RENDER_FLAG flag )
{
  if ( flag == RENDER_BONES  || flag == RENDER_ALL )
    DrawBones();
  if ( flag == RENDER_MESHES || flag == RENDER_ALL )
    DrawFrameMeshes( pFrameRoot );
}

void AnimatedModel::SetAnimationSet( DWORD index )
{
  curAnimSet = min( index, animSets.size() - 1 );
}

void AnimatedModel::DrawFrameMeshes( LPFRAME pFrame )
{
  LPMESHCONTAINER pMesh = (LPMESHCONTAINER)pFrame->pMeshContainer;

  while ( pMesh )
  {
    LPD3DXMESH pD3DMesh = pMesh->pSkinInfo ? pMesh->pSkinMesh : pMesh->MeshData.pMesh;

    for ( DWORD i = 0; i < pMesh->NumMaterials; ++i )
    {
      pDevice->SetMaterial( &pMesh->pMaterials9[i] );
      pDevice->SetTexture( 0, pMesh->ppTextures[i] );
      pD3DMesh->DrawSubset( i );
    }

    pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
  }

  if ( pFrame->pFrameSibling )
    DrawFrameMeshes( (LPFRAME)pFrame->pFrameSibling );

  if ( pFrame->pFrameFirstChild )
    DrawFrameMeshes( (LPFRAME)pFrame->pFrameFirstChild );
}

AnimatedModel::Sphere AnimatedModel::GetBS( void ) const
{
  Sphere curBS = bs;
  D3DXVec3TransformCoord( &curBS.center, &bs.center, &GetWorldTrans() );
  curBS.radius *= MatScale._11; // assumes uniform scale
  return curBS;
}

D3DXMATRIX AnimatedModel::GetWorldTrans( void ) const
{
  D3DXMATRIX world;
  D3DXMatrixMultiply( &world, &matScale, &matRot   );
  D3DXMatrixMultiply( &world, &world,    &matTrans );
  return world;
}
