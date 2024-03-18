/*
 * FILE: ML_ManBase.h
 *
 * DESCRIPTION: Manager base class in game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_MANBASE_H_
#define _ML_MANBASE_H_

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
//	Class CMLManBase
//
///////////////////////////////////////////////////////////////////////////

class CMLManBase : public AManager
{
public:		//	Types

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManBase)

public:		//	Attributes

public:		//	Operations

	virtual bool	TickAnimation() { return true; }					//	Tick animation
	virtual bool	Render(A3DViewport* pViewport) { return true; }		//	Render routines

	virtual bool	OnStartGamePlay() { return true; }				//	Called when begin game playing
	virtual bool	OnEndGamePlay() { return true; }				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission) { return true; }	//	Called when being mission
	virtual bool	OnResetMission() { return true; }				//	Called when reset mission
	virtual bool	OnEndMission() { return true; }					//	Called when end mission

	virtual bool	LoadMissionResources(AArchive& ar) { return true; }	//	Load game mission
	virtual void	ReleaseMissionResources() {}						//	Release mission resources

	virtual void	PauseSound(bool bPause) {}	//	Pause sound
	virtual void	StopSound() {}				//	Stop sound

	DWORD	GetLogicTime()	{ return m_dwLogicTime; }

protected:	//	Attributes

	DWORD	m_dwLogicTime;
	__int64	m_iiTimeCount;
	
protected:	//	Operations

	void BeginLogicCount()	{ m_iiTimeCount = A3DCounter::GetCPUCycle(); }

	void EndLogicCount()
	{
		m_dwLogicTime = (DWORD)(A3DCounter::GetCPUCycle() - m_iiTimeCount);
		m_dwLogicTime /= (DWORD)(A3DCounter::GetCPUFrequency() / 1000000);
	}

	virtual bool	ProcessMsg(MLMSG* pMsg) { return true; }		//	Handle a message
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANBASE_H_


