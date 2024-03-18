/*
 * FILE: VGame.h
 *
 * DESCRIPTION: virtual game object
 *
 * CREATED BY: Hedi, 2002/7/29
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VGAME_H_
#define _VGAME_H_

#include <AC.h>
#include <AF.h>
#include <A3D.h>
#include <AM.h>

#include "VGameCfg.h"
#include "VGameMsg.h"
#include "VMsgQueue.h"
#include "VInputDevice.h"
#include "VKeyBinder.h"
#include "tchar.h"
class VWorld;

#define GAME_MAX_INPUT_DEVICE		16

class VGame
{
private:
	DWORD					m_dwTicks;			// tick number of game object
	bool					m_bEmbedded;		// flag indicates whether the 
												// device window is not the main window of the app
	bool					m_bActive;
	bool					m_bPacked;
	__int64					m_nLastTickTime;
	ALog					*m_pErrLog;			// object to do error logging;

protected:
	bool					m_bAutoHideCursor;	
	A3DCursor				*m_pCursor;			// cursor object;

	VGameCfg				m_config;			// config object;

	// Engine Objects;
	A3DEngine				*m_pA3DEngine;		// pointer of A3DEngine
	A3DDevice				*m_pA3DDevice;		// pointer of A3DDevice
	A3DCamera				*m_pA3DCamera;		// pointer of A3DCamera;
	A3DViewport				*m_pA3DViewport;	// pointer of A3DViewport;
	A3DLight				*m_pA3DDirLight;	// pointer of the directional light;

	VWorld					*m_pVWorld;			// the world
 
	// Message processing;
	int						m_nInputDeviceNum;							// How many input device is there;
	VInputDevice			*m_pInputDevices[GAME_MAX_INPUT_DEVICE];	// the common input device;
	VMsgQueue				*m_pMsgQueue;								// the queue for messages;

	// Window Objects;
	HINSTANCE				m_hInstance;		// application instance
	TCHAR					m_szClassName[64];	// window class name
	TCHAR					m_szWndTitle[256];	// window title
	HWND					m_hDeviceWnd;		// device window's handle
	int						m_nWidth;			// device window's width
	int						m_nHeight;			// device window's height
	int						m_nDepth;			// device window's color depth
	bool					m_bWindowed;		// window or not?


	// preload all resources into memory
	virtual bool PrecacheResources();

	virtual bool Tick();
	virtual bool Render();
	virtual bool RenderContent(A3DViewport * pCurrentViewport);

	virtual bool InitWindow();
	virtual bool InitEngine();
	virtual bool InitWorld();

	virtual bool ShowSplash();
	virtual bool ShowEnd();

	virtual bool ReleaseWorld();
	virtual bool ReleaseEngine();
	virtual bool ReleaseWindow();
	
	bool WindowMessageTick();
	bool GameMessageProcess();

	bool ResizeDevice(int nWidth, int nHeight);	// Resize the device according to window size

public:
	VGame();
	~VGame();

	void ErrLog(char * szFmt, ...);

	virtual bool InitEmbedded(HINSTANCE hInstance, HWND hWndDevice, bool bPakced=false);
	virtual bool RunEmbedded(); // Run and return after just one tick;

	virtual bool Init(HINSTANCE hInstance, bool bPacked=false);
	virtual bool Release();
	virtual bool Run();
				  
	virtual bool OnMsg(const GAMEMSG& msg, DWORD dwUserData);
	virtual bool OnActivateApp(bool bActive);

	virtual bool SetAmbient(BYTE red, BYTE green, BYTE blue);
	virtual bool SetDirectionalColor(BYTE red, BYTE green, BYTE blue);
	virtual bool SetDirectionalDir(A3DVECTOR3 vecDir);
	virtual bool SetFogColor(BYTE red, BYTE green, BYTE blue);
	virtual bool SetFogStartAndEnd(float vStart, float vEnd);

	// Adjust camera methods;
	virtual bool SetCameraPos(const A3DVECTOR3& vecPos);
	virtual bool SetCameraDirAndUp(const A3DVECTOR3& vecDir, const A3DVECTOR3& vecUp);

	virtual bool SetCursorType(int nType);

	// Engine object query interface
	inline A3DEngine * GetA3DEngine() { return m_pA3DEngine; }
	inline A3DDevice * GetA3DDevice() { return m_pA3DDevice; }
	inline A3DCamera * GetA3DCamera() { return m_pA3DCamera; }
	inline A3DViewport * GetA3DViewport() { return m_pA3DViewport; }
	inline A3DLight * GetA3DDirLight() { return m_pA3DDirLight; }
	
	inline VMsgQueue * GetMsgQueue()  { return m_pMsgQueue; }

	// game's window procedure;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// inline function for get width and height
	inline int GetDeviceWidth() { return m_nWidth; }
	inline int GetDeviceHeight() { return m_nHeight; }
	inline HWND GetDeviceWnd() { return m_hDeviceWnd; }
	
	inline VWorld * GetVWorld() { return m_pVWorld; }
};

// global virtual game object
extern VGame		*g_pGame;

#endif//_VGAME_H_
