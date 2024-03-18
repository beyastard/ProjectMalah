/*
 * FILE: ML_VSSetScene.cpp
 *
 * DESCRIPTION: VSSet scene class used in game Malah.
 *
 * CREATED BY: duyuxin, 2002/7/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_VSSetScene.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement of CMLVSSetScene
//
///////////////////////////////////////////////////////////////////////////

CMLVSSetScene::CMLVSSetScene() : A3DVSSScene()
{
	m_iRouteIdx		= -1;
	m_pMergedBuf	= NULL;
	m_iNumVSSBytes	= 0;
	m_iSampleDegree = 18;
	m_iPeriod		= 360 / m_iSampleDegree;
	m_iNumSection	= m_iPeriod * m_iPeriod / 2;

	memset(m_aViewAreas, 0, sizeof (m_aViewAreas));
}

/*	Initialize object

	Return true for success, otherwise return false.

	pA3DDevice: valid A3D device object
*/
bool CMLVSSetScene::Init(A3DDevice* pA3DDevice)
{
	if (!A3DVSSScene::Init(pA3DDevice))
	{
		g_pA3DErrLog->ErrLog("CMLVSSetScene::Init, Failed to call A3DVSSScene::Init.");
		return false;
	}

	return true;
}

/*	Release resources

	Return true for success, otherwise return false.
*/
bool CMLVSSetScene::Release()
{
	int i;
	for (i=0; i < 4; i++)
	{
		if (m_aViewAreas[i].pVSSBytes)
		{
			delete [] m_aViewAreas[i].pVSSBytes;
			m_aViewAreas[i].pVSSBytes = NULL;
		}
	}

	if (m_pMergedBuf)
	{
		delete [] m_pMergedBuf;
		m_pMergedBuf = NULL;
	}

	memset(m_aViewAreas, 0, sizeof (m_aViewAreas));

	return A3DVSSScene::Release();
}

/*	Load VSS file

	Return true for success, otherwise return false.

	szFile: VSS file's name 
*/
bool CMLVSSetScene::LoadVSSFile(char* szFile)
{
	if (m_iRouteIdx >= 0)
	{
		//	Release old VSS file
		RemoveRoute(m_iRouteIdx);
	}

	//	Load route
	if ((m_iRouteIdx = LoadRoute(szFile, true)) < 0)
	{
		g_pA3DErrLog->ErrLog("CMLVSSetScene::LoadVSSFile, Failed to load route file");
		return false;
	}

	ROUTE* pRoute = &m_aRoutes[m_iRouteIdx];

	//	Success of LoadRoute can ensure that secen has been loaded
	m_iNumVSSBytes = pRoute->pRoute->GetVisByteNum();

	for (int i=0; i < 4; i++)
	{
		if (!m_aViewAreas[i].pVSSBytes)
			m_aViewAreas[i].pVSSBytes = new BYTE[m_iNumVSSBytes];

		m_aViewAreas[i].iSecion   = -1;
	}
	
	if (!m_pMergedBuf)
	{
		if (!(m_pMergedBuf = new BYTE[m_iNumVSSBytes]))
		{
			g_pA3DErrLog->ErrLog("CMLVSSetScene::LoadVSSFile, not enough memory for VSS merged buffer.");
			return false;
		}
	}

	//	Calculate sample degree
	m_iNumSection	= pRoute->pRoute->GetSectionNumber();
	m_iPeriod		= pRoute->pRoute->GetRouteParams().iMaxSurf / 10000;
	m_iSampleDegree = 360 / m_iPeriod;

	return true;
}

/*	Update visible surface set

	Return true for success, otherwise return false.
*/
bool CMLVSSetScene::UpdateVisibleSets(A3DViewport* pCurrentViewport)
{
	if (!m_nNumTextures)
		return true;

	int i, j;

	for (i=0; i < m_nNumTextures; i++)
		m_pTextureRecords[i].nFaceVisible = 0;

	if (m_bRenderAll)
	{
		for (i=0; i < m_nNumFaces; i++)
		{
			int idTex = m_pFaceRecords[i].nTexID;
			int nFaceVisible = m_pTextureRecords[idTex].nFaceVisible++;

			memcpy(m_pTextureRecords[idTex].pVerts + nFaceVisible * 3, m_pAllFaces + i * 3, sizeof (A3DVERTEX) * 3);
		}
	}
	else
	{
		if (m_iRouteIdx < 0)
			return true;

		//	Calcualte view area
		A3DVECTOR3 vDir = pCurrentViewport->GetCamera()->GetDir();
		int aSects[4];
		CalcViewArea(pCurrentViewport, aSects);

		bool bMerge = false;
		ROUTE* pRoute = &m_aRoutes[m_iRouteIdx];

		for (i=0; i < 4; i++)
		{
			for (j=0; j < 4; j++)
			{
				if (aSects[i] == m_aViewAreas[j].iSecion)
					break;
			}

			if (j < 4)	//	VSS bytes of this section has been calculated
			{
				if (i != j)
				{
					bMerge = true;
					memcpy(m_aViewAreas[i].pVSSBytes, m_aViewAreas[j].pVSSBytes, m_iNumVSSBytes);
					m_aViewAreas[i].iSecion = m_aViewAreas[j].iSecion;
				}
			}
			else
			{
				//	Reload vss bytes of this section
				pRoute->pRoute->GetSectionVisBytes(aSects[i], m_aViewAreas[i].pVSSBytes);
				bMerge = true;

				m_aViewAreas[i].iSecion = aSects[i];
			}
		}

		if (bMerge)
		{
			//	Re-merge vss bytes of all visible section
			MergeVSSBytes();

			//	Get visible surfaces indices
			GetVisSurfIndices(pRoute->aVisSurfs, &pRoute->iNumVisSurf);
		}

		for (i=0; i < pRoute->iNumVisSurf; i++)
		{
			int iFace = pRoute->aVisSurfs[i];
			int idTex = m_pFaceRecords[iFace].nTexID;
			int nFaceVisible = m_pTextureRecords[idTex].nFaceVisible++;
			
			memcpy(m_pTextureRecords[idTex].pVerts + nFaceVisible * 3, m_pAllFaces + iFace * 3, sizeof (A3DVERTEX) * 3);
		}
	}

	m_nNumVisibleFaces = 0;

	for (i=0; i < m_nNumTextures; i++)
	{
#ifdef A3DSCENE_USESTREAM
		if (m_pTextureRecords[i].nFaceVisible > 0)
			m_pTextureRecords[i].pStream->SetVerts((LPBYTE)m_pTextureRecords[i].pVerts, m_pTextureRecords[i].nFaceVisible * 3);
#endif

		m_nNumVisibleFaces += m_pTextureRecords[i].nFaceVisible;
	}

	return true;
}

