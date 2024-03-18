/*
 * FILE: ML_WeaponRifle.cpp
 *
 * DESCRIPTION: Weapon-rifle class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_WeaponRifle.h"
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
//	Implement CMLWeaponRifle
//
///////////////////////////////////////////////////////////////////////////

CMLWeaponRifle::CMLWeaponRifle() : CMLWeapon()
{
	m_fSnipeFOV		= CAMERA_FOV / GETGAMELOGICPARAMS->fRifleMagnifyScale;
	m_bContinue		= true;
	m_bTriggered	= false;
//	m_vRelativePos	= A3DVECTOR3(-0.007f, -1.78999f, -0.008f);
	m_fMaxTrembleY	= 4.0f;
	m_fMaxTrembleX	= 3.0f;
	m_bHasBullet	= false;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLWeaponRifle::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	m_iFireInterval = GETGAMELOGICPARAMS->iRifleStrafeInterval;
	m_iFireInterCnt = m_iFireInterval;

	return true;
}

//	Release resources
bool CMLWeaponRifle::Release()
{
	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLWeaponRifle::UnlimitedAmmo()
{
	if (m_bUnlimitedAmmo || GETGAMESWITCH(SWITCH_UNLIMITEDRIFLE))
		return true;

	return false;
}

//	Adjust weapon's position and direction
void CMLWeaponRifle::AdjustWeaponPos()
{
	CMLWeapon::AdjustWeaponPos();
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLWeaponRifle::Fire(bool bFirst)
{
	if (!bFirst && !m_bTriggered)
		return 0;

	int iNumBullet = CMLWeapon::Fire(bFirst);
	if (!iNumBullet)
	{
		if (bFirst && !HasBullet())
		{
			PlayModelAction("[开火无弹]", true);
			g_theGame.Play2DSound(RES_S2D_NOAMMO, false);
		}

		return 0;
	}

	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	A3DVECTOR3 vDir;

	for (int i=0; i < iNumBullet; i++)
	{
		vDir = GetFireDir();
		CMLBullet* pBullet = pBulletMan->CreateBullet(ID_BULLET_RIFLE, m_pPlayer->GetPos(), vDir, 10000.0f);
		if (pBullet && m_bFunction)
		{
			//	Scale damage in snipe mode
			int iDamage = pBullet->GetDamage();
			iDamage = (int)(iDamage * GETGAMELOGICPARAMS->fRifleSingleDamage);
			pBullet->SetDamage(iDamage);
		}

		//	Throw a bullet shell
		ThrowBulletShell();

		//	Tremble
		FireTremble();
	}

	if (!m_bFunction && (bFirst || !m_bTriggered))
	{
		PlayModelAction("[开火循环]", true);
		m_bTriggered = true;
	}

	g_theGame.Play2DSound(RES_S2D_RIFLEFIRE, false);

	//	Decrease number of bullet
	if (!UnlimitedAmmo())
		AddBullet(-iNumBullet);

	if (m_bHasBullet && !HasBullet())
	{
		PlayModelAction("[停]", true);
		g_theGame.Play2DSound(RES_S2D_NOAMMO, false);
	}

	m_bHasBullet = HasBullet() ? true : false;

	return iNumBullet;
}

/*	Open / close snipe lens

	bOpen: open / close flag
	bQuick: true, quickly open / close
*/
void CMLWeaponRifle::OpenSnipeLens(bool bOpen, bool bQuick)
{
	A3DCamera* pCamera = m_pPlayer->GetCamera();

	if (bOpen)
	{
		m_iFireInterval = GETGAMELOGICPARAMS->iRifleSingleInterval;
		m_bContinue		= false;
		m_bRender		= false;	//	Hide gun body

		if (bQuick)
			pCamera->SetFOV(DEG2RAD(m_fSnipeFOV));
		else
			pCamera->SetFOV(DEG2RAD(CAMERA_FOV));

		pCamera->SetFOVSet(DEG2RAD(m_fSnipeFOV));
	}
	else
	{
		m_iFireInterval = GETGAMELOGICPARAMS->iRifleStrafeInterval;
		m_bContinue		= true;
		m_bRender		= true;		//	Show gun body

		if (bQuick)
			pCamera->SetFOV(DEG2RAD(CAMERA_FOV));
		else
			pCamera->SetFOV(DEG2RAD(m_fSnipeFOV));

		pCamera->SetFOVSet(DEG2RAD(CAMERA_FOV));
	}
}

