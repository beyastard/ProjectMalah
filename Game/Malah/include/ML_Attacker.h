/*
 * FILE: ML_Attacker.h
 *
 * DESCRIPTION: Attacker class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_ATTACKER_H_
#define _ML_ATTACKER_H_

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
//	Class CMLAttacker
//
///////////////////////////////////////////////////////////////////////////

class CMLAttacker : public CMLModelUnit
{
public:		//	Types

	//	States
	enum
	{
		ST_NONE = 0,	//	None
		ST_WAIT,		//	Wait to appear
		ST_APPEAR,		//	Appear
		ST_NORMALFLY,	//	Normal fly
		ST_DISAPPEAR,	//	Disappear
	};

public:		//	Constructor and Destructor

	CMLAttacker();
	virtual ~CMLAttacker() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(A3DVECTOR3 vAttackPos);	//	Initialize object
	virtual bool	Release();	//	Release resources

	virtual void	SetPos(A3DVECTOR3& vPos);	//	Set position

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);	//	Logic tick

protected:	//	Attributes

	int			m_iState;		//	State
	A3DVECTOR3	m_vxzPos;		//	Attacker's position on xz plane
	A3DVECTOR3	m_vAttackPos;	//	Attack poistion
	A3DVECTOR3	m_vAttackDir;	//	Attack direction
	float		m_fDistance;	//	Distance
	float		m_fDistCnt;		//	Distance counter

	float		m_fDropTime;	//	Drop bomb time
	float		m_fDropTimeCnt;	//	Drop time counter
	float		m_fDropInter;	//	Drop interval time
	int			m_iBombCnt;		//	Bomb dropped counter
	bool		m_bDropping;	//	Dropping bombs
	A3DVECTOR3	m_vDropPos;		//	Postion to drop the first bomb
	A3DVECTOR3	m_vDropDist;	//	Distance between two bombs

protected:	//	Operations

	void		SetState(int iState);	//	Set state
	void		CancluateDropTime(A3DVECTOR3& vCurPos);		//	Calculate drop bomb time
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_ATTACKER_H_

