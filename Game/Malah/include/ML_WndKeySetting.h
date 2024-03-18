/*
 * FILE: ML_WndKeySetting.h
 *
 * DESCRIPTION: Key setting interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDKEYSETTING_H_
#define	_ML_WNDKEYSETTING_H_

#include "AWnd.h"
#include "ML_KeyBox.h"

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
//	Class CMLWndKeyBox
//
///////////////////////////////////////////////////////////////////////////

class CMLWndKeyBox : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndKeyBox)

public:		//	Constructor and Destructor

	CMLWndKeyBox();
	virtual ~CMLWndKeyBox();

public:		//	Attributes

public:		//	Operations

	virtual bool LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool LoadRes();
	virtual bool FreeRes();

	UINT	GetKey();
	void	SetKey(UINT uKey);

protected:	//	Attributes

	UINT	m_uKeyValue;
	bool	m_bFirstMouseBtn;
	APOINT	m_ptTextExtent;

protected:	//	Operations

	virtual bool OnSetFocus(bool bFocus);
	virtual bool OnDraw();
	virtual bool OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);
	virtual bool OnMouseDown(const APOINT& ptClick);
};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWndKeySetting
//
///////////////////////////////////////////////////////////////////////////

class CMLWndKeySetting : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndKeySetting)

	enum KEY_SETTING_CONST
	{
		MAX_KEY_BOX	= 13,
	};

public:		//	Constructor and Destructor

	CMLWndKeySetting();
	virtual ~CMLWndKeySetting();

public:		//	Attributes

public:		//	Operations

	AUString&	GetKeyString(int nKey);

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
	virtual bool	SaveSetting(AWndIniFile& IniFile);
	virtual bool	LoadSetting(AWndIniFile& IniFile);

protected:	//	Attributes

	friend class CMLWndKeyBox;

	AUString		m_strKeyMap[256];
	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;

	CMLKeyBox		m_wndKeyBox;
	CMLWndKeyBox	m_kbSettings[MAX_KEY_BOX];

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual void	OnKeyChanged(CMLWndKeyBox* pBox);
};



#endif	// _ML_WNDKEYSETTING_H_

