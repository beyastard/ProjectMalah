/*
 * FILE: ML_WndVideo.h
 *
 * DESCRIPTION: Video window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDVIDEO_H_
#define	_ML_WNDVIDEO_H_

#include "AWnd.h"
#include "AMVideoClip.h"

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
//	Class CMLWndVideo
//
///////////////////////////////////////////////////////////////////////////

class CMLWndVideo : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndVideo)
		
public:		//	Constructor and Destructor

	CMLWndVideo();
	virtual ~CMLWndVideo();

public:		//	Attributes

	AWndNotify		m_wnOnVideoStop;

public:		//	Operations

	virtual bool	ShowWindow(bool bShow);

	bool	Open(const char* pszFile);
	bool	Start();
	bool	Stop();
	void	Close();
	
	bool	GetIsClickToFinish() { return m_bClickToFinish; }
	void	SetIsClickToFinish(bool bIsClickToFinish) { m_bClickToFinish = bIsClickToFinish;}
	bool	GetIsEscToFinish() { return m_bEscToFinish;}
	void	SetIsEscToFinish(bool bIsEscToFinish) { m_bEscToFinish = bIsEscToFinish;}

protected:	//	Attributes

	AMVideoClip		m_VideoClip;
	bool			m_bInited;
	bool			m_bClickToFinish;
	bool			m_bEscToFinish;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept);
	virtual bool	OnMouseDown(const APOINT& ptClick);

	bool	OnVideoStop();
};


#endif	// _ML_WNDVIDEO_H_