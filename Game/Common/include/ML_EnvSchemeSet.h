/*
 * FILE: ML_EnvSchemeSet.h
 *
 * DESCRIPTION: Environment scheme set in game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/12
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ENVSCHEMESET_H_
#define _ML_ENVSCHEMESET_H_

#include "ML_ABasePatch.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEnvSchemeSet
//
///////////////////////////////////////////////////////////////////////////

class CMLEnvSchemeSet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLEnvSchemeSet)

public:		//	Constructor and Destructor

	CMLEnvSchemeSet();
	virtual ~CMLEnvSchemeSet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	virtual bool	Load(AArchive& ar);		//	Load environment scheme set
	virtual bool	Save(AArchive& ar);		//	Save environment scheme set

	void		AddEnvScheme(CMLEnvScheme* pEnv);		//	Add a environment scheme
	void		DelEnvScheme(CMLEnvScheme* pEnv);		//	Delete a environment scheme

	CMLEnvScheme*	SearchEnvScheme(AUString& strName);	//	Search a environment scheme by name
	CMLEnvScheme*	SearchEnvScheme(int iIndex);		//	Search a environment scheme by index

	int			GetEnvSchemeNum()	{	return m_SchemeList.GetCount();		}

	inline CMLEnvScheme*	GetFirstEnvScheme();	//	Get first environment scheme
	inline CMLEnvScheme*	GetNextEnvScheme();		//	Get next environment scheme

protected:	//	Attributes

	AListPosition	m_CurPos;		//	current position

	AListT<CMLEnvScheme*>	m_SchemeList;		//	Environment list
	
protected:	//	Operations

	void		ReleaseAllEnvScheme();	//	Release all environment scheme
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Get first environment scheme
CMLEnvScheme* CMLEnvSchemeSet::GetFirstEnvScheme()
{
	m_CurPos = m_SchemeList.GetHeadPosition();
	if (m_CurPos)
		return m_SchemeList.GetNext(m_CurPos);
	else
		return NULL;
}

//	Get next environment scheme
CMLEnvScheme* CMLEnvSchemeSet::GetNextEnvScheme()
{
	if (!m_CurPos)
		return NULL;

	return m_SchemeList.GetNext(m_CurPos);
}


#endif	//	_ML_ENVSCHEMESET_H_



