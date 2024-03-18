/*
 * FILE: ML_MissionData.h
 *
 * DESCRIPTION: Mission data in game Malah
 *
 * CREATED BY: Duyuxin, 2003/3/25
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_MISSIONDATA_H_
#define _ML_MISSIONDATA_H_

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

//	Mission type
enum
{
	MISSION_KILLALL = 0,	//	Kill all
	MISSION_KILL,			//	Kill specified number of enemy
	MISSION_HOLDPOS,		//	Hold position
	MISSION_PRESERVE,		//	Preserve a area
	MISSION_STOPTRUCK,		//	Stop truck troop
	NUM_MISSIONTYPE,		//	Number of mission type
};

struct MLMISSIONDATA
{
	int		iType;			//	Mission type
	int		iTime;			//	Mission time (s)
	int		iNumFootMan;	//	Number of footman
	int		iNumGuerilla;	//	Number of guerilla
	int		iNumRocket;		//	Number of rocket man
	int		iNumTruck;		//	Number of truck
	int		iNumTank;		//	Number of tank
	int		iNumArmored;	//	Number of armored car
	int		iNumPlane;		//	Number of enemy plane
	int		iNumCopter;		//	Number of Helicopter
	float	fPreAreaRad;	//	Preserve area radius

	A3DVECTOR3	vPreAreaPos;	//	Preserve area center postion

	BYTE	aReserved[48];	//	Reserved
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLMissionData
//
///////////////////////////////////////////////////////////////////////////

class CMLMissionData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLMissionData)

public:		//	Constructor and Destructor

	CMLMissionData()
	{
		memset(&m_Data, 0, sizeof (m_Data));
	}

	virtual ~CMLMissionData() {}

public:		//	Attributes

public:		//	Operations

	void		SetMissionDesc(AUString strDesc)	{	m_strDesc = strDesc;	}
	AUString&	GetMissionDesc()					{	return m_strDesc;		}

	MLMISSIONDATA*	GetData()	{	return &m_Data;		}

	//	Load mission data
	virtual bool Load(AArchive& ar)
	{
		if (!AData::Load(ar))
			return false;

		if (!ar.Read(&m_Data, sizeof (m_Data)))
			return false;

		if (!ar.ReadString(m_strDesc))
			return false;

		return true;
	}

	//	Save mission data
	virtual bool Save(AArchive& ar)
	{
		if (!AData::Save(ar))
			return false;

		if (!ar.Write(&m_Data, sizeof (m_Data)))
			return false;

		if (!ar.WriteString(m_strDesc))
			return false;

		return true;
	}

protected:	//	Attributes

	AUString		m_strDesc;	//	Mission description
	MLMISSIONDATA	m_Data;		//	Mission data

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLMissionSet
//
///////////////////////////////////////////////////////////////////////////

class CMLMissionSet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLMissionSet)

public:		//	Constructor and Destructor

	CMLMissionSet();
	virtual ~CMLMissionSet();

public:		//	Attributes

public:		//	Operations

	virtual bool	Load(AArchive& ar);		//	Load mission set
	virtual bool	Save(AArchive& ar);		//	Save mission set

	CMLMissionData*	GetMissionData(int i)
	{
		if (i >= 0 && i < NUM_GAMELEVEL)
			return m_aMissions[i];
		else
			return NULL;
	}

protected:	//	Attributes

	CMLMissionData*	m_aMissions[NUM_GAMELEVEL];

protected:	//	Operations

};


#endif	//	_ML_MISSIONDATA_H_
