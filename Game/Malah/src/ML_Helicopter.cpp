/*
 * FILE: ML_Helicopter.cpp
 *
 * DESCRIPTION: Helicopter class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Helicopter.h"
#include "ML_ID.h"
#include "ML_Resources.h"
#include "ML_Route.h"
#include "ML_GamePlay.h"
#include "ML_Player.h"
#include "ML_Bullet.h"

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
//	Implement CMLHelicopter
//
///////////////////////////////////////////////////////////////////////////

//	Actions
const char* CMLHelicopter::m_aActions[ACT_NUM] = 
{
	"[∑…––]", "[∑…––µΩ–¸Õ£]", "[–¸Õ£]", "[–¸Õ£µΩ∑…––]", "[ª˜÷–]", "[≤•∑≈∂Øª≠]"
};

CMLHelicopter::CMLHelicopter() : CMLModelUnit()
{
	m_nId = ID_SUPP_COPTER;

	m_pRoute	= NULL;
	m_iState	= ST_NONE;
	m_pFlySnd	= NULL;
}

//	Initialize object
bool CMLHelicopter::Init(CMLRoute* pRoute)
{
	m_pRoute = pRoute;

	//	Load helicopter models
	m_pMoxModel	= g_theGame.LoadA3DModel(RES_MOX_COPTER);
	m_pFlySnd	= g_theGame.Create3DSound(RES_S3D_COPTERFLY);

	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	m_iLife	= GETPLAYERILDVALUE(ILD_LIFE_HELICOPTER, iLevel);

	m_iState		= ST_WAIT;
	m_iCurEventPt	= 0;
	m_fDistance		= GETGAMELOGICPARAMS->iCopterArriveTime * 1000.0f;
	m_fDistCnt		= 0.0f;
	m_bRender		= false;

	return true;
}

//	Release resources
bool CMLHelicopter::Release()
{
	g_theGame.Release3DSound(m_pFlySnd);

	m_pFlySnd	= NULL;
	m_pRoute	= NULL;
	m_iState	= ST_NONE;

	return CMLModelUnit::Release();
}

//	Get current model
void CMLHelicopter::ChangeModel()
{
	A3DModel* pModel = NULL;
	
	switch (m_iState)
	{
	case ST_NORMALFLY:	
	case ST_ENTER:				
	case ST_SUPPLY:		
	case ST_LEAVE:
	case ST_CIRCLEFLY:
	case ST_DISAPPEAR:
	case ST_APPEAR:

		pModel = g_theGame.LoadA3DModel(RES_MOX_COPTER);
		break;

	case ST_CRASHING:

		pModel = g_theGame.LoadA3DModel(RES_MOX_COPTERCRASH);
		break;

	case ST_WRECKAGE:

		pModel = g_theGame.LoadA3DModel(RES_MOX_COPTERWRECK);
		break;
	}

	//	Chnage model
	if (pModel)
	{
		if (m_pMoxModel)
			pModel->SetDirAndUp(m_pMoxModel->GetDir(), m_pMoxModel->GetUp());

		pModel->SetPos(m_vPos);
	}

	//	Release old model
	if (m_pMoxModel)
		g_theGame.ReleaseA3DModel(m_pMoxModel);

	m_pMoxModel = pModel;
}

//	Be flying ?
bool CMLHelicopter::BeFlying()
{
	bool bRet = false;

	switch (m_iState)
	{
	case ST_NORMALFLY:	
	case ST_ENTER:				
	case ST_SUPPLY:		
	case ST_LEAVE:
	case ST_CIRCLEFLY:
	case ST_DISAPPEAR:
	case ST_APPEAR:

		bRet = true;
		break;
	}

	return bRet;
}

//	Set position
void CMLHelicopter::SetPos(A3DVECTOR3& vPos)
{
	CMLModelUnit::SetPos(vPos);
	m_vxzPos = A3DVECTOR3(vPos.x, 0.0f, vPos.z);
}

//	Logic tick
bool CMLHelicopter::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_iState == ST_NONE)
		return true;

	m_fTimeSpan	= fTimeSpan;
	m_dwTimeCnt = dwTimeCnt;
	m_iTimeSpan	= ML_ConvertTimeSpan(fTimeSpan);

	switch (m_iState)
	{
	case ST_WAIT:
	{
		m_fDistCnt += m_fTimeSpan;
		if (m_fDistCnt >= m_fDistance)
		{
			m_bRender = true;
			BeginEventPoint();

			g_theGame.Play3DSound(m_pFlySnd, m_vPos, true);
		}

		break;
	}
	case ST_NORMALFLY:	LogicRun_NormalFly();	break;
	case ST_SUPPLY:		LogicRun_Supply();		break;

	case ST_ENTER:
	case ST_LEAVE:
	case ST_CIRCLEFLY:	LogicRun_CircleFly();	break;

	case ST_APPEAR:		LogicRun_Appear();		break;
	case ST_DISAPPEAR:	LogicRun_Disappear();	break;
	case ST_CRASHING:	LogicRun_Crash();		break;
	case ST_WRECKAGE:	LogicRun_Wreck();		break;
	}

	if (m_pMoxModel)
		m_pMoxModel->UpdateModelOBB();

	if (m_pFlySnd && m_iState != ST_WAIT && m_iState != ST_WRECKAGE)
	{
		if (GETMLGAMEPLAY->UpdateSound())
			g_theGame.Update3DSoundPos(m_pFlySnd, m_vPos);
	}

	return true;
}

//	Logic run in ST_NORMALFLY
void CMLHelicopter::LogicRun_NormalFly()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	m_fDistCnt += m_fSpeed * fTickTime;
	if (m_fDistCnt >= m_fDistance)
		BeginEventPoint();
	else	
		SetPos(m_vPos + vDelta);
}

//	Logic run when circle flying
void CMLHelicopter::LogicRun_CircleFly()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	m_fDistCnt += fTickTime;
	if (m_fDistCnt >= m_fDistance)
	{
		BeginEventPoint();
		return;
	}

	A3DVECTOR3 vPos = m_vPos + m_vVel * fTickTime;
	vPos.y += m_fHeightDelta * fTickTime;
	SetPos(vPos);

	//	Adjust model's yaw and roll
	AdjustModelYawAndRoll(fTickTime);

	//	Set new velocity
	float fSpeed = m_fSpeed + m_fAccelerator * fTickTime;
	SetVel(m_vCurMoveDir * fSpeed);
}

//	Logic run in ST_SUPPLY
void CMLHelicopter::LogicRun_Supply()
{
	float fTickTime = m_fTimeSpan * 0.001f;

	switch (m_iSubState)
	{
	case SUBST_ADJUST:
	{
		m_fDistCnt += fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
	//		SetVel(g_vAxisY * -5.0f);
	//		m_iSubState = SUBST_DOWN;
	//		m_fDistance	= m_vPos.y - GETGAMELOGICPARAMS->fCopterSupplyHeight;
	//		m_fDistCnt	= 0.0f;

			//	Use m_fDistance and m_fDistCnt as time
			m_fDistance	= GETGAMELOGICPARAMS->iCopterHangTime;
			m_fDistCnt	= 0.0f;
			m_iSubState = SUBST_SUPPLY;

			return;
		}

		//	Adjust model's yaw and roll
		AdjustModelYawAndRoll(fTickTime);
		break;
	}
	case SUBST_DOWN:
	{
		A3DVECTOR3 vDelta = m_vVel * fTickTime;
		
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
		//	if (m_pMoxModel)
		//		m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_HANG], true);

			//	Use m_fDistance and m_fDistCnt as time
			m_fDistance	= GETGAMELOGICPARAMS->iCopterHangTime;
			m_fDistCnt	= 0.0f;
			m_iSubState = SUBST_SUPPLY;
		}
		else	
			SetPos(m_vPos + vDelta);
			
		break;
	}
	case SUBST_SUPPLY:
	{
		m_fDistCnt += fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			DoSupply();		//	Do supply

			if (m_pMoxModel)
				m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_HANGTOFLY], true);

			//	Finish supply
		//	SetVel(g_vAxisY * 5.0f);
		//	m_iSubState = SUBST_UP;
		//	m_fDistance	= GETGAMELOGICPARAMS->fCopterFlyHeight - m_vPos.y;
		//	m_fDistCnt	= 0.0f;

			//	Wait 2 second so that action ACT_HANGTOFLY can be fully seen
			m_iSubState = SUBST_UP;
			m_fDistance	= 3.0f;
			m_fDistCnt	= 0.0f;
		}

		break;
	}
	case SUBST_UP:
	{
/*		A3DVECTOR3 vDelta = m_vVel * fTickTime;
		
		m_fDistCnt += m_fSpeed * fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			if (m_pMoxModel)
				m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_FLY], true);

			SetVel(m_vCurMoveDir * 0.1f);
			LeaveSupply();
		}
		else
			SetPos(m_vPos + vDelta);
*/
		m_fDistCnt += fTickTime;
		if (m_fDistCnt >= m_fDistance)
		{
			SetVel(m_vCurMoveDir * 0.1f);
			LeaveSupply();
		}

		break;
	}
	}
}

