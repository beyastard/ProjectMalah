/*
 * FILE: ML_WndKeySetting.cpp
 *
 * DESCRIPTION: Key setting interface for Malah game
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

#include "ML_WndKeySetting.h"
#include "ML_WndManager.h"
#include "ML_Gameplay.h"
#include "ML_TransKeyboard.h"
#include <jexception.h>

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

static char* szIniKeys[] =
{
	"weapon_0",
	"weapon_1",
	"weapon_2",
	"weapon_3",
	"weapon_4",
	"artillery",
	"attacker",
	"helicopter",
	"weaponfunc",
	"lastweapon",
	"fire",
	"pause",
	"missionhelp",
};

static int nKeyCommands[] = 
{
	LKEY_WEAPON0,		//	Weapon 0
	LKEY_WEAPON1,		//	Weapon 1
	LKEY_WEAPON2,		//	Weapon 2
	LKEY_WEAPON3,		//	Weapon 3
	LKEY_WEAPON4,		//	Weapon 4
	LKEY_CALLCANNON,
	LKEY_CALLATTACKER,
	LKEY_CALLCOPTER,
	LKEY_WEAPONFUNC,
	LKEY_LASTWEAPON,	//	Last weapon
	LKEY_FIRE,
	LKEY_PAUSE,			//	Pause game
	LKEY_HELP,			//	Mission help
};

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndKeyBox
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndKeyBox, AWnd, 1)

CMLWndKeyBox::CMLWndKeyBox()
{
	m_uKeyValue		 = 0;
	m_bCanBeFocus	 = true;
	m_bFirstMouseBtn = false;
}

CMLWndKeyBox::~CMLWndKeyBox()
{
}

bool CMLWndKeyBox::OnSetFocus(bool bFocus)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnSetFocus(bFocus))
		return false;
	
	return true;
}

bool CMLWndKeyBox::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnKeyboardMsg(Msg, bAccept))
		return false;
	
	int		nKey;
	APOINT	ptMouse;
	ARECT	rAb;

	AWC::GetWC()->GetMouseMsg()->GetPos(ptMouse);
	GetAbsoluteArea(rAb);

	if (ptMouse && rAb)
	{
		for (nKey=0; nKey < 5; nKey++)
		{
			bool bDown, bChanged;
			Msg.GetButtonStatus((AKeyboardButton)nKey, bDown, bChanged);

			if (bDown && bChanged)
			{
				SetKey(nKey);
				break;
			}
		}
	}

	for (nKey=5; nKey < 256; nKey++)
	{
		if (nKey == VK_ESCAPE)
			continue;
		
		bool bDown, bChanged;
		Msg.GetButtonStatus((AKeyboardButton)nKey, bDown, bChanged);
		
		if (bDown && bChanged)
		{
			SetKey(nKey);
			break;
		}
	}

	((CMLWndKeySetting*)m_pParent)->OnKeyChanged(this);

	return true;
}

bool CMLWndKeyBox::OnMouseDown(const APOINT& ptClick)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnMouseDown(ptClick))
		return false;
	
	return true;
}

bool CMLWndKeyBox::OnDraw()
{
	J_EXCEPTION_RECORD(0);
	
	if (!AWnd::OnDraw())
		return false;
	
	ARECT rAb;
	GetAbsoluteArea(rAb);

	if (AWC::GetWC()->GetFocus() == this)
		AWC::DrawRectangle(rAb, 0x7F8080FF);
	
	AUString& strKeyString = ((CMLWndKeySetting*)m_pParent)->GetKeyString((int)m_uKeyValue);
	int nLength	= strKeyString.GetLength();

	if (nLength > 0)
	{
		AWC::SetCurrentFont(FONT_FONT2_12);
		APOINT ptText(rAb.left+(rAb.GetWidth()-m_ptTextExtent.x)/2,
					  rAb.top+(rAb.GetHeight()-m_ptTextExtent.y)/2 + 2);
		AWC::DrawText(ptText, strKeyString, nLength, 0x7F7DFFFF);
	}

	return true;
}

bool CMLWndKeyBox::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	SetKey(0);
	return true;
}

bool CMLWndKeyBox::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
	return true;
}

bool CMLWndKeyBox::FreeRes()
{
	J_EXCEPTION_RECORD(0);
	bool bRet = true;
	return bRet && AWnd::FreeRes();
}

UINT CMLWndKeyBox::GetKey()
{
	return m_uKeyValue;
}

VOID CMLWndKeyBox::SetKey(UINT uKey)
{
	J_EXCEPTION_RECORD(0);

	AUString& strKeyString = ((CMLWndKeySetting*)m_pParent)->GetKeyString((int)uKey);
	int nLength	= strKeyString.GetLength();

	if (nLength > 0)
	{
		m_uKeyValue	= uKey;
		AWC::SetCurrentFont(FONT_FONT2_12);
		AWC::GetTextExtent(strKeyString, nLength, m_ptTextExtent);
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndKeySetting
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndKeySetting, AWnd, 1)

CMLWndKeySetting::CMLWndKeySetting()
{
	m_pBackimage	= NULL;
	m_bCanBeFocus	= true;
}

CMLWndKeySetting::~CMLWndKeySetting()
{
}

bool CMLWndKeySetting::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndKeySetting::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

void CMLWndKeySetting::OnKeyChanged(CMLWndKeyBox* pBox)
{
	J_EXCEPTION_RECORD(0);

	int i;
	for (i=0; i < MAX_KEY_BOX; i ++)
	{
		if (m_kbSettings+i != pBox && m_kbSettings[i].GetKey() == pBox->GetKey())
			m_kbSettings[i].SetKey(0);
	}
}

bool CMLWndKeySetting::OnDraw()
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

AUString& CMLWndKeySetting::GetKeyString(int nKey)
{
	J_EXCEPTION_RECORD(0);
	return m_strKeyMap[nKey];
}

bool CMLWndKeySetting::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	m_strBackimage	= pszBackimage;

	int nKey;
	for (nKey = 0; nKey < 256; nKey++)
	{
		const char* pszStr = NULL;
		char szIntKey[16];
		sprintf(szIntKey, "%d", nKey);

		if (IniFile.ReadString("data_keymap", szIntKey, pszStr))
			m_strKeyMap[nKey] = pszStr;
	}
	
	int i;
	char szSecBuf[256];

	for (i=0; i < MAX_KEY_BOX; i++)
	{
		sprintf(szSecBuf, "wnd_keysetting::keybox_%d", i);
		if (!m_kbSettings[i].LoadFromIni(this, IniFile, szSecBuf))
		{
			ADEBUG_TRACE1("cannot load window <%s>.", szSecBuf);
			return false;
		}
	}

	return true;
}

bool CMLWndKeySetting::SaveSetting(AWndIniFile& IniFile)
{
	J_EXCEPTION_RECORD(0);

	int i;
	for (i=0; i < sizeof (szIniKeys) / sizeof (char*); i++)
	{
		IniFile.WriteInt("setting_keyboard", szIniKeys[i], m_kbSettings[i].GetKey());
		GETMLGAMEPLAY->SetKeySetting(nKeyCommands[i], m_kbSettings[i].GetKey(), 0);
	}

	return true;
}

bool CMLWndKeySetting::LoadSetting(AWndIniFile& IniFile)
{
	J_EXCEPTION_RECORD(0);

	int i;
	for (i=0; i < sizeof (szIniKeys) / sizeof (char*); i++)
	{
		INT nKey1;
		if (!IniFile.ReadInt("setting_keyboard", szIniKeys[i], nKey1))
			nKey1 = 0;
		
		m_kbSettings[i].SetKey(nKey1);
	}
	
	return true;
}

bool CMLWndKeySetting::LoadRes()
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

bool CMLWndKeySetting::FreeRes()
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