/*
 * FILE: ML_AIUnit.cpp
 *
 * DESCRIPTION: AI unit class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/30
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
#include "ML_Bullet.h"
#include "ML_ManAI.h"
#include "ML_Player.h"
#include "ML_Helicopter.h"
#include "ML_ManBullet.h"
#include "ML_ManObject.h"

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

CMLAIUnit::CMLAIUnit() : CMLUnit()
{
	m_pImgModel			= NULL;
	m_pMoxModel			= NULL;
	m_pRoute			= NULL;
	m_pCamera			= NULL;
	m_pGFX				= NULL;
	m_bUseMoxModel		= false;
	m_iCurEventPt		= 0;
	m_iEventTimeCnt		= 0;
	m_fAISpeed			= 0.0f;
	m_iYawFlag			= 0;
	m_iActTimeCnt		= 0;
	m_iWaitAnimCnt		= 0;
	m_iLife				= 100;
	m_bCatchFire		= false;

	m_iStrategy			= STRATEGY_NONE;
	m_iState			= STATE_NONE;
	m_iSubState			= SUBSTATE_NONE;
	m_iAction			= ACT_REMAIN;
}

CMLAIUnit::~CMLAIUnit()
{
	//	Do real release here
	if (m_pImgModel)
	{
		m_pImgModel->Release();
		delete m_pImgModel;
		m_pImgModel = NULL;
	}
}

/*	Initialize object

	Return true for success, otherwise return false.

	bNewObj: true, this object is just allocated from memory.
			 false, this object is a used object.
	iAIID: AI's ID
	pRoute: pointer of route on which AI generate
	dwCol: image model's diffuse color
*/
bool CMLAIUnit::Init(bool bNewObj, int iAIID, CMLRoute* pRoute, DWORD dwCol)
{
	assert(ISAIID(iAIID));
	SetId(iAIID);

	m_pRoute = pRoute;

	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	assert(pAIMan);

	//	Load image model
	A3DImgModel* pDefModel = pAIMan->GetDefImageModel(m_nId);
	if (bNewObj)
	{
		if (!(m_pImgModel = new A3DImgModel))
		{
			ADEBUG_TRACE("Not enough memory");
			return false;
		}

		m_pImgModel->Copy(pDefModel);
	}
	else
		m_pImgModel->BitCopy(pDefModel);

	m_pImgModel->UseSheetRayTrace(false);
	m_pImgModel->SetVertexCol(-1, dwCol);

	//	Initialize states
	InitStates();

	return true;
}

//	Initialize states
void CMLAIUnit::InitStates()
{
	assert(m_pRoute);

	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	int iAIIdx = m_nId - ID_AI_FOOTMAN;

	//	Get AI speed
	m_fAISpeed = GETAIFLDVALUE(FLD_SPEED_STAND_F + iAIIdx, iLevel);
	float fSpeed = m_fAISpeed * GETGAMELOGICPARAMS->iAISpeedRange / 100.0f;
	fSpeed = MLA_Random(-fSpeed, fSpeed);
	if (m_fAISpeed + fSpeed > 0.0f)
		m_fAISpeed = m_fAISpeed + fSpeed;

	if (m_nId == ID_AI_ROCKET)
		m_iAIAttackCopter = GETAIILDVALUE(ILD_PER_ATTACKCOPTER, iLevel);

	m_iAction		= ACT_REMAIN;
	m_iYawFlag		= 0;
	m_bDoTrace		= true;
	m_bDead			= false;
	m_bInPreserve	= false;
	m_vCurMoveDir	= g_vOrigin;
	m_iLife			= GETAIILDVALUE(ILD_LIFE_FOOTMAN + iAIIdx, iLevel);
	m_iActTimeCnt	= 0;
	m_iCheckPosCnt	= 0;
	m_iStrategy		= STRATEGY_WAIT;
	m_iState		= STATE_STAND;
	m_bCatchFire	= false;
	m_iAISSInterval	= m_pRoute->GetAIWaitTime();	//	Temporarily use m_iAISSInterval
}

/*	Release object, This function doesn't relese all resources really, because
	this object will be pushed into a object buffer managered by ai manager and
	may be poped again.
*/
bool CMLAIUnit::Release()
{
	//	Release mox model if it exists
	if (m_pMoxModel)
	{
		g_theGame.ReleaseA3DModel(m_pMoxModel);
		m_pMoxModel = NULL;
	}

	if (m_pGFX)
	{
		g_theGame.ReleaseGFX(m_pGFX);
		m_pGFX = NULL;
	}

	m_pRoute		= NULL;
	m_pCamera		= NULL;
	m_pMoxModel		= NULL;
	m_bUseMoxModel	= false;
	m_iAction		= ACT_REMAIN;
	m_iStrategy		= STRATEGY_NONE;
	m_iState		= STATE_NONE;

	return true;
}

