/*
 * FILE: ML_EnemyVehicle.cpp
 *
 * DESCRIPTION: Enemy vehicle class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_EnemyVehicle.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
#include "ML_ManBullet.h"
#include "ML_Player.h"
#include "ML_EventPt.h"
#include "ML_Route.h"

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

DWORD CMLEnemyVehicle::m_dwLastVoice = 0;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLEnemyVehicle
//
///////////////////////////////////////////////////////////////////////////

CMLEnemyVehicle::CMLEnemyVehicle() : CMLModelUnit()
{
	m_iState		= ST_NONE;
	m_iLife			= 0;
	m_bInPreserve	= false;
	m_pRunSnd		= NULL;
	m_pSmokeGFX		= NULL;
	m_bCCW			= false;
	m_bCollect		= true;
}

/*	Initialize object

	iID: enemy vehicle's ID.
	pRoute: pointer of route on which AI generate
	iWaitTime: wait time in ms.
*/
bool CMLEnemyVehicle::Init(int iID, CMLRoute* pRoute, int iWaitTime)
{
	SetId(iID);

	m_pRoute = pRoute;

	int iModel;
	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (iID == ID_EW_TRUCK)
	{
		iModel	= RES_MOX_TRUCK;
		m_iLife	= GETGAMELOGICPARAMS->iTruckLife;
		m_fMoveSpeed = GETGAMELOGICPARAMS->fTruckSpeed;
	}
	else if (iID == ID_EW_TANK)
	{
		iModel  = RES_MOX_TANK;
		m_iLife	= GETGAMELOGICPARAMS->iTankLife;
		m_fMoveSpeed = GETGAMELOGICPARAMS->fTankSpeed;

		if (pConfig->bSoundMisc)
			m_pRunSnd = g_theGame.Create3DSound(RES_S3D_TANKRUNNING, 30.0f, 120.0f);

		if (pConfig->bGraphicsMisc)
			m_pSmokeGFX	= g_theGame.LoadGFX(RES_GFX_TANKSMOKE);
	}
	else	//	ID_EW_ARMORED
	{
		iModel  = RES_MOX_ARMORED;
		m_iLife	= GETGAMELOGICPARAMS->iArmoredLife;
		m_fMoveSpeed = GETGAMELOGICPARAMS->fArmoredSpeed;

		if (pConfig->bSoundMisc)
			m_pRunSnd = g_theGame.Create3DSound(RES_S3D_ARMOREDRUNNING, 30.0f, 120.0f);
	}

	if (!(m_pMoxModel = g_theGame.LoadA3DModel(iModel)))
		return false;

	m_iDestTime		= iWaitTime;
	m_iTimeCnt		= 0;
	m_vUp1			= g_vAxisY;
	m_vUp2			= g_vAxisY;
	m_fUpFactor		= 1.0f;
	m_iCheckPosCnt	= 0;
	m_bInPreserve	= false;
	
	SetState(ST_WAIT);

	return true;
}

//	Release resources
bool CMLEnemyVehicle::Release()
{
	g_theGame.Release3DSound(m_pRunSnd);

	if (m_pSmokeGFX)
	{
		m_pSmokeGFX->Stop(false);
		g_theGame.ReleaseGFX(m_pSmokeGFX);
	}

	return CMLModelUnit::Release();
}

//	Set position
void CMLEnemyVehicle::SetPos(A3DVECTOR3& vPos)
{
	CMLModelUnit::SetPos(vPos);
	m_vxzPos = A3DVECTOR3(vPos.x, 0.0f, vPos.z);
}

//	Set position without height info.
void CMLEnemyVehicle::SetNoHeightPos(A3DVECTOR3& vPos)
{
	A3DVECTOR3 vNormal;
	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(vPos, &vNormal);
	SetPos(A3DVECTOR3(vPos.x, fHeight, vPos.z));

	if (m_pMoxModel)
	{
	//	if (DotProduct(m_vUp2, vNormal) < 0.999f)
		{
			m_vUp1 = m_pMoxModel->GetUp();
			m_vUp2 = vNormal;
			m_fUpFactor = 0.0f;
		}
	}
}

//	Go dead
void CMLEnemyVehicle::GoDead()
{
	m_bDead		= true;
	m_bDoTrace	= false;

	m_pRoute->AIDied();		//	Notify route AI's death
}

