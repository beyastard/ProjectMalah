/*
 * FILE: ML_WndConsole.h
 *
 * DESCRIPTION: Console window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/1/14
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_WNDCONSOLE_H_
#define	_ML_WNDCONSOLE_H_

#include "AWndConsole.h"

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
//	Class CMLWndConsole
//
///////////////////////////////////////////////////////////////////////////

class CMLWndConsole : public AWndConsole
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndConsole)

public:		//	Constructor and Destructor

	CMLWndConsole();
	virtual ~CMLWndConsole();

public:		//	Attributes

public:		//	Operations

	virtual bool	ShowWindow(bool bShow);
	virtual bool	Redraw();

	bool		AddString(const char* szStr);

protected:	//	Attributes

protected:	//	Operations

	virtual bool	OnInputEnter();
	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnInputKey(AKeyboardButton akb, bool down);
};


#endif	// _ML_WNDCONSOLE_H_

