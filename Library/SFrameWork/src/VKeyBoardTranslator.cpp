/*
 * FILE: VKeyBoardTranslator.cpp
 *
 * DESCRIPTION: class which take a VMsgBinder and a input device as input and output message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VKeyBoardTranslator.h"

#include "VGame.h"
#include "VMsgQueue.h"
#include "VKeyBoardDevice.h"

VKeyBoardTranslator::VKeyBoardTranslator() : VTranslator()
{
	m_pKeyBoardDevice		= NULL;
}

VKeyBoardTranslator::~VKeyBoardTranslator()
{
}

bool VKeyBoardTranslator::Translate(float vDeltaTime)
{
	if( !m_pKeyBoardDevice || !m_pMsgQueue )
		return true;

	GAMEMSG			msg;

	if( m_pKeyBoardDevice->IsKeyPressedNow(VK_ESCAPE) )
	{
		msg.dwMsgID			= GAMEMSG_EXIT;
		m_pMsgQueue->MsgQueueIn(msg);
	}
	return true;
}

bool VKeyBoardTranslator::SetInputDevice(VInputDevice * pInputDevice)
{
	// First you should check the device must be a VKeyBoardDevice
	m_pKeyBoardDevice = reinterpret_cast<VKeyBoardDevice *>(pInputDevice);
	if( NULL == m_pKeyBoardDevice )
	{
		m_pGame->ErrLog("VKeyBoardTranslator::SetInputDevice(), the supplied pInputDevice is no a VKeyBoardDevice");
		return false;
	}

	return true;	
}