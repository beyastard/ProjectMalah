/*
 * FILE: ML_EventPt.cpp
 *
 * DESCRIPTION: Event point class in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_EventPt.h"
#include "ML_GamePlay.h"

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
//	Implement CMLEventPt
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CEditorEventPt
IMPLEMENT_ASERIAL(CMLEventPt, AData, 1)

CMLEventPt::CMLEventPt()
{
	memset(m_aILD, 0, sizeof (m_aILD));
}

CMLEventPt::~CMLEventPt()
{
	ReleaseEventPt();
}

//	Release resources
bool CMLEventPt::ReleaseEventPt()
{
	int i;

	//	Release event point ILDs
	for (i=0; i < NUM_EPILD; i++)
	{
		DWORD dwFlags = m_MLEventPt.m_Data.dwOwnFlags & m_MLEventPt.m_Data.dwNotFixFlags;

		if ((dwFlags & (1 << i)) && m_aILD[i])
			delete m_aILD[i];
	}

	memset(m_aILD, 0, sizeof (m_aILD));

	return true;
}

//	Load event point
bool CMLEventPt::Load(AArchive& ar)
{
	if (!m_MLEventPt.Load(ar))
	{
		ADEBUG_TRACE("Load error!");
		return false;
	}

	DWORD dwFlags = m_MLEventPt.m_Data.dwOwnFlags & m_MLEventPt.m_Data.dwNotFixFlags;
	for (int i=0; i < NUM_EPILD; i++)
	{
		if (dwFlags & (1 << i))
		{
			if (!(m_aILD[i] = new CMLLevelDataInt))
			{
				ADEBUG_TRACE("Not enough memory!");
				return false;
			}
			
			m_aILD[i]->Load(ar);
		}
	}

	return true;
}

//	Save event point
bool CMLEventPt::Save(AArchive& ar)
{
	if (!m_MLEventPt.Save(ar))
	{
		ADEBUG_TRACE("Save error!");
		return false;
	}

	DWORD dwFlags = m_MLEventPt.m_Data.dwOwnFlags & m_MLEventPt.m_Data.dwNotFixFlags;
	for (int i=0; i < NUM_EPILD; i++)
	{
		if (dwFlags & (1 << i))
			m_aILD[i]->Save(ar);
	}

	return true;
}

/*	Get next event

	Return true if next event was got, otherwise return false if no event can be got

	pInterator (in, out): event interator
*/
bool CMLEventPt::GetNextEvent(EVENTINTERATOR* pInterator)
{
	MLEVENTPTDATA* pData = &m_MLEventPt.m_Data;

	if (!pData->iNumEvent)
		return false;

	pInterator->iEventCnt++;
	if (pInterator->iEventCnt >= pData->iNumEvent)
	{
		if (pData->iNumLoop > 0)
		{
			pInterator->iLoopCnt++;
			if (pInterator->iLoopCnt >= pData->iNumLoop)
				return false;
		}

		pInterator->iEventCnt = 0;
	}

	pInterator->Event = pData->aEvents[pInterator->iEventCnt];

	return true;
}

/*	Get random position in the event point

	Note: position returned by this function doesn't include height information,
		  and is always 0.0f.
*/
A3DVECTOR3 CMLEventPt::GetRandomPos()
{
	MLEVENTPTDATA* pData = &m_MLEventPt.m_Data;
	A3DVECTOR3 vPos = pData->vPos;

	switch (pData->iType)
	{
	case EPT_ROUND:
	{
		float fDist = MLA_Random(0.0f, pData->fRadius);

		A3DVECTOR3 vDir;
		MLA_GenRandomVectorH(vDir);
		
		vPos = pData->vPos + vDir * fDist;
		break;
	}
	case EPT_ARC:
	{
		float fDegree = MLA_Random(-pData->fDegree * 0.5f, pData->fDegree * 0.5f);
		A3DMATRIX4 mat = RotateY(DEG2RAD(fDegree));
		vPos = pData->vCenter + mat * (pData->vPos - pData->vCenter);
		break;
	}
	case EPT_LINE:
	{
		float fLen = MLA_Random(0.0f, pData->fLineLen);
		vPos = pData->vLineEnd + pData->vLineDir * fLen;
		break;
	}
	case EPT_DBARC:
	{
		float fDegree = MLA_Random(-pData->fDegree * 0.5f, pData->fDegree * 0.5f);
		A3DMATRIX4 mat = RotateY(DEG2RAD(fDegree));

		A3DVECTOR3 vDir;
		float fLen = Normalize(pData->vPos - pData->vCenter, vDir);
		fLen = MLA_Random(fLen - pData->fHalfWidth, fLen + pData->fHalfWidth);
		vPos = pData->vCenter + mat * (vDir * fLen);
		break;
	}
	}

	return vPos;
}

