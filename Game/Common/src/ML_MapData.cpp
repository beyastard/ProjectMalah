/*
 * FILE: ML_MapData.cpp
 *
 * DESCRIPTION: Map data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/29
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_MapData.h"

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

//	Make sure g_iMapVersion's initial value is same as CMLMapData's version
int		g_iMapVersion = 2;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLMapData
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLSceneData, ASceneInit, 1)
IMPLEMENT_ASERIAL(CMLMapData, AData, 2)

//	Load scene data
bool CMLSceneData::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;
	
	if (!ASceneInit::Load(ar))
		return false;
	
	if (!ar.ReadString(m_strScheme))
		return false;

	if (g_iMapVersion >= 2)
		ar.ReadString(m_strVSSFile);
	
	return true;
}

//	Save scene data
bool CMLSceneData::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;
	
	if (!ASceneInit::Save(ar))
		return false;
	
	if (!ar.WriteString(m_strScheme))
		return false;
	
	if (g_iMapVersion >= 2)
		ar.WriteString(m_strVSSFile);

	return true;
}

CMLMapData::CMLMapData()
{
	m_vActorPos = A3DVECTOR3(0.0f);
	m_vActorDir = A3DVECTOR3(0.0f, 0.0f, 1.0f);
}

/*	Initialize object

	Return true for success, otherwise return false.
*/
bool CMLMapData::Init()
{
	return true;
}

//	Release object
bool CMLMapData::Release()
{
	return AData::Release();
}

//	Load map data from file
bool CMLMapData::Load(const char* szFile)
{
	AUFile File;
	if (!File.Create(szFile, OPEN_EXISTING))
	{
		ADebug::Msg(0, "Failed to open file!", __FILE__, __LINE__);
		return false;
	}

	AArchive ar(&File, false);
	if (!Load(ar))
	{
		ADebug::Msg(0, "Failed to load data!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

//	Save map data to file
bool CMLMapData::Save(const char* szFile)
{
	AUFile File;
	if (!File.Create(szFile, CREATE_ALWAYS))
	{
		ADebug::Msg(0, "Failed to create file!", __FILE__, __LINE__);
		return false;
	}

	AArchive ar(&File, true);
	if (!Save(ar))
	{
		ADebug::Msg(0, "Failed to save data!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

//	Load map data
bool CMLMapData::Load(AArchive& ar)
{
	if (!AData::Load(ar))
		return false;

	//	Check data flag
	DWORD dwFlag;
	if (!ar.Read(&dwFlag, sizeof (dwFlag)) || dwFlag != GetDataFlag())
	{
		ADebug::Msg(0, "Wrong file format!", __FILE__, __LINE__);
		return false;
	}

	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion > GetClassInfo()->GetVersion())
	{
		assert(0);
		ADebug::Msg(0, "Wrong version!", __FILE__, __LINE__);
		return false;
	}

	g_iMapVersion = iVersion;

	//	Load scene data
	if (!m_SceneData.Load(ar))
	{
		ADebug::Msg(0, "Failed to load scene data!", __FILE__, __LINE__);
		return false;
	}

	//	Load actor's position and direction
	if (!ar.Read(&m_vActorPos, sizeof (m_vActorPos)) ||
		!ar.Read(&m_vActorDir, sizeof (m_vActorDir)))
	{
		ADebug::Msg(0, "Failed to load actor pos and dir!", __FILE__, __LINE__);
		return false;
	}

	//	Restore g_iMapVersion to current version, so ugly the code is !
	g_iMapVersion = GetClassInfo()->GetVersion();

	return true;
}

//	Save general set
bool CMLMapData::Save(AArchive& ar)
{
	if (!AData::Save(ar))
		return false;

	//	Write data flag
	DWORD dwFlag = GetDataFlag();
	if (!ar.Write(&dwFlag, sizeof (dwFlag)))
	{
		ADebug::Msg(0, "Failed to write data flag", __FILE__, __LINE__);
		return false;
	}

	//	Write version
	int iVersion = GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		ADebug::Msg(0, "Failed to write version", __FILE__, __LINE__);
		return false;
	}

	g_iMapVersion = iVersion;

	//	Save scene data
	if (!m_SceneData.Save(ar))
	{
		ADebug::Msg(0, "Failed to save scene data!", __FILE__, __LINE__);
		return false;
	}

	//	Save actor's position and direction
	if (!ar.Write(&m_vActorPos, sizeof (m_vActorPos)) ||
		!ar.Write(&m_vActorDir, sizeof (m_vActorDir)))
	{
		ADebug::Msg(0, "Failed to save actor pos and dir!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

