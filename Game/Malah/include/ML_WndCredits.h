/*
 * FILE: ML_WndCredits.h
 *
 * DESCRIPTION: Credits interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDCREDITS_H_
#define	_ML_WNDCREDITS_H_

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
//	Class CMLWndCredits
//
///////////////////////////////////////////////////////////////////////////

class CMLWndCredits : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndCredits)

public:		//	Constructor and Destructor

	CMLWndCredits();
	virtual ~CMLWndCredits();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;
	CMLButton		m_btnOk;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnBtnClickBack(AWnd* pWnd);
};



#endif	// _ML_WNDUNITHELP_H_

