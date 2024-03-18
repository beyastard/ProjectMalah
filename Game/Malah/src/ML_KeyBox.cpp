/*
 * FILE: ML_KeyBox.cpp
 *
 * DESCRIPTION: Key box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "ML_KeyBox.h"
#include "ML_WndManager.h"
#include "ML_GamePlay.h"

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

extern char	g_szSettingFile[];

/*
static char* g_pszKeys[] = 
{
	"fire",
	"reload",
	"toggleconsole",
	"togglemenu",
	"pause"
};

static int g_nKeyCommands[] = 
{
	AINPUT_KEY_FIRE,
	AINPUT_KEY_RELOAD,
	AINPUT_KEY_TOGGLECONSOLE,
	AINPUT_KEY_TOGGLEMENU,
	AINPUT_KEY_PAUSE
};
*/

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLKeyBox
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLKeyBox, AWndGrid, 1)

CMLKeyBox::CMLKeyBox()
{
	m_bCanBeFocus	= true;
	m_ptSel.x		= -1;
	m_ptSel.y		= -1;
}

CMLKeyBox::~CMLKeyBox()
{
}

bool CMLKeyBox::OnDraw()
{
	AWndGrid::OnDraw();
	return true;
}

bool CMLKeyBox::OnDrawGrid(int nRow, int nCol, ARECT rGrid)
{
	AWndGrid::OnDrawGrid(nRow, nCol, rGrid);
	return true;
}

bool CMLKeyBox::OnSel(const APOINT& ptNewSel, bool& bCanSel)
{
	AWndGrid::OnSel(ptNewSel, bCanSel);

	if (ptNewSel.x <= 0)
	{
		bCanSel = false;
		return true;
	}

	if (m_ptSel == ptNewSel)
	{
		ChangeKeySetting(1);
		//m_GridData.SetItemText(m_ptSel.y, m_ptSel.x, m_strKeyMap[1]);
		//m_GridData.SetData(m_ptSel.y, m_ptSel.x, 1);
	}

	return true;
}

bool CMLKeyBox::OnMouseMsg(AWndMouseMsg& Msg)
{
	if (!AWndGrid::OnMouseMsg(Msg))
		return false;
	
	return true;
}

bool CMLKeyBox::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	if (!AWndGrid::OnKeyboardMsg(Msg, bAccept))
		return false;
	
	if (m_ptSel.x <= 0)
		return true;
	
	int nKey;
	for (nKey = 0; nKey < 256; nKey++)
	{
		bool bDown, bChanged;
		Msg.GetButtonStatus((AKeyboardButton)nKey, bDown, bChanged);

		if (bDown && bChanged)
		{
			if (nKey == 1)
				break;
			
			if (m_strKeyMap[nKey].GetLength() != 0)
			{
				ChangeKeySetting(nKey);
				//m_GridData.SetItemText(m_ptSel.y, m_ptSel.x, m_strKeyMap[nKey]);
				//m_GridData.SetData(m_ptSel.y, m_ptSel.x, nKey);
				break;
			}
		}
	}

	return true;
}

void CMLKeyBox::ChangeKeySetting(int nKey)
{
	if ((INT)m_GridData.GetData(m_ptSel.y, m_ptSel.x) == nKey)
		return;
	
	int c, r;
	for (c=1; c < m_GridData.GetColCount(); c++)
	{
		for (r=0; r < m_GridData.GetRowCount(); r++)
		{
			if ((int)m_GridData.GetData(r, c) == nKey)
			{
				m_GridData.SetItemText(r, c, m_strKeyMap[0]);
				m_GridData.SetData(r, c, 0);
			}
		}
	}

	m_GridData.SetItemText(m_ptSel.y, m_ptSel.x, m_strKeyMap[nKey]);
	m_GridData.SetData(m_ptSel.y, m_ptSel.x, nKey);
}

bool CMLKeyBox::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	if (!AWndGrid::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
	int nKey;
	for (nKey=0; nKey < 256; nKey++)
	{
		const char* pszStr	= NULL;
		char szIntKey[16];
		sprintf(szIntKey, "%d", nKey);
		
		if (IniFile.ReadString("data_keymap", szIntKey, pszStr))
			m_strKeyMap[nKey] = pszStr;
	}

	return true;
}

bool CMLKeyBox::SaveSetting(AWndIniFile& IniFile)
{
	return true;
}

bool CMLKeyBox::LoadSetting(AWndIniFile& IniFile)
{
	return true;
}

bool CMLKeyBox::LoadRes()
{
	if (!AWndGrid::LoadRes())
		return false;
	
	return true;
}

bool CMLKeyBox::FreeRes()
{
	return AWndGrid::FreeRes();
}
