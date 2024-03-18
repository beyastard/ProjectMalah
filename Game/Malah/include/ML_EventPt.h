/*
 * FILE: ML_EventPt.h
 *
 * DESCRIPTION: Event point class in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _ML_EVENTPT_H_
#define _ML_EVENTPT_H_

#include "ML_LevelData.h"
#include "ML_EventPtData.h"

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

//	Event interator
struct EVENTINTERATOR
{
	int				iLoopCnt;	//	Loop counter
	int				iEventCnt;	//	Event counter
	MLEVENTPTEVENT	Event;		//	Event content
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEventPt
//
///////////////////////////////////////////////////////////////////////////

class CMLEventPt : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLEventPt)

	enum
	{
		EILD_AKCOPTERPOSS	= 0,	//	Possible of rocket man to attack copter
		NUM_EPILD			= 1		//	Number of route ILD
	};

public:		//	Constructor and Destructor

	CMLEventPt();
	virtual ~CMLEventPt();

public:		//	Attributes

public:		//	Operations

	static void	InitEventInterator(EVENTINTERATOR* pInterator)
	{
		pInterator->iLoopCnt		= 0;
		pInterator->iEventCnt		= -1;
		pInterator->Event.iEvent	= EPE_NONE;
		pInterator->Event.iTime		= 0;
	}

	virtual bool	Load(AArchive& ar);		//	Load event point
	virtual bool	Save(AArchive& ar);		//	Save event point

	bool		OnBeginMission(int iMission);	//	Called when begin a mission
	bool		OnResetMission();				//	Called when reset mission

	bool		GetNextEvent(EVENTINTERATOR* pInterator);	//	Get next event
	A3DVECTOR3	GetRandomPos();								//	Get random position in the event point
	bool		PosInEventPt(A3DVECTOR3& vPos);				//	Check whether specified position in event point

	int			GetAIAttackCopterPoss()		{	return m_iAIAttackCopter;		}

	MLEVENTPTDATA*		GetMLEventPtData()	{	return &m_MLEventPt.m_Data;		}
	CMLLevelDataInt*	GetILD(int iIndex)	{	return m_aILD[iIndex];			}

protected:	//	Attributes

	CMLEventPtData	m_MLEventPt;		//	Malah event point data

	CMLLevelDataInt*	m_aILD[NUM_EPILD];	//	Event point ILDs

	int			m_iAIAttackCopter;		//	AI attack copter posibility

protected:	//	Operations

	bool		ReleaseEventPt();	//	Release resources
	int			GetAIParameter(int iLevel, int iILDIndex, int iDefVal);	//	Get AI parameter value of level
	int			GetGeneralAIILD(int iILDIndex);		//	Get ILD index of general AI set
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_EVENTPT_H_

