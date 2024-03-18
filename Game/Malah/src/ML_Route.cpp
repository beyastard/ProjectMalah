/*
 * FILE: ML_Route.cpp
 *
 * DESCRIPTION: Route class in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Route.h"
#include "ML_GamePlay.h"
#include "ML_ManAI.h"

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
//	Implement CMLRoute
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLRoute
IMPLEMENT_ASERIAL(CMLRoute, AData, 1)

CMLRoute::CMLRoute()
{
	m_aEventPts			= NULL;
	m_iNumEventPt		= 0;
	m_iNumAI			= 0;
	m_iGenTotalNum		= 0;
	m_iTotalCnt			= 0;

	m_iAIMSPrecise		= 0;
	m_iAIMSInterval		= 0;
	m_iAISSPrecise		= 0;
	m_iAISSInterval		= 0;
	m_iAICSPrecise		= 0;
	m_iAICSInterval		= 0;
	m_iAIGenInterval	= 0;
	m_iAIGenNumber		= 0;
	m_iAIGenTurn		= 0;
	m_iAIGenTime		= 0;
	m_iAIMaxNumber		= 0;

	memset(m_aILD, 0, sizeof (m_aILD));
}

CMLRoute::~CMLRoute()
{
	ReleaseRoute();
}

//	Release resources
bool CMLRoute::ReleaseRoute()
{
	int i;

	//	Release event point data
	if (m_aEventPts)
	{
		for (i=0; i < m_iNumEventPt; i++)
		{
			if (m_aEventPts[i])
				delete m_aEventPts[i];
		}
		
		delete m_aEventPts;
		m_aEventPts = NULL;
	}

	m_iNumEventPt = 0;

	//	Release Route ILDs
	for (i=0; i < NUM_ROUTEILD; i++)
	{
		DWORD dwFlags = m_MLRoute.m_Data.dwOwnFlags & m_MLRoute.m_Data.dwNotFixFlags;

		if ((dwFlags & (1 << i)) && m_aILD[i])
			delete m_aILD[i];
	}

	memset(m_aILD, 0, sizeof (m_aILD));

	return true;
}

//	Load route
bool CMLRoute::Load(AArchive& ar)
{
	if (!m_MLRoute.Load(ar))
	{
		ADEBUG_TRACE("Load error!");
		return false;
	}

	//	Release old resources
	ReleaseRoute();

	//	Load event point number
	int i;
	if (!ar.Read(&m_iNumEventPt, sizeof (m_iNumEventPt)))
	{
		ADEBUG_TRACE("Load error!");
		return false;
	}

	if (!(m_aEventPts = (CMLEventPt**)malloc(sizeof (CMLEventPt*) * m_iNumEventPt)))
	{
		ADEBUG_TRACE("Not enough memory!");
		return false;
	}

	for (i=0; i < m_iNumEventPt; i++)
	{
		//	Create object
		CMLEventPt* ptr = new CMLEventPt;

		//	Load data
		if (!ptr->Load(ar))
		{
			ADEBUG_TRACE("Load error!");
			return false;
		}

		m_aEventPts[i] = ptr;
	}

	//	Route ILDs
	DWORD dwFlags = m_MLRoute.m_Data.dwOwnFlags & m_MLRoute.m_Data.dwNotFixFlags;
	for (i=0; i < NUM_ROUTEILD; i++)
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

//	Save route
bool CMLRoute::Save(AArchive& ar)
{
	if (!m_MLRoute.Save(ar))
	{
		ADEBUG_TRACE("Save error!");
		return false;
	}

	//	Save event point number
	if (!ar.Write(&m_iNumEventPt, sizeof (m_iNumEventPt)))
	{
		ADEBUG_TRACE("Save error!");
		return false;
	}

	int i;

	//	Save all event point data
	for (i=0; i < m_iNumEventPt; i++)
	{
		if (!m_aEventPts[i]->Save(ar))
		{
			ADEBUG_TRACE("Save error!");
			return false;
		}
	}

	//	Save Route ILDs
	DWORD dwFlags = m_MLRoute.m_Data.dwOwnFlags & m_MLRoute.m_Data.dwNotFixFlags;
	for (i=0; i < NUM_ROUTEILD; i++)
	{
		if ((dwFlags & (1 << i)) && m_aILD[i])
			m_aILD[i]->Save(ar);
	}

	return true;
}

/*	Get ILD index of general AI set

	Return ILD index of general AI, -1 means failure.

	iILDIndex: ILD index of route
*/
int	CMLRoute::GetGeneralAIILD(int iILDIndex)
{
	MLROUTEDATA* pData = GetMLRouteData();
	int iUserID, iILD = -1;

	if (ISEWID(pData->iUserID))
		iUserID = ID_AI_ROCKET;
	else
		iUserID = pData->iUserID;

	int iAIIndex = iUserID - ID_FIRSTAI;

	switch (iILDIndex)
	{
	case RILD_AIMS_PRECISE:
		
		if (pData->iUserID != ID_AI_ROCKET)
			iILD = ILD_PRECISE_MOVE_F + iAIIndex;

		break;
	
	case RILD_AIMS_INTEVAL:		iILD = ILD_FREQ_MOVE_F + iAIIndex;		break;
	case RILD_AISS_PRECISE:		iILD = ILD_PRECISE_STAND_F + iAIIndex;	break;
	case RILD_AISS_INTEVAL:		iILD = ILD_FREQ_STAND_F + iAIIndex;		break;
	case RILD_AICS_PRECISE:		iILD = ILD_PRECISE_CREEP_F + iAIIndex;	break;
	case RILD_AICS_INTEVAL:		iILD = ILD_FREQ_CREEP_F + iAIIndex;		break;
	case RILD_AIGENINTERVAL:	iILD = ILD_AIGEN_INTERVAL;				break;
	case RILD_AIGENNUMBER:		iILD = ILD_AIGEN_NUMBER;				break;
	case RILD_AIGENTURN:		iILD = ILD_AIGEN_TURN;					break;
	case RILD_AIGENTIME:		iILD = ILD_AIGEN_TIME;					break;
	}

	return iILD;
}

