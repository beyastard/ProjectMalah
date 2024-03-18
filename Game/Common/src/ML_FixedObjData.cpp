/*
 * FILE: ML_FixedObjData.cpp
 *
 * DESCRIPTION: Fixed object data in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/13
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_FixedObjData.h"

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
//	Implement CMLFixedObjData
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLFixedObjData
IMPLEMENT_ASERIAL(CMLFixedObjData, AData, 1)

CMLFixedObjData::CMLFixedObjData()
{
	memset(&m_Data, 0, sizeof (m_Data));

	m_Data.vDir	= A3DVECTOR3(0.0f, 0.0f, 1.0f);
	m_Data.vUp	= A3DVECTOR3(0.0f, 1.0f, 0.0f);
}

//	Load route
bool CMLFixedObjData::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	if (!ar.Read(&m_Data, sizeof (m_Data)))
	{
		ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
		return false;
	}

	if (!ar.ReadString(m_szModelFile))
		return false;

	if (!ar.ReadString(m_szRuinFile))
		return false;

	return true;
}

//	Save route
bool CMLFixedObjData::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	if (!ar.Write(&m_Data, sizeof (m_Data)))
	{
		ADebug::Msg(0, "%s %d write error!", __FILE__, __LINE__);
		return false;
	}

	if (!ar.WriteString(m_szModelFile))
		return false;
	
	if (!ar.WriteString(m_szRuinFile))
		return false;

	return true;
}


