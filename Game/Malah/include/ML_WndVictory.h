/*
 * FILE: ML_WndVictory.h
 *
 * DESCRIPTION: Victory (pass scene) window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDVICTORY_H_
#define	_ML_WNDVICTORY_H_

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
class CMLMessageBox;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWndVictory
//
///////////////////////////////////////////////////////////////////////////

class CMLWndVictory : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndVictory)

public:		//	Constructor and Destructor

	CMLWndVictory();
	virtual ~CMLWndVictory();

public:		//	Operations

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void	SetYesNoMsgBox(CMLMessageBox* pMsgBox);

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLButton		m_btnReplay;
	CMLButton		m_btnExit;
	CMLButton		m_btnContinue;

	CMLMessageBox*	m_pYesNoMsgBox;

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnReplayClick(AWnd* pWnd);
	virtual bool	OnExitClick(AWnd* pWnd);
	virtual bool	OnContinueClick(AWnd* pWnd);
	virtual bool	OnYesClick(AWnd* pWnd);
	virtual bool	OnNoClick(AWnd* pWnd);
};


#endif	// _ML_WNDVICTORY_H_