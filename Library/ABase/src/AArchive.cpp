/*
 * FILE: AArchive.cpp
 *
 * DESCRIPTION: ���л�������AArchive�ķ���ʵ��
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#include "AArchive.h"
#include "AObject.h"
#include <assert.h>
#include <string.h>

AArchive::AArchive(AStream * pStream, bool bSave)
{
	assert(pStream);
	m_pStream = pStream;
	m_bMode = bSave;
}

AArchive::~AArchive()
{

}

bool AArchive::IsLoading()
{
	return !m_bMode;
}

bool AArchive::IsStoring()
{
	return m_bMode;
}

bool AArchive::Read(void *buff, int size)
{
	assert(m_pStream);
	return m_pStream->Read(buff, size);
}

bool AArchive::Write(const void *buff, int size)
{
	assert(m_pStream);
	return m_pStream->Write(buff, size);
}

AObject * AArchive::CreateObject()
{
	AObject * pObject = NULL;

	AClassInfo * pClassInfo = ReadClassInfo();
	assert(pClassInfo);

	// ������Ϣ���������
	CREATEMETHOD pfCreateObject = pClassInfo->GetCreateMethod();
	assert(pfCreateObject);
	if(!pfCreateObject)
	{
		return NULL;
	}

	// �����¶���
	pObject = pfCreateObject();
	if(!pObject)
	{
		return NULL;
	}

	return pObject;
}

bool AArchive::WriteClassInfo(AObject *pObject)
{
	assert(pObject);
	if(!pObject)
	{
		return false;
	}

	AClassInfo * pClassInfo = pObject->GetClassInfo();
	assert(pObject);
	if(!pClassInfo)
	{
		return false;
	}

	return WriteClassInfo(pClassInfo);
}

bool AArchive::WriteClassInfo(AClassInfo *pClassInfo)
{
	assert(pClassInfo);

	const char * pszClassName = pClassInfo->GetClassName();
	assert(pszClassName);
	if(!pszClassName)
	{
		return false;
	}

	AClassInfo * pValidClass = AClassInfo::GetClassInfo(pszClassName);
	assert(pValidClass);

	int namelen = strlen(pszClassName);
	assert(namelen > 0);
	if(namelen <= 0)
	{
		return false;
	}

	// ��������
	if(!Write(&namelen, sizeof(namelen)))
	{
		return false;
	}

	// ����
	if(!Write(pszClassName, namelen))
	{
		return false;
	}

	// �汾
	int iVersion = pClassInfo->GetVersion();
	if(!Write(&iVersion, sizeof(iVersion)))
	{
		return false;
	}

	return true;
}

AClassInfo * AArchive::ReadClassInfo()
{
	// �����Ƴ���
	char * pszName = NULL;

	int namelen = 0;
	if(!Read(&namelen, sizeof(namelen)))
	{
		return NULL;
	}

	// ���ȱ��������
	if(namelen <= 0)
	{
		return NULL;
	}

	// ������
	pszName = new char[namelen + 1];
	if(!Read(pszName, namelen))
	{
		delete []pszName;
		return NULL;
	}
	pszName[namelen] = 0;

	// �汾
	int iVersion = 0;
	if(!Read(&iVersion, sizeof(iVersion)))
	{
		delete []pszName;
		return NULL;
	}

	// ��������ȡ������Ϣ
	AClassInfo * pClassInfo = AClassInfo::GetClassInfo(pszName);
	assert(pClassInfo);
	if(!pClassInfo)
	{
		delete []pszName;
		return NULL;
	}
	delete []pszName;

	return pClassInfo;
}

bool AArchive::WriteString(AUString &str)
{
	int len = str.GetLength();
	
	Write(&len, sizeof(len));
	if(len > 0)
	{
		Write(str.GetBuffer(), len);
	}
	return true;
}

bool AArchive::ReadString(AUString &str)
{
	int len = 0;
	Read(&len, sizeof(len));
	if(len > 0)
	{
		char * buff = str.GetBufferSetLength(len);
		assert(buff);
		Read(buff, len);
	}

	return true;
}
