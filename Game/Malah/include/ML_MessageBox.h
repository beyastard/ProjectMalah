/*
 * FILE: ML_MessageBox.h
 *
 * DESCRIPTION: Message box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_MESSAGEBOX_H_
#define	_ML_MESSAGEBOX_H_

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
//	Class CMLMessageBox
//
///////////////////////////////////////////////////////////////////////////

class CMLMessageBox : public AWnd
{
public:

	DECLARE_ASERIAL(CMLMessageBox)

public:		//	Constructor and Destructor

	CMLMessageBox();
	virtual ~CMLMessageBox();

public:		//	Attributes

	CMLButton	m_btnYes;
	CMLButton	m_btnNo;

public:		//	Operations

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);

protected:	//	Attributes

	AUString	m_strBackimage;
	A2DSprite*	m_pBackimage;

protected:	//	Operations

	virtual bool	OnDraw();
};


#endif	// _ML_MESSAGEBOX_H_

