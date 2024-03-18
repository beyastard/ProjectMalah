/*
 * FILE: ML_Bullet.cpp
 *
 * DESCRIPTION: Bullet class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/6
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Bullet.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
#include "ML_AIUnit.h"
#include "ML_Helicopter.h"
#include "ML_Player.h"
#include "ML_EnemyPlane.h"
#include "ML_FixedObject.h"
#include "ML_EnemyVehicle.h"

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

int CMLBullet::m_iRocketSoundCnt = 0;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLBullet
//
///////////////////////////////////////////////////////////////////////////

CMLBullet::CMLBullet() : CMLUnit()
{
	m_fLimitDist	= 500.0f;
	m_fDistCnt		= 0.0f;
	m_iDamage		= 0;
	m_fInnerRadius	= 1.0f;
	m_fOuterRadius	= 3.0f;
	m_pMoxModel		= NULL;
	m_pSound0		= NULL;
	m_pGFX			= NULL;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iBulletID: bullet ID.
	vStart: bullet's start position
	vDir: bullet's direction
	fSpeed: bullet's speed
*/
bool CMLBullet::Init(int iBulletID, A3DVECTOR3& vStart, A3DVECTOR3& vDir, float fSpeed)
{
	SetId(iBulletID);

	SetPos(vStart);
	SetVel(vDir * fSpeed);

	m_fDistCnt		= 0.0f;
	m_bDead			= false;
	m_iWaitCnt		= 0;
	m_vMoveDir		= vDir;
	m_iState		= 0;
	m_dwFlag		= 0;

	m_bChangeRocketSound = false;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	switch (iBulletID)
	{
	case ID_BULLET_MORTAR:

		m_fInnerRadius	= GETGAMELOGICPARAMS->fMortarInnerRadius;
		m_fOuterRadius	= GETGAMELOGICPARAMS->fMortarOuterRadius;
		m_iWaitTime		= GETGAMELOGICPARAMS->iMortarArriveTime - 1000;
		break;

	case ID_BULLET_FIREBALL:

		m_fInnerRadius	= GETGAMELOGICPARAMS->fFireballInnerRadius;
		m_fOuterRadius	= GETGAMELOGICPARAMS->fFireballOuterRadius;
		break;

	case ID_BULLET_CANNON:

		m_fInnerRadius	= GETGAMELOGICPARAMS->fCannonInnerRadius;
		m_fOuterRadius	= GETGAMELOGICPARAMS->fCannonOuterRadius;
		m_iWaitTime		= MLA_Random(1000, 2500);
		break;

	case ID_BULLET_ROCKET:
	case ID_BULLET_FANTAN:
	case ID_BULLET_ARMORED:
	
		//	Load rocket model
		m_pMoxModel		= g_theGame.LoadA3DModel(RES_MOX_ROCKETBULLET);
		m_pGFX			= g_theGame.LoadGFX(RES_GFX_ROCKETFLY0+GETMLGAMEPLAY->GetCurUserConfigs()->iGFXQuality);
		m_fLimitDist	= 300.0f;

		//	At most 2 rocket sounds can be played at the same time
		if (pConfig->bSoundMisc && m_iRocketSoundCnt < 2)
		{
			if ((m_pSound0 = g_theGame.Create3DSound(RES_S3D_ROCKETFLY)))
			{
				m_pSound0->SetMinDistance(5.0f);
				m_pSound0->SetMaxDistance(50.0f);
				m_pSound0->SetPosition(vStart);
				m_pSound0->SetVelocity(m_vVel);
				m_pSound0->UpdateChanges();
				m_pSound0->Play(true);

				m_iRocketSoundCnt++;
				m_bChangeRocketSound = true;
			}
		}

		break;

	case ID_BULLET_MIG21:

		m_iWaitTime	= 100;
		break;

	case ID_BULLET_RPG:

		m_pMoxModel		= g_theGame.LoadA3DModel(RES_MOX_ROCKETBULLET);
		m_pGFX			= g_theGame.LoadGFX(RES_GFX_ROCKETFLY0+GETMLGAMEPLAY->GetCurUserConfigs()->iGFXQuality);
		m_fLimitDist	= 300.0f;
		m_fInnerRadius	= GETGAMELOGICPARAMS->fRPGRadius;
		m_fOuterRadius	= GETGAMELOGICPARAMS->fRPGRadius;

		break;
	}

	if (m_pMoxModel)
	{
		A3DVECTOR3 vUp;
		MLA_GetViewUpAndRight(vDir, &vUp, NULL);

		if (m_pMoxModel)
		{
			m_pMoxModel->SetDirAndUp(vDir, vUp);
			m_pMoxModel->SetPos(vStart);
			m_bRender = true;
		}
	}

	if (m_pGFX)
	{
		if (m_nId == ID_BULLET_ROCKET || m_nId == ID_BULLET_FANTAN ||
			m_nId == ID_BULLET_ARMORED || m_nId == ID_BULLET_RPG)
		{
			m_pGFX->SetPos(g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_效果挂点", NULL));
			m_pGFX->Start(false);
			m_pGFX->SetDir(-vDir);
		}
	}

	return true;
}

//	Release resources
bool CMLBullet::Release()
{
	if (m_pMoxModel)
	{
		g_theGame.ReleaseA3DModel(m_pMoxModel);
		m_pMoxModel = NULL;
	}

	if (m_pSound0)
	{
		if (m_bChangeRocketSound)
		{
			if (--m_iRocketSoundCnt < 0)
				m_iRocketSoundCnt = 0;

			m_bChangeRocketSound = false;
		}

		g_theGame.Release3DSound(m_pSound0);
		m_pSound0 = NULL;
	}

	if (m_pGFX)
	{
		g_theGame.ReleaseGFX(m_pGFX);
		m_pGFX = NULL;
	}

	return true;
}

//	Logic tick
bool CMLBullet::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_bDead)
		return true;

	m_fTimeSpan = fTimeSpan;
	m_iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);
	m_dwTimeCnt = m_iTimeSpan;

	switch (m_nId)
	{
	case ID_BULLET_PISTOL:		LogicRun_Pistol();		break;
	case ID_BULLET_RIFLE:		LogicRun_Rifle();		break;
	case ID_BULLET_MORTAR:		LogicRun_Mortar();		break;
	case ID_BULLET_MACHINE:		LogicRun_Machine();		break;
	case ID_BULLET_FIREBALL:	LogicRun_Fireball();	break;
	case ID_BULLET_CANNON:		LogicRun_Cannon();		break;
	case ID_BULLET_ROCKET:
	case ID_BULLET_FANTAN:	
	case ID_BULLET_ARMORED:		LogicRun_Rocket();		break;
	case ID_BULLET_MIG21:		LogicRun_MIG21();		break;
	case ID_BULLET_RPG:			LogicRun_RPG();			break;
	}

	return true;
}