//	Logic run when disappear
void CMLHelicopter::LogicRun_Disappear()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	m_fDistCnt += m_fSpeed * fTickTime;
	if (m_fDistCnt >= m_fDistance)
		m_bDead = true;
	else	
		SetPos(m_vPos + vDelta);

	AdjustModelYawAndRoll(fTickTime);
}

//	Logic run when appear
void CMLHelicopter::LogicRun_Appear()
{
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;

	m_fDistCnt += m_fSpeed * fTickTime;
	if (m_fDistCnt >= m_fDistance)
	{
		CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
		BeginNormalFly(m_vxzPos, pEventPt->GetMLEventPtData()->vPos, false);
	}
	else	
		SetPos(m_vPos + vDelta);
}

//	Logic run in ST_CRASHING
void CMLHelicopter::LogicRun_Crash()
{
	if (m_vVel.y > -0.1f)	//	This shouldn't happen
		m_vVel.y = -GETGAMELOGICPARAMS->fCopterDropSpeed;

	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta = m_vVel * fTickTime;
	SetPos(m_vPos + vDelta);

	float fHSpeed = MagnitudeH(m_vVel) + m_fAccelerator * fTickTime;
	if (fHSpeed < m_fMinHSpeed)
		fHSpeed = m_fMinHSpeed;

	if (fHSpeed > 0.5f)
	{
		float v	 = m_vVel.y;
		m_vVel.y = 0.0f;
		m_vVel	 = Normalize(m_vVel) * fHSpeed;
		m_vVel.y = v;

		SetVel(m_vVel);
	}

	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(m_vPos);
	if (m_vPos.y < fHeight - 1.0f)
	{
		m_iState	= ST_WRECKAGE;
		m_fDistance	= 12000.0f;
		m_fDistCnt	= 0.0f;

		g_theGame.Stop3DSound(m_pFlySnd);

		//	Chnage model
		ChangeModel();

		if (m_pMoxModel)
			m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_WRECK], true);

		//	Play some shiver effect
		g_theGame.PlayGFX(RES_GFX_COPTERSHIVER, m_vPos + g_vAxisY, 2.0f);
		g_theGame.Play3DSound(RES_S3D_COPTERCRASH1+MLA_Random(0, 4), m_vPos, 40, 100);
	}
}

