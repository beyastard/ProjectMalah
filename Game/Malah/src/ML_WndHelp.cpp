/*
 * FILE: ML_WndHelp.cpp
 *
 * DESCRIPTION: Help interface for Malah game
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
#include "ML_WndHelp.h"
#include "ML_WndManager.h"
#include "jexception.h"

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
//	Implement CMLWndHelp
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndHelp, AWnd, 1)

CMLWndHelp::CMLWndHelp()
{
	m_pBackimage	= NULL;
	m_bVisible		= false;
	m_bKeyPreview	= true;
	m_bCanBeFocus	= true;
}

CMLWndHelp::~CMLWndHelp()
{
}

bool CMLWndHelp::OnCreate()
{
	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndHelp::OnDestroy()
{
	AWnd::OnDestroy();
}

bool CMLWndHelp::OnDraw()
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

	m_btnUnitHelp.EnableWnd(!m_wndUnitHelp.IsVisible());
	m_btnPlayHelp.EnableWnd(!m_wndPlayHelp.IsVisible());

	return true;
}

bool CMLWndHelp::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::OnKeyboardMsg(Msg, bAccept))
		return false;

	bool bDown, bChanged;
	Msg.GetButtonStatus(AKB_ESC, bDown, bChanged);

	if (!bDown && bChanged)
		OnBtnClickBack(&m_btnBack);
	
	return true;
}

bool CMLWndHelp::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	//	How to play window
	if (!m_wndPlayHelp.LoadFromIni(this, IniFile, "wnd_playhelp"))
	{
		ADEBUG_TRACE("Cannot create play help window.");
		return false;
	}

	//	Unit help window
	if (!m_wndUnitHelp.LoadFromIni(this, IniFile, "wnd_unithelp"))
	{
		ADEBUG_TRACE("Cannot create unit help window.");
		return false;
	}

	//	§Ú§ç¨[§á
	if (!m_btnOK.LoadFromIni(this, IniFile, "wnd_help::button_back"))
	{
		ADEBUG_TRACE("Cannot create Back button.");
		return false;
	}

	m_btnOK.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndHelp::OnBtnClickOK);

	//	How to play button
	if (!m_btnPlayHelp.LoadFromIni(this, IniFile, "wnd_help::button_playhelp"))
	{
		ADEBUG_TRACE("Cannot create play help button.");
		return false;
	}

	m_btnPlayHelp.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndHelp::OnBtnClickPlayHelp);

	//	Unit help button
	if (!m_btnUnitHelp.LoadFromIni(this, IniFile, "wnd_help::button_unithelp"))
	{
		ADEBUG_TRACE("Cannot create unit help button.");
		return false;
	}

	m_btnUnitHelp.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndHelp::OnBtnClickUnitHelp);

	return true;
}

bool CMLWndHelp::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;

	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
											   m_rArea.GetHeight(),	0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}

	return true;
}

bool CMLWndHelp::FreeRes()
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

bool CMLWndHelp::OnBtnClickBack(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	ShowWindow(false);
	return true;
}

bool CMLWndHelp::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
	{
		AWC::GetWC()->SetFocus(this);
		OnBtnClickPlayHelp(NULL);
	}

	return true;
}

bool CMLWndHelp::OnBtnClickOK(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	ShowWindow(false);
	return true;
}

bool CMLWndHelp::OnBtnClickPlayHelp(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	m_wndPlayHelp.ShowWindow(true);
	m_wndUnitHelp.ShowWindow(false);
	return true;
}

bool CMLWndHelp::OnBtnClickUnitHelp(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	m_wndPlayHelp.ShowWindow(false);
	m_wndUnitHelp.ShowWindow(true);
	return true;
}

