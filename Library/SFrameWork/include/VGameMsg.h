/*
 * FILE: VGameMsg.h
 *
 * DESCRIPTION: common message in game
 *
 * CREATED BY: Hedi, 2002/8/2
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VGAMEMSG_H_
#define _VGAMEMSG_H_

typedef struct _GAMEMSG
{
	DWORD		dwMsgID;

	DWORD		dwParam1;
	DWORD		dwParam2;
} GAMEMSG, * PGAMEMSG;

// Message id definition;
#define GAMEMSG_COMMONMSG	0x00001000
#define GAMEMSG_RESTART		(GAMEMSG_COMMONMSG + 1)
#define GAMEMSG_RESIZE		(GAMEMSG_COMMONMSG + 2)
#define GAMEMSG_EXIT		(GAMEMSG_COMMONMSG + 3)
#define GAMEMSG_HIDECURSOR	(GAMEMSG_COMMONMSG + 4)
#define GAMEMSG_SHOWCURSOR	(GAMEMSG_COMMONMSG + 5)

#define GAMEMSG_SPECMSG		0x10000000

#endif
