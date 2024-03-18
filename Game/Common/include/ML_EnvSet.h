/*
 * FILE: ML_EnvSet.h
 *
 * DESCRIPTION: Environment data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/15
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ENVSET_H_
#define _ML_ENVSET_H_

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
//	Class CMLSkySet
//
///////////////////////////////////////////////////////////////////////////

class CMLEnvSet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLEnvSet)

public:		//	Constructor and Destructor

	CMLEnvSet();
	virtual ~CMLEnvSet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	virtual bool	Load(AArchive& ar);		//	Load environment set
	virtual bool	Save(AArchive& ar);		//	Save environment set

	void		AddEnv(CMLEnvironment* pEnv);		//	Add a environment
	void		DelEnv(CMLEnvironment* pEnv);		//	Delete a environment

	CMLEnvironment*	SearchEnv(AUString& strName);	//	Search a environment by name
	CMLEnvironment*	SearchEnv(int iIndex);			//	Search a environment by index

	int			GetEnvNum()		{	return m_EnvList.GetCount();	}

	inline CMLEnvironment*	GetFirstEnv();	//	Get first environment
	inline CMLEnvironment*	GetNextEnv();	//	Get next environment

protected:	//	Attributes

	AListPosition	m_CurPos;		//	current position

	AListT<CMLEnvironment*>	m_EnvList;		//	Environment list
	
protected:	//	Operations

	void		ReleaseAllEnvironment();	//	Release all environment
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Get first environment
CMLEnvironment* CMLEnvSet::GetFirstEnv()
{
	m_CurPos = m_EnvList.GetHeadPosition();
	if (m_CurPos)
		return m_EnvList.GetNext(m_CurPos);
	else
		return NULL;
}

//	Get next environment
CMLEnvironment* CMLEnvSet::GetNextEnv()
{
	if (!m_CurPos)
		return NULL;

	return m_EnvList.GetNext(m_CurPos);
}


#endif	//	_ML_ENVSET_H_



