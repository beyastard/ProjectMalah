/*
 * FILE: PreDeclare.h
 *
 * DESCRIPTION: �ࡢ�ṹ��ö������Ԥ����
 *
 * CREATED BY: ��Զ��, 2002/8/9
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */
#ifndef __PREDECLARE__H__
#define __PREDECLARE__H__

#include <windows.h>

struct AMSG;
typedef AMSG * PAMSG;

struct tagCREATEFRMSTRUCT;
typedef tagCREATEFRMSTRUCT  CREATEFRMSTRUCT, *LPCREATEFRMSTRUCT;

struct tagINITAGAMESTRUCT;
typedef tagINITAGAMESTRUCT INITAGAMESTRUCT,*PINITAGAMESTRUCT;

#endif  // __PREDECLARE__H__