//	Logic run in ST_WRECKAGE
void CMLHelicopter::LogicRun_Wreck()
{
	m_fDistCnt += m_fTimeSpan;
	if (m_fDistCnt >= m_fDistance)
	{
		m_bDead = true;
	}
}

/*	Adjust yaw and roll

	fTickTime: tick time in second
*/
void CMLHelicopter::AdjustModelYawAndRoll(float fTickTime)
{
	float fYawDelta = m_fYawDelta * fTickTime;
	A3DMATRIX4 mat = RotateY(fYawDelta);
	m_vCurMoveDir = Normalize(m_vCurMoveDir) * mat;

	if (!m_pMoxModel)
		return;

	m_fRollCnt += m_fRollDelta * fTickTime;
	if (m_fRollDelta >= 0.0f)
	{
		if (m_fRollCnt > m_fRollRadian)
			m_fRollCnt = m_fRollRadian;
	}
	else if (m_fRollCnt < m_fRollRadian)
		m_fRollCnt = m_fRollRadian;
	
	mat = RotateAxis(m_vCurMoveDir, m_fRollCnt);
	A3DVECTOR3 vUp = g_vAxisY * mat;

	m_pMoxModel->SetDirAndUp(m_vCurMoveDir, vUp);
}

//	Begin event point
void CMLHelicopter::BeginEventPoint()
{
	if (m_iCurEventPt >= m_pRoute->GetEventPtNum())
		return;

	//	Get the first event point of route
	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);

	//	Initiailize event interator
	CMLEventPt::InitEventInterator(&m_EtInterator);
	pEventPt->GetNextEvent(&m_EtInterator);

	if (m_EtInterator.Event.iEvent == EPE_CT_LEAVEBATTLE)
	{
		m_iState		= ST_DISAPPEAR;
		m_bDoTrace		= false;	//	Disable trace
		m_fDistance		= 300.0f;
		m_fDistCnt		= 0.0f;

		m_fRollCnt		= m_fRollRadian;
		m_fRollDelta	= -m_fRollRadian / 3.0f;
		m_fRollRadian	= 0.0f;
		m_fYawDelta		= 0.0f;

		m_vCurMoveDir = Normalize(A3DVECTOR3(m_vCurMoveDir.x, -0.2f, m_vCurMoveDir.z));
		SetVel(m_vCurMoveDir * m_fSpeed);

		return;
	}
	else if (m_EtInterator.Event.iEvent == EPE_CT_SUPPLY)
	{
		if (m_iCurEventPt == 1)		//	Cannot not be the event point
			m_iState = ST_NONE;
		else
			BeginSupply();

		return;
	}

	//	For helicopter, we only handle the first event of each event point
	m_iCurEventPt++;

	//	Get next event point's position
	if (m_iCurEventPt >= m_pRoute->GetEventPtNum())
	{
		m_iState = ST_NONE;
		return;
	}

	CMLEventPt* pNextEventPt = m_pRoute->GetEventPt(m_iCurEventPt);

	A3DVECTOR3 vCurPos	= pEventPt->GetMLEventPtData()->vPos;
	A3DVECTOR3 vNextPos	= pNextEventPt->GetMLEventPtData()->vPos;

	switch (m_EtInterator.Event.iEvent)
	{
	case EPE_CT_ENTERSUPPLY:

		if (m_iCurEventPt == 1)		//	Cannot not be the event point
		{
			m_iState = ST_NONE;
			return;
		}

		if (m_pMoxModel)
			m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_FLYTOHANG], true);

	//	vNextPos.y	= (GETGAMELOGICPARAMS->fCopterFlyHeight + GETGAMELOGICPARAMS->fCopterSupplyHeight) * 0.5f;
		vNextPos.y	= GETGAMELOGICPARAMS->fCopterSupplyHeight;
		m_iState	= ST_ENTER;
		BeginCircleFly(m_vPos, vNextPos, 0.0f);

		break;

	default:	//	Normal fly or circle fly
	{
		if (m_iCurEventPt == 1)
		{
			BeginNormalFly(vCurPos, vNextPos, true);
		}
		else
		{
			vNextPos.y	= GETGAMELOGICPARAMS->fCopterFlyHeight;
			m_iState	= ST_CIRCLEFLY;
			BeginCircleFly(m_vPos, vNextPos, GETGAMELOGICPARAMS->fCopterSpeed);
		}

		break;
	}
	}
}