//	Set position
void CMLAIUnit::SetPos(A3DVECTOR3& vPos)
{
	CMLUnit::SetPos(vPos);

	m_vxzPos = A3DVECTOR3(vPos.x, 0.0f, vPos.z);

	if (m_bUseMoxModel)
	{
		m_pMoxModel->SetPos(vPos);
	}
	else
	{
		assert(m_pImgModel);
		m_pImgModel->SetPos(vPos);
	}
}

//	Set position without height info.
void CMLAIUnit::SetNoHeightPos(A3DVECTOR3& vPos)
{
	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(vPos);
	SetPos(A3DVECTOR3(vPos.x, fHeight, vPos.z));
}

//	Tick animation
bool CMLAIUnit::TickAnimation()
{
	if (m_iSubState == SUBSTATE_WAITANIM && m_iWaitAnimCnt > 0)
		m_iWaitAnimCnt--;

	if (m_bUseMoxModel)
	{
		//	Tick animation mox model
		m_pMoxModel->TickAnimation();
		return true;
	}

	//	Tick animation image model
	m_pImgModel->TickAnimation();
	m_pImgModel->UpdateShape(m_pCamera);

	return true;
}

//	Render routine
bool CMLAIUnit::Render(A3DViewport* pViewport)
{
	if (m_bUseMoxModel)
	{
		if (m_pMoxModel)
			m_pMoxModel->Render(pViewport, true, true);

		return true;
	}

	//	Render image model
	m_pImgModel->FakeRender(pViewport, false);

	return true;
}

/*	Reach event point and begin do events

	bNewPos: true, get new position for AI. false, use m_vNextEPPos as AI's position
*/
void CMLAIUnit::BeginEventPoint(bool bNewPos)
{
	//	Get the first event point of route
	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);

	//	Initiailize event interator
	CMLEventPt::InitEventInterator(&m_EtInterator);

	//	AI attack copter possibility on event point
	m_iAIAttackCopter = pEventPt->GetAIAttackCopterPoss();

	if (bNewPos)
		m_vxzPos = pEventPt->GetRandomPos();
	else
		m_vxzPos = m_vNextEPPos;

	m_vLastEPPos = m_vxzPos;
	m_vCurTarget = m_vxzPos;
	SetNoHeightPos(m_vxzPos);

	m_iStrategy = STRATEGY_DOEVENT;
	m_iState	= STATE_ONEVENTPT;
}

/*	Logic tick

	Return true for success, otherwise return false
	
	fTimeSpan: time span of this tick, in ms
	dwTimeCnt: time count from beginning of mission (including this tick), in ms
*/
bool CMLAIUnit::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_iStrategy == STRATEGY_NONE || m_iState == STATE_NONE)
		return true;

	bool bRet = true;

	m_fTimeSpan	= fTimeSpan;
	m_dwTimeCnt = dwTimeCnt;
	m_iTimeSpan	= ML_ConvertTimeSpan(fTimeSpan);

	switch (m_iStrategy)
	{
	case STRATEGY_WAIT:		bRet = LogicRun_Wait();			break;
	case STRATEGY_DOEVENT:	bRet = LogicRun_DoEvent();		break;
	case STRATEGY_RUNING:	bRet = LogicRun_Runing();		break;
	case STRATEGY_DYING:	bRet = LogicRun_Dying();		break;
	case STARTEGY_BEDEAD:	bRet = DoState_BeDead(false);	break;
	}

	if (m_iStrategy == STRATEGY_RUNING || m_iStrategy == STRATEGY_DOEVENT)
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

	if (m_bUseMoxModel)
	{
		//	Update OBB
		m_pMoxModel->UpdateModelOBB();
	}
	else
	{
		m_pImgModel->UpdateShape(m_pCamera);
	}

	return bRet;
}

//	Logic tick when waiting
bool CMLAIUnit::LogicRun_Wait()
{
	m_iActTimeCnt += m_iTimeSpan;
	if (m_iActTimeCnt >= m_iAISSInterval)
	{
		//	Begin the first event point
		m_iCurEventPt = 0;
		BeginEventPoint(true);
	}

	return true;
}

//	Logic tick when doing events
bool CMLAIUnit::LogicRun_DoEvent()
{
	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);

	if (m_EtInterator.Event.iEvent == EPE_NONE)
	{
		//	Try to get next event
		if (!pEventPt->GetNextEvent(&m_EtInterator))
		{
			//	All events have been executed
			if (m_iCurEventPt + 1 >= m_pRoute->GetEventPtNum())
			{
				//	All route has been run, just stand
				m_iStrategy = STRATEGY_NONE;
				m_iState	= STATE_NONE;
			}
			else
			{
				//	Setout to next event on the route
				m_vCurTarget = m_vxzPos;
				SetoutToNextEventPt();
			}
		}
		else	//	Begin next event
		{
			DoEvent(true);
		}
	}
	else	//	Do current event
	{
		DoEvent(false);
	}

	return true;
}

//	Logic tick when running
bool CMLAIUnit::LogicRun_Runing()
{
	switch (m_iState)
	{
	case STATE_RUN:
	{
		DoState_Run(false);
		break;
	}
	case STATE_RUNSHOT:
	{
		DoState_RunShot(false);
		break;
	}
	}

	return true;
}

