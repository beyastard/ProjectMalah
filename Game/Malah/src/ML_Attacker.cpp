/*
 * FILE: ML_Attacker.cpp
 *
 * DESCRIPTION: Attacker class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_Attacker.h"
#include "ML_ID.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
#include "ML_Player.h"
#include "ML_ManBullet.h"

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
//	Implement CMLAttacker
//
///////////////////////////////////////////////////////////////////////////

CMLAttacker::CMLAttacker() : CMLModelUnit()
{
	m_nId = ID_SUPP_ATTACKER;

	m_iState	= ST_NONE;
	m_iBombCnt	= 0;
	m_bDropping	= false;
}

/*	Initialize object

	Return true for success, otherwise return false.

	vAttackPos: attack poistion on which attacker will drop fireballs
*/
bool CMLAttacker::Init(A3DVECTOR3 vAttackPos)
{
	m_vAttackPos = A3DVECTOR3(vAttackPos.x, 0.0f, vAttackPos.z);

	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	A3DVECTOR3 vxzPlayer = pPlayer->GetPos();
	vxzPlayer.y = 0.0f;

	A3DVECTOR3 vRight = Normalize(m_vAttackPos - vxzPlayer);
	m_vAttackDir = CrossProduct(vRight, g_vAxisY);

	//	Load models
	if ((m_pMoxModel = g_theGame.LoadA3DModel(RES_MOX_ATTACKER)) != NULL)
	{
		m_pMoxModel->SetDirAndUp(m_vAttackDir, g_vAxisY);
		m_pMoxModel->PlayActionByName("[Í£Ö¹]", true);
	}

	SetState(ST_WAIT);

	return true;
}

//	Release resources
bool CMLAttacker::Release()
{
	m_iBombCnt	= 0;
	m_bDropping	= false;

	return CMLModelUnit::Release();
}

//	Set position
void CMLAttacker::SetPos(A3DVECTOR3& vPos)
{
	CMLModelUnit::SetPos(vPos);
	m_vxzPos = A3DVECTOR3(vPos.x, 0.0f, vPos.z);
}

//	Set state
void CMLAttacker::SetState(int iState)
{
	m_iState = iState;

	A3DVECTOR3 vStart, vEnd, vVel;

	switch (iState)
	{
	case ST_WAIT:
	{
		float fTime = 1000.0f / GETGAMELOGICPARAMS->fAttackerSpeed;	//	fly time
		if (fTime < GETGAMELOGICPARAMS->iAttackerArriveTime)
			fTime = GETGAMELOGICPARAMS->iAttackerArriveTime - fTime;
		else
			fTime = 0.0f;

		//	Use m_fDistance and m_fDistCnt as time
		m_fDistance	= fTime;
		m_fDistCnt	= 0.0f;
		m_bRender	= false;
		break;
	}
	case ST_APPEAR:
	{
		m_bRender	= true;
		vStart		= m_vAttackPos - m_vAttackDir * 1000.0f;
		vEnd		= m_vAttackPos - m_vAttackDir * 800.0f;
		vStart.y	= GETGAMELOGICPARAMS->fAttackerHeight;
		vEnd.y		= GETGAMELOGICPARAMS->fAttackerHeight;
		m_fDistance	= Normalize(vEnd - vStart, vVel);
		m_fDistCnt	= 0.0f;
		vVel		= vVel * GETGAMELOGICPARAMS->fAttackerSpeed;

		SetPos(vStart);
		SetVel(vVel);

		if (m_pMoxModel)
		{
			PlayModelAction("[·ÉÐÐ]", true);
			m_pMoxModel->SetVelocity(vVel);
		}
		
		break;
	}
	case ST_NORMALFLY:
	{
		vStart		= m_vPos;
		vEnd		= m_vAttackPos + m_vAttackDir * 800.0f;
		vEnd.y		= GETGAMELOGICPARAMS->fAttackerHeight;
		m_fDistance	= Normalize(vEnd - vStart, vVel);
		m_fDistCnt	= 0.0f;
		vVel		= vVel * GETGAMELOGICPARAMS->fAttackerSpeed;

		SetPos(vStart);
		SetVel(vVel);

		//	Calculate drop bomb time
		CancluateDropTime(m_vxzPos);
		break;
	}
	case ST_DISAPPEAR:
	{
		vStart		= m_vPos;
		vEnd		= m_vAttackPos + m_vAttackDir * 1000.0f;
		vEnd.y		= GETGAMELOGICPARAMS->fAttackerHeight;
		m_fDistance	= Normalize(vEnd - vStart, vVel);
		m_fDistCnt	= 0.0f;
		vVel		= vVel * GETGAMELOGICPARAMS->fAttackerSpeed;

		SetPos(vStart);
		SetVel(vVel);
		break;
	}
	}
}

