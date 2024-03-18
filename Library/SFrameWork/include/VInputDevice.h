/*
 * FILE: VInputDevice.h
 *
 * DESCRIPTION: class representing the input device of the game;
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VINPUTDEVICE_H_
#define _VINPUTDEVICE_H_

#include <Windows.h>

class VGame;
class VMsgQueue;
class VTranslator;

class VInputDevice
{
private:
	bool				m_bActive;

protected:
	HWND				m_hWnd;
	VGame				*m_pGame;
	VMsgQueue			*m_pMsgQueue;

public:
	VInputDevice();
	~VInputDevice();

	virtual bool Init(HWND hWnd, VGame * pGame, VMsgQueue * pMsgQueue);
	virtual bool Release();
	virtual bool Tick(float vDeltaTime);

	virtual bool ConnectTranslator(VTranslator * pTranslator);

	inline void SetActive(bool bActive) { m_bActive = bActive; }
	inline bool IsActive() { return m_bActive; }
};

#endif//_VINPUTDEVICE_H_