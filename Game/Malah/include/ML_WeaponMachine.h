/*
 * FILE: ML_WeaponMachine.h
 *
 * DESCRIPTION: Weapon-machine class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WEAPONMACHINE_H_
#define _ML_WEAPONMACHINE_H_

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
//	Class CMLWeaponMachine
//
///////////////////////////////////////////////////////////////////////////

class CMLWeaponMachine : public CMLWeapon
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLWeaponMachine();
	virtual ~CMLWeaponMachine() {}

public:		//	Attributes

public:		//	Operations

	friend void		_MachineGunLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg);

	virtual bool	Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual void	Show(bool bShow);			//	Show or hide weapon
	virtual void	AdjustWeaponPos();			//	Adjust weapon's position and direction
	virtual int		Fire(bool bFirst);			//	Fire
	virtual void	OpenFunction(bool bOpen);	//	Open special function of the weapon
	virtual bool	UnlimitedAmmo();			//	Unlimited ammo ?
	virtual void	ForceStop();				//	Stop fire immediately, including all gfx and sound
	virtual void	ReleaseTrigger();			//	Release trigger

protected:	//	Attributes

	bool		m_bFullPrepared;	//	true, can shot bullet.
	bool		m_bTriggered;		//	true, pressed trigger
	bool		m_bHasBullet;		//	true, has bullet last tick

protected:	//	Operations

	virtual void	NoBulletTrigger();		//	Trigger when there is no bullet
	A3DVECTOR3		GetFireDir();			//	Get fire direction
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_WEAPONMACHINE_H_

