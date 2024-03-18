/*
 * FILE: ML_MsgDef.h
 *
 * DESCRIPTION: Message definitions for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _ML_MSGDEF_H_
#define _ML_MSGDEF_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define MSG_EXITGAME		100		//	Exit game, p1 - exit code, 
#define MSG_INTERFACE		101		//	Interface message. p1 - window ID. p2 - game status
#define MSG_MISSIONBRIEF	102		//	Trigger mission brief

//	Player interrelated messages
#define MSG_P_PITCH			200		//	Pitch, p1 - degree
#define MSG_P_YAW			201		//	Yaw, p1 - degree
#define MSG_P_FIRE			202		//	Player fire
#define MSG_P_FIRESTOP		203		//	Player fire stop
#define MSG_P_OPENFUNC		204		//	Open weapon special function
#define MSG_P_CHANGEWEAPON	205		//	Change weapon
#define MSG_P_CALLSUPPORT	206		//	Call cannon support, p1: support ID

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

//	Message struct
typedef struct _MLMSG
{
	DWORD		dwMsg;			//	Message code
	int			idManager;		//	Receiver manager's ID
	int			idObject;		//	Receiver object's ID
	DWORD		dwParam1;		//	Parameter 1
	DWORD		dwParam2;		//	Parameter 2
	DWORD		dwParam3;		//	Parameter 3
	DWORD		dwFrameCnt;		//	Frame count

} MLMSG, *PMLMSG;

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


#endif	//	_ML_MSGDEF_H_