//	Begin normal fly
void CMLHelicopter::BeginNormalFly(A3DVECTOR3& p0, A3DVECTOR3& p1, bool bAppear)
{
	A3DVECTOR3 vModelDir;

	if (bAppear)
	{
		A3DVECTOR3 vCurPos, vNextPos;

		m_iState		= ST_APPEAR;
		vModelDir		= Normalize(p1 - p0);
		vNextPos		= p0;
		vNextPos.y		= GETGAMELOGICPARAMS->fCopterFlyHeight;
		m_vCurMoveDir	= Normalize(A3DVECTOR3(vModelDir.x, 0.2f, vModelDir.z));
		vCurPos			= vNextPos - m_vCurMoveDir * 300.0f;
		m_fDistance		= 300.0f;
		m_fDistCnt		= 0.0f;
		
		SetPos(vCurPos);
	}
	else
	{
		m_iState		= ST_NORMALFLY;
		m_fDistCnt		= 0.0f;
		m_fDistance		= Normalize(p1 - m_vxzPos, m_vCurMoveDir);
		m_fRollRadian	= 0.0f;
		m_fRollCnt		= 0.0f;
		m_fAccelerator	= 0.0f;
		vModelDir		= m_vCurMoveDir;
	}

	SetVel(m_vCurMoveDir * GETGAMELOGICPARAMS->fCopterSpeed);
	
	if (m_pMoxModel)
	{
		m_pMoxModel->SetDirAndUp(vModelDir, g_vAxisY);
		m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_FLY], true);
	}
}

