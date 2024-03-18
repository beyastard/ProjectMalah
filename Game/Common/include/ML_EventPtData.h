/*
 * FILE: ML_EventPtData.h
 *
 * DESCRIPTION: Event point data in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _ML_EVENTPTDATA_H_
#define _ML_EVENTPTDATA_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Type of event point
#define EPT_ROUND		0
#define EPT_ARC			1
#define EPT_LINE		2
#define EPT_DBARC		3		//	Double arc

#define EPE_NONE				0		//	None

//	AI events on event point
#define EPE_AI_STANDSHOT		10		//	Stand shot of footman
#define EPE_AI_CREEPSHOT		11		//	Creep shot of footman
#define EPE_AI_SHIFTSHOT		12		//	Shift shot of footman
#define EPE_AI_USEMOXMODEL		13		//	Use mox model
#define EPE_AI_JUMPFORWARD		14		//	Jump forward
#define EPE_AI_STAB				15		//	Stab
#define EPE_AI_STAND			16		//	Stand
#define EPE_AI_CREEP			17		//	Creep
#define EPE_AI_STANDSHOT_R		18		//	Stand shot of rocket man
#define EPE_AI_CREEPSHOT_R		19		//	Creep shot of rocket man
#define EPE_AI_SHIFTSHOT_R		20		//	Shift shot of rocket man
#define NUM_AIEPEVENT			11

#define FIRSTAIEPEVENT			EPE_AI_STANDSHOT
#define ISAIEPEVENT(i)			((i) >= FIRSTAIEPEVENT && ((i) < FIRSTAIEPEVENT + NUM_AIEPEVENT))

//	Copter events on event point
#define EPE_CT_ENTERSUPPLY		60		//	Enter supply area
#define EPE_CT_SUPPLY			61		//	Supply (for helicopter)
#define EPE_CT_LEAVEBATTLE		62		//	Leave battle
#define NUM_COPTEREPEVENT		3

#define FIRSTCOPTEREPEVENT		EPE_CT_ENTERSUPPLY
#define ISCOPTEREPEVENT(i)		((i) >= FIRSTCOPTEREPEVENT && ((i) < FIRSTCOPTEREPEVENT + NUM_COPTEREPEVENT))

//	Misc events on event point
#define EPE_MISC_BLOCKED		80		//	Blocked area
#define EPE_MISC_JUMPFORWARD	81		//	Jump area
#define NUM_MISCEPEVENT			2

#define FIRSTMISCEPEVENT		EPE_MISC_BLOCKED
#define ISMISCEPEVENT(i)		((i) >= FIRSTMISCEPEVENT && ((i) < FIRSTMISCEPEVENT + NUM_MISCEPEVENT))

//	Events on AI vehicle route
#define EPE_AIV_BEGINFIRE		90		//	Begin fire (Enter fire are)
#define EPE_AIV_LEAVE			91		//	Leave battle
#define NUM_AIVEPEVENT			2

#define FIRSTAIVEPEVENT			EPE_AIV_BEGINFIRE
#define ISAIVEPEVENT(i)			((i) >= FIRSTAIVEPEVENT && ((i) < FIRSTAIVEPEVENT + NUM_AIVEPEVENT))

//	Maximum number of event which can be hold by a event point
#define MAXNUM_EVENTPTEVENT		12

//	Flags of event point to identify whether they have their own data
#define EPF_OWN_ATTACKCOPTER	0x0001	//	Own rocket shot possibility

//	Flags of event point to identify whether they don't use fixed value
#define EPF_NFIX_ATTACKCOPTER	0x0001	//	Not fixed rocket shot possibility

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

//	Event on event point
struct MLEVENTPTEVENT
{
	int			iEvent;		//	Event ID
	int			iTime;		//	Event time
};

struct MLEVENTPTDATA
{
	int			iType;			//	Type of event point
	DWORD		dwOwnFlags;		//	Route own data flags
	DWORD		dwNotFixFlags;	//	Route use not-fixed value flags
	A3DVECTOR3	vPos;			//	Position
	bool		bRushTo;		//	true, rush to next point
	int			iNumEvent;		//	Event number
	int			iNumLoop;		//	Loop time, -1 means loop forever

	int			iAkCopterPoss;	//	Possible of rocket man to attack copter

	MLEVENTPTEVENT	aEvents[MAXNUM_EVENTPTEVENT];	//	Event array
	
	//	Used only by round type
	float		fRadius;		//	Radius of round

	//	Used only by arc and double-arc type
	A3DVECTOR3	vCenter;		//	Arc's center
	float		fDegree;		//	degrees of arc in 
	float		fHalfWidth;		//	double-arc's half width

	//	Used only by line type
	A3DVECTOR3	vLineEnd;		//	Line end
	A3DVECTOR3	vLineDir;		//	Line direction
	float		fLineLen;		//	Line length
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEventPtData
//
///////////////////////////////////////////////////////////////////////////

class CMLEventPtData : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLEventPtData)

public:		//	Constructor and Destructor

	CMLEventPtData();
	virtual ~CMLEventPtData() {}

public:		//	Attributes

	MLEVENTPTDATA	m_Data;		//	Data

public:		//	Operations

	virtual bool	Load(AArchive& ar);		//	Load event point
	virtual bool	Save(AArchive& ar);		//	Save event point

protected:	//	Attributes

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_EVENTPTDATA_H_

