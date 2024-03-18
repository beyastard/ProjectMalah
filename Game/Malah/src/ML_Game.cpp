/*
 * FILE: ML_Game.cpp
 *
 * DESCRIPTION: Main class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "AM.h"
#include "ML_Game.h"
#include "ML_Strings.h"
#include "ML_Resources.h"
#include "ML_GamePlay.h"
#include "ML_Interface.h"
#include "ML_WndManager.h"

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

CMLGame		g_theGame;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLGame
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLGame, AGame, 1)

CMLGame::CMLGame() : AGame()
{
	m_p3DSoundMan = NULL;
}

//	Initialize game core
bool CMLGame::InitGameCore()
{
	//	Set engine and device states
	m_pA3DEngine->GetA3DDevice()->SetSpecularEnable(true);

	//	Create AGamePlay object
	if (!(m_pAGamePlay = new CMLGamePlay()))
	{
		ADEBUG_TRACE("Not enough memory");
		return false;
	}

	CMLGamePlay* pMLGamePlay = (CMLGamePlay*)m_pAGamePlay;

	if (!m_pAGamePlay->Init(this))
	{
		ADEBUG_TRACE("Failed to initialize game play object");
		return false;
	}

	return true;
}

//	Initialize A3D tools
bool CMLGame::InitA3DTools()
{
	if (!AGame::InitA3DTools())
	{
		ADEBUG_TRACE("Failed to call AGame::InitA3DTools");
		return false;
	}

	if (!InitFonts())
	{
		ADEBUG_TRACE("Failed to initialize fonts");
		return false;
	}

	int a2DTypeBuf[NUM_SND2DTYPE] = {64, 1};
	int a3DTypeBuf[NUM_SND3DTYPE] = {32, 5, 3};
	if (!GetA3DAssistTools()->S_SndInit(a2DTypeBuf, NUM_SND2DTYPE, 32, a3DTypeBuf, NUM_SND3DTYPE, 256))
	{
		ADEBUG_TRACE("Init S_SndInit error");
		return false;
	}

	m_p3DSoundMan = m_pA3DEngine->GetAMSoundEngine()->Get3DSoundBufferMan();

	return true;
}

//	Run game
int CMLGame::Run()
{
	ResetEnvironment();

	CMLGamePlay* pMLGamePlay = (CMLGamePlay*)m_pAGamePlay;
	if (pMLGamePlay->GetWndManager())
	{
		pMLGamePlay->GetWndManager()->SetCurVideo(VIDEO_LOGO);
		pMLGamePlay->GetWndManager()->SetActiveStatus(MLIG_VIDEO);//MLIG_LOADING);
	}
	else
		pMLGamePlay->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

	int iRet = AGame::Run();

	return iRet;
}

//	Initalize fonts
bool CMLGame::InitFonts()
{
	assert(m_pA3DAssistTools);

	//	Register font1, size 12
	char szName[64];
	g_MLStrings.GetString(SI_FONT_FONT1, szName);

	A3DFONT_INFO_T Info;
	Info.pszName	= szName;
	Info.nHeight	= 12;
	Info.nFlag		= A3DFONT_FILTERED;
	Info.nBufNum	= 10;
	Info.nStrLen	= 256;
	Info.nSetBufNum	= 0;
	Info.nSetStrLen	= 256;

	if (!m_pA3DAssistTools->F_RegisterFont(FONT_FONT1_12, &Info))
	{
		ADEBUG_TRACE1("Failed to register font %s", szName);
		return false;
	}

	//	Register font1, size 16
	Info.nHeight = 16;
	if (!m_pA3DAssistTools->F_RegisterFont(FONT_FONT1_16, &Info))
	{
		ADEBUG_TRACE1("Failed to register font %s", szName);
		return false;
	}

	//	Register font2, size 12
	g_MLStrings.GetString(SI_FONT_FONT2, szName);
	Info.nHeight = 12;
	if (!m_pA3DAssistTools->F_RegisterFont(FONT_FONT2_12, &Info))
	{
		ADEBUG_TRACE1("Failed to register font %s", szName);
		return false;
	}

	//	Register font2, size 16
	Info.nHeight = 16;
	if (!m_pA3DAssistTools->F_RegisterFont(FONT_FONT2_16, &Info))
	{
		ADEBUG_TRACE1("Failed to register font %s", szName);
		return false;
	}

	return true;
}

//	Text out
bool CMLGame::TextOut(int iFont, int x, int y, char* szText, A3DCOLOR color, bool bShade)
{
	assert(m_pA3DAssistTools);

	if (m_pA3DAssistTools->F_GetCurFont() != iFont)
		m_pA3DAssistTools->F_SetCurFont(iFont);

	return m_pA3DAssistTools->F_TextOut(x, y, szText, color, bShade);
}

//	Text out debu information
bool CMLGame::DebugTextOut(int x, int y, char* szText)
{
	assert(m_pA3DAssistTools);

	if (m_pA3DAssistTools->F_GetCurFont() != FONT_FONT2_12)
		m_pA3DAssistTools->F_SetCurFont(FONT_FONT2_12);

	return m_pA3DAssistTools->F_TextOut(x, y, szText, A3DCOLORRGB(255, 255, 0), false);
}

/*	Load GFX from file

	Return GFX object's address for success, otherwise return NULL.

	iResID: resource ID defined in ML_Resources.h
*/
A3DGraphicsFX* CMLGame::LoadGFX(int iResID, A3DVECTOR3* pvPos/* NULL */)
{
	A3DGraphicsFX* pGFX = NULL;
	bool bRet;

	if (pvPos)
		bRet = m_pA3DEngine->GetA3DGFXMan()->LoadGFXFromFile((char*)RES_GetGFXFile(iResID), NULL, NULL, false, &pGFX, *pvPos);
	else
		bRet = m_pA3DEngine->GetA3DGFXMan()->LoadGFXFromFile((char*)RES_GetGFXFile(iResID), NULL, NULL, false, &pGFX);

	if (!bRet)
	{
		ADEBUG_TRACE1("Failed to load GFX %s.", RES_GetGFXFile(iResID));
		return NULL;
	}

	return pGFX;
}

