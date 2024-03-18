/*
 * FILE: VInputDevice.cpp
 *
 * DESCRIPTION: class representing the input device of the game;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VInputDevice.h"
#include "VGame.h"
#include "VMsgQueue.h"
#include "VTranslator.h"

VInputDevice::VInputDevice()
{
	m_bActive			= true;
	m_hWnd				= NULL;
	m_pGame				= NULL;
	m_pMsgQueue			= NULL;

}

VInputDevice::~VInputDevice()
{
}

bool VInputDevice::Init(HWND hWnd, VGame * pGame, VMsgQueue * pMsgQueue)
{
	m_hWnd			= hWnd;
	m_pGame			= pGame;
	m_pMsgQueue		= pMsgQueue;

	return true;
}

bool VInputDevice::Release()
{
	return true;
}

bool VInputDevice::Tick(float vDeltaTime)
{
	return true;
}

bool VInputDevice::ConnectTranslator(VTranslator * pTranslator)
{
	return pTranslator->SetInputDevice(this);
}
