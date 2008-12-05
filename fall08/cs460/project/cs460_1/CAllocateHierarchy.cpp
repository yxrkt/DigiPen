 /////////////////////////////////////////////////
//
// File: "CAllocateHierarchy.cpp"
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
// Purpose: This is an Allocation class that is
//		used with the D3DXLoadMeshHierarchyFromX
//		function. It handles the Creation and Deletion
//		of Frames and Mesh Containers. The overloaded
//		functions are callbacks, so there is no need
//		to call any of the functions in written code
//		just pass an Object of this class to the function
/////////////////////////////////////////////////
#include "CAllocateHierarchy.h"


HRESULT CAllocateHierarchy::CreateFrame( LPCTSTR Name, LPD3DXFRAME *ppNewFrame )
{
  LPFRAME pFrame = new FRAME;
  ZeroMemory( pFrame, sizeof( FRAME ) );

  *ppNewFrame = NULL;

  pFrame->Name = NULL;
  if ( Name )
  {
    pFrame->Name = new char[strlen( Name ) + 1];
    strcpy( pFrame->Name, Name );
  }

  pFrame->pFrameFirstChild  = NULL;
  pFrame->pFrameSibling     = NULL;
  pFrame->pMeshContainer    = NULL;
  D3DXMatrixIdentity( &pFrame->matCombined );
  D3DXMatrixIdentity( &pFrame->TransformationMatrix );

  *ppNewFrame = (LPD3DXFRAME)pFrame;

  return S_OK;
}

HRESULT CAllocateHierarchy::CreateMeshContainer(  THIS_ 
                                                  LPCSTR Name, 
                                                  CONST D3DXMESHDATA *pMeshData, 
                                                  CONST D3DXMATERIAL *pMaterials, 
                                                  CONST D3DXEFFECTINSTANCE *pEffectInstances, 
                                                  DWORD NumMaterials, 
                                                  CONST DWORD *pAdjacency, 
                                                  LPD3DXSKININFO pSkinInfo, 
                                                  LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
  HRESULT hr;

  LPMESHCONTAINER pMeshContainer = new MESHCONTAINER;
  ZeroMemory( pMeshContainer, sizeof( MESHCONTAINER ) );

  *ppNewMeshContainer = NULL;

  pMeshContainer->Name = NULL;
  if ( Name )
  {
    pMeshContainer->Name = new char[strlen( Name ) + 1];
    strcpy( pMeshContainer->Name, Name );
  }

  pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

  DWORD dwFaces = pMeshData->pMesh->GetNumFaces();

  pMeshContainer->NumMaterials = NumMaterials;

  pMeshContainer->pMaterials9 = new D3DMATERIAL9[pMeshContainer->NumMaterials];
  
  memcpy( pMeshContainer->pMaterials9, &pMaterials->MatD3D, sizeof( pMaterials->MatD3D ) );
  const D3DCOLORVALUE &ambient = pMaterials->MatD3D.Ambient;
  if ( ambient.r == 0.f && ambient.g == 0.f && ambient.b == 0.f )
    pMeshContainer->pMaterials9->Ambient = .5f * pMeshContainer->pMaterials9->Diffuse;

		//if(pMesh->pSkinInfo)
		//{
		//	//Create a copy of the mesh
		//	pMesh->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, 
		//		pMesh->MeshData.pMesh->GetFVF(), m_pd3dDevice, 
		//		&pMesh->pSkinMesh);

  // Multiply by 3 because there are three adjacent triangles
  pMeshContainer->pAdjacency = new DWORD[dwFaces * 3];
  memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof( DWORD ) * dwFaces * 3 );

  LPDIRECT3DDEVICE9 pd3dDevice = NULL;
  pMeshData->pMesh->GetDevice( &pd3dDevice );

  pMeshData->pMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshData->pMesh->GetFVF(),
                                  pd3dDevice, &pMeshContainer->MeshData.pMesh );
	
  pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[NumMaterials];
  for ( DWORD i = 0; i < NumMaterials; ++i )
  {
    pMeshContainer->ppTextures [i] = NULL;

    if( pMaterials[i].pTextureFilename && strlen( pMaterials[i].pTextureFilename ) > 0 )
    {
      hr = D3DXCreateTextureFromFile( pd3dDevice, pMaterials[i].pTextureFilename, &pMeshContainer->ppTextures[i] );
      if ( FAILED( hr ) )
      {
        pMeshContainer->ppTextures[i] = NULL;
      }
    }
  }

  if ( pSkinInfo )
  {
    // first save off the SkinInfo and original mesh data
    pMeshContainer->pSkinInfo = pSkinInfo;
    pSkinInfo->AddRef();

    // Will need an array of offset matrices to move the vertices from 
    //	the figure space to the bone's space
    UINT uBones = pSkinInfo->GetNumBones();
    pMeshContainer->pBoneOffsets = new D3DXMATRIX[uBones];

    //Create the arrays for the bones and the frame matrices
    pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[uBones];

    // get each of the bone offset matrices so that we don't need to 
    //	get them later
    for ( UINT i = 0; i < uBones; ++i )
      pMeshContainer->pBoneOffsets[i] = *( pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( i ) );

    pMeshContainer->MeshData.pMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshContainer->MeshData.pMesh->GetFVF(), 
                                                  pd3dDevice, &pMeshContainer->pSkinMesh );
	}
	else
	{
		pMeshContainer->pSkinInfo       = NULL;
		pMeshContainer->pBoneOffsets    = NULL;
		pMeshContainer->pSkinMesh       = NULL;
		pMeshContainer->ppFrameMatrices = NULL;
	}

  //Release the device
  SAFE_RELEASE( pd3dDevice );

	pMeshContainer->pMaterials = NULL;
	pMeshContainer->pEffects   = NULL;

	//pMeshContainer->MeshData.pMesh->OptimizeInplace(
	//	D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT,
	//	pMeshContainer->pAdjacency,NULL,NULL,NULL);

	// Set the output mesh container to the temp one
  *ppNewMeshContainer = pMeshContainer;
  pMeshContainer      = NULL;

	// Returns an HRESULT so give it the AOk result
  return S_OK;
}

HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree ) 
{
	LPFRAME pFrame = (LPFRAME)pFrameToFree;
	SAFE_DELETE_ARRAY( pFrame->Name )
  SAFE_DELETE( pFrame )
  return S_OK;
}

HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
  LPMESHCONTAINER pMeshContainer = (LPMESHCONTAINER)pMeshContainerBase;

  SAFE_DELETE_ARRAY( pMeshContainer->Name );
  SAFE_DELETE_ARRAY( pMeshContainer->pMaterials9 );
  SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
  SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
  SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsets );
  SAFE_DELETE_ARRAY( pMeshContainer->ppFrameMatrices );
  SAFE_DELETE_ARRAY( pMeshContainer->pAttributeTable );

  SAFE_RELEASE( pMeshContainer->pSkinMesh );
  SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
  SAFE_RELEASE( pMeshContainer->pSkinInfo );
  SAFE_DELETE( pMeshContainer );

  if ( pMeshContainer->ppTextures )
  {
    for ( UINT i = 0; i < pMeshContainer->NumMaterials; ++i )
      SAFE_RELEASE( pMeshContainer->ppTextures[i] );
  }

  return S_OK;
}