void CMLEnemyVehicle::SetState(int iState)
{
	m_iState = iState;

	switch (iState)
	{
	case ST_WAIT:

		m_bDoTrace = false;
		m_pMoxModel->SetVisibility(false);
		break;

	case ST_RUNNING:

		if (m_nId == ID_EW_TRUCK)
			PlayModelAction("[开车]", true);
		else	//	m_nId == ID_EW_TANK || m_nId == RES_MOX_ARMORED
			PlayModelAction("[炮塔居中]", true);

		g_theGame.Play3DSound(m_pRunSnd, m_vPos, true);
		break;

	case ST_FIRE:

		DoState_Fire(true);
		break;

	case ST_LEAVE:

		//	Increase escaped vehicle number
		INCSTATISTIC(m_nId, 1, 1);
		GoDead();
		
		if (m_bInPreserve)
		{
			INCSTATISTIC(m_nId, -1, 2);
			m_bInPreserve = false;
		}

		break;

	case ST_STOP:

		if (m_nId == ID_EW_TRUCK)
			PlayModelAction("[停车]", true);

		break;

	case ST_WRECK:
	{
		if (m_pMoxModel)
			g_theGame.ReleaseA3DModel(m_pMoxModel);

		int iModel = m_nId - ID_EW_TRUCK + RES_MOX_TRUCKWRECKAGE;
		if ((m_pMoxModel = g_theGame.LoadA3DModel(iModel)))
		{
			m_pMoxModel->SetVisibility(true);
			m_pMoxModel->SetPos(m_vPos);
			m_pMoxModel->SetDirAndUp(m_vMoveDir, g_vAxisY);
		}

		m_iTimeCnt	= 0;
		m_iDestTime	= 5000;
		break;
	}
	}
}

//	Logic tick
bool CMLEnemyVehicle::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_bDead || m_iState == ST_NONE)
		return true;

	m_fTimeSpan = fTimeSpan;
	m_iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);
	m_dwTimeCnt = dwTimeCnt;

	switch (m_iState)
	{
	case ST_WAIT:

		m_iTimeCnt += m_iTimeSpan;
		if (m_iTimeCnt < m_iDestTime)
			return true;

		m_bDoTrace		= true;
		m_iCurEventPt	= 0;

		BeginEventPoint(true);

		if (m_pMoxModel)
		{
			m_pMoxModel->SetVisibility(true);
			m_pMoxModel->SetDirAndUp(m_vMoveDir, g_vAxisY);
		}

		break;

	case ST_STOP:
		
		return true;

	case ST_RUNNING:
	{
		if (m_pRunSnd && GETMLGAMEPLAY->UpdateSound())
			g_theGame.Update3DSoundPos(m_pRunSnd, m_vPos);

		float fTickTime = fTimeSpan * 0.001f;
		A3DVECTOR3 vDest = m_vxzPos + m_vVel * fTickTime;
		if (ReachCurrentTarget(vDest))
		{
			g_theGame.Stop3DSound(m_pRunSnd);
			BeginEventPoint(false);
		}
		else
			SetNoHeightPos(vDest);
		
		break;
	}
	case ST_FIRE:

		if (m_pRunSnd && GETMLGAMEPLAY->UpdateSound())
			g_theGame.Update3DSoundPos(m_pRunSnd, m_vPos);

		DoState_Fire(false);
		break;

	case ST_LEAVE:

		break;

	case ST_WRECK:

		m_iTimeCnt += m_iTimeSpan;
		if (m_iTimeCnt < m_iDestTime)
			return true;

		GoDead();
		break;
	}

	if (m_iState == ST_RUNNING || m_iState == ST_FIRE || m_iState == ST_STOP)
	{
		m_iCheckPosCnt += m_iTimeSpan;
		if (m_iCheckPosCnt >= 2000)		//	Check every 2 seconds
		{
			CMLMission* pMission = GETMLGAMEPLAY->GetMission();
			if (pMission->InPreserveArea(m_vxzPos))
			{
				if (!m_bInPreserve)
				{
					pMission->IncStatistic(m_nId, 1, 2);
					m_bInPreserve = true;
				}
			}
			else if (m_bInPreserve)
			{
				pMission->IncStatistic(m_nId, -1, 2);
				m_bInPreserve = false;
			}
		}
	}

	if (m_pMoxModel)
	{
		AdjustBalance();
		m_pMoxModel->UpdateModelOBB();
	}

	return true;
}

