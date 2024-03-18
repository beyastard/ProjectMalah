/*
 * FILE: ML_Scroll.h
 *
 * DESCRIPTION: Scroll bar control for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDSCROLL_H_
#define	_ML_WNDSCROLL_H_

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
//	Class CMLScrollBarButton
//
///////////////////////////////////////////////////////////////////////////

class CMLScrollBarButton : public CMLButton
{
public:		//	Types

	DECLARE_ASERIAL(CMLScrollBarButton)

public:		//	Constructor and Destructor

	CMLScrollBarButton() {}
	virtual ~CMLScrollBarButton() {}

public:		//	Attributes

public:		//	Operations

	void ForceClick();

protected:	//	Attributes

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnMouseDown(const APOINT& ptClick);
	virtual bool	OnMouseMsg(AWndMouseMsg& Msg);
};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLScrollBar
//
///////////////////////////////////////////////////////////////////////////

class CMLScrollBar : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLScrollBar)

public:		//	Constructor and Destructor

	CMLScrollBar();
	virtual ~CMLScrollBar();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void	SetScrollPos(float nPos);
	float	GetScrollLength();

	void	SetScrollStep(float nStep)	{	m_nScrollStep	= nStep;	}
	void	SetVertical(bool bVertical) {	m_bVertical = bVertical;	}
	float	GetScrollPos()				{	return m_nScrollPos;		}
	float	GetScrollStep()				{	return m_nScrollStep;		}
	bool	IsVertical()				{	return m_bVertical;			}
	void	SetMinData(float fMin)		{	m_fMinData = fMin;			}
	void	SetMaxData(float fMax)		{	m_fMaxData = fMax;			}
	float	GetRangeSize()				{	return m_fMaxData - m_fMinData;	}

protected:	//	Attributes

	friend class CMLScrollBarButton;

	float		m_nScrollPos;
	float		m_fMaxData;
	float		m_fMinData;
	bool		m_bIntData;
	float		m_nScrollStep;
	bool		m_bVertical;
	CMLButton	m_btnLeftTop;
	CMLButton	m_btnRightBottom;
	bool		m_bHideArrows;		//	true, hide arrows at ends of scroll bar

	CMLScrollBarButton	m_btnScroll;

	AUString	m_strScrollimage;
	A2DSprite*	m_pScrollimage;
	AUString	m_strBackimage;
	A2DSprite*	m_pBackimage;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnScrollMove(const APOINT& ptMove);
	virtual bool	OnbtnClick(AWnd* pWnd);
	virtual bool	OnMouseDown(const APOINT& ptClick);
};


#endif	// _ML_WNDSCROLL_H_

