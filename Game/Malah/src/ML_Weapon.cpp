/*
 * FILE: ML_Weapon.cpp
 *
 * DESCRIPTION: Weapon class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Weapon.h"
#include "ML_Resources.h"
#include "ML_Player.h"
#include "ML_GamePlay.h"
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
//	Implement CMLWeapon
//
///////////////////////////////////////////////////////////////////////////

CMLWeapon::CMLWeapon() : CMLModelUnit()
{
	m_bRender			= false;
	m_pPlayer			= NULL;
	m_iFireInterval		= 100;
	m_iFireInterCnt		= 0;
	m_bContinue			= true;
	m_bUnlimitedAmmo	= false;
	m_bCassette			= false;
	m_iNumCassette		= 0;
	m_iNumBullet		= 0;
	m_iCassetteBullet	= 0;
	m_iMaxBullet		= -1;
	m_iCassetteVol		= 30;
	m_bFunction			= false;
	m_vRelativePos		= g_vOrigin;
	m_vRelativeDir		= g_vAxisZ;
	m_bLoadModel		= true;
	m_fTrembleX			= 0.0f;
	m_fTrembleY			= 0.0f;
}

/*	Initialize object

	Return true for success, otherwise return false.

	iWeaponID: weapon's ID
	bLoadModel: true, load weapon model. false, we also load model too, but
				ignore all textures and hide weapon model, it looks seem that
				we don't load model
	pUser: weapon user
*/
bool CMLWeapon::Init(int iWeaponID, bool bLoadModel, CMLPlayer* pUser)
{
	SetId(iWeaponID);

	m_pPlayer	 = pUser;
	m_bLoadModel = bLoadModel;

	if (!LoadModel(bLoadModel))
	{
		ADEBUG_TRACE("Failed to load weapon model");
		return false;
	}

	//	Weapon's default state is invisible
	Show(false);

	return true;
}

//	Release resources
bool CMLWeapon::Release()
{
	m_pPlayer = NULL;
	return CMLModelUnit::Release();
}

/*	Load model

	bLoadModel: true, load complete model. false. only load model but not
				texture.
*/
bool CMLWeapon::LoadModel(bool bLoadModel)
{
	if (m_pMoxModel)
		return true;

	int iModelRes = -1;
	switch (m_nId)
	{
	case ID_WEAPON_PISTOL:	iModelRes = RES_MOX_PISTOL;		break;
	case ID_WEAPON_RIFLE:	iModelRes = RES_MOX_RIFLE;		break;
	case ID_WEAPON_MORTAR:	iModelRes = RES_MOX_MORTAR;		break;
	case ID_WEAPON_MACHINE:	iModelRes = RES_MOX_MACHINE;	break;
	case ID_WEAPON_RPG:		iModelRes = RES_MOX_RPG;		break;
	}

	if (!bLoadModel)
	{
	}

	if (iModelRes >= 0 && !(m_pMoxModel = g_theGame.LoadA3DModel(iModelRes)))
		ADEBUG_TRACE("Failed to load model file");

	if (!bLoadModel)
	{
		m_pMoxModel->SetVisibility(false);
	}

	return true;
}

//	Show or hide weapon
void CMLWeapon::Show(bool bShow)
{
	m_bRender = bShow;

	if (!m_pMoxModel || (!m_bLoadModel && bShow))
		return;

	m_pMoxModel->SetVisibility(bShow);
}

