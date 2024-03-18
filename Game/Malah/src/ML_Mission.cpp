/*
 * FILE: ML_Mission.cpp
 *
 * DESCRIPTION: Mission class for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/27
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Database.h"
#include "ML_MissionData.h"
#include "ML_GamePlay.h"
#include "ML_ManAI.h"
#include "ML_Player.h"
#include "ML_ManRoute.h"

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
//	Implement CMLMission
//
///////////////////////////////////////////////////////////////////////////

CMLMission::CMLMission()
{
	m_pMission		= NULL;
	m_pData			= NULL;
	m_iTimeLimit	= 0;
	m_iTimeCnt		= 0;
}

//	Begin mission ?
void CMLMission::BeginMission(CMLMissionData* pMission)
{
	m_pMission = pMission;
	if (m_pMission)
	{
		m_pData	= m_pMission->GetData();
		m_iTimeLimit = m_pData->iTime * 1000;
	}
	else
		m_iTimeLimit = 0;

	memset(&m_Statistic, 0, sizeof (m_Statistic));
}

//	Reset mission ?
void CMLMission::ResetMission()
{
	memset(&m_Statistic, 0, sizeof (m_Statistic));
}

//	Increase statistic
void CMLMission::IncStatistic(int iID, int iNum, DWORD dwFlag)
{
	if (dwFlag == 2)
	{
		m_Statistic.iNumInPreserve += iNum;
		return;
	}

	if (dwFlag == 1)
	{
		switch (iID)
		{
		case ID_EW_TRUCK:	m_Statistic.iNumEscTruck += iNum;		break;
		case ID_EW_TANK:	m_Statistic.iNumEscTank += iNum;		break;
		case ID_EW_ARMORED:	m_Statistic.iNumEscArmored += iNum;		break;
		}

		return;
	}

	assert(dwFlag == 0);

	switch (iID)
	{
	case ID_AI_FOOTMAN:		m_Statistic.iNumDeadFootman += iNum;	GETMLGAMEPLAY->AddKilledPeopleNum(1);	break;
	case ID_AI_GUERILLA:	m_Statistic.iNumDeadGuerilla += iNum;	GETMLGAMEPLAY->AddKilledPeopleNum(1);	break;
	case ID_AI_ROCKET:		m_Statistic.iNumDeadRocket += iNum;		GETMLGAMEPLAY->AddKilledPeopleNum(1);	break;
	case ID_EW_MIG21:		m_Statistic.iNumDeadPlane += iNum;		GETMLGAMEPLAY->AddKilledPlaneNum(1);	break;
	case ID_EW_FANTAN:		m_Statistic.iNumDeadPlane += iNum;		GETMLGAMEPLAY->AddKilledPlaneNum(1);	break;
	case ID_EW_TANK:		m_Statistic.iNumDeadTank += iNum;		GETMLGAMEPLAY->AddKilledVehicleNum(1);	break;
	case ID_EW_ARMORED:		m_Statistic.iNumDeadArmored += iNum;	GETMLGAMEPLAY->AddKilledVehicleNum(1);	break;
	case ID_EW_TRUCK:		m_Statistic.iNumDeadTruck += iNum;		GETMLGAMEPLAY->AddKilledVehicleNum(1);	break;
	case ID_SUPP_COPTER:	m_Statistic.iNumCopter += iNum;			break;
	}
}

/*	Finished mission

	Return value:

		-1: mission failed
		0: mission isn't ending
		1: mission completed successfully.

	iTimeCnt: time (ms) from mission begin
*/
int CMLMission::MissionCompleted(int iTimeCnt)
{
	assert(m_pMission && m_pData);

	m_iTimeCnt = iTimeCnt;

	int iRet = 0;

	switch (m_pData->iType)
	{
	case MISSION_KILLALL:	iRet = MC_KillAll(iTimeCnt);		break;
	case MISSION_KILL:		iRet = MC_Kill(iTimeCnt);			break;
	case MISSION_HOLDPOS:	iRet = MC_HoldPos(iTimeCnt);		break;
	case MISSION_PRESERVE:	iRet = MC_PreserveArea(iTimeCnt);	break;
	case MISSION_STOPTRUCK:	iRet = MC_StopTruck(iTimeCnt);		break;
	default:
		return 0;
	}

	if (!iRet)
		iRet = MC_Common(iTimeCnt);

	return iRet;
}

