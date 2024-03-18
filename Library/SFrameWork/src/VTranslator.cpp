/*
 * FILE: VTranslator.cpp
 *
 * DESCRIPTION: class which translate some kind of device message into game message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VTranslator.h"
#include "VGame.h"
#include "VMsgQueue.h"
#include "VInputDevice.h"

VTranslator::VTranslator()
{
	m_pGame			= NULL;
	m_pMsgQueue		= NULL;
}

VTranslator::~VTranslator()
{
}

bool VTranslator::Init(VGame * pGame, VMsgQueue * pMsgQueue)
{
	m_pGame			= pGame;
	m_pMsgQueue		= pMsgQueue;

	return true;
}

bool VTranslator::Release()
{
	return true;
}

bool VTranslator::Translate(float vDeltaTime)
{
	return true;
}

bool VTranslator::SetInputDevice(VInputDevice * pInputDevice)
{
	return true;
}