//	Logic tick when dying
bool CMLAIUnit::LogicRun_Dying()
{
	switch (m_iState)
	{
	case STATE_DYING:
	{
		DoState_Dying(false);
		break;
	}
	case STATE_DIEFLY:
	{
		DoState_DieFly(false);
		break;
	}
	case STATE_CATCHFIRE:
	{
		DoState_CatchFire(false);
		break;
	}
	}

	return true;
}

//	Setout to next event point
void CMLAIUnit::SetoutToNextEventPt()
{
	//	Get rush to flag
	CMLEventPt* pEventPt = m_pRoute->GetEventPt(m_iCurEventPt++);
	m_bRushTo = pEventPt->GetMLEventPtData()->bRushTo;

	//	Get next event point
	pEventPt = m_pRoute->GetEventPt(m_iCurEventPt);
	assert(pEventPt);
	
	m_vNextEPPos = pEventPt->GetRandomPos();
	m_fLinkerLen = Normalize(m_vNextEPPos - m_vxzPos, m_vLinkerDir);

	//	Posibility to hit copter
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	m_iAIAttackCopter = GETAIILDVALUE(ILD_PER_ATTACKCOPTER, iLevel);

	m_iStrategy = STRATEGY_RUNING;
	DoState_Run(true);

	SetoutToNextRunTarget();
}

//	Setout to next running target position
void CMLAIUnit::SetoutToNextRunTarget()
{
	float fLenLeft = m_fLinkerLen - DotProduct(m_vCurTarget - m_vLastEPPos, m_vLinkerDir);

	A3DVECTOR3 vRight, vNewTarget;

	//	Only less than 5 meters left to next event point
	if (fLenLeft < 5.0f || m_bRushTo)
	{
		vNewTarget		= m_vNextEPPos;
		m_bLastRunSeg	= true;
	}
	else
	{
		vRight = CrossProduct(g_vAxisY, m_vLinkerDir);
		
		float fLen = MLA_Random(8.0f, 16.0f);
		if (fLen >= fLenLeft)
		{
			fLen = fLenLeft;
			m_bLastRunSeg = true;
			vNewTarget = m_vNextEPPos;
		}
		else
		{
			m_bLastRunSeg = false;

			float fRightLen = MLA_Random(fLen * 0.3f, fLen * 0.7f);
			if (!m_iYawFlag)
				m_iYawFlag = (int)(MLA_Percent50() ? 1.0f : -1.0f);
			
			fRightLen *= m_iYawFlag;
			m_iYawFlag = m_iYawFlag > 0 ? -1 : 1;
			
			//	New move target and direction
			vNewTarget = m_vCurTarget + m_vLinkerDir * fLen + vRight * fRightLen;
		}
	}

	m_vCurMoveDir = Normalize(vNewTarget - m_vCurTarget);
	m_vCurTarget  = vNewTarget;

	if (m_bUseMoxModel)
		m_pMoxModel->SetDirAndUp(m_vCurMoveDir, g_vAxisY);

	//	Select proper iamge model action
	SelectDirAction(ACT_CHARGE, m_vCurMoveDir);

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (pConfig->bSoundMisc && MLA_PercentProbability(5))
		g_theGame.Play3DSound(RES_S3D_AIRUSH1+MLA_Random(0, 11), m_vPos);
}

//	Reach current position ?
bool CMLAIUnit::ReachCurrentTarget(A3DVECTOR3& vDestPos)
{
	float fCurDelta, fDelta;

	fCurDelta = DotProduct(vDestPos - m_vxzPos, m_vCurMoveDir);
	fDelta	  = DotProduct(m_vCurTarget - m_vxzPos, m_vCurMoveDir);

	if (fCurDelta >= fDelta)
		return true;

	return false;
}

//	Play action
void CMLAIUnit::UpdateAction()
{
	if (m_iAction == ACT_REMAIN)
		return;

	int iAIIndex = m_nId - ID_FIRSTAI;

	if (m_bUseMoxModel)		//	Use mox model
	{
		g_theGame.PlayA3DModelAction(m_pMoxModel, (char*)m_aActions[iAIIndex][m_iAction],
									 true, true, false);
	}
	else	//	Use iamge model
	{
		m_pImgModel->PlayActionByName((char*)m_aActions[iAIIndex][m_iAction]);
		m_pImgModel->UpdateShape(m_pCamera);
	}

	m_iAction = ACT_REMAIN;
}

