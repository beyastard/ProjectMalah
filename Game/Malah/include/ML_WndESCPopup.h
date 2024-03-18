/*
 * FILE: ML_WndESPPopup.h
 *
 * DESCRIPTION: ESP popup window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDESPPOPUP_H_
#define	_ML_WNDESPPOPUP_H_

#include "AWnd.h"
#include "ML_Button.h"
#include "ML_WndScroll.h"
//	#include "AEnyaWndStat.h"

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
//	Class CMLWndESCPopup
//
///////////////////////////////////////////////////////////////////////////

class CMLWndESCPopup : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndESCPopup)

	enum
	{
		YESNO_EXIT = 0,
		YESNO_RESTART,
	};

public:		//	Constructor and Destructor

	CMLWndESCPopup();
	virtual ~CMLWndESCPopup();

public:		//	Operations

	bool ShowStat();

	virtual bool	ShowWindow(bool bShow);
	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void	SetYesNoMsgBox(CMLMessageBox* pMsgBox);

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLButton		m_btnBack;
	CMLButton		m_btnExit;
	CMLButton		m_btnDefault;
	CMLScrollBar	m_scrSound;
//	CMLScrollBar	m_scrMusic;
	CMLScrollBar	m_scrBrightness;
	float			m_fOldSoundPos;
	float			m_fOldMusicPos;
	float			m_fOldBrightnessPos;

	CMLMessageBox*	m_pYesNoMsgBox;
	int				m_iYesNoTrigger;
//	AEnyaWndStat	m_wndStat;

protected:	//	Operations

	virtual bool	OnDraw();
	virtual bool	OnBackClick(AWnd* pWnd);
	virtual bool	OnExitClick(AWnd* pWnd);
	virtual bool	OnRestartClick(AWnd* pWnd);
	virtual bool	OnYesClick(AWnd* pWnd);
	virtual bool	OnNoClick(AWnd* pWnd);
	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);

	virtual bool	LoadSettings();
	virtual bool	SaveSettings();
	virtual bool	ApplySettings();
};


#endif	// _ML_WNDESPPOPUP_H_