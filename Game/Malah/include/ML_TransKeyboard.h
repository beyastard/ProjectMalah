/*
 * FILE: ML_TransKeyboard.h
 *
 * DESCRIPTION: Common keyboard translator for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_TRANSKEYBOARD_H_
#define _ML_TRANSKEYBOARD_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Define virtual key of `
#define	VK_CONSOLE		0xC0

//	Logic keys
enum
{
	LKEY_FIRE = 0,		//	Fire
	LKEY_CONSOLE,		//	Trigger console
	LKEY_WEAPONFUNC,	//	Open / Close weapon function
	LKEY_WEAPON0,		//	Change to weapon 0
	LKEY_WEAPON1,		//	Change to weapon 1
	LKEY_WEAPON2,		//	Change to weapon 2
	LKEY_WEAPON3,		//	Change to weapon 3
	LKEY_WEAPON4,		//	Change to weapon 4
	LKEY_CALLCANNON,	//	Call cannon support
	LKEY_CALLCOPTER,	//	Call helicopter support
	LKEY_CALLATTACKER,	//	Call attacker support
	LKEY_PAUSE,			//	Pause game
	LKEY_LASTWEAPON,	//	Last weapon
	LKEY_HELP,			//	Mission help
	NUM_LKEY,			//	Number of logic key
};

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLTransKeyboard
//
///////////////////////////////////////////////////////////////////////////

class CMLTransKeyboard : public ATrans
{
public:		//	Types

	//	Map between logic key and keyboard
	typedef struct _KEYMAP
	{
		char	szName[64];		//	Logic key's name
		int		aKeys[2];		//	Each logic key can be mapped to 2 key buttons
		
	} KEYMAP, *PKEYMAP;

public:		//	Constructor and Destructor

	CMLTransKeyboard();
	virtual ~CMLTransKeyboard() {}

public:		//	Attributes

public:		//	Operations

	bool	SetKeySetting(int iCommand, int iValue, int iPos);

	virtual bool	TranslateIn(ADevice *pADevice);

protected:	//	Attributes

	KEYMAP		m_aKeyMaps[NUM_LKEY];		//	Key map table

protected:	//	Operations

	inline bool LKeyIsFirstPressed(ADeviceKeyboard* pDevice, int iKey);	//	Key is first pressed ?
	inline bool LKeyIsPressed(ADeviceKeyboard* pDevice, int iKey);		//	Key is pressed ?
	inline bool LKeyIsLastPressed(ADeviceKeyboard* pDevice, int iKey);	//	Key is last pressed ?

	void		SetDefaultKeyMaps();		//	Set default key maps
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

/*	Key is first pressed ?

	Return true if key is pressed, otherwise return false.

	pDevice: keyboard device
	iKey: logic key code
*/
bool CMLTransKeyboard::LKeyIsFirstPressed(ADeviceKeyboard* pDevice, int iKey)
{
	if (pDevice->VK_FirstPress(m_aKeyMaps[iKey].aKeys[0]) || 
		pDevice->VK_FirstPress(m_aKeyMaps[iKey].aKeys[1]))
		return true;

	return false;
}

/*	Key is pressed ?

	Return true if key is pressed, otherwise return false.

	pDevice: keyboard device
	iKey: logic key code
*/
bool CMLTransKeyboard::LKeyIsPressed(ADeviceKeyboard* pDevice, int iKey)
{
	if (pDevice->VK_Pressing(m_aKeyMaps[iKey].aKeys[0]) || 
		pDevice->VK_Pressing(m_aKeyMaps[iKey].aKeys[1]))
		return true;

	return false;
}

/*	Key is last pressed ?

	Return true if key is pressed, otherwise return false.

	pDevice: keyboard device
	iKey: logic key code
*/
bool CMLTransKeyboard::LKeyIsLastPressed(ADeviceKeyboard* pDevice, int iKey)
{
	if (pDevice->VK_LastPress(m_aKeyMaps[iKey].aKeys[0]) || 
		pDevice->VK_LastPress(m_aKeyMaps[iKey].aKeys[1]))
		return true;

	return false;
}


#endif	//	_ML_TRANSKEYBOARD_H_
