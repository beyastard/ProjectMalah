/*
 * FILE: ML_WndSetting.cpp
 *
 * DESCRIPTION: Setting interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "A2DSprite.h"
#include "AWndSprite.h"
#include "ML_WndSetting.h"
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

char g_szSettingFile[] = "ini\\Settings.ini";

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndSetting
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndSetting, AWnd, 1)

CMLWndSetting::CMLWndSetting()
{
	m_pBackimage	= NULL;
	m_bVisible		= false;
	m_bKeyPreview	= true;
	m_bCanBeFocus	= true;
}

CMLWndSetting::~CMLWndSetting()
{
}

bool CMLWndSetting::OnCreate()
{
	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndSetting::OnDestroy()
{
	AWnd::OnDestroy();
}

bool CMLWndSetting::OnDraw()
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

	m_btnKeySetting.EnableWnd(!m_wndKeySetting.IsVisible());
	m_btnSysSetting.EnableWnd(!m_wndSysSetting.IsVisible());

	return true;
}

bool CMLWndSetting::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
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

bool CMLWndSetting::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	//	系统设置窗口
	if (!m_wndSysSetting.LoadFromIni(this, IniFile, "wnd_syssetting"))
	{
		ADEBUG_TRACE("Cannot create SysSetting window.");
		return false;
	}

	//	按键设置窗口
	if (!m_wndKeySetting.LoadFromIni(this, IniFile, "wnd_keysetting"))
	{
		ADEBUG_TRACE("Cannot create KeySetting window.");
		return false;
	}

	//	返回
	if (!m_btnOK.LoadFromIni(this, IniFile, "wnd_setting::button_back"))
	{
		ADEBUG_TRACE("Cannot create Back button.");
		return false;
	}

	m_btnOK.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndSetting::OnBtnClickOK);

	//	系统设置
	if (!m_btnSysSetting.LoadFromIni(this, IniFile, "wnd_setting::button_system"))
	{
		ADEBUG_TRACE("Cannot create SysSetting button.");
		return false;
	}

	m_btnSysSetting.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndSetting::OnBtnClickSysSetting);

	//	控制键设置
	if (!m_btnKeySetting.LoadFromIni(this, IniFile, "wnd_setting::button_keyboard"))
	{
		ADEBUG_TRACE("Cannot create KeySetting button.");
		return false;
	}

	m_btnKeySetting.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndSetting::OnBtnClickKeySetting);

	LoadSetting();

	return true;
}

bool CMLWndSetting::SaveSetting()
{
	J_EXCEPTION_RECORD(0);

	AWndIniFile	IniOptions;
	if (!IniOptions.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot load options inifile.");
		return false;
	}

	bool bRet = true;

	if (!m_wndKeySetting.SaveSetting(IniOptions))
	{
		ADEBUG_TRACE("Cannot save keysetting ini.");
		bRet = false;
	}

	if (!m_wndSysSetting.SaveSetting(IniOptions))
	{
		ADEBUG_TRACE("Cannot save syssetting ini.");
		bRet = false;
	}

	IniOptions.Save();
	IniOptions.Release();

	return bRet;
}

bool CMLWndSetting::LoadSetting()
{
	J_EXCEPTION_RECORD(0);

	AWndIniFile	IniOptions;
	if (!IniOptions.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot load options inifile.");
		return false;
	}

	if (!m_wndKeySetting.LoadSetting(IniOptions))
	{
		ADEBUG_TRACE("Cannot load keysetting ini.");
		return false;
	}

	if (!m_wndSysSetting.LoadSetting(IniOptions))
	{
		ADEBUG_TRACE("Cannot load syssetting ini.");
		return false;
	}

	IniOptions.Release();

	return true;
}

bool CMLWndSetting::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;

	return true;
}

bool CMLWndSetting::FreeRes()
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

bool CMLWndSetting::OnBtnClickBack(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	ShowWindow(false);
	return true;
}

bool CMLWndSetting::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
	{
		LoadSetting();
		AWC::GetWC()->SetFocus(this);

		OnBtnClickKeySetting(NULL);
	//	OnBtnClickSysSetting(NULL);
	}

	return true;
}

bool CMLWndSetting::OnBtnClickOK(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	SaveSetting();
	ShowWindow(false);

/*	if(m_wndNameInput.GetLineCount() > 0)
	{
		char szBuf[64];
		int len	= m_wndNameInput.GetLine(0, szBuf, sizeof(szBuf));
		if(len > 0)
		{
			szBuf[len]	= 0;
			AUString strName(szBuf);
			if(!IsEmptyName(strName))
				OnInputName(strName);
			else
				RemoveScore(m_nInsertScorePos);
		}
		else
		{
			RemoveScore(m_nInsertScorePos);
		}
	}
*/
	return true;
}

bool CMLWndSetting::OnBtnClickSysSetting(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	m_wndKeySetting.ShowWindow(false);
	m_wndSysSetting.ShowWindow(true);
	return true;
}

bool CMLWndSetting::OnBtnClickKeySetting(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	m_wndKeySetting.ShowWindow(true);
	m_wndSysSetting.ShowWindow(false);
	return true;
}

