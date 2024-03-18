/*
 * FILE: ML_ManBullet.h
 *
 * DESCRIPTION: Bullet manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/6
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANBULLET_H_
#define _ML_MANBULLET_H_

#include "ObjectPool.h"
#include "ML_Bullet.h"
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
//	Class CMLManBullet
//
///////////////////////////////////////////////////////////////////////////

class CMLManBullet : public CMLManBase
{
public:		//	Types

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManBullet)

	CMLManBullet();
	virtual ~CMLManBullet() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);				//	Initialize object
	virtual bool	Release();						//	Release object

	virtual bool	LogicRun(float fTimeSpan);		//	Logic tick
	virtual bool	TickAnimation();				//	Tick animation
	virtual int		OnMsg(AMSG* pMSG);				//	Message handler

	virtual bool	Render(A3DViewport* pViewport);	//	Render routines

	virtual void	PauseSound(bool bPause);		//	Pause sound
	virtual void	StopSound();					//	Stop sound

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	CMLBullet*	CreateBullet(int iBulletID, A3DVECTOR3& vStart, A3DVECTOR3& vDir, float fSpeed);	//	Create bullet

protected:	//	Attributes

	DWORD		m_dwTimeCnt;	//	Time counter

	CObjectPool<CMLBullet>	m_BulletPool;		//	Bullet pool
	AListT<CMLBullet*>		m_BulletList;		//	Bullet list

protected:	//	Operations

	virtual bool	ProcessMsg(MLMSG* pMsg);	//	Handle a message

	void		ReleaseAllBullets();			//	Release all bullets
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANBULLET_H_

