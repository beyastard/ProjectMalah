/*
 * FILE: AClassInfo.h
 *
 * DESCRIPTION: ������������AObjectʵ�ִ��л���������AClassInfo
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef _ACLASSINFO_H__
#define _ACLASSINFO_H__

#include "AList.h"

class AObject;
typedef AObject * (* CREATEMETHOD)(void);

class AClassInfo  
{
public:
	int GetVersion();
	CREATEMETHOD GetCreateMethod();
	AClassInfo * GetBaseClass();
	const char * GetClassName();
	AClassInfo(const char * pcszName, CREATEMETHOD pfCreate, AClassInfo * pBase, int iVersion);
	~AClassInfo();
	static AClassInfo * GetClassInfo(const char * pszClass);

private:

	const char *	m_pcszName;		//����
	AClassInfo *	m_pciBase;		//������Ϣ
	int				m_iVersion;		//�മ�л��汾
	CREATEMETHOD	m_pfCreate;		//��������ĺ���ָ��

	AClassInfo *	m_pciPrev;		//��һ������Ϣ
	AClassInfo *	m_pciNext;		//��һ������Ϣ
};

#endif
