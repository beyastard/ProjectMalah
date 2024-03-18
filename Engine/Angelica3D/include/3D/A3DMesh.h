/*
 * FILE: A3DMesh.h
 *
 * DESCRIPTION: Class representing one mesh in A3D Engine
 *
 * CREATED BY: Hedi, 2001/12/3
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _A3DMESH_H_
#define _A3DMESH_H_

#include "A3DPlatform.h"
#include "A3DObject.h"
#include "A3DVertex.h"
#include "A3DMaterial.h"
#include "A3DTexture.h"
#include "A3DDevice.h"
#include "A3DStream.h"

class A3DBox;

class A3DMesh : public A3DObject
{
private:
	A3DStream *		m_pA3DStream;

	bool			m_bHWIMesh;	//Using outside graphics engine;

	bool			m_bWire;
	A3DVERTEX **	m_ppVertsBuffer;
	WORD *			m_pIndices;
	int				m_nFrameCount;
	int				m_nFrame;
	
	int				m_nVertCount;
	int				m_nIndexCount;
	int				m_nShowVertCount;
	int				m_nShowIndexCount;

	A3DTexture *	m_pTexture;
	A3DMaterial 	m_Material;	
	A3DMaterial	*	m_pMaterialHost;	// material that host want me to use, if this is NULL, I will use my own m_Material;

	A3DTexture *	m_pDetailTexture;
	A3DMATRIX4		m_matDetailTM;

	A3DMESH_PROP	m_property;

	A3DDevice *		m_pA3DDevice;

	A3DAABB *		m_pAutoAABBs;
	A3DOBB *		m_pAutoOBBs;
	A3DBox *		m_pA3DBox;

	DWORD			m_MeshCollectorStoreHandle;

	bool			m_bStreamFilled;

	char			m_szTextureMap[MAX_PATH];
	char			m_szDetailTextureMap[MAX_PATH];

public:
	A3DMesh();
	~A3DMesh();

	bool Init(A3DDevice * pDevice, int nFrameCount, int nVertCount, int nIndexCount, bool bHasLOD, bool bWire=false);
	bool SetMaterial(A3DCOLORVALUE clAmbient, A3DCOLORVALUE clDiffuse, A3DCOLORVALUE clEmissive, A3DCOLORVALUE clSpecular, FLOAT fShiness, bool b2Sided=false);
	bool SetTexture(char * szTextureName);
	bool SetDetailTexture(char * szDetailTextureName, A3DMATRIX4 matDetailTM);

	inline DWORD GetMeshCollectorStoreHandle() { return m_MeshCollectorStoreHandle; }

	bool SetIndices(WORD * pIndices, int nIndexCount);
	bool SetVerts(int nFrame, A3DVERTEX * pVerts, int nVertCount);
	inline A3DVERTEX * GetVerts(int i) { return m_ppVertsBuffer[i]; }
	inline WORD * GetIndices() { return m_pIndices; }

	bool Release();
	bool BuildAutoAABB();
	bool BuildAutoOBB();
	
	inline bool GetWireFrame() { return m_bWire; }
	inline A3DMaterial * GetMaterial() { return &m_Material; }
	inline A3DTexture * GetTexture() { return m_pTexture; }
	inline A3DTexture * GetDetailTexture() { return m_pDetailTexture; }
	inline A3DMATRIX4 GetDetailTM() { return m_matDetailTM; }
	inline int GetVertCount() { return m_nVertCount; }
	inline int GetIndexCount() { return m_nIndexCount; }
	inline int GetShowVertCount() { return m_nShowVertCount; }
	inline int GetShowIndexCount() { return m_nShowIndexCount; }
	inline int GetFrameCount() { return m_nFrameCount; }
	inline int GetFrame() { return m_nFrame; }

	const A3DAABB& GetMeshAutoAABB(int nIndex) { return m_pAutoAABBs[nIndex]; }
	const A3DOBB& GetMeshAutoOBB(int nIndex) { return m_pAutoOBBs[nIndex]; }

	// Raytrace in triangles;
	bool RayTrace(A3DVECTOR3& vecStart, A3DVECTOR3& vecDelta, A3DVECTOR3 * pvecPos, A3DVECTOR3 * pvecNormal, float * pvFraction);

	//User defined properties;
	inline void SetProperty(A3DMESH_PROP * pProp) { m_property = *pProp; }
	inline A3DMESH_PROP GetProperty() { return m_property; }
	inline bool Is2Sided() { return m_property.b2Sided; }
	inline bool CanShootThrough() { return m_property.bCanShootThrough; }
	inline bool CanPickThrough() { return m_property.bCanPickThrough; }
	inline bool IsAlphaMesh() { return (m_pTexture && m_pTexture->IsAlphaTexture()) || m_Material.IsAlphaMaterial(); }
	
	bool RenderToBuffer(A3DViewport * pCurrentViewport, int nStartVert, A3DVERTEX * pVertexBuffer, WORD * pIndices, A3DMATRIX4 absoluteTM,  int nCurrentFrame, int * pNewVerts, int * pNewIndices);
	bool Render(A3DViewport * pCurrentViewport);
	bool RenderDirect(A3DViewport * pCurrentViewport);

	bool UpdateLOD(A3DViewport * pCurrentViewport, A3DMATRIX4 matWorld);

	bool UpdateToFrame(int nFrame);
	bool UpdateVertexBuffer();
	bool UpdateIndexBuffer(WORD * pIndices, int nShowIndexCount);

	inline void SetHostMaterial(A3DMaterial * pMaterialHost) { m_pMaterialHost = pMaterialHost; }

	bool Save(AFile * pFileToSave);
	bool Load(A3DDevice * pDevice, AFile * pFileToLoad);

private:
	bool		m_bHasLOD;
	WORD*		m_pMapTable;		//	Index map tab used to do LOD
	WORD*		m_pRDIndices;		//	Array used to store indices passed to rendering
	int			m_iLODLimit;		//	The minimum number of vertex should be rendered
	FLOAT		m_vLODMinDis;		//	The minimum distance that this mesh begin to collapse vertex;
	FLOAT		m_vLODMaxDis;		//  The maximum distance that this mesh will reach its iLODLimit vert

public:
	bool SetMapTable(WORD* pMapTable, int iSize);	//	Set index map tab
	int PrepareIndex(WORD * pIndex, int iVertLimit, int * pnNewVertCount);//	Prepare index used to render mesh
	void SetLODLimit(int iLimit)	{	m_iLODLimit = iLimit;		}
	int	 GetLODLimit()				{	return m_iLODLimit;			}
	bool IsLODMesh()				{	return m_bHasLOD;	}
	void SetLODMaxDis(FLOAT vLODMaxDis) { m_vLODMaxDis = vLODMaxDis; }
	FLOAT GetLODMaxDis() { return m_vLODMaxDis; }
	void SetLODMinDis(FLOAT vLODMinDis) { m_vLODMinDis = vLODMinDis; }
	FLOAT GetLODMinDis() { return m_vLODMinDis; }

	inline WORD MapIndex(WORD wIndex, int iLimit);

public:
	bool GetNbrTriangle(int nTriIndex, int * pnNbrTriIndex);

	inline char * GetTextureMap() { return m_szTextureMap; }
	inline char * GetDetailTextureMap() { return m_szDetailTextureMap; }
};

WORD A3DMesh::MapIndex(WORD wIndex, int iLimit)
{
	while (wIndex >= iLimit)
		wIndex = m_pMapTable[wIndex];
	
	return wIndex;
}

typedef A3DMesh * PA3DMesh;
#endif