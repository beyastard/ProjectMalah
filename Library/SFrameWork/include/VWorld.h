/*
 * FILE: VWorld.h
 *
 * DESCRIPTION: class representing the whole world
 *
 * CREATED BY: Hedi, 2002/7/29
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VWORLD_H_
#define _VWORLD_H_

#include <A3D.h>   
#include "VGameMsg.h"

class VGame;

class VWorld
{
protected:
	__int64					m_nLastTickTime;
	
	DWORD					m_dwTicks;

protected:
	__int64					m_nStartTime;
	__int64					m_nCurrentTime;

	float					m_vTickTimeSpan;
	VGame					*m_pGame;
	A3DFont					m_fontDebug;
	bool					m_bDebugInfo;

public:
	VWorld();
	~VWorld();

	virtual bool Tick();
	virtual bool TickAnimation();

	virtual bool Restart();

	virtual bool RenderBeforeEngineScene(A3DViewport * pCurrentViewport);
	virtual bool RenderAfterEngineScene(A3DViewport * pCurrentViewport);

	virtual bool Init(VGame * pGame);
	virtual bool Release();

	virtual bool OnMsg(const GAMEMSG& msg, DWORD dwUserData);
	inline VGame * GetVGame() { return m_pGame; }

	inline DWORD GetTicks() { return m_dwTicks; }
	inline float GetTickTimeSpan() { return m_vTickTimeSpan; }
};

#endif//_VWORLD_H_