//	Adjust weapon's position and direction
void CMLWeapon::AdjustWeaponPos()
{
	if (!m_pMoxModel)
		return;

	//	For convenient, we directly get keyboard's states here. Maybe we
	//	should do this through message channel.
	if (GETGAMESWITCH(SWITCH_ADJUSTGUNPOS))
	{
		A3DVECTOR3 vOff(0.0f);
		float fOff = 0.01f;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			fOff = 0.001f;

		//	Offset
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			vOff.z += fOff;
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			vOff.z -= fOff;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			vOff.x -= fOff;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			vOff.x += fOff;

		if (GetAsyncKeyState(VK_PRIOR) & 0x8000)
			vOff.y += fOff;
		else if (GetAsyncKeyState(VK_NEXT) & 0x8000)
			vOff.y -= fOff;

		MoveRelativePos(vOff);

		fOff = 0.05f;

		//	Pitch
		if (GetAsyncKeyState('A') & 0x8000)
			PitchRelativeDir(fOff);
		else if (GetAsyncKeyState('Z') & 0x8000)
			PitchRelativeDir(-fOff);
	
		//	Yaw
		if (GetAsyncKeyState('X') & 0x8000)
			YawRelativeDir(-fOff);
		else if (GetAsyncKeyState('C') & 0x8000)
			YawRelativeDir(fOff);
	}

	A3DVECTOR3 vViewPos		= m_pPlayer->GetPos();
	A3DVECTOR3 vViewDir		= m_pPlayer->GetCameraDir();
	A3DVECTOR3 vUiewRight	= m_pPlayer->GetCameraRight();
	A3DVECTOR3 vViewUp		= m_pPlayer->GetCameraUp();

	A3DMATRIX4 mat = TransformMatrix(vViewDir, vViewUp, vViewPos);
	SetPos(mat * m_vRelativePos);

	mat._41 = mat._42 = mat._43 = 0.0f;
	A3DVECTOR3 vDir = mat * m_vRelativeDir;

	//	We cann't use MLA_GetViewUpAndRight here, because weapon's up may
	//	not be in the plane built by cross-producting vDir and g_vAxisY. If
	//	we adjust pitch of weapon, we must calculate weapon's up using vViewUp
	//	and vDir, however, we now don't do picth to weapon, so use vViewUp directly.
	A3DVECTOR3 vRight = Normalize(CrossProduct(vViewUp, vDir));
	A3DVECTOR3 vUp = CrossProduct(vDir, vRight);
		
//	m_pMoxModel->SetDirAndUp(vDir, vViewUp);
	m_pMoxModel->SetDirAndUp(vDir, vUp);
}

//	Move relative position
void CMLWeapon::MoveRelativePos(A3DVECTOR3& vOff)
{
	m_vRelativePos = m_vRelativePos + vOff;
}

//	Pitch relative diretion
//	fPitch: pitch value in degree
void CMLWeapon::PitchRelativeDir(float fPitch)
{
	A3DMATRIX4 mat = RotateX(DEG2RAD(fPitch));
	m_vRelativeDir = mat * m_vRelativeDir;
}

//	Yaw relative direction
//	fYaw: yaw value in degree
void CMLWeapon::YawRelativeDir(float fYaw)
{
	A3DMATRIX4 mat = RotateY(DEG2RAD(fYaw));
	m_vRelativeDir = mat * m_vRelativeDir;
}

//	Logic tick
bool CMLWeapon::LogicRun(float fTimeSpan, DWORD dwTimeCnt)
{
	assert(m_pPlayer);
	AdjustWeaponPos();
	return true;
}

//	Add bullet number
void CMLWeapon::AddBullet(int iNum)
{
	m_iNumBullet += iNum;

	if (!m_bCassette)
	{
		if (m_iNumBullet < 0)
			m_iNumBullet = 0;

		if (m_iMaxBullet >= 0 && m_iNumBullet > m_iMaxBullet)
			m_iNumBullet = m_iMaxBullet;

		m_iCassetteBullet = m_iNumBullet;
	}
	else
	{
		assert(m_iCassetteVol > 0);

		if (m_iNumBullet < 0)
		{
			m_iNumCassette		= 0;
			m_iNumBullet		= 0;	
			m_iCassetteBullet	= 0;
		}
		else
		{
			if (m_iMaxBullet >= 0 && m_iNumBullet > m_iMaxBullet)
				m_iNumBullet = m_iMaxBullet;

			m_iNumCassette		= (m_iNumBullet - 1) % m_iCassetteVol;
			m_iCassetteBullet	= m_iNumBullet - m_iNumCassette * m_iCassetteVol;
		}
	}
}

//	Set number of bullet
void CMLWeapon::SetBullet(int iNum)
{
	if (iNum < 0)
		iNum = 0;

	if (m_iMaxBullet >= 0 && iNum > m_iMaxBullet)
		iNum = m_iMaxBullet;

	if (!m_bCassette)
	{
		m_iNumBullet		= iNum;
		m_iCassetteBullet	= iNum;
	}
	else
	{
		m_iNumBullet		= iNum;
		m_iNumCassette		= (iNum - 1) % m_iCassetteVol;
		m_iCassetteBullet	= m_iNumBullet - m_iNumCassette * m_iCassetteVol;
	}
}

