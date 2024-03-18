/*
 * FILE: ML_AISet.cpp
 *
 * DESCRIPTION: AI (player) data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_AISet.h"
#include "ML_Strings.h"

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
//	Implement CMLAISet
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLAISet, AData, 1)

CMLAISet::CMLAISet()
{
	m_aILD[ILD_DAMAGE_F].SetName(g_MLStrings.GetString(SI_DAMAGE_F));
	m_aILD[ILD_DAMAGE_G].SetName(g_MLStrings.GetString(SI_DAMAGE_G));
	m_aILD[ILD_DAMAGE_R].SetName(g_MLStrings.GetString(SI_DAMAGE_R));
	m_aILD[ILD_DAMAGE_STAB].SetName(g_MLStrings.GetString(SI_DAMAGE_STAB));

	m_aILD[ILD_PRECISE_STAND_F].SetName(g_MLStrings.GetString(SI_PRECISE_STAND_F));
	m_aILD[ILD_PRECISE_STAND_G].SetName(g_MLStrings.GetString(SI_PRECISE_STAND_G));
	m_aILD[ILD_PRECISE_STAND_R].SetName(g_MLStrings.GetString(SI_PRECISE_STAND_R));
	m_aILD[ILD_PRECISE_MOVE_F].SetName(g_MLStrings.GetString(SI_PRECISE_MOVE_F));
	m_aILD[ILD_PRECISE_MOVE_G].SetName(g_MLStrings.GetString(SI_PRECISE_MOVE_G));
	m_aILD[ILD_PRECISE_CREEP_F].SetName(g_MLStrings.GetString(SI_PRECISE_CREEP_F));
	m_aILD[ILD_PRECISE_CREEP_G].SetName(g_MLStrings.GetString(SI_PRECISE_CREEP_G));
	m_aILD[ILD_PRECISE_CREEP_R].SetName(g_MLStrings.GetString(SI_PRECISE_CREEP_R));
	m_aILD[ILD_PRECISE_HITCOPTER].SetName(g_MLStrings.GetString(SI_PRECISE_HITCOPTER));
	m_aILD[ILD_PER_ATTACKCOPTER].SetName(g_MLStrings.GetString(SI_PER_ATTACKCOPTER));

	m_aILD[ILD_FREQ_STAND_F].SetName(g_MLStrings.GetString(SI_FREQ_STAND_F));
	m_aILD[ILD_FREQ_STAND_G].SetName(g_MLStrings.GetString(SI_FREQ_STAND_G));
	m_aILD[ILD_FREQ_STAND_R].SetName(g_MLStrings.GetString(SI_FREQ_STAND_R));
	m_aILD[ILD_FREQ_MOVE_F].SetName(g_MLStrings.GetString(SI_FREQ_MOVE_F));
	m_aILD[ILD_FREQ_MOVE_G].SetName(g_MLStrings.GetString(SI_FREQ_MOVE_G));
	m_aILD[ILD_FREQ_MOVE_R].SetName(g_MLStrings.GetString(SI_FREQ_MOVE_R));
	m_aILD[ILD_FREQ_CREEP_F].SetName(g_MLStrings.GetString(SI_FREQ_CREEP_F));
	m_aILD[ILD_FREQ_CREEP_G].SetName(g_MLStrings.GetString(SI_FREQ_CREEP_G));
	m_aILD[ILD_FREQ_CREEP_R].SetName(g_MLStrings.GetString(SI_FREQ_CREEP_R));

	m_aILD[ILD_TIME_AIGEN].SetName(g_MLStrings.GetString(SI_TIME_AIGEN));

	m_aILD[ILD_LIFE_FOOTMAN].SetName(g_MLStrings.GetString(SI_LIFE_FOOTMAN));
	m_aILD[ILD_LIFE_GUERILLA].SetName(g_MLStrings.GetString(SI_LIFE_GUERILLA));
	m_aILD[ILD_LIFE_ROCKET].SetName(g_MLStrings.GetString(SI_LIFE_ROCKET));

	m_aILD[ILD_AIGEN_INTERVAL].SetName(g_MLStrings.GetString(SI_AIGEN_INTERVAL));
	m_aILD[ILD_AIGEN_NUMBER].SetName(g_MLStrings.GetString(SI_AIGEN_NUMBER));
	m_aILD[ILD_AIGEN_TURN].SetName(g_MLStrings.GetString(SI_AIGEN_TURN));
	m_aILD[ILD_AIGEN_TIME].SetName(g_MLStrings.GetString(SI_AIGEN_TIME));

	m_aILD[ILD_MIG21_NUMBER].SetName(g_MLStrings.GetString(SI_MIG21_NUMBER));
	m_aILD[ILD_MIG21_INTERVAL].SetName(g_MLStrings.GetString(SI_MIG21_INTERVAL));
	m_aILD[ILD_MIG21_SIMUNUM].SetName(g_MLStrings.GetString(SI_MIG21_SIMUNUM));

	m_aILD[ILD_FANTAN_NUMBER].SetName(g_MLStrings.GetString(SI_FANTAN_NUMBER));
	m_aILD[ILD_FANTAN_INTERVAL].SetName(g_MLStrings.GetString(SI_FANTAN_INTERVAL));
	m_aILD[ILD_FANTAN_SIMUNUM].SetName(g_MLStrings.GetString(SI_FANTAN_SIMUNUM));
	m_aILD[ILD_FANTAN_ROCKETNUM].SetName(g_MLStrings.GetString(SI_FANTAN_ROCKETNUM));

	m_aFLD[FLD_SPEED_STAND_F].SetName(g_MLStrings.GetString(SI_SPEED_STAND_F));
	m_aFLD[FLD_SPEED_STAND_G].SetName(g_MLStrings.GetString(SI_SPEED_STAND_G));
	m_aFLD[FLD_SPEED_STAND_R].SetName(g_MLStrings.GetString(SI_SPEED_STAND_R));

	m_aFLD[FLD_DAMAGE_DISTANCE].SetName(g_MLStrings.GetString(SI_DAMAGE_DISTANCE));
	m_aFLD[FLD_PRECISE_DISTANCE].SetName(g_MLStrings.GetString(SI_PRECISE_DISTANCE));
}	

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLAISet::Init()
{
	return true;
}

//	Release object
bool CMLAISet::Release()
{
	return true;
}

/*	Get ILD object's index

	Return -1 if specified integer level data doesn't belong to CMLAISet.
*/
int	CMLAISet::GetILDIdx(CMLLevelDataInt* pLvlData)
{
	for (int i=0; i < NUM_AIILD; i++)
	{
		if (&m_aILD[i] == pLvlData)
			return i;
	}

	return -1;
}

//	Get FLD object's index
int	CMLAISet::GetFLDIdx(CMLLevelDataFloat* pLvlData)
{
	for (int i=0; i < NUM_AIFLD; i++)
	{
		if (&m_aFLD[i] == pLvlData)
			return i;
	}

	return -1;
}

//	Load actor data set
bool CMLAISet::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion != CMLAISet::GetClassInfo()->GetVersion())
	{
		assert(0);
		return false;
	}

	int i;
	for (i=0; i < NUM_AIILD; i++)
		m_aILD[i].Load(ar);

	for (i=0; i < NUM_AIFLD; i++)
		m_aFLD[i].Load(ar);

	return true;
}

//	Save actor data set
bool CMLAISet::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	int iVersion = CMLAISet::GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		assert(0);
		return false;
	}

	int i;
	for (i=0; i < NUM_AIILD; i++)
		m_aILD[i].Save(ar);

	for (i=0; i < NUM_AIFLD; i++)
		m_aFLD[i].Save(ar);

	return true;
}




