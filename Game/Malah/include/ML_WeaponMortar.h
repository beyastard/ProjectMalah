/*
 * FILE: ML_WeaponMortar.h
 *
 * DESCRIPTION: Weapon-mortar class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WEAPONMORTAR_H_
#define _ML_WEAPONMORTAR_H_

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
//	Class CMLWeaponMortar
//
///////////////////////////////////////////////////////////////////////////

class CMLWeaponMortar : public CMLWeapon
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLWeaponMortar();
	virtual ~CMLWeaponMortar() {}

public:		//	Attributes

public:		//	Operations

	friend void		_MortarLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg);

	virtual bool	Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual bool	TickAnimation();								//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);					//	Render routine

	virtual void	Show(bool bShow);			//	Show or hide weapon
	virtual void	AdjustWeaponPos();			//	Adjust weapon's position and direction
	virtual int		Fire(bool bFirst);			//	Fire
	virtual void	OpenFunction(bool bOpen);	//	Open special function of the weapon
	virtual bool	UnlimitedAmmo();			//	Unlimited ammo ?

protected:	//	Attributes

	A3DModel*	m_pLocalizer;		//	Localizer model
	bool		m_bLocatedOk;		//	true, localizer located ok

protected:	//	Operations

	virtual void	NoBulletTrigger();		//	Trigger when there is no bullet

	void		LaunchBomb();		//	Launch bomb
	bool		TraceLocalizerPos(A3DVECTOR3* pvPos);	//	Get localizer position
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_WEAPONMORTAR_H_

