/*
 * FILE: ML_ComboBox.cpp
 *
 * DESCRIPTION: ComboBox class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/4/14
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_ComboBox.h"
#include "AWC.h"
#include "ML_Gameplay.h"
#include "AWndSprite.h"
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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLComboBox
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLComboBox, AWnd, 1)

CMLComboBox::ABoxItem::ABoxItem() :
m_pSprite(NULL),
m_ptSize(0, 0)
{
}

CMLComboBox::ABoxItem::~ABoxItem()
{
	if (m_pSprite != NULL)
	{
		AWndSpriteMan::ReleaseSprite(m_pSprite);
		m_pSprite	= NULL;
	}
}

CMLComboBox::CMLComboBox():
m_pPanelSel		(NULL),
m_pPanelList	(NULL),
m_bOpened		(false),
m_nItemCount	(0),
m_pItems		(NULL),
m_nItemSel		(-1),
m_nItemCurr		(-1),
m_nItemHeight	(0)
{
	m_bVisible	= true;
	
}

CMLComboBox::~CMLComboBox()
{
	if(m_pItems != NULL)
	{
		delete[] m_pItems;
		m_pItems	= NULL;
	}
}

bool CMLComboBox::ShowWindow(bool bShow)
{
	if(!AWnd::ShowWindow(bShow))
	{
		return false;
	}

	return true;
}

void CMLComboBox::SetCurrent(int nIndex)
{
	if(nIndex < 0)
	{
		m_nItemCurr	= -1;
	}
	else if(nIndex >= m_nItemCount)
	{
		m_nItemCurr	= m_nItemCount - 1;
	}
	else
	{
		m_nItemCurr	= nIndex;
	}
}

int CMLComboBox::GetCurrent()
{
	return m_nItemCurr;
}

bool CMLComboBox::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);
	if(!AWnd::LoadFromIni(pParent, IniFile, pszSection))
	{
		return false;
	}

	if(!m_btnOpen.LoadFromIni(this, IniFile, "button_combobox_open"))
	{
		ADEBUG_TRACE("Cannot load button_combobox_open");
		return false;
	}
	ARECT rOpen(
		m_rArea.GetWidth()-m_btnOpen.GetArea().GetWidth(),
		0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_btnOpen.SetArea(rOpen);
	m_btnOpen.SetCheckedButton(true);
	m_btnOpen.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLComboBox::OnbtnOpenClick);

	if(!IniFile.ReadInt(pszSection, "height_item", m_nItemHeight))
	{
		ADEBUG_TRACE1("Cannot read height item of <%s>.", pszSection);
		return false;
	}
	if(m_nItemHeight<0)
	{
		ADEBUG_TRACE1("Invalid height item of <%s>.", pszSection);
		return false;
	}
	if(!IniFile.ReadInt(pszSection, "count_item", m_nItemCount))
	{
		ADEBUG_TRACE1("Cannot read count item of <%s>.", pszSection);
		return false;
	}
	if(m_nItemCount<0)
	{
		ADEBUG_TRACE1("Invalid count item of <%s>.", pszSection);
		return false;
	}
	if(m_nItemCount > 0)
	{
		m_pItems	= new ABoxItem[m_nItemCount];
		char szItemBuf[256];
		int i;
		for(i = 0; i < m_nItemCount; i ++)
		{
			sprintf(szItemBuf, "size_item_%d", i);
			if(!IniFile.ReadPoint(
				pszSection, szItemBuf, m_pItems[i].m_ptSize))
			{
				ADEBUG_TRACE2("cannot read item_%d size of <%s>.", i, pszSection);
				return false;
			}

			sprintf(szItemBuf, "image_item_%d", i);
			const char* pszImage	= NULL;
			if(!IniFile.ReadString(
				pszSection, szItemBuf, pszImage))
			{
				ADEBUG_TRACE2("cannot read item_%d image of <%s>.", i, pszSection);
				return false;
			}
			m_pItems[i].m_strSprite	= pszImage;
		}
	}

	const char* pszPanelSel = NULL;
	if(!IniFile.ReadString(pszSection, "image_panelsel", pszPanelSel))
	{
		ADEBUG_TRACE("cannot read panel sel image.");
		return false;
	}
	m_strPanelSel	= pszPanelSel;
	if(!IniFile.ReadRect(pszSection, "rect_panelsel", m_rPanelSel))
	{
		ADEBUG_TRACE("cannot read panel sel rect.");
		return false;
	}

	const char* pszPanelList = NULL;
	if(!IniFile.ReadString(pszSection, "image_panellist", pszPanelList))
	{
		ADEBUG_TRACE("cannot read panel list image.");
		return false;
	}
	m_strPanelList	= pszPanelList;
	if(!IniFile.ReadRect(pszSection, "rect_panellist", m_rPanelList))
	{
		ADEBUG_TRACE("cannot read panel list rect.");
		return false;
	}

	return true;
}

bool CMLComboBox::LoadRes()
{
	J_EXCEPTION_RECORD(0);
	if(!AWnd::LoadRes())
	{
		return false;
	}

	m_pPanelSel	= AWndSpriteMan::CreateSprite(
		m_strPanelSel, m_rPanelSel.GetWidth(), m_rPanelSel.GetHeight(),
		0, 1, &m_rPanelSel);
	if(m_pPanelSel == NULL)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strPanelSel);
	}

	m_pPanelList	= AWndSpriteMan::CreateSprite(
		m_strPanelList, m_rPanelList.GetWidth(), m_rPanelList.GetHeight(),
		0, 1, &m_rPanelList);
	if(m_pPanelList == NULL)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strPanelList);
	}

	int i;
	for(i = 0; i < m_nItemCount; i ++)
	{
		ARECT rSprite(0, 0, m_pItems[i].m_ptSize.x, m_pItems[i].m_ptSize.y);
		m_pItems[i].m_pSprite	=
			AWndSpriteMan::CreateSprite(
				m_pItems[i].m_strSprite,
				rSprite.GetWidth(),
				rSprite.GetHeight(),
				0, 1, &rSprite);
		if(m_pItems[i].m_pSprite == NULL)
		{
			ADEBUG_TRACE1("Cannot load image <%s>", (PCSTR) m_pItems[i].m_strSprite);
		}
	}

	return true;
}

bool CMLComboBox::FreeRes()
{
	J_EXCEPTION_RECORD(0);
	bool bRet	= true;

	int i;
	for(i = 0; i < m_nItemCount; i ++)
	{
		if(m_pItems[i].m_pSprite != NULL)
		{
			AWndSpriteMan::ReleaseSprite(m_pItems[i].m_pSprite);
			m_pItems[i].m_pSprite	= NULL;
		}
	}

	if(m_pPanelSel != NULL)
	{
		AWndSpriteMan::ReleaseSprite(m_pPanelSel);
		m_pPanelSel	= NULL;
	}
	if(m_pPanelList != NULL)
	{
		AWndSpriteMan::ReleaseSprite(m_pPanelList);
		m_pPanelList	= NULL;
	}

	return bRet && AWnd::FreeRes();
}

bool CMLComboBox::OnDraw()
{
	J_EXCEPTION_RECORD(0);
	APOINT ptAb;
	GetAbsolutePos(ptAb);
	if(m_pPanelSel != NULL)
	{
		AWC::DrawSprite(m_pPanelSel, ptAb);
	}

	if(m_nItemCurr >= 0)
	{
		APOINT ptItem(
			ptAb.x + (m_rArea.GetWidth()-m_rArea.GetHeight()-m_pItems[m_nItemCurr].m_ptSize.x)/2,
			ptAb.y + (m_rArea.GetHeight()-m_pItems[m_nItemCurr].m_ptSize.y)/2);
		if(m_pItems[m_nItemCurr].m_pSprite != NULL)
		{
			AWC::DrawSprite(m_pItems[m_nItemCurr].m_pSprite, ptItem);
		}
	}
	while(m_bOpened)
	{
		APOINT ptList(ptAb.x, ptAb.y+m_rArea.GetHeight());
		if(m_pPanelList != NULL)
		{
			AWC::DrawSprite(m_pPanelList, ptList);
		}
		if(m_nItemCount <= 0)
		{
			break;
		}
		int i;
		ARECT	rList(m_rPanelList);
		rList.MoveVer(m_rArea.GetHeight());
		int nMargin		= (rList.GetHeight() - m_nItemCount*m_nItemHeight)/2;
		rList.top		+= nMargin;
		rList.bottom	-= nMargin;
		rList.Move(ptAb);

		for(i = 0; i < m_nItemCount; i ++)
		{
			APOINT ptItem(
				rList.left + (rList.GetWidth()-m_pItems[i].m_ptSize.x)/2,
				rList.top + m_nItemHeight*i + (m_nItemHeight-m_pItems[i].m_ptSize.y)/2);
			if(m_nItemSel == i)
			{
				ARECT rSel(
					rList.left, rList.top + m_nItemHeight*i,
					rList.right, rList.top + m_nItemHeight*(1+i));
				AWC::DrawRectangle(rSel, 0x9F000FAF);
			}
			if(m_pItems[i].m_pSprite != NULL)
			{
				AWC::DrawSprite(m_pItems[i].m_pSprite, ptItem);
			}
		}
		break;
	}

	return true;
}

bool CMLComboBox::OnCreate()
{
	J_EXCEPTION_RECORD(0);
	if(!AWnd::OnCreate())
	{
		return false;
	}

	return true;
}

void CMLComboBox::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLComboBox::OnbtnOpenClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	if(m_bOpened)
	{
		m_btnOpen.Push(false);
		m_bOpened	= false;
		m_btnDown.ShowWindow(false);
		m_btnUp.ShowWindow(false);
		AWC::GetWC()->SetCapture(NULL);
	}
	else
	{
		m_btnDown.ShowWindow(true);
		m_btnUp.ShowWindow(true);
		m_btnOpen.Push(true);
		m_bOpened	= true;
		AWC::GetWC()->SetCapture(this);
		AWnd* pParent	= m_pParent;
		SetParent(NULL);
		SetParent(pParent);
	}

	return true;
}

bool CMLComboBox::OnMouseMsg(AWndMouseMsg& Msg)
{
	J_EXCEPTION_RECORD(0);
	if(!AWnd::OnMouseMsg(Msg))
	{
		return false;
	}

	ARECT	rBox;
	APOINT	ptMouse;
	bool	bDown, bChanged;

	GetAbsoluteArea(rBox);
	Msg.GetPos(ptMouse);
	Msg.GetButtonStatus(AMB_LEFT, bDown, bChanged);
	if(!bDown && bChanged)
	{
		if(!m_bOpened)
		{
			OnbtnOpenClick(NULL);
			return true;
		}
		OnbtnOpenClick(NULL);
	}

	ARECT	rList(m_rPanelList);
	rList.MoveVer(m_rArea.GetHeight());
	int nVisible	= rList.GetHeight() / m_nItemHeight;
	int nMargin		= (rList.GetHeight() - nVisible*m_nItemHeight)/2;
	rList.top		+= nMargin;
	rList.bottom	-= nMargin;
	rList.Move(rBox.GetLeftTop());
	if(!(ptMouse && rList))
	{
		m_nItemSel	= -1;
		return true;
	}

	if(m_nItemCount <= 0)
	{
		return true;
	}
	m_nItemSel	= (ptMouse.y-rList.top)/m_nItemHeight;
	if(m_nItemSel >= m_nItemCount)
	{
		m_nItemSel	= m_nItemCount - 1;
	}
	if(!bDown && bChanged)
	{
		m_nItemCurr	= m_nItemSel;
	}

	return true;
}