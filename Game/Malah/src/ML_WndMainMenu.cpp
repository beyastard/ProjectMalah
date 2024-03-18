/*
 * FILE: ML_WndMainMenu.cpp
 *
 * DESCRIPTION: Main menu interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "ML_Interface.h"
#include "ML_WndMainMenu.h"
#include "ML_WndManager.h"
#include "ML_GamePlay.h"
#include "AWndSprite.h"
#include <jexception.h>

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
//	Implement CMLWndMainMenu
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndMainMenu, AWnd, 1)

CMLWndMainMenu::CMLWndMainMenu()
{
	m_pBackimage	= NULL;
	m_pCurWnd		= NULL;
//	m_bVisible		= false;
}

CMLWndMainMenu::~CMLWndMainMenu()
{
}

bool CMLWndMainMenu::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;

	const char* pszBackimage = NULL;
	if (!IniFile.ReadString(pszSection, "image_back", pszBackimage))
	{
		ADEBUG_TRACE("cannot read back image.");
		return false;
	}

	m_strBackimage = pszBackimage;

	//	Copright string
	if (!IniFile.ReadString(pszSection, "copyright", pszBackimage))
		m_strCopyright = "Copyright(c) E-Pie Entertainment Corporation, Chinese Kungfu Studio";
	else
		m_strCopyright = pszBackimage;

	//	Buttons loading ...
	if (!m_btnNewGame.LoadFromIni(this, IniFile, "button_newgame"))
	{
		ADEBUG_TRACE("Cannot create NewGame button.");
		return false;
	}

	m_btnNewGame.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickNewGame);

	if (!m_btnLoadGame.LoadFromIni(this, IniFile, "button_loadgame"))
	{
		ADEBUG_TRACE("Cannot create LoadGame button.");
		return false;
	}

	m_btnLoadGame.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickLoadGame);

	if (!m_btnScore.LoadFromIni(this, IniFile, "button_score"))
	{
		ADEBUG_TRACE("Cannot create Score button.");
		return false;
	}

	m_btnScore.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickScore);

	if (!m_btnHelp.LoadFromIni(this, IniFile, "button_help"))
	{
		ADEBUG_TRACE("Cannot create Help button.");
		return false;
	}

	m_btnHelp.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickHelp);

	if (!m_btnCredits.LoadFromIni(this, IniFile, "button_credits"))
	{
		ADEBUG_TRACE("Cannot create Credits button.");
		return false;
	}

	m_btnCredits.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickCredits);

	if (!m_btnSetup.LoadFromIni(this, IniFile, "button_setup"))
	{
		ADEBUG_TRACE("Cannot create Setup button.");
		return false;
	}

	m_btnSetup.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickSetup);

	if (!m_btnQuit.LoadFromIni(this, IniFile, "button_quit"))
	{
		ADEBUG_TRACE("Cannot create Quit button.");
		return false;
	}

	m_btnQuit.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndMainMenu::OnBtnClickQuit);

	//	Windows loading ...
	if (!m_wndSetting.LoadFromIni(this, IniFile, "wnd_setting"))
	{
		ADEBUG_TRACE("Cannot create setting window.");
		return false;
	}

	m_wndSetting.LoadSetting();
	m_wndSetting.SaveSetting();

	if (!m_wndScore.LoadFromIni(this, IniFile, "wnd_score"))
	{
		ADEBUG_TRACE("Cannot create scoreboard window.");
		return false;
	}

	m_wndScore.EnableAutoLoadRes(false);

	if (!m_wndHelp.LoadFromIni(this, IniFile, "wnd_help"))
	{
		ADEBUG_TRACE("Cannot create help window.");
		return false;
	}

	if (!m_wndLoadGame.LoadFromIni(this, IniFile, "wnd_loadgame"))
	{
		ADEBUG_TRACE("Cannot create load game window.");
		return false;
	}

	if (!m_wndCredits.LoadFromIni(this, IniFile, "wnd_credits"))
	{
		ADEBUG_TRACE("Cannot create credits window.");
		return false;
	}

	return true;
}

void CMLWndMainMenu::LoadSetting()
{
}

bool CMLWndMainMenu::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	//	Load score list
	m_wndScore.LoadScoreList();

	AListPosition Pos = m_lstChilds.GetHeadPosition();
	while (Pos)
	{
		AWnd* pChild = m_lstChilds.GetNext(Pos);
		if (pChild == &m_wndSetting || pChild == &m_wndScore ||
			pChild == &m_wndHelp || pChild == &m_wndLoadGame ||
			pChild == &m_wndCredits)
			continue;

		if (!pChild->LoadRes())
		{
			return false;
		}
	}

	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
											   m_rArea.GetHeight(), 0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}
	
	return true;
}

bool CMLWndMainMenu::FreeRes()
{
	J_EXCEPTION_RECORD(0);

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

	if (m_pCurWnd)
	{
		m_pCurWnd->FreeRes();
		m_pCurWnd = NULL;
	}

	AListPosition Pos = m_lstChilds.GetHeadPosition();
	while (Pos)
	{
		AWnd* pChild = m_lstChilds.GetNext(Pos);
		if (pChild == &m_wndSetting || pChild == &m_wndScore ||
			pChild == &m_wndHelp || pChild == &m_wndLoadGame ||
			pChild == &m_wndCredits)
			continue;

		pChild->FreeRes();
	}

	return true;
}

bool CMLWndMainMenu::OnDraw()
{
	J_EXCEPTION_RECORD(0);
	APOINT ptAb;
	GetAbsolutePos(ptAb);
	
	if (m_pCurWnd && m_pCurWnd->IsVisible())
		return true;

	if (m_pBackimage != NULL)
	{
		if (!AWC::DrawSprite(m_pBackimage, ptAb))
			ADEBUG_TRACE("Cannot draw to back.");
	}

	//	Draw copyright information
//	int iFont = ML_GetAppropriateFont(16);
//	g_theGame.TextOut(iFont, 10, BASERENDER_HEIGHT-24, m_strCopyright, A3DCOLORRGB(128, 128, 255), true);

	return true;
}

//	Load window resources
bool CMLWndMainMenu::LoadWindowRes(AWnd* pWnd)
{
	if (m_pCurWnd && m_pCurWnd != pWnd)
	{
		m_pCurWnd->FreeRes();
		m_pCurWnd = NULL;
	}

	if (m_pCurWnd != pWnd)
	{
		if (!pWnd->LoadRes())
		{
			ADEBUG_TRACE("failed to load window resources.");
			return false;
		}
	}

	m_pCurWnd = pWnd;
	return true;
}

bool CMLWndMainMenu::OnBtnClickNewGame(AWnd* pWnd)
{
	CMLWndManager* pWndMan = (CMLWndManager*)CMLWndManager::GetWC();
	pWndMan->StartGame(g_CmdParams.iStartMission);
	return true;
}

bool CMLWndMainMenu::OnBtnClickLoadGame(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	LoadWindowRes(&m_wndLoadGame);
	m_wndLoadGame.ShowWindow(true);
	return true;
}

bool CMLWndMainMenu::OnBtnClickScore(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	LoadWindowRes(&m_wndScore);
	m_wndScore.ShowWindow(true);
	return true;
}

bool CMLWndMainMenu::OnBtnClickHelp(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	LoadWindowRes(&m_wndHelp);
	m_wndHelp.ShowWindow(true);
	return true;
}

bool CMLWndMainMenu::OnBtnClickCredits(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	LoadWindowRes(&m_wndCredits);
	m_wndCredits.ShowWindow(true);
	return true;
}

bool CMLWndMainMenu::OnBtnClickSetup(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	LoadWindowRes(&m_wndSetting);
	m_wndSetting.ShowWindow(true);
	return true;
}

bool CMLWndMainMenu::OnBtnClickQuit(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	MLMSG Msg;

	Msg.idManager	= -1;
	Msg.dwMsg		= MSG_EXITGAME;
	Msg.dwParam1	= EXIT_CODE_NORMAL;

	GETMLGAMEPLAY->OnMsg((AMSG*)&Msg);

	return true;
}

