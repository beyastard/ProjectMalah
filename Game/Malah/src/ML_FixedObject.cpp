/*
 * FILE: ML_FixedObject.cpp
 *
 * DESCRIPTION: Fixed object class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_FixedObject.h"
#include "ML_GamePlay.h"
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
//	Implement CMLFixedObject
//
///////////////////////////////////////////////////////////////////////////

CMLFixedObject::CMLFixedObject() : CMLModelUnit()
{
	m_iLife		= 0;
	m_iState	= ST_NORMAL;
	m_iWaitTime	= 0;
	m_iWaitCnt	= 0;
	m_bCollect	= true;

	SetId(ID_FIXEDOBJECT);
}

bool CMLFixedObject::Init()
{
	return true;
}

//	Release resources
bool CMLFixedObject::Release()
{
	return CMLModelUnit::Release();
}

//	Release current model
void CMLFixedObject::ReleaseCurModel()
{
	if (m_pMoxModel)
	{
		g_theGame.ReleaseA3DModel(m_pMoxModel);
		m_pMoxModel = NULL;
	}
}

//	Load data
bool CMLFixedObject::Load(AArchive& ar)
{
	if (!m_MLFixedObj.Load(ar))
	{
		ADEBUG_TRACE("Failed to load fixed object data !");
		return false;
	}

	Reset();

	return true;
}

//	Save data
bool CMLFixedObject::Save(AArchive& ar)
{
	if (!m_MLFixedObj.Save(ar))
	{
		ADEBUG_TRACE("Failed to save fixed object data");
		return false;
	}

	return true;
}

//	Set state
void CMLFixedObject::SetState(int iState)
{
	m_iState = iState;
}

//	Logic tick
bool CMLFixedObject::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	if (m_bDead)
		return true;

	int iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);

	switch (m_iState)
	{
	case ST_NORMAL:

		break;

	case ST_DESTROYED:

		if (m_MLFixedObj.m_Data.iRuinTime >= 0)
		{
			m_iWaitCnt += iTimeSpan;
			if (m_iWaitCnt >= m_iWaitTime)
				m_bDead = true;
		}

		break;
	}

	return true;
}

//	Reset object
void CMLFixedObject::Reset()
{
	ReleaseCurModel();

	//	Load model
	if ((m_pMoxModel = g_theGame.LoadA3DModel(m_MLFixedObj.m_szModelFile)))
	{
		SetPos(m_MLFixedObj.m_Data.vPos);
		m_pMoxModel->SetDirAndUp(m_MLFixedObj.m_Data.vDir, m_MLFixedObj.m_Data.vUp);

		if (m_pMoxModel->FindGroupActionByName("[播放动作]"))
			PlayModelAction("[播放动作]", true);
		
		m_pMoxModel->SetAutoAABBEnable(true);
	}

	m_iLife		= m_MLFixedObj.m_Data.iLife;
	m_bDoTrace	= CanBeDestroy();
	m_bRender	= true;
	m_bDead		= false;

	SetState(ST_NORMAL);
}

//	Ray trace function
bool CMLFixedObject::RayTrace(MLRAYTRACE* pTrace)
{
	if (!m_bDoTrace)
		return false;

	RAYTRACE Trace;
	if (m_pMoxModel->RayTrace(pTrace->vStart, pTrace->vVel * pTrace->fTime, &Trace))
	{
		pTrace->iObjectType = MLTRACE_FIXEDOBJ;
		pTrace->pObject		= (void*) this;
		
		pTrace->Trace.fFraction	= Trace.fFraction;
		pTrace->Trace.vPoint	= Trace.vPoint;

		return true;
	}

	return false;
}

//	Explode
bool CMLFixedObject::Explode(CMLBullet* pBullet, bool bFire)
{
	if (!m_bDoTrace)
		return false;

	A3DVECTOR3 vDist = m_vPos - pBullet->GetPos();
	float fRadius2 = pBullet->GetOuterRadius() * pBullet->GetOuterRadius();

	float fDist2 = vDist.x * vDist.x + vDist.y * vDist.y + vDist.z * vDist.z;
	if (fDist2 > fRadius2)
		return false;

	Strike(pBullet);

	return true;
}

//	Strike
void CMLFixedObject::Strike(CMLBullet* pBullet)
{
	if (!CanBeDestroy() || m_iLife < 0 || m_iState == ST_DESTROYED)
		return;

	int iDamage = pBullet->GetDamage();

	if ((m_iLife -= iDamage) > 0)
		return;

	m_iLife		= 0;

	m_bDoTrace	= m_MLFixedObj.m_Data.iDestroyed == FO_RUIN_ONLYSHOW ? false : true;
	m_iWaitTime	= m_MLFixedObj.m_Data.iRuinTime;
	m_iWaitCnt	= 0;

	ReleaseCurModel();

	if (strstr(m_MLFixedObj.m_szRuinFile, ".gfx"))
	{
		//	Play GFX
		g_theGame.PlayGFX(m_MLFixedObj.m_szRuinFile, m_vPos, 1.0f);
	}
	else if (strstr(m_MLFixedObj.m_szRuinFile, ".mod"))
	{
		//	Load ruin model
		if ((m_pMoxModel = g_theGame.LoadA3DModel(m_MLFixedObj.m_szRuinFile)))
		{
			SetPos(m_MLFixedObj.m_Data.vPos);
			m_pMoxModel->SetDirAndUp(m_MLFixedObj.m_Data.vDir, m_MLFixedObj.m_Data.vUp);
		}
	}

	SetState(ST_DESTROYED);
}

