/*
 * FILE: ML_WndESPPopup.cpp
 *
 * DESCRIPTION: ESP popup window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_WndESCPopup.h"
#include "A3DAssistTools.h"
#include "A2DSprite.h"
#include "AWndSprite.h"
//#include "AWIniFile.h"
#include "ML_Interface.h"
#include "ML_GamePlay.h"
#include "ML_WndManager.h"
#include "ML_MessageBox.h"
#include "jexception.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////

extern char	g_szSettingFile[];

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
//	Implement CMLWndESCPopup
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndESCPopup, AWnd, 1)

CMLWndESCPopup::CMLWndESCPopup()
{
	m_bVisible			= false;
	m_bKeyPreview		= true;
	m_bCanBeFocus		= true;
	m_fOldSoundPos		= 0;
	m_fOldMusicPos		= 0;
	m_fOldBrightnessPos	= 0;
	m_pBackimage		= 0;
	m_pYesNoMsgBox		= NULL;
	m_iYesNoTrigger		= YESNO_EXIT;

	m_scrSound.SetBackColor(0);
//	m_scrMusic.SetBackColor(0);
	m_scrBrightness.SetBackColor(0);
}

CMLWndESCPopup::~CMLWndESCPopup()
{
}

bool CMLWndESCPopup::ShowStat()
{
	J_EXCEPTION_RECORD(0);
//	ShowWindow(false);
//	return m_wndStat.ShowWindow(true);
	return true;
}

bool CMLWndESCPopup::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::OnKeyboardMsg(Msg, bAccept))
		return false;
	
	bool bDown, bChanged;
	Msg.GetButtonStatus(AKB_ESC, bDown, bChanged);

	if (bDown && bChanged)
	{
		if (AWC::GetWC()->GetFocus() == this)
			AWC::GetWC()->SetActiveStatus(0);
	}

	return true;
}

bool CMLWndESCPopup::OnDraw()
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

	if (m_fOldSoundPos != m_scrSound.GetScrollPos())
	{
		m_fOldSoundPos	= m_scrSound.GetScrollPos();
		float fVolume	= 100.0f*m_fOldSoundPos/m_scrSound.GetScrollLength();
		m_pAGame->GetA3DAssistTools()->S_SetVolume((UINT)fVolume);
	}

/*	if (m_fOldMusicPos != m_scrMusic.GetScrollPos())
	{
		m_fOldMusicPos	= m_scrMusic.GetScrollPos();
		float fVolume	= 100.0f*m_fOldMusicPos/m_scrMusic.GetScrollLength();
		m_pAGame->GetA3DAssistTools()->BM_SetVolume((UINT)fVolume);
	}
*/
	if (m_fOldBrightnessPos != m_scrBrightness.GetScrollPos())
	{
		m_fOldBrightnessPos	= m_scrBrightness.GetScrollPos();
		float fVolume		= 200.0f*m_scrBrightness.GetScrollPos()/m_scrBrightness.GetScrollLength();
		m_pAGame->GetA3DEngine()->GetA3DDevice()->SetGammaLevel((UINT)fVolume);
	}

	return true;
}

bool CMLWndESCPopup::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	if (!m_scrSound.LoadFromIni(this, IniFile, "wnd_escape::scrollbar_sound"))
	{
		ADEBUG_TRACE("Cannot load scroll bar sound.");
		return false;
	}

	m_fOldSoundPos	= m_scrSound.GetScrollPos();

/*	if (!m_scrMusic.LoadFromIni(this, IniFile, "wnd_escape::scrollbar_music"))
	{
		ADEBUG_TRACE("Cannot load scroll bar music.");
		return false;
	}

	m_fOldMusicPos	= m_scrMusic.GetScrollPos();
*/
	if (!m_scrBrightness.LoadFromIni(this, IniFile, "wnd_escape::scrollbar_brightness"))
	{
		ADEBUG_TRACE("Cannot load scroll bar brightness.");
		return false;
	}

	m_fOldBrightnessPos	= m_scrBrightness.GetScrollPos();

	if (!m_btnBack.LoadFromIni(this, IniFile, "wnd_escape::button_back"))
	{
		ADEBUG_TRACE("cannot load back button.");
		return false;
	}

	m_btnBack.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndESCPopup::OnBackClick);

	if (!m_btnDefault.LoadFromIni(this, IniFile, "wnd_escape::button_restart"))
	{
		ADEBUG_TRACE("cannot load restart button.");
		return false;
	}

	m_btnDefault.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndESCPopup::OnRestartClick);

	if (!m_btnExit.LoadFromIni(this, IniFile, "wnd_escape::button_exit"))
	{
		ADEBUG_TRACE("cannot load exit button.");
		return false;
	}

	m_btnExit.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndESCPopup::OnExitClick);

	LoadSettings();

	return true;
}

