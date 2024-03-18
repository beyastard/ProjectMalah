/*
 * FILE: ML_WeaponRifle.h
 *
 * DESCRIPTION: Weapon-rifle class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WEAPONRIFLE_H_
#define _ML_WEAPONRIFLE_H_

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
//	Class CMLWeaponRifle
//
///////////////////////////////////////////////////////////////////////////

class CMLWeaponRifle : public CMLWeapon
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLWeaponRifle();
	virtual ~CMLWeaponRifle() {}

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
	virtual void	ForceStop();				//	Stop fire immediately, including all gfx and sound
	virtual void	ReleaseTrigger();			//	Release trigger

protected:	//	Attributes

	float		m_fSnipeFOV;		//	FOV in snipe state
	bool		m_bTriggered;		//	Triggered flag
	float		m_fMaxTrembleY;		//	Maximum tremble on y, in degree
	float		m_fMaxTrembleX;		//	Maximum tremble on x, in degree
	bool		m_bHasBullet;		//	true, has bullet last tick

protected:	//	Operations

	virtual void	NoBulletTrigger();		//	Trigger when there is no bullet
	void			OpenSnipeLens(bool bOpen, bool bQuick);		//	Open / close snipe lens
	void			FireTremble();			//	Fire tremble
	void			RestoreTremble();		//	Restore fire tremble
	A3DVECTOR3		GetFireDir();			//	Get fire direction
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_WEAPONRIFLE_H_

