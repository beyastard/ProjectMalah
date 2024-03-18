/*
 * FILE: ML_WndESPPopup.h
 *
 * DESCRIPTION: Failure window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDFAILURE_H_
#define	_ML_WNDFAILURE_H_

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
//	Class CMLWndFailure
//
///////////////////////////////////////////////////////////////////////////

class CMLWndFailure : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndFailure)

public:		//	Constructor and Destructor

	CMLWndFailure();
	virtual ~CMLWndFailure();

public:		//	Operations

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void	SetYesNoMsgBox(CMLMessageBox* pMsgBox);

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLButton		m_btnRestart;
	CMLButton		m_btnExit;

	CMLMessageBox*	m_pYesNoMsgBox;

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnRestartClick(AWnd* pWnd);
	virtual bool	OnExitClick(AWnd* pWnd);
	virtual bool	OnYesClick(AWnd* pWnd);
	virtual bool	OnNoClick(AWnd* pWnd);
};


#endif	// _ML_WNDFAILURE_H_