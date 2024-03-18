/*
 * FILE: VMsgQueue.cpp
 *
 * DESCRIPTION: class representing the message queue;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VMsgQueue.h"
#include "VGame.h"

VMsgQueue::VMsgQueue()
{
}

VMsgQueue::~VMsgQueue()
{
}

bool VMsgQueue::Init()
{
	m_nMsgQueueHead = 0;
	m_nMsgQueueTail = 0;

	return true;
}

bool VMsgQueue::Release()
{
	return true;
}

bool VMsgQueue::MsgQueueIn(const GAMEMSG& msg)
{
	if( ((m_nMsgQueueTail + 1) % MAX_MSG_NUM) == m_nMsgQueueHead )
	{
		g_pGame->ErrLog("VMsgQueue::MsgQueueIn(), OverFlow!");
		return false; // Overflow;
	}

	m_arrayMsg[m_nMsgQueueTail ++] = msg;
	m_nMsgQueueTail %= MAX_MSG_NUM;
	return true;
}

bool VMsgQueue::MsgQueueOut(GAMEMSG * pMsg)
{
	if( m_nMsgQueueHead == m_nMsgQueueTail )
	{
		//g_pGame->ErrLog("VMsgQueue::MsgQueueOut(), Empty Queue!");
		return false;//Empty queue
	}

	*pMsg = m_arrayMsg[m_nMsgQueueHead ++];
	m_nMsgQueueHead %= MAX_MSG_NUM;
	return true;
}

// Discard all message in the queue;
bool VMsgQueue::FlushAllMsgs()
{
	m_nMsgQueueHead = m_nMsgQueueTail;

	return true;
}

bool VMsgQueue::Tick(float vDeltaTime)
{

	return true;
}


