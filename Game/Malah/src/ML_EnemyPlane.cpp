/*
 * FILE: ML_EnemyPlane.cpp
 *
 * DESCRIPTION: Enemy plane class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_EnemyPlane.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
#include "ML_ManBullet.h"
#include "ML_Player.h"

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
//	Implement CMLEnemyPlane
//
///////////////////////////////////////////////////////////////////////////

CMLEnemyPlane::CMLEnemyPlane() : CMLModelUnit()
{
	m_iState	= ST_NONE;
	m_iLife		= 0;
	m_bCollect	= true;
}

/*	Initialize object

	iID: enemy plane's ID.
	vMoveDir: plane's move direction on xz plane
*/
bool CMLEnemyPlane::Init(int iID, A3DVECTOR3& vMoveDir)
{
	SetId(iID);

	A3DVECTOR3 vxzPlayer = GETMAPDATA->GetActorPos();
//	A3DVECTOR3 vxzPlayer = GETMAPDATA->GetActorPos() + A3DVECTOR3(50.0f, 0.0f, 0.0f);
	vxzPlayer.y = 0.0f;

	m_vAttackDir = vMoveDir;

	A3DVECTOR3 vPos, vDest;
	vPos = vxzPlayer - vMoveDir * 1000.0f;
	
	if (iID == ID_EW_MIG21)		//	MIG-21
	{
		vPos  = vPos + g_vAxisY * GETGAMELOGICPARAMS->fMIG21MaxHeight;
		vDest = vxzPlayer - m_vAttackDir * 20.0f + g_vAxisY * GETGAMELOGICPARAMS->fMIG21MinHeight;

		m_fSpeed	 = GETGAMELOGICPARAMS->fMIG21Speed;
		m_iLife		 = GETGAMELOGICPARAMS->iMIG21Life;
		m_vFireStart = vxzPlayer - m_vAttackDir * (GETGAMELOGICPARAMS->fMIG21FireLen * 0.5f);

		//	Load models
		if (!(m_pMoxModel = g_theGame.LoadA3DModel(RES_MOX_MIG21)))
			return false;
	}
	else	//	FANTAN
	{
		vPos  = vPos + g_vAxisY * GETGAMELOGICPARAMS->fFantanMaxHeight;
		vDest = vxzPlayer + m_vAttackDir * 20.0f + g_vAxisY * GETGAMELOGICPARAMS->fFantanMinHeight;

		m_fSpeed	 = GETGAMELOGICPARAMS->fFantanSpeed;
		m_iLife		 = GETGAMELOGICPARAMS->iFantanLife;
		m_vFireStart = GETMAPDATA->GetActorPos();

		//	Load models
		if (!(m_pMoxModel = g_theGame.LoadA3DModel(RES_MOX_FANTAN)))
			return false;
	}

	m_fDistance		= Normalize(vDest - vPos, m_vCurMoveDir);
	m_fDistCnt		= 0.0f;
	m_vAttackRight	= CrossProduct(g_vAxisY, m_vAttackDir);

	float fCos = DotProduct(m_vCurMoveDir, m_vAttackDir);

	m_fPitchDest	= (float)acos(fCos);
	m_fPitchCnt		= m_fPitchDest;
	m_fPitchDelta	= 0.0f;

	m_bFire			= false;
	m_iFireNumCnt	= 0;

	if (m_pMoxModel)
	{
		m_pMoxModel->SetDirAndUp(m_vAttackDir, g_vAxisY);
		m_pMoxModel->RotateXRelative(m_fPitchCnt);
		m_pMoxModel->PlayActionByName("[Í£Ö¹]", true);
		m_pMoxModel->SetVelocity(m_vCurMoveDir * m_fSpeed);
	}

	//	Set plane's position and velocity
	SetPos(vPos);
	SetVel(m_vCurMoveDir * m_fSpeed);

	SetState(ST_DIVE);

	return true;
}

