/*
 * FILE: ML_WndCredits.cpp
 *
 * DESCRIPTION: Credits interface for Malah game
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

#include "ML_WndCredits.h"
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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndCredits
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndCredits, AWnd, 1)

CMLWndCredits::CMLWndCredits()
{
	m_pBackimage	= NULL;
	m_bVisible		= false;
	m_bCanBeFocus	= true;
}

CMLWndCredits::~CMLWndCredits()
{
}

bool CMLWndCredits::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndCredits::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndCredits::OnDraw()
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

bool CMLWndCredits::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
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

	//	§Ú§ç¨[§á
	if (!m_btnOk.LoadFromIni(this, IniFile, "wnd_credits::button_back"))
	{
		ADEBUG_TRACE("Cannot create wnd_credits::back button.");
		return false;
	}

	m_btnOk.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndCredits::OnBtnClickBack);

	return true;
}

bool CMLWndCredits::LoadRes()
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

bool CMLWndCredits::FreeRes()
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

bool CMLWndCredits::OnBtnClickBack(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	ShowWindow(false);
	return true;
}


