/*
 * FILE: ML_ID.h
 *
 * DESCRIPTION: IDs used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _ML_ID_H_
#define _ML_ID_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Player ID
#define ID_LOCALPLAYER		100
#define NUM_PLAYER_ID		1

#define ID_FIRSTPLAYER		ID_PLAYER
#define ISPLAYERID(i)		((i) >= ID_FIRSTPLAYER && (i) < ID_FIRSTPLAYER + NUM_PLAYER_ID)

//	Weapon ID
#define	ID_WEAPON_PISTOL	120		//	Pistol
#define ID_WEAPON_RIFLE		121		//	Rifle
#define ID_WEAPON_MORTAR	122		//	Mortar
#define ID_WEAPON_MACHINE	123		//	Machine gun
#define ID_WEAPON_RPG		124		//	RPG
#define ID_WEAPON_ATTACKER	125		//	Attacker caller
#define ID_WEAPON_CANNON	126		//	Cannon caller
#define NUM_WEAPON_ID		7

#define ID_FIRSTWEAPON		ID_WEAPON_PISTOL
#define ISWEAPONID(i)		((i) >= ID_FIRSTWEAPON && (i) < ID_FIRSTWEAPON + NUM_WEAPON_ID)

//	Sky object ID
#define ID_SKYOBJECT		140

//	Environment object ID
#define ID_ENVOBJECT		141

//	Level data object ID
#define ID_LEVELDATA		142

//	Bullet ID
#define ID_BULLET_PISTOL	150		//	Pistol bullet
#define ID_BULLET_RIFLE		151		//	Rifle bullet
#define ID_BULLET_MORTAR	152		//	Mortar bullet
#define ID_BULLET_MACHINE	153		//	Machine gun bullet
#define ID_BULLET_FIREBALL	154		//	Fire ball
#define ID_BULLET_CANNON	155		//	Cannon
#define ID_BULLET_ROCKET	156		//	AI's rocket
#define ID_BULLET_MIG21		157		//	MIG-21's bullet
#define ID_BULLET_FANTAN	158		//	Fantan's rocket
#define ID_BULLET_ARMORED	159		//	Armored car's rocket
#define ID_BULLET_RPG		160		//	RPG bullet
#define NUM_BULLET_ID		11

#define ID_FIRSTBULLET		ID_BULLET_PISTOL
#define ISBULLETID(i)		((i) >= ID_FIRSTBULLET && (i) < ID_FIRSTBULLET + ID_BULLET_PISTOL)

//	AI ID
#define ID_AI_FOOTMAN		170		//	Footman using rifle
#define ID_AI_GUERILLA		171		//	Guerilla
#define ID_AI_ROCKET		172		//	Rocket man
#define NUM_AI_ID			3

#define ID_FIRSTAI			ID_AI_FOOTMAN
#define ISAIID(i)			((i) >= ID_FIRSTAI && (i) < ID_FIRSTAI + NUM_AI_ID)

//	Enemy weapons
#define ID_EW_MIG21			180		//	MIG-21
#define ID_EW_FANTAN		181		//	Fantan
#define ID_EW_TRUCK			182		//	Truck
#define ID_EW_TANK			183		//	Tank
#define ID_EW_ARMORED		184		//	Armored car
#define NUM_EW_ID			5

#define ID_FIRSTEW			ID_EW_MIG21
#define ISEWID(i)			((i) >= ID_FIRSTEW && (i) < ID_FIRSTEW + NUM_EW_ID)

//	Support unit ID
#define ID_SUPP_CANNON		200		//	Cannon
#define ID_SUPP_COPTER		201		//	Helicopter
#define ID_SUPP_ATTACKER	202		//	Attacker
#define NUM_SUPPORT_ID		3

#define ID_FIRSTSUPPORT		ID_SUPP_CANNON
#define ISSUPPORTID(i)		((i) >= ID_FIRSTSUPPORT && (i) < ID_FIRSTSUPPORT + NUM_SUPPORT_ID)

#define ID_ROUTE			210
#define ID_EVENTPOINT		211

//	Special route user's ID
#define ID_ROUTEUSER_MARK	-1
#define ID_ROUTEUSER_MISC	-2

//	Bullet shell
#define ID_SHELL_PISTOL		220		//	Pistol bullet shell
#define ID_SHELL_RIFLE		221		//	Rifle bullet shell
#define ID_SHELL_MACHINE	222		//	Machine gun bullet shell
#define ID_SHELL_CHAIN		223		//	Machine gun chain shell
#define NUM_SHELL_ID		4

#define ID_FIRSTSHELL		ID_SHELL_PISTOL
#define ISSHELLID(i)		((i) >= ID_FIRSTSHELL && (i) < ID_FIRSTSHELL + NUM_SHELL_ID)

//	Fixed object
#define ID_FIXEDOBJECT		240

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



#endif	//	_ML_ID_H_


