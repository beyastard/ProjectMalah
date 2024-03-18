/*
 * FILE: ML_WndPlayHelp.h
 *
 * DESCRIPTION: Play help interface for Malah game
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

#include "ML_WndPlayHelp.h"
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
//	Implement CMLWndPlayHelp
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndPlayHelp, AWnd, 1)

CMLWndPlayHelp::CMLWndPlayHelp()
{
	m_pBackimage	= NULL;
	m_bCanBeFocus	= true;
	m_iNumLine		= 0;
	m_iNumShowed	= 18;
	m_iFirstLine	= 0;

	memset(m_aDescLines, 0, sizeof (m_aDescLines));
}

CMLWndPlayHelp::~CMLWndPlayHelp()
{
	ReleaseAllDesc();
}

bool CMLWndPlayHelp::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndPlayHelp::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndPlayHelp::OnDraw()
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

	//	Output description
	m_iFirstLine = (int)(m_scrText.GetRangeSize() * m_scrText.GetScrollPos() / 
					     m_scrText.GetScrollLength());

	int x = m_rcDesc.left;
	int y = m_rcDesc.top;
	int iFont = ML_GetAppropriateFont(16);

	for (int i=m_iFirstLine; i < m_iFirstLine + m_iNumShowed; i++)
	{
		if (!m_aDescLines[i])
			continue;

		char ch;
		m_aDescLines[i]->GetAt(0, ch);
		if (ch != '#')
			g_theGame.TextOut(iFont, x, y, *m_aDescLines[i], A3DCOLORRGB(255, 255, 0), false);

		y += 20;
	}

	return true;
}

bool CMLWndPlayHelp::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
/*	const char* pszBackimage = NULL;
	if (!IniFile.ReadString(pszSection, "image_back", pszBackimage))
	{
		ADEBUG_TRACE("cannot read back image.");
		return false;
	}

	m_strBackimage	= pszBackimage;
*/
	//	Load description text area
	if (!IniFile.ReadRect(pszSection, "rect_desc", m_rcDesc))
	{
		ADEBUG_TRACE("Cannot load rect_desc.");
		return false;
	}

	//	Load description text
	LoadDescription();

	if (!m_scrText.LoadFromIni(this, IniFile, "wnd_playhelp::scrollbar_desc"))
	{
		ADEBUG_TRACE("Cannot load scroll bar description.");
		return false;
	}

	float fMax = 0.0f;
	if (m_iNumLine > m_iNumShowed)
		fMax = (float)(m_iNumLine - m_iNumShowed);

	m_scrText.SetMinData(0.0f);
	m_scrText.SetMaxData(fMax);
	m_scrText.SetVertical(true);
	m_scrText.SetScrollPos(0.0f);

	return true;
}

bool CMLWndPlayHelp::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
/*	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
											   m_rArea.GetHeight(), 0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}
*/
	return true;
}

bool CMLWndPlayHelp::FreeRes()
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

//	Load description
bool CMLWndPlayHelp::LoadDescription()
{
	AWndIniFile IniFile;
	if (!IniFile.Init("Ini\\PlayHelp.ini"))
	{
		ADEBUG_TRACE("Cannot create playhelp file.");
		return false;
	}

	char szSect[] = "play_help";

	if (!IniFile.ReadInt(szSect, "count_line", m_iNumLine))
		return false;

	if (!m_iNumLine)
		return true;

	if (m_iNumLine > MAX_DESCLINE)
		m_iNumLine = MAX_DESCLINE;

	int  i;
	char szKeyBuf[64];

	for (i=0; i < m_iNumLine; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "line_%d", i);

		if (!IniFile.ReadString(szSect, szKeyBuf, pszName))
		{
			m_iNumLine = 0;
			break;
		}

		m_aDescLines[i] = new AUString(pszName);
	}

	IniFile.Release();

	return true;
}

//	Release all description
void CMLWndPlayHelp::ReleaseAllDesc()
{
	for (int i=0; i < MAX_DESCLINE; i++)
	{
		if (m_aDescLines[i])
			delete m_aDescLines[i];
	}
}

bool CMLWndPlayHelp::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (bShow)
		m_scrText.SetScrollPos(0.0f);

	return AWnd::ShowWindow(bShow);
}

