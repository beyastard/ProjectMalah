/*
 * FILE: ML_GameParams.h
 *
 * DESCRIPTION: Handle game logic parameters of game Malah that are
 *				described in text file.
 *
 * CREATED BY: Duyuxin, 2003/1/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_GAMEPARAMS_H_
#define _ML_GAMEPARAMS_H_

#include "AScriptFile.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

struct GAMELOGICPARAMS
{
	//	AI parameters
	float	fAIShotCopterRadius;	//	Radius in which AI attack copter
	float	fAIJumpDist;			//	AI jump distance
	int		iAIShotInterRange;		//	AI shot interval change range
	int		iAIGenInterRange;		//	AI generate interval change range
	int		iAISpeedRange;			//	AI run speed change range
	int		iAICorpseTime;			//	AI's corpse time
	int		iAIFusilladePercent;	//	AI fusillade possibility
	float	fAIRocketSpeed;			//	AI's rocket speed
	float	fAIAttenuationNear;		//	Attenuation near distance
	float	fAIAttenuationFar;		//	Attenuation far distance
	int		iAIThrowGunPercentage;	//	Percentage to throw gun and helmet
	float	fAIShiftMin;			//	Minimum distance of shift moving
	float	fAIShiftMax;			//	Maximum distance of shift moving

	//	Enemy weapon parameters
	float	fMIG21Speed;			//	MIG-21 speed
	float	fMIG21MinHeight;		//	MIG-21 minimum height
	float	fMIG21MaxHeight;		//	MIG-21 maximum height
	int		iMIG21BulletNum;		//	MIG-21 bullet number
	int		iMIG21MinDamage;		//	MIG-21 minimum damage
	int		iMIG21MaxDamage;		//	MIG-21 maximum damage
	int		iMIG21FireInterval;		//	MIG-21 fire interval
	float	fMIG21FireDist;			//	MIG-21 fire distance
	float	fMIG21FireLen;			//	MIG-21 fire length in distance
	int		iMIG21Life;				//	MIG-21 life value

	float	fFantanSpeed;			//	Fantan speed
	float	fFantanMinHeight;		//	Fantan minimum height
	float	fFantanMaxHeight;		//	Fantan maximum height
	float	fFantanRocketSpeed;		//	Fantan rocket speed
	int		iFantanRocketNum;		//	Fantan rocket number
	int		iFantanPrecise;			//	Fantan fire precise
	int		iFantanDamage;			//	Fantan rocket's damage
	int		iFantanFireInterval;	//	Fantan fire interval
	float	fFantanFireDist;		//	Fantan fire distance
	int		iFantanLife;			//	Fantan life value

	float	fTruckSpeed;			//	Truck speed
	float	fTankSpeed;				//	Tank speed
	float	fArmoredSpeed;			//	Armored car speed
	int		iTruckLife;				//	Truck's life
	int		iTankLife;				//	Tank's life
	int		iArmoredLife;			//	Armored car's life
	int		iTankDamage;			//	Tank's damage
	int		iArmoredDamage;			//	Armored car's damage

	//	Weapon parameters
	int		iPistolInterval;		//	Pistol shot interval
	int		iRifleSingleInterval;	//	Rifle single shot interval
	int		iRifleStrafeInterval;	//	Rifle strafe shot interval
	int		iMortarInterval;		//	Mortar shot interval
	int		iMachineGunInterval;	//	Machine gun shot interval
	int		iRPGInterval;			//	RPG interval
	int		iMachineGunDither;		//	Machine gun dither parameter
	float	fMortarInnerRadius;		//	Inner radius of mortar bomb
	float	fMortarOuterRadius;		//	Outer radius of mortar bomb
	int		iMortarArriveTime;		//	Time of mortar arrive from lanuched
	float	fMortarMinDist;			//	Minimum distance of mortar drop
	float	fRifleSingleDamage;		//	Rifle single damage scale
	float	fRifleMagnifyScale;		//	Rifle snipe lens magnify scale
	float	fRPGRadius;				//	RPG exploding radius

	//	Support paraemters
	int		iCopterArriveTime;		//	Time of copter arrive from called
	float	fCopterFlyHeight;		//	Copter fly height
	float	fCopterSupplyHeight;	//	Copter supply height
	float	fCopterSpeed;			//	Copter fly speed
	float	fCopterDropSpeed;		//	Copter droping speed
	int		iCopterHangTime;		//	Length of copter hang time

	int		iCannonArriveTime;		//	Time of cannom arrive from called
	int		iCannonFireTimes;		//	Cannon fire times
	int		iNumCannonBomb;			//	Number of cannon bomb each time
	float	fCannonFallRadius;		//	Radius of cannon bomb's point of fall
	float	fCannonMinDist;			//	Minimum distance of cannon bomb drop
	float	fCannonInnerRadius;		//	Inner radius of cannon bomb
	float	fCannonOuterRadius;		//	Outer radius of cannon bomb

	int		iAttackerArriveTime;	//	Time of attackcer arrive from called
	float	fAttackerHeight;		//	Attacker fly height
	float	fAttackerSpeed;			//	Attacker fly speed
	float	fFireballInnerRadius;	//	Inner radius of fireball bomb
	float	fFireballOuterRadius;	//	Outer radius of fireball bomb
	int		iFireballNum;			//	Fireball number
	int		iFireballBombLen;		//	Fireball bomb distance length
	float	fFireballMinDist;		//	Minimum distance of fireball drop

	//	Others parameters
	int		iMaxMission;			//	Maximum mission the player can enjoy
	int		iScoreFootman;			//	Score of footman
	int		iScoreGuerilla;			//	Score of guerilla
	int		iScoreRocket;			//	Score of rocket man
	int		iScoreTruck;			//	Score of truck
	int		iScoreTank;				//	Score of tank
	int		iScoreArmored;			//	Score of armored car
	int		iScoreMIG21;			//	Score of MIG21
	int		iScoreFantan;			//	Score of FANTAN
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	CMLGameParams
//
///////////////////////////////////////////////////////////////////////////

class CMLGameParams
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLGameParams();
	virtual ~CMLGameParams() {}

public:		//	Attributes

public:		//	Operations

	bool		Load(const char* szFile);		//	Load parameters from file

	GAMELOGICPARAMS*	GetParams()		{	return &m_Params;	}

protected:	//	Attributes

	GAMELOGICPARAMS		m_Params;		//	Parameters

protected:	//	Operations

	bool		ParseFile(AScriptFile* pFile);				//	Parse parameter file
	bool		ParseAIParameter(AScriptFile* pFile);		//	Parse parameter in 'AI' section
	bool		ParseWeaponParameter(AScriptFile* pFile);	//	Parse parameter in 'weapon' section
	bool		ParseSupplyParameter(AScriptFile* pFile);	//	Parse parameter in 'supply' section
	bool		ParseEWParameter(AScriptFile* pFile);		//	Parse parameter in 'enemyweapon' section
	bool		ParseOthersParameter(AScriptFile* pFile);	//	Parse parameter in 'others' section
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_GAMEPARAMS_H_



