/*
 * FILE: ML_Scroll.cpp
 *
 * DESCRIPTION: Scroll bar control for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_WndScroll.h"
#include "AWC.h"
#include "AWndSprite.h"
#include "AFontDef.h"
#include <jexception.h>

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
//	Implement CMLScrollBarButton
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLScrollBarButton, CMLButton, 1)

bool CMLScrollBarButton::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	if (!CMLButton::OnDraw())
		return false;
	
	CMLScrollBar* pParent = (CMLScrollBar*)m_pParent;
	if (!IsPushed() && !pParent->m_btnLeftTop.IsPushed() &&
		!pParent->m_btnRightBottom.IsPushed())
		return true;
	
	float fData	= pParent->m_fMinData + (pParent->m_fMaxData-pParent->m_fMinData) *
				  pParent->GetScrollPos() / pParent->GetScrollLength();
	char szData[64];

	if (pParent->m_bIntData)
		sprintf(szData, "%d", (int)fData);
	else
		sprintf(szData, "%.1f", fData);
	
	AWC::SetCurrentFont(SONGTI12);

	APOINT ptSize;
	AWC::GetTextExtent(szData, -1, ptSize);

	APOINT ptAb;
	GetAbsolutePos(ptAb);

	ptAb.y -= ptSize.y + 0;

	AWC::DrawText(ptAb, szData, -1, m_uTextColor);

	return true;
}

void CMLScrollBarButton::ForceClick()
{
	J_EXCEPTION_RECORD(0);

	AWC::GetWC()->SetCapture(this);

	if (m_pParent)
	{
		AWnd* pParent = m_pParent->GetParent();
		m_pParent->SetParent(NULL);
		m_pParent->SetParent(pParent);
	}
}

bool CMLScrollBarButton::OnMouseDown(const APOINT& ptClick)
{
	J_EXCEPTION_RECORD(0);

	if (!AButton::OnMouseDown(ptClick))
		return false;
	
	AWC::GetWC()->SetCapture(this);

	if (m_pParent)
	{
		AWnd* pParent = m_pParent->GetParent();
		m_pParent->SetParent(NULL);
		m_pParent->SetParent(pParent);
	}

	return true;
}

bool CMLScrollBarButton::OnMouseMsg(AWndMouseMsg& Msg)
{
	J_EXCEPTION_RECORD(0);

	if (!CMLButton::OnMouseMsg(Msg))
		return false;
	
	bool bDown, bChanged;
	Msg.GetButtonStatus(AMB_LEFT, bDown, bChanged);

	if (!bDown && bChanged)
	{
		if (AWC::GetWC()->GetCapture() == this)
			AWC::GetWC()->SetCapture(NULL);
	}
	else if (bDown)
	{
		APOINT ptMouse;
		Msg.GetPos(ptMouse);
		APOINT ptAb;
		GetAbsolutePos(ptAb);
		ptMouse -= ptAb;
		APOINT ptMove(ptMouse.x - m_rArea.GetWidth() / 2, ptMouse.y - m_rArea.GetHeight() / 2);
		CMLScrollBar* pParent = (CMLScrollBar*)m_pParent;
		pParent->OnScrollMove(ptMove);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLScrollBar
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLScrollBar, AWnd, 1)

CMLScrollBar::CMLScrollBar() :
m_pBackimage	(NULL),
m_pScrollimage	(NULL),
m_nScrollPos	(0.0f),
m_nScrollStep	(1.0f),
m_bVertical		(false),
m_fMaxData		(100.0f),
m_fMinData		(0.000f),
m_bIntData		(true)
{
	m_bHideArrows	= false;
}

CMLScrollBar::~CMLScrollBar()
{
}

bool CMLScrollBar::OnCreate()
{
	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

float CMLScrollBar::GetScrollLength()
{
	J_EXCEPTION_RECORD(0);

	float r;
	if (m_bVertical)
	{
		r = (float)m_rArea.GetHeight() - m_btnLeftTop.GetArea().GetHeight() - 
			m_btnRightBottom.GetArea().GetHeight() - m_btnScroll.GetArea().GetHeight();
	}
	else
	{
		r = (float)m_rArea.GetWidth() - m_btnLeftTop.GetArea().GetWidth() - 
			m_btnRightBottom.GetArea().GetWidth() - m_btnScroll.GetArea().GetWidth();
	}

	return r;
}

void CMLScrollBar::SetScrollPos(float nPos)
{
	J_EXCEPTION_RECORD(0);

	m_nScrollPos = nPos;
	if (m_bVertical)
	{
		int nLength = m_rArea.GetHeight() -	m_btnLeftTop.GetArea().GetHeight() - 
			m_btnRightBottom.GetArea().GetHeight() - m_btnScroll.GetArea().GetHeight();

		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if(m_nScrollPos < 0)
			m_nScrollPos = 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((m_rArea.GetWidth()-rScroll.GetWidth()) / 2,
					   (long)m_nScrollPos + m_btnLeftTop.GetArea().GetHeight());
		m_btnScroll.SetArea(rScroll);
	}
	else
	{
		int nLength	= m_rArea.GetWidth() - m_btnLeftTop.GetArea().GetWidth() - 
			m_btnRightBottom.GetArea().GetWidth() - m_btnScroll.GetArea().GetWidth();

		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if(m_nScrollPos < 0)
			m_nScrollPos	= 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((long)m_nScrollPos + m_btnLeftTop.GetArea().GetWidth(),
					   (m_rArea.GetHeight()-rScroll.GetHeight()) / 2);
		m_btnScroll.SetArea(rScroll);
	}
	return;
}

bool CMLScrollBar::OnScrollMove(const APOINT& ptMove)
{
	J_EXCEPTION_RECORD(0);

	if (m_bVertical)
	{
		int nLength	= m_rArea.GetHeight() -	m_btnLeftTop.GetArea().GetHeight() - 
			m_btnRightBottom.GetArea().GetHeight() - m_btnScroll.GetArea().GetHeight();

		m_nScrollPos += ptMove.y;
		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if (m_nScrollPos < 0)
			m_nScrollPos = 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((m_rArea.GetWidth()-rScroll.GetWidth()) / 2,
					   (long)m_nScrollPos + m_btnLeftTop.GetArea().GetHeight());
		m_btnScroll.SetArea(rScroll);
	}
	else
	{
		int nLength	= m_rArea.GetWidth() - m_btnLeftTop.GetArea().GetWidth() - 
			m_btnRightBottom.GetArea().GetWidth() - m_btnScroll.GetArea().GetWidth();

		m_nScrollPos += ptMove.x;
		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if(m_nScrollPos < 0)
			m_nScrollPos	= 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((long)m_nScrollPos + m_btnLeftTop.GetArea().GetWidth(),
					   (m_rArea.GetHeight()-rScroll.GetHeight())/2);
		m_btnScroll.SetArea(rScroll);
	}

	return true;
}

bool CMLScrollBar::OnbtnClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	if (pWnd == &m_btnLeftTop)
		m_nScrollPos -= m_nScrollStep;
	else
		m_nScrollPos += m_nScrollStep;
	
	if (m_bVertical)
	{
		int nLength	= m_rArea.GetHeight() -	m_btnLeftTop.GetArea().GetHeight() - 
					  m_btnRightBottom.GetArea().GetHeight() - m_btnScroll.GetArea().GetHeight();

		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if(m_nScrollPos < 0)
			m_nScrollPos = 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((m_rArea.GetWidth()-rScroll.GetWidth()) / 2,
					   (long)m_nScrollPos + m_btnLeftTop.GetArea().GetHeight());
		m_btnScroll.SetArea(rScroll);
	}
	else
	{
		int nLength	= m_rArea.GetWidth() - m_btnLeftTop.GetArea().GetWidth() - 
			m_btnRightBottom.GetArea().GetWidth() - m_btnScroll.GetArea().GetWidth();
		if (m_nScrollPos > nLength)
			m_nScrollPos = (float)nLength;
		else if(m_nScrollPos < 0)
			m_nScrollPos	= 0;
		
		ARECT rScroll = m_btnScroll.GetArea();
		rScroll.MoveTo((long)m_nScrollPos + m_btnLeftTop.GetArea().GetWidth(),
					   (m_rArea.GetHeight()-rScroll.GetHeight())/2);
		m_btnScroll.SetArea(rScroll);
	}

	return true;
}

void CMLScrollBar::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLScrollBar::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	ARECT rArea;
	GetAbsoluteArea(rArea);

	if (m_pBackimage)
	{
		if (!AWC::DrawSprite(m_pBackimage, rArea.GetLeftTop()))
		{
			ADEBUG_TRACE("Cannot draw to back.");
		}
	}
//	else
//	{
//		AWC::DrawRectangle(rArea, m_uBackColor);
//	}

	return true;
}

bool CMLScrollBar::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	const char* pszBackimage = NULL;
	if (IniFile.ReadString(pszSection, "image_back", pszBackimage))
		m_strBackimage = pszBackimage;
	
	int iHide = 0;
	IniFile.ReadInt(pszSection, "hidearrows", iHide);
	m_bHideArrows = iHide ? true : false;

	const char* pszBtnSec = NULL;
	if (!IniFile.ReadString(pszSection, "button_lefttop", pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot read button_lefttop in section <%s>.", pszSection);
		return false;
	}

	if (!m_btnLeftTop.LoadFromIni(this, IniFile, pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot load button_lefttop in section <%s>.", pszBtnSec);
		return false;
	}

	if (m_bHideArrows)
	{
		m_btnLeftTop.EnableWnd(false);
		m_btnLeftTop.ShowWindow(false);
	}
	else
	{
		m_btnLeftTop.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLScrollBar::OnbtnClick);
		m_btnLeftTop.SetContinuousClick(true);
	}

	if (!IniFile.ReadString(pszSection, "button_rightbottom", pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot read button_rightbottom in section <%s>.", pszSection);
		return false;
	}
	
	if (!m_btnRightBottom.LoadFromIni(this, IniFile, pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot load button_rightbottom in section <%s>.", pszBtnSec);
		return false;
	}

	ARECT rRightBottom = m_btnRightBottom.GetArea();
	if (m_bVertical)
		rRightBottom.MoveVer(m_rArea.GetHeight()-rRightBottom.GetHeight());
	else
		rRightBottom.MoveHor(m_rArea.GetWidth()-rRightBottom.GetWidth());
	
	m_btnRightBottom.SetArea(rRightBottom);

	if (m_bHideArrows)
	{
		m_btnRightBottom.EnableWnd(false);
		m_btnRightBottom.ShowWindow(false);
	}
	else
	{
		m_btnRightBottom.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLScrollBar::OnbtnClick);
		m_btnRightBottom.SetContinuousClick(true);
	}

	if (!IniFile.ReadString(pszSection, "button_scroll", pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot read button_scroll in section <%s>.", pszSection);
		return false;
	}

	if (!m_btnScroll.LoadFromIni(this, IniFile, pszBtnSec))
	{
		ADEBUG_TRACE1("Cannot load button_scroll in section <%s>.", pszBtnSec);
		return false;
	}

	ARECT rScroll	= m_btnScroll.GetArea();
	ARECT rLeftTop	= m_btnLeftTop.GetArea();

	if (m_bVertical)
		rScroll.Move(rLeftTop.GetHeight(), (m_rArea.GetWidth()-rScroll.GetWidth()) / 2);
	else
		rScroll.Move(rLeftTop.GetWidth(), (m_rArea.GetHeight()-rScroll.GetHeight()) / 2);
	
	m_btnScroll.SetArea(rScroll);

	if (!IniFile.ReadFloat(pszSection, "float_maxdata", m_fMaxData))
		m_fMaxData = 100.0f;
	
	if (!IniFile.ReadFloat(pszSection, "float_mindata", m_fMinData))
		m_fMinData = 0.000f;
	
	int nIntData = 0;
	if (!IniFile.ReadInt(pszSection, "bool_intdata", nIntData))
		m_bIntData = true;
	else
		m_bIntData = (nIntData != 0);
	
	int nDigitColor	= 0;
	if (IniFile.ReadInt(pszSection, "int_digitcolor", nDigitColor))
		m_btnScroll.SetTextColor((UINT)nDigitColor);
	
	return true;
}

bool CMLScrollBar::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
	if (m_strBackimage.GetLength() > 0)
	{
		ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
		m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), m_rArea.GetHeight(),
												   0, 1, &rSprite);
		if (!m_pBackimage)
		{
			ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
		}
	}

	return true;
}

bool CMLScrollBar::FreeRes()
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

bool CMLScrollBar::OnMouseDown(const APOINT& ptClick)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnMouseDown(ptClick))
		return false;

	float p1, p2;

	if (m_bVertical)
	{
		//	Click in arrow area ?
		if (ptClick.y <= m_btnLeftTop.GetArea().bottom ||
			ptClick.y >= m_rArea.GetHeight() - m_btnRightBottom.GetArea().GetHeight())
			return true;

		p1 = (float)m_btnLeftTop.GetArea().GetHeight() + m_btnScroll.GetArea().GetHeight() / 2;
		p2 = (float)m_rArea.GetHeight() - m_btnRightBottom.GetArea().GetHeight() - m_btnScroll.GetArea().GetHeight() / 2;

		if (ptClick.y <= p1)
			SetScrollPos(0.0f);
		else if (ptClick.y >= p2)
			SetScrollPos((float)m_rArea.GetHeight());
		else
			SetScrollPos(ptClick.y - p1);

		m_btnScroll.ForceClick();
	}
	else	//	Horizontal scroll
	{
		//	Click in arrow area ?
		if (ptClick.x <= m_btnLeftTop.GetArea().right ||
			ptClick.x >= m_rArea.GetWidth() - m_btnRightBottom.GetArea().GetWidth())
			return true;

		p1 = (float)m_btnLeftTop.GetArea().GetWidth() + m_btnScroll.GetArea().GetWidth() / 2;
		p2 = (float)m_rArea.GetWidth() - m_btnRightBottom.GetArea().GetWidth() - m_btnScroll.GetArea().GetWidth() / 2;

		if (ptClick.x <= p1)
			SetScrollPos(0.0f);
		else if (ptClick.x >= p2)
			SetScrollPos((float)m_rArea.GetWidth());
		else
			SetScrollPos(ptClick.x - p1);

		m_btnScroll.ForceClick();
	}

	return true;
}

