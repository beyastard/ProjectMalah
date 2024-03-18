/*
 * FILE: ML_RouteData.cpp
 *
 * DESCRIPTION: Route data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_RouteData.h"
#include "ML_ID.h"

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
//	Implement CMLRouteData
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLSky
IMPLEMENT_ASERIAL(CMLRouteData, AData, 1)

CMLRouteData::CMLRouteData()
{
	memset(&m_Data, 0, sizeof (m_Data));

	m_Data.iUserID = ID_AI_FOOTMAN;
}

//	Load route
bool CMLRouteData::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	if (!ar.Read(&m_Data, sizeof (m_Data)))
	{
		ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

//	Save route
bool CMLRouteData::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	if (!ar.Write(&m_Data, sizeof (m_Data)))
	{
		ADebug::Msg(0, "%s %d write error!", __FILE__, __LINE__);
		return false;
	}

	return true;
}



