/*
 * FILE: ML_ManBullet.cpp
 *
 * DESCRIPTION: Bullet manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/6
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManBullet.h"
#include "ML_ManagerDef.h"
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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLManBullet
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManBullet, CMLManBase, 1)

CMLManBullet::CMLManBullet() : CMLManBase()
{
	m_managerId = MANID_BULLET;

	m_dwTimeCnt	= 0;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManBullet::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	return true;
}

//	Release object
bool CMLManBullet::Release()
{
	return CMLManBase::Release();
}

/*	Create bullet

	Return bullet object for success, otherwise return NULL

	iBulletID: bullet ID.
	vStart: bullet's start position
	vDir: bullet's direction
	fSpeed: bullet's speed
*/
CMLBullet* CMLManBullet::CreateBullet(int iBulletID, A3DVECTOR3& vStart, A3DVECTOR3& vDir, float fSpeed)
{
	assert(ISBULLETID(iBulletID));

	CMLBullet* pBullet = m_BulletPool.AllocObject();
	if (!pBullet)
	{
		ADEBUG_TRACE("Not enough memory");
		return NULL;
	}

	if (!pBullet->Init(iBulletID, vStart, vDir, fSpeed))
	{
		m_BulletPool.FreeObject(pBullet);
		ADEBUG_TRACE("Failed to initialize AI object");
		return NULL;
	}

	int iLevel = GETMLGAMEPLAY->GetCurrentMission();
	int iDamage = 0;

	//	Get damage
	switch (iBulletID)
	{
	case ID_BULLET_PISTOL:
	case ID_BULLET_RIFLE:
	case ID_BULLET_MORTAR:
	case ID_BULLET_MACHINE:

		iDamage = GETPLAYERILDVALUE(ILD_DAMAGE_PISTOL + iBulletID - ID_BULLET_PISTOL, iLevel);
		break;

	case ID_BULLET_FIREBALL:

		iDamage = GETPLAYERILDVALUE(ILD_DAMAGE_FIREBOMB, iLevel);
		break;

	case ID_BULLET_CANNON:

		iDamage = GETPLAYERILDVALUE(ILD_DAMAGE_CANNON, iLevel);
		break;

	case ID_BULLET_ROCKET:

		iDamage = GETAIILDVALUE(ILD_DAMAGE_R, iLevel);
		break;

	case ID_BULLET_MIG21:

		iDamage = MLA_Random(GETGAMELOGICPARAMS->iMIG21MinDamage, GETGAMELOGICPARAMS->iMIG21MaxDamage);
		break;

	case ID_BULLET_FANTAN:

		iDamage = GETGAMELOGICPARAMS->iFantanDamage;
		break;

	case ID_BULLET_ARMORED:

		iDamage = GETGAMELOGICPARAMS->iArmoredDamage;
		break;

	case ID_BULLET_RPG:

		iDamage = GETPLAYERILDVALUE(ILD_DAMAGE_RPG, iLevel);
		break;
	}

	pBullet->SetDamage(iDamage);

	m_BulletList.AddTail(pBullet);

	return pBullet;
}

//	Release all bullets
void CMLManBullet::ReleaseAllBullets()
{
	AListPosition pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		CMLBullet* pBullet = (CMLBullet*)m_BulletList.GetNext(pos);
		pBullet->Release();
		m_BulletPool.FreeObject(pBullet);
	}

	m_BulletList.RemoveAll();

	//	Check object leak
	if (m_BulletPool.CheckObjectLeak() > 0)
		assert(0);
}

//	Message handler
int	CMLManBullet::OnMsg(AMSG* pMSG)
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
bool CMLManBullet::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

//	Called when begin game playing
bool CMLManBullet::OnStartGamePlay()
{
	if (!m_BulletPool.Init())
	{
		ADEBUG_TRACE("Failed to initialize bullet allocated pool");
		return false;
	}

	return true;
}

//	Called when end game playing
bool CMLManBullet::OnEndGamePlay()
{
	m_BulletPool.Release();
	return true;
}

//	Called when being mission
bool CMLManBullet::OnBeginMission(int iMission)
{
	m_dwTimeCnt = 0;

	return true;
}

//	Called when reset mission
bool CMLManBullet::OnResetMission()
{
	m_dwTimeCnt = 0;

	return true;
}

//	Called when end mission
bool CMLManBullet::OnEndMission()
{
	ReleaseAllBullets();
	return true;
}

//	Logic tick
bool CMLManBullet::LogicRun(float fTimeSpan)
{
	BeginLogicCount();

	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	AListPosition thePos, pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLBullet* pBullet = (CMLBullet*)m_BulletList.GetNext(pos);
		pBullet->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pBullet->IsDead())
		{
			pBullet->Release();
			m_BulletPool.FreeObject(pBullet);
			m_BulletList.RemoveAt(thePos);
		}
	}

	EndLogicCount();

	return true;
}

//	Tick animation
bool CMLManBullet::TickAnimation()
{
	AListPosition pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		CMLBullet* pBullet = (CMLBullet*)m_BulletList.GetNext(pos);
		pBullet->TickAnimation();
	}

	return true;
}

//	Render routines
bool CMLManBullet::Render(A3DViewport* pViewport)
{
	AListPosition pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		CMLBullet* pBullet = (CMLBullet*)m_BulletList.GetNext(pos);
		if (pBullet->GetRenderFlag())
			pBullet->Render(pViewport);
	}

	return true;
}

//	Pause sound
void CMLManBullet::PauseSound(bool bPause)
{
	AListPosition pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_BulletList.GetNext(pos);
		pObject->PauseSound(bPause);
	}
}

//	Stop sound
void CMLManBullet::StopSound()
{
	AListPosition pos = m_BulletList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_BulletList.GetNext(pos);
		pObject->StopSound();
	}
}