//	Check whether specified position in event point
bool CMLEventPt::PosInEventPt(A3DVECTOR3& vPos)
{
	A3DVECTOR3 vxzPos(vPos.x, 0.0f, vPos.z);
	MLEVENTPTDATA* pData = &m_MLEventPt.m_Data;
	bool bRet = false;

	switch (pData->iType)
	{
	case EPT_ROUND:
	{
		float fRadius = Magnitude(vxzPos - pData->vPos);
		if (fRadius <= pData->fRadius)
			bRet = true;

		break;
	}
	case EPT_ARC:
	{
		break;
	}
	case EPT_LINE:
	{
		break;
	}
	case EPT_DBARC:
	{
		A3DVECTOR3 vRadius, vDist;
		float fRadius = Normalize(pData->vPos - pData->vCenter, vRadius);
		float fDist	  = Normalize(vxzPos - pData->vCenter, vDist);

		if (fDist < fRadius - pData->fHalfWidth ||
			fDist > fRadius + pData->fHalfWidth)
			break;

		float fDot = (float)cos(DEG2RAD(pData->fDegree * 0.5f));
		if (DotProduct(vDist, vRadius) >= fDot)
			bRet = true;

		break;
	}
	}

	return bRet;
}

/*	Get ILD index of general AI set

	Return ILD index of general AI, -1 means failure.

	iILDIndex: ILD index of route
*/
int	CMLEventPt::GetGeneralAIILD(int iILDIndex)
{
	MLEVENTPTDATA* pData = GetMLEventPtData();
	int iILD = -1;

	switch (iILDIndex)
	{
	case EILD_AKCOPTERPOSS:		iILD = ILD_PER_ATTACKCOPTER;	break;
	}

	return iILD;
}

/*	Get AI ILD parameter value of level

	Return value of specified ILD parameter.

	iLevel: mission level
	iILD: ILD index
	iDefVal: default value
*/
int	CMLEventPt::GetAIParameter(int iLevel, int iILDIndex, int iDefVal)
{
	MLEVENTPTDATA* pData = GetMLEventPtData();
	DWORD dwOwnFlags = pData->dwOwnFlags;
	DWORD dwNoFixFlags = pData->dwNotFixFlags;
	int iValue = iDefVal, iILD;

	if (dwOwnFlags & (1 << iILDIndex))	//	Use own value
	{
		if (dwNoFixFlags & (1 << iILDIndex))
		{
			//	Use variable value
			assert(m_aILD[iILDIndex]);
			iValue = m_aILD[iILDIndex]->GetAt(iLevel);
		}
	}
	else	//	Use general AI set value
	{
		if ((iILD = GetGeneralAIILD(iILDIndex)) >= 0)
			iValue = GETAIILDVALUE(iILD, iLevel);
	}

	return iValue;
}

//	Called when begin a mission
bool CMLEventPt::OnBeginMission(int iMission)
{
	MLEVENTPTDATA* pData = GetMLEventPtData();

	m_iAIAttackCopter = GetAIParameter(iMission, EILD_AKCOPTERPOSS, pData->iAkCopterPoss);

	OnResetMission();

	return true;
}

//	Called when reset mission
bool CMLEventPt::OnResetMission()
{
	return true;
}


