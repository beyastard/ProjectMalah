/*
 * FILE: ML_EnemyVehicle.h
 *
 * DESCRIPTION: Enemy vehicle class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_ENEMYVEHICLE_H_
#define _ML_ENEMYVEHICLE_H_

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

class CMLBullet;
class CMLRoute;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEnemyVehicle
//
///////////////////////////////////////////////////////////////////////////

class CMLEnemyVehicle : public CMLModelUnit
{
public:		//	Types

	//	State
	enum
	{
		ST_NONE = 0,
		ST_WAIT,		//	Wait
		ST_RUNNING,		//	Running
		ST_FIRE,		//	Fire
		ST_LEAVE,		//	Leave battle
		ST_STOP,		//	Stop
		ST_WRECK,		//	Wreckage
	};

public:		//	Constructor and Destructor

	CMLEnemyVehicle();
	virtual ~CMLEnemyVehicle() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(int iID, CMLRoute* pRoute, int iWaitTime);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual void	SetPos(A3DVECTOR3& vPos);	//	Set position
	void			SetNoHeightPos(A3DVECTOR3& vPos);	//	Set position without height info.

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	bool	RayTrace(MLRAYTRACE* pTrace);				//	Ray trace function
	void	Strike(CMLBullet* pBullet);					//	Strike
	bool	Explode(CMLBullet* pBullet, bool bFire);	//	Explode

protected:	//	Attributes

	CMLRoute*	m_pRoute;		//	Route pointer

	int			m_iState;		//	State
	int			m_iSubState;	//	Sub state
	int			m_iLife;		//	vehicle's life
	A3DVECTOR3	m_vxzPos;		//	vehicle's position on xz plane
	A3DVECTOR3	m_vCurTarget;	//	Current target position
	A3DVECTOR3	m_vMoveDir;		//	Move direction
	float		m_fMoveSpeed;	//	Move speed
	int			m_iCurEventPt;	//	Current evnet point

	A3DVECTOR3	m_vUp1;			//	Up 1
	A3DVECTOR3	m_vUp2;			//	Up 2
	float		m_fUpFactor;	//	Up factor

	float		m_fTimeSpan;	//	Time spane of this logic tick
	int			m_iTimeSpan;	//	Time spane of this logic tick
	int			m_iDestTime;	//	Destination time
	DWORD		m_dwTimeCnt;	//	Level time counter
	int			m_iTimeCnt;		//	Time counter
	int			m_iCheckPosCnt;	//	Time counter used to check position
	bool		m_bInPreserve;	//	true, in preserve area

	float		m_fDistCnt;		//	Distance counter
	float		m_fDistance;	//	Destination distance
	bool		m_bCCW;			//	Counter-Clockwise flag

	static DWORD		m_dwLastVoice;	//	Last voice time
	AM3DSoundBuffer*	m_pRunSnd;		//	Running sound
	A3DGraphicsFX*		m_pSmokeGFX;	//	Smoke gfx

protected:	//	Operations

	void		SetState(int iState);		//	Set state
	void		Fire();						//	Fire
	void		BeginEventPoint(bool bNewPos);
	bool		ReachCurrentTarget(A3DVECTOR3& vDestPos);
	void		AdjustBalance();
	void		Aim();		//	Aim
	void		GoDead();	//	Go dead

	bool		DoState_Fire(bool bNew);	//	Do state: fire
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_ENEMYPLANE_H_

