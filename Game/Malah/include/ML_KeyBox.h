/*
 * FILE: ML_KeyBox.h
 *
 * DESCRIPTION: Key box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_KEYBOX_H_
#define	_ML_KEYBOX_H_

#include "AWndGrid.h"

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
//	Class CMLKeyBox
//
///////////////////////////////////////////////////////////////////////////

class CMLKeyBox : public AWndGrid
{
public:

	DECLARE_ASERIAL(CMLKeyBox)

public:		//	Constructor and Destructor

	CMLKeyBox();
	virtual ~CMLKeyBox();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();
	virtual bool	SaveSetting(AWndIniFile& IniFile);
	virtual bool	LoadSetting(AWndIniFile& IniFile);

protected:	//	Attributes

	AUString	m_strKeyMap[256];

protected:	//	Operations

	void	ChangeKeySetting(int nKey);

	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);
	virtual bool	OnMouseMsg(AWndMouseMsg& Msg);
	virtual bool	OnDraw();
	virtual bool	OnDrawGrid(int nRow, int nCol, ARECT rGrid);
	virtual bool	OnSel(const APOINT& ptNewSel, bool& bCanSel);
};


#endif	// _ML_KEYBOX_H_
