/*
 * FILE: ML_ListBox.cpp
 *
 * DESCRIPTION: List box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_ListBox.h"
#include "A2DSprite.h"
#include "AWC.h"

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
//	Implement CMLListBox
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLListBox, AListBox, 1)

CMLListBox::CMLListBox() : AListBox()
{
	m_iNumReadItem	= -1;
	m_bShowSel		= true;
	m_bScrollBar	= false;
	m_nMarginVer	= 0;
	m_nItemSpace	= 0;
	m_uTextColor	= A3DCOLORRGB(255, 255, 0);
	m_uItemSelColor	= A3DCOLORRGB(128, 128, 255);
}

CMLListBox::~CMLListBox()
{
}

bool CMLListBox::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	int nLineCount = 0;
	if (m_iNumReadItem < 0)		//	Read from ini file
	{
		if (!IniFile.ReadInt(pszSection, "count_line", nLineCount))
			return true;
	}
	else
		nLineCount = m_iNumReadItem;

	int nIndex;
	char buf[128], nilstr[] = "";

	for (nIndex = 0; nIndex < nLineCount; nIndex++)
	{
		sprintf(buf, "line_%d", nIndex);
		const char* str	= NULL;

		if (!IniFile.ReadString(pszSection, buf, str))
			str	= nilstr;
		
		AppendString(str);
	}

	return true;
}

bool CMLListBox::LoadRes()
{
	return AListBox::LoadRes();
}

bool CMLListBox::FreeRes()
{
	return AListBox::FreeRes();
}

bool CMLListBox::OnDraw()
{
	return AListBox::OnDraw();
}

void CMLListBox::SetCurSel(int iItem)
{
	if (iItem < 0 || iItem >= m_Items.GetLength())
		return;

	m_nSelected	= iItem;
}
