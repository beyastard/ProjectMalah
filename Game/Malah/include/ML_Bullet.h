/*
 * FILE: ML_Bullet.h
 *
 * DESCRIPTION: Bullet class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/6
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_BULLET_H_
#define _ML_BULLET_H_

#include "ML_Unit.h"

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
//	Class CMLBullet
//
///////////////////////////////////////////////////////////////////////////

class CMLBullet : public CMLUnit
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLBullet();
	virtual ~CMLBullet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(int iBulletID, A3DVECTOR3& vStart, A3DVECTOR3& vDir, float fSpeed);	//	Initialize object
	virtual bool	Release();

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual bool	Render(A3DViewport* pViewport);					//	Render routine
	virtual bool	TickAnimation();								//	Tick animation

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	void		SetLimitDist(float fDist)	{	m_fLimitDist = fDist;	}
	float		GetLimitDist()				{	return m_fLimitDist;	}
	void		SetDamage(int iDamage)		{	m_iDamage = iDamage;	}
	int			GetDamage()					{	return m_iDamage;		}
	void		SetWaitTime(int iTime)		{	m_iWaitTime = iTime;	}
	DWORD		GetFlag()					{	return m_dwFlag;		}
	void		SetFlag(DWORD dwFlag)		{	m_dwFlag = dwFlag;		}

	float		GetInnerRadius()			{	return m_fInnerRadius;	}
	float		GetOuterRadius()			{	return m_fOuterRadius;	}
	void		SetRenderFlag(bool bRender)	{	m_bRender = bRender;	}
	bool		GetRenderFlag()				{	return m_bRender;		}

protected:	//	Attributes

	A3DVECTOR3	m_vMoveDir;			//	Move direction
	float		m_fTimeSpan;		//	Time span
	int			m_iTimeSpan;
	DWORD		m_dwTimeCnt;		//	Time counter from mission start
	float		m_fLimitDist;		//	Limit distance
	float		m_fDistCnt;			//	Distance counter
	int			m_iDamage;			//	Bullet's damage
	bool		m_bRender;			//	Render flag
	DWORD		m_dwFlag;			//	Extra flag

	float		m_fInnerRadius;		//	Explosion inner radius
	float		m_fOuterRadius;		//	Explostion outer radius

	int			m_iState;			//	State
	int			m_iWaitTime;		//	Wait time
	int			m_iWaitCnt;			//	Wait counter

	A3DModel*			m_pMoxModel;	//	Bullet model
	AM3DSoundBuffer*	m_pSound0;		//	Sound buffer
	A3DGraphicsFX*		m_pGFX;			//	GFX

	static int	m_iRocketSoundCnt;		//	Rocket sound counter
	bool		m_bChangeRocketSound;	//	true, this object has changed m_iRocketSoundCnt

protected:	//	Operations

	void		LogicRun_Pistol();		//	Pistol bullet logic run
	void		LogicRun_Rifle();		//	Rifle bullet logic run
	void		LogicRun_Mortar();		//	Mortar bullet logic run
	void		LogicRun_Machine();		//	Machine gun bullet logic run
	void		LogicRun_Fireball();	//	Fire ball logic run
	void		LogicRun_Cannon();		//	Cannon ball logic run
	void		LogicRun_Rocket();		//	AI's rocket logic run
	void		LogicRun_MIG21();		//	MIG-21's bullet
	void		LogicRun_RPG();			//	RGP bullet

	void		PlayerGunBulletHit(MLRAYTRACE& Trace);	//	Player's gun bullet hit
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_BULLET_H_

