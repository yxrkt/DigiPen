#include "AnimHierarchy.h"

HRESULT AnimMeshHierarchy::CreateFrame( LPCSTR name, LPD3DXFRAME *ppNewFrame )
{
  ppNewFrame = NULL;

  AnimFrame *newFrame = new AnimFrame;
  ZeroMemory( newFrame, sizeof( AnimFrame ) );

  D3DXMatrixIdentity(&newFrame->TransformationMatrix);
  //D3DXMatrixIdentity(&newFrame->exCombinedTransformationMatrix);

  newFrame->pMeshContainer    = NULL;
  newFrame->pFrameSibling     = NULL;
  newFrame->pFrameFirstChild  = NULL;

  *ppNewFrame = newFrame;

  if ( name && strlen( name ) )
    strcpy( newFrame->Name, name );

  return S_OK;
}

HRESULT AnimMeshHierarchy::CreateMeshContainer(
    LPCSTR name,
    const D3DXMESHDATA        *meshData,
    const D3DXMATERIAL        *materials,
    const D3DXEFFECTINSTANCE  *effectInstances,
    DWORD                      nMaterials,
    const DWORD               *adjacency,
    LPD3DXSKININFO             pSkinInfo,
    LPD3DXMESHCONTAINER       *retNewMeshContainer )
{    
	D3DXMESHCONTAINER_EXTENDED *newMeshContainer = new D3DXMESHCONTAINER_EXTENDED;
	ZeroMemory(newMeshContainer, sizeof(D3DXMESHCONTAINER_EXTENDED));

	*retNewMeshContainer = NULL;

	// The mesh name (may be 0) needs copying over
	if ( name && strlen( name ) )
		newMeshContainer->name=CUtility::DuplicateCharString(name);

	// The mesh type (D3DXMESHTYPE_MESH, D3DXMESHTYPE_PMESH or D3DXMESHTYPE_PATCHMESH)
	if ( meshData->Type != D3DXMESHTYPE_MESH )
	{
		DestroyMeshContainer(newMeshContainer);
		return E_FAIL;
	}

	newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	
	// Adjacency data - holds information about triangle adjacency, required by the ID3DMESH object
	DWORD dwFaces = meshData->pMesh->GetNumFaces();
	newMeshContainer->pAdjacency = new DWORD[dwFaces*3];
	memcpy(newMeshContainer->pAdjacency, adjacency, sizeof(DWORD) * dwFaces*3);
	
	// Get the Direct3D device, luckily this is held in the mesh itself (Note: must release it when done with it)
	LPDIRECT3DDEVICE9 pd3dDevice = 0;
	meshData->pMesh->GetDevice(&pd3dDevice);

	// Changed 24/09/07 - can just assign pointer and add a ref rather than need to clone
	newMeshContainer->MeshData.pMesh=meshData->pMesh;
	newMeshContainer->MeshData.pMesh->AddRef();

	// Create material and texture arrays. Note that I always want to have at least one
	newMeshContainer->NumMaterials = max(nMaterials,1);
	newMeshContainer->exMaterials = new D3DMATERIAL9[newMeshContainer->NumMaterials];
	newMeshContainer->exTextures  = new LPDIRECT3DTEXTURE9[newMeshContainer->NumMaterials];

	ZeroMemory(newMeshContainer->exTextures, sizeof(LPDIRECT3DTEXTURE9) * newMeshContainer->NumMaterials);

	if (nMaterials>0)
	{
		// Load all the textures and copy the materials over		
		for(DWORD i = 0; i < nMaterials; ++i)
		{
			newMeshContainer->exTextures[i] = 0;	
			newMeshContainer->exMaterials[i]=materials[i].MatD3D;

			if(materials[i].pTextureFilename)
			{
				std::string texturePath(materials[i].pTextureFilename);
				if (CUtility::FindFile(&texturePath))
				{
					// Use the D3DX function to load the texture
					if(FAILED(D3DXCreateTextureFromFile(pd3dDevice, texturePath.c_str(),
						&newMeshContainer->exTextures[i])))
					{
						CUtility::DebugString("Could not load texture: "+texturePath+"\n");					
					}
				}
				else
				{
					CUtility::DebugString("Could not find texture: "+ToString(materials[i].pTextureFilename)+"\n");					
				}
			}
		}
	}
	else    
	// make a default material in the case where the mesh did not provide one
    {
		ZeroMemory(&newMeshContainer->exMaterials[0], sizeof( D3DMATERIAL9 ) );
        newMeshContainer->exMaterials[0].Diffuse.r = 0.5f;
        newMeshContainer->exMaterials[0].Diffuse.g = 0.5f;
        newMeshContainer->exMaterials[0].Diffuse.b = 0.5f;
        newMeshContainer->exMaterials[0].Specular = newMeshContainer->exMaterials[0].Diffuse;
		newMeshContainer->exTextures[0]=0;
    }

	// If there is skin data associated with the mesh copy it over
	if (pSkinInfo)
	{
		// save off the SkinInfo
	    newMeshContainer->pSkinInfo = pSkinInfo;
	    pSkinInfo->AddRef();

	    // Need an array of offset matrices to move the vertices from the figure space to the bone's space
	    UINT numBones = pSkinInfo->GetNumBones();
	    newMeshContainer->exBoneOffsets = new D3DXMATRIX[numBones];

		// Create the arrays for the bones and the frame matrices
		newMeshContainer->exFrameCombinedMatrixPointer = new D3DXMATRIX*[numBones];

	    // get each of the bone offset matrices so that we don't need to get them later
	    for (UINT i = 0; i < numBones; i++)
	        newMeshContainer->exBoneOffsets[i] = *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));

		CUtility::DebugString("Mesh has skinning info.\n Number of bones is: "+ToString(numBones)+"\n");
        // Note: in the Microsoft samples a GenerateSkinnedMesh function is called here in order to prepare
		// the skinned mesh data for optimial hardware acceleration. As mentioned in the notes this sample
		// does not do hardware skinning but instead uses software skinning.
	}
	else	
	{
		// No skin info so 0 all the pointers
		newMeshContainer->pSkinInfo = 0;
		newMeshContainer->exBoneOffsets = 0;
		newMeshContainer->exSkinMesh = 0;
		newMeshContainer->exFrameCombinedMatrixPointer = 0;
	}

	// When we got the device we caused an internal reference count to be incremented
	// So we now need to release it
	pd3dDevice->Release();

	// The mesh may contain a reference to an effect file
	if (effectInstances)
	{
		if (effectInstances->pEffectFilename)
			CUtility::DebugString("This .x file references an effect file. Effect files are not handled by this demo\n");
	}
	
	// Set the output mesh container pointer to our newly created one
	*retNewMeshContainer = newMeshContainer;    

	return S_OK;
}