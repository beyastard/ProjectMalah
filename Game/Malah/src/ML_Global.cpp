/*
 * FILE: ML_Global.cpp
 *
 * DESCRIPTION: Global variables and functions for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"

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

char	g_szWorkDir[MAX_PATH];
char	g_szIniFile[MAX_PATH];
char	g_szGenSetFile[MAX_PATH];

char	g_szDefUserName[] = "Player";

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement
//
///////////////////////////////////////////////////////////////////////////

//	Set default user configs
void ML_DefaultUserConfigs(USERCONFIGS* pConfigs)
{
	memset(pConfigs, 0, sizeof (USERCONFIGS));

	pConfigs->iRenderWid		= g_CmdParams.iRenderWid;
	pConfigs->iRenderHei		= g_CmdParams.iRenderHei;
	pConfigs->iColorBit			= 16;

	pConfigs->iTexQuality		= 2;
	pConfigs->iTextureFilter	= 0;
	pConfigs->iGFXQuality		= 2;
	pConfigs->bVideo			= true;
	pConfigs->bWeather			= true;
	pConfigs->bWeaponModel		= true;
	pConfigs->bHighSoundQuality	= true;
	pConfigs->bHighQualitySky	= true;
	pConfigs->bDetailTexture	= true;
	pConfigs->bBlood			= true;
	pConfigs->bGraphicsMisc		= true;
	pConfigs->bSoundMisc		= true;

	pConfigs->iBrightness		= 50;
	pConfigs->iSoundVolume		= 50;
	pConfigs->iMusicVolume		= 50;
	pConfigs->fMouseSpeed		= 1.0f;
	pConfigs->iMouseInvert		= 1;
}

//	Set default value for game switches
void ML_DefaultGameSwitches(GAMESWITCHES* pSwitches)
{
	memset(pSwitches, 0, sizeof (GAMESWITCHES));

	pSwitches->bUndead				= false;
	pSwitches->bLockSysMouse		= true;
	pSwitches->bUnlimitedPistol		= false;
	pSwitches->bUnlimitedRifle		= false;
	pSwitches->bUnlimitedMortar		= false;
	pSwitches->bUnlimitedMachine	= false;
	pSwitches->bAdjustGunPos		= false;
}

//	Get appropriate font according to language
int ML_GetAppropriateFont(int iSize)
{
	int iFont = FONT_FONT1_12;

#ifdef ML_CHINESE_VERSION

	switch (iSize)
	{
	case 12:	iFont = FONT_FONT1_12;	break;
	case 16:	iFont = FONT_FONT1_16;	break;
	}

#else	//	English version

	switch (iSize)
	{
	case 12:	iFont = FONT_FONT2_12;	break;
	case 16:	iFont = FONT_FONT2_16;	break;
	}

#endif

	return iFont;
}


