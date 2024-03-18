/*
 * FILE: ML_Global.h
 *
 * DESCRIPTION: Global variables and definitions for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_GLOBAL_H_
#define _ML_GLOBAL_H_

#include "ML_Language.h"
#include "ML_ABase.h"
#include "A3DMathUtility.h"
#include "ML_Utility.h"
#include "ML_Database.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define BASERENDER_WIDTH	800
#define BASERENDER_HEIGHT	600

#define CAMERA_FOV			56.0f

//	Font
#define FONT_FONT1_12		0
#define FONT_FONT1_16		1
#define FONT_FONT2_12		2
#define FONT_FONT2_16		3

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

//	Parameters can be provided using program command line
struct CMDLINEPARAMS
{
	int		iRenderWid;		//	Render width
	int		iRenderHei;		//	Render height
	int		iFullScreen;	//	1: fullscreen; 0: windowed
	int		iStartMission;	//	Mission from which game start
};

extern CMDLINEPARAMS	g_CmdParams;

//	Ray trace object type
enum
{
	MLTRACE_NONE = 0,	//	None
	MLTRACE_ESPMODEL,	//	ESP model
	MLTRACE_AI,			//	AI
	MLTRACE_COPTER,		//	Helicopter
	MLTRACE_PLAYER,		//	Player
	MLTRACE_FIXEDOBJ,	//	Fixed object
	MLTRACE_EWPLANE,	//	Enemy plane
	MLTRACE_EWVEHICLE,	//	Enemy vehicle
};

//	Ray trace information
struct MLRAYTRACE
{
	//	In parameters
	A3DVECTOR3	vStart;			//	Ray's start position
	A3DVECTOR3	vDir;			//	Ray's direction (= Normalized vVel)
	A3DVECTOR3	vVel;			//	Ray's velocity
	float		fTime;			//	Ray's moving time

	//	Out values
	int			iObjectType;	//	Trace object type
	void*		pObject;		//	Object's pointer
	DWORD		dwInfo;			//	Extra information
	RAYTRACE	Trace;			//	A3D trace information
};

//	2D sound type
enum
{
	SND2DTYPE_NORMAL = 0,		//	Normal
	SND2DTYPE_ENV,				//	Environment
	NUM_SND2DTYPE,
};

//	3d sound type
enum
{
	SND3DTYPE_NORMAL = 0,		//	Normal
	SND3DTYPE_AIGUNFIRE,		//	AI gun fire
	SND3DTYPE_BULLETHIT,		//	Bullet hit
	NUM_SND3DTYPE,
};

extern char		g_szWorkDir[];
extern char		g_szIniFile[];
extern char		g_szGenSetFile[];

///////////////////////////////////////////////////////////////////////////
//
//	Headers in malah game
//
///////////////////////////////////////////////////////////////////////////

#include "ML_Game.h"

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

extern CMLGame		g_theGame;

//	Current user's game configs
struct USERCONFIGS
{
	int		iRenderWid;			//	Render size
	int		iRenderHei;
	int		iColorBit;			//	Color bit

	int		iTexQuality;		//	Texture quality, 0, 1, 2
	int		iTextureFilter;		//	Texture filter mode, 0, 1
	int		iGFXQuality;		//	GFX quality, 0, 1, 2
	bool	bVideo;				//	Viedeo effect
	bool	bWeather;			//	Weather switch
	bool	bWeaponModel;		//	Load weapon model ?
	bool	bHighSoundQuality;	//	High sound quality
	bool	bHighQualitySky;	//	High quality sky
	bool	bDetailTexture;		//	Detail texture
	bool	bBlood;				//	Blood effect
	bool	bGraphicsMisc;		//	Graphics misc.
	bool	bSoundMisc;			//	Sound misc.

	int		iBrightness;		//	Brightness
	int		iSoundVolume;		//	Sound volume
	int		iMusicVolume;		//	Music volume
	float	fMouseSpeed;		//	Mouse speed. formula: 0.3 + (3.f - 0.3f) * XXXX / 100.f
	int		iMouseInvert;		//	Mouse invert. 1: right, -1: left
};

//	Dynamic game switches ID
enum
{
	SWITCH_UNDEAD = 0,			//	Player undead flag
	SWITCH_LOCKSYSMOUSE,		//	Lock system mouse flag
	SWITCH_UNLIMITEDPISTOL,		//	Unlimited pistol ammo
	SWITCH_UNLIMITEDRIFLE,		//	Unlimited rifle ammo
	SWITCH_UNLIMITEDMORTAR,		//	Unlimited mortar
	SWITCH_UNLIMITEDMACHINE,	//	Unlimited machine gun
	SWITCH_UNLIMITEDRPG,		//	Unlimited RPG
	SWITCH_ADJUSTGUNPOS,		//	Adjust gun position
};

//	Dynamic game switches
struct GAMESWITCHES
{
	bool	bUndead;			//	Player is undead
	bool	bLockSysMouse;		//	Lock system mouse at window's center postion
	bool	bUnlimitedPistol;	//	Unlimited pistol ammo
	bool	bUnlimitedRifle;	//	Unlimited rifle ammo
	bool	bUnlimitedMortar;	//	Unlimited mortar
	bool	bUnlimitedMachine;	//	Unlimited machine gun
	bool	bUnlimitedRPG;		//	Unlimited RPG
	bool	bAdjustGunPos;		//	Adjust gun position
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////

void ML_DefaultUserConfigs(USERCONFIGS* pConfigs);
void ML_DefaultGameSwitches(GAMESWITCHES* pSwitches);
int ML_GetAppropriateFont(int iSize);

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

inline int ML_ConvertTimeSpan(float fTimeSpan)
{
	return (int)(fTimeSpan + 0.5f);
}

#endif	//	_ML_GLOBAL_H_