/*	Get AI ILD parameter value of level

	Return value of specified ILD parameter.

	iLevel: mission level
	iILD: ILD index
	iDefVal: default value
*/
int	CMLRoute::GetAIParameter(int iLevel, int iILDIndex, int iDefVal)
{
	MLROUTEDATA* pData	= GetMLRouteData();
	DWORD dwOwnFlags	= pData->dwOwnFlags & RTF_MASK_AI;
	DWORD dwNoFixFlags	= pData->dwNotFixFlags & RTF_MASK_AI;
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
bool CMLRoute::OnBeginMission(int iMission)
{
	MLROUTEDATA* pData = GetMLRouteData();

	if (ISAIID(pData->iUserID) || ISEWID(pData->iUserID))
	{
		m_iAIMSPrecise		= GetAIParameter(iMission, RILD_AIMS_PRECISE, pData->iAIMSPrecise);
		m_iAIMSInterval		= GetAIParameter(iMission, RILD_AIMS_INTEVAL, pData->iAIMSInterval);
		m_iAISSPrecise		= GetAIParameter(iMission, RILD_AISS_PRECISE, pData->iAISSPrecise);
		m_iAISSInterval		= GetAIParameter(iMission, RILD_AISS_INTEVAL, pData->iAISSInterval);
		m_iAICSPrecise		= GetAIParameter(iMission, RILD_AICS_PRECISE, pData->iAICSPrecise);
		m_iAICSInterval		= GetAIParameter(iMission, RILD_AICS_INTEVAL, pData->iAICSInterval);
		m_iAIGenInterval	= GetAIParameter(iMission, RILD_AIGENINTERVAL, pData->iAIGenInterval);
		m_iAIGenNumber		= GetAIParameter(iMission, RILD_AIGENNUMBER, pData->iAIGenNum);
		m_iAIGenTurn		= GetAIParameter(iMission, RILD_AIGENTURN, pData->iAIGenTurn);
		m_iAIGenTime		= GetAIParameter(iMission, RILD_AIGENTIME, pData->iAIGenTime);
		m_iAIMaxNumber		= GetAIParameter(iMission, RILD_AIMAXNUMBER, pData->iAIMaxNum);

		//	Convert second to millisecond
		m_iAIGenTime *= 1000;

		//	Calculate AI generate delta
		m_iAIGenDelta = (int)(m_iAIGenInterval * GETGAMELOGICPARAMS->iAIGenInterRange / 100.0f);
		if (m_iAIGenDelta > m_iAIGenInterval)
			m_iAIGenInterval = m_iAIGenDelta;

		assert(m_iAIGenInterval);

		if ((m_iAIGenTime -= m_iAIGenDelta) < 0)
			m_iAIGenTime = 0;

		m_iAIGenDelta *= 2;

		m_iAICopterPrecise = GETAIILDVALUE(ILD_PRECISE_HITCOPTER, iMission);
	}

	for (int i=0; i < m_iNumEventPt; i++)
		m_aEventPts[i]->OnBeginMission(iMission);

	ResetMissionData();

	return true;
}

//	Called when reset mission
bool CMLRoute::OnResetMission()
{
	ResetMissionData();

	for (int i=0; i < m_iNumEventPt; i++)
		m_aEventPts[i]->OnResetMission();

	return true;
}

//	Reset route's mission data
void CMLRoute::ResetMissionData()
{
	m_iNumAI		= 0;
	m_iGenTurnCnt	= 0;
	m_iGenTimeCnt	= m_iAIGenInterval;
	m_iGenTotalNum	= m_iAIGenTurn < 0 ? -1 : m_iAIGenTurn * m_iAIGenNumber;
	m_iTotalCnt		= 0;
}

/*	Logic tick

	Return true for success, otherwise return false
	
	fTimeSpan: time span of this tick, in ms
	dwTimeCnt: time count from beginning of mission (including this tick), in ms
*/
bool CMLRoute::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	int iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);
	MLROUTEDATA* pData = GetMLRouteData();
	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	CMLManObject* pObjMan = GETMLGAMEPLAY->GetObjectMan();
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();

	//	It's time to generate AI ?
	if (ISAIID(pData->iUserID) || ISEWID(pData->iUserID))
	{
		bool bGenAI = true;

		if (m_iGenTotalNum >= 0 && m_iTotalCnt >= m_iGenTotalNum)
			bGenAI = false;

		if (dwTimeCnt < (DWORD)m_iAIGenTime || m_iNumAI >= m_iAIMaxNumber)
			bGenAI = false;

		if (dwTimeCnt >= GETAIILDVALUE(ILD_TIME_AIGEN, iLevel) * 1000)
			bGenAI = false;

		if (bGenAI)
		{
			m_iGenTimeCnt += iTimeSpan;
			DWORD dwWait = 0;

			while (m_iGenTimeCnt > m_iAIGenInterval)
			{
				for (int i=0; i < m_iAIGenNumber; i++, m_iNumAI++, m_iTotalCnt++)
				{
					if (m_iAIMaxNumber >= 0 && m_iNumAI >= m_iAIMaxNumber)
						break;

					if (m_iGenTotalNum >= 0 && m_iTotalCnt >= m_iGenTotalNum)
						break;
				
					if (ISAIID(pData->iUserID))
						pAIMan->CreateAI(this);
					else
						pAIMan->CreateEnemyVehicle(pData->iUserID, this, dwWait += 1000);
				}

				m_iGenTimeCnt -= m_iAIGenInterval;
				m_iGenTurnCnt++;
			}
		}
	}

	return true;
}

