/*
 * FILE: ML_WeaponPistol.cpp
 *
 * DESCRIPTION: Weapon-pistol class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_WeaponPistol.h"
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
//	Implement CMLWeaponPistol
//
///////////////////////////////////////////////////////////////////////////

CMLWeaponPistol::CMLWeaponPistol() : CMLWeapon()
{
	m_bContinue		= false;
//	m_vRelativePos	= A3DVECTOR3(-0.04f, -1.64999f, -0.0f);
//	m_vRelativeDir	= A3DVECTOR3(-0.068886f, 0.0f, 0.997627f);
	m_fMaxTrembleY	= 5.0f;
	m_fMaxTrembleX	= 0.1f;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLWeaponPistol::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	m_iFireInterval = GETGAMELOGICPARAMS->iPistolInterval;
	m_iFireInterCnt = m_iFireInterval;

	return true;
}

//	Release resources
bool CMLWeaponPistol::Release()
{
	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLWeaponPistol::UnlimitedAmmo()
{
	if (m_bUnlimitedAmmo || GETGAMESWITCH(SWITCH_UNLIMITEDPISTOL))
		return true;

	return false;
}

//	Adjust weapon's position and direction
void CMLWeaponPistol::AdjustWeaponPos()
{
	CMLWeapon::AdjustWeaponPos();
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLWeaponPistol::Fire(bool bFirst)
{
	if (!bFirst)
		return 0;

	int iNumBullet = CMLWeapon::Fire(bFirst);
	if (!iNumBullet)
		return 0;

	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	pBulletMan->CreateBullet(ID_BULLET_PISTOL, m_pPlayer->GetPos(), 
							 GetFireDir(), 10000.0f);

	//	Throw a bullet shell
	ThrowBulletShell();

	//	Tremble
	FireTremble();

	if (UnlimitedAmmo() || GetBulletNum() > 1)
		PlayModelAction("[开火]", true);
	else
		PlayModelAction("[最后一枪]", true);

	g_theGame.Play2DSound(RES_S2D_PISTOLFIRE, false);

	//	Decrease number of bullet
	if (!UnlimitedAmmo())
		AddBullet(-iNumBullet);

	return iNumBullet;
}

//	Show or hide weapon
void CMLWeaponPistol::Show(bool bShow)
{
	if (bShow)
	{
		if (HasBullet())
			PlayModelAction("[停]", true);
		else
			PlayModelAction("[无弹]", true);
	}

	CMLWeapon::Show(bShow);
}

//	Open special function of the weapon
void CMLWeaponPistol::OpenFunction(bool bOpen)
{
}

//	Trigger when there is no bullet
void CMLWeaponPistol::NoBulletTrigger()
{
	PlayModelAction("[无弹]", true);
	g_theGame.Play2DSound(RES_S2D_NOAMMO, false);
}

//	Fire tremble
void CMLWeaponPistol::FireTremble()
{
	m_fTrembleY += 2.0f;
	m_fTrembleX += MLA_Random(-0.1f, 0.1f);

	CLAMPMAXVALUE(m_fTrembleY, m_fMaxTrembleY);
	CLAMPVALUE(m_fTrembleX, -m_fMaxTrembleX, m_fMaxTrembleX);
}

//	Restore fire tremble
void CMLWeaponPistol::RestoreTremble()
{
	//	Restore fire tremble
	if (m_fTrembleX != 0.0f || m_fTrembleY != 0.0f)
	{
		A3DVECTOR3 vDir, vPos(m_fTrembleX, m_fTrembleY, 0.0f);
		float fLen = Normalize(vPos, vDir);
		float fDelta = 0.17f;

		if (fLen < fDelta)
		{
			m_fTrembleX = 0.0f;
			m_fTrembleY = 0.0f;
		}
		else
		{
			A3DVECTOR3 vNewPos = vPos - vDir * fDelta;
			if (vPos.x * vNewPos.x < 0.0f)
				vNewPos.x = 0.0f;

			if (vNewPos.y < 0.0f)
				vNewPos.y = 0.0f;

			m_fTrembleX = vNewPos.x;
			m_fTrembleY = vNewPos.y;
		}
	}
}

//	Get fire direction
A3DVECTOR3 CMLWeaponPistol::GetFireDir()
{
	A3DVECTOR3 vDir = m_pPlayer->GetCameraDir();

	if (m_fTrembleX == 0.0f && m_fTrembleY == 0.0f)
		return vDir;
	
	float fTrembleX = m_fMaxTrembleX < 0.0f ? -m_fMaxTrembleX : m_fMaxTrembleX;
	float fFactor = (fTrembleX + m_fTrembleY) / (m_fMaxTrembleX + m_fMaxTrembleY);

	A3DVECTOR3 vPos = m_pPlayer->GetPos() + vDir * 100.0f;
	vPos = MLA_GetRandomPosInDisk(vPos, m_pPlayer->GetCameraUp(), m_pPlayer->GetCameraRight(), 
								  fFactor * 1.8f, 0.0f);

	vDir = Normalize(vPos - m_pPlayer->GetPos());
	return vDir;
}

//	Logic tick
bool CMLWeaponPistol::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	CMLWeapon::LogicRun(fTimeSpan, dwTimeCnt);

	//	Restore fire tremble
	RestoreTremble();

	return true;
}


