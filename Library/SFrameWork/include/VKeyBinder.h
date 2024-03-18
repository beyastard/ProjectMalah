/*
 * FILE: VMsgBinder.h
 *
 * DESCRIPTION: class representing the binding table between input and message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VKEYBINDER_H_
#define _VKEYBINDER_H_

#include <Windows.h>

#include "VGameMsg.h"

#define GAME_KEY_COMBINE_MAX			3

typedef struct _KEYBIND
{
	int			vkey[GAME_KEY_COMBINE_MAX];
	DWORD		dwMsgID;
} KEYBIND;

class VKeyBinder
{
private:
	int			m_nKeyBindNum;
	KEYBIND		*m_pKeyBindTable;

protected:

public:
	VKeyBinder();
	~VKeyBinder();

	bool Init();
	bool Release();

	inline int GetKeyBindCount() { return m_nKeyBindNum; }
	virtual bool GetKeyBind(int index, KEYBIND * pKeyBind);
};

#endif//_VKEYBINDER_H_