int	CMLMission::MC_Common(int iTimeCnt)
{
	//	Player has been dead ?
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	if (pPlayer->GetLife() <= 0)
		return -1;

	//	Too many helicopters have been hit ?
	if (ReachDest(m_pData->iNumCopter, m_Statistic.iNumCopter))
		return -1;

	//	All enemy have been killed ?
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	int iTime = GETAIILDVALUE(ILD_TIME_AIGEN, iLevel);

	if ((iTime && iTimeCnt >= iTime * 1000) || 
		GETMLGAMEPLAY->GetRouteMan()->AIGenerationOver())
	{
		CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
		if (!pAIMan->GetCurAINum() && !pAIMan->GetCurEWNum())
		{
			if (m_pData->iType != MISSION_KILLALL)
				ADEBUG_TRACE1("%d level end with killing all enemies.", iLevel+1);

			return 1;
		}
	}

	return 0;
}

//	Judge MISSION_KILLALL mission type
int CMLMission::MC_KillAll(int iTimeCnt)
{
	if (ReachDest(m_iTimeLimit, iTimeCnt))
		return -1;

	return 0;
}

//	Judge MISSION_KILL mission type
int CMLMission::MC_Kill(int iTimeCnt)
{
	if (ReachDest(m_iTimeLimit, iTimeCnt))
		return -1;

	if (m_Statistic.iNumDeadFootman >= m_pData->iNumFootMan &&
		m_Statistic.iNumDeadGuerilla >= m_pData->iNumGuerilla &&
		m_Statistic.iNumDeadRocket >= m_pData->iNumRocket &&
		m_Statistic.iNumDeadTruck >= m_pData->iNumTruck &&
		m_Statistic.iNumDeadTank >= m_pData->iNumTank &&
		m_Statistic.iNumDeadArmored >= m_pData->iNumArmored &&
		m_Statistic.iNumDeadPlane >= m_pData->iNumPlane)
		return 1;

	//	Too many trucks have escaped ?
	const CMLManRoute::AINUM* pAINum = GETMLGAMEPLAY->GetRouteMan()->GetAITotalGenNum();
	if (m_pData->iNumTruck > 0 && pAINum->iNumTruck >= 0 && 
		pAINum->iNumTruck - m_Statistic.iNumEscTruck < m_pData->iNumTruck)
		return -1;
	
	//	Too many tanks have escaped ?
	if (m_pData->iNumTank > 0 && pAINum->iNumTank >= 0 &&
		pAINum->iNumTank - m_Statistic.iNumEscTank < m_pData->iNumTank)
		return -1;

	//	Too many armored cars have escaped ?
	if (m_pData->iNumArmored > 0 && pAINum->iNumArmored >= 0 &&
		pAINum->iNumArmored - m_Statistic.iNumEscArmored < m_pData->iNumArmored)
		return -1;

	return 0;
}

//	Judge MISSION_HOLDPOS mission type
int CMLMission::MC_HoldPos(int iTimeCnt)
{
	if (ReachDest(m_iTimeLimit, iTimeCnt))
		return 1;

	return 0;
}

//	Judge MISSION_PRESERVE mission type
int CMLMission::MC_PreserveArea(int iTimeCnt)
{
	if (ReachDest(m_iTimeLimit, iTimeCnt))
		return 1;

	if (ReachDest(m_pData->iNumFootMan, m_Statistic.iNumInPreserve))
		return -1;

	return 0;
}

//	Judge MISSION_STOPTRUCK mission type
int CMLMission::MC_StopTruck(int iTimeCnt)
{
	if (ReachDest(m_iTimeLimit, iTimeCnt))
		return 1;

	if (m_Statistic.iNumEscTruck >= m_pData->iNumTruck)
		return -1;

	int iTotalNum = GETMLGAMEPLAY->GetRouteMan()->GetAITotalGenNum()->iNumTruck;
	if (iTotalNum >= 0)
	{
		if (iTotalNum - m_Statistic.iNumDeadTruck < m_pData->iNumTruck)
			return 1;
	}

	return 0;
}


