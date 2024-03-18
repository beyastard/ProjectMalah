/*
 * FILE: ML_ManAI.cpp
 *
 * DESCRIPTION: AI manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/31
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManAI.h"
#include "ML_ManagerDef.h"
#include "ML_Resources.h"
#include "ML_Route.h"
#include "ML_GamePlay.h"
#include "ML_Player.h"
#include "ML_EnemyPlane.h"
#include "ML_EnemyVehicle.h"
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
//	Implement CMLManAI
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManAI, CMLManBase, 1)

CMLManAI::CMLManAI() : CMLManBase()
{
	m_managerId		= MANID_AI;
	m_pImageMan		= NULL;
	m_iIMGroupAI	= -1;
	m_dwTimeCnt		= 0;
	m_iMIG21Time	= 0;
	m_iMIG21Num		= 0;
	m_iFantanTime	= 0;
	m_iFantanNum	= 0;
	m_dwImageColor	= 0xffffffff;
	m_iNumPlane		= 0;

	memset(m_aDefModels, 0, sizeof (m_aDefModels));
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManAI::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	m_pImageMan	= m_pAGame->GetA3DEngine()->GetA3DImageModelMan();

	return true;
}

//	Release object
bool CMLManAI::Release()
{
	m_pImageMan	= NULL;

	return CMLManBase::Release();
}

//	Message handler
int	CMLManAI::OnMsg(AMSG* pMSG)
{
	MLMSG* pMLMsg = (MLMSG*)pMSG;

	if (pMLMsg->idObject == m_managerId)
		return ProcessMsg(pMLMsg) ? 1 : 0;

	return 1;
}

/*	Handle a message

	Return true for success, otherwise return false.

	pMsg: message will be handled
*/
bool CMLManAI::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

//	Initialize object pools
bool CMLManAI::InitObjectPools()
{
	for (int i=0; i < NUM_AI_ID; i++)
	{
		if (!m_aAIPool[i].Init())
		{
			ADEBUG_TRACE("Failed to initialize AI allocated pool");
			return false;
		}
	}

	return true;
}

//	Release all object pools
void CMLManAI::ReleaseObjectPools()
{
	for (int i=0; i < NUM_AI_ID; i++)
		m_aAIPool[i].Release();
}

//	Release all AI
void CMLManAI::ReleaseAllAI()
{
	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->Release();
		GetAIPool(pAI->GetId())->FreeObject(pAI);
	}

	m_AIList.RemoveAll();

	//	Check object leak
	for (int i=0; i < NUM_AI_ID; i++)
	{
		if (m_aAIPool[i].CheckObjectLeak() > 0)
			assert(0);
	}

	//	Release all enemy weapon
	pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		pObject->Release();
		delete pObject;
	}

	m_EWList.RemoveAll();

	m_iNumPlane	= 0;
}

//	Load AI image models
bool CMLManAI::LoadAIImageModels()
{
	//	Load AI image model groups
	if ((m_iIMGroupAI = m_pImageMan->LoadModelGroup((char*)RES_GetOtherFile(RES_IM_AIGROUPFILE))) < 0)
	{
		ADEBUG_TRACE("Failed to call load iamge model group");
		return false;
	}

	//	Get default image models
	m_aDefModels[IDX_FOOTMAN] = m_pImageMan->CreateImageModel(m_iIMGroupAI, (char*)RES_GetName(RES_IM_FOOTMAN));
	m_aDefModels[IDX_GUERILLA] = m_pImageMan->CreateImageModel(m_iIMGroupAI, (char*)RES_GetName(RES_IM_GUERILLA));
	m_aDefModels[IDX_ROCKET] = m_pImageMan->CreateImageModel(m_iIMGroupAI, (char*)RES_GetName(RES_IM_ROCKETMAN));

	return true;
}

//	Release AI image models
void CMLManAI::ReleaseAIImageModels()
{
	//	Release AI image model groups
	if (m_iIMGroupAI >= 0)
	{
		m_pImageMan->ReleaseModelGroup(m_iIMGroupAI);
		m_iIMGroupAI = -1;

		for (int i=0; i < NUM_AI_ID; i++)
		{
			if (m_aDefModels[i])
			{
				m_pImageMan->ReleaseImageModel(m_aDefModels[i]);
				m_aDefModels[i] = NULL;
			}
		}
	}
}

//	Called when begin game playing
bool CMLManAI::OnStartGamePlay()
{
	if (!InitObjectPools())
	{
		ADEBUG_TRACE("Failed to initialize AI allocated pool");
		return false;
	}

	if (!LoadAIImageModels())
	{
		ADEBUG_TRACE("Failed to initialize AI iamge models");
		return false;
	}

	return true;
}

//	Called when end game playing
bool CMLManAI::OnEndGamePlay()
{
	ReleaseAIImageModels();
	ReleaseObjectPools();

	return true;
}

