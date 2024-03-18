/*
 * FILE: ML_ManagerDef.h
 *
 * DESCRIPTION: Definations for managers in game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_MANAGERDEF_H_
#define _ML_MANAGERDEF_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Manager's ID
#define MANID_MESSAGE		0	//	Message manager
#define MANID_PLAYER		1	//	Player manager
#define MANID_AI			2	//	AI manager
#define MANID_ROUTE			3	//	Route manager
#define MANID_BULLET		4	//	Bullet manager
#define MANID_IMAGE			5	//	Image manager
#define MANID_OBJECT		6	//	Object manager
#define NUM_MANAGER			7	//	Number of manager

#define MANID_FIRSTONE		MANID_MESSAGE
#define ISMANAGERID(i)		((i) >= MANID_FIRSTONE && (i) < MANID_FIRSTONE + NUM_MANAGER)

//	Manager's mask
#define MASK_MESSAGEMAN		(1 << MANID_MESSAGE)
#define MASK_PLAYERMAN		(1 << MANID_PLAYER)
#define MASK_AIMAN			(1 << MANID_AI)
#define MASK_ROUTEMAN		(1 << MANID_ROUTE)
#define MASK_BULLETMAN		(1 << MANID_BULLET)
#define MASK_IMAGEMAN		(1 << MANID_IMAGE)
#define MASK_OBJECTMAN		(1 << MANID_OBJECT)

//	Translator bind type
#define TBT_NONE			0	//	None
#define TBT_ONLYKEYBOARD	1	//	Only bind keyboard translator
#define TBT_ONLYMOUSE		2	//	Only bind mouse translator
#define TBT_MOUSEANDKB		3	//	Bind both keyboard and mouse translator	

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
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_MANAGERDEF_H_



