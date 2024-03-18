/*
 * FILE: ML_AttackerCaller.h
 *
 * DESCRIPTION: Attacker caller class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_ATTACKERCALLER_H_
#define _ML_ATTACKERCALLER_H_

#include "ML_Weapon.h"

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
//	Class CMLAttackerCaller
//
///////////////////////////////////////////////////////////////////////////

class CMLAttackerCaller : public CMLWeapon
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLAttackerCaller();
	virtual ~CMLAttackerCaller() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual bool	TickAnimation();								//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);					//	Render routine

	virtual int		Fire(bool bFirst);			//	Fire
	virtual void	ReleaseTrigger();			//	Release trigger
	virtual bool	UnlimitedAmmo();			//	Unlimited ammo ?

	virtual bool	CanFire();		//	Can fire ?

	A3DVECTOR3		GetLocatedPos()		{	return m_vLocatedPos;	}

protected:	//	Attributes

	A3DModel*	m_pLocalizer;		//	Localizer model
	bool		m_bLocatedOk;		//	true, localizer located ok
	A3DVECTOR3	m_vLocatedPos;		//	Located position

protected:	//	Operations

	bool		TraceLocalizerPos(A3DVECTOR3* pvPos);	//	Get localizer position
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_ATTACKERCALLER_H_

