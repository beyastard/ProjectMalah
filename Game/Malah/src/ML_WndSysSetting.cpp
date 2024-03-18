/*
 * FILE: ML_WndSysSetting.cpp
 *
 * DESCRIPTION: System setting interface for Malah game
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
#include "A3DAssistTools.h"

#include "ML_WndSysSetting.h"
#include "ML_WndManager.h"
#include "ML_GamePlay.h"
#include "jexception.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define READ_COMBOBOX(cb, def) \
	{\
		int nIndex;\
		if(!IniFile.ReadInt("setting_system", szKey_##cb, nIndex))\
		{\
			nIndex	= def;\
		}\
		cb.SetCurrent(nIndex);\
	}

#define WRITE_COMBOBOX(cb) \
	{\
		IniFile.WriteInt("setting_system", szKey_##cb, (int) cb.GetCurrent());\
	}

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

static char szKey_m_cbMode[]				= "displaymode";
static char szKey_m_cbDepth[]				= "colordepth";
static char szKey_m_cbTexPrecision[]		= "texprecision";
static char szKey_m_cbTexFilter[]			= "texfilter";
static char szKey_m_cbTexQuality[]			= "texquality";
static char szKey_m_cbEffSmoke[]			= "effectsmoke";
static char szKey_m_cbEffWeather[]			= "effectweather";
static char szKey_m_cbSoundQuality[]		= "soundquality";
static char szKey_m_cbBlood[]				= "blood";
static char szKey_m_cbGraphicsMisc[]		= "graphicsmisc";
static char szKey_m_cbSoundMisc[]			= "soundmisc";
static char szKey_m_cbMouseInvert[]			= "mouseinvert";

char g_szSysSec[]	= "setting_system";
char g_szSysDM[]	= "displaymode";
char g_szSysCD[]	= "colordepth";
char g_szSysMI[]	= "mouseinv";
char g_szSysBR[]	= "brightness";
char g_szSysSV[]	= "sound";
char g_szSysMV[]	= "music";
char g_szSysMS[]	= "mouse";

extern char	g_szDefUserName[];

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndSysSetting
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndSysSetting, AWnd, 1)

CMLWndSysSetting::CMLWndSysSetting()
{
	m_fOldSound		 = 0.0f;
	m_fOldMusic		 = 0.0f;
	m_fOldBrightness = 0.0f;
	m_bVisible		 = false;
	m_pBackimage	 = NULL;
}

CMLWndSysSetting::~CMLWndSysSetting()
{
}

bool CMLWndSysSetting::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndSysSetting::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndSysSetting::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	APOINT ptAb;
	GetAbsolutePos(ptAb);

	if (m_pBackimage)
	{
		if (!AWC::DrawSprite(m_pBackimage, ptAb))
			ADEBUG_TRACE("Cannot draw to back.");
	}

	float fVolume = 0.0f;
	if (m_fOldSound != m_scrSound.GetScrollPos())
	{
		m_fOldSound	= m_scrSound.GetScrollPos();
		fVolume	= 100.0f*m_scrSound.GetScrollPos()/m_scrSound.GetScrollLength();
		m_pAGame->GetA3DAssistTools()->S_SetVolume((UINT)fVolume);
	}

	if (m_fOldMusic != m_scrMusic.GetScrollPos())
	{
		m_fOldMusic	= m_scrMusic.GetScrollPos();
		fVolume	= 100.0f*m_scrMusic.GetScrollPos()/m_scrMusic.GetScrollLength();
		m_pAGame->GetA3DAssistTools()->BM_SetVolume((UINT)fVolume);
	}

	if (m_fOldBrightness != m_scrBrightness.GetScrollPos())
	{
		m_fOldBrightness = m_scrBrightness.GetScrollPos();
		fVolume	= 200.0f*m_scrBrightness.GetScrollPos()/m_scrBrightness.GetScrollLength();
		m_pAGame->GetA3DEngine()->GetA3DDevice()->SetGammaLevel((UINT)fVolume);
	}

	return true;
}

bool CMLWndSysSetting::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	if (!m_cbMode.LoadFromIni(this, IniFile, "wnd_setting::combobox_mode"))
	{
		ADEBUG_TRACE("Cannot load combobox displaymode.");
		return false;
	}

	if (!m_cbDepth.LoadFromIni(this, IniFile, "wnd_setting::combobox_color"))
	{
		ADEBUG_TRACE("Cannot load combobox colordepth.");
		return false;
	}

	if (!m_cbTexFilter.LoadFromIni(this, IniFile, "wnd_setting::combobox_texfilter"))
	{
		ADEBUG_TRACE("Cannot load combobox_texfilter.");
		return false;
	}

	if (!m_cbTexQuality.LoadFromIni(this, IniFile, "wnd_setting::combobox_texdetail"))
	{
		ADEBUG_TRACE("Cannot load combobox_texdetail.");
		return false;
	}

	if (!m_cbEffWeather.LoadFromIni(this, IniFile, "wnd_setting::combobox_weather"))
	{
		ADEBUG_TRACE("Cannot load combobox_weather.");
		return false;
	}

	if (!m_cbEffSmoke.LoadFromIni(this, IniFile, "wnd_setting::combobox_smoke"))
	{
		ADEBUG_TRACE("Cannot load combobox combobox_smoke.");
		return false;
	}

	if (!m_cbBlood.LoadFromIni(this, IniFile, "wnd_setting::combobox_blood"))
	{
		ADEBUG_TRACE("Cannot load combobox combobox_blood.");
		return false;
	}

	if (!m_cbGraphicsMisc.LoadFromIni(this, IniFile, "wnd_setting::combobox_graphicsmisc"))
	{
		ADEBUG_TRACE("Cannot load combobox combobox_graphicsmisc.");
		return false;
	}

	if (!m_cbSoundMisc.LoadFromIni(this, IniFile, "wnd_setting::combobox_soundmisc"))
	{
		ADEBUG_TRACE("Cannot load combobox combobox_soundmisc.");
		return false;
	}

	if (!m_cbMouseInvert.LoadFromIni(this, IniFile, "wnd_setting::combobox_mouseinvert"))
	{
		ADEBUG_TRACE("Cannot load combobox combobox_mouseinvert.");
		return false;
	}

	if (!m_scrBrightness.LoadFromIni(this, IniFile, "wnd_syssetting::scrollbar_brightness"))
	{
		ADEBUG_TRACE("Cannot load scroll bar brightness.");
		return false;
	}

	if (!m_scrSound.LoadFromIni(this, IniFile, "wnd_syssetting::scrollbar_sound"))
	{
		ADEBUG_TRACE("Cannot load scroll bar sound.");
		return false;
	}

	if (!m_scrMusic.LoadFromIni(this, IniFile, "wnd_syssetting::scrollbar_music"))
	{
		ADEBUG_TRACE("Cannot load scroll bar music.");
		return false;
	}

	if (!m_scrMouse.LoadFromIni(this, IniFile, "wnd_syssetting::scrollbar_mouse"))
	{
		ADEBUG_TRACE("Cannot load scroll bar mouse.");
		return false;
	}

	if(!m_wndNameInput.LoadFromIni(this, IniFile, "wnd_syssetting::edit_name"))
	{
		ADEBUG_TRACE("cannot read name input editbox");
		return false;
	}
	
	m_wndNameInput.SetCursorColor(0xFF00AFFF);
	m_wndNameInput.SetTextColor(0xFF00FFFF);
	m_wndNameInput.SetBackColor(0x00000000);

	return true;
}

bool CMLWndSysSetting::SaveSetting(AWndIniFile& IniFile)
{
	J_EXCEPTION_RECORD(0);

	ApplySetting();

	WRITE_COMBOBOX(m_cbMode);
	WRITE_COMBOBOX(m_cbDepth);
	WRITE_COMBOBOX(m_cbEffSmoke);
	WRITE_COMBOBOX(m_cbTexFilter);
	WRITE_COMBOBOX(m_cbTexQuality);
	WRITE_COMBOBOX(m_cbEffWeather);
	WRITE_COMBOBOX(m_cbBlood);
	WRITE_COMBOBOX(m_cbGraphicsMisc);
	WRITE_COMBOBOX(m_cbSoundMisc);
	WRITE_COMBOBOX(m_cbMouseInvert);

	WRITE_SCROLL(BR, m_scrBrightness);
	WRITE_SCROLL(SV, m_scrSound);
	WRITE_SCROLL(MV, m_scrMusic);
	WRITE_SCROLL(MS, m_scrMouse);

	//	Save player's name
	if (m_wndNameInput.GetLineCount() > 0)
	{
		char szBuf[64];
		int iLen = m_wndNameInput.GetLine(0, szBuf, 64);
		szBuf[iLen] = '\0';
	//	ADEBUG_TRACE2("save user name: %s %d.", szBuf, iLen);

		if (!iLen || IsEmptyName(szBuf))
			strcpy(szBuf, g_szDefUserName);

		IniFile.WriteString("record", "username", szBuf);
	}

	return true;
}

bool CMLWndSysSetting::LoadSetting(AWndIniFile& IniFile)
{
	J_EXCEPTION_RECORD(0);

	READ_COMBOBOX(m_cbMode, 1);
	READ_COMBOBOX(m_cbDepth, 0);
	READ_COMBOBOX(m_cbEffSmoke, 0);
	READ_COMBOBOX(m_cbTexFilter, 0);
	READ_COMBOBOX(m_cbTexQuality, 0);
	READ_COMBOBOX(m_cbEffWeather, 0);
	READ_COMBOBOX(m_cbBlood, 0);
	READ_COMBOBOX(m_cbGraphicsMisc, 0);
	READ_COMBOBOX(m_cbSoundMisc, 0);
	READ_COMBOBOX(m_cbMouseInvert, 0);

	READ_SCROLL(BR, m_scrBrightness);
	READ_SCROLL(SV, m_scrSound);
	READ_SCROLL(MV, m_scrMusic);
	READ_SCROLL(MS, m_scrMouse);

	ApplySetting();

	//	Load user name
	m_wndNameInput.SetSel();

	const char* pszName = NULL;
	if (!IniFile.ReadString("record", "username", pszName))
		m_wndNameInput.ReplaceSel(g_szDefUserName);
	else
	{
		m_wndNameInput.ReplaceSel(pszName);
	//	ADEBUG_TRACE1("load user name: %s.", pszName);
	}

	return true;
}

bool CMLWndSysSetting::ApplySetting()
{
	J_EXCEPTION_RECORD(0);
	USERCONFIGS* pData = GETCURUSERCONFIGS;

	float fVolume;

	//	音效音量
	fVolume	= 100.0f * m_scrSound.GetScrollPos() / m_scrSound.GetScrollLength();
	m_pAGame->GetA3DAssistTools()->S_SetVolume((UINT)fVolume);
	pData->iSoundVolume = (int)fVolume;

	//	音乐音量
	fVolume	= 100.0f * m_scrMusic.GetScrollPos() / m_scrMusic.GetScrollLength();
	m_pAGame->GetA3DAssistTools()->BM_SetVolume((UINT)fVolume);
	pData->iMusicVolume = (int)fVolume;

	//	亮度
	fVolume	= 200.0f * m_scrBrightness.GetScrollPos() / m_scrBrightness.GetScrollLength();
	m_pAGame->GetA3DEngine()->GetA3DDevice()->SetGammaLevel((UINT)fVolume);
	pData->iBrightness = (int)fVolume;

	//	鼠标速度
	fVolume	= 2.0f * m_scrMouse.GetScrollPos() / m_scrMouse.GetScrollLength();
	pData->fMouseSpeed = fVolume;

	int nMode = m_cbMode.GetCurrent();
	switch (nMode)
	{
	case 0:
	
		pData->iRenderWid = 640;
		pData->iRenderHei = 480;
		break;

	case 2:
	
		pData->iRenderWid = 1024;
		pData->iRenderHei = 768;
		break;

	default:
		
		pData->iRenderWid = 800;
		pData->iRenderHei = 600;
		break;
	}

	int nDepth = m_cbDepth.GetCurrent();
	if (nDepth == 1)
		pData->iColorBit = 32;
	else
		pData->iColorBit = 16;

	pData->iGFXQuality		= m_cbEffSmoke.GetCurrent();		//	烟火效果
	pData->iTextureFilter	= m_cbTexFilter.GetCurrent();		//	插值方式
	pData->iTexQuality		= m_cbTexQuality.GetCurrent();		//	贴图精度
	pData->bWeather			= m_cbEffWeather.GetCurrent() == 0;	//	天气效果
	pData->bBlood			= m_cbBlood.GetCurrent() == 0;
	pData->bGraphicsMisc	= m_cbGraphicsMisc.GetCurrent() == 0;
	pData->bSoundMisc		= m_cbSoundMisc.GetCurrent() == 0;
	pData->iMouseInvert		= m_cbMouseInvert.GetCurrent() == 0 ? -1 : 1;

	return true;
}

bool CMLWndSysSetting::LoadRes()
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

bool CMLWndSysSetting::FreeRes()
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

bool CMLWndSysSetting::IsEmptyName(char* szName)
{
	int iLen;
	if (!szName || !(iLen = strlen(szName)))
		return true;

	for (int i=0; i < iLen; i++)
	{
		if (szName[i] != ' ')
			return false;
	}

	return true;
}

