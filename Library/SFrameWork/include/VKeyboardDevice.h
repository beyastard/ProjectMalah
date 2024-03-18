/*
 * FILE: VKeyBoardDevice.h
 *
 * DESCRIPTION: class representing the keyboard and mouse input device of the game;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VKEYBOARDDEVICE_H_
#define _VKEYBOARDDEVICE_H_

#include <Windows.h>
#include "VInputDevice.h"

class VGame;
class VMsgQueue;
class VKeyBoardTranslator;

class VKeyBoardDevice : public VInputDevice
{
private:
	bool						m_bFocus;
	BYTE						m_pKeyState[255];
	BYTE						m_pKeyStateOld[255];
	POINT						m_ptCursorPos;
	POINT						m_ptCursorPosOld;

	VKeyBoardTranslator			* m_pKeyTranslator;

	__int64						m_nCursorSettleTime;	// Time from then on, the cursor has settle down

protected:
public:
	VKeyBoardDevice();
	~VKeyBoardDevice();

	bool Init(HWND hWnd, VGame * pGame, VMsgQueue * pMsgQueue);
	bool Release();
	bool Tick(float vDeltaTime);

	bool ConnectTranslator(VTranslator * pTranslator);

	inline bool IsKeyPressedNow(int nVKey);
	inline bool IsKeyPressedLast(int nVKey);
	inline POINT GetMousePosNow() { return m_ptCursorPos; }
	inline POINT GetMousePosLast() { return m_ptCursorPosOld; }
	inline bool IsFocus() { return m_bFocus; }
};

bool VKeyBoardDevice::IsKeyPressedNow(int nVKey)
{
	if( !IsActive() || nVKey <= 0 || nVKey > 255 )
		return false;

	return m_pKeyState[nVKey] ? true : false;
}

bool VKeyBoardDevice::IsKeyPressedLast(int nVKey)
{
	if( !IsActive() || nVKey <= 0 || nVKey > 255 )
		return false;

	return m_pKeyStateOld[nVKey] ? true : false;
}

#endif//_VKEYBOARDDEVICE_H_