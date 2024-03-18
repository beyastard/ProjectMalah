/*
 * FILE: ML_WndMainMenu.h
 *
 * DESCRIPTION: Main menu interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDMAINMENU_H_
#define	_ML_WNDMAINMENU_H_

#include "AWnd.h"
#include "AWndIniFile.h"
#include "ML_Button.h"
#include "ML_WndSetting.h"
#include "ML_WndScore.h"
#include "ML_WndHelp.h"
#include "ML_WndLoadGame.h"
#include "ML_WndCredits.h"

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
//	Class CMLWndMainMenu
//
///////////////////////////////////////////////////////////////////////////

class CMLWndMainMenu : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndMainMenu)

public:		//	Constructor and Destructor

	CMLWndMainMenu();
	virtual ~CMLWndMainMenu();

public:		//	Attributes

public:		//	Operations

	void	LoadSetting();

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void SetMisPassCount(int nCount, bool bTemp = false)
	{
	//	m_wndMission.SetMisPassCount(nCount, bTemp);
	}

	int GetMisPassCount()
	{
	//	return m_wndMission.GetMisPassCount();
	}

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLButton		m_btnNewGame;
	CMLButton		m_btnLoadGame;
	CMLButton		m_btnScore;
	CMLButton		m_btnHelp;
	CMLButton		m_btnCredits;
	CMLButton		m_btnSetup;
	CMLButton		m_btnQuit;

	CMLWndSetting	m_wndSetting;
	CMLWndScore		m_wndScore;
	CMLWndHelp		m_wndHelp;
	CMLWndLoadGame	m_wndLoadGame;
	CMLWndCredits	m_wndCredits;

	AWnd*			m_pCurWnd;
	AUString		m_strCopyright;

protected:	//	Operations

	bool	OnBtnClickNewGame(AWnd* pWnd);
	bool	OnBtnClickLoadGame(AWnd* pWnd);
	bool	OnBtnClickScore(AWnd* pWnd);
	bool	OnBtnClickHelp(AWnd* pWnd);
	bool	OnBtnClickCredits(AWnd* pWnd);
	bool	OnBtnClickSetup(AWnd* pWnd);
	bool	OnBtnClickQuit(AWnd* pWnd);

	bool	LoadWindowRes(AWnd* pWnd);		//	Load window resources

	virtual bool	OnDraw();
};

#endif	// _ML_WNDMAINMENU_H_