//	Pistol bullet logic run
void CMLBullet::LogicRun_Pistol()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	//	Do ray trace ...
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		PlayerGunBulletHit(Trace);
		m_bDead = true;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);
	}
}

//	Rifle bullet logic run
void CMLBullet::LogicRun_Rifle()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	//	Do ray trace ...
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		PlayerGunBulletHit(Trace);
		m_bDead = true;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);
	}
}

//	Mortar bullet logic run
void CMLBullet::LogicRun_Mortar()
{
	m_iWaitCnt += m_iTimeSpan;
	if (m_iWaitCnt < m_iWaitTime)
		return;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (!m_iState)
	{
		if (pConfig->bSoundMisc)
			g_theGame.Play3DSound(RES_S3D_MORTARWHIZZ, m_vPos);

		m_iWaitTime = 1000;
		m_iWaitCnt	= 0;
		m_iState	= 1;
		return;
	}

	//	Play exploding effect
	g_theGame.PlayGFX(RES_GFX_EXPLODEMUD, m_vPos, 2.0f);
	g_theGame.Play3DSound(RES_S3D_MORTARHIT, m_vPos);

	if (pConfig->bGraphicsMisc)
		g_theGame.PlayGFX(RES_GFX_BOMBHOLE1, m_vPos + g_vAxisY * 0.05f, 1.0f);

	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	pWorld->Explode(this, false);

	m_bDead = true;
}