//	Logic tick
bool CMLAttacker::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	float fTickTime = fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta;

	if (m_iState == ST_WAIT)
		m_fDistCnt += fTickTime;
	else
	{
		vDelta = m_vVel * fTickTime;
		m_fDistCnt += m_fSpeed * fTickTime;
	}

	if (m_fDistCnt >= m_fDistance)
	{
		switch (m_iState)
		{
		case ST_WAIT:		SetState(ST_APPEAR);		break;
		case ST_APPEAR:		SetState(ST_NORMALFLY);		break;
		case ST_NORMALFLY:	SetState(ST_DISAPPEAR);		break;
		case ST_DISAPPEAR:	m_bDead = true;				break;
		}
	}
	else	
		SetPos(m_vPos + vDelta);

	if (m_pMoxModel)
		m_pMoxModel->UpdateModelOBB();

	//	Drop fire bombs
	if (m_iState == ST_NORMALFLY)
	{
		m_fDropTimeCnt += fTickTime;
		if (!m_bDropping)
		{
			if (m_fDropTimeCnt < m_fDropTime)
				return true;

			m_bDropping	= true;
			m_fDropTimeCnt = m_fDropTimeCnt - m_fDropTime + m_fDropInter;
		}

		if (m_fDropTimeCnt >= m_fDropInter && 
			m_iBombCnt < GETGAMELOGICPARAMS->iFireballNum)
		{
			A3DVECTOR3 vPos = m_vDropPos + m_vDropDist * m_iBombCnt;
			vPos.y = GETGAMELOGICPARAMS->fAttackerHeight;

			//	Drop a fireball
			CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
			CMLBullet* pBullet = pBulletMan->CreateBullet(ID_BULLET_FIREBALL, vPos, g_vOrigin, 0.0f);
			if (!m_iBombCnt && pBullet)
				pBullet->SetFlag(1);

			m_fDropTimeCnt -= m_fDropInter;
			if (m_fDropTimeCnt < 0.0f)
				m_fDropTimeCnt = 0.0f;

			m_iBombCnt++;
		}
	}

	return true;
}

//	Calculate drop bomb time
void CMLAttacker::CancluateDropTime(A3DVECTOR3& vCurPos)
{
	A3DVECTOR3 vDist = m_vAttackPos - vCurPos;
	float fDist = Normalize(vDist, vDist);
	float fLen	= (float)GETGAMELOGICPARAMS->iFireballBombLen;

	m_fDropTime		= (fDist - fLen * 0.5f) / GETGAMELOGICPARAMS->fAttackerSpeed;
	m_fDropTimeCnt	= 0.0f;
	m_fDropInter	= (fLen / GETGAMELOGICPARAMS->fAttackerSpeed) / GETGAMELOGICPARAMS->iFireballNum;
	m_iBombCnt		= 0;
	m_bDropping		= false;
	m_vDropPos		= m_vAttackPos - vDist * (fLen * 0.5f);
	m_vDropDist		= vDist * (fLen / GETGAMELOGICPARAMS->iFireballNum);
}

