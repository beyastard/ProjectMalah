/*
 * FILE: ML_RouteData.h
 *
 * DESCRIPTION: Route data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ROUTEDATA_H_
#define _ML_ROUTEDATA_H_

#include "ML_EventPtData.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Flags of route to identify whether they have their own data
#define RTF_OWN_AIMS_PRECISE	0x0001	//	Own AI move shot precise
#define RTF_OWN_AIMS_INTEVAL	0x0002	//	Own AI move shot inteval time
#define RTF_OWN_AISS_PRECISE	0x0004	//	Own AI stand shot precise
#define RTF_OWN_AISS_INTEVAL	0x0008	//	Own AI stand shot inteval time
#define RTF_OWN_AICS_PRECISE	0x0010	//	Own AI creep shot precise
#define RTF_OWN_AICS_INTEVAL	0x0020	//	Own AI creep shot inteval time
#define RTF_OWN_AIGENINTERVAL	0x0040	//	Own AI generate interval
#define RTF_OWN_AIGENNUMBER		0x0080	//	Own AI generate number
#define RTF_OWN_AIGENTURN		0x0100	//	Own AI generate turn number
#define RTF_OWN_AIGENTIME		0x0200	//	Own AI generate time
#define RTF_OWN_AIMAXNUMBER		0x0400	//	Maximum number of AI that can appear on route at same time
#define RTF_OWN_COPTERINTERVAL	0x0800	//	Own copter appear interval

//	Flags of route to identify whether they don't use fixed value
#define RTF_NFIX_AIMS_PRECISE	0x0001	//	Not fixed AI move shot precise
#define RTF_NFIX_AIMS_INTEVAL	0x0002	//	Not fixed AI move shot inteval time
#define RTF_NFIX_AISS_PRECISE	0x0004	//	Not fixed AI stand shot precise
#define RTF_NFIX_AISS_INTEVAL	0x0008	//	Not fixed AI stand shot inteval time
#define RTF_NFIX_AICS_PRECISE	0x0010	//	Not fixed AI creep shot precise
#define RTF_NFIX_AICS_INTEVAL	0x0020	//	Not fixed AI creep shot inteval time
#define RTF_NFIX_AIGENINTERVAL	0x0040	//	Not fixed AI generate interval
#define RTF_NFIX_AIGENNUMBER	0x0080	//	Not fixed AI generate number
#define RTF_NFIX_AIGENTURN		0x0100	//	Not fixed AI generate turn number
#define RTF_NFIX_AIGENTIME		0x0200	//	Not fixed AI generate time
#define RTF_NFIX_AIMAXNUMBER	0x0400	//	Maximum number of AI that can appear on route at same time

#define RTF_MASK_AI				0x07ff	//	Mask of AI's data

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

struct MLROUTEDATA
{
	int		iUserID;			//	Route user's ID
	DWORD	dwOwnFlags;			//	Route own data flags
	DWORD	dwNotFixFlags;		//	Route use not-fixed value flags
	int		iAIMSPrecise;		//	AI move shot precise
	int		iAIMSInterval;		//	AI move shot interval
	int		iAISSPrecise;		//	AI stand shot precise
	int		iAISSInterval;		//	AI stand shot interval
	int		iAICSPrecise;		//	AI creep shot precise
	int		iAICSInterval;		//	AI creep shot interval
	int		iAIGenInterval;		//	AI generate interval
	int		iAIGenNum;			//	AI generate number
	int		iAIGenTurn;			//	AI generate turn number
	int		iAIGenTime;			//	AI generate time
	int		iAIMaxNum;			//	Maximum number of AI that can appear on route at same time
	int		iCopterInterval;	//	Copter appear interval
	int		iCopterTurn;		//	Copter appear turn number

	//	Below properties only used by editor
	int		iClassID;			//	Class ID
	bool	bLocked;			//	Locked flag
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLRouteData
//
///////////////////////////////////////////////////////////////////////////

class CMLRouteData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLRouteData)

public:		//	Constructor and Destructor

	CMLRouteData();
	virtual ~CMLRouteData() {}

public:		//	Attributes

	MLROUTEDATA		m_Data;		//	Data
	
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