bool CMLWndESCPopup::LoadSettings()
{
	J_EXCEPTION_RECORD(0);

	AWndIniFile	IniFile;
	if (!IniFile.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot load options inifile.");
		return false;
	}

	READ_SCROLL(SV, m_scrSound);
//	READ_SCROLL(MV, m_scrMusic);
	READ_SCROLL(BR, m_scrBrightness);

	ApplySettings();

	IniFile.Release();

	return true;
}

bool CMLWndESCPopup::SaveSettings()
{
	J_EXCEPTION_RECORD(0);

	ApplySettings();

	AWndIniFile	IniFile;
	if (!IniFile.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot load options inifile.");
		return false;
	}

	WRITE_SCROLL(SV, m_scrSound);
//	WRITE_SCROLL(MV, m_scrMusic);
	WRITE_SCROLL(BR, m_scrBrightness);

	IniFile.Save();
	IniFile.Release();

	return true;
}

bool CMLWndESCPopup::ApplySettings()
{
	J_EXCEPTION_RECORD(0);

	float fVolume;

	fVolume	= 100.0f*m_scrSound.GetScrollPos()/m_scrSound.GetScrollLength();
	m_pAGame->GetA3DAssistTools()->S_SetVolume((UINT)fVolume);

//	fVolume	= 100.0f*m_scrMusic.GetScrollPos()/m_scrMusic.GetScrollLength();
//	m_pAGame->GetA3DAssistTools()->BM_SetVolume((UINT)fVolume);

	fVolume	= 200.0f*m_scrBrightness.GetScrollPos()/m_scrBrightness.GetScrollLength();
	m_pAGame->GetA3DEngine()->GetA3DDevice()->SetGammaLevel((UINT)fVolume);

	return true;
}

bool CMLWndESCPopup::OnYesClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);

	AWC::GetWC()->SetActiveStatus(MLIG_NONE);

	if (m_iYesNoTrigger == YESNO_EXIT)
		((CMLWndManager*)AWC::GetWC())->ReturnToMainMenu();
	else if (m_iYesNoTrigger == YESNO_RESTART)
		GETMLGAMEPLAY->ReplayMission(true);

	return true;
}

bool CMLWndESCPopup::OnNoClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	assert(m_pYesNoMsgBox);
	m_pYesNoMsgBox->ShowWindow(false);
	ShowWindow(true);

	return true;
}

bool CMLWndESCPopup::LoadRes()
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

//	m_wndStat.LoadRes();

	return true;
}

bool CMLWndESCPopup::FreeRes()
{
	J_EXCEPTION_RECORD(0);

	bool bRet = true;

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

//	m_wndStat.FreeRes();

	return bRet && AWnd::FreeRes();
}

bool CMLWndESCPopup::OnBackClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	AWC::GetWC()->SetActiveStatus(0);
	GETMLGAMEPLAY->PauseSounds(false);

	return true;
}

bool CMLWndESCPopup::OnRestartClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	assert(m_pYesNoMsgBox);

	m_iYesNoTrigger = YESNO_RESTART;
	m_pYesNoMsgBox->ShowWindow(true);
	ShowWindow(false);

	return true;
}

bool CMLWndESCPopup::OnExitClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	assert(m_pYesNoMsgBox);

	m_iYesNoTrigger = YESNO_EXIT;
	m_pYesNoMsgBox->ShowWindow(true);
	ShowWindow(false);
	
	return true;
}

bool CMLWndESCPopup::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
		AWC::GetWC()->SetFocus(this);
	else
		SaveSettings();
	
	return true;
}

void CMLWndESCPopup::SetYesNoMsgBox(CMLMessageBox* pMsgBox)
{
	if (!(m_pYesNoMsgBox = pMsgBox))
		return;

	m_pYesNoMsgBox->m_btnYes.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndESCPopup::OnYesClick);
	m_pYesNoMsgBox->m_btnNo.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndESCPopup::OnNoClick);
	m_pYesNoMsgBox->ShowWindow(false);
}