//	Machine gun bullet logic run
void CMLBullet::LogicRun_Machine()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	//	Do ray trace ...
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		PlayerGunBulletHit(Trace);
		m_bDead = true;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);
	}
}

//	Fire ball logic run
void CMLBullet::LogicRun_Fireball()
{
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();

	if (m_iState == 1)	//	After explode
	{
		m_iWaitCnt += ML_ConvertTimeSpan(m_fTimeSpan);
		if (m_iWaitCnt < m_iWaitTime)
			return;

		//	Play some fire remains
		g_theGame.PlayGFX(RES_GFX_FIREREMAINS, m_vPos, 1.0f);

		if (m_dwFlag == 1)
		{
			m_pSound0 = g_theGame.Create3DSound(RES_S3D_FIREBOMBFIRE);
			g_theGame.Play3DSound(m_pSound0, m_vPos, true);
		}

		m_iState	= 2;
		m_iWaitTime	= 10000;
		m_iWaitCnt	= 0;
		return;
	}
	else if (m_iState == 2)		//	Remain firing
	{
		int iOldCnt = m_iWaitCnt;

		m_iWaitCnt += ML_ConvertTimeSpan(m_fTimeSpan);
		if (m_iWaitCnt >= m_iWaitTime)
		{
			if (m_dwFlag == 1)
			{
				g_theGame.Stop3DSound(m_pSound0);
				g_theGame.Play3DSound(RES_S3D_FIREBOMBEXTINCT, m_vPos);
			}

			m_bDead = true;
			return;
		}

		//	Do fire damage every 500 ms
		if (iOldCnt / 500 != m_iWaitCnt / 500)
			pWorld->Explode(this, true);

		if (GETMLGAMEPLAY->UpdateSound())
			g_theGame.Update3DSoundPos(m_pSound0, m_vPos);

		return;
	}

	//	Falling state
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta, v1(m_vVel.x, m_vVel.y - 10.0f * fTickTime, m_vVel.z);

	vDelta.x = m_vVel.x * fTickTime;
	vDelta.z = m_vVel.z * fTickTime;
	vDelta.y = (m_vVel.y + v1.y) * 0.5f * fTickTime;

	SetVel(v1);
	
	//	Do ray trace ...
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		pWorld->Explode(this, false);

		//	Play explosion effect
		float fScale = MLA_Random(0.6f, 1.4f);
		int iEffect = MLA_Random(0, 1);
		g_theGame.PlayGFX(RES_GFX_FIREBALL0 + iEffect, Trace.Trace.vPoint, fScale);

		if (MLA_PercentProbability(40))
			g_theGame.Play3DSound(RES_S3D_FIREBOMBEXPLODE, Trace.Trace.vPoint);

		SetPos(Trace.Trace.vPoint);

		m_iState	= 1;
		m_iWaitTime	= MLA_Random(1400, 3300);
		m_iWaitCnt	= 0;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);
	}
}