/*	Get direction degree for image model

	Return cosine of result angle.

		cosine(10) = 0.9848
		cosine(30) = 0.866
		cosine(45) = 0.7071
		cosine(60) = 0.5
		cosine(80) = 0.17365
	
	vDir: AI's current direction on xz plane
	piSide (out): used to receive side flag, can be NULL. -1 means left, 0, face
				  1, right.
	fFace: compare cosine value
*/
float CMLAIUnit::GetDirDegree(A3DVECTOR3& vDir, int* piSide, float fFace/* 0.17365f */)
{
	A3DVECTOR3 vPlayPos = GETMAPDATA->GetActorPos();
	
	vPlayPos.y = 0.0f;
	A3DVECTOR3 v = Normalize(vPlayPos - m_vxzPos);

	if (piSide)
	{
		A3DVECTOR3 vRight = CrossProduct(g_vAxisY, v);
		float t = DotProduct(vRight, vDir);
		if (t > fFace)
			*piSide = 1;
		else if (t < -fFace)
			*piSide = -1;
		else
			*piSide = 0;
	}

	return DotProduct(vDir, v);
}

/*	Get the direction from AI to player

	Return the direction from AI to player.

	pvRight (out): used to receive AI's right direction, can be NULL
	pfDist (out): used to receive distance between AI and player, can be NULL
*/
A3DVECTOR3 CMLAIUnit::GetAIToPlayerDir(A3DVECTOR3* pvRight, float* pfDist)
{
	float fDist;
	A3DVECTOR3 vToPlayer = GETMAPDATA->GetActorPos();
	vToPlayer.y = 0.0f;

	fDist = Normalize(vToPlayer - m_vxzPos, vToPlayer);
	
	if (pvRight)
		*pvRight = CrossProduct(g_vAxisY, vToPlayer);

	if (pfDist)
		*pfDist = fDist;

	return vToPlayer;
}

/*	Select proper action considering the AI's moving direction

	iAction: base action.
	vDir: AI current direction
*/
void CMLAIUnit::SelectDirAction(int iAction, A3DVECTOR3& vDir)
{
	if (m_bUseMoxModel)
	{
		//	Play action directly
		PlayAction(iAction);
		return;
	}

	int iSide;
	float fDot = GetDirDegree(vDir, &iSide);

	if (!iSide && fDot > 0.0f)	//	Face to player
	{
		//	iAction is the propery action
	}
	else if (iSide > 0)
	{
		if (fDot > 0.17365f)	//	Right 45 degree
		{
			switch (iAction)
			{
			case ACT_CHARGE:	iAction = ACT_CHARGE_RIGHT45;	break;
			case ACT_JUMP:		iAction = ACT_JUMP_RIGHT;		break;
			}
		}
		else	//	Right 90 degree
		{
			switch (iAction)
			{
			case ACT_CHARGE:	iAction = ACT_CHARGE_RIGHT90;	break;
			case ACT_JUMP:		iAction = ACT_JUMP_RIGHT;		break;
			}
		}
	}
	else
	{
		if (fDot > 0.17365f)	//	Left 45 degree
		{
			switch (iAction)
			{
			case ACT_CHARGE:	iAction = ACT_CHARGE_LEFT45;	break;
			case ACT_JUMP:		iAction = ACT_JUMP_LEFT;		break;
			}
		}
		else	//	Left 90 degree
		{
			switch (iAction)
			{
			case ACT_CHARGE:	iAction = ACT_CHARGE_LEFT90;	break;
			case ACT_JUMP:		iAction = ACT_JUMP_LEFT;		break;
			}
		}
	}

	PlayAction(iAction);
}

//	Select dying action
void CMLAIUnit::SelectDyingAction(bool bFly, A3DVECTOR3* pDir)
{
	int iAction;

	if (m_bUseMoxModel)
	{
		PlayAction(MOXACT_DIE0 + MLA_Random(0, 2));
		return;
	}

	if (bFly && m_nId != ID_AI_GUERILLA)
	{
		if (MLA_PercentProbability(40))
		{
			int iSide;
			GetDirDegree(*pDir, &iSide, 0.0f);
			if (iSide >= 0)
				iAction = ACT_DIE_FLY_RIGHT;
			else
				iAction = ACT_DIE_FLY_LEFT;
		}
		else
			iAction = ACT_DIE_BACKWARD0 + MLA_Random(0, 6);

		m_iFlyAction = iAction;
	}
	else
		iAction = ACT_DIE_BACKWARD0 + MLA_Random(0, 6);

	//	Select proper dying action
	PlayAction(iAction);
}

/*	Select catch fire action

	fDist: distance AI will run with fire on body
*/
void CMLAIUnit::SelectCatchFireAction(float fDist)
{
	int iAction;
	A3DVECTOR3 vPos;

	if (fDist < 5.0f)
		iAction = ACT_ONFIRE;
	else
		iAction = ACT_ONFIRE_RUN;

	if (m_nId == ID_AI_GUERILLA)
		vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_Éí³¤");
	else
		vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_Éí³¤");

	if ((m_pGFX	= g_theGame.LoadGFX(RES_GFX_BODYFIRE)) != NULL)
	{
		m_pGFX->SetPos(vPos);
		m_pGFX->SetScale(0.6f);
		m_pGFX->Start(false);
	}

	PlayAction(iAction);
}

