/*
 * FILE: VTranslator.h
 *
 * DESCRIPTION: class which translate some kind of device message into game message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VTRANSLATOR_H_
#define _VTRANSLATOR_H_

#include <Windows.h>

class VGame;
class VMsgQueue;
class VInputDevice;

class VTranslator
{
	friend class VInputDevice;

protected:
	VGame				*m_pGame;
	VMsgQueue			*m_pMsgQueue;

	virtual bool SetInputDevice(VInputDevice * pInputDevice);

public:
	VTranslator();
	~VTranslator();

	virtual bool Init(VGame * pGame, VMsgQueue * pMsgQueue);
	virtual bool Release();
	virtual bool Translate(float vDeltaTime);
};

#endif//_VTRANSLATOR_H_