//	Cannon ball logic run
void CMLBullet::LogicRun_Cannon()
{
	m_iWaitCnt += m_iTimeSpan;
	if (m_iWaitCnt < m_iWaitTime)
		return;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (!m_iState)
	{
		if (pConfig->bSoundMisc && MLA_PercentProbability(25))
			g_theGame.Play3DSound(RES_S3D_CANNONWHIZZ1+MLA_Random(0, 2), m_vPos);

		m_iWaitCnt	= 0;
		m_iWaitTime	= 1000;
		m_iState	= 1;
		return;
	}

	//	Play exploding effect
	g_theGame.PlayGFX(RES_GFX_EXPLODEMUD, m_vPos, 2.0f);

	if (pConfig->bGraphicsMisc)
		g_theGame.PlayGFX(RES_GFX_BOMBHOLE1, m_vPos + g_vAxisY * 0.05f, 1.0f);
	
	int iSnd = RES_S3D_CANNONHIT1 + MLA_Random(0, 3);
	g_theGame.Play3DSound(iSnd, m_vPos);

	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	pWorld->Explode(this, false);

	m_bDead = true;
}

//	AI's rocket logic run
void CMLBullet::LogicRun_Rocket()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	//	Do ray trace ...
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		SetPos(Trace.Trace.vPoint);

		switch (Trace.iObjectType)
		{
		case MLTRACE_PLAYER:
		
			assert(Trace.pObject);
			((CMLPlayer*)Trace.pObject)->Strike(this);
			break;
		
		case MLTRACE_COPTER:
		
			assert(Trace.pObject);
			((CMLHelicopter*)Trace.pObject)->Strike(this, Trace.dwInfo == 1 ? true : false);
			g_theGame.PlayGFX(RES_GFX_ROCKETHIT, Trace.Trace.vPoint, 1.0f);
			break;
		
		case MLTRACE_FIXEDOBJ:
		
			assert(Trace.pObject);
			((CMLFixedObject*)Trace.pObject)->Strike(this);
			g_theGame.PlayGFX(RES_GFX_ROCKETHIT, Trace.Trace.vPoint, 1.0f);
			break;
		
		case MLTRACE_ESPMODEL:
		
			//	Play exploding effect
			g_theGame.PlayGFX(RES_GFX_EXPLODEMUD, Trace.Trace.vPoint, 1.0f);

			if (pConfig->bGraphicsMisc)
				g_theGame.PlayGFX(RES_GFX_BOMBHOLE1, Trace.Trace.vPoint + g_vAxisY * 0.05f, 1.0f);

			break;
		}

		g_theGame.Play3DSound(RES_S3D_ROCKETHIT, Trace.Trace.vPoint);
		m_bDead = true;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);

		if (pConfig->bSoundMisc && GETMLGAMEPLAY->UpdateSound())
			g_theGame.Update3DSoundPos(m_pSound0, m_vPos);

		if (m_pMoxModel)
		{
			m_pMoxModel->SetPos(m_vPos);
			m_pGFX->SetPos(g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_效果挂点", NULL));
		}
	}
}

//	MIG-21's bullet
void CMLBullet::LogicRun_MIG21()
{
	m_iWaitCnt += m_iTimeSpan;
	if (m_iWaitCnt < m_iWaitTime)
		return;

	//	Play smoke effect
	g_theGame.PlayGFX(RES_GFX_GUNHITGROUD, m_vPos, 3.0f);

	if (MLA_PercentProbability(60))
		g_theGame.Play3DSound(RES_S3D_BULLETHITMUD1+MLA_Random(0, 20), m_vPos);

	if (m_dwFlag)
	{
		//	Do damage to player
		GETMLGAMEPLAY->GetLocalPlayer()->Strike(m_iDamage);
	}

	m_bDead = true;
}

