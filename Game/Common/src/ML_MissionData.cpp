/*
 * FILE: ML_MissionData.cpp
 *
 * DESCRIPTION: Mission data in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_MissionData.h"

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
//	Implement CMLMissionSet
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLMissionData, AData, 1)
IMPLEMENT_ASERIAL(CMLMissionSet, AData, 1)

CMLMissionSet::CMLMissionSet()
{
	for (int i=0; i < NUM_GAMELEVEL; i++)
	{
		m_aMissions[i] = new CMLMissionData;
	//	ASSERT(m_aMissions[i]);
	}
}

CMLMissionSet::~CMLMissionSet()
{
	for (int i=0; i < NUM_GAMELEVEL; i++)
		delete m_aMissions[i];
}

//	Load mission set
bool CMLMissionSet::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	for (int i=0; i < NUM_GAMELEVEL; i++)
	{
		if (!m_aMissions[i]->Load(ar))
			return false;
	}

	return true;
}

//	Save mission set
bool CMLMissionSet::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	for (int i=0; i < NUM_GAMELEVEL; i++)
	{
		if (!m_aMissions[i]->Save(ar))
			return false;
	}

	return true;
}

