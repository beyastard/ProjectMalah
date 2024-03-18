/*
 * FILE: ML_EventPtData.cpp
 *
 * DESCRIPTION: Event point data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_EventPtData.h"

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
//	Implement CMLEventPtData
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLSky
IMPLEMENT_ASERIAL(CMLEventPtData, AData, 1)

CMLEventPtData::CMLEventPtData()
{
	memset(&m_Data, 0, sizeof (m_Data));

	m_Data.vPos			= A3DVECTOR3(0.0f);
	m_Data.fRadius		= 1.0f;
	m_Data.vCenter		= A3DVECTOR3(0.0f);
	m_Data.fDegree		= 30.0f;
	m_Data.fHalfWidth	= 2.0f;
	m_Data.vLineEnd		= m_Data.vPos;
	m_Data.vLineDir		= A3DVECTOR3(0.0f, 0.0f, 1.0f);
	m_Data.fLineLen		= 1.0f;
	m_Data.iNumEvent	= 0;
	m_Data.bRushTo		= false;
	m_Data.iNumLoop		= 1;
}

//	Load route
bool CMLEventPtData::Load(AArchive& ar)
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
bool CMLEventPtData::Save(AArchive& ar)
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




