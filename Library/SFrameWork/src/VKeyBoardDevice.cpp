/*
 * FILE: VKeyBoardDevice.cpp
 *
 * DESCRIPTION: class representing the keyboard input device of the game;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VKeyBoardDevice.h"

#include "VGame.h"
#include "VMsgQueue.h"
#include "VKeyBoardTranslator.h"

VKeyBoardDevice::VKeyBoardDevice() : VInputDevice()
{
	m_pKeyTranslator			= NULL;
	m_ptCursorPos.x				= 0;
	m_ptCursorPos.y				= 0;
}

VKeyBoardDevice::~VKeyBoardDevice()
{
}

bool VKeyBoardDevice::Init(HWND hWnd, VGame * pGame, VMsgQueue * pMsgQueue)
{
	if( !VInputDevice::Init(hWnd, pGame, pMsgQueue) )
		return false;

	m_nCursorSettleTime = A3DCounter::GetMicroSecondNow();
	return true;
}

bool VKeyBoardDevice::Release()
{
	if( !VInputDevice::Release() )
		return false;

	return true;
}

bool VKeyBoardDevice::Tick(float vDeltaTime)
{
	if( !VInputDevice::Tick(vDeltaTime) )
		return false;

	memcpy(m_pKeyStateOld, m_pKeyState, sizeof(m_pKeyState));
	for(int i=1; i<=255; i++)
	{
		if( GetAsyncKeyState(i) & 0x8000 )
			m_pKeyState[i] = 1;
		else
			m_pKeyState[i] = 0;
	}

	m_ptCursorPosOld = m_ptCursorPos;
	GetCursorPos(&m_ptCursorPos );
	ScreenToClient(m_hWnd, &m_ptCursorPos);

	// First see if I have the focus
	if( GetFocus() != m_hWnd )
	{
		m_bFocus = false;
		return true;
	}

	// See if out of client rect;
	RECT rectClient;
	GetClientRect(m_hWnd, &rectClient);
	if( m_ptCursorPos.x > rectClient.right || m_ptCursorPos.x < 0 ||
		m_ptCursorPos.y < 0 || m_ptCursorPos.y > rectClient.bottom )
		return true;

	m_bFocus = true;
	// Do something to hide the cursor automatically
	__int64		nTimeNow = A3DCounter::GetMicroSecondNow();
	if( m_ptCursorPos.x != m_ptCursorPosOld.x || m_ptCursorPos.y != m_ptCursorPosOld.y )
	{
		m_nCursorSettleTime = nTimeNow;

		GAMEMSG		msg;
		msg.dwMsgID	= GAMEMSG_SHOWCURSOR;
		m_pMsgQueue->MsgQueueIn(msg);
	}
	else
	{
		if( nTimeNow - m_nCursorSettleTime > 2000000 )
		{
			GAMEMSG		msg;
			msg.dwMsgID = GAMEMSG_HIDECURSOR;
			m_pMsgQueue->MsgQueueIn(msg);
		}
	}

	if( m_pKeyTranslator )
	{
		if( !m_pKeyTranslator->Translate(vDeltaTime) )
			return false;
	}

	return true;
}

bool VKeyBoardDevice::ConnectTranslator(VTranslator * pTranslator)
{
	m_pKeyTranslator = reinterpret_cast<VKeyBoardTranslator *>(pTranslator);
	if( NULL == m_pKeyTranslator )
	{
		m_pGame->ErrLog("VKeyBoardDevice::ConnectTranslator(), You must supply a VKeyBoardTranslator as pTranslator");
		return false;
	}

	if( !VInputDevice::ConnectTranslator(pTranslator) )
	{
		m_pGame->ErrLog("VKeyBoardDevice::ConnectTranslator(), call VInputDevice::ConnectTranslator() Fail!");
		return false;
	}
	return true;
}
