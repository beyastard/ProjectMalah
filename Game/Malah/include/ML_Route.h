/*
 * FILE: ML_Route.h
 *
 * DESCRIPTION: Route class in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ROUTE_H_
#define _ML_ROUTE_H_

#include "ML_EventPt.h"
#include "ML_RouteData.h"

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
//	Class CMLRoute
//
///////////////////////////////////////////////////////////////////////////

class CMLRoute : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLRoute)

	enum
	{
		RILD_AIMS_PRECISE	= 0,	//	AI move shot precise
		RILD_AIMS_INTEVAL	= 1,	//	AI move shot inteval time
		RILD_AISS_PRECISE	= 2,	//	AI stand shot precise
		RILD_AISS_INTEVAL	= 3,	//	AI stand shot inteval time
		RILD_AICS_PRECISE	= 4,	//	AI creep shot precise
		RILD_AICS_INTEVAL	= 5,	//	AI creep shot inteval time
		RILD_AIGENINTERVAL	= 6,	//	AI generate interval
		RILD_AIGENNUMBER	= 7,	//	AI generate number
		RILD_AIGENTURN		= 8,	//	AI generate turn
		RILD_AIGENTIME		= 9,	//	AI generate time
		RILD_AIMAXNUMBER	= 10,	//	Maximum number of AI that can appear on route at same time
		NUM_ROUTEILD		= 11	//	Number of route ILD
	};

public:		//	Constructor and Destructor

	CMLRoute();
	virtual ~CMLRoute();

public:		//	Attributes

public:		//	Operations

	virtual bool	Load(AArchive& ar);		//	Load route
	virtual bool	Save(AArchive& ar);		//	Save route

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

	bool		OnBeginMission(int iMission);	//	Called when begin a mission
	bool		OnResetMission();				//	Called when reset mission

	int			GetAIMSInterval(bool bExact);	//	Get AI move shot inteval time
	int			GetAISSInterval(bool bExact);	//	Get AI stand shot inteval time
	int			GetAICSInterval(bool bExact);	//	Get AI creep shot inteval time
	int			GetAIWaitTime();				//	Get AI wait time

	int			GetAIMSPrecise()	{	return m_iAIMSPrecise;	}	//	Get AI move shot precise
	int			GetAISSPrecise()	{	return m_iAISSPrecise;	}	//	Get AI stand shot precise
	int			GetAICSPrecise()	{	return m_iAICSPrecise;	}	//	Get AI creep shot precise

	int			GetHitCopterPrecise()	{	return m_iAICopterPrecise;	}
	int			GetGenTotalNum()		{	return m_iGenTotalNum;		}
	int			GetGeneratedNum()		{	return m_iTotalCnt;			}
	int			GetMaxNumAtOneTime()	{	return m_iAIMaxNumber;		}

	inline void		AIDied();		//	One AI on this route died

	MLROUTEDATA*	GetMLRouteData()	{	return &m_MLRoute.m_Data;			}
	int				GetEventPtNum()		{	return m_iNumEventPt;				}
	int				GetUserID()			{	return m_MLRoute.m_Data.iUserID;	}

	CMLEventPt*		GetEventPt(int iEventPt)	{	return m_aEventPts[iEventPt];	}

protected:	//	Attributes

	CMLEventPt**	m_aEventPts;		//	Event point array
	int				m_iNumEventPt;		//	Number of event point
	CMLRouteData	m_MLRoute;			//	Malah route data

	int		m_iAIMSPrecise;		//	AI move shot precise
	int		m_iAIMSInterval;	//	AI move shot inteval time
	int		m_iAISSPrecise;		//	AI stand shot precise
	int		m_iAISSInterval;	//	AI stand shot inteval time
	int		m_iAICSPrecise;		//	AI creep shot precise
	int		m_iAICSInterval;	//	AI creep shot inteval time
	int		m_iAIGenInterval;	//	AI generate interval
	int		m_iAIGenNumber;		//	AI generate number
	int		m_iAIGenTurn;		//	AI generate turn
	int		m_iAIGenTime;		//	AI generate time
	int		m_iAIMaxNumber;		//	Maximum number of AI that can appear on route at same time
	int		m_iAIGenDelta;		//	AI Generate interval delta
	int		m_iAICopterPrecise;	//	AI hit helicopter precise

	int		m_iNumAI;			//	Current number of AI on route
	int		m_iGenTurnCnt;		//	Generate turn counter
	int		m_iGenTimeCnt;		//	Generate time counter
	int		m_iGenTotalNum;		//	Total number of AI, -1 means infinite
	int		m_iTotalCnt;		//	Total number counter

	CMLLevelDataInt*	m_aILD[NUM_ROUTEILD];	//	Route ILDs

protected:	//	Operations

	bool		ReleaseRoute();			//	Release resources
	void		ResetMissionData();		//	Reset route's mission data
	int			GetAIParameter(int iLevel, int iILDIndex, int iDefVal);	//	Get AI parameter value of level
	int			GetGeneralAIILD(int iILDIndex);		//	Get ILD index of general AI set
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	One AI on this route died
void CMLRoute::AIDied()
{
	assert(m_iNumAI > 0);
	m_iNumAI--;
}


#endif	//	_ML_ROUTE_H_