//	Release resources
bool CMLEnemyPlane::Release()
{
	return CMLModelUnit::Release();
}

//	Set position
void CMLEnemyPlane::SetPos(A3DVECTOR3& vPos)
{
	CMLModelUnit::SetPos(vPos);
	m_vxzPos = A3DVECTOR3(vPos.x, 0.0f, vPos.z);
}

void CMLEnemyPlane::SetState(int iState)
{
	m_iState = iState;

	if (m_nId == ID_EW_MIG21)
	{
		switch (iState)
		{
		case ST_DIVE:
			
			m_pMoxModel->PlayActionByName("[·ÉÐÐ]", true);
			break;
			
		case ST_UP:
		{
			float fRadian	= DEG2RAD(35.0f);
			m_fPitchDest	= -fRadian;
			m_fPitchDelta	= (m_fPitchDest - m_fPitchCnt) / 1.0f;
			
			float fDelta = GETGAMELOGICPARAMS->fMIG21MaxHeight - GETGAMELOGICPARAMS->fMIG21MinHeight;
			fDelta /= (float)sin(fRadian);
			
			m_fDistance	= fDelta;
			m_fDistCnt	= 0.0f;
			break;
		}
		case ST_LEAVE:
			
			m_fPitchDest	= 0.0f;
			m_fPitchDelta	= (m_fPitchDest - m_fPitchCnt) / 3.0f;
			
			m_fDistance	= 800.0f;
			m_fDistCnt	= 0.0f;
			break;
		}
	}
	else	//	FANTAN
	{
		switch (iState)
		{
		case ST_DIVE:
		
			m_pMoxModel->PlayActionByName("[·ÉÐÐ]", true);
			break;
			
		case ST_UP:
		{
			float fRadian	= DEG2RAD(35.0f);
			m_fPitchDest	= -fRadian;
			m_fPitchDelta	= (m_fPitchDest - m_fPitchCnt) / 1.0f;
			
			float fDelta = GETGAMELOGICPARAMS->fFantanMaxHeight - GETGAMELOGICPARAMS->fFantanMinHeight;
			fDelta /= (float)sin(fRadian);
			
			m_fDistance	= fDelta;
			m_fDistCnt	= 0.0f;
			break;
		}
		case ST_LEAVE:
			
			m_fPitchDest	= 0.0f;
			m_fPitchDelta	= (m_fPitchDest - m_fPitchCnt) / 3.0f;
			
			m_fDistance	= 800.0f;
			m_fDistCnt	= 0.0f;
			break;
		}
	}
}

//	Adjust model pitch and
void CMLEnemyPlane::AdjustModelPitch(float fTickTime)
{
	if (!m_fPitchDelta)
		return;

	float fPitchDelta = m_fPitchDelta * fTickTime;
	float fNextCnt = m_fPitchCnt + fPitchDelta;

	if (m_fPitchDelta > 0.0f)
	{
		if (m_fPitchCnt < m_fPitchDest && fNextCnt >= m_fPitchDest)
		{
			m_fPitchCnt		= m_fPitchDest;
			m_fPitchDelta	= 0.0f;
		}
		else
			m_fPitchCnt = fNextCnt;
	}
	else
	{
		if (m_fPitchCnt > m_fPitchDest && fNextCnt <= m_fPitchDest)
		{
			m_fPitchCnt		= m_fPitchDest;
			m_fPitchDelta	= 0.0f;
		}
		else
			m_fPitchCnt = fNextCnt;
	}
	
	A3DMATRIX4 mat = RotateAxis(m_vAttackRight, m_fPitchCnt);
	m_vCurMoveDir = Normalize(m_vAttackDir * mat);

	if (m_pMoxModel)
	{
	//	m_pMoxModel->RotateXRelative(fPitchDelta);
		A3DVECTOR3 vUp = g_vAxisY * mat;
		m_pMoxModel->SetDirAndUp(m_vCurMoveDir, vUp);
	}
}

