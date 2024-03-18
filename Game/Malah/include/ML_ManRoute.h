/*
 * FILE: ML_ManRoute.h
 *
 * DESCRIPTION: Route manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/31
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANROUTE_H_
#define _ML_MANROUTE_H_

#include "ML_Route.h"
#include "ML_MsgDef.h"
#include "ML_ManBase.h"

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
//	Class CMLManRoute
//
///////////////////////////////////////////////////////////////////////////

class CMLManRoute : public CMLManBase
{
public:		//	Types

	struct AINUM
	{
		int		iNumFootman;	//	Number of footman
		int		iNumGuerilla;	//	Number of guerilla
		int		iNumRocket;		//	Number of rocket man
		int		iNumTruck;		//	Number of truck
		int		iNumTank;		//	Number of tank
		int		iNumArmored;	//	Number of armored car
	};

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManRoute)

	CMLManRoute();
	virtual ~CMLManRoute() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);				//	Initialize object
	virtual bool	Release();						//	Release object

	virtual bool	LogicRun(float fTimeSpan);		//	Logic tick
	virtual int		OnMsg(AMSG* pMSG);				//	Message handler

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	virtual bool	LoadMissionResources(AArchive& ar);		//	Load game mission
	virtual void	ReleaseMissionResources();				//	Release mission resources

	CMLRoute*	GetCalledCopterRoute();		//	Get called helicopter route
	bool		AIGenerationOver();			//	All AI have been generated ?

	const AINUM*	GetAITotalGenNum()	{	return &m_AINum;	}

protected:	//	Attributes

	CMLRoute**	m_aRoutes;			//	All routes
	CMLRoute**	m_aAIRoutes;		//	AI routes
	CMLRoute**	m_aCopterRoutes;	//	Helicopter routes
	CMLRoute**	m_aMiscRoutes;		//	Misc routes
	int			m_iNumRoute;		//	Total number of route
	int			m_iNumAIRoute;		//	Number of AI route
	int			m_iNumCopterRoute;	//	Number of copter route
	int			m_iNumMiscRoute;	//	Number of misc route

	AINUM		m_AINum;			//	Total number of kinds of AI
	DWORD		m_dwTimeCnt;		//	Time counter

protected:	//	Operations

	bool		LabelRoutes();		//	Label routes
	void		CalcAITotalNum();	//	Calculate total number of AI will be generated

	virtual bool	ProcessMsg(MLMSG* pMsg);	//	Handle a message
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANROUTE_H_

