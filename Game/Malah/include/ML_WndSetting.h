/*
 * FILE: ML_WndSetting.h
 *
 * DESCRIPTION: Setting interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDSETTING_H_
#define	_ML_WNDSETTING_H_

#include <AWnd.h>
#include "ML_Button.h"
#include "ML_WndSysSetting.h"
#include "ML_WndKeySetting.h"

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
//	Class CMLWndSetting
//
///////////////////////////////////////////////////////////////////////////

class CMLWndSetting : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndSetting)

public:		//	Constructor and Destructor

	CMLWndSetting();
	virtual ~CMLWndSetting();

public:		//	Attributes

public:		//	Operations

protected:	//	Attributes

	AUString	m_strBackimage;
	A2DSprite*	m_pBackimage;

	CMLButton	m_btnBack;
	CMLButton	m_btnOK;
	CMLButton	m_btnSysSetting;
	CMLButton	m_btnKeySetting;

	CMLWndSysSetting	m_wndSysSetting;
	CMLWndKeySetting	m_wndKeySetting;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnBtnClickBack(AWnd* pWnd);
	virtual bool	OnBtnClickOK(AWnd* pWnd);
	virtual bool	OnBtnClickSysSetting(AWnd* pWnd);
	virtual bool	OnBtnClickKeySetting(AWnd* pWnd);
	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);

public:

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
	virtual bool	SaveSetting();
	virtual bool	LoadSetting();
};


#endif	// _ML_WNDSETTING_H_

