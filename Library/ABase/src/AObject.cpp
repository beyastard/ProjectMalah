/*
 * FILE: AObject.h
 *
 * DESCRIPTION: AObject�ķ���ʵ��
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#include "AObject.h"
#include <assert.h>

AClassInfo AObject::m_classAObject(CLASSNAME(AObject), AObject::CreateObject, NULL, 1);

AObject::AObject()
{
}

AObject::~AObject()
{
}

AObject * AObject::CreateObject()
{
	return new AObject();
}

AClassInfo * AObject::GetClassInfo()
{
	return &m_classAObject;
}

bool AObject::Load(AArchive &ar)
{
	int nVersion;
	if(ar.Read(&nVersion,sizeof(nVersion))==false)
	{
		assert(0);
		return false;
	}
	//��������汾�Ž��а汾����
	return true;
}

bool AObject::Save(AArchive &ar)
{
	int nVersion = AObject::GetClassInfo()->GetVersion();
	if(ar.Write(&nVersion,sizeof(nVersion))==false)
	{
		assert(0);
		return false;
	}
	return true;
}

// �����Ƿ�������ĳ��
bool AObject::IsKindOf(AClassInfo * pClassInfo)
{
	if(!pClassInfo) return false;
	AClassInfo * pInfo = GetClassInfo();

	while(pInfo)
	{
		if(pInfo == pClassInfo) return true;
		pInfo = pInfo->GetBaseClass();
	}

	return false;
}