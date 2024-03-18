/*
 * FILE: ML_AIUnit.cpp
 *
 * DESCRIPTION: AI unit event routines for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_AIUnit.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLAIUnit
//
///////////////////////////////////////////////////////////////////////////

/*	Do event on event point

	bNew: true, the event is just started.
*/
void CMLAIUnit::DoEvent(bool bNew)
{
	m_bNewEvent	= bNew;

	bool bUseTime = true;
	if (m_EtInterator.Event.iEvent == EPE_AI_STANDSHOT_R ||
		m_EtInterator.Event.iEvent == EPE_AI_CREEPSHOT_R ||
		m_EtInterator.Event.iEvent == EPE_AI_SHIFTSHOT_R)
		bUseTime = false;

	if (bNew)
	{
		m_iEventTimeCnt	= 0;
		m_iActTimeCnt	= 0;
		m_iWaitAnimCnt	= 0;
	}
	else if (bUseTime)
		m_iEventTimeCnt += m_iTimeSpan;
	
	int iLimit = bUseTime ? m_EtInterator.Event.iTime : m_EtInterator.Event.iTime;

	if (m_EtInterator.Event.iTime >= 0 && m_iEventTimeCnt >= iLimit)
	{
		m_EtInterator.Event.iEvent = EPE_NONE;
		return;
	}

	bool bRet = false;

	switch (m_EtInterator.Event.iEvent)
	{
	case EPE_AI_STANDSHOT:		bRet = DoEvent_StandShot();			break;	
	case EPE_AI_CREEPSHOT:		bRet = DoEvent_CreepShot();			break;
	case EPE_AI_SHIFTSHOT:		bRet = DoEvent_ShiftShot();			break;
	case EPE_AI_USEMOXMODEL:	bRet = DoEvent_UseMoxModel();		break;
	case EPE_AI_JUMPFORWARD:	bRet = DoEvent_JumpForward();		break;
	case EPE_AI_STAB:			bRet = DoEvent_Stab();				break;
	case EPE_AI_STAND:			bRet = DoEvent_Stand();				break;
	case EPE_AI_CREEP:			bRet = DoEvent_Creep();				break;
	case EPE_AI_STANDSHOT_R:	bRet = DoEvent_RocketStandShot();	break;
	case EPE_AI_CREEPSHOT_R:	bRet = DoEvent_RocketCreepShot();	break;
	case EPE_AI_SHIFTSHOT_R:	bRet = DoEvent_RocketShiftShot();	break;
	}

	if (!bRet)	//	End event
	{
		m_EtInterator.Event.iEvent = EPE_NONE;
	}
}

//	Footman stand shot
bool CMLAIUnit::DoEvent_StandShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_STAND_AIM);
		m_iSubState = SUBSTATE_STANDSHOT;
		m_iAISSInterval = m_pRoute->GetAISSInterval(false);
	}

	m_iActTimeCnt += m_iTimeSpan;
	if (m_iActTimeCnt >= m_iAISSInterval)
	{
		//	Fire !
		Fire(POSE_STAND);

		m_iActTimeCnt -= m_iAISSInterval;
	}

	return true;
}

//	Footman creep shot
bool CMLAIUnit::DoEvent_CreepShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_SQUAT_AIM);
		m_iSubState = SUBSTATE_SQUATSHOT;
		m_iAICSInterval = m_pRoute->GetAICSInterval(false);
	}

	m_iActTimeCnt += m_iTimeSpan;
	if (m_iActTimeCnt >= m_iAICSInterval)
	{
		//	Fire !
		Fire(POSE_SQUAT);

		m_iActTimeCnt -= m_iAICSInterval;
	}

	return true;
}

