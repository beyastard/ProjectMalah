/*
 * FILE: ML_Button.h
 *
 * DESCRIPTION: Button control for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_BUTTON_H_
#define	_ML_BUTTON_H_

#include "AButton.h"

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
//	Class CMLButton
//
///////////////////////////////////////////////////////////////////////////

class CMLButton : public AButton
{
public:		//	Types

	DECLARE_ASERIAL(CMLButton)

public:		//	Constructor and Destructor

	CMLButton();
	virtual ~CMLButton();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	bool	SetIniInfo(AWnd* pParent, const char* szBackImage, ARECT& Rect);

protected:	//	Attributes

	AUString		m_strButtonimage;
	A2DSprite*		m_pButtonimage;
	APOINT			m_ptImageSize;
	ARECT			m_rButtonRects[4];

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnClick();
};


#endif	// _ML_BUTTON_H_