//	Release GFX
void CMLGame::ReleaseGFX(A3DGraphicsFX* pGFX)
{
	if (pGFX)
		m_pA3DEngine->GetA3DGFXMan()->ReleaseGFX(pGFX);
}

/*	Play one-off GFX

	Return true if GFX was played, otherwise return false.

	iResID: resource ID defined in ML_Resources.h
	vPos: GFX position
	fScale: scale of GFX
	pvDir: GFX's direction, can be NULL
*/
bool CMLGame::PlayGFX(int iResID, A3DVECTOR3& vPos, float fScale, A3DVECTOR3* pvDir/* NULL */)
{
	A3DGraphicsFX* pGFX = LoadGFX(iResID, &vPos);
	if (!pGFX)
		return false;

	if (pvDir)
		pGFX->SetDir(*pvDir);

	pGFX->SetScale(fScale);
	pGFX->Start(true);

	return true;
}

//	Play GFX
bool CMLGame::PlayGFX(char* szFile, A3DVECTOR3& vPos, float fScale, A3DVECTOR3* pvDir/* NULL */)
{
	A3DGraphicsFX* pGFX = NULL;
	if (!m_pA3DEngine->GetA3DGFXMan()->LoadGFXFromFile(szFile, NULL, NULL, false, &pGFX))
	{
		ADEBUG_TRACE1("Failed to load GFX %s.", szFile);
		return false;
	}

	if (pvDir)
		pGFX->SetDir(*pvDir);

	pGFX->SetScale(fScale);
	pGFX->SetPos(vPos);
	pGFX->Start(true);

	return true;
}

//	Load mox model
A3DModel* CMLGame::LoadA3DModel(char* szModelFile)
{
	A3DModel* pModel = NULL;
	if (!m_pA3DEngine->GetA3DModelMan()->LoadModelFile(szModelFile, &pModel))
	{
		ADEBUG_TRACE1("Failed to load model %s.", szModelFile);
		return NULL;
	}

	return pModel;
}

