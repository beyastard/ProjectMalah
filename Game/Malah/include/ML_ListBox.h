/*
 * FILE: ML_ListBox.h
 *
 * DESCRIPTION: List box for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_LISTBOX_H_
#define	_ML_LISTBOX_H_

#include "AWnd.h"
#include "AListBox.h"

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
//	Class CMLListBox
//
///////////////////////////////////////////////////////////////////////////

class CMLListBox : public AListBox
{
public:

	DECLARE_ASERIAL(CMLListBox)

public:		//	Constructor and Destructor

	CMLListBox();
	virtual ~CMLListBox();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	void	SetReadItemCount(int iNum)	{	m_iNumReadItem = iNum;	}
	int		GetCurSel()					{	return m_nSelected;		}
	void	SetCurSel(int iItem);

protected:	//	Attributes

	int		m_iNumReadItem;

protected:	//	Operations

	virtual bool	OnDraw();
};


#endif	// _ML_LISTBOX_H_

