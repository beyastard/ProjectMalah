/*
 * FILE: ML_BulletShell.cpp
 *
 * DESCRIPTION: Bullet shell class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/24
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_BulletShell.h"
#include "ML_Resources.h"
#include "ML_ID.h"
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
//	Implement CMLUnit
//
///////////////////////////////////////////////////////////////////////////

CMLBulletShell::CMLBulletShell() : CMLModelUnit()
{
	m_bCollect	= true;
}

//	Initialize object
bool CMLBulletShell::Init(int iID, A3DVECTOR3& vPos, A3DVECTOR3& vVel)
{
	SetId(iID);

	int iModelRes = -1;
	switch (iID)
	{
	case ID_SHELL_PISTOL:	iModelRes = RES_MOX_PISTOLSHELL;	break;
	case ID_SHELL_RIFLE:	iModelRes = RES_MOX_RIFLESHELL;		break;
	case ID_SHELL_MACHINE:	iModelRes = RES_MOX_MACHINESHELL;	break;
	case ID_SHELL_CHAIN:	iModelRes = RES_MOX_MACHINECHAIN;	break;
	default:
		return false;
	}

	m_pMoxModel = g_theGame.LoadA3DModel(iModelRes);

	SetPos(vPos);
	SetVel(vVel);

	A3DVECTOR3 vAxis;

	m_vRotateAxis	= MLA_GetRandomVector();
	m_fRadian		= DEG2RAD(MLA_RandomSign(1500.0f, 3000.0f));
	m_bDead			= false;

	return true;
}

//	Release resources
bool CMLBulletShell::Release()
{
	return CMLModelUnit::Release();
}

//	Logic tick
bool CMLBulletShell::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	float fTickTime = fTimeSpan * 0.001f;
	A3DVECTOR3 vDelta, v1(m_vVel.x, m_vVel.y - 10.0f * fTickTime, m_vVel.z);

	vDelta.x = m_vVel.x * fTickTime;
	vDelta.z = m_vVel.z * fTickTime;
	vDelta.y = (m_vVel.y + v1.y) * 0.5f * fTickTime;

	SetVel(v1);
	SetPos(m_vPos + vDelta);

	float fHeight = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(m_vPos);
	if (m_vPos.y <= fHeight)
	{
		g_theGame.Play2DSound(RES_S2D_BULLETDROP1+MLA_Random(0, 11), false);
		m_bDead = true;
		return true;
	}

//	SetPos(m_vPos + m_vVel * fTickTime);

	if (m_pMoxModel)
	{
		//	Rotate shell
		float fRad = m_fRadian * fTickTime;
	//	A3DMATRIX4 mat = RotateAxis(m_vRotateAxis, fRad);
		m_pMoxModel->RotateAxisRelative(m_vRotateAxis, fRad);

		m_pMoxModel->UpdateModelOBB();
	}

	return true;
}


