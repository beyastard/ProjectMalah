/*
 * FILE: VKeyBoardTranslator.h
 *
 * DESCRIPTION: class which take translate a keyboard message into game message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VKEYBOARDTRANSLATOR_H_
#define _VKEYBOARDTRANSLATOR_H_

#include <Windows.h>
#include "VTranslator.h"

class VKeyBoardDevice;

class VKeyBoardTranslator : public VTranslator
{
protected:
	VKeyBoardDevice		*m_pKeyBoardDevice;

public:
	VKeyBoardTranslator();
	~VKeyBoardTranslator();

	bool Translate(float vDeltaTime);
	bool SetInputDevice(VInputDevice * pInputDevice);
};

#endif//_VTRANSLATOR_H_
