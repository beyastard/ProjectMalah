/*
 * FILE: AMemFile.h
 *
 * DESCRIPTION: �ڴ��ļ�
 *
 * CREATED BY: ����ӯ, 2002/10/27
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef _AMEMFILE_H__
#define _AMEMFILE_H__

#include "AStream.h"
#include "abasetypes.h"
#include "AObject.h"

class AMemFile : public AStream  
{
public:

	static bool DupData(AObject * pSrc, AObject * pDst);
	static AObject * Dup(AObject * pSrc);

	void SeekToEnd();
	void SeekToBegin();
	DWORD GetPosition();
	DWORD GetLength();
	bool IsEOF();
	bool IsBOF();

	virtual bool Write(const void * buff, int size);	// д����
	virtual bool Read(void * buff, int size);			// ������

	AMemFile(DWORD buffersize = 1024);
	virtual ~AMemFile();

protected:

	HGLOBAL	m_handle_memory;
	DWORD	m_dw_position;
	DWORD	m_dw_length;
};

#endif
