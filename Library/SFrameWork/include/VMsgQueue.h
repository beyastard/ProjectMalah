/*
 * FILE: VMsgQueue.h
 *
 * DESCRIPTION: class representing the input device of the game;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VMSGQUEUE_H_
#define _VMSGQUEUE_H_

#include <Windows.h>
#include "VGameMsg.h"

#define MAX_MSG_NUM				128

class VMsgQueue
{
private:
	GAMEMSG				m_arrayMsg[MAX_MSG_NUM];
	int					m_nMsgQueueHead;
	int					m_nMsgQueueTail;

protected:

public:
	VMsgQueue();
	~VMsgQueue();

	bool Init();
	bool Release();

	bool Tick(float vDeltaTime);

	// push a message into the queue;
	bool MsgQueueIn(const GAMEMSG& msg);
	// pop a message out of the queue;
	bool MsgQueueOut(GAMEMSG * pMsg);

	// Discard all message in the queue
	bool FlushAllMsgs();
};

#endif//_VMSGQUEUE_H_
