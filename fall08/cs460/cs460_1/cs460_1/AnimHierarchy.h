#pragma once

#include <d3dx9.h>

// mesh container
struct AnimMC: public D3DXMESHCONTAINER
{
  LPDIRECT3DTEXTURE9 *pTextures;  // Array of texture pointers  
  D3DMATERIAL9       *pMaterials; // Array of materials
                                
	// Skinned mesh variables
  ID3DXMesh     *pSkinMesh;                     // The skin mesh
  D3DXMATRIX    *pBoneOffsets;                  // The bone matrix Offsets, one per bone
  LPD3DXMATRIX  *ppFrameCombinedMatrixPointer;  // Array of frame matrix pointers
};

// frame (bone) struct
struct AnimFrame: public D3DXFRAME
{
  //D3DXMATRIX exCombinedTransformationMatrix;
};

class AnimMeshHierarchy : public ID3DXAllocateHierarchy
{
  private:
    // callback to create a D3DXFRAME-derived object and initialize it
    STDMETHOD( CreateFrame )( LPCSTR name, LPD3DXFRAME *ppNewFrame );

    // callback to create a D3DXMESHCONTAINER-derived object and initialize it
    STDMETHOD( CreateMeshContainer )( LPCSTR Name, const D3DXMESHDATA *pMeshData, 
                                      const D3DXMATERIAL *pMaterials, const D3DXEFFECTINSTANCE *pEffectInstances,
                                      DWORD nMaterials, const DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                                      LPD3DXMESHCONTAINER *ppNewMeshContainer );

    // callback to release a D3DXFRAME-derived object
    STDMETHOD( DestroyFrame )( LPD3DXFRAME pFrame );

    // callback to release a D3DXMESHCONTAINER-derived object
    STDMETHOD( DestroyMeshContainer )( LPD3DXMESHCONTAINER pMeshContainerToFree );

  public:
    AnimMeshHierarchy();
};
