/*
 * FILE: ML_ManRoute.cpp
 *
 * DESCRIPTION: Route manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/31
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManRoute.h"
#include "ML_ManagerDef.h"

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
//	Implement CMLManRoute
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManRoute, CMLManBase, 1)

CMLManRoute::CMLManRoute() : CMLManBase()
{
	m_managerId	= MANID_ROUTE;

	m_aRoutes			= NULL;
	m_aAIRoutes			= NULL;
	m_aCopterRoutes		= NULL;
	m_aMiscRoutes		= NULL;
	m_iNumRoute			= 0;
	m_iNumAIRoute		= 0;
	m_iNumCopterRoute	= 0;
	m_iNumMiscRoute		= 0;
	m_dwTimeCnt			= 0;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManRoute::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	return true;
}

//	Release object
bool CMLManRoute::Release()
{
	return CMLManBase::Release();
}

//	Message handler
int	CMLManRoute::OnMsg(AMSG* pMSG)
{
	MLMSG* pMLMsg = (MLMSG*)pMSG;

	if (pMLMsg->idObject == m_managerId)
		return ProcessMsg(pMLMsg) ? 1 : 0;

	return 1;
}

/*	Handle a message

	Return true for success, otherwise return false.

	pMsg: message will be handled
*/
bool CMLManRoute::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

//	Load game mission
bool CMLManRoute::LoadMissionResources(AArchive& ar)
{
	//	Load all routes
	int i, iNumRoute, iUser;

	//	Load number of route
	if (!ar.Read(&iNumRoute, sizeof (iNumRoute)))
		return false;

	if (!(m_aRoutes = (CMLRoute**)malloc(sizeof (CMLRoute*) * iNumRoute)))
	{
		ADEBUG_TRACE("Not enough memory!");
		return false;
	}

	m_iNumRoute = 0;

	for (i=0; i < iNumRoute; i++)
	{
		CMLRoute* pRoute = new CMLRoute;
		if (!pRoute)
		{
			assert(0);
			ADEBUG_TRACE("Not enough memory!");
			return false;
		}
	
		if (!pRoute->Load(ar))
			return false;

		//	ID_ROUTEUSER_MARK route is only used by editor
		iUser = pRoute->GetMLRouteData()->iUserID;
		if (iUser == ID_ROUTEUSER_MARK)
		{
			delete pRoute;
			continue;
		}
		else if (ISAIID(iUser) || ISEWID(iUser)) 
			m_iNumAIRoute++;
		else if (iUser == ID_SUPP_COPTER)
			m_iNumCopterRoute++;
		else if (iUser == ID_ROUTEUSER_MISC)
			m_iNumMiscRoute++;

		m_aRoutes[m_iNumRoute++] = pRoute;
	}

	if (!LabelRoutes())
	{
		ADEBUG_TRACE("Failed to label routes!");
		return false;
	}

	return true;
}

//	Release mission resources
void CMLManRoute::ReleaseMissionResources()
{
	//	Release all routes
	if (!m_aRoutes)
		return;

	for (int i=0; i < m_iNumRoute; i++)
	{
		if (m_aRoutes[i])
			delete m_aRoutes[i];
	}

	free(m_aRoutes);

	if (m_aAIRoutes)
		free(m_aAIRoutes);

	if (m_aCopterRoutes)
		free(m_aCopterRoutes);

	if (m_aMiscRoutes)
		free(m_aMiscRoutes);

	m_aRoutes			= NULL;
	m_aAIRoutes			= NULL;
	m_aCopterRoutes		= NULL;
	m_aMiscRoutes		= NULL;
	m_iNumRoute			= 0;
	m_iNumAIRoute		= 0;
	m_iNumCopterRoute	= 0;
	m_iNumMiscRoute		= 0;
}

//	Label routes
bool CMLManRoute::LabelRoutes()
{
	if (m_iNumAIRoute)
	{
		m_aAIRoutes = (CMLRoute**)malloc(sizeof (CMLRoute*) * m_iNumAIRoute);
		if (!m_aAIRoutes)
		{
			ADEBUG_TRACE("Not enough memory!");
			return false;
		}
	}

	if (m_iNumCopterRoute)
	{
		m_aCopterRoutes = (CMLRoute**)malloc(sizeof (CMLRoute*) * m_iNumCopterRoute);
		if (!m_aCopterRoutes)
		{
			ADEBUG_TRACE("Not enough memory!");
			return false;
		}
	}

	if (m_iNumMiscRoute)
	{
		m_aMiscRoutes = (CMLRoute**)malloc(sizeof (CMLRoute*) * m_iNumMiscRoute);
		if (!m_aMiscRoutes)
		{
			ADEBUG_TRACE("Not enough memory!");
			return false;
		}
	}

	int i, iAICnt=0, iCopterCnt=0, iMiscCnt=0;

	for (i=0; i < m_iNumRoute; i++)
	{
		int iUser = m_aRoutes[i]->GetMLRouteData()->iUserID;

		if (ISAIID(iUser) || ISEWID(iUser)) 
			m_aAIRoutes[iAICnt++] = m_aRoutes[i];
		else if (iUser == ID_SUPP_COPTER)
			m_aCopterRoutes[iCopterCnt++] = m_aRoutes[i];
		else if (iUser == ID_ROUTEUSER_MISC)
			m_aMiscRoutes[iMiscCnt++] = m_aRoutes[i];
	}

	return true;
}

