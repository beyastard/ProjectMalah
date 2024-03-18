/*
 * FILE: ML_Player.cpp
 *
 * DESCRIPTION: Player class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Player.h"
#include "ML_GamePlay.h"
#include "ML_WeaponPistol.h"
#include "ML_WeaponRifle.h"
#include "ML_WeaponMortar.h"
#include "ML_WeaponMachine.h"
#include "ML_WeaponRPG.h"
#include "ML_AttackerCaller.h"
#include "ML_CannonCaller.h"
#include "ML_Route.h"
#include "ML_ManRoute.h"
#include "ML_ManObject.h"
#include "ML_ManAI.h"
#include "ML_Bullet.h"
#include "ML_Resources.h"
#include "AWndIniFile.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////

extern char		g_szSettingFile[];

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
//	Implement CMLPlayer
//
///////////////////////////////////////////////////////////////////////////

CMLPlayer::CMLPlayer()
{
	SetId(ID_LOCALPLAYER);

	m_bWeaponModel	= false;
	m_pWorld		= NULL;
	m_pCamera		= NULL;
	m_iCurWeapon	= -1;
	m_iOldWeapon	= -1;
	m_vExtents		= A3DVECTOR3(0.7f);
	m_fRadius		= Magnitude(m_vExtents);
	m_fCurPitch		= 0.0f;
	m_fCurYaw		= 0.0f;
	m_bShaking		= false;
	m_iShakeTime	= 600;
	m_iAskingVoice	= -1;
	m_iReplyVoice	= -1;
	m_iReplyTimeCnt	= 0;

	memset(m_aWeapons, 0, sizeof (m_aWeapons));
}

/*	Initialize object

	Return true for success, otherwise return false.

	pWorld: world object
	pCamera: player's camera
*/
bool CMLPlayer::Init(CMLLogicWorld* pWorld, A3DCamera* pCamera)
{
	m_pWorld	= pWorld;
	m_pCamera	= pCamera;

	return true;
}

//	Release resources
bool CMLPlayer::Release()
{
	m_pWorld	= NULL;
	m_pCamera	= NULL;

	return CMLUnit::Release();
}

//	Set position
void CMLPlayer::SetPos(A3DVECTOR3& vPos)
{
	CMLUnit::SetPos(vPos);

	if (m_pCamera)
		m_pCamera->SetPos(vPos);
}

/*	Called when begin game playing

	Return true for success, otherwise return false.
*/
bool CMLPlayer::OnStartGamePlay()
{
	int i;
	CMLWeapon* pWeapon;
	bool bLoadModel = GETCURUSERCONFIGS->bWeaponModel;

	//	Load all weapons
	for (i=0; i < NUM_WEAPON_ID; i++)
	{
		switch (i)
		{
		case WEAPON_PISTOL:		pWeapon = new CMLWeaponPistol;		break;
		case WEAPON_RIFLE:		pWeapon = new CMLWeaponRifle;		break;
		case WEAPON_MORTAR:		pWeapon = new CMLWeaponMortar;		break;
		case WEAPON_MACHINE:	pWeapon = new CMLWeaponMachine;		break;
		case WEAPON_RPG:		pWeapon = new CMLWeaponRPG;			break;
		case WEAPON_ATTACKER:	pWeapon = new CMLAttackerCaller;	break;
		case WEAPON_CANNON:		pWeapon = new CMLCannonCaller;		break;
		}

		if (!pWeapon->Init(ID_FIRSTWEAPON + i, bLoadModel, this))
		{
			ADEBUG_TRACE1("Failed to load weapon %d", ID_FIRSTWEAPON + i);
			assert(0);
			continue;
		}

		m_aWeapons[i] = pWeapon;
	}

	//	Set weapons' position and direction
	LoadWeaponsPosAndDir();

	return true;
}

/*	Called when game play is being ended

	Return true for success, otherwise return false.
*/
bool CMLPlayer::OnEndGamePlay()
{
	int i;

	//	Release all weapons
	for (i=0; i < NUM_WEAPON_ID; i++)
	{
		if (m_aWeapons[i])
		{
			m_aWeapons[i]->Release();
			delete m_aWeapons[i];
			m_aWeapons[i] = NULL;
		}
	}

	return true;
}

