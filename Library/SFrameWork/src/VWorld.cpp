/*
 * FILE: VWorld.cpp
 *
 * DESCRIPTION: class representing the whole world
 *
 * CREATED BY: Hedi, 2002/7/29
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */
#include "VWorld.h"
#include "VGame.h"
#include "VGameMsg.h"
						  
VWorld::VWorld()
{
	m_pGame			= NULL;
	m_vTickTimeSpan = 0.0f;
}

VWorld::~VWorld()
{
}

bool VWorld::Init(VGame * pGame)
{
	m_pGame = pGame;
	
	m_dwTicks = 0;

	m_bDebugInfo = true;
	char szCommandLine[1024];
	strncpy(szCommandLine, GetCommandLine(), 1024);
	_strlwr(szCommandLine);
	if( strstr(szCommandLine, "-nodbginfo") )
		m_bDebugInfo = false;

	if( m_bDebugInfo )
	{
		HA3DFONT hA3DFont = m_pGame->GetA3DEngine()->GetA3DFontMan()->RegisterFont("Courier New", 12, NULL);
		if( !m_fontDebug.Init(m_pGame->GetA3DEngine()->GetA3DDevice(), hA3DFont, false) )
		{
			m_pGame->ErrLog("VWorld::Init(), Create debug font fail!");
			return false;
		}
	}
	
	return true;
}

bool VWorld::Release()
{
	if( m_bDebugInfo )
		m_fontDebug.Release();
	
	return true;
}

bool VWorld::TickAnimation()
{
	return true;
}

bool VWorld::Restart()
{
	m_nStartTime = m_nLastTickTime = A3DCounter::GetMicroSecondNow();

	return true;
}

bool VWorld::Tick()
{
	if( m_dwTicks == 0 )
		m_nStartTime = m_nLastTickTime = A3DCounter::GetMicroSecondNow();

	m_nCurrentTime = A3DCounter::GetMicroSecondNow();
	m_vTickTimeSpan = (m_nCurrentTime - m_nLastTickTime) * 0.000001f;
	m_nLastTickTime = m_nCurrentTime;

	m_dwTicks ++;
	return true;
}

bool VWorld::RenderBeforeEngineScene(A3DViewport * pCurrentViewport)
{
	return true;
}

bool VWorld::RenderAfterEngineScene(A3DViewport * pCurrentViewport)
{
	return true;
}

bool VWorld::OnMsg(const GAMEMSG& msg, DWORD dwUserData)
{
	return true;
}