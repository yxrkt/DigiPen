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
    }
  }

  pMtrlBuffer->Release();

  Scale = 1.f;
  Pos   = D3DXVECTOR3( 1.f, 1.f, 1.f );
}

void StaticModel::Render() const
{
  size_t nMaterials = materials.size();
  for ( size_t i = 0; i < nMaterials; ++i )
  {
    pDevice->SetMaterial( &materials[i] );
    pDevice->SetTexture( 0, textures[i] );

    D3DXVECTOR3 vecScale( Scale, Scale, Scale );
    D3DXMATRIX matWorld;
    D3DXMatrixTransformation( &matWorld, NULL, NULL, &vecScale, NULL, NULL, &Pos );
    pDevice->SetTransform( D3DTS_WORLD, &matWorld );
    pData->DrawSubset( (DWORD)i );
    pDevice->SetTransform( D3DTS_WORLD, D3DXMatrixIdentity( &matWorld ) );
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

  std::string filePath = std::string( ASSETS_DIR ) + file;

  CAllocateHierarchy allocHierarchy;
  hr = D3DXLoadMeshHierarchyFromX( filePath.c_str(), 0, pDevice, &allocHierarchy, 
                                   NULL, (LPD3DXFRAME *)&pFrameRoot, NULL );
  ASSERT( hr == S_OK, "Loading mesh hierarchy failed." );

  ReadAnimData( filePath );

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
  if ( !CreateSkeleton() ) return;

  pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  pDevice->SetFVF( D3DFVF_COLOREDVERTEX );
  pDevice->DrawPrimitiveUP( D3DPT_LINELIST, (UINT)boneLines.size() / 2,
                            &boneLines[0], sizeof( ColoredVertex ) );
  ( (AnimatedModel *)this )->ForEachFrame( pFrameRoot, &AnimatedModel::DrawCoordSys );
  pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void AnimatedModel::MoveFrame( const LPFRAME pFrame, const D3DXMATRIX &matrix )
{
  D3DXMatrixMultiply( &pFrame->matCombined, &pFrame->TransformationMatrix, &matrix );

  if ( pFrame->pFrameSibling )
    MoveFrame( (LPFRAME)pFrame->pFrameSibling, matrix );

  if ( pFrame->pFrameFirstChild )
    MoveFrame( (LPFRAME)pFrame->pFrameFirstChild, pFrame->matCombined );
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

void AnimatedModel::SetFrameMatrix( LPFRAME pFrame )
{
  if ( pFrame->Name != NULL )
  {
    AnimationMap::iterator animIt = animSets[curAnimSet].animMap.find( pFrame->Name );
    if ( animIt != animSets[curAnimSet].animMap.end() )
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

      D3DXMATRIX matFinal;
      vqs.GetMatrix( matFinal );

      pFrame->TransformationMatrix = matFinal;
    }
  }
}

void AnimatedModel::FrameMove( DWORD elapsedTime, const D3DXMATRIX &matWorld, ANIMCALLBACKFN callback )
{
  SetKeyFrame( elapsedTime );
  ForEachFrame( pFrameRoot, &AnimatedModel::SetFrameMatrix );
  MoveFrame( pFrameRoot, matWorld );
  if ( callback )
  {
    callback();
    MoveFrame( pFrameRoot, matWorld );
  }
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
  curAnimSet = std::min( index, (DWORD)( animSets.size() - 1 ) );
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

LPFRAME AnimatedModel::GetFrameRoot( void )
{
  return pFrameRoot;
}

inline float AnimatedModel::GetScale( void ) const
{
  return matScale._11;
}

void AnimatedModel::DrawCoordSys( LPFRAME pFrame )
{
  D3DCOLOR colors[3] = { 0xFFCC0000, 0xFF00CC00, 0xFF0000CC };

  float size = 30.f;

  for ( int i = 0; i < 3; ++i )
  {
    ColoredVertex line[2];
    (*line[0].ToLPD3DXVECTOR3())[i] = size;
    line[0].color = colors[i];
    line[1].color = colors[i];

    D3DXVec3TransformCoord( line[0].ToLPD3DXVECTOR3(), line[0].ToLPD3DXVECTOR3(), &pFrame->matCombined );
    D3DXVec3TransformCoord( line[1].ToLPD3DXVECTOR3(), line[1].ToLPD3DXVECTOR3(), &pFrame->matCombined );

    pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof( ColoredVertex ) );
  }
}

void AnimatedModel::ForEachFrame( LPFRAME pFrame, CHANGEFRAMEFN fn )
{
  ( this->*fn )( pFrame );

  if ( pFrame->pFrameSibling )
    ForEachFrame( (LPFRAME)pFrame->pFrameSibling, fn );

  if ( pFrame->pFrameFirstChild )
    ForEachFrame( (LPFRAME)pFrame->pFrameFirstChild, fn );
}

void AnimatedModel::DrawLinesToChildren( LPFRAME pFrame )
{
  LPFRAME pCurChild = (LPFRAME)pFrame->pFrameFirstChild;
  D3DXVECTOR3 padre, origin( 0.f, 0.f, 0.f );
  D3DXVec3TransformCoord( &padre, &origin, &pFrame->matCombined );
  while ( pCurChild )
  {
    D3DXVECTOR3 child;
    D3DXVec3TransformCoord( &child, &origin, &pCurChild->matCombined );
    boneLines.push_back( padre );
    boneLines.push_back( child );
    pCurChild = (LPFRAME)pCurChild->pFrameSibling;
  }
}

bool AnimatedModel::CreateSkeleton( void ) const
{
  boneLines.clear();
  ( (AnimatedModel *)this )->ForEachFrame( pFrameRoot, &AnimatedModel::DrawLinesToChildren );
  return !boneLines.empty();
}
