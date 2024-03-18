/*
 * FILE: ML_CannonCaller.cpp
 *
 * DESCRIPTION: Cannon caller class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_CannonCaller.h"
#include "ML_GamePlay.h"
#include "ML_Player.h"
#include "ML_ManBullet.h"
#include "ML_Resources.h"

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
//	Implement CMLCannonCaller
//
///////////////////////////////////////////////////////////////////////////

CMLCannonCaller::CMLCannonCaller() : CMLWeapon()
{
	m_bContinue		= false;
	m_pLocalizer	= NULL;
	m_bLocatedOk	= false;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, don't load model.
	pUser: weapon user
*/
bool CMLCannonCaller::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	if (!CMLWeapon::Init(iWeaponID, bLoadModel, pUser))
	{
		ADEBUG_TRACE("Failed to call CMLWeapon::Init");
		return false;
	}

	if ((m_pLocalizer = g_theGame.LoadA3DModel(RES_MOX_CNLOCALIZER)) != NULL)
		m_pLocalizer->PlayActionByName("[²¥·Å¶¯»­]", true);

	return true;
}

//	Release resources
bool CMLCannonCaller::Release()
{
	if (m_pLocalizer)
	{
		g_theGame.ReleaseA3DModel(m_pLocalizer);
		m_pLocalizer = NULL;
	}

	return CMLWeapon::Release();
}

//	Unlimited ammo ?
bool CMLCannonCaller::UnlimitedAmmo()
{
	return true;
}

//	Can fire ?
bool CMLCannonCaller::CanFire()
{
//	if (m_pPlayer && m_pPlayer->SupportReady(ID_SUPP_CANNON))
//		return true;

	return true;
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int CMLCannonCaller::Fire(bool bFirst)
{
	if (!bFirst || !CanFire())
		return 0;

	A3DVECTOR3 vPos;

	if (TraceLocalizerPos(&vPos))
	{
		m_vLocatedPos = vPos;
		if (m_pPlayer->CallSupport(ID_SUPP_CANNON))
		{
			//	Restore to previous weapon
			m_pPlayer->RestoreOldWeapon();
		}
	}

	return 0;
}

//	Release trigger
void CMLCannonCaller::ReleaseTrigger()
{
}

//	Logic tick
bool CMLCannonCaller::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	CMLWeapon::LogicRun(fTimeSpan, dwTimeCnt);

	if (!m_pLocalizer)
		return true;

	A3DVECTOR3 vPos;
	m_bLocatedOk = TraceLocalizerPos(&vPos);

	if (m_bLocatedOk)
	{
		vPos.y += 0.2f;
		m_pLocalizer->SetPos(vPos);
	}

	return true;
}

//	Tick animation
bool CMLCannonCaller::TickAnimation()
{
	CMLWeapon::TickAnimation();

	if (m_pLocalizer && m_bLocatedOk)
		m_pLocalizer->TickAnimation();

	return true;
}

//	Render routine
bool CMLCannonCaller::Render(A3DViewport* pViewport)
{
	CMLWeapon::Render(pViewport);

	if (m_pLocalizer && m_bLocatedOk)
		m_pLocalizer->Render(pViewport);

	return true;
}

/*	Get localizer position

	Return true if valid localizer position was got, otherwise return false.

	pvPos (out): receive localizer position if true is returned, can be NULL
*/
bool CMLCannonCaller::TraceLocalizerPos(A3DVECTOR3* pvPos)
{
	//	Get position pointed by mouse
	RAYTRACE Trace;
	if (!GETMLGAMEPLAY->GetLogicWorld()->RayTraceSence(m_pPlayer->GetPos(), 
		m_pPlayer->GetCameraDir() * 1000.0f, 1.0f, &Trace))
		return false;
	
	//	Protect localizer from moving too close
	A3DVECTOR3 vPlayPos, vDropPos, vDir;

	vPlayPos	= m_pPlayer->GetPos();
	vDropPos	= Trace.vPoint;
	vPlayPos.y	= 0.0f;
	vDropPos.y	= 0.0f;

	float fDist = Normalize(vDropPos - vPlayPos, vDir);

	if (fDist < GETGAMELOGICPARAMS->fCannonMinDist)
	{	
		*pvPos	 = vPlayPos + vDir * GETGAMELOGICPARAMS->fCannonMinDist;
		pvPos->y = GETMLGAMEPLAY->GetLogicWorld()->GetHeightOfPos(*pvPos);
	}
	else
		*pvPos = Trace.vPoint;

	return true;
}


