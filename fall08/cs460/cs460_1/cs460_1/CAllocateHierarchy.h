/////////////////////////////////////////////////
//
// File: "CAllocateHierarchy.h"
//
// Author: Jason Jurecka
//
// Creation Date: June 9, 2003
//
// Purpose: This is an Allocation class that is
//		used with the D3DXLoadMeshHierarchyFromX
//		function. It handles the Creation and Deletion
//		of Frames and Mesh Containers. The overloaded
//		functions are callbacks so there is no need
//		to call any of the functions in written code
//		just pass an Object of this class to the function
/////////////////////////////////////////////////

#ifndef _ALLOCATE_HIERARCHY_H_
#define _ALLOCATE_HIERARCHY_H_

#include "DerivedStructs.h"
#include "Macros.h"

class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
  public:
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ 
            LPCSTR Name, 
            CONST D3DXMESHDATA *pMeshData, 
            CONST D3DXMATERIAL *pMaterials, 
            CONST D3DXEFFECTINSTANCE *pEffectInstances, 
            DWORD NumMaterials, 
            CONST DWORD *pAdjacency, 
            LPD3DXSKININFO pSkinInfo, 
            LPD3DXMESHCONTAINER *ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};

#endif