/*
 * FILE: ML_ManMessage.cpp
 *
 * DESCRIPTION: Message manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_ManMessage.h"
#include "ML_ManagerDef.h"
#include "ML_ID.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLManMessage
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManMessage, CMLManBase, 1)

CMLManMessage::CMLManMessage() : CMLManBase()
{
	m_managerId = MANID_MESSAGE;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManMessage::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);
	
	m_MsgQueue.Release();
	
	if (m_MsgQueue.Init(sizeof (MLMSG) * 1024) == -1)
	{
		ADEBUG_TRACE("Failed to initialize message queue");
		return false;
	}

	return true;
}

/*	Logic tick

	Return false to terminate game.

	fTimeSpan: time span of this tick
*/
bool CMLManMessage::LogicRun(float fTimeSpan)
{
	BeginLogicCount();
	
	MLMSG* pMsg;
	int iSize;

	//	Dispatch all messages
	while (m_MsgQueue.GetNum())
	{
		m_MsgQueue.PeekQueue((void**) &pMsg, &iSize);

		if (!pMsg)
		{
			ADEBUG_TRACE("Invalid message object");
			break;
		}

		//	Handle messages
		if (!ProcessMsg(pMsg))
		{
			ClearMessages();
			break;
		}
	
		m_MsgQueue.DelQueueHead();
	}

	EndLogicCount();

	return true;
}

/*	Handle a message

	Return true for success, otherwise return false.

	pMsg: message will be processed
*/
bool CMLManMessage::ProcessMsg(MLMSG* pMsg)
{
	if (!ISMANAGERID(pMsg->idManager))
		return m_pAGame->GetAGamePlay()->OnMsg((AMSG*)pMsg) ? true : false;

	//	Send to manager
	AManager* pManager = m_pAGame->GetAGamePlay()->GetManagerById(pMsg->idManager);
	if (!pManager)
	{
		assert(0);
		return false;
	}

	return pManager->OnMsg((AMSG*)pMsg) ? true : false;
}

/*	Add a message

	pMsg: message object's address
*/
bool CMLManMessage::AddMsg(MLMSG* pMsg)
{
	return m_MsgQueue.Push(pMsg, sizeof (MLMSG)) ? false : true;
}

//	Clear all messages	 
void CMLManMessage::ClearMessages()
{
	m_MsgQueue.ClearAll();
}

