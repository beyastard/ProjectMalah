/*
 * FILE: ML_WndPlayHelp.h
 *
 * DESCRIPTION: Play help interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDPLAYHELP_H_
#define	_ML_WNDPLAYHELP_H_

#include "AWnd.h"
#include "ML_Button.h"
#include "ML_WndScroll.h"

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
//	Class CMLWndPlayHelp
//
///////////////////////////////////////////////////////////////////////////

class CMLWndPlayHelp : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndPlayHelp)

	enum
	{
		MAX_DESCLINE = 256
	};

public:		//	Constructor and Destructor

	CMLWndPlayHelp();
	virtual ~CMLWndPlayHelp();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	virtual bool	ShowWindow(bool bShow);

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLScrollBar	m_scrText;		//	Scroll bar
	ARECT			m_rcDesc;		//	Description area

	AUString*		m_aDescLines[MAX_DESCLINE];
	int				m_iNumLine;
	int				m_iFirstLine;	//	First line
	int				m_iNumShowed;	//	Number of showed line

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();

	bool	LoadDescription();		//	Load description
	void	ReleaseAllDesc();		//	Release all description
};



#endif	// _ML_WNDPLAYHELP_H_