//	Load mox model
A3DModel* CMLGame::LoadA3DModel(int iResID)
{
	A3DModel* pModel = NULL;
	if (!m_pA3DEngine->GetA3DModelMan()->LoadModelFile((char*)RES_GetModelFile(iResID), &pModel))
	{
		ADEBUG_TRACE1("Failed to load model %s.", RES_GetModelFile(iResID));
		return NULL;
	}

	return pModel;
}

//	Release mox model
void CMLGame::ReleaseA3DModel(A3DModel* pModel)
{
	if (pModel)
		m_pA3DEngine->GetA3DModelMan()->ReleaseModel(pModel);
}

/*	Play A3DModel action

	Return true if action was played, otherwise return false.

	pModel: A3D model object
	szName: action's name
	bDiscardOld: true, stop old action immediately.
	bStopSFX: true, stop old SFX
	bStopGFX: true, stop old GFX
*/
bool CMLGame::PlayA3DModelAction(A3DModel* pModel, char* szName, bool bDiscardOld,
								 bool bStopSFX/* true */, bool bStopGFX/* true */)
{
	assert(pModel);

	if (bStopSFX)
		pModel->StopAllSFX();

	if (bStopGFX)
		pModel->StopAllGFX();

	pModel->PlayActionByName(szName, bDiscardOld);

	return true;
}

/*	Get hang point's position and direction

	Return hang point's position

	pModel: A3D model object
	szName: hang point's name
	pvDir (out): used to receive hang point's direction, can be NULL
*/
A3DVECTOR3 CMLGame::GetHangPointPosAndDir(A3DModel* pModel, char* szName, A3DVECTOR3* pvDir)
{
	if (!pModel)
		return g_vOrigin;

	A3DFrame* pFrame = pModel->FindChildFrameByName(szName);
	if (!pFrame)
		return g_vOrigin;

	A3DMATRIX4 mat = pFrame->GetAbsoluteTM();
	
	if (pvDir)
		*pvDir = mat.GetRow(2);

	return mat.GetRow(3);
}

void CMLGame::Play2DSound(int iResID, bool bImmEffect, bool bLoop, bool bCanFade, int iType/* 0 */)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_2DSndPlay(iType, (char*)RES_GetSoundFile(iResID), bImmEffect, bLoop, bCanFade);
}

void CMLGame::Play2DSound(int iResID, bool bLoop, int iType/* 0 */)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_2DSndPlay(iType, (char*)RES_GetSoundFile(iResID), false, bLoop, false);
}

void CMLGame::Stop2DSound(int iResID, int iType/* 0 */)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_2DSndStop(iType, (char*)RES_GetSoundFile(iResID));
}

void CMLGame::Pause2DSound(int iResID, int iType/* 0 */)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_2DSndPause(iType, (char*)RES_GetSoundFile(iResID));
}

void CMLGame::Play3DSound(int iResID, A3DVECTOR3& vPos, float fMinDist/* 20.0f */, 
						  float fMaxDist/* 100.0f */, int iType/* 0 */)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_3DSndPlay(iType, (char*)RES_GetSoundFile(iResID), vPos, fMinDist, fMaxDist, false, false, false);
}

void CMLGame::Play3DSound(int iResID, A3DVECTOR3& vPos, float fMinDist, float fMaxDist, 
						  bool bImmEffect, bool bForce2D, bool bCanFade, int iType)
{
	A3DAssistTools* pAssistTools = GetA3DAssistTools();
	assert(pAssistTools);
	pAssistTools->S_3DSndPlay(iType, (char*)RES_GetSoundFile(iResID), vPos, fMinDist, fMaxDist, bImmEffect, bForce2D, bCanFade);
}

//	Create 3D sound from file
AM3DSoundBuffer* CMLGame::Create3DSound(int iResID, float fMin/* 20.0f */, float fMax/* 100.0f */)
{
	if (!m_p3DSoundMan)
		return NULL;

	AM3DSoundBuffer* pSoundBuf = NULL;

	if (!m_p3DSoundMan->Load3DSoundBufferFromFile((char*)RES_GetSoundFile(iResID), &pSoundBuf))
	{
		ADEBUG_TRACE1("Failed to load sound file %s", RES_GetSoundFile(iResID));
		return NULL;
	}

	pSoundBuf->SetVelocity(A3DVECTOR3(0.0f));
	pSoundBuf->SetMinDistance(fMin);
	pSoundBuf->SetMaxDistance(fMax);

	return pSoundBuf;
}

