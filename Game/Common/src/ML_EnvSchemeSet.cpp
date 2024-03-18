/*
 * FILE: ML_EnvSchemeSet.cpp
 *
 * DESCRIPTION: Environment scheme set in game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/12
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_EnvSchemeSet.h"

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
//	Implement CMLEnvSchemeSet
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLEnvironment
IMPLEMENT_ASERIAL(CMLEnvScheme, AData, 1)
IMPLEMENT_ASERIAL(CMLEnvSchemeSet, AData, 1)

CMLEnvSchemeSet::CMLEnvSchemeSet() : AData()
{
	m_CurPos = NULL;
}

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLEnvSchemeSet::Init()
{
	return true;
}

//	Release object
bool CMLEnvSchemeSet::Release()
{
	ReleaseAllEnvScheme();

	return AData::Release();
}

//	Release all environment scheme
void CMLEnvSchemeSet::ReleaseAllEnvScheme()
{
	AListPosition pos = m_SchemeList.GetHeadPosition();
	while (pos)
	{
		CMLEnvScheme* ptr = m_SchemeList.GetNext(pos);
		delete ptr;
	}

	m_SchemeList.RemoveAll();
}

/*	Load sky set

	Return true for success, otherwise return false.
*/
bool CMLEnvSchemeSet::Load(AArchive& ar)
{
	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion != CMLEnvSchemeSet::GetClassInfo()->GetVersion())
	{
		assert(0);
		return false;
	}

	int iNum = 0;
	if (!ar.Read(&iNum, sizeof (iNum)))
	{
		ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
		return false;
	}

	//	Load scheme set.
	for (int i=0; i < iNum; i++)
	{
		//	Create object
		CMLEnvScheme* ptr = (CMLEnvScheme*)ar.CreateObject();

		//	Load data
		if (!ptr->Load(ar))
		{
			ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
			return false;
		}

		m_SchemeList.AddTail(ptr);
	}

	return true;
}

/*	Save sky set

	Return true for success, otherwise return false.
*/
bool CMLEnvSchemeSet::Save(AArchive& ar)
{
	int iVersion = CMLEnvSchemeSet::GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		assert(0);
		return false;
	}

	int iNum = m_SchemeList.GetCount();
	if (!ar.Write(&iNum, sizeof (iNum)))
	{
		ADebug::Msg(0," %s %d AArchive write error", __FILE__, __LINE__);
		return false;
	}

	//	Save sky data.
	AListPosition pos = m_SchemeList.GetHeadPosition();
	while (pos)
	{
		CMLEnvScheme* ptr = m_SchemeList.GetNext(pos);
		if (!ptr)
		{
			assert(0);
			continue;
		}

		//	Class information
		ar.WriteClassInfo(ptr);

		//	Save data
		if (!ptr->Save(ar))
		{
			ADebug::Msg(0,"%s %d Save error", __FILE__, __LINE__);
			return false;
		}
	}

	return true;
}

//	Add a environment scheme
void CMLEnvSchemeSet::AddEnvScheme(CMLEnvScheme* pEnv)
{
	m_SchemeList.AddTail(pEnv);
}

//	Del a sky
void CMLEnvSchemeSet::DelEnvScheme(CMLEnvScheme* pEnv)
{
	AListPosition Pos = m_SchemeList.Find(pEnv);
	if (Pos)
	{
		delete pEnv;
		m_SchemeList.RemoveAt(Pos);
	}
}

//	Search a environment scheme by name
CMLEnvScheme* CMLEnvSchemeSet::SearchEnvScheme(AUString& strName)
{
	AListPosition pos = m_SchemeList.GetHeadPosition();
	while (pos)
	{
		CMLEnvScheme* ptr = m_SchemeList.GetNext(pos);
		if (ptr->GetName() == strName)
			return ptr;
	}

	return NULL;
}

//	Search a environment scheme by index
CMLEnvScheme* CMLEnvSchemeSet::SearchEnvScheme(int iIndex)
{
	AListPosition pos = m_SchemeList.FindByIndex(iIndex);
	if (!pos)
		return NULL;

	return (CMLEnvScheme*)m_SchemeList.GetAt(pos);
}

