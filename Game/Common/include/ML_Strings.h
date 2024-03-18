/*
 * FILE: ML_Strings.h
 *
 * DESCRIPTION: Outside strings used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/19
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_STRINGS_H_
#define _ML_STRINGS_H_

#include "AUString.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Indices of strings
#define	SI_WEAPON_PISTOL		0		//	Pistol
#define SI_WEAPON_RIFLE			1		//	Rifle
#define SI_WEAPON_MORTAR		2		//	Mortar
#define SI_WEAPON_MACHINE		3		//	Machine gun
#define SI_WEAPON_RPG			4		//	RPG

#define	SI_AMMO_PISTOL			20		//	Pistol ammo
#define SI_AMMO_RIFLE			21		//	Rifle ammo
#define SI_AMMO_MORTAR			22		//	Shrapnel ammo
#define SI_AMMO_MACHINE			23		//	Machine gun ammo
#define SI_AMMO_RPG				24		//	RPG

#define	SI_AMMOINC_PISTOL		40		//	Pistol ammo increment
#define SI_AMMOINC_RIFLE		41		//	Rifle ammo increment
#define SI_AMMOINC_MORTAR		42		//	Shrapnel ammo increment
#define SI_AMMOINC_MACHINE		43		//	Machine gun ammo increment
#define SI_AMMOINC_RPG			44		//	RPG ammo increment
#define SI_LIFE_SUPPLY			45		//	Life increment

#define SI_SUPP_CANNON			60		//	Cannon support interval (s)
#define SI_SUPP_HELIPCOPTER		61		//	Supply-helicopter support interval (s)
#define SI_SUPP_ATTACKER		62		//	Attacker plane support interval (s)

#define SI_TIME_AIGEN			75		//	Length of time to generate AI

#define SI_DAMAGE_F				80		//	Damage of footman
#define SI_DAMAGE_G				81		//	Damage of guerilla
#define SI_DAMAGE_R				82		//	Damage of rocket man
#define SI_DAMAGE_STAB			83		//	Stab damage
#define SI_DAMAGE_DISTANCE		84		//	Damage scale factor on distance

#define SI_PRECISE_STAND_F		100		//	Stand shot precise of footman
#define SI_PRECISE_STAND_G		101		//	Stand shot precise of guerilla
#define SI_PRECISE_STAND_R		102		//	Stand shot precise of rocket man
#define SI_PRECISE_MOVE_F		103		//	Move shot precise of footman
#define SI_PRECISE_MOVE_G		104		//	Move shot precise of guerilla
#define SI_PRECISE_CREEP_F		105		//	Creep shot precise of footman
#define SI_PRECISE_CREEP_G		106		//	Creep shot precise of guerilla
#define SI_PRECISE_CREEP_R		107		//	Creep shot precise of rocket man
#define SI_PRECISE_HITCOPTER	108		//	Precise of hit helicopter
#define SI_PRECISE_DISTANCE		109		//	Precise scale factor on distance
#define SI_PER_ATTACKCOPTER		110		//	Percent to attack helicopter

#define SI_FREQ_STAND_F			120		//	Stand shot frequence of footman
#define SI_FREQ_STAND_G			121		//	Stand shot frequence of guerilla
#define SI_FREQ_STAND_R			122		//	Stand shot frequence of rocket man
#define SI_FREQ_MOVE_F			123		//	Move shot frequence of footman
#define SI_FREQ_MOVE_G			124		//	Move shot frequence of guerilla
#define SI_FREQ_MOVE_R			125		//	Move shot frequence of rocket man
#define SI_FREQ_CREEP_F			126		//	Creep shot frequence of footman
#define SI_FREQ_CREEP_G			127		//	Creep shot frequence of guerilla
#define SI_FREQ_CREEP_R			128		//	Creep shot frequence of rocket man

#define SI_SPEED_STAND_F		140		//	Stand moving speed of footman
#define SI_SPEED_STAND_G		141		//	Stand moving speed of guerilla
#define SI_SPEED_STAND_R		142		//	Stand moving speed of rocket man

#define	SI_DAMAGE_PISTOL		160		//	Pistol damage
#define SI_DAMAGE_RIFLE			161		//	Rifle damage
#define SI_DAMAGE_MORTAR		162		//	Shrapnel damage
#define SI_DAMAGE_MACHINE		163		//	Machine gun damage
#define SI_DAMAGE_CANNON		164		//	Cannon damage
#define SI_DAMAGE_FIREBOMB		165		//	Fire bomb from attacker damage
#define SI_DAMAGE_RPG			166		//	RPG damage

#define SI_LIFE_PLAYER			180		//	Player's life
#define SI_LIFE_HELICOPTER		181		//	Helicopter's life
#define SI_LIFE_FOOTMAN			182		//	Life of footman
#define SI_LIFE_GUERILLA		183		//	Life of guerilla
#define SI_LIFE_ROCKET			184		//	Life of rocket man

#define SI_AI_FOOTMAN			200		//	Footman
#define SI_AI_GUERILLA			201		//	Guerilla
#define SI_AI_ROCKETMAN			202		//	Rocket man

#define SI_OBJ_HELICOPTER		210		//	Helicopter

#define SI_AIGEN_INTERVAL		230		//	Generate interval
#define SI_AIGEN_NUMBER			231		//	Generate number every turn
#define SI_AIGEN_TURN			232		//	Generate turn number
#define SI_AIGEN_TIME			233		//	First generate time

#define SI_MIG21_NUMBER			250		//	Number of MIG-21
#define SI_MIG21_INTERVAL		251		//	Appear interval of MIG-21
#define SI_MIG21_SIMUNUM		252		//	Number of MIG-21 appearing simultaneously
#define SI_FANTAN_NUMBER		253		//	Number of Fantan
#define SI_FANTAN_INTERVAL		254		//	Appear interval of Fantan
#define SI_FANTAN_SIMUNUM		255		//	Number of Fantan appearing simultaneously
#define SI_FANTAN_ROCKETNUM		256		//	Number of Fantan's rocket bomb

#define SI_EPE_NONE				400		//	None event

//	AI events on event point
#define SI_EPE_AI_STANDSHOT		410		//	Stand shot of footman
#define SI_EPE_AI_CREEPSHOT		411		//	Creep shot of footman
#define SI_EPE_AI_SHIFTSHOT		412		//	Shift shot of footman
#define SI_EPE_AI_USEMOXMODEL	413		//	Use mox model
#define SI_EPE_AI_JUMPFORWARD	414		//	Jump forward
#define SI_EPE_AI_STAB			415		//	Stab
#define SI_EPE_AI_STAND			416		//	Stand
#define SI_EPE_AI_CREEP			417		//	Creep
#define SI_EPE_AI_STANDSHOT_R	418		//	Stand shot of rocket man
#define SI_EPE_AI_CREEPSHOT_R	419		//	Creep shot of rocket man
#define SI_EPE_AI_SHIFTSHOT_R	420		//	Shift shot of rocket man

//	Copter events on event point
#define SI_EPE_CT_ENTERSUPPLY	460		//	Enter supply area
#define SI_EPE_CT_SUPPLY		461		//	Supply (for helicopter)
#define SI_EPE_CT_LEAVEBATTLE	462		//	Leave battle

//	Misc events on event point
#define SI_EPE_MISC_BLOCKED		480		//	Blocked area
#define SI_EPE_MISC_JUMPFORWARD	481		//	Jump area

//	Events on AI vehicle routes
#define SI_EPE_AIV_BEGINFIRE	490		//	Begin fire
#define SI_EPE_AIV_LEAVE		491		//	Leave

#define SI_FONT_FONT1			500		//	Font 1
#define SI_FONT_FONT2			501		//	Font 2

#define SI_MISSION_KILLALL		520		//	Kill all
#define SI_MISSION_KILL			521		//	Kill specified number of enemy
#define SI_MISSION_HOLDPOS		522		//	Hold position
#define SI_MISSION_PRESERVE		523		//	Preserve a area
#define SI_MISSION_STOPTRUCK	524		//	Stop truck troop

#define ST_GAMETITLE			550		//	Game window's title

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
//	CMLStrings
//
///////////////////////////////////////////////////////////////////////////

class CMLStrings
{
public:		//	Types

	enum
	{
		MAXNUM_STRINGS	= 1024
	};

public:		//	Constructor and Destructor

	CMLStrings();
	virtual ~CMLStrings();

public:		//	Attributes

public:		//	Operations

	bool		LoadStrings(const char* szFile);	//	Load strings

	const AUString&	GetString(int iIndex)
	{
		assert(iIndex >= 0 && iIndex < MAXNUM_STRINGS);
		return *m_aStrings[iIndex];
	}

	void GetString(int iIndex, char* szBuf)
	{
		assert(iIndex >= 0 && iIndex < MAXNUM_STRINGS);
		assert(m_aStrings[iIndex]);
		strcpy(szBuf, (const char*)(*m_aStrings[iIndex]));
	}

protected:	//	Attributes

	AUString*	m_aStrings[MAXNUM_STRINGS];		//	String array

protected:	//	Operations

};

extern CMLStrings	g_MLStrings;

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_STRINGS_H_