/*	Called when begin mission

	Return true for success, otherwise return false.
*/
bool CMLPlayer::OnBeginMission(int iMission)
{
	assert(m_pWorld && m_pCamera);

	//	Reset supply information
	for (int i=0; i < NUM_SUPPORT_ID; i++)
		m_aSupports[i].iInterval = 1000 * GETPLAYERILDVALUE(ILD_SUPP_CANNON+i, iMission);

	OnResetMission();

	return true;
}

/*	Called when mission is being reset

	Return true for success, otherwise return false.
*/
bool CMLPlayer::OnResetMission()
{
	int i, iLevel = GETMLGAMEPLAY->GetCurrentMission();

	//	Reset weapons
	for (i=0; i < NUM_WEAPON_ID; i++)
	{
		int iNumBullet;
		if (i == WEAPON_RPG)
			iNumBullet = GETPLAYERILDVALUE(ILD_AMMO_RPG, iLevel);
		else
			iNumBullet = GETPLAYERILDVALUE(ILD_AMMO_PISTOL+i, iLevel);

		m_aWeapons[i]->SetBullet(iNumBullet);
		m_aWeapons[i]->SetMaxBullet(iNumBullet);
		m_aWeapons[i]->ForceStop();
		m_aWeapons[i]->StopTremble();
	}

	//	Reset supply information
	for (i=0; i < NUM_SUPPORT_ID; i++)
	{
		m_aSupports[i].iCallCnt	= 0;
		m_aSupports[i].iTimeCnt	= m_aSupports[i].iInterval;
	}

//	ChangeWeapon(ID_WEAPON_PISTOL);
	ChangeWeapon(ID_WEAPON_RIFLE);
//	ChangeWeapon(ID_WEAPON_MORTAR);
//	ChangeWeapon(ID_WEAPON_MACHINE);

	//	Set player's position and direction
	CMLMapData* pMapData = m_pWorld->GetMapData();
	SetPos(pMapData->GetActorPos());
	m_pCamera->SetDirAndUp(pMapData->GetActorDir(), g_vAxisY);
	m_pCamera->SetFOV(DEG2RAD(CAMERA_FOV));
	m_pCamera->SetFOVSet(DEG2RAD(CAMERA_FOV));

	m_fCurPitch	= m_pCamera->GetPitch();
	m_fCurYaw	= m_pCamera->GetDeg();

	m_iMaxLife	= GETPLAYERILDVALUE(ILD_LIFE_PLAYER, iLevel);
	m_iLife		= m_iMaxLife;
	m_bShaking	= false;

	m_iAskingVoice	= -1;
	m_iReplyVoice	= -1;
	m_iReplyTimeCnt	= 0;

	return true;
}

/*	Called when end mission

	Return true for success, otherwise return false.
*/
bool CMLPlayer::OnEndMission()
{
	for (int i=0; i < NUM_WEAPON_ID; i++)
	{
		m_aWeapons[i]->ForceStop();
		m_aWeapons[i]->StopTremble();
	}

	return true;
}

//	Message handler
int	CMLPlayer::OnMsg(MLMSG* pMsg)
{
	assert(m_pWorld && m_pCamera);

	MLMSG* pMLMsg = (MLMSG*)pMsg;

	switch (pMLMsg->dwMsg)
	{
	case MSG_P_PITCH:
	{
		Pitch(FIX16TOFLOAT((int)pMLMsg->dwParam1));
		break;
	}
	case MSG_P_YAW:
	{
		Yaw(FIX16TOFLOAT((int)pMLMsg->dwParam1));
		break;
	}
	case MSG_P_FIRE:
	{
		Fire(pMLMsg->dwParam1 ? true : false);
		break;
	}
	case MSG_P_FIRESTOP:
	{
		if (m_iCurWeapon >= 0)
			m_aWeapons[m_iCurWeapon]->ReleaseTrigger();

		break;
	}
	case MSG_P_OPENFUNC:
	{
		if (m_iCurWeapon < 0)
			break;

		if (m_aWeapons[m_iCurWeapon]->FunctionOpened())
			m_aWeapons[m_iCurWeapon]->OpenFunction(false);
		else
			m_aWeapons[m_iCurWeapon]->OpenFunction(true);

		break;
	}
	case MSG_P_CHANGEWEAPON:
	{
		if (pMLMsg->dwParam1 == (DWORD)(-1))
			RestoreOldWeapon();
		else
			ChangeWeapon(pMLMsg->dwParam1);

		break;
	}
	case MSG_P_CALLSUPPORT:

		CallSupport(pMLMsg->dwParam1);
		break;
	}

	return 1;
}

