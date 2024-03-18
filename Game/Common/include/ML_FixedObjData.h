/*
 * FILE: ML_FixedObjData.h
 *
 * DESCRIPTION: Fixed object data in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/13
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_FIXEDOBJDATA_H_
#define _ML_FIXEDOBJDATA_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define FOF_DESTROYABLE		0x0001	//	Can be destroyed

//	Fixed object's strategy
enum
{
	FO_RUIN_ONLYSHOW = 0,	//	Only show but not do collision
	FO_RUIN_SHOWANDBLOCK,	//	Show and do collision
};

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

struct MLFIXEDOBJECTDATA
{
	A3DVECTOR3	vPos;		//	Object's position
	A3DVECTOR3	vDir;		//	Object's direction
	A3DVECTOR3	vUp;		//	Object's up direction
	DWORD		dwFlags;	//	Flags
	int			iLife;		//	Life value
	int			iDestroyed;	//	Strategy after destroyed
	int			iRuinTime;	//	Ruin remain time (ms)

	BYTE		aReserved[64];	//	Reserved bytes
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLFixedObjData
//
///////////////////////////////////////////////////////////////////////////

class CMLFixedObjData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLFixedObjData)

public:		//	Constructor and Destructor

	CMLFixedObjData();
	virtual ~CMLFixedObjData() {}

public:		//	Attributes

	MLFIXEDOBJECTDATA	m_Data;

	AUString	m_szModelFile;		//	Model file's name
	AUString	m_szRuinFile;		//	Ruin model file's name

public:		//	Operations

	virtual bool	Load(AArchive& ar);		//	Load route
	virtual bool	Save(AArchive& ar);		//	Save route

protected:	//	Attributes

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_ROUTEDATA_H_

