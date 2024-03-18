/*
 * FILE: ML_MapData.h
 *
 * DESCRIPTION: Map data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/29
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_MAPDATA_H_
#define _ML_MAPDATA_H_

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
//	Class CMLMapData
//
///////////////////////////////////////////////////////////////////////////

class CMLMapData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLMapData)

public:		//	Constructor and Destructor

	CMLMapData();
	virtual ~CMLMapData() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	bool			Load(const char* szFile);	//	Load map data from file
	bool			Save(const char* szFile);	//	Save map data to file
	virtual bool	Load(AArchive& ar);			//	Load map data
	virtual bool	Save(AArchive& ar);			//	Save map data

	CMLSceneData*	GetSceneData()		{	return &m_SceneData;	}
	A3DVECTOR3		GetActorPos()		{	return m_vActorPos;		}
	A3DVECTOR3		GetActorDir()		{	return m_vActorDir;		}

	void			SetActorPos(A3DVECTOR3& vPos)	{	m_vActorPos = vPos;		}
	void			SetActorDir(A3DVECTOR3& vDir)	{	m_vActorDir = vDir;		}

protected:	//	Attributes

	CMLSceneData	m_SceneData;	//	Scene data
	A3DVECTOR3		m_vActorPos;	//	Player's position
	A3DVECTOR3		m_vActorDir;	//	Player's direction

protected:	//	Operations

	static DWORD	GetDataFlag()	{	return ('M'<<24) | ('L'<<16) | ('M'<<8) | 'D';	}
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_MAPDATA_H_