//	Pitch
void CMLPlayer::Pitch(float fDegree)
{
	//	In snipe mode, slow camera's moving speed
	if (m_iCurWeapon == WEAPON_RIFLE && m_aWeapons[WEAPON_RIFLE]->FunctionOpened())
		fDegree /= GETGAMELOGICPARAMS->fRifleMagnifyScale;

	m_pCamera->PitchDelta(fDegree);
	m_fCurPitch += fDegree;

	CLAMPVALUE(m_fCurPitch, -89.0f, 89.0f);
}

//	Yaw
void CMLPlayer::Yaw(float fDegree)
{
	//	In snipe mode, slow camera's moving speed
	if (m_iCurWeapon == WEAPON_RIFLE && m_aWeapons[WEAPON_RIFLE]->FunctionOpened())
		fDegree /= GETGAMELOGICPARAMS->fRifleMagnifyScale;

	m_pCamera->DegDelta(fDegree);
	m_fCurYaw += fDegree;
}

//	Logic tick
bool CMLPlayer::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	int iTimeSpan = ML_ConvertTimeSpan(fTimeSpan);

	if (m_bShaking)
		DoShake(iTimeSpan);

	if (m_iCurWeapon >= 0 && m_aWeapons[m_iCurWeapon])
	{
		CMLWeapon* pWeapon = m_aWeapons[m_iCurWeapon];
		pWeapon->LogicRun(fTimeSpan, dwTimeCnt);

		//	Adjust camera according to weapon's tremble
		float fPitch = m_fCurPitch + pWeapon->GetTrembleY();
		float fYaw = m_fCurYaw + pWeapon->GetTrembleX();
	
		CLAMPVALUE(fPitch, -89.0f, 89.0f);

		m_pCamera->SetPitch(fPitch);
		m_pCamera->SetDeg(fYaw);
	}

	//	Increase weapon's fire interval time
	int i;
	for (i=0; i < NUM_WEAPON_ID; i++)
	{
		if (i != m_iCurWeapon)
			m_aWeapons[i]->IncFireIntervalCnt(iTimeSpan, true);
		else
			m_aWeapons[i]->IncFireIntervalCnt(iTimeSpan, false);
	}

	//	Add support time counter
	for (i=0; i < NUM_SUPPORT_ID; i++)
	{
		if ((m_aSupports[i].iTimeCnt += iTimeSpan) > m_aSupports[i].iInterval)
			m_aSupports[i].iTimeCnt = m_aSupports[i].iInterval;
	}

	//	Time to play supply reply voice ?
	if (m_iReplyVoice >= 0)
	{
		if ((m_iReplyTimeCnt += iTimeSpan) >= 4000)
		{
			g_theGame.Play2DSound(m_iReplyVoice, false);
			m_iReplyVoice = -1;
		}
	}

	return true;
}

//	Tick animation
bool CMLPlayer::TickAnimation()
{
	if (m_iCurWeapon >= 0 && m_aWeapons[m_iCurWeapon])
		m_aWeapons[m_iCurWeapon]->TickAnimation();

	return true;
}

//	Render routine
bool CMLPlayer::Render(A3DViewport* pViewport)
{
	if (m_iCurWeapon >= 0 && m_aWeapons[m_iCurWeapon])
		m_aWeapons[m_iCurWeapon]->Render(pViewport);

	return true;
}

