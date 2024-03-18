/*
 * FILE: ML_GeneralSet.cpp
 *
 * DESCRIPTION: All general data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_GeneralSet.h"

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

//	Make sure g_iMapVersion's initial value is same as CMLGeneralSet's version
int		g_iGenVersion = 4;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLGeneralSet
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of Level Data classes
IMPLEMENT_ASERIAL(CMLLevelData, AData, 1)
IMPLEMENT_ASERIAL(CMLLevelDataInt, CMLLevelData, 1)
IMPLEMENT_ASERIAL(CMLLevelDataFloat, CMLLevelData, 1)

IMPLEMENT_ASERIAL(CMLGeneralSet, AData, 4)

CMLGeneralSet::CMLGeneralSet()
{
}

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLGeneralSet::Init()
{
	if (!m_SkySet.Init())
	{
		ADebug::Msg(0, "Failed to initialize sky set!", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSet.Init())
	{
		ADebug::Msg(0, "Failed to initialize environment set!", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSchemeSet.Init())
	{
		ADebug::Msg(0, "Failed to initialize environment scheme set!", __FILE__, __LINE__);
		return false;
	}

	if (!m_ActorSet.Init())
	{
		ADebug::Msg(0, "Failed to initialize actor set!", __FILE__, __LINE__);
		return false;
	}

	if (!m_AISet.Init())
	{
		ADebug::Msg(0, "Failed to initialize AI set!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

//	Release object
bool CMLGeneralSet::Release()
{
	m_SkySet.Release();
	m_EnvSet.Release();
	m_EnvSchemeSet.Release();
	m_ActorSet.Release();
	m_AISet.Release();

	return AData::Release();
}

//	Load general set from file
bool CMLGeneralSet::Load(const char* szFile)
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

//	Save general set to file
bool CMLGeneralSet::Save(const char* szFile)
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

//	Load environment set
bool CMLGeneralSet::Load(AArchive& ar)
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

	g_iGenVersion = iVersion;

	if (!m_SkySet.Load(ar))
	{
		ADebug::Msg(0, "Failed to load sky data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSet.Load(ar))
	{
		ADebug::Msg(0, "Failed to load environment data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_ActorSet.Load(ar))
	{
		ADebug::Msg(0, "Failed to load actor data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_AISet.Load(ar))
	{
		ADebug::Msg(0, "Failed to load AI data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSchemeSet.Load(ar))
	{
		ADebug::Msg(0, "Failed to load environment scheme set", __FILE__, __LINE__);
		return false;
	}

	if (iVersion > 1)
	{
		if (!m_MissionSet.Load(ar))
		{
			ADebug::Msg(0, "Failed to load mission data set", __FILE__, __LINE__);
			return false;
		}
	}

	//	Restore g_iGenVersion to current version, so ugly the code !
	g_iGenVersion = GetClassInfo()->GetVersion();

	return true;
}

//	Save environment set
bool CMLGeneralSet::Save(AArchive& ar)
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

	g_iGenVersion = iVersion;
	
	if (!m_SkySet.Save(ar))
	{
		ADebug::Msg(0, "Failed to save sky data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSet.Save(ar))
	{
		ADebug::Msg(0, "Failed to save environment data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_ActorSet.Save(ar))
	{
		ADebug::Msg(0, "Failed to save actor data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_AISet.Save(ar))
	{
		ADebug::Msg(0, "Failed to save AI data set", __FILE__, __LINE__);
		return false;
	}

	if (!m_EnvSchemeSet.Save(ar))
	{
		ADebug::Msg(0, "Failed to save environment scheme set", __FILE__, __LINE__);
		return false;
	}

	if (iVersion > 1)
	{
		if (!m_MissionSet.Save(ar))
		{
			ADebug::Msg(0, "Failed to save mission data set", __FILE__, __LINE__);
			return false;
		}
	}

	return true;
}


