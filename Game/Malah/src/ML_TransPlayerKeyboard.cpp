/*
 * FILE: ML_TransPlayerKeyboard.cpp
 *
 * DESCRIPTION: Player's keyboard translator for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_TransPlayerKeyboard.h"
#include "ML_MsgDef.h"
#include "ML_ManagerDef.h"
#include "ML_GamePlay.h"
#include "ML_Interface.h"

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
//	Implement CMLTransPlayerKeyboard
//
///////////////////////////////////////////////////////////////////////////

CMLTransPlayerKeyboard::CMLTransPlayerKeyboard() : CMLTransKeyboard()
{
}

/*	Translate keyboard

	Return true for success, otherwise return false
*/
bool CMLTransPlayerKeyboard::TranslateIn(ADevice *pADevice)
{
	//	Common translator first
	CMLTransKeyboard::TranslateIn(pADevice);

	ADeviceKeyboard* pKeyboard = (ADeviceKeyboard*)pADevice;

	MLMSG Msg;
	memset(&Msg, 0, sizeof (Msg));

	Msg.idManager	= MANID_PLAYER;
	Msg.idObject	= ID_LOCALPLAYER;

	//	First fire
	if (LKeyIsFirstPressed(pKeyboard, LKEY_FIRE))
	{
		Msg.dwMsg		= MSG_P_FIRE;
		Msg.dwParam1	= 1;
		POSTMESSAGE(&Msg);
	}
	else if (LKeyIsPressed(pKeyboard, LKEY_FIRE))
	{
		//	Continue fire
		Msg.dwMsg		= MSG_P_FIRE;
		Msg.dwParam1	= 0;
		POSTMESSAGE(&Msg);
	}
	else if (LKeyIsLastPressed(pKeyboard, LKEY_FIRE))
	{
		//	Continue fire
		Msg.dwMsg		= MSG_P_FIRESTOP;
		POSTMESSAGE(&Msg);
	}

	//	Open / Close weapon function
	if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPONFUNC))
	{
		Msg.dwMsg		= MSG_P_OPENFUNC;
		POSTMESSAGE(&Msg);
	}

	//	Chnage current weapon
	int iWeapon = -1;
	if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPON0))
		iWeapon = ID_WEAPON_PISTOL;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPON1))
		iWeapon = ID_WEAPON_RIFLE;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPON2))
		iWeapon = ID_WEAPON_MORTAR;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPON3))
		iWeapon = ID_WEAPON_MACHINE;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_WEAPON4))
		iWeapon = ID_WEAPON_RPG;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_CALLATTACKER))
		iWeapon = ID_WEAPON_ATTACKER;
	else if (LKeyIsFirstPressed(pKeyboard, LKEY_CALLCANNON))
		iWeapon = ID_WEAPON_CANNON;

	if (iWeapon >= 0)
	{
		Msg.dwMsg		= MSG_P_CHANGEWEAPON;
		Msg.dwParam1	= iWeapon;
		POSTMESSAGE(&Msg);
	}

	if (LKeyIsFirstPressed(pKeyboard, LKEY_LASTWEAPON))
	{
		Msg.dwMsg		= MSG_P_CHANGEWEAPON;
		Msg.dwParam1	= (DWORD)(-1);
		POSTMESSAGE(&Msg);
	}

	//	Call helicopter support
	if (LKeyIsFirstPressed(pKeyboard, LKEY_CALLCOPTER))
	{
		//	Continue fire
		Msg.dwMsg		= MSG_P_CALLSUPPORT;
		Msg.dwParam1	= ID_SUPP_COPTER;
		POSTMESSAGE(&Msg);
	}

	//	Popup escape panel
	if (LKeyIsFirstPressed(pKeyboard, LKEY_PAUSE))
	{
		Msg.idManager	= -1;
		Msg.dwMsg		= MSG_INTERFACE;
		Msg.dwParam1	= MLIG_ESCAPE;
		Msg.dwParam2	= CMLGamePlay::GPS_NOTACTIVE;
		POSTMESSAGE(&Msg);
	}

	//	Trigger mission brief
	if (LKeyIsFirstPressed(pKeyboard, LKEY_HELP))
	{
		Msg.idManager	= -1;
		Msg.dwMsg		= MSG_MISSIONBRIEF;
		POSTMESSAGE(&Msg);
	}
	
	return true;
}


