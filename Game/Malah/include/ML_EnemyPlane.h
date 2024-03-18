/*
 * FILE: ML_EnemyPlane.h
 *
 * DESCRIPTION: Enemy plane class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_ENEMYPLANE_H_
#define _ML_ENEMYPLANE_H_

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

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEnemyPlane
//
///////////////////////////////////////////////////////////////////////////

class CMLEnemyPlane : public CMLModelUnit
{
public:		//	Types

	enum
	{
		ST_NONE = 0,
		ST_DIVE,		//	Dive
		ST_UP,			//	Up
		ST_LEAVE,		//	Leave
	};

public:		//	Constructor and Destructor

	CMLEnemyPlane();
	virtual ~CMLEnemyPlane() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(int iID, A3DVECTOR3& vMoveDir);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual void	SetPos(A3DVECTOR3& vPos);	//	Set position
	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

	bool	RayTrace(MLRAYTRACE* pTrace);		//	Ray trace function
	void	Strike(CMLBullet* pBullet);			//	Strike

	int		GetLife()	{	return m_iLife;		}

protected:	//	Attributes

	int			m_iState;		//	State
	int			m_iLife;		//	Plane's life
	A3DVECTOR3	m_vxzPos;		//	Plane's position on xz plane
	A3DVECTOR3	m_vAttackDir;	//	Attack direction on sz plane
	A3DVECTOR3	m_vAttackRight;	//	Right direction relate to m_vAttackDir
	A3DVECTOR3	m_vCurMoveDir;	//	Current moving direction

	float		m_fTimeSpan;	//	Time spane of this logic tick
	int			m_iTimeSpan;	//	Time spane of this logic tick
	DWORD		m_dwTimeCnt;	//	time counter of this logic tick

	float		m_fDistCnt;		//	Distance counter
	float		m_fDistance;	//	Distance
	float		m_fPitchCnt;	//	Pitch counter
	float		m_fPitchDelta;	//	Pitch change delta
	float		m_fPitchDest;	//	Pitch destination

	bool		m_bFire;		//	Being fire
	int			m_iFireInter;	//	Fire interval
	int			m_iFireTime;	//	Fire time counter
	int			m_iFireNumCnt;	//	Fire number counter
	int			m_iFireNumber;	//	Fire number
	A3DVECTOR3	m_vFireStart;	//	Fire start position (MIG-21)

protected:	//	Operations

	void		SetState(int iState);	//	Set state
	void		AdjustModelPitch(float fTickTime);	//	Adjust model pitch and
	void		CreateBullet(int iCount);	//	Create a bullet

	void		LogicRun_MIG21();		//	Logic run function for MIG-21
	void		LogicRun_fantan();		//	Logic run function for Fantan
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_ENEMYPLANE_H_