//	Stab
void CMLAIUnit::Stab()
{
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	int iDamage = GETAIILDVALUE(ILD_DAMAGE_STAB, iLevel);

	GETMLGAMEPLAY->GetLocalPlayer()->Stabbed(iDamage);

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	PlayAction(MOXACT_STAB);
	g_theGame.Play3DSound(RES_S3D_STAB1+MLA_Random(0, 2), m_vPos);
	
	if (pConfig->bSoundMisc && MLA_PercentProbability(20))
		g_theGame.Play3DSound(RES_S3D_AISTABSHOUT1+MLA_Random(0, 4), m_vPos);
}

/*	Fire function

	iPose: AI pose, 
*/
void CMLAIUnit::Fire(int iPose)
{
	assert(!m_bUseMoxModel);

	if (m_nId == ID_AI_ROCKET)
	{
		RocketManFire(iPose);
		return;
	}

	A3DVECTOR3 vPos, vPlayerPos = GETMAPDATA->GetActorPos();
	int iPrecise;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (iPose == POSE_STAND)
	{
		iPrecise = AttenuatePrecise(m_pRoute->GetAISSPrecise(), vPlayerPos);

		if (m_nId == ID_AI_FOOTMAN)
		{
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_Õ¾Á¢¿ª»ð");
			g_theGame.PlayGFX(RES_GFX_GUNFIRELIGHT, vPos, 0.4f);
		}
		else if (m_nId == ID_AI_GUERILLA)
		{
			vPos = m_pImgModel->GetMarkPointPos("¶·óÒ_Õ¾Á¢¿ª»ð");
			g_theGame.PlayGFX(RES_GFX_GUNFIRELIGHT, vPos, 0.4f);
		}

		PlayAction(ACT_STAND_SHOT);
	}
	else if (iPose == POSE_SQUAT)
	{
		iPrecise = AttenuatePrecise(m_pRoute->GetAICSPrecise(), vPlayerPos);

		if (m_nId == ID_AI_FOOTMAN)
		{
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_¹ò¿ª»ð");
			g_theGame.PlayGFX(RES_GFX_GUNFIRELIGHT, vPos, 0.4f);
		}
		else if (m_nId == ID_AI_GUERILLA)
		{
		}

		PlayAction(ACT_SQUAT_SHOT);
	}
	else if (iPose == POSE_RUN)
	{
		iPrecise = AttenuatePrecise(m_pRoute->GetAIMSPrecise(), vPlayerPos);

		if (m_nId == ID_AI_FOOTMAN)
		{
		}
		else if (m_nId == ID_AI_GUERILLA)
		{
			vPos = m_pImgModel->GetMarkPointPos("¶·óÒ_³å·æÖÐÉä»÷");
			g_theGame.PlayGFX(RES_GFX_GUNFIRELIGHT, vPos, 0.4f);
		}
	}

	g_theGame.Play3DSound(RES_S3D_AK47FIRE1+MLA_Random(0, 1), m_vPos, 20.0f, 100.0f, SND3DTYPE_AIGUNFIRE);
	
	//	Do damage to player
	if (MLA_PercentProbability(iPrecise))
	{
		int iDamage, iLevel = GETMLGAMEPLAY->GetCurrentMission();

		if (m_nId == ID_AI_FOOTMAN)
			iDamage = GETAIILDVALUE(ILD_DAMAGE_F, iLevel);
		else if (m_nId == ID_AI_GUERILLA)
			iDamage = GETAIILDVALUE(ILD_DAMAGE_G, iLevel);

		iDamage = AttenuateDamage(iDamage, vPlayerPos);

		GETMLGAMEPLAY->GetLocalPlayer()->Strike(iDamage);
	}
	else if (pConfig->bSoundMisc && MLA_PercentProbability(8))
	{
		if (MLA_PercentProbability(60))
			g_theGame.Play2DSound(RES_S2D_BULLETWHIZZ1+MLA_Random(0, 11), false);
		else
		{
			MLA_GenRandomVectorH(vPos);
			vPos = vPlayerPos + vPos * MLA_Random(2.0f, 10.0f);
			g_theGame.Play3DSound(RES_S3D_BULLETHITMUD1+MLA_Random(0, 20), vPos, 2.0f, 20.0f);
		}
	}
}