//	Footman shift shot
bool CMLAIUnit::DoEvent_ShiftShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_STAND_AIM);
		m_iSubState = SUBSTATE_STANDSHOT;
		m_iAISSInterval = m_pRoute->GetAISSInterval(false); 
	}

	switch (m_iSubState)
	{
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);

		if (m_iWaitAnimCnt)
			return true;

		//	Just fire, move to a new position
		Shift(GETGAMELOGICPARAMS->fAIShiftMin, GETGAMELOGICPARAMS->fAIShiftMax);

		m_iSubState = SUBSTATE_RUN;
		break;
	}
	case SUBSTATE_RUN:
	{
		m_iActTimeCnt += m_iTimeSpan;

		A3DVECTOR3 vxzPos = m_vxzPos + m_vCurMoveDir * (m_fAISpeed * m_fTimeSpan * 0.001f);

		if (ReachCurrentTarget(vxzPos))
		{
			PlayAction(ACT_STAND_AIM);
			m_iSubState = SUBSTATE_STANDSHOT;
		}
		else
		{
			//	TODO: Do this every tick is extremely wasting
			SetNoHeightPos(vxzPos);
		}
		
		break;
	}
	case SUBSTATE_STANDSHOT:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAISSInterval)
		{
			//	Fire !
			Fire(POSE_STAND);
			
			m_iActTimeCnt = 0;
			WaitAnimation(10);
		}

		break;
	}
	}

	return true;
}

//	Change to mox model
bool CMLAIUnit::DoEvent_UseMoxModel()
{
	if (m_bNewEvent)
	{
		if (m_bUseMoxModel || m_nId != ID_AI_FOOTMAN)
			return false;

		if (!(m_pMoxModel = g_theGame.LoadA3DModel(RES_MOX_FOOTMAN)))
		{
			ADEBUG_TRACE("Failed to load model");
			return false;
		}

		m_pMoxModel->SetPos(m_vPos);
		m_pMoxModel->SetDirAndUp(-Normalize(m_vxzPos), g_vAxisY);
		m_pMoxModel->UpdateModelOBB();

		m_bUseMoxModel = true;
	}

	return false;
}

//	Jump forward
bool CMLAIUnit::DoEvent_JumpForward()
{
	if (m_bNewEvent)
	{
		if (m_vCurMoveDir.x == 0.0f && m_vCurMoveDir.z == 0.0f)
		{
			m_vCurMoveDir = GetAIToPlayerDir(NULL, NULL);
			PlayAction(ACT_JUMP);
		}
		else
			SelectDirAction(ACT_JUMP, m_vCurMoveDir);

		m_iSubState = SUBSTATE_JUMP;

		float fTime		= 0.8f;
		float fSpeed	= GETGAMELOGICPARAMS->fAIJumpDist / fTime;
		float fVSpeed	= fTime * 5.0f;

		SetVel(m_vCurMoveDir * fSpeed + g_vAxisY * fVSpeed);
		return true;
	}
	
	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta, v1(m_vVel.x, m_vVel.y - 10.0f * fTickTime, m_vVel.z);

	vDelta.x = m_vVel.x * fTickTime;
	vDelta.z = m_vVel.z * fTickTime;
	vDelta.y = (m_vVel.y + v1.y) * 0.5f * fTickTime;

	SetVel(v1);

	A3DVECTOR3 vPos = m_vPos + vDelta;
	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(vPos);

	if (vPos.y <= fHeight)
	{
		vPos.y = fHeight;
		SetPos(vPos);
		return false;
	}

	SetPos(vPos);
	return true;
}

