/*
 * FILE: ML_WeaponRPG.cpp
 *
 * DESCRIPTION: Weapon-RPG class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/4/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_WeaponRPG.h"
#include "ML_GamePlay.h"
#include "ML_Player.h"
#include "ML_ManBullet.h"
#include "ML_Resources.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWeaponRPG
//
///////////////////////////////////////////////////////////////////////////

CMLWeaponRPG::CMLWeaponRPG() : CMLWeapon()
{
	m_bContinue		= false;
//	m_vRelativePos	= A3DVECTOR3(-0.11f, -1.747995f, 0.132f);
//	m_vRelativeDir	= A3DVECTOR3(0.231751f, 0.072092f, 0.970114f);
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLWeaponRPG::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	m_iFireInterval = GETGAMELOGICPARAMS->iRPGInterval;
	m_iFireInterCnt = m_iFireInterval;

	return true;
}

//	Release resources
bool CMLWeaponRPG::Release()
{
	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLWeaponRPG::UnlimitedAmmo()
{
	if (m_bUnlimitedAmmo || GETGAMESWITCH(SWITCH_UNLIMITEDRPG))
		return true;

	return false;
}

//	Adjust weapon's position and direction
void CMLWeaponRPG::AdjustWeaponPos()
{
	CMLWeapon::AdjustWeaponPos();
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLWeaponRPG::Fire(bool bFirst)
{
	if (!bFirst)
		return 0;

	int iNumBullet = CMLWeapon::Fire(bFirst);
	if (!iNumBullet)
		return 0;

	A3DVECTOR3 vPos = g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_效果挂点", NULL);
	A3DVECTOR3 vDir = Normalize(m_pPlayer->GetPos() + m_pPlayer->GetCameraDir() * 200.0f - vPos);

	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	pBulletMan->CreateBullet(ID_BULLET_RPG, vPos, vDir, GETGAMELOGICPARAMS->fAIRocketSpeed);

	//	Throw a bullet shell
//	ThrowBulletShell();

	if (UnlimitedAmmo() || GetBulletNum() > 1)
		PlayModelAction("[开火上弹]", true);
	else
		PlayModelAction("[开火上空弹]", true);

	g_theGame.Play2DSound(RES_S2D_RPGFIRE, false);
	g_theGame.PlayGFX(RES_GFX_ROCKETLAUNCH, vPos, 1.0f);

	//	Decrease number of bullet
	if (!UnlimitedAmmo())
		AddBullet(-iNumBullet);

	return iNumBullet;
}

//	Show or hide weapon
void CMLWeaponRPG::Show(bool bShow)
{
	if (bShow)
	{
		if (HasBullet())
			PlayModelAction("[停有弹]", true);
		else
			PlayModelAction("[停无弹]", true);
	}

	CMLWeapon::Show(bShow);
}

//	Open special function of the weapon
void CMLWeaponRPG::OpenFunction(bool bOpen)
{
}

//	Trigger when there is no bullet
void CMLWeaponRPG::NoBulletTrigger()
{
	PlayModelAction("[停无弹]", true);
	g_theGame.Play2DSound(RES_S2D_NOAMMO, false);
}

//	Logic tick
bool CMLWeaponRPG::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	CMLWeapon::LogicRun(fTimeSpan, dwTimeCnt);
	return true;
}