//	Logic tick
bool CMLEnemyPlane::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_bDead || m_iState == ST_NONE)
		return true;

	m_fTimeSpan = fTimeSpan;
	m_iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);
	m_dwTimeCnt = m_iTimeSpan;

	switch (m_nId)
	{
	case ID_EW_MIG21:	LogicRun_MIG21();		break;
	case ID_EW_FANTAN:	LogicRun_fantan();		break;
	}

	if (m_pMoxModel)
		m_pMoxModel->UpdateModelOBB();

	return true;
}

//	Logic run function for MIG-21
void CMLEnemyPlane::LogicRun_MIG21()
{
	float fTickTime = m_fTimeSpan * 0.001f;

	switch (m_iState)
	{
	case ST_DIVE:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			SetState(ST_UP);
			return;
		}

		SetPos(m_vPos + m_vVel * fTickTime);

		//	It's time to fire ?
		A3DVECTOR3 vPlayer = GETMAPDATA->GetActorPos();
		if (!m_bFire && Magnitude(m_vPos - vPlayer) < GETGAMELOGICPARAMS->fMIG21FireDist)
		{
			m_bFire			= true;
			m_iFireInter	= GETGAMELOGICPARAMS->iMIG21FireInterval;
			m_iFireNumber	= GETGAMELOGICPARAMS->iMIG21BulletNum;
			m_iFireTime		= 0;
		}

		break;
	}
	case ST_UP:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			SetState(ST_LEAVE);
			return;
		}

		AdjustModelPitch(fTickTime);

		SetVel(m_vCurMoveDir * m_fSpeed);
		SetPos(m_vPos + m_vVel * fTickTime);

		break;
	}
	case ST_LEAVE:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			m_bDead = true;
			return;
		}

		AdjustModelPitch(fTickTime);

		SetVel(m_vCurMoveDir * m_fSpeed);
		SetPos(m_vPos + m_vVel * fTickTime);

		break;
	}
	}

	if (m_bFire && m_iFireNumCnt < m_iFireNumber)
	{
		m_iFireTime += m_iTimeSpan;
		while (m_iFireTime >= m_iFireInter)
		{
			m_iFireTime -= m_iFireInter;

			//	Create a bullet
			CreateBullet(m_iFireNumCnt++);
			g_theGame.Play3DSound(RES_S3D_MIG21FIRE, m_vPos);

			if (m_iFireNumCnt >= m_iFireNumber)
			{
				//	Stop fire effect
				if (m_pMoxModel)
					m_pMoxModel->StopAllGFX();
				
				break;
			}
		}
	}
}

//	Logic run function for Fantan
void CMLEnemyPlane::LogicRun_fantan()
{
	float fTickTime = m_fTimeSpan * 0.001f;

	switch (m_iState)
	{
	case ST_DIVE:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			SetState(ST_UP);
			return;
		}

		SetPos(m_vPos + m_vVel * fTickTime);

		//	It's time to fire ?
		A3DVECTOR3 vPlayer = GETMAPDATA->GetActorPos();
		if (!m_bFire && Magnitude(m_vPos - vPlayer) < GETGAMELOGICPARAMS->fFantanFireDist)
		{
			m_bFire			= true;
			m_iFireInter	= GETGAMELOGICPARAMS->iFantanFireInterval;
			m_iFireNumber	= GETGAMELOGICPARAMS->iFantanRocketNum;
			m_iFireTime		= 0;
		}

		break;
	}
	case ST_UP:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			SetState(ST_LEAVE);
			return;
		}

		AdjustModelPitch(fTickTime);

		SetVel(m_vCurMoveDir * m_fSpeed);
		SetPos(m_vPos + m_vVel * fTickTime);

		break;
	}
	case ST_LEAVE:
	{
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			m_bDead = true;
			return;
		}

		AdjustModelPitch(fTickTime);

		SetVel(m_vCurMoveDir * m_fSpeed);
		SetPos(m_vPos + m_vVel * fTickTime);

		break;
	}
	}

	if (m_bFire && m_iFireNumCnt < m_iFireNumber)
	{
		m_iFireTime += m_iTimeSpan;
		while (m_iFireTime >= m_iFireInter)
		{
			m_iFireTime -= m_iFireInter;

			//	Create a bullet
			CreateBullet(m_iFireNumCnt++);
			g_theGame.Play3DSound(RES_S3D_FANTANFIRE, m_vPos);

			if (m_iFireNumCnt >= m_iFireNumber)
				break;
		}
	}
}