/*	Begin circle flying

	p0: start position
	p1: destination position
	v1: the wished speed at p1
*/
void CMLHelicopter::BeginCircleFly(A3DVECTOR3& p0, A3DVECTOR3& p1, float v1)
{
	A3DVECTOR3 vChordDir, vCurPos = A3DVECTOR3(p0.x, 0.0f, p0.z);
	m_vCurTarget = A3DVECTOR3(p1.x, 0.0f, p1.z);

	float fChordLen = Normalize(m_vCurTarget - vCurPos, vChordDir);

	A3DVECTOR3 vCurRight = CrossProduct(g_vAxisY, m_vCurMoveDir);
	bool bInvRight = false;

	float fRadius, fCos = DotProduct(vChordDir, vCurRight);

	if (fCos < 0.0f)
	{
		vCurRight = -vCurRight;
		bInvRight = true;
		fCos = -fCos;
	}

	if (fCos < 0.0001f)
		fRadius = fChordLen * 0.5f;
	else
		fRadius = fChordLen * 0.5f / fCos;

	fCos = DotProduct(m_vCurMoveDir, vChordDir);
	float fArcRad = (float)(acos(fCos) * 2.0f);
	float fArcLen = fRadius * fArcRad;

	/*	In circle flying, we use m_fDistance as time, and m_fDistCnt
		as time counter. Here we calculate the time and accelerator 
		will be used to	pass through the arc by the two equation:

		s  = v0 * t + 0.5 * a * t * t;
		v1 = v0 + a * t;

		Here, s (fArcLen), v0 (m_fSpeed) and v1 are known, 
		try to calculate t and a.
	*/
	m_fDistance		= 2.0f * fArcLen / (m_fSpeed + v1);
	m_fAccelerator	= (v1 * v1 - m_fSpeed * m_fSpeed) / (2.0f * fArcLen);
	m_fDistCnt		= 0.0f;
	m_fHeightDelta	= (p1.y - p0.y) / m_fDistance;

	//	Calculate yaw delta
	m_fYawDelta = fArcRad / m_fDistance;
	if (bInvRight)
		m_fYawDelta = -m_fYawDelta;

	//	Calculate roll delta
	float fRollRad;

	if (fRadius >= 100.0f)
		fRollRad = 5.0f;
	else if (fRadius <= 10.0f)
		fRollRad = 45.0f;
	else
		fRollRad = 5.0f + (45.0f - 5.0f) * (100.0f - fRadius) / (100.0f - 10.0f);
	
	fRollRad = DEG2RAD(fRollRad);
	if (!bInvRight)
		fRollRad = -fRollRad;

	m_fRollCnt		= m_fRollRadian;
	m_fRollRadian	= fRollRad - m_fRollRadian;
	m_fRollDelta	= m_fRollRadian / (m_fDistance * 0.2f);
}