/*	Change user's weapon

	iWeaponID: new weapon's ID, -1 can be used to hide current weapon
*/
void CMLPlayer::ChangeWeapon(int iWeaponID)
{
	int iWeaponIdx = iWeaponID - ID_FIRSTWEAPON;
	if (iWeaponIdx == m_iCurWeapon)
		return;

	if (m_iCurWeapon >= 0)
	{
		//	Hide prior weapon
		m_aWeapons[m_iCurWeapon]->ForceStop();
		m_aWeapons[m_iCurWeapon]->Show(false);

		if (m_iCurWeapon != WEAPON_ATTACKER && m_iCurWeapon != WEAPON_CANNON)
			m_iOldWeapon = m_iCurWeapon;
	}

	if (ISWEAPONID(iWeaponID))
	{
		m_aWeapons[iWeaponIdx]->Show(true);
		m_iCurWeapon = iWeaponIdx;
	}
	else
		m_iCurWeapon = -1;
}

//	Restore old weapon
void CMLPlayer::RestoreOldWeapon()
{
	if (m_iOldWeapon >= 0)
		ChangeWeapon(ID_FIRSTWEAPON + m_iOldWeapon);
}

/*	Fire routine

	bFirst: first trigger
*/
void CMLPlayer::Fire(bool bFirst)
{
	if (m_iCurWeapon < 0)
		return;

	CMLWeapon* pWeapon = m_aWeapons[m_iCurWeapon];

	if (!bFirst && !pWeapon->CanContinueFire())
		return;

	int iNumBullet = pWeapon->Fire(bFirst);
}

//	Ray trace routine
bool CMLPlayer::RayTrace(MLRAYTRACE* pTrace)
{
	A3DVECTOR3 vMins, vMaxs, vHitPos, vNormal;
	float fFrac;

	vMins = m_vPos - m_vExtents;
	vMaxs = m_vPos + m_vExtents;

	if (CLS_RayToAABB3(pTrace->vStart, pTrace->vVel * pTrace->fTime, vMins, vMaxs, 
					   vHitPos, &fFrac, vNormal))
	{
		pTrace->iObjectType		= MLTRACE_PLAYER;
		pTrace->pObject			= (void*) this;
		pTrace->Trace.vPoint	= vHitPos;
		pTrace->Trace.fFraction	= fFrac;
		return true;
	}

	return false;
}

//	Call support
bool CMLPlayer::CallSupport(int iSupportID)
{
	//	Play calling voice
	if (m_iAskingVoice >= 0)
	{
		g_theGame.Stop2DSound(m_iAskingVoice);
		m_iAskingVoice = -1;
	}

	m_iAskingVoice = GetSupplyVoice(iSupportID, 0);
	g_theGame.Play2DSound(m_iAskingVoice, false);

	if (!SupportReady(iSupportID))
		return false;

	int iIndex = iSupportID - ID_FIRSTSUPPORT;
	m_aSupports[iIndex].iCallCnt++;

	switch (iSupportID)
	{
	case ID_SUPP_CANNON:
	{
		//	Get located postion of cannon caller
		A3DVECTOR3 vPos = ((CMLCannonCaller*)m_aWeapons[WEAPON_CANNON])->GetLocatedPos();
		if (!GETMLGAMEPLAY->GetObjectMan()->CreateCannon(vPos))
			ADEBUG_TRACE("Failed to create cannon");

		break;
	}
	case ID_SUPP_COPTER:
	{
		CMLRoute* pRoute = GETMLGAMEPLAY->GetRouteMan()->GetCalledCopterRoute();
		if (!pRoute)
			break;

		if (!GETMLGAMEPLAY->GetObjectMan()->CreateHelicopter(pRoute))
			ADEBUG_TRACE("Failed to create helicopter");
	
		break;
	}
	case ID_SUPP_ATTACKER:
	{
		//	Get located postion of attacker caller
		A3DVECTOR3 vPos = ((CMLAttackerCaller*)m_aWeapons[WEAPON_ATTACKER])->GetLocatedPos();
		if (!GETMLGAMEPLAY->GetObjectMan()->CreateAttacker(vPos))
			ADEBUG_TRACE("Failed to create helicopter");

		break;
	}
	}

	m_aSupports[iIndex].iTimeCnt = 0;

	return true;
}

