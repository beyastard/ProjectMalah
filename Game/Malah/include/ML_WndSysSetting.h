/*
 * FILE: ML_WndSysSetting.h
 *
 * DESCRIPTION: System setting interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDSYSSETTING_H_
#define	_ML_WNDSYSSETTING_H_

#include "AWnd.h"
#include "AEditPlus.h"
#include "ML_Button.h"
#include "ML_WndScroll.h"
#include "ML_ComboBox.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define WRITE_SCROLL(namevar, namescroll)\
	IniFile.WriteInt(g_szSysSec, g_szSys##namevar, (int)(namescroll.GetScrollPos()*100.0f/namescroll.GetScrollLength()))

#define READ_SCROLL(namevar, namescroll) \
	int n##namevar, nDef##namevar	= 50;\
	if(!IniFile.ReadInt(g_szSysSec, g_szSys##namevar, n##namevar))\
	{\
		n##namevar	= nDef##namevar;\
	}\
	if(n##namevar < 0 || n##namevar > 100)\
	{\
		n##namevar = nDef##namevar;\
	}\
	namescroll.SetScrollPos(((float)n##namevar)*(namescroll.GetScrollLength())/100.0f);\

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

class A2DSprite;

extern char g_szSysSec[];
extern char g_szSysDM[];
extern char g_szSysCD[];
extern char g_szSysMI[];
extern char g_szSysBR[];
extern char g_szSysSV[];
extern char g_szSysMV[];
extern char g_szSysMS[];

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWndSysSetting
//
///////////////////////////////////////////////////////////////////////////

class CMLWndSysSetting : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndSysSetting)

	enum
	{
		MODE_COUNT		= 3,
		DEPTH_COUNT		= 2,
		QUALITY_COUNT	= 2,
	};

public:		//	Constructor and Destructor

	CMLWndSysSetting();
	virtual ~CMLWndSysSetting();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
	virtual bool	SaveSetting(AWndIniFile& IniFile);
	virtual bool	LoadSetting(AWndIniFile& IniFile);
	virtual bool	ApplySetting();

protected:	//	Attributes

	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLScrollBar	m_scrBrightness;
	CMLScrollBar	m_scrSound;
	CMLScrollBar	m_scrMusic;
	CMLScrollBar	m_scrMouse;
	float			m_fOldSound;
	float			m_fOldMusic;
	float			m_fOldBrightness;
	AEditPlus		m_wndNameInput;

	CMLComboBox		m_cbMode;
	CMLComboBox		m_cbDepth;
	CMLComboBox		m_cbTexFilter;
	CMLComboBox		m_cbTexQuality;
	CMLComboBox		m_cbEffWeather;
	CMLComboBox		m_cbEffSmoke;
	CMLComboBox		m_cbBlood;
	CMLComboBox		m_cbGraphicsMisc;
	CMLComboBox		m_cbSoundMisc;
	CMLComboBox		m_cbMouseInvert;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();

	bool	IsEmptyName(char* szName);
};


#endif	// _ML_WNDSYSSETTING_H_
