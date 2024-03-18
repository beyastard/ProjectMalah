/*
 * FILE: ML_Weapon.h
 *
 * DESCRIPTION: Weapon class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WEAPON_H_
#define _ML_WEAPON_H_

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

class CMLPlayer;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWeapon
//
///////////////////////////////////////////////////////////////////////////

class CMLWeapon : public CMLModelUnit
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLWeapon();
	virtual ~CMLWeapon() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser);	//	Initialize object
	virtual bool	Release();						//	Release resources
	virtual bool	LoadModel(bool bLoadModel);		//	Load model

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual void	Show(bool bShow);			//	Show or hide weapon

	virtual void	AdjustWeaponPos();			//	Adjust weapon's position and direction
	virtual void	OpenFunction(bool bOpen) {}	//	Open special function of the weapon
	virtual void	ReleaseTrigger() {}			//	Release trigger
	virtual int		Fire(bool bFirst);			//	Fire (pressed trigger)
	virtual void	ForceStop();				//	Force to stop fire immediately, including all gfx and sound
	
	void		AddBullet(int iNum);		//	Add bullet number
	void		AddCassette(int iNum);		//	Add cassette number
	bool		HasBullet();				//	Still has bullet ?
	void		SetBullet(int iNum);		//	Set number of bullet

	void		MoveRelativePos(A3DVECTOR3& vOff);		//	Move relative position
	void		PitchRelativeDir(float fPitch);			//	Pitch relative diretion
	void		YawRelativeDir(float fYaw);				//	Yaw relative direction
	
	bool		CanContinueFire()	{	return m_bContinue;			}
	bool		IsCassetteWeapon()	{	return m_bCassette;			}
	bool		FunctionOpened()	{	return m_bFunction;			}
	int			GetCassetteNum()	{	return m_iNumCassette;		}
	int			GetBulletNum()		{	return m_iNumBullet;		}
	int			GetMaxBullet()		{	return m_iMaxBullet;		}
	int			GetCassetteBullet()	{	return m_iCassetteBullet;	}
	int			GetCassetteVolume()	{	return m_iCassetteVol;		}
	A3DVECTOR3	GetRelativePos()	{	return m_vRelativePos;		}
	A3DVECTOR3	GetRelativeDir()	{	return m_vRelativeDir;		}
	float		GetTrembleX()		{	return m_fTrembleX;			}
	float		GetTrembleY()		{	return m_fTrembleY;			}

	void	SetRelativePos(A3DVECTOR3& v)		{	m_vRelativePos = v;					}
	void	SetRelativeDir(A3DVECTOR3& v)		{	m_vRelativeDir = v;					}
	bool	SetUnlimitedAmmo(bool bUnlimited)	{	m_bUnlimitedAmmo = bUnlimited;		}
	void	SetCassetteVolume(int iVolume)		{	m_iCassetteVol = iVolume;			}
	void	StopTremble()						{	m_fTrembleX = m_fTrembleY = 0.0f;	}
	void	SetMaxBullet(int iNum)				{	m_iMaxBullet = iNum;				}

	virtual bool	UnlimitedAmmo()	{	return m_bUnlimitedAmmo;	}
	virtual bool	CanFire()		{	return m_iFireInterCnt >= m_iFireInterval;	}

	inline void		IncFireIntervalCnt(int iTime, bool bClamp);		//	Increase fire interval counter

protected:	//	Attributes

	CMLPlayer*	m_pPlayer;			//	Weapon user
	bool		m_bContinue;		//	Continue fire enable
	bool		m_bFunction;		//	true, function opened
	bool		m_bLoadModel;		//	Load model flag

	A3DVECTOR3	m_vRelativePos;		//	Position relative to player's position
	A3DVECTOR3	m_vRelativeDir;		//	Direction relative to player's direction

	int			m_iFireInterval;	//	Fire interval
	int			m_iFireInterCnt;	//	fire interval counter

	bool		m_bUnlimitedAmmo;	//	true, unlimited ammo
	bool		m_bCassette;		//	true, the weapon use cassette
	int			m_iCassetteVol;		//	Volume of each cassette
	int			m_iNumCassette;		//	Number of cassette
	int			m_iNumBullet;		//	Total number of bullet
	int			m_iCassetteBullet;	//	Number of bullet in current cassette
	int			m_iMaxBullet;		//	Maximum of bullet, -1 means unlimited

	float		m_fTrembleX;		//	Tremble degree on x axis
	float		m_fTrembleY;		//	Tremble degree on y axis

protected:	//	Operations

	void		ThrowBulletShell();			//	Throw bullet shell

	virtual void	NoBulletTrigger() {}	//	Trigger when there is no bullet
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

/*	Increase fire interval counter

	iTime: time increament
	bClamp: true, clamp m_iFireInterCnt to m_iFireInterval if overflow
*/
void CMLWeapon::IncFireIntervalCnt(int iTime, bool bClamp)
{
	m_iFireInterCnt += iTime;
	if (bClamp && m_iFireInterCnt > m_iFireInterval)
		m_iFireInterCnt = m_iFireInterval;
}


#endif	//	_ML_WEAPON_H_