/*	Calculate view area.

	pViewport: current viewport
	aSects: array used to receive sections in view area.
*/
void CMLVSSetScene::CalcViewArea(A3DViewport* pViewport, int aSects[])
{
	A3DVECTOR3 vDir = pViewport->GetCamera()->GetDir();
	A3DVECTOR3 vUp	= pViewport->GetCamera()->GetUp();

	A3DVECTOR3 vxzDir = A3DVECTOR3(vDir.x, 0.0f, vDir.z);
	float fLongitude, fLatitude, fLen, fDegree;

	//	Adjust dir's projection on xz plane
	if ((fLen = Magnitude(vxzDir)))
		vxzDir = vxzDir * (1.0f / fLen);
	else if (vDir.y >= 0.0f)
		vxzDir = Normalize(A3DVECTOR3(-vUp.x, 0.0f, -vUp.z));
	else
		vxzDir = Normalize(A3DVECTOR3(vUp.x, 0.0f, vUp.z));
	
	fDegree = (float)RAD2DEG(acos(vxzDir.x));

	if (vxzDir.x >= 0.0f)
	{
		if (vxzDir.z >= 0.0f)
			fLongitude = fDegree;
		else
			fLongitude = 360.0f - fDegree;
	}
	else
	{
		if (vxzDir.z >= 0.0f)
			fLongitude = fDegree;
		else
			fLongitude = 360.0f - fDegree;
	}

	//	Calcualte latitude
	fLatitude = (float)RAD2DEG(asin(vDir.y)) + 90.0f;

	if (fLongitude < 0.0f)
		fLongitude = 0.1f;

	if (fLatitude < 0.0f)
		fLatitude = 0.1f;

	int iBase = ((int)floor(fLatitude) / m_iSampleDegree) * m_iPeriod + (int)floor(fLongitude) / m_iSampleDegree;

	aSects[0] = iBase;

	if (!((iBase + 1) % m_iPeriod))
		aSects[1] = iBase + 1 - m_iPeriod;
	else
		aSects[1] = iBase + 1;

	aSects[2] = iBase + m_iPeriod;
	aSects[3] = aSects[1] + m_iPeriod;

	if (aSects[2] > m_iNumSection)
		aSects[2] = iBase - m_iPeriod;

	if (aSects[3] > m_iNumSection)
		aSects[3] = iBase - m_iPeriod + 1;
}

//	Merge VSS bytes
void CMLVSSetScene::MergeVSSBytes()
{
	long* p, *p0, *p1, *p2, *p3;
	int i, iNumLong, iTail;
	
	iNumLong = m_iNumVSSBytes / sizeof (long);
	iTail	 = m_iNumVSSBytes % sizeof (long);

	p  = (long*)m_pMergedBuf;
	p0 = (long*)m_aViewAreas[0].pVSSBytes;
	p1 = (long*)m_aViewAreas[1].pVSSBytes;
	p2 = (long*)m_aViewAreas[2].pVSSBytes;
	p3 = (long*)m_aViewAreas[3].pVSSBytes;

	for (i=0; i < iNumLong; i++)
		*p++ = *p0++ | *p1++ | *p2++ | *p3++;

	//	This may be not necessary, because VSSetRoute always ensure buffer
	//	length can be divided exactly by sizeof (long)
	if (iTail)
	{
		int iCnt = (BYTE*)p - m_pMergedBuf;

		for (i=0; i < iTail; i++, iCnt++)
		{
			m_pMergedBuf[iCnt] = m_aViewAreas[0].pVSSBytes[iCnt] |
								 m_aViewAreas[1].pVSSBytes[iCnt] |
								 m_aViewAreas[2].pVSSBytes[iCnt] |
								 m_aViewAreas[3].pVSSBytes[iCnt];
		}
	}
}

/*	Get visible surfaces indices

	pIdxBuf: buffer used to store surface indices
	piNumVisSuf (out): visible surface number
*/
void CMLVSSetScene::GetVisSurfIndices(int* pIdxBuf, int* piNumVisSuf)
{
	int i, j, iNumVis, iIndex;
	BYTE* pByte = m_pMergedBuf;	

	iNumVis	= 0;
	iIndex	= 0;
	
	for (i=0; i < m_iNumVSSBytes; i++, pByte++)
	{
		if (!(*pByte))
		{
			iIndex += 8;
			continue;
		}

		BYTE byByte = *pByte;
		BYTE byMask = 1;

		for (j=0; j < 8; j++)
		{
			if (byByte & byMask)
				pIdxBuf[iNumVis++] = iIndex + j;

			byMask <<= 1;
		}

		iIndex += 8;
	}

	if (piNumVisSuf)
		*piNumVisSuf = iNumVis;
}


