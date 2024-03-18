/*
 * FILE: ML_LevelData.h
 *
 * DESCRIPTION: classes for level data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_LEVELDATA_H_
#define _ML_LEVELDATA_H_

#include "ML_ID.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Type of level data
#define LDT_UNKNOWN		0
#define LDT_INTEGER		1
#define LDT_FLOAT		2

//	Edit mode of level data
#define LDEM_NATURAL	0		//	Natural mode
#define LDEM_CURVE		1		//	Curve mode
#define LDEM_LINE		2		//	Line mode

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
//	Class CMLLevelData
//
///////////////////////////////////////////////////////////////////////////

class CMLLevelData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLLevelData)

public:		//	Constructor and Destructor

	CMLLevelData()
	{	
		SetId(ID_LEVELDATA);

		m_iDataType		= LDT_UNKNOWN;
		m_iEditMode		= LDEM_NATURAL;
		m_aCtrlPts[0]	= 0.0f;
		m_aCtrlPts[0]	= 50.0f;
		m_aCtrlPts[1]	= NUM_GAMELEVEL / 3.0f;
		m_aCtrlPts[1]	= 50.0f;
		m_aCtrlPts[2]	= NUM_GAMELEVEL * 2.0f / 3.0f;
		m_aCtrlPts[2]	= 50.0f;
		m_aCtrlPts[3]	= NUM_GAMELEVEL - 1.0f;
		m_aCtrlPts[3]	= 0.0f;
	}

	virtual ~CMLLevelData() {}

public:		//	Attributes

public:		//	Operations

	int		GetDataType()			{	return m_iDataType;		}
	int		GetEditMode()			{	return m_iEditMode;		}
	void	SetEditMode(int iMode)	{	m_iEditMode = iMode;	}

	//	Get control points
	void GetCtrlPts(int i, float* px, float* py)
	{	
		assert(i >= 0 && i < 4);
		*px = m_aCtrlPts[i << 1];
		*py = m_aCtrlPts[(i << 1) + 1];
	}

	//	Set control points
	void SetCtrlPts(int i, float x, float y)
	{
		assert(i >= 0 && i < 4);
		m_aCtrlPts[i << 1] = x;
		m_aCtrlPts[(i << 1) + 1] = y;
	}

	//	Load data
	bool Load(AArchive& ar)
	{
		//	Don't do below operation, this will change level data's name
	//	if (!AData::Load(ar))
	//		return false;

		//	TODO: do version control

		if (!ar.Read(&m_iEditMode, sizeof (m_iEditMode)))
			return false;
		
		if (!ar.Read(m_aCtrlPts, sizeof (m_aCtrlPts)))
			return false;

		return true;
	}

	//	Save data
	bool Save(AArchive& ar)
	{
		//	Don't do below operation, this will change level data's name
	//	if (!AData::Save(ar))
	//		return false;
		
		//	TODO: do version control

		if (!ar.Write(&m_iEditMode, sizeof (m_iEditMode)))
			return false;
		
		if (!ar.Write(m_aCtrlPts, sizeof (m_aCtrlPts)))
			return false;

		return true;
	}

protected:	//	Attributes

	int		m_iDataType;		//	Data type
	int		m_iEditMode;		//	Edit mode
	float	m_aCtrlPts[8];		//	Control points

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLLevelDataInt
//
///////////////////////////////////////////////////////////////////////////

class CMLLevelDataInt : public CMLLevelData
{
public:		//	Types

	DECLARE_ASERIAL(CMLLevelDataInt)

public:		//	Constructor and Destructor

	CMLLevelDataInt()
	{
		memset(m_aData, 0, sizeof (m_aData));

		m_iDataType	= LDT_INTEGER;
		m_iMin		= 0;
		m_iMax		= 100;
	}

	virtual ~CMLLevelDataInt() {}

public:		//	Attributes

public:		//	Operations

	//	Get a item
	int	GetAt(int i)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		return m_aData[i];
	}

	//	Set a item
	void SetAt(int i, int d)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		if (d < m_iMin)
			m_aData[i] = m_iMin;
		else if (d > m_iMax)
			m_aData[i] = m_iMax;
		else
			m_aData[i] = d;
	}

	//	Get a item
	int	operator [] (int i)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		return m_aData[i];
	}

	int		GetMinValue()			{	return m_iMin;	}
	int		GetMaxValue()			{	return m_iMax;	}
	void	SetMinValue(int iMin)	{	m_iMin = iMin;	}
	void	SetMaxValue(int iMax)	{	m_iMax = iMax;	}

	//	Load data
	bool Load(AArchive& ar)
	{
		if (!CMLLevelData::Load(ar))
			return false;

		//	TODO: do version control

		if (!ar.Read(m_aData, sizeof (m_aData)))
			return false;
		
		if (!ar.Read(&m_iMin, sizeof (m_iMin)))
			return false;

		if (!ar.Read(&m_iMax, sizeof (m_iMax)))
			return false;

		return true;
	}

	//	Save data
	bool Save(AArchive& ar)
	{
		if (!CMLLevelData::Save(ar))
			return false;
		
		//	TODO: do version control

		if (!ar.Write(m_aData, sizeof (m_aData)))
			return false;
		
		if (!ar.Write(&m_iMin, sizeof (m_iMin)))
			return false;

		if (!ar.Write(&m_iMax, sizeof (m_iMax)))
			return false;

		return true;
	}

protected:	//	Attributes

	int		m_aData[NUM_GAMELEVEL];		//	Data table
	int		m_iMax;						//	Limit value
	int		m_iMin;

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLLevelDataFloat
//
///////////////////////////////////////////////////////////////////////////

class CMLLevelDataFloat : public CMLLevelData
{
public:		//	Types

	DECLARE_ASERIAL(CMLLevelDataFloat)

public:		//	Constructor and Destructor

	CMLLevelDataFloat()
	{	
		memset(m_aData, 0, sizeof (m_aData));

		m_iDataType	= LDT_FLOAT;
		m_fMin		= 0.0f;
		m_fMax		= 100.0f;
	}

	virtual ~CMLLevelDataFloat() {}

public:		//	Attributes

public:		//	Operations

	//	Get a item
	float GetAt(int i)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		return m_aData[i];
	}

	//	Set a item
	void SetAt(int i, float d)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		if (d < m_fMin)
			m_aData[i] = m_fMin;
		else if (d > m_fMax)
			m_aData[i] = m_fMax;
		else
			m_aData[i] = d;
	}

	//	Get a item
	float operator [] (int i)
	{
		assert(i >= 0 && i < NUM_GAMELEVEL);
		return m_aData[i];
	}

	float	GetMinValue()			{	return m_fMin;	}
	float	GetMaxValue()			{	return m_fMax;	}
	void	SetMinValue(float fMin)	{	m_fMin = fMin;	}
	void	SetMaxValue(float fMax)	{	m_fMax = fMax;	}

	//	Load data
	bool Load(AArchive& ar)
	{
		if (!CMLLevelData::Load(ar))
			return false;

		//	TODO: do version control

		if (!ar.Read(m_aData, sizeof (m_aData)))
			return false;

		if (!ar.Read(&m_fMin, sizeof (m_fMin)))
			return false;

		if (!ar.Read(&m_fMax, sizeof (m_fMax)))
			return false;
		
		return true;
	}

	//	Save data
	bool Save(AArchive& ar)
	{
		if (!CMLLevelData::Save(ar))
			return false;

		//	TODO: do version control
	
		if (!ar.Write(m_aData, sizeof (m_aData)))
			return false;
		
		if (!ar.Write(&m_fMin, sizeof (m_fMin)))
			return false;

		if (!ar.Write(&m_fMax, sizeof (m_fMax)))
			return false;

		return true;
	}

protected:	//	Attributes

	float	m_aData[NUM_GAMELEVEL];		//	Data table
	float	m_fMax;						//	Limit value
	float	m_fMin;

protected:	//	Operations

};


#endif	//	_ML_LEVELDATA_H_



