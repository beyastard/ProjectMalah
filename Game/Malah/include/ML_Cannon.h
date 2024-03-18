/*
 * FILE: ML_Cannon.h
 *
 * DESCRIPTION: Cannon class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_CANNON_H_
#define _ML_CANNON_H_

#include "ML_ModelUnit.h"

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
//	Class CMLCannon
//
///////////////////////////////////////////////////////////////////////////

class CMLCannon : public CMLModelUnit
{
public:		//	Types

	//	States
	enum
	{
		ST_NONE	= 0,
		ST_BEFOREFIRE,	//	Wait before fire
		ST_FIRE,		//	Be firing
		ST_AFTERFIRE,	//	Wait after fire
	};

public:		//	Constructor and Destructor

	CMLCannon();
	virtual ~CMLCannon() {}

public:		//	Attributes

	static int	m_iViewWid;			//	Viewport size
	static int	m_iViewHei;

public:		//	Operations

	friend void		_CannonLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg);

	bool			Init(A3DVECTOR3& vBombPos);		//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual bool	Render(A3DViewport* pViewport);					//	Render routine

protected:	//	Attributes

	A3DCamera*		m_pCamera;		//	Cannon camera object
	A3DViewport*	m_pViewport;	//	Cannon viewport object

	A3DVECTOR3	m_vBombPos;			//	Bomb position
	int			m_iState;			//	State
	int			m_iFireTimes;		//	Times of cannon fire
	int			m_iFireCnt;			//	Fire counter
	int			m_iWaitTime;		//	Wait time
	int			m_iWaitCnt;			//	Wait time counter

protected:	//	Operations

	void		SetState(int iState);	//	Set state
	void		EndOneFire();			//	End one fire
	void		CreateBombs(int iNum);	//	Create bombs
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_CANNON_H_

