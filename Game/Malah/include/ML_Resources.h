/*
 * FILE: ML_Resources.h
 *
 * DESCRIPTION: Resources operations for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_RESOURCES_H_
#define _ML_RESOURCES_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Name IDs used by RES_GetName()
enum
{
	RES_IM_FOOTMAN = 0,			//	Footman image model
	RES_IM_GUERILLA,			//	guerilla image model
	RES_IM_ROCKETMAN,			//	Rocket man image model
	NUM_RESNAME
};

//	File IDs used by RES_GetOtherFile()
enum
{
	RES_IM_AIGROUPFILE = 0,		//	AI Image model group file
	RES_LEVELDESCFILE,			//	Level description file
	RES_PARAMETERFILE,			//	Parameter file
	NUM_RESOTHERFILE,
};

//	File IDs used by RES_GetModelFile()
enum
{
	RES_MOX_PISTOL = 0,			//	Pistol
	RES_MOX_RIFLE,				//	Rifle
	RES_MOX_MORTAR,				//	Mortar
	RES_MOX_MACHINE,			//	Machine gun
	RES_MOX_RPG,				//	RPG
	RES_MOX_FOOTMAN,			//	Footman
	RES_MOX_GUERILLA,			//	Guerilla
	RES_MOX_ROCKETMAN,			//	Rocket man
	RES_MOX_ROCKETBULLET,		//	Rocket bullet
	RES_MOX_ATTACKER,			//	Attacker
	RES_MOX_CANNON,				//	Cannon
	RES_MOX_COPTER,				//	Helicopter normal
	RES_MOX_COPTERCRASH,		//	Helicopter crashing
	RES_MOX_COPTERWRECK,		//	Helicopter wreckage
	RES_MOX_MTLOCALIZER,		//	Mortar localizer
	RES_MOX_ATLOCALIZER,		//	Attacker localizer
	RES_MOX_CNLOCALIZER,		//	Cannon localizer
	RES_MOX_PISTOLSHELL,		//	Pistol bullet shell
	RES_MOX_RIFLESHELL,			//	Rifle bullet shell
	RES_MOX_MACHINESHELL,		//	Machine gun bullet shell
	RES_MOX_MACHINECHAIN,		//	Machine gun chain shell
	RES_MOX_MIG21,				//	MIG-21 plane
	RES_MOX_FANTAN,				//	FANTAN plane
	RES_MOX_TRUCK,				//	Truck
	RES_MOX_TANK,				//	Tank
	RES_MOX_ARMORED,			//	Armored car
	RES_MOX_TRUCKWRECKAGE,		//	Truck wreckage
	RES_MOX_TANKWRECKAGE,		//	Tank wreckage
	RES_MOX_ARMOREDWRECKAGE,	//	Armored car wreckage
	RES_MOX_HIGHPISTOL,			//	High precise pistol
	RES_MOX_HIGHRIFLE,			//	High precise rifle
	RES_MOX_HIGHMORTAR,			//	High precise mortar
	RES_MOX_HIGHMACHINE,		//	High precise machine gun
	RES_MOX_HIGHRPG,			//	High precise RPG
	NUM_RESMOXFILE
};

//	File IDs used by RES_GetGFXFile()
enum
{
	RES_GFX_HITBLOOD = 0,		//	Hit blood
	RES_GFX_GUNFIRELIGHT,		//	Gun firelight
	RES_GFX_ROCKETLAUNCH,		//	Rocket launch
	RES_GFX_ROCKETFLY0,			//	Low quality rocket fly
	RES_GFX_ROCKETFLY1,			//	Medium quality rocket fly
	RES_GFX_ROCKETFLY2,			//	High quality rocket fly
	RES_GFX_ROCKETHIT,			//	Rocket hit
	RES_GFX_FIREBALL0,			//	Fireball explosion effect 0
	RES_GFX_FIREBALL1,			//	Fireball explosion effect 1
	RES_GFX_FIREBALL2,			//	Fireball explosion effect 2
	RES_GFX_FIREREMAINS,		//	Fire remains
	RES_GFX_BODYFIRE,			//	Fire on body
	RES_GFX_CANNONEXPLODE,		//	Cannon bomb explode
	RES_GFX_MORTEREXPLODE,		//	Morter bomb explode
	RES_GFX_COPTEREXPLODE,		//	Helicopter explode
	RES_GFX_COPTERSHIVER,		//	Helicopter's shiver
	RES_GFX_COPTERFIRE,			//	Helicopter fire
	RES_GFX_STABHURT0,			//	Stab hurt player 0
	RES_GFX_STABHURT1,			//	Stab hurt player 1
	RES_GFX_ROCKETHURT,			//	Rocket hurt player
	RES_GFX_GUNHITGROUD,		//	Gun hit groud
	RES_GFX_THROWNHELMET,		//	Thrown helmet
	RES_GFX_THROWNGUN,			//	Thrown gun
	RES_GFX_THROWBAZOOKA,		//	Thrown bazooka
	RES_GFX_THROWNBBHAT,		//	Thrown bamboo hat
	RES_GFX_BLOWNHELMET,		//	Blown helmet
	RES_GFX_BLOWNGUN,			//	Blown gun
	RES_GFX_BLOWNBAZOOKA,		//	Blown bazooka
	RES_GFX_BLOWNNBBHAT,		//	Blown bamboo hat
	RES_GFX_EXPLODEMUD,			//	Bomb explode on muddy ground
	RES_GFX_EXPLODEMUDHOLD,		//	Bomb explode on muddy ground and left hole
	RES_GFX_BOMBHOLE0,			//	Small bomb hole after explosion
	RES_GFX_BOMBHOLE1,			//	Big bomb hole after explosion
	RES_GFX_FANTANEXPLODE,		//	Fantan explode
	RES_GFX_MIG21EXPLODE,		//	MIG-21 explode
	RES_GFX_TANKGUNTHROW,		//	Tank gun thrown
	RES_GFX_ARMOREDGUNTHROW,	//	Armored car gun thrown
	RES_GFX_TANKFIRE,			//	Tank fire
	RES_GFX_TANKSMOKE,			//	Tank trail smoke
	RES_GFX_TANKEXPLODE,		//	Tank explode
	RES_GFX_SPARK,				//	Spark
	NUM_RESGFXFILE
};

//	File IDs used by RES_GetSoundFile()
enum
{
	//	2D sounds ...
	RES_S2D_PISTOLFIRE = 0,		//	Pistol fire
	RES_S2D_RIFLEFIRE,			//	Rifle fire
	RES_S2D_MORTARFIRE,			//	Mortar fire
	RES_S2D_MACHINEFIRE,		//	Machine gun fire
	RES_S2D_MACHINERUN,			//	Machine gun run
	RES_S2D_RPGFIRE,			//	RPG fire
	RES_S2D_CANNONFIRE,			//	Cannon fire
	RES_S2D_BULLETWHIZZ1,		//	Bullet whizz 1
	RES_S2D_BULLETWHIZZ2,		//	Bullet whizz 2
	RES_S2D_BULLETWHIZZ3,		//	Bullet whizz 3
	RES_S2D_BULLETWHIZZ4,		//	Bullet whizz 4
	RES_S2D_BULLETWHIZZ5,		//	Bullet whizz 5
	RES_S2D_BULLETWHIZZ6,		//	Bullet whizz 6
	RES_S2D_BULLETWHIZZ7,		//	Bullet whizz 7
	RES_S2D_BULLETWHIZZ8,		//	Bullet whizz 8
	RES_S2D_BULLETWHIZZ9,		//	Bullet whizz 9
	RES_S2D_BULLETWHIZZ10,		//	Bullet whizz 10
	RES_S2D_BULLETWHIZZ11,		//	Bullet whizz 11
	RES_S2D_BULLETWHIZZ12,		//	Bullet whizz 12
	RES_S2D_BULLETDROP1,		//	Bullet drop 1
	RES_S2D_BULLETDROP2,		//	Bullet drop 2
	RES_S2D_BULLETDROP3,		//	Bullet drop 3
	RES_S2D_BULLETDROP4,		//	Bullet drop 4
	RES_S2D_BULLETDROP5,		//	Bullet drop 5
	RES_S2D_BULLETDROP6,		//	Bullet drop 6
	RES_S2D_BULLETDROP7,		//	Bullet drop 7
	RES_S2D_BULLETDROP8,		//	Bullet drop 8
	RES_S2D_BULLETDROP9,		//	Bullet drop 9
	RES_S2D_BULLETDROP10,		//	Bullet drop 10
	RES_S2D_BULLETDROP11,		//	Bullet drop 11
	RES_S2D_BULLETDROP12,		//	Bullet drop 12
	RES_S2D_RAIN1,				//	Rain 1
	RES_S2D_RAIN2,				//	Rain 2
	RES_S2D_RAIN3,				//	Rain 3
	RES_S2D_THUNDER1,			//	Thunder 1
	RES_S2D_THUNDER2,			//	Thunder 2
	RES_S2D_THUNDER3,			//	Thunder 3
	RES_S2D_THUNDER4,			//	Thunder 4
	RES_S2D_THUNDER5,			//	Thunder 5
	RES_S2D_THUNDER6,			//	Thunder 6
	RES_S2D_SUNNY1,				//	Sunny day 1
	RES_S2D_SUNNY2,				//	Sunny day 2
	RES_S2D_SUNNY3,				//	Sunny day 3
	RES_S2D_NIGHT1,				//	Sunny night 1
	RES_S2D_NOAMMO,				//	No ammo
	RES_S2D_PUSHBUTTON,			//	Push button
	RES_S2D_SUPPLYFINISH,		//	Supply finish

	//	Voice sounds ...
	RES_S2D_CANNONCALLING,		//	Cannon calling
	RES_S2D_CANNONDENY,			//	Cannon deny
	RES_S2D_CANNONAGREE,		//	Cannon agree
	RES_S2D_COPTERCALLING,		//	Copter calling
	RES_S2D_COPTERDENY,			//	Copter deny
	RES_S2D_COPTERAGREE,		//	Copter agree
	RES_S2D_COPTERDOWN,			//	Copter is hit down
	RES_S2D_ATTACKERCALLING,	//	Attacker calling
	RES_S2D_ATTACKERDENY,		//	Attacker deny
	RES_S2D_ATTACKERAGREE,		//	Attacker agree
	RES_S2D_MISSIONCOMPLETED1,	//	Mission completed 1
	RES_S2D_MISSIONCOMPLETED2,	//	Mission completed 2
	RES_S2D_MISSIONCOMPLETED3,	//	Mission completed 3
	RES_S2D_MISSIONFAILED1,		//	Mission failed 1
	RES_S2D_MISSIONFAILED2,		//	Mission failed 2
	RES_S2D_MISSIONFAILED3,		//	Mission failed 3
	RES_S2D_HITENEMYPLANE1,		//	Hit enemy plane 1
	RES_S2D_HITENEMYPLANE2,		//	Hit enemy plane 2
	RES_S2D_HITENEMYPLANE3,		//	Hit enemy plane 3
	RES_S2D_HITENEMYVEHICLE1,	//	Hit enemy vehicle 1
	RES_S2D_HITENEMYVEHICLE2,	//	Hit enemy vehicle 2
	RES_S2D_HITENEMYVEHICLE3,	//	Hit enemy vehicle 3

	//	3D sounds ...
	RES_S3D_MORTARWHIZZ,		//	Mortar whizz
	RES_S3D_MORTARHIT,			//	Mortar hit
	RES_S3D_FIREBOMBEXPLODE,	//	Firebomb expolde
	RES_S3D_FIREBOMBFIRE,		//	Firebomb fire
	RES_S3D_FIREBOMBEXTINCT,	//	Firebomb fire extinguish
	RES_S3D_ROCKETFIRE,			//	Rocket fire
	RES_S3D_ROCKETHIT,			//	Rocket hit
	RES_S3D_ROCKETFLY,			//	Rocket fly
	RES_S3D_CANNONHIT1,			//	Cannon hit 1
	RES_S3D_CANNONHIT2,			//	Cannon hit 2
	RES_S3D_CANNONHIT3,			//	Cannon hit 3
	RES_S3D_CANNONHIT4,			//	Cannon hit 4
	RES_S3D_AK47FIRE1,			//	AK-47 single shot
	RES_S3D_AK47FIRE2,			//	AK-47 long shot
	RES_S3D_AK47FIRE3,			//	AK-47 straf
	RES_S3D_COPTERFLY,			//	Copter fly
	RES_S3D_COPTERFIRE,			//	Copter fire
	RES_S3D_COPTEREXPLODE1,		//	Copter explode 1
	RES_S3D_COPTEREXPLODE2,		//	Copter explode 2
	RES_S3D_COPTEREXPLODE3,		//	Copter explode 3
	RES_S3D_COPTERCRASH1,		//	Copter crash 1
	RES_S3D_COPTERCRASH2,		//	Copter crash 2
	RES_S3D_COPTERCRASH3,		//	Copter crash 3
	RES_S3D_COPTERCRASH4,		//	Copter crash 4
	RES_S3D_COPTERCRASH5,		//	Copter crash 5
	RES_S3D_COPTERHURT1,		//	Copter hurt 1
	RES_S3D_COPTERHURT2,		//	Copter hurt 2
	RES_S3D_COPTERHURT3,		//	Copter hurt 3
	RES_S3D_COPTERFLYDOWN,		//	Copter fly down
	RES_S3D_AIRUSH1,			//	AI rush 1
	RES_S3D_AIRUSH2,			//	AI rush 2
	RES_S3D_AIRUSH3,			//	AI rush 3
	RES_S3D_AIRUSH4,			//	AI rush 4
	RES_S3D_AIRUSH5,			//	AI rush 5
	RES_S3D_AIRUSH6,			//	AI rush 6
	RES_S3D_AIRUSH7,			//	AI rush 7
	RES_S3D_AIRUSH8,			//	AI rush 8
	RES_S3D_AIRUSH9,			//	AI rush 9
	RES_S3D_AIRUSH10,			//	AI rush 10
	RES_S3D_AIRUSH11,			//	AI rush 11
	RES_S3D_AIRUSH12,			//	AI rush 12
	RES_S3D_AIBURNDEATH1,		//	AI burn death 1
	RES_S3D_AIBURNDEATH2,		//	AI burn death 2
	RES_S3D_AIBURNDEATH3,		//	AI burn death 3
	RES_S3D_AIBURNDEATH4,		//	AI burn death 4
	RES_S3D_AIBURNDEATH5,		//	AI burn death 5
	RES_S3D_AIBURNDEATH6,		//	AI burn death 6
	RES_S3D_AIBURNDEATH7,		//	AI burn death 7
	RES_S3D_AIDEATH1,			//	AI death 1
	RES_S3D_AIDEATH2,			//	AI death 2
	RES_S3D_AIDEATH3,			//	AI death 3
	RES_S3D_AIDEATH4,			//	AI death 4
	RES_S3D_AIDEATH5,			//	AI death 5
	RES_S3D_AIDEATH6,			//	AI death 6
	RES_S3D_AIBLASTDEATH1,		//	AI blast death 1
	RES_S3D_AIBLASTDEATH2,		//	AI blast death 2
	RES_S3D_AIBLASTDEATH3,		//	AI blast death 3
	RES_S3D_AIBLASTDEATH4,		//	AI blast death 4
	RES_S3D_AIBLASTDEATH5,		//	AI blast death 5
	RES_S3D_AISTABSHOUT1,		//	AI stab shout 1	
	RES_S3D_AISTABSHOUT2,		//	AI stab shout 2	
	RES_S3D_AISTABSHOUT3,		//	AI stab shout 3	
	RES_S3D_AISTABSHOUT4,		//	AI stab shout 4	
	RES_S3D_AISTABSHOUT5,		//	AI stab shout 5	
	RES_S3D_BULLETHITMUD1,		//	Bullet hit mud 1
	RES_S3D_BULLETHITMUD2,		//	Bullet hit mud 2
	RES_S3D_BULLETHITMUD3,		//	Bullet hit mud 3
	RES_S3D_BULLETHITMUD4,		//	Bullet hit mud 4
	RES_S3D_BULLETHITMUD5,		//	Bullet hit mud 5
	RES_S3D_BULLETHITMUD6,		//	Bullet hit mud 6
	RES_S3D_BULLETHITMUD7,		//	Bullet hit mud 7
	RES_S3D_BULLETHITMUD8,		//	Bullet hit mud 8
	RES_S3D_BULLETHITMUD9,		//	Bullet hit mud 9
	RES_S3D_BULLETHITMUD10,		//	Bullet hit mud 10
	RES_S3D_BULLETHITMUD11,		//	Bullet hit mud 11
	RES_S3D_BULLETHITMUD12,		//	Bullet hit mud 12
	RES_S3D_BULLETHITMUD13,		//	Bullet hit mud 13
	RES_S3D_BULLETHITMUD14,		//	Bullet hit mud 14
	RES_S3D_BULLETHITMUD15,		//	Bullet hit mud 15
	RES_S3D_BULLETHITMUD16,		//	Bullet hit mud 16
	RES_S3D_BULLETHITMUD17,		//	Bullet hit mud 17
	RES_S3D_BULLETHITMUD18,		//	Bullet hit mud 18
	RES_S3D_BULLETHITMUD19,		//	Bullet hit mud 19
	RES_S3D_BULLETHITMUD20,		//	Bullet hit mud 20
	RES_S3D_BULLETHITMUD21,		//	Bullet hit mud 21
	RES_S3D_BULLETHITBODY1,		//	Bullet hit body 1
	RES_S3D_BULLETHITBODY2,		//	Bullet hit body 2
	RES_S3D_BULLETHITBODY3,		//	Bullet hit body 3
	RES_S3D_BULLETHITBODY4,		//	Bullet hit body 4
	RES_S3D_BULLETHITBODY5,		//	Bullet hit body 5
	RES_S3D_BULLETHITPLANE1,	//	Bullet hit plane 1
	RES_S3D_BULLETHITPLANE2,	//	Bullet hit plane 2
	RES_S3D_BULLETHITPLANE3,	//	Bullet hit plane 3
	RES_S3D_BULLETHITPLANE4,	//	Bullet hit plane 4
	RES_S3D_BULLETHITPLANE5,	//	Bullet hit plane 5
	RES_S3D_BULLETHITPLANE6,	//	Bullet hit plane 6
	RES_S3D_BULLETHITPLANE7,	//	Bullet hit plane 7
	RES_S3D_BULLETHITPLANE8,	//	Bullet hit plane 8
	RES_S3D_BULLETHITPLANE9,	//	Bullet hit plane 9
	RES_S3D_BULLETHITPLANE10,	//	Bullet hit plane 10
	RES_S3D_BULLETHITPLANE11,	//	Bullet hit plane 11
	RES_S3D_BULLETHITPLANE12,	//	Bullet hit plane 12
	RES_S3D_BULLETHITTANK1,		//	Bullet hit tank 1
	RES_S3D_BULLETHITTANK2,		//	Bullet hit tank 2
	RES_S3D_BULLETHITTANK3,		//	Bullet hit tank 3
	RES_S3D_BULLETHITTANK4,		//	Bullet hit tank 4
	RES_S3D_BULLETHITTANK5,		//	Bullet hit tank 5
	RES_S3D_CANNONWHIZZ1,		//	Cannon bomb whizz 1
	RES_S3D_CANNONWHIZZ2,		//	Cannon bomb whizz 2
	RES_S3D_CANNONWHIZZ3,		//	Cannon bomb whizz 3
	RES_S3D_STAB1,				//	Stab 1
	RES_S3D_STAB2,				//	Stab 2
	RES_S3D_STAB3,				//	Stab 3
	RES_S3D_MIG21FIRE,			//	MIG-21 Fire
	RES_S3D_FANTANFIRE,			//	Fantan fire
	RES_S3D_TANKFIRE1,			//	Tank fire 1
	RES_S3D_TANKFIRE2,			//	Tank fire 2
	RES_S3D_TANKEXPLODE,		//	Tank explode
	RES_S3D_TANKSTARTUP,		//	Tank startup
	RES_S3D_TANKRUNNING,		//	Tank running
	RES_S3D_ARMOREDEXPLODE,		//	Armored car explode
	RES_S3D_ARMOREDRUNNING,		//	Armored car running
	NUM_RESSOUNDFILE
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
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

const char* RES_GetModelFile(int iID);
const char* RES_GetGFXFile(int iID);
const char* RES_GetOtherFile(int iID);
const char* RES_GetSoundFile(int iID);
const char* RES_GetName(int iID);


#endif	//	_ML_RESOURCES_H_



