/*
 * FILE: ML_WeaponRPG.h
 *
 * DESCRIPTION: Weapon-RPG class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/4/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WEAPONRPG_H_
#define _ML_WEAPONRPG_H_

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
//	Class CMLWeaponRPG
//
///////////////////////////////////////////////////////////////////////////

class CMLWeaponRPG : public CMLWeapon
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLWeaponRPG();
	virtual ~CMLWeaponRPG() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

	virtual void	Show(bool bShow);			//	Show or hide weapon
	virtual void	AdjustWeaponPos();			//	Adjust weapon's position and direction
	virtual int		Fire(bool bFirst);			//	Fire
	virtual void	OpenFunction(bool bOpen);	//	Open special function of the weapon
	virtual bool	UnlimitedAmmo();			//	Unlimited ammo ?

protected:	//	Attributes

protected:	//	Operations

	virtual void	NoBulletTrigger();		//	Trigger when there is no bullet
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_WEAPONRPG_H_

