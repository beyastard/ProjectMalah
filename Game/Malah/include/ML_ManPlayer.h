/*
 * FILE: ML_ManPlayer.h
 *
 * DESCRIPTION: Player manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANPLAYER_H_
#define _ML_MANPLAYER_H_

#include "ML_Player.h"
#include "ML_ManBase.h"

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

class CMLTransPlayerKeyboard;
class CMLTransPlayerMouse;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLManPlayer
//
///////////////////////////////////////////////////////////////////////////

class CMLManPlayer : public CMLManBase
{
public:		//	Types

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManPlayer)

	CMLManPlayer();
	virtual ~CMLManPlayer() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);				//	Initialize object
	virtual bool	Release();						//	Release object

	virtual bool	LogicRun(float fTimeSpan);		//	Logic tick
	virtual int		OnMsg(AMSG* pMSG);				//	Message handler

	virtual bool	TickAnimation();					//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);		//	Render routines

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	bool		AddPlayer(CMLPlayer* pPlayer);	//	Add a player
	CMLPlayer*	RemovePlayer(int iID);			//	Remove player from manager
	CMLPlayer*	GetPlayerByID(int iID);			//	Get player object by ID
	bool		BindTrans(int iBindType);		//	Bind translator

	CMLTransPlayerKeyboard*	GetTransKeyboard()	{	return m_pKBTrans;		}
	CMLTransPlayerMouse*	GetTransMouse()		{	return m_pMouseTrans;	}

protected:	//	Attributes

	AListT<CMLPlayer*>		m_PlayerList;		//	Player list

	CMLTransPlayerKeyboard*	m_pKBTrans;			//	Keyboard translator
	CMLTransPlayerMouse*	m_pMouseTrans;		//	Mouse translator

	DWORD		m_dwTimeCnt;		//	Time counter

protected:	//	Operations

	virtual bool	ProcessMsg(MLMSG* pMsg);		//	Handle a message
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANPLAYER_H_