//	Called when begin game playing
bool CMLManRoute::OnStartGamePlay()
{
	return true;
}

//	Called when end game playing
bool CMLManRoute::OnEndGamePlay()
{
	ReleaseMissionResources();
	return true;
}

//	Called when being mission
bool CMLManRoute::OnBeginMission(int iMission)
{
	m_dwTimeCnt = 0;

	for (int i=0; i < m_iNumRoute; i++)
		m_aRoutes[i]->OnBeginMission(iMission);

	CalcAITotalNum();

	return true;
}

//	Called when reset mission
bool CMLManRoute::OnResetMission()
{
	m_dwTimeCnt = 0;

	for (int i=0; i < m_iNumRoute; i++)
		m_aRoutes[i]->OnResetMission();
	
	return true;
}

//	Called when end mission
bool CMLManRoute::OnEndMission()
{
	return true;
}

//	Logic tick
bool CMLManRoute::LogicRun(float fTimeSpan)
{
	BeginLogicCount();
	
	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	for (int i=0; i < m_iNumRoute; i++)
		m_aRoutes[i]->LogicRun(fTimeSpan, m_dwTimeCnt);

	EndLogicCount();

	return true;
}

//	Get called helicopter route
CMLRoute* CMLManRoute::GetCalledCopterRoute()
{
	if (!m_iNumCopterRoute || !m_aCopterRoutes)
		return NULL;

	for (int i=0; i < m_iNumCopterRoute; i++)
	{
		DWORD dwOwnFlags = m_aCopterRoutes[0]->GetMLRouteData()->dwOwnFlags;
		if (!(dwOwnFlags & RTF_OWN_COPTERINTERVAL))
			return m_aCopterRoutes[i];
	}

	return NULL;
}

//	Get number of all kinds of AI will be generated. -1 means infinite
void CMLManRoute::CalcAITotalNum()
{
	memset(&m_AINum, 0, sizeof (m_AINum));

	for (int i=0; i < m_iNumAIRoute; i++)
	{
		int iUser = m_aAIRoutes[i]->GetMLRouteData()->iUserID;
		int iNum = m_aAIRoutes[i]->GetGenTotalNum();

		if (iNum < 0)
		{
			switch (iUser)
			{
			case ID_AI_FOOTMAN:		m_AINum.iNumFootman = -1;		break;
			case ID_AI_GUERILLA:	m_AINum.iNumGuerilla = -1;		break;
			case ID_AI_ROCKET:		m_AINum.iNumRocket = -1;		break;
			case ID_EW_TRUCK:		m_AINum.iNumTruck = -1;			break;
			case ID_EW_TANK:		m_AINum.iNumTank = -1;			break;
			case ID_EW_ARMORED:		m_AINum.iNumArmored = -1;		break;
			}
		}
		else
		{
			switch (iUser)
			{
			case ID_AI_FOOTMAN:		m_AINum.iNumFootman += iNum;	break;
			case ID_AI_GUERILLA:	m_AINum.iNumGuerilla += iNum;	break;
			case ID_AI_ROCKET:		m_AINum.iNumRocket += iNum;		break;
			case ID_EW_TRUCK:		m_AINum.iNumTruck += iNum;		break;
			case ID_EW_TANK:		m_AINum.iNumTank += iNum;		break;
			case ID_EW_ARMORED:		m_AINum.iNumArmored += iNum;	break;
			}
		}
	}
}

//	All AI have been generated ?
bool CMLManRoute::AIGenerationOver()
{
	for (int i=0; i < m_iNumAIRoute; i++)
	{
		if (m_aAIRoutes[i]->GetGenTotalNum() < 0)
			return false;

		if (m_aAIRoutes[i]->GetMaxNumAtOneTime() > 0 &&
			m_aAIRoutes[i]->GetGeneratedNum() < m_aAIRoutes[i]->GetGenTotalNum())
			return false;
	}

	return true;
}

