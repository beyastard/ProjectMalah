/*
 * FILE: ML_FixedObject.h
 *
 * DESCRIPTION: Fixed object class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_FIXEDOBJECT_H_
#define _ML_FIXEDOBJECT_H_

#include "ML_ModelUnit.h"
#include "ML_FixedObjData.h"

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

class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLFixedObject
//
///////////////////////////////////////////////////////////////////////////

class CMLFixedObject : public CMLModelUnit
{
public:		//	Types

	//	States
	enum
	{
		ST_NORMAL = 0,		//	Normal
		ST_DESTROYED,		//	Destroyed
	};

public:		//	Constructor and Destructor

	CMLFixedObject();
	virtual ~CMLFixedObject() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();
	virtual bool	Release();//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

	virtual bool	Load(AArchive& ar);		//	Load data
	virtual bool	Save(AArchive& ar);		//	Save data

	bool	RayTrace(MLRAYTRACE* pTrace);	//	Ray trace function
	bool	Explode(CMLBullet* pBullet, bool bFire);	//	Explode
	void	Strike(CMLBullet* pBullet);		//	Strike
	void	Reset();	//	Reset object

	bool	CanBeDestroy()	{	return (m_MLFixedObj.m_Data.dwFlags & FOF_DESTROYABLE) ? true : false;	}

	MLFIXEDOBJECTDATA*	GetData()	{	return &m_MLFixedObj.m_Data;	}

protected:	//	Attributes

	CMLFixedObjData	m_MLFixedObj;	//	Object data

	int			m_iLife;		//	Life counter
	int			m_iState;		//	State
	int			m_iWaitTime;	//	Wait time
	int			m_iWaitCnt;		//	Wait time counter

protected:	//	Operations

	void	ReleaseCurModel();		//	Release current model
	void	SetState(int iState);	//	Set state
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_FIXEDOBJECT_H_

