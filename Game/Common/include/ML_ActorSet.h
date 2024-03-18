/*
 * FILE: ML_ActorSet.h
 *
 * DESCRIPTION: Actor (player) data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ACTORSET_H_
#define _ML_ACTORSET_H_

#include "ML_LevelData.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Integer level data
enum
{
	ILD_AMMO_PISTOL	= 0,		//	Pistol ammo
	ILD_AMMO_RIFLE,				//	Rifle ammo
	ILD_AMMO_MORTAR,			//	Shrapnel ammo
	ILD_AMMO_MACHINE,			//	Machine gun ammo

	ILD_AMMOINC_PISTOL,			//	Pistol ammo increment
	ILD_AMMOINC_RIFLE,			//	Rifle ammo increment
	ILD_AMMOINC_MORTAR,			//	Shrapnel ammo increment
	ILD_AMMOINC_MACHINE,		//	Machine gun ammo increment

	ILD_LIFE_PLAYER,			//	Player's life
	ILD_LIFE_HELICOPTER,		//	Helicopter's life
	
	ILD_SUPP_CANNON,			//	Cannon support interval (s)
	ILD_SUPP_HELICOPTER,		//	Supply-helicopter support interval (s)
	ILD_SUPP_ATTACKER,			//	Attacker plane support interval (s)
	
	ILD_DAMAGE_PISTOL,			//	Pistol damage
	ILD_DAMAGE_RIFLE,			//	Rifle damage
	ILD_DAMAGE_MORTAR,			//	Shrapnel damage
	ILD_DAMAGE_MACHINE,			//	Machine gun damage
	ILD_DAMAGE_CANNON,			//	Cannon damage
	ILD_DAMAGE_FIREBOMB,		//	Fire bomb from attacker damage
			
	//	Exist when g_iGenVersion >= 3
	ILD_AMMO_RPG,				//	RPG
	ILD_AMMOINC_RPG,			//	RPG ammo increment
	ILD_DAMAGE_RPG,				//	RPG damage
	
	//	Exit when g_iGenVersion >= 4
	ILD_LIFE_SUPPLY,			//	Life supply amount

	NUM_ACTORILD,				//	Number of actor's Integer Level Data
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
//	Class CMLActorSet
//
///////////////////////////////////////////////////////////////////////////

class CMLActorSet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLActorSet)

public:		//	Constructor and Destructor

	CMLActorSet();
	virtual ~CMLActorSet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	virtual bool	Load(AArchive& ar);		//	Load actor data set
	virtual bool	Save(AArchive& ar);		//	Save actor data set

	int		GetILDIdx(CMLLevelDataInt* pLvlData);	//	Get ILD object's index

	CMLLevelDataInt*	GetILD(int iIndex)	{	return &m_aILD[iIndex];		}

protected:	//	Attributes

	CMLLevelDataInt		m_aILD[NUM_ACTORILD];		//	Integer Level Data array
	
protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_ACTORSET_H_