int	CMLRoute::GetAIMSInterval(bool bExact)
{	
	if (bExact)
		return m_iAIMSInterval;
	else
	{
		int iTime = (int)(m_iAIMSInterval * GETGAMELOGICPARAMS->iAIShotInterRange / 100.0f);
		iTime = MLA_Random(-iTime, iTime);
		if ((iTime = m_iAIMSInterval + iTime) < 0)
			iTime = 0;

		return iTime;
	}
}

int	CMLRoute::GetAISSInterval(bool bExact)
{
	if (bExact)
		return m_iAISSInterval;
	else
	{
		int iTime = (int)(m_iAISSInterval * GETGAMELOGICPARAMS->iAIShotInterRange / 100.0f);
		iTime = MLA_Random(-iTime, iTime);
		if ((iTime = m_iAISSInterval + iTime) < 0)
			iTime = 0;

		return iTime;
	}
}

int	CMLRoute::GetAICSInterval(bool bExact)
{
	if (bExact)
		return m_iAICSInterval;
	else
	{
		int iTime = (int)(m_iAICSInterval * GETGAMELOGICPARAMS->iAIShotInterRange / 100.0f);
		iTime = MLA_Random(-iTime, iTime);
		if ((iTime = m_iAICSInterval + iTime) < 0)
			iTime = 0;

		return iTime;
	}
}

//	Get AI wait time
int	CMLRoute::GetAIWaitTime()
{
	return MLA_Random(0, m_iAIGenDelta);
}