//	Create a bullet
void CMLEnemyPlane::CreateBullet(int iCount)
{
	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();

	if (m_nId == ID_EW_MIG21)	//	MIG-21
	{
		float fDist = GETGAMELOGICPARAMS->fMIG21FireLen / m_iFireNumber;
		A3DVECTOR3 vPos = m_vFireStart + m_vAttackDir * (fDist * iCount);
		vPos.y = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(vPos);

		CMLBullet* pBullet = pBulletMan->CreateBullet(ID_BULLET_MIG21, vPos, g_vOrigin, 0.0f);

		if (iCount == m_iFireNumber / 2)
			pBullet->SetFlag(1);	//	This bullet will do damage
	}
	else	//	FANTAN
	{
		A3DVECTOR3 vTarget;
		CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();

		if (MLA_PercentProbability(GETGAMELOGICPARAMS->iFantanPrecise))
			vTarget = pPlayer->GetRandomHitPos();
		else
		{
			float fRadius = pPlayer->GetBoundingRadius();
			vTarget = MLA_GetRandomPosInDisk(m_vFireStart, g_vAxisY, g_vAxisX, 
											 fRadius + 10.0f, fRadius + 1.0f);
		}

		vTarget = Normalize(vTarget - m_vPos);

		//	Generate a rocket
		CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
		pBulletMan->CreateBullet(ID_BULLET_FANTAN, m_vPos, vTarget, GETGAMELOGICPARAMS->fFantanRocketSpeed);
	}
}

//	Rsay trace routine
bool CMLEnemyPlane::RayTrace(MLRAYTRACE* pTrace)
{
	if (!m_bDoTrace)
		return false;

	RAYTRACE Trace;
	if (m_pMoxModel->RayTrace(pTrace->vStart, pTrace->vVel * pTrace->fTime, &Trace))
	{
		pTrace->iObjectType = MLTRACE_EWPLANE;
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
void CMLEnemyPlane::Strike(CMLBullet* pBullet)
{
	if (!m_bDoTrace)
		return;

	//	Pistol has no effect on plane.
	if (pBullet->GetId() == ID_BULLET_PISTOL)
		return;

	int iDamage = pBullet->GetDamage();
	if ((m_iLife -= iDamage) > 0)
		return;

	m_bDoTrace	= false;
	m_bDead		= true;
	m_iLife		= 0;

	m_pMoxModel->SetVisibility(false);

	INCSTATISTIC(m_nId, 1, 0);

	g_theGame.Play3DSound(RES_S3D_COPTEREXPLODE1+MLA_Random(0, 2), m_vPos, 40, 100);

	if (MLA_PercentProbability(60))
		g_theGame.Play2DSound(RES_S2D_HITENEMYPLANE1+MLA_Random(0, 2), false);

	//	Add score
	if (m_nId == ID_EW_MIG21)
	{
		g_theGame.PlayGFX(RES_GFX_MIG21EXPLODE, m_vPos, 2.0f);
		GETMLGAMEPLAY->AddUserScore(GETGAMELOGICPARAMS->iScoreMIG21);
	}
	else if (m_nId == ID_EW_FANTAN)
	{
		g_theGame.PlayGFX(RES_GFX_FANTANEXPLODE, m_vPos, 2.0f);
		GETMLGAMEPLAY->AddUserScore(GETGAMELOGICPARAMS->iScoreFantan);
	}
}


