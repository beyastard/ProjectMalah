/*
 * FILE: ML_TransKeyboard.cpp
 *
 * DESCRIPTION: Common keyboard translater for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Interface.h"
#include "ML_TransKeyboard.h"
#include "ML_MsgDef.h"
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
//	Implement CMLTransKeyboard
//
///////////////////////////////////////////////////////////////////////////

CMLTransKeyboard::CMLTransKeyboard() : ATrans()
{
	//	Set default key maps
	SetDefaultKeyMaps();
}

//	Set default logic key maps
void CMLTransKeyboard::SetDefaultKeyMaps()
{
	memset(m_aKeyMaps, 0, sizeof (m_aKeyMaps));

	m_aKeyMaps[LKEY_FIRE].aKeys[0]			= VK_LBUTTON;
	m_aKeyMaps[LKEY_CONSOLE].aKeys[0]		= VK_CONSOLE;
	m_aKeyMaps[LKEY_WEAPONFUNC].aKeys[0]	= VK_RBUTTON;
	m_aKeyMaps[LKEY_WEAPON0].aKeys[0]		= '1';
	m_aKeyMaps[LKEY_WEAPON1].aKeys[0]		= '2';
	m_aKeyMaps[LKEY_WEAPON2].aKeys[0]		= '3';
	m_aKeyMaps[LKEY_WEAPON3].aKeys[0]		= '4';
	m_aKeyMaps[LKEY_WEAPON4].aKeys[0]		= '5';
	m_aKeyMaps[LKEY_CALLCANNON].aKeys[0]	= '6';
	m_aKeyMaps[LKEY_CALLCOPTER].aKeys[0]	= '7';
	m_aKeyMaps[LKEY_CALLATTACKER].aKeys[0]	= 'S';
	m_aKeyMaps[LKEY_PAUSE].aKeys[0]			= 'P';
	m_aKeyMaps[LKEY_PAUSE].aKeys[1]			= VK_ESCAPE;
	m_aKeyMaps[LKEY_LASTWEAPON].aKeys[0]	= 'Q';
	m_aKeyMaps[LKEY_HELP].aKeys[0]			= 'H';
}

/*	Translate keyboard

	Return true for success, otherwise return false
*/
bool CMLTransKeyboard::TranslateIn(ADevice *pADevice)
{
	ADeviceKeyboard* pKeyboard = (ADeviceKeyboard*)pADevice;

	MLMSG Msg;
	memset(&Msg, 0, sizeof (Msg));

	if (LKeyIsFirstPressed(pKeyboard, LKEY_CONSOLE))
	{
		Msg.idManager	= -1;
		Msg.dwMsg		= MSG_INTERFACE;
		Msg.dwParam1	= MLIG_CONSOLE;
		Msg.dwParam2	= CMLGamePlay::GPS_POPCONSOLE;

		POSTMESSAGE(&Msg);
	}

	return true;
}

bool CMLTransKeyboard::SetKeySetting(int iCommand, int iValue, int iPos)
{
	if (iCommand < 0 || iCommand >= NUM_LKEY || iPos < 0 || iPos >= 2)
		return false;

	m_aKeyMaps[iCommand].aKeys[iPos] = iValue;
	return true;
}

