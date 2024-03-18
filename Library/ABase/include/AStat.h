/*
 * FILE: AStat.h
 *
 * DESCRIPTION: ��Ϸ��ͳ����
 *
 * CREATED BY: ��Զ��, 2002/8/9
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */
#ifndef __ASTAT__H__
#define __ASTAT__H__

#include "AObjectIDMan.h"
#include <assert.h>
#include "AIDObject.h"
#include <string.h>
#include "AUList.h"

struct TU_S 
{
public:
	int			id;
	bool		bReset;
	unsigned long preTime;		//ǰһ��timegettime�õ���ʱ�䣬��Ϊnew�ȽϷ�ʱ������û�ж�����ջ�ṹ
	unsigned long useTime;
	unsigned long totalTime;
	unsigned long maxTime;
	unsigned long minTime;
	char prompt[128];

};


class AStat : public AObjectIDMan
{
protected:
	AListT<TU_S*>	m_itemStack;	//�ڵ��ջ
public:
	AStat();
	virtual ~AStat();
	int		m_num;
	void BeginStatItem(TU_S * pItem);
	void EndStatItem();
protected:
	virtual int OnGetID(AIDObject * pAObject);
	virtual bool OnRelease(AIDObject *__pAIDObject);

};

extern AStat g_stat;

//#define USEDEBUGSTAT

#ifdef	USEDEBUGSTAT
	#define BEGIN_STAT(PROMPT)	{	\
									static int pos = -1; \
									if(pos==-1)	\
									{	\
										g_stat.m_num ++ ; \
										pos = g_stat.m_num;	\
									}	\
									TU_S * pStatItem = (TU_S*)(g_stat.GetPtrByID(pos));	\
									if(pStatItem==0) \
									{	\
										pStatItem = new TU_S;	\
										memset(pStatItem,0,sizeof(TU_S)); \
										pStatItem->id = pos;	\
										pStatItem->minTime	= 0xffffffff;	\
										g_stat.AddObject((AIDObject*)pStatItem); \
									}	\
									else \
									{	\
										if(pStatItem->id != pos) \
											assert(0); \
									}	\
									strcpy(pStatItem->prompt,PROMPT);	\
									g_stat.BeginStatItem(pStatItem);	\
								}
	#define END_STAT()				g_stat.EndStatItem();
#else
	#define BEGIN_STAT(PROMPT)	
	#define END_STAT()
#endif

#endif // __ASTAT__H__