/*	Support is ready ?

	Return true if support is ready, otherwise return false.

	iSupportID: ID of support will be queried
*/
bool CMLPlayer::SupportReady(int iSupportID)
{
	assert(ISSUPPORTID(iSupportID));

	int iIndex = iSupportID - ID_FIRSTSUPPORT;
	if (m_aSupports[iIndex].iTimeCnt < m_aSupports[iIndex].iInterval)
		goto Deny;

	//	If there are enemy planes in battle, deny plane asking
/*	if (iSupportID == ID_SUPP_COPTER || iSupportID == ID_SUPP_ATTACKER)
	{
		CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
		if (pAIMan->GetEnemyPlaneNum() > 0)
			goto Deny;
	}
*/
	//	Play some voice
	m_iReplyVoice	= GetSupplyVoice(iSupportID, 2);
	m_iReplyTimeCnt	= 0;
	return true;

Deny:

	//	Play some voice
	m_iReplyVoice	= GetSupplyVoice(iSupportID, 1);
	m_iReplyTimeCnt	= 0;
	return false;
}

/*	Get support counter

	Return current counter value for success, otherwise return -1.

	iSupportID: ID of support will be queried
	piMax (out): receive maximum count value, can be NULL
*/
int CMLPlayer::GetSupportCounter(int iSupportID, int* piMax)
{
	assert(ISSUPPORTID(iSupportID));

	int iIndex = iSupportID - ID_FIRSTSUPPORT;

	if (piMax)
		*piMax = m_aSupports[iIndex].iInterval;

	return m_aSupports[iIndex].iTimeCnt;
}

//	Strike player
void CMLPlayer::Strike(CMLBullet* pBullet)
{
	if (GETGAMESWITCH(SWITCH_UNDEAD))
		return;

	if (pBullet->GetId() == ID_BULLET_ROCKET ||
		pBullet->GetId() == ID_BULLET_FANTAN ||
		pBullet->GetId() == ID_BULLET_ARMORED)
	{
		Shake();	//	Shake
		g_theGame.PlayGFX(RES_GFX_ROCKETHURT, m_vPos + GetCameraDir() * 1.5f, 1.0f);
	}

	Strike(pBullet->GetDamage());
}

//	Be stabbed
void CMLPlayer::Stabbed(int iDamage)
{
	if (GETGAMESWITCH(SWITCH_UNDEAD))
		return;

	int iIndex = MLA_Random(0, 1);
	A3DVECTOR3 vOffx = m_pCamera->GetRight() * MLA_Random(-0.3f, 0.3f);
	A3DVECTOR3 vOffy = m_pCamera->GetUp() * MLA_Random(-0.3f, 0.3f);

	g_theGame.PlayGFX(RES_GFX_STABHURT0 + iIndex, m_vPos + GetCameraDir() * 1.5f + vOffx + vOffy, 0.4f);

	Strike(iDamage);
}

//	Strike player
void CMLPlayer::Strike(int iDamage)
{
	if (GETGAMESWITCH(SWITCH_UNDEAD))
		return;

	if ((m_iLife -= iDamage) > 0)
		return;
	
	m_iLife = 0;
}

/*	Supply weapon ammo

	iWeaponID: weapon ID.
	iNum: number of supplied ammo
*/
void CMLPlayer::SupplyAmmo(int iWeaponID, int iNum)
{
	assert(ISWEAPONID(iWeaponID));

	int iWeaponIdx = iWeaponID - ID_FIRSTWEAPON;
	m_aWeapons[iWeaponIdx]->AddBullet(iNum);
}

/*	Supply life

	iAmount: life amount.
*/
void CMLPlayer::SupplyLife(int iAmount)
{
	if ((m_iLife += iAmount) > m_iMaxLife)
		m_iLife = m_iMaxLife;
}

