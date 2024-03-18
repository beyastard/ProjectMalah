/*
 * FILE: ML_WeaponMortar.cpp
 *
 * DESCRIPTION: Weapon-mortar class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_WeaponMortar.h"
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

void _MortarLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg)
{
	CMLWeaponMortar* pMortar = (CMLWeaponMortar*)pArg;

	if (!_stricmp(pLogicEvent->szNotifyString, "开始发射"))
		pMortar->LaunchBomb();
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWeaponMortar
//
///////////////////////////////////////////////////////////////////////////

CMLWeaponMortar::CMLWeaponMortar() : CMLWeapon()
{
	m_bContinue		= false;
	m_pLocalizer	= NULL;
	m_bLocatedOk	= false;
//	m_vRelativePos	= A3DVECTOR3(0.103f, -1.791f, 0.01f);
//	m_vRelativeDir	= A3DVECTOR3(-0.04362f, 0.0f, 0.999058f);
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLWeaponMortar::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	if (m_pMoxModel)
		m_pMoxModel->SetLogicEventCallBack(_MortarLogicCallback, (LPVOID)this);

	if ((m_pLocalizer = g_theGame.LoadA3DModel(RES_MOX_MTLOCALIZER)) != NULL)
		m_pLocalizer->PlayActionByName("[播放动画]", true);

	m_iFireInterval = GETGAMELOGICPARAMS->iMortarInterval;
	m_iFireInterCnt = m_iFireInterval;

	return true;
}

//	Release resources
bool CMLWeaponMortar::Release()
{
	if (m_pLocalizer)
	{
		g_theGame.ReleaseA3DModel(m_pLocalizer);
		m_pLocalizer = NULL;
	}

	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLWeaponMortar::UnlimitedAmmo()
{
	if (m_bUnlimitedAmmo || GETGAMESWITCH(SWITCH_UNLIMITEDMORTAR))
		return true;

	return false;
}

//	Adjust weapon's position and direction
void CMLWeaponMortar::AdjustWeaponPos()
{
	CMLWeapon::AdjustWeaponPos();
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLWeaponMortar::Fire(bool bFirst)
{
	if (!bFirst)
		return 0;

	int iNumBullet = CMLWeapon::Fire(bFirst);
	if (!iNumBullet)
		return 0;

	if (UnlimitedAmmo() || GetBulletNum() > 1)
		PlayModelAction("[开火]", true);
	else
		PlayModelAction("[开火2]", true);

	return iNumBullet;
}

//	Launch bomb
void CMLWeaponMortar::LaunchBomb()
{
	A3DVECTOR3 vPos;
	if (!TraceLocalizerPos(&vPos))
		return;

	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	pBulletMan->CreateBullet(ID_BULLET_MORTAR, vPos, m_pPlayer->GetCameraDir(), 0.0f);

	g_theGame.Play2DSound(RES_S2D_MORTARFIRE, false);

	//	Decrease number of bullet
	if (!UnlimitedAmmo())
		AddBullet(-1);
}

//	Open special function of the weapon
void CMLWeaponMortar::OpenFunction(bool bOpen)
{
}

//	Show or hide weapon
void CMLWeaponMortar::Show(bool bShow)
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

//	Trigger when there is no bullet
void CMLWeaponMortar::NoBulletTrigger()
{
	PlayModelAction("[无弹]", true);
}

//	Logic tick
bool CMLWeaponMortar::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	CMLWeapon::LogicRun(fTimeSpan, dwTimeCnt);

	if (!m_pLocalizer)
		return true;

	A3DVECTOR3 vPos;
	m_bLocatedOk = TraceLocalizerPos(&vPos);

	if (m_bLocatedOk)
	{
		vPos.y += 0.2f;
		m_pLocalizer->SetPos(vPos);
	}

	return true;
}

//	Tick animation
bool CMLWeaponMortar::TickAnimation()
{
	CMLWeapon::TickAnimation();

	if (m_pLocalizer && m_bLocatedOk)
		m_pLocalizer->TickAnimation();

	return true;
}

//	Render routine
bool CMLWeaponMortar::Render(A3DViewport* pViewport)
{
	CMLWeapon::Render(pViewport);

	if (m_pLocalizer && m_bLocatedOk)
		m_pLocalizer->Render(pViewport);

	return true;
}

/*	Get localizer position

	Return true if valid localizer position was got, otherwise return false.

	pvPos (out): receive localizer position if true is returned, can not be NULL
*/
bool CMLWeaponMortar::TraceLocalizerPos(A3DVECTOR3* pvPos)
{
	//	Get position pointed by mouse
	RAYTRACE Trace;
	if (!GETMLGAMEPLAY->GetLogicWorld()->RayTraceSence(m_pPlayer->GetPos(), 
		m_pPlayer->GetCameraDir() * 1000.0f, 1.0f, &Trace))
		return false;
	
	//	Protect localizer from moving too close
	A3DVECTOR3 vPlayPos, vDropPos, vDir;

	vPlayPos	= m_pPlayer->GetPos();
	vDropPos	= Trace.vPoint;
	vPlayPos.y	= 0.0f;
	vDropPos.y	= 0.0f;

	float fDist = Normalize(vDropPos - vPlayPos, vDir);

	if (fDist < GETGAMELOGICPARAMS->fMortarMinDist)
	{	
		*pvPos	 = vPlayPos + vDir * GETGAMELOGICPARAMS->fMortarMinDist;
		pvPos->y = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(*pvPos);
	}
	else
		*pvPos = Trace.vPoint;

	return true;
}

