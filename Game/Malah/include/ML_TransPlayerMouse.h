/*
 * FILE: ML_TransPlayerMouse.h
 *
 * DESCRIPTION: Player mouse translator for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_TRANSPLAYERMOUSE_H_
#define _ML_TRANSPLAYERMOUSE_H_

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
//	Class CMLTransPlayerMouse
//
///////////////////////////////////////////////////////////////////////////

class CMLTransPlayerMouse : public ATrans
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLTransPlayerMouse();
	virtual ~CMLTransPlayerMouse() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	TranslateIn(ADevice *pADevice);

protected:	//	Attributes

protected:	//	Operations

	void		CenterWindowMouse();	//	Center window's mouse
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_TRANSPLAYERMOUSE_H_