//	Rocket man fire
void CMLAIUnit::RocketManFire(int iPose)
{
	A3DVECTOR3 vStart, vDir, vTarget;
	CMLHelicopter* pCopter = GETMLGAMEPLAY->GetObjectMan()->SelectRandomCopter();
	bool bCopter = false;
	int iAction;

	//	Select attack target between helicopter and player
	if (pCopter && pCopter->CanBeHit())
	{
		float fDist = Magnitude(pCopter->GetPos() - m_vPos);
		if (fDist < GETGAMELOGICPARAMS->fAIShotCopterRadius &&
			MLA_PercentProbability(m_iAIAttackCopter))
			bCopter = true;
	}

	if (bCopter)	//	Select helicopter as target
	{
		vDir = Normalize(pCopter->GetPos() - m_vPos);

		//	Select AI pose
		A3DVECTOR3 vxzDir(vDir.x, 0.0f, vDir.z);
		vxzDir = Normalize(vxzDir);

		int iSide;
		GetDirDegree(vxzDir, &iSide, 0.7071f);
		if (iSide == 0)
		{
			if (iPose == POSE_STAND)
			{
				iAction = ACT_STAND_SHOT;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_Õ¾Á¢¿ª»ð");
			}
			else
			{
				iAction = ACT_SQUAT_SHOT;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_Õý");
			}
		}
		else if (iSide == 1)
		{
			if (vDir.y < 0.94f)	//	< 70 deg
			{
				iAction = ACT_SQUAT_SHOT_RIGHT45;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_ÓÒ");
			}
			else
			{
				iAction = ACT_SQUAT_SHOT_RIGHT80;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_ÓÒ");
			}
		}
		else
		{
			if (vDir.y < 0.94f)	//	< 70 deg
			{
				iAction = ACT_SQUAT_SHOT_LEFT45;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_×ó");
			}
			else
			{
				iAction = ACT_SQUAT_SHOT_LEFT80;
				vStart	= m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_×ó");
			}
		}

		int iPrecise = AttenuatePrecise(m_pRoute->GetHitCopterPrecise(), pCopter->GetPos());

		if (MLA_PercentProbability(iPrecise))	//	AI will hit copter
			pCopter->CalcHitPos(true, vStart, GETGAMELOGICPARAMS->fAIRocketSpeed, &vTarget);
		else	//	AI won't hit copter
			pCopter->CalcHitPos(false, vStart, GETGAMELOGICPARAMS->fAIRocketSpeed, &vTarget);
	}
	else	//	AI selected player as target
	{
		int iPrecise;

		if (iPose == POSE_STAND)
		{
			vStart	 = m_pImgModel->GetMarkPointPos("»ð¼ý_Õ¾Á¢¿ª»ð");
			iAction	 = ACT_STAND_SHOT;
			iPrecise = m_pRoute->GetAISSPrecise();	
		}
		else
		{
			vStart	 = m_pImgModel->GetMarkPointPos("»ð¼ý_¹ò¿ª»ð_Õý");
			iAction	 = ACT_SQUAT_SHOT;
			iPrecise = m_pRoute->GetAICSPrecise();	
		}

		CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
		iPrecise = AttenuatePrecise(iPrecise, pPlayer->GetPos());

		if (MLA_PercentProbability(iPrecise))	//	This rocket will hit player	
			vTarget = pPlayer->GetRandomHitPos();
		else	//	This rocket won't hit player
			vTarget = pPlayer->GetRandomMissPos(vStart, 0.4f);
	}

	PlayAction(iAction);

	vDir = Normalize(vTarget - vStart);

	//	Generate a rocket bullet
	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	pBulletMan->CreateBullet(ID_BULLET_ROCKET, vStart, vDir, GETGAMELOGICPARAMS->fAIRocketSpeed);

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;
	if (pConfig->bSoundMisc)
		g_theGame.Play3DSound(RES_S3D_ROCKETFIRE, vStart, 10.0f, 50.0f);

	//	Play rocket launch effect
	g_theGame.PlayGFX(RES_GFX_ROCKETLAUNCH, vStart, 1.0f);
}

//	Ray trace routine
bool CMLAIUnit::RayTrace(MLRAYTRACE* pTrace)
{
	if (!DoHitCheck())
		return false;

	bool bRet = false;

	if (m_bUseMoxModel)
	{
		RAYTRACE Trace;
		if (m_pMoxModel->RayTrace(pTrace->vStart, pTrace->vVel * pTrace->fTime, &Trace))
		{
			pTrace->iObjectType = MLTRACE_AI;
			pTrace->pObject		= (void*) this;

			pTrace->Trace.fFraction	= Trace.fFraction;
			pTrace->Trace.vPoint	= Trace.vPoint;

			bRet = true;
		}
	}
	else
	{
		A3DVECTOR3 vHitPos;
		float fFraction;

		if (m_pImgModel->RayTrace(pTrace->vStart, pTrace->vVel, pTrace->fTime,
								  &vHitPos, &fFraction))
		{
			pTrace->iObjectType = MLTRACE_AI;
			pTrace->pObject		= (void*) this;

			pTrace->Trace.fFraction	= fFraction;
			pTrace->Trace.vPoint	= vHitPos;

			bRet = true;
		}
	}

	return bRet;
}

