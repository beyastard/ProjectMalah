/*
 * FILE: ML_ActorSet.cpp
 *
 * DESCRIPTION: Actor (player) data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_ActorSet.h"
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

extern int	g_iGenVersion;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLActorSet
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLActorSet, AData, 1)

CMLActorSet::CMLActorSet()
{
	m_aILD[ILD_AMMO_PISTOL].SetName(g_MLStrings.GetString(SI_AMMO_PISTOL));
	m_aILD[ILD_AMMO_RIFLE].SetName(g_MLStrings.GetString(SI_AMMO_RIFLE));
	m_aILD[ILD_AMMO_MORTAR].SetName(g_MLStrings.GetString(SI_AMMO_MORTAR));
	m_aILD[ILD_AMMO_MACHINE].SetName(g_MLStrings.GetString(SI_AMMO_MACHINE));

	m_aILD[ILD_AMMOINC_PISTOL].SetName(g_MLStrings.GetString(SI_AMMOINC_PISTOL));
	m_aILD[ILD_AMMOINC_RIFLE].SetName(g_MLStrings.GetString(SI_AMMOINC_RIFLE));
	m_aILD[ILD_AMMOINC_MORTAR].SetName(g_MLStrings.GetString(SI_AMMOINC_MORTAR));
	m_aILD[ILD_AMMOINC_MACHINE].SetName(g_MLStrings.GetString(SI_AMMOINC_MACHINE));

	m_aILD[ILD_SUPP_CANNON].SetName(g_MLStrings.GetString(SI_SUPP_CANNON));
	m_aILD[ILD_SUPP_HELICOPTER].SetName(g_MLStrings.GetString(SI_SUPP_HELIPCOPTER));
	m_aILD[ILD_SUPP_ATTACKER].SetName(g_MLStrings.GetString(SI_SUPP_ATTACKER));

	m_aILD[ILD_DAMAGE_PISTOL].SetName(g_MLStrings.GetString(SI_DAMAGE_PISTOL));
	m_aILD[ILD_DAMAGE_RIFLE].SetName(g_MLStrings.GetString(SI_DAMAGE_RIFLE));
	m_aILD[ILD_DAMAGE_MORTAR].SetName(g_MLStrings.GetString(SI_DAMAGE_MORTAR));
	m_aILD[ILD_DAMAGE_MACHINE].SetName(g_MLStrings.GetString(SI_DAMAGE_MACHINE));
	m_aILD[ILD_DAMAGE_CANNON].SetName(g_MLStrings.GetString(SI_DAMAGE_CANNON));
	m_aILD[ILD_DAMAGE_FIREBOMB].SetName(g_MLStrings.GetString(SI_DAMAGE_FIREBOMB));

	m_aILD[ILD_LIFE_PLAYER].SetName(g_MLStrings.GetString(SI_LIFE_PLAYER));
	m_aILD[ILD_LIFE_HELICOPTER].SetName(g_MLStrings.GetString(SI_LIFE_HELICOPTER));

	m_aILD[ILD_AMMO_RPG].SetName(g_MLStrings.GetString(SI_AMMO_RPG));
	m_aILD[ILD_AMMOINC_RPG].SetName(g_MLStrings.GetString(SI_AMMOINC_RPG));
	m_aILD[ILD_DAMAGE_RPG].SetName(g_MLStrings.GetString(SI_DAMAGE_RPG));

	m_aILD[ILD_LIFE_SUPPLY].SetName(g_MLStrings.GetString(SI_LIFE_SUPPLY));
}

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLActorSet::Init()
{
	return true;
}

//	Release object
bool CMLActorSet::Release()
{
	return AData::Release();
}

/*	Get ILD object's index

	Return -1 if specified integer level data doesn't belong to CMLActorSet.
*/
int	CMLActorSet::GetILDIdx(CMLLevelDataInt* pLvlData)
{
	for (int i=0; i < NUM_ACTORILD; i++)
	{
		if (&m_aILD[i] == pLvlData)
			return i;
	}

	return -1;
}

//	Load actor data set
bool CMLActorSet::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion != CMLActorSet::GetClassInfo()->GetVersion())
	{
		assert(0);
		return false;
	}

	int iNumILD = 19;
	if (g_iGenVersion >= 3)
		ar.Read(&iNumILD, sizeof (iNumILD));

	for (int i=0; i < iNumILD; i++)
		m_aILD[i].Load(ar);

	return true;
}

//	Save actor data set
bool CMLActorSet::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	int iVersion = CMLActorSet::GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		assert(0);
		return false;
	}

	int iNumILD = g_iGenVersion < 3 ? 19 : NUM_ACTORILD;
	
	if (g_iGenVersion >= 3)
		ar.Write(&iNumILD, sizeof (iNumILD));

	for (int i=0; i < iNumILD; i++)
		m_aILD[i].Save(ar);

	return true;
}


