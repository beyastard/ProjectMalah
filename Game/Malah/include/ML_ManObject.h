/*
 * FILE: ML_ManObject.h
 *
 * DESCRIPTION: General object manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANOBJECT_H_
#define _ML_MANOBJECT_H_

#include "ML_MsgDef.h"
#include "ML_ManBase.h"
#include "ObjectPool.h"
#include "ML_BulletShell.h"

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

class CMLUnit;
class CMLRoute;
class CMLHelicopter;
class CMLCannon;
class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLManObject
//
///////////////////////////////////////////////////////////////////////////

class CMLManObject : public CMLManBase
{
public:		//	Types

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManObject)

	CMLManObject();
	virtual ~CMLManObject() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);				//	Initialize object
	virtual bool	Release();						//	Release object

	virtual bool	LogicRun(float fTimeSpan);		//	Logic tick
	virtual int		OnMsg(AMSG* pMSG);				//	Message handler

	virtual bool	TickAnimation();					//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);		//	Render routines

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	virtual bool	LoadMissionResources(AArchive& ar);	//	Load game mission
	virtual void	ReleaseMissionResources();			//	Release mission resources

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	bool	CreateHelicopter(CMLRoute* pRoute);			//	Create helicopter
	bool	CreateAttacker(A3DVECTOR3& vAttackPos);		//	Create attacker
	bool	CreateCannon(A3DVECTOR3& vBombPos);			//	Create cannon
	bool	RenderCannon(A3DViewport* pViewport);		//	Render cannon
	bool	CopterRayTrace(MLRAYTRACE* pTrace);			//	Helicopter ray trace routine
	bool	FixedObjectRayTrace(MLRAYTRACE* pTrace);	//	Fixed object ray trace
	bool	FixedObjectExplode(CMLBullet* pBullet, bool bFire);	//	Bullet explode
	int		GetFriendPlaneNum();		//	Get current friend plane number

	bool	CreateBulletShell(int iID, A3DVECTOR3& vPos, A3DVECTOR3& vVel);	//	Create bullet shell

	CMLHelicopter*	SelectRandomCopter();	//	Select a random helicopter
	A3DCamera*		GetCannonCamera()		{	return m_pCannonCamera;	}
	A3DViewport*	GetCannonViewport()		{	return m_pCannonView;	}

protected:	//	Attributes

	DWORD	m_dwTimeCnt;	//	Time counter
	int		m_iNumCopter;	//	Number of copter

	AListT<CMLUnit*>	m_LargeList;		//	Large scaled object list
	AListT<CMLUnit*>	m_ShellList;		//	Bullet shell list
	AListT<CMLUnit*>	m_FixedObjList;		//	Fixed object list
	CMLCannon*			m_pCurCannon;		//	Current cannon

	CObjectPool<CMLBulletShell>	m_ShellPool;	//	Bullet shell allocated pool

	A3DCamera*		m_pCannonCamera;		//	Cannon camera object
	A3DViewport*	m_pCannonView;			//	Cannon viewport object

protected:	//	Operations

	void	ReleaseAllLarge();			//	Release all large scaled objects
	void	ReleaseAllShells();			//	Release all shells
	void	ReleaseAllObjects();		//	Release all objects
	bool	CreateCannonWindow();		//	Create cannon camera and viewport
	void	ReleaseCannonWindow();		//	Release cannon camera and viewport

	virtual bool	ProcessMsg(MLMSG* pMsg);	//	Handle a message
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANOBJECT_H_

