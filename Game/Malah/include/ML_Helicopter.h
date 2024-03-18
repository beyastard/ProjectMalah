/*
 * FILE: ML_Helicopter.h
 *
 * DESCRIPTION: Helicopter class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_HELICOPTER_H_
#define _ML_HELICOPTER_H_

#include "ML_ModelUnit.h"
#include "ML_EventPt.h"

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

class CMLRoute;
class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLHelicopter
//
///////////////////////////////////////////////////////////////////////////

class CMLHelicopter : public CMLModelUnit
{
public:		//	Types

	//	States
	enum
	{
		ST_NONE	= 0,
		ST_WAIT,			//	Wait
		ST_NORMALFLY,		//	Normal fly
		ST_ENTER,			//	enter supply point
		ST_SUPPLY,			//	supporting
		ST_LEAVE,			//	Leave supply point
		ST_CIRCLEFLY,		//	Circle flying
		ST_APPEAR,			//	Appear
		ST_DISAPPEAR,		//	Disappear
		ST_CRASHING,		//	Crashing
		ST_WRECKAGE,		//	wreckage
	};

	//	Sub-states
	enum
	{
		SUBST_ADJUST = 0,	//	Adjust position
		SUBST_DOWN,			//	Down to supporting height
		SUBST_SUPPLY,		//	supporting
		SUBST_UP,			//	Up to flying height
	};

	//	Actions
	enum
	{
		ACT_FLY = 0,		//	Normal fly
		ACT_FLYTOHANG,		//	Fly to hang
		ACT_HANG,			//	Hang
		ACT_HANGTOFLY,		//	Hang to fly
		ACT_CRASH,			//	Crash
		ACT_WRECK,			//	Wreck
		ACT_NUM,			//	Number of action
	};

public:		//	Constructor and Destructor

	CMLHelicopter();
	virtual ~CMLHelicopter() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(CMLRoute* pRoute);		//	Initialize object
	virtual bool	Release();					//	Release resources

	virtual void	SetPos(A3DVECTOR3& vPos);	//	Set position
	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);	//	Logic tick

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	bool		CanBeHit();			//	Can be used as fire target ?
	bool		CalcHitPos(bool bHit, A3DVECTOR3& vObj, float fSpeed, A3DVECTOR3* pHitPos);
	bool		RayTrace(MLRAYTRACE* pTrace);				//	Rsay trace routine
	void		Strike(CMLBullet* pBullet, bool bBody);		//	Strike helicopter

protected:	//	Attributes

	const static char*	m_aActions[ACT_NUM];	//	Action names

	CMLRoute*	m_pRoute;			//	Helicopter route
	int			m_iState;			//	Current state
	int			m_iSubState;		//	Sub-state
	int			m_iLife;			//	Life value

	float		m_fTimeSpan;		//	Time spane of this logic tick
	int			m_iTimeSpan;		//	Time spane of this logic tick
	DWORD		m_dwTimeCnt;		//	time counter of this logic tick

	A3DVECTOR3	m_vCurTarget;		//	Current destination poistion
	A3DVECTOR3	m_vCurMoveDir;		//	Current move direction
	float		m_fDistance;		//	Current distance
	float		m_fDistCnt;			//	Distance counter
	A3DVECTOR3	m_vxzPos;			//	Position on xz plane
	float		m_fHeightDelta;		//	Height change delta

	float		m_fYawDelta;		//	Yaw delta
	float		m_fRollRadian;		//	Roll radian
	float		m_fRollCnt;			//	Roll counter
	float		m_fRollDelta;		//	Roll delta
	float		m_fAccelerator;		//	Accelerator of speed
	float		m_fMinHSpeed;		//	Minimum horizontal speed when crashing

	EVENTINTERATOR	m_EtInterator;	//	Event interator
	int				m_iCurEventPt;	//	Current event point

	AM3DSoundBuffer*	m_pFlySnd;	//	Fly sound

protected:	//	Operations

	void		BeginEventPoint();		//	Begin event point
	void		BeginNormalFly(A3DVECTOR3& p0, A3DVECTOR3& p1, bool bAppear);	//	Begin normal fly
	void		BeginCircleFly(A3DVECTOR3& p0, A3DVECTOR3& p1, float v1);		//	Begin circle flying
	void		BeginSupply();			//	Begin supply
	void		LeaveSupply();			//	Leave supply

	void		LogicRun_NormalFly();	//	Logic run in ST_NORMALFLY
	void		LogicRun_Supply();		//	Logic run in ST_SUPPLY
	void		LogicRun_Crash();		//	Logic run in ST_CRASHING
	void		LogicRun_Wreck();		//	Logic run in ST_WRECKAGE
	void		LogicRun_CircleFly();	//	Logic run when circle flying
	void		LogicRun_Disappear();	//	Logic run when disappear
	void		LogicRun_Appear();		//	Logic run when appear

	void		ChangeModel();			//	Change model
	void		DoSupply();				//	Do supply
	bool		BeFlying();				//	Be flying ?
	void		AdjustModelYawAndRoll(float fTickTime);	//	Adjust model yaw and roll
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_HELICOPTER_H_