//	Called when being mission
bool CMLManAI::OnBeginMission(int iMission)
{
	//	Calculate image color
	CalcImageColor();

	ResetMissionData();
	return true;
}

//	Called when reset mission
bool CMLManAI::OnResetMission()
{
	ResetMissionData();
	return true;
}

//	Reset mission data
void CMLManAI::ResetMissionData()
{
	m_dwTimeCnt		= 0;
	m_iMIG21Time	= 0;
	m_iMIG21Num		= 0;
	m_iFantanTime	= 0;
	m_iFantanNum	= 0;
	m_iNumPlane		= 0;
	m_iNumAIDead	= 0;
	m_iNumAIGen		= 0;
}

//	Called when end mission
bool CMLManAI::OnEndMission()
{
	//	Release all AI
	ReleaseAllAI();

	return true;
}

/*	Create a AI on route

	Return true for success, otherwise return false.

	pRoute: pointer of route on which AI generate
*/
bool CMLManAI::CreateAI(CMLRoute* pRoute)
{
	assert(ISAIID(pRoute->GetUserID()));

	bool bNewObj;
	int iAIIndex = pRoute->GetUserID() - ID_FIRSTAI;

	//	Allocate AI object
	CMLAIUnit* pAI = m_aAIPool[iAIIndex].AllocObject(&bNewObj);
	if (!pAI)
	{
		ADEBUG_TRACE("Not enough memory");
		return false;
	}

	if (!pAI->Init(bNewObj, ID_FIRSTAI + iAIIndex, pRoute, m_dwImageColor))
	{
		m_aAIPool[iAIIndex].FreeObject(pAI);
		ADEBUG_TRACE("Failed to initialize AI object");
		return false;
	}

	//	Set camera for image model
	A3DCamera* pCamera = GETMLGAMEPLAY->GetLocalPlayer()->GetCamera();
	pAI->SetCamera(pCamera);

	//	Add AI to list
	m_AIList.AddTail(pAI);

	m_iNumAIGen++;

	return true;
}

//	Logic tick
bool CMLManAI::LogicRun(float fTimeSpan)
{
	BeginLogicCount();

	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	AListPosition thePos, pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pAI->IsDead())
		{
			pAI->Release();
			GetAIPool(pAI->GetId())->FreeObject(pAI);
			m_AIList.RemoveAt(thePos);
			m_iNumAIDead++;
		}
	}

	//	Generate planes if their time has come
//	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
//	if (m_dwTimeCnt < GETAIILDVALUE(ILD_TIME_AIGEN, iLevel) * 1000)
		GeneratePlanes(fTimeSpan);

	//	Enemy weapons
	pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		pObject->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pObject->IsDead())
		{
			if (pObject->GetId() == ID_EW_FANTAN ||
				pObject->GetId() == ID_EW_MIG21)
				m_iNumPlane--;

			pObject->Release();
			delete pObject;
			m_EWList.RemoveAt(thePos);
		}
	}

	EndLogicCount();

	return true;
}

//	Tick animation
bool CMLManAI::TickAnimation()
{
	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->TickAnimation();
	}

	//	Enemy weapons
	pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		pObject->TickAnimation();
	}

	return true;
}

//	Render routines
bool CMLManAI::Render(A3DViewport* pViewport)
{
	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->Render(pViewport);
	}

	//	Enemy weapons
	pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		if (pObject->CanRender())
			pObject->Render(pViewport);
	}

	return true;
}

//	Update AI actions
void CMLManAI::UpdateActions()
{
	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->UpdateAction();
	}
}

//	Ray trace routine
bool CMLManAI::RayTrace(MLRAYTRACE* pTrace)
{
	MLRAYTRACE Trace;
	Trace.vStart	= pTrace->vStart;
	Trace.vVel		= pTrace->vVel;
	Trace.fTime		= pTrace->fTime;

	float fMinFrac = 1.0f;

	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);

		if (pAI->RayTrace(&Trace) && Trace.Trace.fFraction < fMinFrac)
		{
			fMinFrac = Trace.Trace.fFraction;
			*pTrace	 = Trace;
		}
	}

	if (fMinFrac < 1.0f)
		return true;

	return false;
}

//	Bullet explode
bool CMLManAI::Explode(CMLBullet* pBullet, bool bFire)
{
	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		pAI->Explode(pBullet, bFire);
	}

	pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObj = m_AIList.GetNext(pos);
		if (pObj->GetId() >= ID_EW_TRUCK)
			((CMLEnemyVehicle*)pObj)->Explode(pBullet, bFire);
	}

	return true;
}

//	Generate planes
void CMLManAI::GeneratePlanes(float fTimeSpan)
{
	//	If there are friend planes in battle, enemy planes should
	//	avoid appearing
	CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
	if (pObjectMan->GetFriendPlaneNum())
		return;

	int iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);
	int iLevel = GETMLGAMEPLAY->GetCurrentMission();

	m_iMIG21Time	+= iTimeSpan;
	m_iFantanTime	+= iTimeSpan;

