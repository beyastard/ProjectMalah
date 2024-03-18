/*
 * FILE: ML_Mission.h
 *
 * DESCRIPTION: Mission class for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/27
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_MISSION_H_
#define _ML_MISSION_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLMission
//
///////////////////////////////////////////////////////////////////////////

class CMLMission
{
public:		//	Types

	//	Statisic
	struct STATISTIC
	{
		int		iNumDeadFootman;		//	Dead footman
		int		iNumDeadGuerilla;		//	Dead guerilla
		int		iNumDeadRocket;			//	Dead rocket man
		int		iNumDeadTruck;			//	Destroyed truck
		int		iNumDeadTank;			//	Destroyed tank
		int		iNumDeadArmored;		//	Destroyed armored car
		int		iNumDeadPlane;			//	Destroyed plane
		int		iNumEscTruck;			//	Escaped truck
		int		iNumEscTank;			//	Escaped tank
		int		iNumEscArmored;			//	Escaped armored car
		int		iNumInPreserve;			//	Number of enemy unit in preserved area
		int		iNumCopter;				//	Destroyed helicopter
	};

public:		//	Constructor and Destructor

	CMLMission();
	virtual ~CMLMission() {}

public:		//	Attributes

	void	BeginMission(CMLMissionData* pData);
	void	ResetMission();

	int		MissionCompleted(int iTimeCnt);		//	Mission completed ?
	void	IncStatistic(int iID, int iNum, DWORD dwFlag);	//	Increase statistic
	
	inline bool	InPreserveArea(A3DVECTOR3& vPos);	//	Given position is in preserve area ?

	int		GetTimeLimit()	{	return m_iTimeLimit;	}
	int		GetTimeCount()	{	return m_iTimeCnt;		}

	STATISTIC*		GetStatistics()		{	return &m_Statistic;	}
	MLMISSIONDATA*	GetMissionData()	{	return m_pData;			}

public:		//	Operations

protected:	//	Attributes

	CMLMissionData*		m_pMission;		//	Mission
	MLMISSIONDATA*		m_pData;		//	Mission data
	STATISTIC			m_Statistic;	//	Statistic

	int		m_iTimeLimit;	//	Time limit
	int		m_iTimeCnt;		//	Time counter

protected:	//	Operations

	int		MC_Common(int iTimeCnt);
	int		MC_KillAll(int iTimeCnt);		//	Judge MISSION_KILLALL mission type
	int		MC_Kill(int iTimeCnt);			//	Judge MISSION_KILL mission type
	int		MC_HoldPos(int iTimeCnt);		//	Judge MISSION_HOLDPOS mission type
	int		MC_PreserveArea(int iTimeCnt);	//	Judge MISSION_PRESERVE mission type
	int		MC_StopTruck(int iTimeCnt);		//	Judge MISSION_STOPTRUCK mission type

	inline bool ReachDest(int iDest, int iCur);
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

bool CMLMission::ReachDest(int iDest, int iCur)
{
	if (iDest && iCur >= iDest)
		return true;

	return false;
}

//	Given position is in preserve area ?
bool CMLMission::InPreserveArea(A3DVECTOR3& vPos)
{
	if (m_pData->iType == MISSION_PRESERVE &&
		MagnitudeH(m_pData->vPreAreaPos - vPos) < m_pData->fPreAreaRad)
		return true;

	return false;
}

#endif	//	_ML_MISSION_H_

