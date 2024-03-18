/*
 * FILE: ML_Unit.h
 *
 * DESCRIPTION: Unit base class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_UNIT_H_
#define _ML_UNIT_H_

#include "ML_MsgDef.h"

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
//	Class CMLUnit
//
///////////////////////////////////////////////////////////////////////////

class CMLUnit : public AIDObject
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLUnit();
	virtual ~CMLUnit() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Release()			{	return true;	}		//	Release resources
	virtual int		OnMsg(MLMSG* pMsg)	{	return 1;		}		//	Message handler

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt)	{ return true; }	//	Logic tick
	virtual bool	Render(A3DViewport* pViewport) { return true; }					//	Render routine
	virtual bool	TickAnimation() { return true; }								//	Tick animation

	virtual void	PauseSound(bool bPause) {}	//	Pause sound
	virtual void	StopSound() {}				//	Stop sound

	A3DVECTOR3		GetPos()			{	return m_vPos;		}
	A3DVECTOR3		GetVel()			{	return m_vVel;		}
	float			GetSpeed()			{	return m_fSpeed;	}
	bool			IsDead()			{	return m_bDead;		}
	bool			CanTrace()			{	return m_bDoTrace;	}
	bool			CanRender()			{	return m_bRender;	}

	void	EnableTrace(bool bEnable)	{	m_bDoTrace = bEnable;	}
	void	EnableRender(bool bEnable)	{	m_bRender = bEnable;	}

	virtual void SetPos(A3DVECTOR3& vPos)
	{
		m_vPos = vPos;
	}

	//	Set velocity
	virtual void SetVel(A3DVECTOR3& vVel)
	{
		m_vVel	 = vVel;
		m_fSpeed = Magnitude(vVel);
	}

	//	Set speeed
	virtual void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
		m_vVel	 = Normalize(m_vVel) * fSpeed;
	}

protected:	//	Attributes

	A3DVECTOR3		m_vPos;			//	Position
	A3DVECTOR3		m_vVel;			//	Velocity
	float			m_fSpeed;		//	Speed (module of m_vVel)
	
	bool			m_bDead;		//	Dead flag
	bool			m_bDoTrace;		//	Can do collision trace
	bool			m_bRender;		//	Render flag

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_UNIT_H_

