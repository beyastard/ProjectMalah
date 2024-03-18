/*
 * FILE: ML_WndHelp.h
 *
 * DESCRIPTION: Help interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDHELP_H_
#define	_ML_WNDHELP_H_

#include <AWnd.h>
#include "ML_Button.h"
#include "ML_WndPlayHelp.h"
#include "ML_WndUnitHelp.h"

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
//	Class CMLWndHelp
//
///////////////////////////////////////////////////////////////////////////

class CMLWndHelp : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndHelp)

public:		//	Constructor and Destructor

	CMLWndHelp();
	virtual ~CMLWndHelp();

public:		//	Attributes

public:		//	Operations

protected:	//	Attributes

	AUString	m_strBackimage;
	A2DSprite*	m_pBackimage;

	CMLButton	m_btnBack;
	CMLButton	m_btnOK;
	CMLButton	m_btnPlayHelp;
	CMLButton	m_btnUnitHelp;

	CMLWndPlayHelp	m_wndPlayHelp;
	CMLWndUnitHelp	m_wndUnitHelp;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnBtnClickBack(AWnd* pWnd);
	virtual bool	OnBtnClickOK(AWnd* pWnd);
	virtual bool	OnBtnClickPlayHelp(AWnd* pWnd);
	virtual bool	OnBtnClickUnitHelp(AWnd* pWnd);
	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);

public:

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
};


#endif	// _ML_WNDHELP_H_

