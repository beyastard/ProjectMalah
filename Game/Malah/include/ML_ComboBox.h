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

#ifndef	_ML_COMBOBOX_H_
#define	_ML_COMBOBOX_H_

#include "AWnd.h"
#include "ML_Button.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

class A2DSprite;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLComboBox
//
///////////////////////////////////////////////////////////////////////////

class CMLComboBox : public AWnd
{
public:		//	Type

	DECLARE_ASERIAL(CMLComboBox)

	class ABoxItem
	{
	public:

		AUString	m_strSprite;
		A2DSprite*	m_pSprite;
		APOINT		m_ptSize;

		ABoxItem();
		virtual ~ABoxItem();
	};

public:		//	Constructor and Destructor

	CMLComboBox();
	virtual ~CMLComboBox();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	virtual bool	ShowWindow(bool bShow);

	void	SetCurrent(int nIndex);
	int		GetCurrent();

protected:	//	Attributes

	AUString	m_strPanelSel;
	A2DSprite*	m_pPanelSel;
	ARECT		m_rPanelSel;
	AUString	m_strPanelList;
	A2DSprite*	m_pPanelList;
	ARECT		m_rPanelList;
	CMLButton	m_btnOpen;
	CMLButton	m_btnUp;
	CMLButton	m_btnDown;
	bool		m_bOpened;
	int			m_nItemCount;
	ABoxItem*	m_pItems;
	int			m_nItemSel;
	int			m_nItemCurr;
	int			m_nItemHeight;

protected:	//	Operations

	virtual bool	OnbtnOpenClick(AWnd* pWnd);
	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnMouseMsg(AWndMouseMsg& Msg);
};

#endif	// _ML_COMBOBOX_H_