//	Begin supply
void CMLHelicopter::BeginSupply()
{
	m_iState	= ST_SUPPLY;
	m_iSubState	= SUBST_ADJUST;

	A3DVECTOR3 vDestDir = GETMAPDATA->GetActorPos();
	vDestDir.y	= 0.0f;
	vDestDir	= vDestDir - m_vxzPos;
	vDestDir	= Normalize(CrossProduct(vDestDir, g_vAxisY));

	A3DVECTOR3 vCurRight = CrossProduct(g_vAxisY, m_vCurMoveDir);

	float fCos = DotProduct(m_vCurMoveDir, vDestDir);
	float fYawRad = (float)acos(fCos);

	//	Use m_fDistance and m_fDistCnt as time
	m_fDistance = 1.5f;
	m_fDistCnt	= 0.0f;

	//	Yaw delta
	m_fYawDelta = fYawRad / m_fDistance;
	if (DotProduct(vDestDir, vCurRight) < 0.0f)
		m_fYawDelta = -m_fYawDelta;

	//	Roll delta
	m_fRollDelta	= -m_fRollRadian / m_fDistance;
	m_fRollCnt		= m_fRollRadian;
	m_fRollRadian	= 0.0f;

//	if (m_pMoxModel)
//		m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_FLYTOHANG], true);
}

//	Leave supply
void CMLHelicopter::LeaveSupply()
{
	if (++m_iCurEventPt >= m_pRoute->GetEventPtNum())
		return;

	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);

	A3DVECTOR3 vNextPos	= pEventPt->GetMLEventPtData()->vPos;
	vNextPos.y = GETGAMELOGICPARAMS->fCopterFlyHeight;

	m_iState = ST_LEAVE;

	BeginCircleFly(m_vPos, vNextPos, GETGAMELOGICPARAMS->fCopterSpeed);
}

//	Can be used as fire target ?
bool CMLHelicopter::CanBeHit()
{
	if (m_iState == ST_CRASHING || m_iState == ST_WRECKAGE ||
		m_iState == ST_APPEAR || m_iState == ST_DISAPPEAR)
		return false;

	return true;
}

/*	Use this function to calculate the possible collision position if a object wants
	to hit the helicopter. The function considering copter's current speed when 
	calculate.

	Return true if a position is calculated. return false if helicopter currently 
	cannot be used as a hit target.

	bHit: true, at the result position, copter will be hit. false, at the result 
		  position, copter won't be hit
	vObj: object's position
	fSpeed: object's speed
	pHitPos (out): receive result position if true is returned.
*/
bool CMLHelicopter::CalcHitPos(bool bHit, A3DVECTOR3& vObj, float fSpeed, A3DVECTOR3* pHitPos)
{
	if (!CanBeHit())
		return false;

	if (!bHit)
	{
		A3DVECTOR3 vUp, vRight, vDir = Normalize(m_vPos - vObj);
		MLA_GetViewUpAndRight(vDir, &vUp, &vRight);

		*pHitPos = MLA_GetRandomPosInDisk(m_vPos, vUp, vRight, 10.0f, 5.0f);
		return true;
	}

	if ((m_iSubState == SUBST_SUPPLY || m_iSubState == SUBST_ADJUST) &&
		m_iState == ST_SUPPLY)
	{
		*pHitPos = m_vPos;
		return true;
	}

	A3DVECTOR3 vPos = m_vPos + g_vAxisY;

	//	Give a chance to hit tail
//	if (m_pMoxModel && MLA_Percent50())
//		vPos = g_theGame.GetHangPointPosAndDir(m_pMoxModel, "HH_Œ≤“Ìπ“µ„");

	/*	Our calculation bases on the equation:

		mag((p0 + v0 * t - p1) / t) = fSpeed

		p0: copter's current position (vPos)
		v0: copter's current velocity (m_vVel)
		p1: object's current position (vObj)
		fSpeed: object's current speed (mag(vel))
		t: object's flying time we are concerning about.
	*/
	A3DVECTOR3 vDist = vPos - vObj;
	float t, a, b, c;

	b = -2.0f * DotProduct(vDist, m_vVel);
	a = fSpeed * fSpeed - m_fSpeed * m_fSpeed;
	c = Magnitude(vDist);
	c = -c * c;

	float fDelta = b * b - 4.0f * a * c;
	if (fDelta < 0.0f)
	{
		*pHitPos = vPos;
		return true;
	}

	t = (-b + sqrt(fDelta)) / (2.0f * a);

	*pHitPos = vPos + m_vVel * t;

	return true;
}