void CMLEnemyVehicle::AdjustBalance()
{
	if (m_fUpFactor < 1.0f && m_pMoxModel)
	{
		if ((m_fUpFactor += 0.03f) > 1.0f)
			m_fUpFactor = 1.0f;

		A3DVECTOR3 vUp = Normalize(m_vUp1 * (1.0f - m_fUpFactor) + m_vUp2 * m_fUpFactor);
		A3DVECTOR3 vDir = CrossProduct(vUp, m_vMoveDir);	//	Right
		vDir = CrossProduct(vDir, vUp);

		m_pMoxModel->SetDirAndUp(vDir, vUp);
	}
}

//	Fire
void CMLEnemyVehicle::Fire()
{
	if (m_nId == ID_EW_TRUCK)
		return;

	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();

	if (m_nId == ID_EW_TANK)
	{
		if (MLA_PercentProbability(m_pRoute->GetAIMSPrecise()))
			pPlayer->Strike(GETGAMELOGICPARAMS->iTankDamage);

		A3DVECTOR3 vPos, vDir;
		vPos = g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_效果挂点", &vDir);
		g_theGame.PlayGFX(RES_GFX_TANKFIRE, vPos, 1.0f, &vDir);
		g_theGame.Play3DSound(RES_S3D_TANKFIRE1+MLA_Random(0, 1), m_vPos, 50.0f, 120.0f);
	}
	else	//	Armored car
	{
		A3DVECTOR3 vTarget;
		A3DVECTOR3 vFirePos = m_vPos + g_vAxisY * 2.0f;

		//	This rocket will hit player	
		if (MLA_PercentProbability(m_pRoute->GetAIMSPrecise()))
			vTarget = pPlayer->GetRandomHitPos();
		else	//	This rocket won't hit player
			vTarget = pPlayer->GetRandomMissPos(vFirePos, 0.4f);

		vTarget = Normalize(vTarget - vFirePos);

		//	Generate a rocket
		CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
		pBulletMan->CreateBullet(ID_BULLET_ARMORED, vFirePos, vTarget, GETGAMELOGICPARAMS->fFantanRocketSpeed);

		g_theGame.Play3DSound(RES_S3D_ROCKETFIRE, m_vPos);
	}
}

//	Rsay trace routine
bool CMLEnemyVehicle::RayTrace(MLRAYTRACE* pTrace)
{
	if (!m_bDoTrace)
		return false;

	RAYTRACE Trace;
	if (m_pMoxModel->RayTrace(pTrace->vStart, pTrace->vVel * pTrace->fTime, &Trace))
	{
		pTrace->iObjectType = MLTRACE_EWVEHICLE;
		pTrace->pObject		= (void*) this;
		
		pTrace->Trace.fFraction	= Trace.fFraction;
		pTrace->Trace.vPoint	= Trace.vPoint;

		return true;
	}

	return false;
}

