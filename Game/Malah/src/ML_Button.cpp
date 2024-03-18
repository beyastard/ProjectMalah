/*
 * FILE: ML_Button.cpp
 *
 * DESCRIPTION: Button control for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "ML_Button.h"
#include "A2DSprite.h"
#include "AWndSprite.h"
#include "AWC.h"
#include "ML_Resources.h"

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
//	Implement CMLButton
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLButton, AButton, 1)

CMLButton::CMLButton() : m_pButtonimage(NULL)
{
	memset(m_rButtonRects, 0, sizeof (m_rButtonRects));
}

CMLButton::~CMLButton()
{
}

bool CMLButton::LoadRes()
{
	if (!AButton::LoadRes())
		return false;
	
	if (!m_strButtonimage.IsEmpty())
	{
		m_pButtonimage = AWndSpriteMan::CreateSprite(m_strButtonimage, m_ptImageSize.x, 
													 m_ptImageSize.y, 0x00000000, 4, 
													 m_rButtonRects);
		if (!m_pButtonimage)
		{
			ADEBUG_TRACE1("Cannot load image <%s>", m_strButtonimage);
		}
	}

/*	m_pButtonimage = new A2DSprite;

	if (!m_pButtonimage->Init(AWC::GetWC()->GetDevice(),
		m_strButtonimage, m_ptImageSize.x, m_ptImageSize.y,
		0x00000000, 4, (A3DRECT*)m_rButtonRects, true, false))
	{
		m_pButtonimage->Release();
		delete m_pButtonimage;
		m_pButtonimage	= NULL;
		ADEBUG_TRACE1("Cannot load image <%s>", m_strButtonimage);
		//return false;
	}
*/
	return true;
}

bool CMLButton::FreeRes()
{
	if (m_pButtonimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pButtonimage);
		m_pButtonimage = NULL;
	}

	return AButton::FreeRes();
}

bool CMLButton::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	if (!AButton::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	const char* pszButtonimage = NULL;
	if (!IniFile.ReadString(pszSection, "image_button", pszButtonimage))
	{
	//	ADEBUG_TRACE("cannot read button image.");
	//	return false;
		m_strButtonimage = "";
	}
	else
		m_strButtonimage = pszButtonimage;

	int nHeight	= m_rArea.GetHeight();
	int nWidth	= m_rArea.GetWidth();
	int	nRect = 0;
	char* nNames[4]	= 
	{
		"rect_button_normal",
		"rect_button_disabled",
		"rect_button_pushed",
		"rect_button_hovered"
	};

	if (!IniFile.ReadPoint(pszSection, "size_image_button", m_ptImageSize))
	{
		m_ptImageSize.x	= nWidth * 4;
		m_ptImageSize.y = nHeight;
	}

	if (!IniFile.ReadRect(pszSection, nNames[nRect], m_rButtonRects[nRect]))
	{
		m_rButtonRects[nRect].left	= nWidth*nRect;
		m_rButtonRects[nRect].top	= 0;
		m_rButtonRects[nRect].right	= nWidth*nRect + nWidth;
		m_rButtonRects[nRect].bottom= nHeight;
	}

	nRect++;

	if (!IniFile.ReadRect(pszSection, nNames[nRect], m_rButtonRects[nRect]))
	{
		m_rButtonRects[nRect].left	= nWidth*nRect;
		m_rButtonRects[nRect].top	= 0;
		m_rButtonRects[nRect].right	= nWidth*nRect + nWidth;
		m_rButtonRects[nRect].bottom= nHeight;
	}

	nRect++;

	if (!IniFile.ReadRect(pszSection, nNames[nRect], m_rButtonRects[nRect]))
	{
		m_rButtonRects[nRect].left	= nWidth*nRect;
		m_rButtonRects[nRect].top	= 0;
		m_rButtonRects[nRect].right	= nWidth*nRect + nWidth;
		m_rButtonRects[nRect].bottom= nHeight;
	}

	nRect++;

	if (!IniFile.ReadRect(pszSection, nNames[nRect], m_rButtonRects[nRect]))
	{
		m_rButtonRects[nRect].left	= nWidth*nRect;
		m_rButtonRects[nRect].top	= 0;
		m_rButtonRects[nRect].right	= nWidth*nRect + nWidth;
		m_rButtonRects[nRect].bottom= nHeight;
	}

	//	nRect++;

	return true;
}

bool CMLButton::SetIniInfo(AWnd* pParent, const char* szBackImage, ARECT& Rect)
{
	if (!Create(pParent, Rect))
	{
		ADEBUG_TRACE("Cannot create button");
		return false;
	}

	m_strButtonimage = szBackImage;

	int iHeight	= Rect.GetHeight();
	int iWidth	= Rect.GetWidth();

	m_ptImageSize.x	= iWidth * 4;
	m_ptImageSize.y = iHeight;

	for (int i=0; i < 4; i++)
	{
		m_rButtonRects[i].left		= iWidth * i;
		m_rButtonRects[i].top		= 0;
		m_rButtonRects[i].right		= iWidth * i + iWidth;
		m_rButtonRects[i].bottom	= iHeight;
	}

	return true;
}

bool CMLButton::OnDraw()
{
	int nRect = 0;
	if (!m_bEnabled)
		nRect = 1;
	else if (m_bPushed)
		nRect = 2;
	else if (m_bHovered)
		nRect = 3;
	
	if (m_pButtonimage)
	{
		APOINT ptAb;
		GetAbsolutePos(ptAb);
		m_pButtonimage->SetCurrentItem(nRect);
		AWC::DrawSprite(m_pButtonimage, ptAb);
	}

	return true;
}

bool CMLButton::OnClick()
{
	g_theGame.Play2DSound(RES_S2D_PUSHBUTTON, false);

	if (!m_wnOnClick.IsNil())
		return m_wnOnClick(this);

	return true;
}

