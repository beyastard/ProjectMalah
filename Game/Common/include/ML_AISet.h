/*
 * FILE: ML_AISet.h
 *
 * DESCRIPTION: AI (player) data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_AISET_H_
#define _ML_AISET_H_

#include "ML_LevelData.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Integer level data
enum
{
	ILD_DAMAGE_F = 0,		//	Damage of footman
	ILD_DAMAGE_G,			//	Damage of guerilla
	ILD_DAMAGE_R,			//	Damage of rocket man
	ILD_DAMAGE_STAB,		//	Stab damage

	ILD_PRECISE_STAND_F,	//	Stand shot precise of footman
	ILD_PRECISE_STAND_G,	//	Stand shot precise of guerilla
	ILD_PRECISE_STAND_R,	//	Stand shot precise of rocket man
	ILD_PRECISE_MOVE_F,		//	Move shot precise of footman
	ILD_PRECISE_MOVE_G,		//	Move shot precise of guerilla
	ILD_PRECISE_CREEP_F,	//	Creep shot precise of footman
	ILD_PRECISE_CREEP_G,	//	Creep shot precise of guerilla
	ILD_PRECISE_CREEP_R,	//	Creep shot precise of rocket man
	ILD_PRECISE_HITCOPTER,	//	Precise of hit helicopter

	ILD_FREQ_STAND_F,		//	Stand shot frequence of footman
	ILD_FREQ_STAND_G,		//	Stand shot frequence of guerilla
	ILD_FREQ_STAND_R,		//	Stand shot frequence of rocket man
	ILD_FREQ_MOVE_F,		//	Move shot frequence of footman
	ILD_FREQ_MOVE_G,		//	Move shot frequence of guerilla
	ILD_FREQ_MOVE_R,		//	Move shot frequence of rocket man
	ILD_FREQ_CREEP_F,		//	Creep shot frequence of footman
	ILD_FREQ_CREEP_G,		//	Creep shot frequence of guerilla
	ILD_FREQ_CREEP_R,		//	Creep shot frequence of rocket man

	ILD_TIME_AIGEN,			//	Length of time to generate AI

	ILD_LIFE_FOOTMAN,		//	Life of footman
	ILD_LIFE_GUERILLA,		//	Life of guerilla
	ILD_LIFE_ROCKET,		//	Life of rocket man

	ILD_PER_ATTACKCOPTER,	//	Percent to attack helicopter

	ILD_AIGEN_INTERVAL,		//	Generate interval
	ILD_AIGEN_NUMBER,		//	Generate number every turn
	ILD_AIGEN_TURN,			//	Generate turn number
	ILD_AIGEN_TIME,			//	First generate time

	ILD_MIG21_NUMBER,		//	Number of MIG-21
	ILD_MIG21_INTERVAL,		//	Appear interval of MIG-21
	ILD_MIG21_SIMUNUM,		//	Number of MIG-21 appearing simultaneously

	ILD_FANTAN_NUMBER,		//	Number of Fantan
	ILD_FANTAN_INTERVAL,	//	Appear interval of Fantan
	ILD_FANTAN_SIMUNUM,		//	Number of Fantan appearing simultaneously
	ILD_FANTAN_ROCKETNUM,	//	Number of Fantan's rocket bomb

	NUM_AIILD,
};

//	Float level data
enum
{
	FLD_SPEED_STAND_F = 0,	//	Stand moving speed of footman
	FLD_SPEED_STAND_G,		//	Stand moving speed of guerilla
	FLD_SPEED_STAND_R,		//	Stand moving speed of rocket man

	FLD_DAMAGE_DISTANCE,	//	Damage scale factor on distance
	FLD_PRECISE_DISTANCE,	//	Precise scale factor on distance

	NUM_AIFLD,
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
//	Class CMLAISet
//
///////////////////////////////////////////////////////////////////////////

class CMLAISet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLAISet)

public:		//	Constructor and Destructor

	CMLAISet();
	virtual ~CMLAISet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	virtual bool	Load(AArchive& ar);		//	Load AI data set
	virtual bool	Save(AArchive& ar);		//	Save AI data set

	int			GetILDIdx(CMLLevelDataInt* pLvlData);		//	Get ILD object's index
	int			GetFLDIdx(CMLLevelDataFloat* pLvlData);		//	Get FLD object's index

	CMLLevelDataInt*	GetILD(int iIndex)	{	return &m_aILD[iIndex];		}
	CMLLevelDataFloat*	GetFLD(int iIndex)	{	return &m_aFLD[iIndex];		}

protected:	//	Attributes

protected:	//	Operations

	CMLLevelDataInt		m_aILD[NUM_AIILD];		//	Integer Level Data array
	CMLLevelDataFloat	m_aFLD[NUM_AIFLD];		//	Float level data array
};


///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_AISET_H_