void CMLBullet::LogicRun_RPG()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;
	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	//	Do ray trace ...
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();
	
	MLRAYTRACE Trace;
	Trace.vStart = m_vPos;
	Trace.vVel	 = m_vVel;
	Trace.vDir	 = m_vMoveDir;
	Trace.fTime	 = fTickTime;

	if (pWorld->RayTrace(m_nId, &Trace))
	{
		SetPos(Trace.Trace.vPoint);

		if (Trace.iObjectType == MLTRACE_EWPLANE)
		{
			((CMLEnemyPlane*)Trace.pObject)->Strike(this);
		}
		else if (Trace.iObjectType == MLTRACE_EWVEHICLE)
		{
			((CMLEnemyVehicle*)Trace.pObject)->Strike(this);
		}
		else
			pWorld->Explode(this, false);

		if (Trace.iObjectType == MLTRACE_ESPMODEL)
		{
			g_theGame.PlayGFX(RES_GFX_EXPLODEMUD, Trace.Trace.vPoint, 1.0f);

			if (pConfig->bGraphicsMisc)
				g_theGame.PlayGFX(RES_GFX_BOMBHOLE1, Trace.Trace.vPoint + g_vAxisY * 0.05f, 1.0f);
		}
		else
		{
			//	Play effect
			g_theGame.PlayGFX(RES_GFX_ROCKETHIT, Trace.Trace.vPoint, 1.0f);
		}

		g_theGame.Play3DSound(RES_S3D_ROCKETHIT, Trace.Trace.vPoint);
		m_bDead = true;
	}
	else
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fLimitDist)
			m_bDead = true;
		
		SetPos(m_vPos + vDelta);

		if (m_pMoxModel)
		{
			m_pMoxModel->SetPos(m_vPos);
			m_pGFX->SetPos(g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_效果挂点", NULL));
		}
	}
}

//	Render routine
bool CMLBullet::Render(A3DViewport* pViewport)
{
	if (!m_pMoxModel)
		return true;

	m_pMoxModel->Render(pViewport, true, true);
	return true;
}

//	Tick animation
bool CMLBullet::TickAnimation()
{
	if (!m_pMoxModel)
		return true;

	m_pMoxModel->TickAnimation();
	return true;
}

//	Pause sound
void CMLBullet::PauseSound(bool bPause)
{
	g_theGame.Pause3DSound(m_pSound0, bPause, true);
}

//	Stop sound
void CMLBullet::StopSound()
{
	g_theGame.Stop3DSound(m_pSound0);
}

//	Player's gun bullet hit
void CMLBullet::PlayerGunBulletHit(MLRAYTRACE& Trace)
{
	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	switch (Trace.iObjectType)
	{
	case MLTRACE_AI:
	{
		((CMLAIUnit*)Trace.pObject)->Strike(this);

		//	Play blood effect
		if (pConfig->bBlood)
			g_theGame.PlayGFX(RES_GFX_HITBLOOD, Trace.Trace.vPoint, 1.5f);

		break;
	}
	case MLTRACE_EWPLANE:
	{
		((CMLEnemyPlane*)Trace.pObject)->Strike(this);

		if (pConfig->bSoundMisc)
			g_theGame.Play3DSound(RES_S3D_BULLETHITPLANE1+MLA_Random(0, 11), m_vPos, 20.0f, 100.0f, SND3DTYPE_BULLETHIT);

		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_SPARK, Trace.Trace.vPoint, 1.0f);

		break;
	}
	case MLTRACE_EWVEHICLE:
	{
		((CMLEnemyVehicle*)Trace.pObject)->Strike(this);
		
		if (pConfig->bSoundMisc)
		{
			if (((CMLUnit*)Trace.pObject)->GetId() == ID_EW_TRUCK)
				g_theGame.Play3DSound(RES_S3D_BULLETHITPLANE1+MLA_Random(0, 11), m_vPos, 20.0f, 100.0f, SND3DTYPE_BULLETHIT);
			else
				g_theGame.Play3DSound(RES_S3D_BULLETHITTANK1+MLA_Random(0, 4), m_vPos, 20.0f, 100.0f, SND3DTYPE_BULLETHIT);
		}
		
		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_SPARK, Trace.Trace.vPoint, 1.0f);

		break;
	}
	case MLTRACE_FIXEDOBJ:
	{
		((CMLFixedObject*)Trace.pObject)->Strike(this);
		break;
	}
	case MLTRACE_ESPMODEL:
	{
		//	Play dust effect
		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_GUNHITGROUD, Trace.Trace.vPoint, 0.5f);

		break;
	}
	}
}
