/*
 * FILE: ML_WndUnitHelp.h
 *
 * DESCRIPTION: Unit help interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDUNITHELP_H_
#define	_ML_WNDUNITHELP_H_

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
//	Class CMLWndUnitHelp
//
///////////////////////////////////////////////////////////////////////////

class CMLWndUnitHelp : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndUnitHelp)

	//	Buttons
	enum
	{
		BT_PISTOL = 0,
		BT_RIFLE,
		BT_MORTAR,
		BT_MACHINE,
		BT_RPG,
		BT_CANNON,
		BT_ATTACKER,
		BT_COPTER,
		BT_FOOTMAN,
		BT_GUERILLA,
		BT_ROCKET,
		BT_TRUCK,
		BT_TANK,
		BT_ARMORED,
		BT_MIG21,
		BT_FANTAN,
		NUM_BUTTON,
	};

	enum
	{
		MAX_DESCLINE = 10
	};

	struct UNITDESC
	{
		AUString	aLines[MAX_DESCLINE];
		int			iNumLine;
	};

public:		//	Constructor and Destructor

	CMLWndUnitHelp();
	virtual ~CMLWndUnitHelp();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	virtual bool	ShowWindow(bool bShow);
	virtual bool	TickAnimation(float fTimeSpan);			//	Tick animation

protected:	//	Attributes

	AUString	m_strBackimage;
	A2DSprite*	m_pBackimage;

	CMLButton	m_btnPrev;
	CMLButton	m_btnNext;
	CMLButton	m_aButtons[NUM_BUTTON];
	int			m_iNumBtPerLine;		//	Number of button each line
	int			m_iNumBtLine;			//	Number of button line can be showed at same time
	ARECT		m_rcBtArea;				//	Button area

	bool		m_bObserve;				//	true, in observe mode
	int			m_iCurUnit;				//	Current unit

	A3DCamera*		m_pCamera;			//	Unit camera object
	A3DViewport*	m_pViewport;		//	Unit viewport object
	A3DModel*		m_pMoxModel;		//	Model
	UNITDESC		m_UnitDesc;			//	Unit description

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	Redraw();
	virtual bool	OnMouseDown(const APOINT& ptClick);

	virtual bool	OnBtnClickPrev(AWnd* pWnd);
	virtual bool	OnBtnClickNext(AWnd* pWnd);
	virtual bool	OnBtnUnit(AWnd* pWnd);

	void	ShowUnitButtons(bool bShow);
	void	ObserveUnit(bool bOpen, int iUnit);
	bool	CreateCameraAndView();
	void	ReleaseCameraAndView();
	void	RenderUnit();
	bool	LoadUnitDesc(int iUnit);	//	Load unit description text
};



#endif	// _ML_WNDUNITHELP_H_

