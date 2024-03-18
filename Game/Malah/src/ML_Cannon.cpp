/*
 * FILE: ML_Cannon.cpp
 *
 * DESCRIPTION: Cannon class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Cannon.h"
#include "ML_Resources.h"
#include "ML_ManBullet.h"
#include "ML_ManObject.h"
#include "ML_GamePlay.h"

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

int CMLCannon::m_iViewWid = 200;
int CMLCannon::m_iViewHei = 150;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////

void _CannonLogicCallback(A3DMODEL_LOGIC_EVENT* pLogicEvent, LPVOID pArg)
{
	CMLCannon* pCannon = (CMLCannon*)pArg;

	if (!_stricmp(pLogicEvent->szNotifyString, "开火结束"))
		pCannon->EndOneFire();
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLCannon
//
///////////////////////////////////////////////////////////////////////////

CMLCannon::CMLCannon() : CMLModelUnit()
{
	m_pCamera	= NULL;
	m_pViewport	= NULL;
}

/*	Initialize object

	Return true for success, otherwise return false
*/
bool CMLCannon::Init(A3DVECTOR3& vBombPos)
{
	m_vBombPos = vBombPos;

	if (GETCURUSERCONFIGS->bVideo && 
		(m_pMoxModel = g_theGame.LoadA3DModel(RES_MOX_CANNON)))
	{
		m_pMoxModel->SetLogicEventCallBack(_CannonLogicCallback, (LPVOID)this);

		//	We should render all model GFX on our own
		AList* pGFXList = m_pMoxModel->GetGFXEventList();
		ALISTELEMENT* pGFXNode = pGFXList->GetFirst();
		while (pGFXNode != pGFXList->GetTail())
		{
			A3DMODEL_GFX_EVENT* pGFXEvent = (A3DMODEL_GFX_EVENT*)pGFXNode->pData;
			pGFXEvent->pGFX->SetDrawByMan(false);
			pGFXNode = pGFXNode->pNext;
		}

		m_pCamera	= GETMLGAMEPLAY->GetObjectMan()->GetCannonCamera();
		m_pViewport	= GETMLGAMEPLAY->GetObjectMan()->GetCannonViewport();
	}

	SetState(ST_BEFOREFIRE);

	return true;
}

//	Release resources
bool CMLCannon::Release()
{
	m_pCamera	= NULL;
	m_pViewport	= NULL;

	return CMLModelUnit::Release();
}

//	Set state
void CMLCannon::SetState(int iState)
{
	m_iState = iState;

	switch (iState)
	{
	case ST_BEFOREFIRE:

		m_iWaitTime		= GETGAMELOGICPARAMS->iCannonArriveTime * 1000;
		m_iWaitCnt		= 0;
		m_bRender		= false;

		PlayModelAction("[停]", true);
		break;

	case ST_FIRE:

		m_iFireTimes	= GETGAMELOGICPARAMS->iCannonFireTimes;
		m_iFireCnt		= 0;
		m_iWaitTime		= m_iFireTimes * 1500;
		m_iWaitCnt		= 0;
		m_bRender		= true;

		PlayModelAction("[开火]", true);
		g_theGame.Play2DSound(RES_S2D_CANNONFIRE, false);
		break;

	case ST_AFTERFIRE:

		m_iWaitTime		= 1000;
		m_iWaitCnt		= 0;
		break;
	}
}

//	Logic tick
bool CMLCannon::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	int iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);

	switch (m_iState)
	{
	case ST_BEFOREFIRE:
	{
		m_iWaitCnt += iTimeSpan;
		if (m_iWaitCnt < m_iWaitTime)
			return true;

		SetState(ST_FIRE);
		break;
	}
	case ST_FIRE:
	{
		if (m_pMoxModel)
		{
			if (m_iFireCnt >= m_iFireTimes)
				SetState(ST_AFTERFIRE);
		}
		else
		{
			m_iWaitCnt += iTimeSpan;
			if (m_iWaitCnt >= m_iWaitTime)
				SetState(ST_AFTERFIRE);
		}

		break;
	}
	case ST_AFTERFIRE:
	{
		m_iWaitCnt += iTimeSpan;
		if (m_iWaitCnt < m_iWaitTime)
			return true;

		m_bDead = true;
		break;
	}
	}

	if (m_pMoxModel)
		m_pMoxModel->UpdateModelOBB();

	return true;
}

//	Render routine
bool CMLCannon::Render(A3DViewport* pViewport)
{
	if (!m_pMoxModel || !m_pViewport)
		return true;

	//	Use our own viewport and camera
	m_pViewport->Active();
	m_pViewport->ClearDevice();

	m_pMoxModel->Render(m_pViewport);

	//	We should render all model GFX on our own
	AList* pGFXList = m_pMoxModel->GetGFXList();
	ALISTELEMENT* pGFXNode = pGFXList->GetFirst();
	while (pGFXNode != pGFXList->GetTail())
	{
		A3DMODEL_GFX_RECORD* pGFXRecord = (A3DMODEL_GFX_RECORD*)pGFXNode->pData;
		pGFXRecord->pGFX->Render(m_pViewport);
		pGFXNode = pGFXNode->pNext;
	}

	g_theGame.GetA3DEngine()->FlushVertexCollector(m_pViewport);

	//	Restore old viewport
	pViewport->Active();

	return true;
}

//	End one fire
void CMLCannon::EndOneFire()
{
	if (++m_iFireCnt < m_iFireTimes && m_pMoxModel)
	{
		PlayModelAction("[开火]", true);
		g_theGame.Play2DSound(RES_S2D_CANNONFIRE, false);
	}

	CreateBombs(GETGAMELOGICPARAMS->iNumCannonBomb);
}

/*	Create bombs

	iNum: number of bombs will be generated
*/
void CMLCannon::CreateBombs(int iNum)
{
	int i;
	A3DVECTOR3 vPos;
	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	CMLLogicWorld* pWorld = GETMLGAMEPLAY->GetLogicWorld();

	for (i=0; i < iNum; i++)
	{
		MLA_GenRandomVectorH(vPos);
		vPos	= m_vBombPos + vPos * MLA_Random(0.0f, GETGAMELOGICPARAMS->fCannonFallRadius);
		vPos.y	= pWorld->GetHeightOfPos(vPos);
		pBulletMan->CreateBullet(ID_BULLET_CANNON, vPos, g_vOrigin, 0.0f);
	}
}