//	Release 3D sound buffer
void CMLGame::Release3DSound(AM3DSoundBuffer* pSndBuf)
{
	if (m_p3DSoundMan && pSndBuf)
		m_p3DSoundMan->Release3DSoundBuffer(pSndBuf);
}

//	Play 3D sound buffer
void CMLGame::Play3DSound(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos, bool bLoop/* false */)
{
	if (pSndBuf)
	{
		pSndBuf->SetPosition(vPos);
		pSndBuf->UpdateChanges();
		pSndBuf->Play(bLoop);
	}
}

void CMLGame::Play3DSound(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos, A3DVECTOR3& vVel, bool bLoop/* false */)
{
	if (pSndBuf)
	{
		pSndBuf->SetPosition(vPos);
		pSndBuf->SetVelocity(vVel);
		pSndBuf->UpdateChanges();
		pSndBuf->Play(bLoop);
	}
}

//	Stop 3D sound buffer
void CMLGame::Stop3DSound(AM3DSoundBuffer* pSndBuf)
{
	if (pSndBuf)
		pSndBuf->Stop();
}

//	Pause 3D sound
void CMLGame::Pause3DSound(AM3DSoundBuffer* pSndBuf, bool bPause, bool bLoop)
{
	if (!pSndBuf)
		return;

	if (bPause && pSndBuf->IsPlaying())
		pSndBuf->Pause();
	else if (!bPause && pSndBuf->IsPaused())
		pSndBuf->Play(bLoop);
}

//	Update 3D sound position
void CMLGame::Update3DSoundPos(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos)
{
	if (pSndBuf)
	{
		pSndBuf->SetPosition(vPos);
		pSndBuf->UpdateChanges();
	}
}

void CMLGame::EnableAlphaTest(bool bEnable, A3DCMPFUNC CompFunc, DWORD dwRefValue)
{
	A3DDevice* pDevice = GetA3DEngine()->GetA3DDevice();

	if (bEnable)
	{
		pDevice->SetAlphaRef(dwRefValue);
		pDevice->SetAlphaFunction(CompFunc);
		pDevice->SetAlphaTestEnable(true);
	}
	else
		pDevice->SetAlphaTestEnable(false);
}

//	Reset environment
void CMLGame::ResetEnvironment()
{
	assert(m_pA3DEngine);

	A3DDevice* pDevice = m_pA3DEngine->GetA3DDevice();
	A3DLight* pLight = GetA3DLight();

	if (pLight)
	{
		//	Change ambient light
		pDevice->SetAmbient(0xff808080);
		
		//	Change directional light
		A3DLIGHTPARAM Params = pLight->GetLightparam();
		Params.Diffuse		= A3DCOLORVALUE(0.5f, 0.5f, 0.5f, 1.0f);
		Params.Direction	= Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
		pLight->SetLightParam(&Params);
	}

	//	Disable fog may cause mistaking render result on some cards ?
	pDevice->SetFogEnable(true);
	pDevice->SetFogStart(1000.0f);
	pDevice->SetFogEnd(1100.0f);
}

bool CMLGame::InitRainSnow()
{
	if (!AGame::InitRainSnow())
		return false;

	if ((m_pLightning = new A3DLightning))
	{
		m_pLightning->Init(m_pA3DEngine->GetA3DDevice(), 10.0f, 100.0f, 150, 4);
	}

	m_pA3DRain->Stop();
	m_pA3DSnow->Stop();

	return true;
}

bool CMLGame::ReleaseRainSnow()
{
	RELEASE(m_pLightning);
	return AGame::ReleaseRainSnow();
}

bool CMLGame::Reset()
{
	if (!AGame::Reset())
		return false;

	//	Re-initialize weather effect, otherwise error will occur
	ReleaseRainSnow();
	InitRainSnow();

	return true;
}

