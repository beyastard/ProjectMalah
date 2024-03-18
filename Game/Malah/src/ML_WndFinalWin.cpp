/*
 * FILE: ML_WndFinalWin.cpp
 *
 * DESCRIPTION: Final win window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

//	#include "ML_Global.h"
#include "ML_WndFinalWin.h"
#include "A2DSprite.h"
#include "AWndSprite.h"
//#include "AWIniFile.h"
#include "ML_Interface.h"
#include "ML_GamePlay.h"
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
//	Implement CMLWndFinalWin
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndFinalWin, AWnd, 1)

CMLWndFinalWin::CMLWndFinalWin()
{
	m_bVisible		= false;
	m_bKeyPreview	= true;
	m_bCanBeFocus	= true;
	m_pBackimage	= 0;
	m_pYesNoMsgBox	= NULL;
}

CMLWndFinalWin::~CMLWndFinalWin()
{
}

bool CMLWndFinalWin::OnDraw()
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

	return true;
}

bool CMLWndFinalWin::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	if (!m_btnReplay.LoadFromIni(this, IniFile, "wnd_finalwin::button_replay"))
	{
		ADEBUG_TRACE("cannot load replay button.");
		return false;
	}

	m_btnReplay.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndFinalWin::OnReplayClick);

	if (!m_btnExit.LoadFromIni(this, IniFile, "wnd_finalwin::button_exit"))
	{
		ADEBUG_TRACE("cannot load exit button.");
		return false;
	}

	m_btnExit.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndFinalWin::OnExitClick);

	return true;
}

bool CMLWndFinalWin::LoadRes()
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

	return true;
}

bool CMLWndFinalWin::FreeRes()
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

bool CMLWndFinalWin::OnReplayClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	AWC::GetWC()->SetActiveStatus(0);
	GETMLGAMEPLAY->ReplayMission(true);
	return true;
}

bool CMLWndFinalWin::OnExitClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(true);
	ShowWindow(false);
	
	return true;
}

bool CMLWndFinalWin::OnYesClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);

	AWC::GetWC()->SetActiveStatus(MLIG_NONE);
	((CMLWndManager*)AWC::GetWC())->ReturnToMainMenu();

	return true;
}

bool CMLWndFinalWin::OnNoClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);
	ShowWindow(true);

	return true;
}

bool CMLWndFinalWin::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
		AWC::GetWC()->SetFocus(this);
	
	return true;
}

void CMLWndFinalWin::SetYesNoMsgBox(CMLMessageBox* pMsgBox)
{
	if (!(m_pYesNoMsgBox = pMsgBox))
		return;

	m_pYesNoMsgBox->m_btnYes.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndFinalWin::OnYesClick);
	m_pYesNoMsgBox->m_btnNo.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndFinalWin::OnNoClick);
	m_pYesNoMsgBox->ShowWindow(false);
}

