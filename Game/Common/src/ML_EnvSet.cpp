/*
 * FILE: ML_EnvSet.cpp
 *
 * DESCRIPTION: Environment data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/15
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_EnvSet.h"

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
//	Implement CMLEnvSet
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLEnvironment
IMPLEMENT_ASERIAL(CMLEnvironment, AEnvironmentInit, 1)
IMPLEMENT_ASERIAL(CMLEnvSet, AData, 1)

CMLEnvSet::CMLEnvSet()
{
	m_CurPos = NULL;
}

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLEnvSet::Init()
{
	return true;
}

//	Release object
bool CMLEnvSet::Release()
{
	ReleaseAllEnvironment();

	return AData::Release();
}

//	Release all environment
void CMLEnvSet::ReleaseAllEnvironment()
{
	AListPosition pos = m_EnvList.GetHeadPosition();
	while (pos)
	{
		CMLEnvironment* ptr = m_EnvList.GetNext(pos);
		delete ptr;
	}

	m_EnvList.RemoveAll();
}

/*	Load sky set

	Return true for success, otherwise return false.
*/
bool CMLEnvSet::Load(AArchive& ar)
{
	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion != CMLEnvSet::GetClassInfo()->GetVersion())
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

	//	Load sky data.
	for (int i=0; i < iNum; i++)
	{
		//	Create object
		CMLEnvironment* ptr = (CMLEnvironment*)ar.CreateObject();

		//	Load data
		if (!ptr->Load(ar))
		{
			ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
			return false;
		}

		m_EnvList.AddTail(ptr);
	}

	return true;
}

/*	Save sky set

	Return true for success, otherwise return false.
*/
bool CMLEnvSet::Save(AArchive& ar)
{
	int iVersion = CMLEnvSet::GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		assert(0);
		return false;
	}

	int iNum = m_EnvList.GetCount();
	if (!ar.Write(&iNum, sizeof (iNum)))
	{
		ADebug::Msg(0," %s %d AArchive write error", __FILE__, __LINE__);
		return false;
	}

	//	Save sky data.
	AListPosition pos = m_EnvList.GetHeadPosition();
	while (pos)
	{
		CMLEnvironment* ptr = m_EnvList.GetNext(pos);
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

//	Add a environment
void CMLEnvSet::AddEnv(CMLEnvironment* pEnv)
{
	m_EnvList.AddTail(pEnv);
}

//	Del a sky
void CMLEnvSet::DelEnv(CMLEnvironment* pEnv)
{
	AListPosition Pos = m_EnvList.Find(pEnv);
	if (Pos)
	{
		delete pEnv;
		m_EnvList.RemoveAt(Pos);
	}
}

//	Search a environment by name
CMLEnvironment*	CMLEnvSet::SearchEnv(AUString& strName)
{
	AListPosition pos = m_EnvList.GetHeadPosition();
	while (pos)
	{
		CMLEnvironment* ptr = m_EnvList.GetNext(pos);
		if (ptr->GetName() == strName)
			return ptr;
	}

	return NULL;
}

//	Search a environment by index
CMLEnvironment*	CMLEnvSet::SearchEnv(int iIndex)
{
	AListPosition pos = m_EnvList.FindByIndex(iIndex);
	if (!pos)
		return NULL;

	return (CMLEnvironment*)m_EnvList.GetAt(pos);
}