/*	Strike helicopter

	pBullet: bullet object.
*/
void CMLEnemyVehicle::Strike(CMLBullet* pBullet)
{
	if (!m_bDoTrace)
		return;

	//	Pistol and rifle have no effect on vehicles
	if (pBullet->GetId() == ID_BULLET_PISTOL || 
		pBullet->GetId() == ID_BULLET_RIFLE)
		return;

	//	Machine gun only do effect on truck
	if (pBullet->GetId() == ID_BULLET_MACHINE && m_nId != ID_EW_TRUCK)
		return;

	int iDamage = pBullet->GetDamage();
	if ((m_iLife -= iDamage) > 0)
		return;

	m_bDoTrace	= false;
	m_iLife		= 0;

	if (m_bInPreserve)
	{
		INCSTATISTIC(m_nId, -1, 2);
		m_bInPreserve = false;
	}

	//	test code
/*	if (m_nId == ID_EW_TANK)
	{
	//	char sz[100];
	//	sprintf(sz, "one tank (%p) destroyed by %d, time: %d !\n", this, pBullet->GetId(), m_dwTimeCnt);
	//	OutputDebugString(sz);
		ADEBUG_TRACE3("one tank (%p) destroyed by %d, time: %d !", this, pBullet->GetId(), m_dwTimeCnt);
	}
*/	
	INCSTATISTIC(m_nId, 1, 0);

	//	Add score
	int iScore = 0;
	switch (m_nId)
	{
	case ID_EW_TRUCK:	iScore = GETGAMELOGICPARAMS->iScoreTruck;	break;
	case ID_EW_TANK:	iScore = GETGAMELOGICPARAMS->iScoreTank;	break;
	case ID_EW_ARMORED:	iScore = GETGAMELOGICPARAMS->iScoreArmored;	break;
	}

	GETMLGAMEPLAY->AddUserScore(iScore);

	g_theGame.Stop3DSound(m_pRunSnd);

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (m_pSmokeGFX)
		m_pSmokeGFX->Stop(false);

	//	Play voice
	if (m_dwTimeCnt >= m_dwLastVoice + 6000 && MLA_PercentProbability(60))
	{
		g_theGame.Play2DSound(RES_S2D_HITENEMYVEHICLE1+MLA_Random(0, 2), false);
		m_dwLastVoice = m_dwTimeCnt;
	}

	//	Play explosion effect
	if (pConfig->bGraphicsMisc)
		g_theGame.PlayGFX(RES_GFX_COPTERSHIVER, m_vPos + g_vAxisY * 1.0f, 1.0f);
	
	if (m_nId == ID_EW_TANK)
	{
		g_theGame.Play3DSound(RES_S3D_TANKEXPLODE, m_vPos);

		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_TANKEXPLODE, m_vPos, 1.0f);

	#ifdef LANGUAGE_ENGLISH
		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_TANKGUNTHROW, m_vPos + g_vAxisY * 2.0f, 1.0f);
	#endif 
	}
	else if (m_nId == ID_EW_ARMORED)
	{
		g_theGame.Play3DSound(RES_S3D_ARMOREDEXPLODE, m_vPos);

		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_TANKEXPLODE, m_vPos, 1.0f);

	#ifdef LANGUAGE_ENGLISH
		if (pConfig->bGraphicsMisc)
			g_theGame.PlayGFX(RES_GFX_ARMOREDGUNTHROW, m_vPos + g_vAxisY * 2.0f, 1.0f);
	#endif 
	}
	
	SetState(ST_WRECK);
}

//	Explode
bool CMLEnemyVehicle::Explode(CMLBullet* pBullet, bool bFire)
{
	if (!m_bDoTrace || !m_pMoxModel)
		return false;

/*	A3DOBB obb = m_pMoxModel->GetModelOBB();
	float fRadius = pBullet->GetOuterRadius();

	A3DVECTOR3 vDelta = m_vPos - pBullet->GetPos();
	float fDelta = fabs(DotProduct(vDelta, obb.XAxis));
	if (fDelta > obb.Extents.x + fRadius)
		return false;

	fDelta = fabs(DotProduct(vDelta, obb.YAxis));
	if (fDelta > obb.Extents.y + fRadius)
		return false;

	fDelta = fabs(DotProduct(vDelta, obb.ZAxis));
	if (fDelta > obb.Extents.z + fRadius)
		return false;
*/
	A3DAABB aabb = m_pMoxModel->GetModelAABB();
	float fRadius = pBullet->GetOuterRadius();

	A3DVECTOR3 vDelta = m_vPos - pBullet->GetPos();
	if (fabs(vDelta.x) > aabb.Extents.x + fRadius)
		return false;

	if (fabs(vDelta.y) > aabb.Extents.y + fRadius)
		return false;

	if (fabs(vDelta.z) > aabb.Extents.z + fRadius)
		return false;

	Strike(pBullet);

	return true;
}

//	Reach event point and begin do events
void CMLEnemyVehicle::BeginEventPoint(bool bNewPos)
{
	EVENTINTERATOR EtInterator;	//	Event interator

	//	Get the first event point of route
	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);

	//	Initiailize event interator
	CMLEventPt::InitEventInterator(&EtInterator);
	pEventPt->GetNextEvent(&EtInterator);

	if (bNewPos)
	{
		m_vxzPos = pEventPt->GetRandomPos();
		SetNoHeightPos(m_vxzPos);
	}

	if (EtInterator.Event.iEvent == EPE_AIV_BEGINFIRE)
	{
		if (m_nId == ID_EW_TANK || m_nId == ID_EW_ARMORED)
		{
			SetState(ST_FIRE);
			return;
		}
	}
	else if (EtInterator.Event.iEvent == EPE_AIV_LEAVE)
	{
		SetState(ST_LEAVE);
		return;
	}

	//	Move to next event point
	if (++m_iCurEventPt >= m_pRoute->GetEventPtNum())
	{
		//	Stop
		SetState(ST_STOP);
		return;
	}
	
	pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	
	m_vCurTarget = pEventPt->GetRandomPos();
	m_vMoveDir	 = Normalize(m_vCurTarget - m_vxzPos);
	
	SetVel(m_vMoveDir * m_fMoveSpeed);
	
	SetState(ST_RUNNING);
}