//	Stab
bool CMLAIUnit::DoEvent_Stab()
{
	if (!m_pMoxModel)
	{
		ADEBUG_TRACE("Stab before change model !!");
		return false;
	}

	if (m_bNewEvent)
	{
		PlayAction(MOXACT_STAND);
		m_iSubState = SUBSTATE_STAND;
		m_iAIStabInterval = m_pRoute->GetAISSInterval(false);

		//	Fact to player
		A3DVECTOR3 vPlayerPos = GETMAPDATA->GetActorPos();
		vPlayerPos.y = 0.0f;
		m_pMoxModel->SetDirAndUp(Normalize(vPlayerPos - m_vxzPos), g_vAxisY);
	}

	switch (m_iSubState)
	{
	case SUBSTATE_STAND:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAIStabInterval)
		{
			//	Stab !
			Stab();
			
			m_iActTimeCnt -= m_iAIStabInterval;
			WaitAnimation(20);
		}

		break;
	}
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);

		if (m_iWaitAnimCnt)
			return true;

		//	Move to a new position
		Shift(GETGAMELOGICPARAMS->fAIShiftMin, GETGAMELOGICPARAMS->fAIShiftMax);

		PlayAction(MOXACT_CHARGE);
		m_iSubState = SUBSTATE_RUN;

		break;
	}
	case SUBSTATE_RUN:
	{
		m_iActTimeCnt += m_iTimeSpan;

		A3DVECTOR3 vxzPos = m_vxzPos + m_vCurMoveDir * (m_fAISpeed * m_fTimeSpan * 0.001f);

		if (ReachCurrentTarget(vxzPos))
		{
			//	Fact to player
			A3DVECTOR3 vPlayerPos = GETMAPDATA->GetActorPos();
			vPlayerPos.y = 0.0f;
			m_pMoxModel->SetDirAndUp(Normalize(vPlayerPos - m_vxzPos), g_vAxisY);

			PlayAction(MOXACT_STAND);
			m_iSubState = SUBSTATE_STAND;
		}
		else
		{
			SetNoHeightPos(vxzPos);
		}
		
		break;
	}
	}

	return true;
}

//	Stand
bool CMLAIUnit::DoEvent_Stand()
{
	if (m_bNewEvent)
		PlayAction(ACT_STAND_AIM); 

	return true;
}

//	Creep
bool CMLAIUnit::DoEvent_Creep()
{
	if (m_bNewEvent)
		PlayAction(ACT_SQUAT_AIM); 

	return true;
}

//	Rocket man stand shot
bool CMLAIUnit::DoEvent_RocketStandShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_STAND_AIM);
		m_iSubState = SUBSTATE_STANDSHOT;
		m_iAISSInterval = m_pRoute->GetAISSInterval(false);
	}

	switch (m_iSubState)
	{
	case SUBSTATE_STANDSHOT:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAISSInterval)
		{
			//	Fire !
			Fire(POSE_STAND);
			WaitAnimation(30, 1);
			
			m_iActTimeCnt -= m_iAISSInterval;
		}

		break;
	}
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);
		if (m_iWaitAnimCnt)
			return true;

		PlayAction(ACT_STAND_AIM);
		m_iSubState = SUBSTATE_STANDSHOT;

		//	Finish a complete fire action
		m_iEventTimeCnt++;

		break;
	}
	}

	return true;
}

//	Rocket man creep shot
bool CMLAIUnit::DoEvent_RocketCreepShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_SQUAT_AIM);
		m_iSubState = SUBSTATE_SQUATSHOT;
		m_iAICSInterval = m_pRoute->GetAICSInterval(false);
	}

	switch (m_iSubState)
	{
	case SUBSTATE_SQUATSHOT:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAICSInterval)
		{
			//	Fire !
			Fire(POSE_SQUAT);
			WaitAnimation(20, 1);

			m_iActTimeCnt -= m_iAICSInterval;
		}

		break;
	}
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);
		if (m_iWaitAnimCnt)
			return true;

		PlayAction(ACT_SQUAT_AIM);
		m_iSubState = SUBSTATE_SQUATSHOT;

		//	Finish a complete fire action
		m_iEventTimeCnt++;

		break;
	}
	}

	return true;
}