//	Rsay trace routine
bool CMLHelicopter::RayTrace(MLRAYTRACE* pTrace)
{
	if (!m_bDoTrace)
		return false;

	RAYTRACE Trace;
	if (m_pMoxModel->RayTrace(pTrace->vStart, pTrace->vVel * pTrace->fTime, &Trace))
	{
		pTrace->iObjectType = MLTRACE_COPTER;
		pTrace->pObject		= (void*) this;
		
		pTrace->Trace.fFraction	= Trace.fFraction;
		pTrace->Trace.vPoint	= Trace.vPoint;

	//	A3DMODELOBB* pOBB = Trace.pModelOBB;
	//	if (pOBB && !_stricmp(pOBB->frameOBB.szName, "BB_≈ˆ◊≤ºÏ≤‚01"))
		if (MLA_Percent50())
			pTrace->dwInfo = 1;
		else
			pTrace->dwInfo = 2;

		return true;
	}

	return false;
}

/*	Strike helicopter

	pBullet: bullet object.
	bBody: true, helicopter's body was hit. false, helicopter's tail was hit
*/
void CMLHelicopter::Strike(CMLBullet* pBullet, bool bBody)
{
	if (!m_bDoTrace)
		return;

	int iDamage = pBullet->GetDamage();
	if ((m_iLife -= iDamage) > 0)
		return;

	m_bDoTrace = false;

	INCSTATISTIC(m_nId, 1, 0);

	if (bBody)
	{
		g_theGame.PlayGFX(RES_GFX_COPTEREXPLODE, m_vPos, 1.0f);
		g_theGame.Play3DSound(RES_S3D_COPTEREXPLODE1+MLA_Random(0, 2), m_vPos, 40, 100);
		g_theGame.Stop3DSound(m_pFlySnd);
		m_bDead = true;
		return;
	}

	if (!m_pMoxModel)
	{
		m_bDead = true;
		return;
	}

	m_iState		= ST_CRASHING;
	m_fAccelerator	= -8.0f;
	m_fMinHSpeed	= MagnitudeH(m_vVel) * 0.2f;
	m_vVel.y		= -GETGAMELOGICPARAMS->fCopterDropSpeed;

	SetVel(m_vVel);

	//	Chnage model
	ChangeModel();

	if (m_pMoxModel)
		m_pMoxModel->PlayActionByName((char*)m_aActions[ACT_CRASH], true);

	g_theGame.Play3DSound(RES_S3D_COPTERHURT1+MLA_Random(0, 2), m_vPos, 40, 100);
	g_theGame.Play2DSound(RES_S2D_COPTERDOWN, false);

	g_theGame.Release3DSound(m_pFlySnd);
	m_pFlySnd = g_theGame.Create3DSound(RES_S3D_COPTERFLYDOWN);
	g_theGame.Play3DSound(m_pFlySnd, m_vPos, false);
}

//	Do supply
void CMLHelicopter::DoSupply()
{
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();

	pPlayer->SupplyAmmo(ID_WEAPON_PISTOL, GETPLAYERILDVALUE(ILD_AMMOINC_PISTOL, iLevel));
	pPlayer->SupplyAmmo(ID_WEAPON_RIFLE, GETPLAYERILDVALUE(ILD_AMMOINC_RIFLE, iLevel));
	pPlayer->SupplyAmmo(ID_WEAPON_MORTAR, GETPLAYERILDVALUE(ILD_AMMOINC_MORTAR, iLevel));
	pPlayer->SupplyAmmo(ID_WEAPON_MACHINE, GETPLAYERILDVALUE(ILD_AMMOINC_MACHINE, iLevel));
	pPlayer->SupplyAmmo(ID_WEAPON_RPG, GETPLAYERILDVALUE(ILD_AMMOINC_RPG, iLevel));
	pPlayer->SupplyLife(GETPLAYERILDVALUE(ILD_LIFE_SUPPLY, iLevel));

	g_theGame.Play2DSound(RES_S2D_SUPPLYFINISH, false);
}

//	Pause sound
void CMLHelicopter::PauseSound(bool bPause)
{
	CMLModelUnit::PauseSound(bPause);
	g_theGame.Pause3DSound(m_pFlySnd, bPause, true);
}

//	Stop sound
void CMLHelicopter::StopSound()
{
	CMLModelUnit::StopSound();
	g_theGame.Stop3DSound(m_pFlySnd);
}

