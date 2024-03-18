/*
 * FILE: ML_TransPlayerMouse.cpp
 *
 * DESCRIPTION: Player mouse translator for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_TransPlayerMouse.h"
#include "ML_MsgDef.h"
#include "ML_ManagerDef.h"
#include "ML_GamePlay.h"

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
//	Implement CMLTransPlayerMouse
//
///////////////////////////////////////////////////////////////////////////

CMLTransPlayerMouse::CMLTransPlayerMouse() : ATrans()
{
}

bool CMLTransPlayerMouse::TranslateIn(ADevice *pADevice)
{
	ADeviceMouse* p = (ADeviceMouse*)pADevice;
	int dx, dy;

	//	Need to center window's mouse ?
	if (GETGAMESWITCH(SWITCH_LOCKSYSMOUSE))
		CenterWindowMouse();

	p->GetCursorDeltaPos(&dx, &dy);

	MLMSG Msg;
	memset(&Msg, 0, sizeof (Msg));

	int iMouseVert		= GETCURUSERCONFIGS->iMouseInvert;
	float fMouseSpeed	= GETCURUSERCONFIGS->fMouseSpeed;

	if (dx)
	{
		float fDeg = dx * fMouseSpeed * 0.1f;

		Msg.idObject	= ID_LOCALPLAYER;
		Msg.dwMsg		= MSG_P_YAW;
		Msg.dwParam1	= FLOATTOFIX16(fDeg);
		Msg.idManager	= MANID_PLAYER;

		POSTMESSAGE(&Msg);
	}

	if (dy)
	{
		float fDeg = -dy * fMouseSpeed * iMouseVert * 0.1f;

		Msg.idObject	= ID_LOCALPLAYER;
		Msg.dwMsg		= MSG_P_PITCH;
		Msg.dwParam1	= FLOATTOFIX16(fDeg);
		Msg.idManager	= MANID_PLAYER;

		POSTMESSAGE(&Msg);
	}

	return true;
}

//	Center window's mouse
void CMLTransPlayerMouse::CenterWindowMouse()
{
	HWND hWnd = g_theGame.GetHWND();
	if (!hWnd)
		return;

	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	POINT pos = {rcClient.right >> 1, rcClient.bottom >> 1};
	ClientToScreen(hWnd, &pos);

	SetCursorPos(pos.x, pos.y);
}