/*	Explode.

	pBullet: exploding bullet
	bFire: true, only do fire effect
*/
bool CMLAIUnit::Explode(CMLBullet* pBullet, bool bFire)
{
	if (!DoHitCheck())
		return false;

	A3DVECTOR3 vDist = m_vPos - pBullet->GetPos();
	float fRadius2;
	
	if (!bFire)
		fRadius2 = pBullet->GetOuterRadius() * pBullet->GetOuterRadius();
	else
		fRadius2 = 5.0f * 5.0f;

	float fDist2 = vDist.x * vDist.x + vDist.y * vDist.y + vDist.z * vDist.z;
	if (fDist2 > fRadius2)
		return false;

	int iDamage = pBullet->GetDamage();
	if ((m_iLife -= iDamage) > 0)
		return false;

	fRadius2 = pBullet->GetInnerRadius() * pBullet->GetInnerRadius();

	bool bCatchFire = (pBullet->GetId() == ID_BULLET_FIREBALL && fDist2 > fRadius2) || bFire;
	if (bCatchFire && m_iState == STATE_CATCHFIRE)
		return false;

	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	//	AI is dying
	m_iLife = 0;
	m_iStrategy = STRATEGY_DYING;

	if (bCatchFire)
	{
		if (m_bUseMoxModel)
		{
			DoState_Dying(true);
			OnDie(0);
		}
		else
		{
			DoState_CatchFire(true);

			//	Play scream sounds
			if (pConfig->bSoundMisc && MLA_PercentProbability(30) && GetAIToPlayerDist() < 80.0f)
				g_theGame.Play3DSound(RES_S3D_AIBURNDEATH1+MLA_Random(0, 6), m_vPos);
		}
	}
	else	//	Cannon or mortar
	{
		m_vCurMoveDir   = m_vPos - pBullet->GetPos();
		m_vCurMoveDir.y = 0.0f;

		float fMag = Normalize(m_vCurMoveDir, m_vCurMoveDir);
		if (fMag == 0.0f && fMag > 1.01f)
			MLA_GenRandomVectorH(m_vCurMoveDir);

		float fSpeed	= MLA_Random(2.0f, 5.0f);
		float fVSpeed	= MLA_Random(8.0f, 12.0f);
	
		SetVel(m_vCurMoveDir * fSpeed + g_vAxisY * fVSpeed);

		DoState_DieFly(true);

		if (pConfig->bSoundMisc && MLA_PercentProbability(30) && GetAIToPlayerDist() < 80.0f)
			g_theGame.Play3DSound(RES_S3D_AIBLASTDEATH1+MLA_Random(0, 4), m_vPos);

		OnDie(1);
	}

	return true;
}

/*	Strike AI

	pBullet: bullet object that do strike AI
*/
void CMLAIUnit::Strike(CMLBullet* pBullet)
{
#ifdef _DEBUG

	switch (pBullet->GetId())
	{
	case ID_BULLET_PISTOL:
	case ID_BULLET_RIFLE:
	case ID_BULLET_MACHINE:	break;
	default:
		assert(0);
		break;
	}

#endif

	if (!DoHitCheck())
		return;

	int iDamage = pBullet->GetDamage();

	if ((m_iLife -= iDamage) > 0)
		return;

	//	AI is dying
	m_iLife = 0;
	m_iStrategy = STRATEGY_DYING;
	DoState_Dying(true);

	OnDie(0);
}

/*	On die

	iDieFrom: 0, gun kill
			  1, bomb kill
			  2, fire kill
*/
void CMLAIUnit::OnDie(int iDieFrom)
{
	USERCONFIGS* pConfig = GETCURUSERCONFIGS;

	if (iDieFrom == 0)
	{
		if (pConfig->bSoundMisc)
		{
			//	Play crying sounds
			if (MLA_PercentProbability(30) && GetAIToPlayerDist() < 60.0f)
				g_theGame.Play3DSound(RES_S3D_AIDEATH1+MLA_Random(0, 5), m_vPos);

			//	Bullet hit body sounds
			if (MLA_PercentProbability(40) && GetAIToPlayerDist() < 20.0f)
				g_theGame.Play3DSound(RES_S3D_BULLETHITBODY1+MLA_Random(0, 5), m_vPos);
		}
	}
	else if (iDieFrom == 1)
	{
		if (pConfig->bSoundMisc)
		{
			//	Play crying sounds
			if (MLA_PercentProbability(30) && GetAIToPlayerDist() < 60.0f)
				g_theGame.Play3DSound(RES_S3D_AIDEATH1+MLA_Random(0, 5), m_vPos);
		}
	}
	else if (iDieFrom == 2)
	{
	}

	int iScore = 0;
	switch (m_nId)
	{
	case ID_AI_FOOTMAN:		iScore = GETGAMELOGICPARAMS->iScoreFootman;		break;
	case ID_AI_GUERILLA:	iScore = GETGAMELOGICPARAMS->iScoreGuerilla;	break;
	case ID_AI_ROCKET:		iScore = GETGAMELOGICPARAMS->iScoreRocket;		break;
	}

	GETMLGAMEPLAY->AddUserScore(iScore);

	if (m_bInPreserve)
	{
		GETMLGAMEPLAY->GetMission()->IncStatistic(m_nId, -1, 2);
		m_bInPreserve = false;
	}
	
	INCSTATISTIC(m_nId, 1, 0);
}

