/*
 * FILE: ML_WndVictory.cpp
 *
 * DESCRIPTION: Victory (pass scene) window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

//	#include "ML_Global.h"
#include "ML_WndVictory.h"
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
//	Implement CMLWndVictory
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndVictory, AWnd, 1)

CMLWndVictory::CMLWndVictory()
{
	m_bVisible		= false;
	m_bKeyPreview	= true;
	m_bCanBeFocus	= true;
	m_pBackimage	= 0;
	m_pYesNoMsgBox	= NULL;
}

CMLWndVictory::~CMLWndVictory()
{
}

bool CMLWndVictory::OnDraw()
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

bool CMLWndVictory::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

/*	if (!m_btnReplay.LoadFromIni(this, IniFile, "wnd_victory::button_replay"))
	{
		ADEBUG_TRACE("cannot load replay button.");
		return false;
	}

	m_btnReplay.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)OnReplayClick);
*/
	if (!m_btnExit.LoadFromIni(this, IniFile, "wnd_victory::button_exit"))
	{
		ADEBUG_TRACE("cannot load exit button.");
		return false;
	}

	m_btnExit.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndVictory::OnExitClick);

	if (!m_btnContinue.LoadFromIni(this, IniFile, "wnd_victory::button_continue"))
	{
		ADEBUG_TRACE("cannot load continue button.");
		return false;
	}

	m_btnContinue.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndVictory::OnContinueClick);

	return true;
}

bool CMLWndVictory::LoadRes()
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

bool CMLWndVictory::FreeRes()
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

bool CMLWndVictory::OnReplayClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	AWC::GetWC()->SetActiveStatus(0);
	GETMLGAMEPLAY->ReplayMission(false);
	return true;
}

bool CMLWndVictory::OnExitClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(true);
	ShowWindow(false);
	
	return true;
}

bool CMLWndVictory::OnContinueClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	AWC::GetWC()->SetActiveStatus(0);
	GETMLGAMEPLAY->PassMission();
	return true;
}

bool CMLWndVictory::OnYesClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);

	AWC::GetWC()->SetActiveStatus(MLIG_NONE);
	((CMLWndManager*)AWC::GetWC())->ReturnToMainMenu();

	return true;
}

bool CMLWndVictory::OnNoClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);
	ShowWindow(true);

	return true;
}

bool CMLWndVictory::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
		AWC::GetWC()->SetFocus(this);
	
	return true;
}

void CMLWndVictory::SetYesNoMsgBox(CMLMessageBox* pMsgBox)
{
	if (!(m_pYesNoMsgBox = pMsgBox))
		return;

	m_pYesNoMsgBox->m_btnYes.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndVictory::OnYesClick);
	m_pYesNoMsgBox->m_btnNo.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndVictory::OnNoClick);
	m_pYesNoMsgBox->ShowWindow(false);
}
