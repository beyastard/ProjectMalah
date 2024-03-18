/*
 * FILE: ML_WndLoadGame.cpp
 *
 * DESCRIPTION: Load game interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "A2DSprite.h"
#include "AWndSprite.h"

#include "ML_WndLoadGame.h"
#include "ML_WndManager.h"
#include "ML_Gameplay.h"
#include "jexception.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


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

extern char		g_szSettingFile[];

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndLoadGame
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndLoadGame, AWnd, 1)

CMLWndLoadGame::CMLWndLoadGame()
{
	m_pBackimage	= NULL;
	m_bVisible		= false;
	m_bCanBeFocus	= true;
	m_fOldMission	= 0.0f;
}

CMLWndLoadGame::~CMLWndLoadGame()
{
}

bool CMLWndLoadGame::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndLoadGame::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndLoadGame::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	APOINT ptAb;
	GetAbsolutePos(ptAb);

	if (m_pBackimage)
	{
		if (!AWC::DrawSprite(m_pBackimage, ptAb))
		{
			ADEBUG_TRACE("Cannot draw to back.");
		}
	}

	if (m_fOldMission != m_scrMission.GetScrollPos())
	{
		m_fOldMission = m_scrMission.GetScrollPos();
		int iFirst = (int)(m_scrMission.GetRangeSize() * m_scrMission.GetScrollPos() / 
						   m_scrMission.GetScrollLength());
		m_MissionList.SetFirstVisible(iFirst);
	}

	return true;
}

bool CMLWndLoadGame::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	if (!m_btnOk.LoadFromIni(this, IniFile, "wnd_loadgame::button_back"))
	{
		ADEBUG_TRACE("cannot load button wnd_loadgame::button_back");
		return false;
	}

	m_btnOk.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndLoadGame::OnBtnClickBack);

	if (!m_btnLoad.LoadFromIni(this, IniFile, "wnd_loadgame::button_load"))
	{
		ADEBUG_TRACE("cannot load button wnd_loadgame::load");
		return false;
	}

	m_btnLoad.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndLoadGame::OnBtnClickLoad);

	m_MissionList.SetReadItemCount(NUM_GAMELEVEL);
	m_MissionList.m_nScrollWidth = 0;

	if (!m_MissionList.LoadFromIni(this, IniFile, "wnd_loadgame::list_mission"))
	{
		ADEBUG_TRACE("cannot load wnd_loadgame::list_mission");
		return false;
	}

	m_MissionList.SetCurSel(0);

	if (!m_scrMission.LoadFromIni(this, IniFile, "wnd_loadgame::scrollbar_mission"))
	{
		ADEBUG_TRACE("Cannot load scroll bar mission.");
		return false;
	}

	m_scrMission.SetVertical(true);
	m_scrMission.SetScrollPos(0.0f);

	return true;
}

bool CMLWndLoadGame::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
											   m_rArea.GetHeight(), 0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}

	//	Get max level
	AWndIniFile IniList;
	m_iMaxLevel = 1;
	if (IniList.Init(g_szSettingFile))
	{
		if (IniList.ReadInt("record", "maxlevel", m_iMaxLevel))
			m_iMaxLevel += 1;

		IniList.Release();
	}

	if (m_iMaxLevel > GETGAMELOGICPARAMS->iMaxMission + 1)
		m_iMaxLevel = GETGAMELOGICPARAMS->iMaxMission + 1;

	//m_MissionList.SetLimitItemNum(m_iMaxLevel);
	m_scrMission.SetMinData(0.0f);
	m_scrMission.SetMaxData(m_iMaxLevel > 18 ? m_iMaxLevel - 18.0f : 0.0f);

	if (m_iMaxLevel <= 18)
	{
		m_scrMission.ShowWindow(false);
		m_scrMission.EnableWnd(false);
	}
	else
	{
		m_scrMission.ShowWindow(true);
		m_scrMission.EnableWnd(true);
	}
	
	return true;
}

bool CMLWndLoadGame::FreeRes()
{
	J_EXCEPTION_RECORD(0);

	bool bRet = true;

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

	return bRet && AWnd::FreeRes();
}

bool CMLWndLoadGame::OnBtnClickBack(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	ShowWindow(false);
	return true;
}

bool CMLWndLoadGame::OnBtnClickLoad(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	int iSel = m_MissionList.GetCurSel();
	if (iSel < 0 || iSel >= m_MissionList.GetItemCount())
		return false;

	ShowWindow(false);

	//	Start new game
	CMLWndManager* pWndMan = (CMLWndManager*)CMLWndManager::GetWC();
	pWndMan->StartGame(iSel);
	return true;
}

