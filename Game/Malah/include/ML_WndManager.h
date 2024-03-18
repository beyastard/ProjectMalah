/*
 * FILE: ML_WndManager.h
 *
 * DESCRIPTION: Window manager for Malah game
 *
 * CREATED BY: Duyuxin, 2003/1/13
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WNDMANAGER_H_
#define	_ML_WNDMANAGER_H_

#include "ML_WndConsole.h"
#include "ML_WndLoading.h"
#include "ML_WndESCPopup.h"
#include "ML_WndVictory.h"
#include "ML_WndFailure.h"
#include "ML_WndFinalWin.h"
#include "ML_WndVideo.h"
#include "ML_MessageBox.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Video
enum
{
	VIDEO_LOGO = 0
};

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
//	Class CMLWndManager
//
///////////////////////////////////////////////////////////////////////////

class CMLWndManager : public AWC
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndManager)

public:		//	Constructor and Destructor

	CMLWndManager();
	virtual ~CMLWndManager();

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(const AWC_INIT& wci);	//	Initailize object
	virtual bool	Release();		//	Release resources

	virtual bool	Draw();			//	Draw
	virtual bool	SetDisplayMode(int iWid, int iHei, int iColorBit);	//	Set display mode
	virtual int		SetActiveStatus(int iActive);		//	Set active state

	bool		LoadInGameInterface();		//	Load interface used in game
	void		FreeInGameInterface();		//	Free interface used in game
	void		ReturnToMainMenu();			//	Return to main menu from game

	bool		ActivateWndConsole(bool bActive);		//	Activate console 
	bool		ActivateWndLoading(bool bActive);		//	Activate loading interface
	bool		ActivateWndEscape(bool bActive);		//	Activate escape panel
	bool		ActivateWndVictory(bool bActive);		//	Activate pass mission panel
	bool		ActivateWndFailure(bool bActive);		//	Activate failure panel
	bool		ActivateWndScore(bool bActive);			//	Activate score window
	bool		ActivateWndFinalWin(bool bActive);		//	Activate victory panel
	bool		ActivateWndVideo(bool bActive);			//	Activate video window

	bool		ShowStat();
	bool		AddConsoleString(const char* szStr);	//	Add console string
	bool		StartGame(int iLevel);					//	Start game

	bool		GetShowCursor()				{	return m_bShowCursor;	}
	void		SetShowCursor(bool bShow)	{	m_bShowCursor = bShow;	}
	int			GetCurVideo()				{	return m_iCurVideo;		}
	void		SetCurVideo(int iVideo)		{	m_iCurVideo = iVideo;	}

protected:	//	Attributes

	CMLWndConsole	m_WndConsole;		//	Console window
	CMLWndLoading	m_WndLoading;		//	Loading window
	CMLWndESCPopup	m_WndEscape;		//	Escape panel
	CMLWndVictory	m_WndVictory;		//	Pass mission panel
	CMLWndFailure	m_WndFailure;		//	Failure panel
	CMLWndFinalWin	m_WndFinalWin;		//	Final win panel
	CMLWndVideo		m_wndVideo;			//	Video window

	CMLMessageBox	m_YesNoMegBox;		//	Yes or No message box
	CMLWndScore		m_WndScore;			//	Score window

	A3DCursor*		m_pCursor;
	bool			m_bShowCursor;
	int				m_iCurVideo;		//	Current video

protected:	//	Operations

	virtual bool	CreateMainWnd();
	virtual bool	OnVideoStop(AWnd* pWnd);
	
	void		ShowLoadingWait();		//	Show loading wait picture
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	// _ML_WNDMANAGER_H_

