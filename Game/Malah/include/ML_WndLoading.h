/*
 * FILE: ML_WndLoading.h
 *
 * DESCRIPTION: Loading interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDLOADING_H_
#define	_ML_WNDLOADING_H_

#include "AWnd.h"
#include "AButton.h"
#include "ML_WndMainMenu.h"
//	#include "AEnyaWndVideo.h"

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


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLLoadButton
//
///////////////////////////////////////////////////////////////////////////

class CMLLoadButton : public AButton
{
public:		//	Types

	DECLARE_ASERIAL(CMLLoadButton)

public:		//	Constructor and Destructor

	CMLLoadButton() {}
	virtual ~CMLLoadButton() {}

public:		//	Attributes

public:		//	Operations

protected:	//	Attributes

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnClick();
	virtual bool	PointIn(const APOINT& pt);
};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWndLoading
//
///////////////////////////////////////////////////////////////////////////

class CMLWndLoading : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndLoading)

public:		//	Constructor and Destructor

	CMLWndLoading();
	virtual ~CMLWndLoading();

public:		//	Attributes

public:		//	Operations

	virtual int		OnLoadingUpdate(float fPercent);
	virtual bool	ShowWindow(bool bShow);
	virtual bool	ProcessMouseMsg(AWndMouseMsg& Msg);

	void SetMisPassCount(int nCount, bool bTemp = false)
	{
		m_wndMainMenu.SetMisPassCount(nCount, bTemp);
	}

	int GetMisPassCount()
	{
		return m_wndMainMenu.GetMisPassCount();
	}

protected:	//	Attributes

	friend class	CMLLoadButton;

//	CMLLoadButton	m_btnLoading;
	CMLWndMainMenu	m_wndMainMenu;
//	AEnyaWndVideo	m_wndVideoHead;

protected:	//	Operations

	virtual bool	OnLoadingClick();
	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnHeadVideoStop(AWnd* pWnd);
};

#endif	// _ML_WNDLOADING_H_

