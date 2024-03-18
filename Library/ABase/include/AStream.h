/*
 * FILE: AStream.h
 *
 * DESCRIPTION: ���崮�л��ļ����ݽӿ�AStream
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef _ASTREAM_H__
#define _ASTREAM_H__

class AStream  
{
public:
	virtual bool Write(const void * buff, int size) = 0;	// д����
	virtual bool Read(void * buff, int size) = 0;			// ������
};

#endif