//	Add cassette number
void CMLWeapon::AddCassette(int iNum)
{
	if (!m_bCassette)
		return;

	m_iNumCassette += iNum;

	if (m_iNumCassette < 0)
	{
		m_iNumCassette		= 0;
		m_iNumBullet		= 0;	
		m_iCassetteBullet	= 0;
	}
	else
	{
		m_iNumBullet = m_iNumCassette * m_iCassetteVol + m_iCassetteBullet;
		if (m_iMaxBullet >= 0 && m_iNumBullet > m_iMaxBullet)
		{
			m_iNumBullet		= m_iMaxBullet;
			m_iNumCassette		= (m_iNumBullet - 1) % m_iCassetteVol;
			m_iCassetteBullet	= m_iNumBullet - m_iNumCassette * m_iCassetteVol;
		}
	}
}

//	Still has bullet ?
bool CMLWeapon::HasBullet()
{
	if (UnlimitedAmmo() || m_iNumCassette > 0 || m_iNumBullet > 0)
		return true;

	return false;
}

/*	Fire routine

	Return number of bullet were shot.
	
	bFirst: true, first time flag
*/
int	CMLWeapon::Fire(bool bFirst)
{
	if (!CanFire())
		return 0;

	if (!HasBullet())
	{
		NoBulletTrigger();
		return 0;
	}

	int iNumBullet	= m_iFireInterCnt / m_iFireInterval;
	m_iFireInterCnt = m_iFireInterCnt % m_iFireInterval;

	if (bFirst)
		iNumBullet = 1;

	return iNumBullet;
}

//	Force to stop fire immediately, including all gfx and sound
void CMLWeapon::ForceStop()
{
	if (m_pMoxModel)
	{
		m_pMoxModel->StopAllSFX();
		m_pMoxModel->StopAction();
	}
}

//	Throw bullet shell
void CMLWeapon::ThrowBulletShell()
{
	USERCONFIGS* pConfig = GETCURUSERCONFIGS;
	if (!m_bLoadModel || !m_pMoxModel || !pConfig->bGraphicsMisc)
		return;

	int iShellID;
	switch (m_nId)
	{
	case ID_WEAPON_PISTOL:	iShellID = ID_SHELL_PISTOL;		break;
	case ID_WEAPON_RIFLE:	iShellID = ID_SHELL_RIFLE;		break;
	case ID_WEAPON_MACHINE:	iShellID = ID_SHELL_MACHINE;	break;
	default:
		return;
	}

	A3DFrame* pFrame = m_pMoxModel->FindChildFrameByName("HH_µ¯¿Ç¹Òµã");
	A3DMATRIX4 mat = pFrame->GetAbsoluteTM();

	//	Hang point's z axis is approximate direction of shell poping
	A3DVECTOR3 vPos		= mat.GetRow(3);
	A3DVECTOR3 vDir		= mat.GetRow(2);
	A3DVECTOR3 vRight	= mat.GetRow(0);
	A3DVECTOR3 vUp		= mat.GetRow(1);
	A3DVECTOR3 vPopDir;

	MLA_GenRandomVectorH(vPopDir);

	float fDeg = MLA_Random(45.0f, 90.0f);
	float fCos = g_pA3DMathUtility->COS(fDeg);
	vPopDir.x *= fCos;
	vPopDir.z *= fCos;
	vPopDir.y  = g_pA3DMathUtility->SIN(fDeg);

	vPopDir = vRight * vPopDir.x + vUp * vPopDir.z + vDir * vPopDir.y;

	//	Add speed
	vPopDir = vPopDir * MLA_Random(1.5f, 3.0f);

	CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
	pObjectMan->CreateBulletShell(iShellID, vPos, vPopDir);

	//	Throw chain shell at same time
//	if (m_nId == ID_WEAPON_MACHINE)
//		pObjectMan->CreateBulletShell(ID_SHELL_CHAIN, vPos, vDir * 1.0f);
}


