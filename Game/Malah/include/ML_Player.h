/*
 * FILE: ML_Player.h
 *
 * DESCRIPTION: Player class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_PLAYER_H_
#define _ML_PLAYER_H_

#include "ML_Unit.h"
#include "ML_ID.h"

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

class CMLLogicWorld;
class CMLWeapon;
class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLPlayer
//
///////////////////////////////////////////////////////////////////////////

class CMLPlayer : public CMLUnit
{
public:		//	Types

	//	Weapon index
	enum
	{
		WEAPON_PISTOL	= 0,	//	Pistol
		WEAPON_RIFLE	= 1,	//	Rifle
		WEAPON_MORTAR	= 2,	//	Mortar
		WEAPON_MACHINE	= 3,	//	Machine gun
		WEAPON_RPG		= 4,	//	RPG
		WEAPON_ATTACKER	= 5,	//	Attacker caller
		WEAPON_CANNON	= 6,	//	Cannon caller
	};

	//	Support struct
	struct SUPPORT
	{
		int		iInterval;		//	Interval time
		int		iTimeCnt;		//	Time counter
		int		iCallCnt;		//	Number of calling time
	};

public:		//	Constructor and Destructor

	CMLPlayer();
	virtual ~CMLPlayer() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(CMLLogicWorld* pWorld, A3DCamera* pCamera);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual int		OnMsg(MLMSG* pMsg);			//	Message handler
	virtual void	SetPos(A3DVECTOR3& vPos);	//	Set position

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick
	virtual bool	TickAnimation();								//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);					//	Render routine

	bool		OnStartGamePlay();				//	Called when begin game playing
	bool		OnEndGamePlay();				//	Called when end game playing
	bool		OnBeginMission(int iMission);	//	Called when being mission
	bool		OnResetMission();				//	Called when reset mission
	bool		OnEndMission();					//	Called when end mission

	bool		RayTrace(MLRAYTRACE* pTrace);	//	Ray trace routine
	void		ChangeWeapon(int iWeaponID);	//	Change user's weapon
	bool		SupportReady(int iSupportID);	//	Support is ready ?
	bool		CallSupport(int iSupportID);	//	Call support
	void		RestoreOldWeapon();				//	Restore old weapon
	void		Strike(CMLBullet* pBullet);		//	Strike player
	void		Strike(int iDamage);			//	Strike player
	void		Stabbed(int iDamage);			//	Be stabbed
	void		Shake();						//	Begin to shake
	void		StopFire();						//	Force to stop fire

	void		SupplyAmmo(int iWeaponID, int iNum);	//	Supply weapon ammo
	void		SupplyLife(int iAmount);				//	Supply life
	int			GetSupportCounter(int iSupportID, int* piMax);	//	Get support counter
	A3DVECTOR3	GetRandomHitPos();
	A3DVECTOR3	GetRandomMissPos(A3DVECTOR3& vFirePos, float fMissRad);

	A3DVECTOR3	GetCameraDir()		{	return m_pCamera->GetDir();		}
	A3DVECTOR3	GetCameraUp()		{	return m_pCamera->GetUp();		}
	A3DVECTOR3	GetCameraRight()	{	return m_pCamera->GetRight();	}

	A3DCamera*	GetCamera()			{	return m_pCamera;	}
	float		GetBoundingRadius()	{	return m_fRadius;	}
	A3DVECTOR3	GetAABBExtents()	{	return m_vExtents;	}
	int			GetLife()			{	return m_iLife;		}
	int			GetMaxLife()		{	return m_iMaxLife;	}

	inline void			SetDir(A3DVECTOR3& vDir);	//	Set direction
	inline CMLWeapon*	GetCurrentWeapon();			//	Get current weapon

protected:	//	Attributes

	A3DCamera*		m_pCamera;		//	Player's cameara
	CMLLogicWorld*	m_pWorld;		//	World object

	bool		m_bWeaponModel;		//	true, weapon model is loaded
	float		m_fRadius;			//	Radius of plyaer's bounding sphere
	A3DVECTOR3	m_vExtents;			//	Player's AABB extents
	int			m_iMaxLife;			//	Maximum life value
	int			m_iLife;			//	Current Life value
	float		m_fCurPitch;		//	Current pitch in degree
	float		m_fCurYaw;			//	Current jaw in degree

	A3DVECTOR3	m_vShakePos;		//	View position when shake start
	bool		m_bShaking;			//	true, view is shaking
	int			m_iShakeTimeCnt;	//	Shake time counter
	int			m_iShakeTime;		//	Time length of shaking

	CMLWeapon*	m_aWeapons[NUM_WEAPON_ID];		//	Weapons
	int			m_iCurWeapon;					//	Current weapon
	int			m_iOldWeapon;					//	Old weapon

	int			m_iAskingVoice;		//	Asking voice sound
	int			m_iReplyVoice;		//	Reply voice sound
	int			m_iReplyTimeCnt;	//	Deny voice time counter

	SUPPORT		m_aSupports[NUM_SUPPORT_ID];	//	Supports information

protected:	//	Operations

	void		Fire(bool bFirst);			//	Fire routine
	void		DoShake(int iTickSpan);		//	Do shake
	void		Pitch(float fDegree);		//	Pitch
	void		Yaw(float fDegree);			//	Yaw

	int			GetSupplyVoice(int iSupportID, int iWhich);	//	Get supply voice
	void		LoadWeaponsPosAndDir();						//	Load weapons' position and direction
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Get current weapon
CMLWeapon* CMLPlayer::GetCurrentWeapon()
{
	if (m_iCurWeapon < 0)
		return NULL;
	
	return m_aWeapons[m_iCurWeapon];
}

//	Set direction
void CMLPlayer::SetDir(A3DVECTOR3& vDir)
{
	if (m_pCamera)
		m_pCamera->SetDirAndUp(vDir, g_vAxisY);
}

#endif	//	_ML_PLAYER_H_