//	Begin to shake
void CMLPlayer::Shake()
{
	m_iShakeTimeCnt	= 0;
	m_bShaking		= true;
	m_vShakePos		= GETMAPDATA->GetActorPos();	//	m_vPos;
}

//	Do shake
void CMLPlayer::DoShake(int iTickSpan)
{
	int iDelta;

	m_iShakeTimeCnt += iTickSpan;
	if (m_iShakeTimeCnt >= m_iShakeTime)
	{
		iDelta = m_iShakeTime;
		m_bShaking = false;
	}
	else
		iDelta = m_iShakeTimeCnt;

	float fSwing = 0.8f * (1.0f - (float)iDelta / m_iShakeTime);

	float fTime = (iDelta % 50) / 50.0f;
	float fOff1 = (float)sin(A3D_PI * 2.0f * fTime) * fSwing;
	fTime = (iDelta % 80) / 80.0f;
	float fOff2 = (float)sin(A3D_PI * 2.0f * fTime) * fSwing;

	SetPos(m_vShakePos + GetCameraUp() * fOff1 + GetCameraRight() * fOff2);

	if (!m_bShaking)	//	End shaking
		SetPos(m_vShakePos);
}

A3DVECTOR3 CMLPlayer::GetRandomHitPos()
{
	A3DVECTOR3 vTarget;
	
	vTarget.x = m_vPos.x + MLA_Random(-m_vExtents.x, m_vExtents.x);
	vTarget.y = m_vPos.y + MLA_Random(-m_vExtents.y, m_vExtents.y);
	vTarget.z = m_vPos.z + MLA_Random(-m_vExtents.z, m_vExtents.z);

	return vTarget;
}

/*	Get a position of miss hitting

	vFirePos: fire position.
	fMissRad: miss radius.
*/
A3DVECTOR3 CMLPlayer::GetRandomMissPos(A3DVECTOR3& vFirePos, float fMissRad)
{
	A3DVECTOR3 vRight, vUp, vDir;
	vDir = Normalize(m_vPos - vFirePos);
	MLA_GetViewUpAndRight(vDir, &vUp, &vRight);
	return MLA_GetRandomPosInDisk(m_vPos, vUp, vRight, m_fRadius + fMissRad, m_fRadius + 0.2f);
}

/*	Get supply voice

	iSupplyID: supply ID.
	iWhich: 0, calling
			1, deny
			2, agree
*/
int	CMLPlayer::GetSupplyVoice(int iSupportID, int iWhich)
{
	int iSound;

	switch (iSupportID)
	{
	case ID_SUPP_CANNON:	iSound = RES_S2D_CANNONCALLING + iWhich;	break;
	case ID_SUPP_COPTER:	iSound = RES_S2D_COPTERCALLING + iWhich;	break;
	case ID_SUPP_ATTACKER:	iSound = RES_S2D_ATTACKERCALLING + iWhich;	break;
	default:
		assert(0);
		break;
	}

	return iSound;
}

//	Force to stop fire
void CMLPlayer::StopFire()
{
	if (m_iCurWeapon >= 0)
		m_aWeapons[m_iCurWeapon]->ReleaseTrigger();
}

//	Load weapons' position and direction
void CMLPlayer::LoadWeaponsPosAndDir()
{
	AWndIniFile	IniFile;
	if (!IniFile.Init(g_szSettingFile))
		return;

	char szSec[20];
	A3DVECTOR3 v;

	for (int i=0; i <= WEAPON_RPG; i++)
	{
		sprintf(szSec, "weapon_%d", i);

		const char* szStr = NULL;
		if (!IniFile.ReadString(szSec, "pos", szStr))
			continue;
		
		sscanf(szStr, "%f, %f, %f", &v.x, &v.y, &v.z);
		m_aWeapons[i]->SetRelativePos(v);

		if (!IniFile.ReadString(szSec, "dir", szStr))
			continue;
		
		sscanf(szStr, "%f, %f, %f", &v.x, &v.y, &v.z);
		m_aWeapons[i]->SetRelativeDir(v);
	}

	IniFile.Release();
}

