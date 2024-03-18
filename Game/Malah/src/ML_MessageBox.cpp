/*
 * FILE: ML_MessageBox.cpp
 *
 * DESCRIPTION: Message box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_MessageBox.h"
#include "A2DSprite.h"
#include "AWndSprite.h"
#include "ML_WndManager.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////

void DrawBackRect(RECT rArea);

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
//	Implement CMLMessageBox
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLMessageBox, AWnd, 1)

CMLMessageBox::CMLMessageBox() :
m_pBackimage(NULL)
{
	m_bKeyPreview	= true;
	m_bCanBeFocus	= true;
}

CMLMessageBox::~CMLMessageBox()
{
}

bool CMLMessageBox::OnDraw()
{
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

bool CMLMessageBox::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	if (!AWnd::OnKeyboardMsg(Msg, bAccept))
		return false;
	
	bool bDown, bChanged;
	Msg.GetButtonStatus(AKB_ESC, bDown, bChanged);

	if (!bDown && bChanged)
	{
		if (!m_btnNo.m_wnOnClick.IsNil())
			m_btnNo.m_wnOnClick(&m_btnNo);
	}
	else
	{
		Msg.GetButtonStatus(AKB_ENTER, bDown, bChanged);
		if (!bDown && bChanged)
		{
			if (!m_btnYes.m_wnOnClick.IsNil())
				m_btnYes.m_wnOnClick(&m_btnYes);
		}
	}

	return true;
}

bool CMLMessageBox::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	const char* pszBackimage = NULL;
	if(!IniFile.ReadString(pszSection, "image_back", pszBackimage))
	{
		ADEBUG_TRACE("cannot read back image.");
		return false;
	}

	m_strBackimage	= pszBackimage;

	if(!m_btnYes.LoadFromIni(this, IniFile, "wnd_messagebox::button_ok"))
	{
		ADEBUG_TRACE("cannot read ok button.");
		return false;
	}

	if(!m_btnNo.LoadFromIni(this, IniFile, "wnd_messagebox::button_cancel"))
	{
		ADEBUG_TRACE("cannot read cancel button.");
		return false;
	}

	return true;
}

bool CMLMessageBox::LoadRes()
{
	if(!AWnd::LoadRes())
	{
		return false;
	}

	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), m_rArea.GetHeight(),
											0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}

	return true;
}

bool CMLMessageBox::FreeRes()
{
	bool bRet	= true;

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

	return bRet && AWnd::FreeRes();
}

bool CMLMessageBox::ShowWindow(bool bShow)
{
	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
		AWC::GetWC()->SetFocus(this);
	
	return true;
}