//	Reach current position ?
bool CMLEnemyVehicle::ReachCurrentTarget(A3DVECTOR3& vDestPos)
{
	float fCurDelta, fDelta;

	fCurDelta = DotProduct(vDestPos - m_vxzPos, m_vMoveDir);
	fDelta	  = DotProduct(m_vCurTarget - m_vxzPos, m_vMoveDir);

	if (fCurDelta >= fDelta)
		return true;

	return false;
}

//	Do state: fire
bool CMLEnemyVehicle::DoState_Fire(bool bNew)
{
	if (bNew)
	{
		//	Rotate cannon, aim player
		Aim();

		m_iSubState = 1;
		m_iDestTime	= m_pRoute->GetAIMSInterval(false); 
		m_iTimeCnt	= 0;
		m_fDistCnt	= 0.0f;
		m_fDistance	= MLA_Random(12.0f, 18.0f);
		return true;
	}

	switch (m_iSubState)
	{
	case 0:		//	Wait to fire
	{
		m_iTimeCnt += m_iTimeSpan;
		if (m_iTimeCnt >= m_iDestTime)
		{
			//	Fire !
			Fire();
			
			m_iTimeCnt = 0;

			if (MLA_PercentProbability(60))
			{
				//	Start moving
				m_fDistCnt	= 0.0f;
				m_fDistance	= MLA_Random(8.0f, 14.0f);
				m_iSubState = 1;

				if (m_nId == ID_EW_TANK)
				{
					g_theGame.Play3DSound(RES_S3D_TANKSTARTUP, m_vPos);

					if (m_pSmokeGFX)
					{
						m_pSmokeGFX->SetPos(g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_尾烟挂点", NULL));
						m_pSmokeGFX->Start(false);
					}
				}
				else if (m_nId == ID_EW_ARMORED)
				{
					g_theGame.Play3DSound(m_pRunSnd, m_vPos, true);
				}
			}
		}

		break;
	}
	case 1:		//	Move
	{
		m_iTimeCnt += m_iTimeSpan;

		A3DVECTOR3 vRight, vToPlayer = GETMAPDATA->GetActorPos();
		vToPlayer.y = 0.0f;
		vToPlayer = Normalize(vToPlayer - m_vxzPos);
		vRight	  = CrossProduct(g_vAxisY, vToPlayer);

		m_vMoveDir = m_bCCW ? vRight : -vRight;

		float fLen = (float)(m_fTimeSpan * 0.001 * m_fMoveSpeed);
		SetNoHeightPos(m_vxzPos + m_vMoveDir * fLen);
		SetVel(m_vMoveDir * m_fMoveSpeed);

		if (m_pSmokeGFX)
			m_pSmokeGFX->SetPos(g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_尾烟挂点", NULL));

		m_fDistCnt += fLen;
		if (m_fDistCnt >= m_fDistance)
		{
			g_theGame.Stop3DSound(m_pRunSnd);
			m_iSubState = 0;
		}
		
		break;
	}
	}

	return true;
}

//	Aim
void CMLEnemyVehicle::Aim()
{
	if (m_nId == ID_EW_TRUCK || !m_pMoxModel)
		return;
	
	A3DVECTOR3 vRight, vToPlayer, vPlayerPos = GETMAPDATA->GetActorPos();
	vPlayerPos.y = 0.0f;

	vToPlayer = Normalize(vPlayerPos - m_vxzPos);
	vRight = CrossProduct(g_vAxisY, vToPlayer);

	if (DotProduct(m_vMoveDir, vRight) >= 0.0f)
	{
		m_bCCW = true;
		PlayModelAction("[炮塔左转]", true);
	}
	else
	{
		m_bCCW = false;
		PlayModelAction("[炮塔右转]", true);
	}
}

//	Pause sound
void CMLEnemyVehicle::PauseSound(bool bPause)
{
	CMLModelUnit::PauseSound(bPause);
	g_theGame.Pause3DSound(m_pRunSnd, bPause, true);
}

//	Stop sound
void CMLEnemyVehicle::StopSound()
{
	CMLModelUnit::StopSound();
	g_theGame.Stop3DSound(m_pRunSnd);
}