//	if (m_iMIG21Num < GETAIILDVALUE(ILD_MIG21_NUMBER, iLevel) &&
	if (m_iMIG21Time >= GETAIILDVALUE(ILD_MIG21_INTERVAL, iLevel) * 1000)
	{
		A3DVECTOR3 vDir;
		MLA_GenRandomVectorH(vDir);

		CreateEnemyPlane(ID_EW_MIG21, vDir);

		m_iMIG21Num++;
		m_iMIG21Time = 0;
	}

//	if (m_iFantanNum < GETAIILDVALUE(ILD_FANTAN_NUMBER, iLevel) &&
	if (m_iFantanTime >= GETAIILDVALUE(ILD_FANTAN_INTERVAL, iLevel) * 1000)
	{
		A3DVECTOR3 vDir;
		MLA_GenRandomVectorH(vDir);

		CreateEnemyPlane(ID_EW_FANTAN, vDir);

		m_iFantanNum++;
		m_iFantanTime = 0;
	}
}

//	Create a enemy plane
bool CMLManAI::CreateEnemyPlane(int iID, A3DVECTOR3& vDir)
{
	CMLEnemyPlane* pPlane = new CMLEnemyPlane;

	if (!pPlane || !pPlane->Init(iID, vDir))
	{
		ADEBUG_TRACE("Failed to create enemy plane");
		return false;
	}

	m_EWList.AddTail(pPlane);

	m_iNumPlane++;

	return true;
}

//	Create a enemy vehicle
bool CMLManAI::CreateEnemyVehicle(int iID, CMLRoute* pRoute, int iWaitTime)
{
	CMLEnemyVehicle* pVehicle = new CMLEnemyVehicle;

	if (!pVehicle || !pVehicle->Init(iID, pRoute, iWaitTime))
	{
		ADEBUG_TRACE("Failed to create enemy vehicle");
		return false;
	}

	m_EWList.AddTail(pVehicle);

	return true;
}

//	Enemy plane ray trace routine
bool CMLManAI::EnemyWeaponRayTrace(MLRAYTRACE* pTrace)
{
	MLRAYTRACE Trace;
	Trace.vStart	= pTrace->vStart;
	Trace.vVel		= pTrace->vVel;
	Trace.fTime		= pTrace->fTime;

	float fMinFrac = 1.0f;

	AListPosition pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		if (!ISEWID(pObject->GetId()))
			continue;

		bool bRet = false;
		if (pObject->GetId() <= ID_EW_FANTAN)
			bRet = ((CMLEnemyPlane*)pObject)->RayTrace(&Trace);
		else
			bRet = ((CMLEnemyVehicle*)pObject)->RayTrace(&Trace);

		if (bRet &&	Trace.Trace.fFraction < fMinFrac)
		{
			fMinFrac = Trace.Trace.fFraction;
			*pTrace	 = Trace;
		}
	}

	if (fMinFrac < 1.0f)
		return true;

	return false;
}

//	Adjust image model's brightness according to environment
void CMLManAI::CalcImageColor()
{
	DWORD dwCol = GETMLGAMEPLAY->GetLogicWorld()->GetAmbient();
	int r = A3DCOLOR_GETRED(dwCol);
	int g = A3DCOLOR_GETGREEN(dwCol);
	int b = A3DCOLOR_GETBLUE(dwCol);

	dwCol = GETMLGAMEPLAY->GetLogicWorld()->GetDiffuse();
	r += A3DCOLOR_GETRED(dwCol) >> 1;
	g += A3DCOLOR_GETGREEN(dwCol) >> 1;
	b += A3DCOLOR_GETBLUE(dwCol) >> 1;

	CLAMPMAXVALUE(r, 255);
	CLAMPMAXVALUE(g, 255);
	CLAMPMAXVALUE(b, 255);

	m_dwImageColor = A3DCOLORRGB(r, g, b);
}

//	Pause sound
void CMLManAI::PauseSound(bool bPause)
{
	AListPosition pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		pObject->PauseSound(bPause);
	}
}

//	Stop sound
void CMLManAI::StopSound()
{
	AListPosition pos = m_EWList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_EWList.GetNext(pos);
		pObject->StopSound();
	}
}

//	Output debug information
void CMLManAI::OutputDebugInfo()
{
	int iNumAI = m_AIList.GetCount();
	ADEBUG_TRACE1("Current AI num: %d", iNumAI);

	iNumAI = 1;

	AListPosition pos = m_AIList.GetHeadPosition();
	while (pos)
	{
		CMLAIUnit* pAI = (CMLAIUnit*)m_AIList.GetNext(pos);
		ADEBUG_TRACE1("------------- AI %d info:", iNumAI++);
		pAI->OutputDebugInfo();
	}
}