//	Play action
void CMLAIUnit::PlayAction(int iAction)
{
	if (!m_bUseMoxModel)
	{
		m_iAction = iAction;
		return;
	}

	m_iAction = iAction;

	switch (iAction)
	{
	case ACT_CHARGE:			m_iAction = MOXACT_CHARGE;		break;
	case ACT_STAND_AIM:			m_iAction = MOXACT_STAND;		break;
	case ACT_DIE_BACKWARD0:		m_iAction = MOXACT_DIE0;		break;
	case ACT_DIE_BACKWARD1:		m_iAction = MOXACT_DIE1;		break;
	case ACT_DIE_BACKWARD2:		m_iAction = MOXACT_DIE2;		break;
	}
}

/*	Attenuate damage

	Return attenuated damage.

	iDamage: source damage.
	vDest: destination position
*/
int	CMLAIUnit::AttenuateDamage(int iDamage, A3DVECTOR3& vDest)
{
	float fNear = GETGAMELOGICPARAMS->fAIAttenuationNear;
	float fFar  = GETGAMELOGICPARAMS->fAIAttenuationFar;
	float fDist = Magnitude(vDest - m_vPos);

	if (fDist <= fNear)
		return iDamage;

	int iFactor;
	if (fDist >= fFar)
		iFactor = NUM_GAMELEVEL-1;
	else
		iFactor = (int)((NUM_GAMELEVEL-1) * (fDist - fNear) / (fFar - fNear));

	iDamage = (int)(iDamage * GETAIFLDVALUE(FLD_DAMAGE_DISTANCE, iFactor) / 100.0f + 0.5f);
	return iDamage;
}

/*	Attenuate precise

	Return attenuated precise.

	iPrecise: source precise.
	vDest: destination position
*/
int	CMLAIUnit::AttenuatePrecise(int iPrecise, A3DVECTOR3& vDest)
{
	float fNear = GETGAMELOGICPARAMS->fAIAttenuationNear;
	float fFar  = GETGAMELOGICPARAMS->fAIAttenuationFar;
	float fDist = Magnitude(vDest - m_vPos);

	if (fDist <= fNear)
		return iPrecise;

	int iFactor;
	if (fDist >= fFar)
		iFactor = NUM_GAMELEVEL-1;
	else
		iFactor = (int)((NUM_GAMELEVEL-1) * (fDist - fNear) / (fFar - fNear));

	iPrecise = (int)(iPrecise * GETAIFLDVALUE(FLD_PRECISE_DISTANCE, iFactor) / 100.0f + 0.5f);
	return iPrecise;
}

/*	Shift position

	fMin: minimum distance of shifting
	fMax: maximum distance of shifting
*/
void CMLAIUnit::Shift(float fMin, float fMax)
{
	A3DVECTOR3 vRight, vPlayerPos = GETMAPDATA->GetActorPos();
	float fRadius;
	
	vPlayerPos.y = 0.0f;

	GetAIToPlayerDir(&vRight, &fRadius);
	float fDist = MLA_RandomSign(fMin, fMax);
	
	m_vCurTarget	= m_vxzPos + vRight * fDist;
	m_vCurTarget	= vPlayerPos + Normalize(m_vCurTarget - vPlayerPos) * fRadius;
	m_vCurMoveDir	= Normalize(m_vCurTarget - m_vxzPos);

	if (fDist > 0.0f)
	{
		if (!m_bUseMoxModel)
			PlayAction(ACT_CHARGE_RIGHT90);
	}
	else
	{
		if (!m_bUseMoxModel)
			PlayAction(ACT_CHARGE_LEFT90);
	}
}

//	Get distance between AI and player
float CMLAIUnit::GetAIToPlayerDist()
{
	A3DVECTOR3 vPlayer = GETMAPDATA->GetActorPos();
	return Magnitude(m_vPos - vPlayer);
}

//	Do hit check ?
bool CMLAIUnit::DoHitCheck()
{
	if (!m_bDoTrace || m_iStrategy == STRATEGY_WAIT)
		return false;

	if (m_iStrategy == STRATEGY_DYING && m_iState != STATE_CATCHFIRE)
		return false;

	return true;
}

//	Move fire
void CMLAIUnit::MoveFireOnBody(bool bStand)
{
	if (!m_bCatchFire || !m_pGFX)
		return;

	A3DVECTOR3 vPos;

	if (bStand)
	{
		if (m_nId == ID_AI_GUERILLA)
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_Éí³¤");
		else
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_Éí³¤");
	}
	else
	{
		if (m_nId == ID_AI_GUERILLA)
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_ÉÕËÀ");
		else
			vPos = m_pImgModel->GetMarkPointPos("¸Ö¿ø_×Å»ð_ÉÕËÀ");
	}

	m_pGFX->SetPos(vPos);
}

//	Output debug information
void CMLAIUnit::OutputDebugInfo()
{
	ADEBUG_TRACE1("ID %d", m_nId);
	ADEBUG_TRACE3("Pos: %f, %f, %f", m_vPos.x, m_vPos.y, m_vPos.z);
	ADEBUG_TRACE3("Hori pos: %f, %f, %f", m_vxzPos.x, m_vxzPos.y, m_vxzPos.z);
	ADEBUG_TRACE3("Strategy: %d, State: %d, SubState: %d", m_iStrategy, m_iState, m_iSubState);
}