//	Open special function of the weapon
void CMLWeaponRifle::OpenFunction(bool bOpen)
{
	if (bOpen == m_bFunction)
		return;

	//	Don't allow open snipe lens when strafe
	if (m_bTriggered && bOpen)
		return;

	m_bFunction = bOpen;
	OpenSnipeLens(bOpen, true);

	m_iFireInterCnt = m_iFireInterval;
}

//	Stop fire immediately, including all gfx and sound
void CMLWeaponRifle::ForceStop()
{
	if (m_bFunction)
	{
		m_bFunction	= false;
		OpenSnipeLens(false, true);
	}

	m_bTriggered = false;
	m_bHasBullet = false;

	CMLWeapon::ForceStop();
}

//	Release trigger
void CMLWeaponRifle::ReleaseTrigger()
{
	PlayModelAction("[停]", true);
	m_bTriggered = false;
	m_bHasBullet = false;
}

//	Show or hide weapon
void CMLWeaponRifle::Show(bool bShow)
{
	if (bShow)
	{
		//	Rifle model hasn't '[无弹]' action :(
	//	if (HasBullet())
			PlayModelAction("[停]", true);
	//	else
	//		PlayModelAction("[无弹]", true);
	}

	CMLWeapon::Show(bShow);
}

//	Trigger when there is no bullet
void CMLWeaponRifle::NoBulletTrigger()
{
}

//	Fire tremble
void CMLWeaponRifle::FireTremble()
{
	m_fTrembleY += 0.4f;
	CLAMPMAXVALUE(m_fTrembleY, m_fMaxTrembleY);

	float fFactor = m_fTrembleY / m_fMaxTrembleY;
	float fLimitX = fFactor * 0.8f;

	m_fTrembleX += MLA_Random(-fLimitX, fLimitX);

	CLAMPVALUE(m_fTrembleX, -m_fMaxTrembleX, m_fMaxTrembleX);
}

//	Restore fire tremble
void CMLWeaponRifle::RestoreTremble()
{
	//	Restore fire tremble
	if (m_fTrembleX != 0.0f || m_fTrembleY != 0.0f)
	{
		A3DVECTOR3 vDir, vPos(m_fTrembleX, m_fTrembleY, 0.0f);
		float fLen = Normalize(vPos, vDir);
		float fDelta = 0.7f;

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
A3DVECTOR3 CMLWeaponRifle::GetFireDir()
{
	A3DVECTOR3 vDir = m_pPlayer->GetCameraDir();

	if (m_fTrembleX == 0.0f && m_fTrembleY == 0.0f)
		return vDir;
	
	float fTrembleX = m_fMaxTrembleX < 0.0f ? -m_fMaxTrembleX : m_fMaxTrembleX;
	float fFactor = (fTrembleX + m_fTrembleY) / (m_fMaxTrembleX + m_fMaxTrembleY);

	A3DVECTOR3 vPos = m_pPlayer->GetPos() + vDir * 100.0f;
	vPos = MLA_GetRandomPosInDisk(vPos, m_pPlayer->GetCameraUp(), m_pPlayer->GetCameraRight(), 
								  fFactor * 2.0f, 0.0f);

	vDir = Normalize(vPos - m_pPlayer->GetPos());
	return vDir;
}

//	Logic tick
bool CMLWeaponRifle::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	CMLWeapon::LogicRun(fTimeSpan, dwTimeCnt);

	//	Restore fire tremble
	if (!m_bTriggered || !HasBullet())
		RestoreTremble();

	return true;
}


