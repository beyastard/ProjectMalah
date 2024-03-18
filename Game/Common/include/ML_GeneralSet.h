/*
 * FILE: ML_GeneralSet.h
 *
 * DESCRIPTION: All general data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_GENERALSET_H_
#define _ML_GENERALSET_H_

#include "ML_EnvSet.h"
#include "ML_SkySet.h"
#include "ML_ActorSet.h"
#include "ML_AISet.h"
#include "ML_EnvSchemeSet.h"
#include "ML_MissionData.h"

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
//	Class CMLGeneralSet
//
///////////////////////////////////////////////////////////////////////////

class CMLGeneralSet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLGeneralSet)

public:		//	Constructor and Destructor

	CMLGeneralSet();
	virtual ~CMLGeneralSet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	bool			Load(const char* szFile);	//	Load general set from file
	bool			Save(const char* szFile);	//	Save general set to file
	virtual bool	Load(AArchive& ar);			//	Load general set
	virtual bool	Save(AArchive& ar);			//	Save general set

	CMLSkySet*			GetSkyDataSet()		{	return &m_SkySet;		}
	CMLEnvSet*			GetEnvDataSet()		{	return &m_EnvSet;		}
	CMLActorSet*		GetActorDataSet()	{	return &m_ActorSet;		}
	CMLAISet*			GetAIDataSet()		{	return &m_AISet;		}
	CMLEnvSchemeSet*	GetEnvSchemeSet()	{	return &m_EnvSchemeSet;	}
	CMLMissionSet*		GetMissionSet()		{	return &m_MissionSet;	}

protected:	//	Attributes

	CMLSkySet		m_SkySet;		//	Sky data set
	CMLEnvSet		m_EnvSet;		//	Environment data set
	CMLEnvSchemeSet	m_EnvSchemeSet;	//	Environment scheme set
	CMLActorSet		m_ActorSet;		//	Actor (player) data set
	CMLAISet		m_AISet;		//	AI data set
	CMLMissionSet	m_MissionSet;	//	Mission data set

protected:	//	Operations

	static DWORD	GetDataFlag()	{	return ('M'<<24) | ('L'<<16) | ('G'<<8) | 'S';	}
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_GENERALSET_H_



