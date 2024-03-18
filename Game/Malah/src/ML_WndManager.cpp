/*
 * FILE: ML_WndManager.cpp
 *
 * DESCRIPTION: Window manager for Malah game
 *
 * CREATED BY: Duyuxin, 2003/1/13
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Interface.h"
#include "ML_WndManager.h"
#include "ML_WndConsole.h"
#include "ML_GamePlay.h"
#include "ML_Resources.h"
#include "AWndSprite.h"

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
//	Implement CMLMainWnd
//
///////////////////////////////////////////////////////////////////////////

class CMLMainWnd : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLMainWnd)

public:		//	Constructor and Destructor

	CMLMainWnd() {}
	virtual ~CMLMainWnd() {}

public:		//	Operations

protected:	//	Attributes

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnCreate() { return true; }
	virtual void	OnDestroy()	{}
};

IMPLEMENT_ASERIAL(CMLMainWnd, AWnd, 1)

bool CMLMainWnd::OnDraw()
{
	if (!AWnd::OnDraw())
		return false;
	
	return true;
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndManager
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndManager, AWC, 1)

CMLWndManager::CMLWndManager() : AWC()
{
	m_bShowCursor	= true;
	m_pCursor		= NULL;
	m_iCurVideo		= VIDEO_LOGO;
}

CMLWndManager::~CMLWndManager()
{
}

//	Initailize object
bool CMLWndManager::Init(const AWC_INIT& wci)
{
	m_nDefFont = ML_GetAppropriateFont(16);

	if (!AWC::Init(wci))
		return false;

	//	Load cursor ...
	A3DRECT	rCursor(0, 0, 32, 32);
	A3DPOINT2 rPoint(0, 0);

	m_pCursor = new A3DCursor;

	if (!m_pCursor->Init(m_pAGame->GetA3DEngine()->GetA3DDevice(), "interface\\Cursor.tga",
						 32, 32, 0, 1, &rCursor, rPoint))
	{
		m_pCursor->Release();
		delete m_pCursor;
		m_pCursor = NULL;

		ADEBUG_TRACE("Cannot create cursor.");
		return false;
	}

	//	Create windows ...
	ARECT rcConsole(0, 0, 600, 400);
	if (!m_WndConsole.Create(m_pMainWnd, rcConsole))
	{
		ADEBUG_TRACE("Cannot create console interface.");
		return false;
	}

	if (!m_WndLoading.Create(m_pMainWnd, m_pMainWnd->GetArea()))
	{
		ADEBUG_TRACE("Cannot create loading interface.");
		return false;
	}

	if (!m_wndVideo.Create(m_pMainWnd, m_pMainWnd->GetArea()))
	{
		ADEBUG_TRACE("Cannot create video interface.");
		return false;
	}

	ADEBUG_TRACE("Succeed Init AEnyaWC");

	return true;
}

//	Release resources
bool CMLWndManager::Release()
{
	if (m_pCursor)
	{
		m_pCursor->Release();
		delete m_pCursor;
		m_pCursor = NULL;
	}

	m_pWC = this;

	return AWC::Release();
}

//	Draw
bool CMLWndManager::Draw()
{
	if (!AWC::Draw())
		return false;
	
	if (m_pActiveInterface && m_pActiveInterface != &m_WndConsole)
	{
		if (m_pCursor != NULL && m_bShowCursor)
		{
			m_pCursor->TickAnimation();
			m_pCursor->Draw();
		}
	}

	return true;
}

//	Set display mode
bool CMLWndManager::SetDisplayMode(int iWid, int iHei, int iColorBit)
{
	m_pWC = this;

	if (!GETMLGAMEPLAY->ResetScreen(iWid, iHei, iColorBit))
	{
		ADEBUG_TRACE3("Cannot set display mode %dx%d %dbit.", iWid, iHei, iColorBit);
		return false;
	}

	m_fScaleX = iWid / 800.0f;
	m_fScaleY = iHei / 600.0f;

	return true;
}

//	Set active state
int	CMLWndManager::SetActiveStatus(int iActive)
{
	int iPre = AWC::SetActiveStatus(iActive);
	if (iPre == iActive)
		return iPre;

	if (m_pActiveInterface)
		m_pActiveInterface->ShowWindow(false);
	
	switch (iActive)
	{
	case MLIG_CONSOLE:

		ActivateWndConsole(true);
		break;

	case MLIG_LOADING:

		ActivateWndLoading(true);
		break;

	case MLIG_ESCAPE:

		ActivateWndEscape(true);
		break;

	case MLIG_PASS:

		ActivateWndVictory(true);
		break;

	case MLIG_NOTPASS:

		ActivateWndFailure(true);
		break;

	case MLIG_VICTORY:

		ActivateWndFinalWin(true);
		break;

	case MLIG_SCORE:
		
		ActivateWndScore(true);
		break;

	case MLIG_VIDEO:

		ActivateWndVideo(true);
		break;

	case MLIG_NONE:
	default:
	{
		if (m_pActiveInterface == &m_WndConsole)
			ActivateWndConsole(false);
		else if (m_pActiveInterface == &m_WndLoading)
			ActivateWndLoading(false);
		else if (m_pActiveInterface == &m_WndEscape)
			ActivateWndEscape(false);
		else if (m_pActiveInterface == &m_WndVictory)
			ActivateWndVictory(false);
		else if (m_pActiveInterface == &m_WndFailure)
			ActivateWndFailure(false);
		else if (m_pActiveInterface == &m_WndScore)
			ActivateWndScore(false);
		else if (m_pActiveInterface == &m_WndFinalWin)
			ActivateWndFinalWin(false);
		else if (m_pActiveInterface == &m_wndVideo)
			ActivateWndVideo(false);

		m_pActiveInterface = NULL;
		AWC::GetWC()->SetFocus(NULL);

		break;
	}
	}

	return iPre;
}

//	Activate / Deactivate console 
bool CMLWndManager::ActivateWndConsole(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndConsole;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

	//	if(m_pAGame->GetADeviceMouse())
	//		m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndConsole.ShowWindow(true);
		AWC::SetFocus(&m_WndConsole);
	}
	else
	{
		m_WndConsole.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
	//	m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(NULL);
		GETMLGAMEPLAY->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);
	}

	return true;
}

//	Activate loading interface
bool CMLWndManager::ActivateWndLoading(bool bActive)
{
	if (bActive)
	{
		if (!m_WndLoading.LoadRes())
		{
			ADEBUG_TRACE("failed to load loadingwindow resource.");
			return false;
		}

		m_pActiveInterface = &m_WndLoading;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndLoading.ShowWindow(true);

		g_theGame.GetA3DAssistTools()->BM_SetLoop(true);
		g_theGame.GetA3DAssistTools()->BM_Play("music\\mainmenu.mp3");
	}
	else
	{
		m_WndLoading.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);
		
		m_WndLoading.FreeRes();

		g_theGame.GetA3DAssistTools()->BM_Stop();
	}

	return true;
}

//	Activate escape panel
bool CMLWndManager::ActivateWndEscape(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndEscape;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndEscape.SetYesNoMsgBox(&m_YesNoMegBox);
		m_WndEscape.ShowWindow(true);
	}
	else
	{
		m_WndEscape.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);
	}

	return true;
}

//	Activate victory panel
bool CMLWndManager::ActivateWndVictory(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndVictory;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndVictory.SetYesNoMsgBox(&m_YesNoMegBox);
		m_WndVictory.ShowWindow(true);

		g_theGame.GetA3DAssistTools()->BM_SetLoop(false);
		g_theGame.GetA3DAssistTools()->BM_Play("music\\victory.wav");
		g_theGame.Play2DSound(RES_S2D_MISSIONCOMPLETED1+MLA_Random(0, 2), false);
	}
	else
	{
		m_WndVictory.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

		g_theGame.GetA3DAssistTools()->BM_Stop();
	}

	return true;
}

//	Activate failure panel
bool CMLWndManager::ActivateWndFailure(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndFailure;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndFailure.SetYesNoMsgBox(&m_YesNoMegBox);
		m_WndFailure.ShowWindow(true);

		g_theGame.GetA3DAssistTools()->BM_SetLoop(false);
		g_theGame.GetA3DAssistTools()->BM_Play("music\\failure.wav");
		g_theGame.Play2DSound(RES_S2D_MISSIONFAILED1+MLA_Random(0, 2), false);
	}
	else
	{
		m_WndFailure.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

		g_theGame.GetA3DAssistTools()->BM_Stop();
	}

	return true;
}

//	Activate victory panel
bool CMLWndManager::ActivateWndFinalWin(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndFinalWin;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndFinalWin.SetYesNoMsgBox(&m_YesNoMegBox);
		m_WndFinalWin.ShowWindow(true);

		g_theGame.GetA3DAssistTools()->BM_SetLoop(false);
		g_theGame.GetA3DAssistTools()->BM_Play("music\\victory.wav");
	//	g_theGame.Play2DSound(RES_S2D_MISSIONFAILED1+MLA_Random(0, 2), false);
	}
	else
	{
		m_WndFinalWin.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

	//	g_theGame.GetA3DAssistTools()->BM_Stop();
	}

	return true;
}

//	Activate score window
bool CMLWndManager::ActivateWndScore(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_WndScore;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_WndScore.ShowWindow(true);
	}
	else
	{
		m_WndScore.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());
		m_pAGame->GetAGamePlay()->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);
	}

	return true;
}

//	Activate game video window
bool CMLWndManager::ActivateWndVideo(bool bActive)
{
	if (bActive)
	{
		m_pActiveInterface = &m_wndVideo;
		
		if (m_pAGame->GetADeviceKeyboard())
			m_pAGame->GetADeviceKeyboard()->SetTrans(m_pTransKeyboard);

		if (m_pAGame->GetADeviceMouse())
			m_pAGame->GetADeviceMouse()->SetTrans(m_pTransMouse);
		
		m_wndVideo.ShowWindow(true);

		g_theGame.GetA3DAssistTools()->BM_Pause(true);
	}
	else
	{
		m_wndVideo.ShowWindow(false);
		
		m_pAGame->GetADeviceKeyboard()->SetTrans(NULL);
		m_pAGame->GetADeviceMouse()->SetTrans(NULL);
		
		::SetFocus(GetMainHandle());

		g_theGame.GetA3DAssistTools()->BM_Pause(false);
	}

	return true;
}

//	Add console string
bool CMLWndManager::AddConsoleString(const char* szStr)
{
	m_WndConsole.AddString(szStr);
	return true;
}

bool CMLWndManager::CreateMainWnd()
{
	assert(m_pMainWnd == NULL);

	if (!(m_pMainWnd = new CMLMainWnd))
	{
		assert(0);
		ADEBUG_TRACE("Failed to create main window");
		return false;
	}

	ARECT rect(0, 0, m_pAGame->GetClientWidth(), m_pAGame->GetClientHeight());

	return m_pMainWnd->Create(NULL, rect);
}

bool CMLWndManager::OnVideoStop(AWnd* pWnd)
{
	if (m_pActiveInterface)
		m_pActiveInterface->ShowWindow(true);
	
	return true;
}

//	Start game
bool CMLWndManager::StartGame(int iLevel)
{
	SetActiveStatus(MLIG_NONE);

	CMLGamePlay* pGamePlay = GETMLGAMEPLAY;
	USERCONFIGS* pUserConfig = pGamePlay->GetCurUserConfigs();

	//	Show loading wait picture
	ShowLoadingWait();

	//	Normally, scale factor will be set in Load in SetDisplayMode(). We set
	//	them there in order in game interfaces will be loaded correctly.
	m_fScaleX = pUserConfig->iRenderWid / 800.0f;
	m_fScaleY = pUserConfig->iRenderHei / 600.0f;
	LoadInGameInterface();

	pGamePlay->SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

	//	Start game
	if (iLevel > GETGAMELOGICPARAMS->iMaxMission)
		iLevel = GETGAMELOGICPARAMS->iMaxMission;

	pGamePlay->StartGamePlay(iLevel);

	//	Change display mode
	SetDisplayMode(pUserConfig->iRenderWid, pUserConfig->iRenderHei, pUserConfig->iColorBit);

	return true;
}

//	Load interface used in game
bool CMLWndManager::LoadInGameInterface()
{
	m_pWC = this;

	AWndIniFile	IniFile;
	if (!IniFile.Init("Ini/InGame.ini"))
	{
		ADEBUG_TRACE("Cannot load file /Ini/InGame.ini.");
		return false;
	}

	//	Yes or No message box
	if (!m_YesNoMegBox.LoadFromIni(GetMainWnd(), IniFile, "wnd_messagebox"))
	{
		ADEBUG_TRACE("Cannot load yes or no message box");
		return false;
	}

	m_YesNoMegBox.LoadRes();
	m_YesNoMegBox.ShowWindow(false);

	//	Escape panel
	if (!m_WndEscape.LoadFromIni(GetMainWnd(), IniFile, "wnd_escape"))
	{
		ADEBUG_TRACE("Cannot load escape panel window");
		return false;
	}

	m_WndEscape.LoadRes();

	//	Victory panel
	if (!m_WndVictory.LoadFromIni(GetMainWnd(), IniFile, "wnd_victory"))
	{
		ADEBUG_TRACE("Cannot load victory panel window");
		return false;
	}

	m_WndVictory.LoadRes();

	//	Failure panel
	if (!m_WndFailure.LoadFromIni(GetMainWnd(), IniFile, "wnd_failure"))
	{
		ADEBUG_TRACE("Cannot load failure panel window");
		return false;
	}

	m_WndFailure.LoadRes();

	//	Final win panel
	if (!m_WndFinalWin.LoadFromIni(GetMainWnd(), IniFile, "wnd_finalwin"))
	{
		ADEBUG_TRACE("Cannot load final win panel window");
		return false;
	}

	m_WndFinalWin.LoadRes();

	IniFile.Save();
	IniFile.Release();

	if (!IniFile.Init("Ini/MainMenu.ini"))
	{
		ADEBUG_TRACE("Cannot load file /Ini/InGame.ini.");
		return false;
	}

	if (!m_WndScore.LoadFromIni(GetMainWnd(), IniFile, "wnd_score"))
	{
		ADEBUG_TRACE("Cannot create scoreboard window.");
		return false;
	}

	m_WndScore.EnableAutoLoadRes(true);

	IniFile.Release();

	return true;
}

//	Free interface used in game
void CMLWndManager::FreeInGameInterface()
{
	m_pWC = this;

	m_YesNoMegBox.FreeRes();
	m_YesNoMegBox.Release();

	m_WndEscape.FreeRes();
	m_WndEscape.Release();

	m_WndVictory.FreeRes();
	m_WndVictory.Release();

	m_WndFailure.FreeRes();
	m_WndFailure.Release();

	m_WndFinalWin.FreeRes();
	m_WndFinalWin.Release();

	m_WndScore.Release();
}

//	Return to main menu from game
void CMLWndManager::ReturnToMainMenu()
{
	USERCONFIGS* pUserConfig = GETMLGAMEPLAY->GetCurUserConfigs();
	SetDisplayMode(BASERENDER_WIDTH, BASERENDER_HEIGHT, pUserConfig->iColorBit);

	GETMLGAMEPLAY->SetActiveStatus(CMLGamePlay::GPS_NOTACTIVE);
	SetActiveStatus(MLIG_SCORE);
}

//	Show loading wait picture
void CMLWndManager::ShowLoadingWait()
{
	ARECT rcSprite(0, 0, 800, 600);
	char* szImg = "Interface\\MainMenu\\Loading01.bmp";

	A2DSprite* pBackimage = AWndSpriteMan::CreateSprite(szImg, 800, 600, 0, 1, &rcSprite);
	if (!pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", szImg);
		return;
	}

	A3DViewport* pA3DViewport = g_theGame.GetA3DViewport();
	A3DEngine* pA3DEngine = g_theGame.GetA3DEngine();
	assert(pA3DViewport && pA3DEngine);

	if (!pA3DEngine->BeginRender())
		return;

	pA3DViewport->Active();
//	pA3DViewport->ClearDevice();

	APOINT ptAb(0, 0);

	DrawSprite(pBackimage, ptAb);

	pA3DEngine->EndRender();
	pA3DEngine->Present();
	
	AWndSpriteMan::ReleaseSprite(pBackimage);
}

