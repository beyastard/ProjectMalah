/*
 * FILE: ML_SkySet.cpp
 *
 * DESCRIPTION: Sky data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/15
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_SkySet.h"

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
//	Implement CMLSkySet
//
///////////////////////////////////////////////////////////////////////////

//	Implement class-serial of CMLSky
IMPLEMENT_ASERIAL(CMLSky, ASkyInit, 1)
IMPLEMENT_ASERIAL(CMLSkySet, AData, 1)

CMLSkySet::CMLSkySet()
{
	m_CurPos = NULL;
}

/*	Initialize object.

	Return true for success, otherwise return false.
*/
bool CMLSkySet::Init()
{
	return true;
}

//	Release object
bool CMLSkySet::Release()
{
	ReleaseAllSkys();

	return AData::Release();
}

//	Release all skys
void CMLSkySet::ReleaseAllSkys()
{
	AListPosition pos = m_SkyList.GetHeadPosition();
	while (pos)
	{
		CMLSky* ptr = m_SkyList.GetNext(pos);
		delete ptr;
	}

	m_SkyList.RemoveAll();
}

/*	Load sky set

	Return true for success, otherwise return false.
*/
bool CMLSkySet::Load(AArchive& ar)
{
	//	Check version
	int iVersion;
	if (!ar.Read(&iVersion, sizeof (iVersion)) ||
		iVersion != CMLSkySet::GetClassInfo()->GetVersion())
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

	//	Load sky data
	for (int i=0; i < iNum; i++)
	{
		//	Create object
		CMLSky* ptr = (CMLSky*)ar.CreateObject();

		//	Load data
		if (!ptr->Load(ar))
		{
			ADebug::Msg(0, "%s %d read error!", __FILE__, __LINE__);
			return false;
		}

		m_SkyList.AddTail(ptr);
	}

	return true;
}

/*	Save sky set

	Return true for success, otherwise return false.
*/
bool CMLSkySet::Save(AArchive& ar)
{
	int iVersion = CMLSkySet::GetClassInfo()->GetVersion();
	if (!ar.Write(&iVersion, sizeof (iVersion)))
	{
		assert(0);
		return false;
	}

	int iNum = m_SkyList.GetCount();
	if (!ar.Write(&iNum, sizeof (iNum)))
	{
		ADebug::Msg(0," %s %d AArchive write error", __FILE__, __LINE__);
		return false;
	}

	//	Save sky data. 
	AListPosition pos = m_SkyList.GetHeadPosition();
	while (pos)
	{
		CMLSky* ptr = m_SkyList.GetNext(pos);
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

//	Add a sky
void CMLSkySet::AddSky(CMLSky* pSky)
{
	m_SkyList.AddTail(pSky);
}

//	Del a sky
void CMLSkySet::DelSky(CMLSky* pSky)
{
	AListPosition Pos = m_SkyList.Find(pSky);
	if (Pos)
	{
		delete pSky;
		m_SkyList.RemoveAt(Pos);
	}
}

//	Search a sky by name
CMLSky*	CMLSkySet::SearchSky(AUString& strName)
{
	AListPosition pos = m_SkyList.GetHeadPosition();
	while (pos)
	{
		CMLSky* ptr = m_SkyList.GetNext(pos);
		if (ptr->GetName() == strName)
			return ptr;
	}

	return NULL;
}

//	Search a sky by index
CMLSky*	CMLSkySet::SearchSky(int iIndex)
{
	AListPosition pos = m_SkyList.FindByIndex(iIndex);
	if (!pos)
		return NULL;

	return (CMLSky*)m_SkyList.GetAt(pos);
}

