/*
 * FILE: ABinary.h
 *
 * DESCRIPTION: �����ƿ�����࣬��Ľ��ͷ�ʽ�ɸ�����GUID����˵��
 *
 * CREATED BY: ����ӯ, 2002/8/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef _ABINARY_H__
#define _ABINARY_H__

#include "AObject.h"
#include <memory.h>

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;
#endif /* GUID_DEFINED */

// {00000000-0000-0000-0000-0000000000000000} �ձ�ʶ�����ڳ�ʼ�����жϳ�ʼ��
static const GUID BT_EMPTY = 
{ 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };


class ABinary : public AObject  
{
public:

	DECLARE_ASERIAL(ABinary)

public:

	virtual bool Load(AArchive &ar);			// ��ȡ����
	virtual bool Save(AArchive &ar);

	int GetLength();							// ��ȡ��������
	void * GetBufferSetLength(int length);		// ��ʼ/���� ������������
	void SetType(GUID guidType);				// ���ý�������
	GUID GetType();								// ��ȡ��������

	const ABinary & operator = (const ABinary & bin);

	ABinary();
	ABinary(GUID guidType, int iLength, const void * pInitData = NULL);
	ABinary(const ABinary & bin);
	~ABinary();

	// ����������ָ���ת��
	void * GetBuffer() {return m_pData;}

protected:

	GUID	m_guidType;				// ���ͷ�ʽ
	void *	m_pData;				// ������
	int		m_iLength;				// ����������
};

#endif
