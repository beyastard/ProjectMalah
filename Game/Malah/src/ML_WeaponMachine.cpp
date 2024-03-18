/*
 * FILE: ML_WeaponMachine.cpp
 *
 * DESCRIPTION: Weapon-machine class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_WeaponMachine.h"
#include "ML_GamePlay.h"
#include "ML_ManBullet.h"
#include "ML_Player.h"
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

void _MachineGunLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg)
{
	CMLWeaponMachine* pGun = (CMLWeaponMachine*)pArg;

	if (!_stricmp(pLogicEvent->szNotifyString, "开始发射"))
	{
		pGun->m_bFullPrepared = true;
		g_theGame.Play2DSound(RES_S2D_MACHINERUN, true);
	}
	else if (!_stricmp(pLogicEvent->szNotifyString, "停止转动"))
		pGun->m_bFullPrepared = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWeaponMachine
//
///////////////////////////////////////////////////////////////////////////

CMLWeaponMachine::CMLWeaponMachine() : CMLWeapon()
{
	m_bContinue		= true;
	m_vRelativePos	= A3DVECTOR3(0.234f, -0.875f, 0.681999f);
	m_bFullPrepared	= false;
	m_bTriggered	= false;
	m_bHasBullet	= false;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLWeaponMachine::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	m_iFireInterval = GETGAMELOGICPARAMS->iMachineGunInterval;
	m_iFireInterCnt = m_iFireInterval;

	if (m_pMoxModel)
		m_pMoxModel->SetLogicEventCallBack(_MachineGunLogicCallback, (LPVOID)this);

	return true;
}

//	Release resources
bool CMLWeaponMachine::Release()
{
	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLWeaponMachine::UnlimitedAmmo()
{
	if (m_bUnlimitedAmmo || GETGAMESWITCH(SWITCH_UNLIMITEDMACHINE))
		return true;

	return false;
}

//	Adjust weapon's position and direction
void CMLWeaponMachine::AdjustWeaponPos()
{
	CMLWeapon::AdjustWeaponPos();
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLWeaponMachine::Fire(bool bFirst)
{
	int iNumBullet = CMLWeapon::Fire(bFirst);
//	if (!iNumBullet)
//		return 0;

	if (!m_bFullPrepared)
	{
		if (bFirst || !m_bTriggered)
		{
			m_bTriggered = true;

			if (HasBullet())
				PlayModelAction("[开火]", true);
			else
				PlayModelAction("[开火空转]", true);
		}

		return 0;
	}

	if (iNumBullet)
	{
		CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();

		for (int i=0; i < iNumBullet; i++)
		{
			pBulletMan->CreateBullet(ID_BULLET_MACHINE, m_pPlayer->GetPos(), 
									 GetFireDir(), 10000.0f);
			//	Throw a bullet shell
			ThrowBulletShell();

			g_theGame.Play2DSound(RES_S2D_MACHINEFIRE, false);
		}
	}

	//	Decrease number of bullet
	if (!UnlimitedAmmo())
		AddBullet(-iNumBullet);

	if (!m_bTriggered && bFirst)
	{
		m_bTriggered = true;

		if (HasBullet())
			PlayModelAction("[立即开火]", true);
		else
			PlayModelAction("[空转]", true);

		g_theGame.Play2DSound(RES_S2D_MACHINERUN, true);
	}
	else if (m_bHasBullet && !HasBullet())
	{
		PlayModelAction("[空转]", true);
		g_theGame.Play2DSound(RES_S2D_MACHINERUN, true);
	}

	m_bHasBullet = HasBullet() ? true : false;

	return iNumBullet;
}

//	Open special function of the weapon
void CMLWeaponMachine::OpenFunction(bool bOpen)
{
}

//	Show or hide weapon
void CMLWeaponMachine::Show(bool bShow)
{
	if (bShow)
		PlayModelAction("[停]", true);

	CMLWeapon::Show(bShow);
}

//	Trigger when there is no bullet
void CMLWeaponMachine::NoBulletTrigger()
{
}

//	Stop fire immediately, including all gfx and sound
void CMLWeaponMachine::ForceStop()
{
	m_bFullPrepared	= false;
	m_bTriggered	= false;
	m_bHasBullet	= false;

	g_theGame.Stop2DSound(RES_S2D_MACHINERUN);

	CMLWeapon::ForceStop();
}

//	Release trigger
void CMLWeaponMachine::ReleaseTrigger()
{
	m_bTriggered	= false;
	m_bHasBullet	= 0;

	PlayModelAction("[开火停]", true);
	g_theGame.Stop2DSound(RES_S2D_MACHINERUN);
}

//	Get fire direction
A3DVECTOR3 CMLWeaponMachine::GetFireDir()
{
	A3DVECTOR3 vPos = m_pPlayer->GetPos() + m_pPlayer->GetCameraDir() * 100.0f;
	vPos = MLA_GetRandomPosInDisk(vPos, m_pPlayer->GetCameraUp(), m_pPlayer->GetCameraRight(), 
								  GETGAMELOGICPARAMS->iMachineGunDither, 0.0f);

	A3DVECTOR3 vDir = Normalize(vPos - m_pPlayer->GetPos());
	return vDir;
}