//	Rocket man shift shot
bool CMLAIUnit::DoEvent_RocketShiftShot()
{
	if (m_bNewEvent)
	{
		PlayAction(ACT_STAND_AIM);
		m_iSubState = SUBSTATE_STANDSHOT;
		m_iAISSInterval = m_pRoute->GetAISSInterval(false);
	}

	switch (m_iSubState)
	{
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);
		if (m_iWaitAnimCnt)
			return true;

		//	Finish a complete fire action
		if (m_EtInterator.Event.iTime >= 0 &&
			++m_iEventTimeCnt >= m_EtInterator.Event.iTime)
			return false;
			
		//	Just fire, move to a new position
		Shift(GETGAMELOGICPARAMS->fAIShiftMin, GETGAMELOGICPARAMS->fAIShiftMax);

		m_iSubState = SUBSTATE_RUN;
		break;
	}
	case SUBSTATE_RUN:
	{
		m_iActTimeCnt += m_iTimeSpan;

		A3DVECTOR3 vxzPos = m_vxzPos + m_vCurMoveDir * (m_fAISpeed * m_fTimeSpan * 0.001f);

		if (ReachCurrentTarget(vxzPos))
		{
			PlayAction(ACT_STAND_AIM);
			m_iSubState = SUBSTATE_STANDSHOT;
		}
		else
		{
			//	TODO: Do this every tick is extremely wasting
			SetNoHeightPos(vxzPos);
		}

		break;
	}
	case SUBSTATE_STANDSHOT:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAISSInterval)
		{
			//	Fire !
			Fire(POSE_STAND);
			WaitAnimation(30, 1);
			
			m_iActTimeCnt = 0;
		}

		break;
	}
	}

	return true;
}

//	Run
void CMLAIUnit::DoState_Run(bool bNew)
{
	if (bNew)
	{
		m_iState		= STATE_RUN;
		m_iSubState		= SUBSTATE_RUN;
		m_iActTimeCnt	= 0;
		m_iAIMSInterval = m_pRoute->GetAIMSInterval(false);
		return;
	}

	switch (m_iSubState)
	{
	case SUBSTATE_RUN:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iAIMSInterval > 0 && m_iActTimeCnt >= m_iAIMSInterval &&
			!m_bUseMoxModel && !m_bRushTo)
		{
			m_iActTimeCnt = 0;
			DoState_RunShot(true);
			return;
		}

		A3DVECTOR3 vxzPos = m_vxzPos + m_vCurMoveDir * (m_fAISpeed * m_fTimeSpan * 0.001f);
		
		if (ReachCurrentTarget(vxzPos))
		{
			if (m_bLastRunSeg)	//	Is last running segment ?
			{
				BeginEventPoint(false);
				return;
			}
			else	//	Continue run, get next target position
			{
				vxzPos = m_vCurTarget;
				SetoutToNextRunTarget();
			}
		}
		
		//	TODO: Do this every tick is extremely wasting
		SetNoHeightPos(vxzPos);
		break;
	}
	}
}

//	Run state
void CMLAIUnit::DoState_RunShot(bool bNew)
{
	if (bNew)
	{
		if (m_nId == ID_AI_GUERILLA)
		{
			Fire(POSE_RUN);
			return;
		}

		PlayAction(ACT_STAND_AIM);

		m_iState		= STATE_RUNSHOT;
		m_iSubState		= SUBSTATE_STANDSHOT;
		m_iAISSInterval	= MLA_Random(200, 600);		//	Wait a little while
		m_iActTimeCnt	= 0;
		return;
	}

	switch (m_iSubState)
	{
	case SUBSTATE_STANDSHOT:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= m_iAISSInterval)
		{
			//	Fire !
			Fire(POSE_STAND);
			WaitAnimation(m_nId == ID_AI_ROCKET ? 30 : 20, 1);
		}

		break;
	}
	case SUBSTATE_WAITANIM:
	{
		assert(m_iWaitAnimCnt >= 0);

		if (m_iWaitAnimCnt)
			return;

		bool bGoToRun = false;

		if (m_nId == ID_AI_ROCKET)
			bGoToRun = true;
		else if (MLA_PercentProbability(GETGAMELOGICPARAMS->iAIFusilladePercent))
		{
			//	Continue shot
			m_iActTimeCnt	= 0;
			m_iSubState		= SUBSTATE_STANDSHOT;
		}
		else
			bGoToRun = true;

		if (bGoToRun)	//	Restore to running state
		{
			m_iState		= STATE_RUN;
			m_iSubState		= SUBSTATE_RUN;
			m_iActTimeCnt	= 0;

			SelectDirAction(ACT_CHARGE, m_vCurMoveDir);
		}

		break;
	}
	}
}

