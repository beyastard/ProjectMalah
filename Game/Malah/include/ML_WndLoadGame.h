/*
 * FILE: ML_WndLoadGame.h
 *
 * DESCRIPTION: Load game interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDLOADGAME_H_
#define	_ML_WNDLOADGAME_H_

#include "AWnd.h"
#include "ML_ListBox.h"
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
//	Class CMLWndLoadGame
//
///////////////////////////////////////////////////////////////////////////

class CMLWndLoadGame : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndLoadGame)

public:		//	Constructor and Destructor

	CMLWndLoadGame();
	virtual ~CMLWndLoadGame();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;
	CMLButton		m_btnOk;
	CMLButton		m_btnLoad;

	CMLListBox		m_MissionList;		//	Mission list
	CMLScrollBar	m_scrMission;		//	Scroll bar
	float			m_fOldMission;	
	int				m_iMaxLevel;		//	Maximum level

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnBtnClickBack(AWnd* pWnd);
	virtual bool	OnBtnClickLoad(AWnd* pWnd);
public:
	void SetLimitItemNum(int maxLevel) {m_iMaxLevel = maxLevel;}
};



#endif	// _ML_WNDLOADGAME_H_