//	Dying
void CMLAIUnit::DoState_Dying(bool bNew)
{
	if (bNew)
	{
		m_iState	= STATE_DYING;
		m_bDoTrace	= false;

		SelectDyingAction(false, NULL);

		USERCONFIGS* pConfig = GETCURUSERCONFIGS;
		if (pConfig->bGraphicsMisc)
		{
			int iGFX;

			//	Throw gun and helmet randomly
		#ifdef LANGUAGE_CHINESE

			if (m_nId != ID_AI_GUERILLA && MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
				g_theGame.PlayGFX(RES_GFX_THROWNHELMET, m_vPos + g_vAxisY * 1.7f, 1.0f);

		#else

			if (MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
			{
				iGFX = (m_nId == ID_AI_GUERILLA) ? RES_GFX_THROWNBBHAT : RES_GFX_THROWNHELMET;
				g_theGame.PlayGFX(iGFX, m_vPos + g_vAxisY * 1.7f, 1.0f);
			}

		#endif	//	LANGUAGE_CHINESE

			if (MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
			{
				iGFX = (m_nId == ID_AI_ROCKET) ? RES_GFX_THROWBAZOOKA : RES_GFX_THROWNGUN;
				g_theGame.PlayGFX(iGFX, m_vPos + g_vAxisY * 1.2f, 1.0f);
			}
		}

		WaitAnimation(40);
		return;
	}

	switch (m_iSubState)
	{
	case SUBSTATE_WAITANIM:

		assert(m_iWaitAnimCnt >= 0);

		if (m_iWaitAnimCnt)
			return;

		DoState_BeDead(true);
		break;
	}
}

//	Die fly
void CMLAIUnit::DoState_DieFly(bool bNew)
{
	if (bNew)
	{
		A3DVECTOR3 vDir = Normalize(A3DVECTOR3(m_vVel.x, 0.0f, m_vVel.z));
		SelectDyingAction(true, &vDir);

		m_iState		= STATE_DIEFLY;
		m_iSubState		= SUBSTATE_JUMP;
		m_bDoTrace		= false;
		m_iActTimeCnt	= 0;

		USERCONFIGS* pConfig = GETCURUSERCONFIGS;
		if (pConfig->bGraphicsMisc)
		{
		int iGFX;

		//	Throw gun and helmet randomly
		#ifdef LANGUAGE_CHINESE

			if (m_nId != ID_AI_GUERILLA && MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
				g_theGame.PlayGFX(RES_GFX_BLOWNHELMET, m_vPos + g_vAxisY * 1.7f, 1.0f);

		#else

			if (MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
			{
				iGFX = (m_nId == ID_AI_GUERILLA) ? RES_GFX_BLOWNNBBHAT : RES_GFX_BLOWNHELMET;
				g_theGame.PlayGFX(iGFX, m_vPos + g_vAxisY * 1.7f, 1.0f);
			}

		#endif	//	LANGUAGE_CHINESE

			if (MLA_PercentProbability(GETGAMELOGICPARAMS->iAIThrowGunPercentage))
			{
				iGFX = (m_nId == ID_AI_ROCKET) ? RES_GFX_BLOWNBAZOOKA : RES_GFX_BLOWNGUN;
				g_theGame.PlayGFX(iGFX, m_vPos + g_vAxisY * 1.2f, 1.0f);
			}
		}

		return;
	}

	m_iActTimeCnt += m_iTimeSpan;

	float fTickTime = m_fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta, v1(m_vVel.x, m_vVel.y - 10.0f * fTickTime, m_vVel.z);

	vDelta.x = m_vVel.x * fTickTime;
	vDelta.z = m_vVel.z * fTickTime;
	vDelta.y = (m_vVel.y + v1.y) * 0.5f * fTickTime;

	SetVel(v1);

	A3DVECTOR3 vPos = m_vPos + vDelta;
	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(vPos);

	if (vPos.y <= fHeight || m_iActTimeCnt >= 10000)
	{
		vPos.y = fHeight;
		SetPos(vPos);

		if (!m_bUseMoxModel)
		{
			if (m_iFlyAction == ACT_DIE_FLY_LEFT)
				PlayAction(ACT_DIE_FLYFALL_LEFT);
			else if (m_iFlyAction == ACT_DIE_FLY_RIGHT)
				PlayAction(ACT_DIE_FLYFALL_RIGHT);
		}

		DoState_BeDead(true);
	}
	else
	{
		SetPos(vPos);
		MoveFireOnBody(true);
	}
}

//	Catch fire
void CMLAIUnit::DoState_CatchFire(bool bNew)
{
	if (bNew)
	{
		m_iState		= STATE_CATCHFIRE;
		m_iSubState		= SUBSTATE_RUN;
		m_bCatchFire	= true;
		m_iActTimeCnt	= 0;

	/*	A3DVECTOR3 vRight, vDir;
		vDir = GetAIToPlayerDir(&vRight, NULL);

		float fRad = DEG2RAD(MLA_Random(-60.0f, 60.0f));
		m_vCurMoveDir = vDir * cos(fRad) + vRight * sin(fRad);

		fRad = MLA_Random(1.0f, 10.0f);
		m_vCurTarget = m_vxzPos + m_vCurMoveDir * fRad;
		m_fAISpeed = fRad / MLA_Random(3.0f, 6.0f);
	*/
		MLA_GenRandomVectorH(m_vCurMoveDir);
		float fRad	 = MLA_Random(3.0f, 10.0f);
		m_vCurTarget = m_vxzPos + m_vCurMoveDir * fRad;
		m_fAISpeed	 = fRad / MLA_Random(3.0f, 6.0f);

		SelectCatchFireAction(fRad);
		return;
	}

	m_iActTimeCnt += m_iTimeSpan;

	switch (m_iSubState)
	{
	case SUBSTATE_RUN:
	{
		A3DVECTOR3 vxzPos = m_vxzPos + m_vCurMoveDir * (m_fAISpeed * m_fTimeSpan * 0.001f);
		if (ReachCurrentTarget(vxzPos) || m_iActTimeCnt >= 10000)
		{
			PlayAction(ACT_ONFIRE_DIE);
			m_iSubState = SUBSTATE_WAITANIM;
			WaitAnimation(15);
			return;
		}

		SetNoHeightPos(vxzPos);
		MoveFireOnBody(true);

		break;
	}
	case SUBSTATE_WAITANIM:
	{
		A3DVECTOR3 vPos;

		if (!m_iWaitAnimCnt)
		{
			vPos = m_pGFX->GetPos();
			vPos.y -= 0.08f;
			m_pGFX->SetPos(vPos);
			return;
		}

		OnDie(2);
		DoState_BeDead(true);
		break;
	}
	}
}

//	Be dead
bool CMLAIUnit::DoState_BeDead(bool bNew)
{
	if (bNew)
	{
		m_iStrategy		= STARTEGY_BEDEAD;
		m_iSubState		= SUBSTATE_STAND;
		m_bDoTrace		= false;
		m_iActTimeCnt	= 0;

		MoveFireOnBody(false);
		return true;
	}

	switch (m_iSubState)
	{
	case SUBSTATE_STAND:
	{
		m_iActTimeCnt += m_iTimeSpan;
		if (m_iActTimeCnt >= GETGAMELOGICPARAMS->iAICorpseTime)
		{
			m_pRoute->AIDied();		//	Notify route AI's death

			m_bDead		= true;
			m_iStrategy = STRATEGY_NONE;
			m_iState	= STATE_NONE;
		}

		break;
	}
	}

	return true;
}

