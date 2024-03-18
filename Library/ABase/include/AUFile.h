/*
 * FILE: AUFile.h
 *
 * DESCRIPTION: ���崮�л��ļ����ݽӿ�AUFile
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY: By Jiangli. ʵ����һЩ�ӿڣ�
 *			Write, Read, Create, Close, SetPointer, GetSize.
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef _AUFILE_H__
#define _AUFILE_H__

#include "ABaseTypes.h"
#include "AStream.h"


class AUFile : public AStream
{
protected:
	HANDLE	m_hFile;
public:
	AUFile();
	virtual ~AUFile();

	virtual bool Write(const void * buff, int size);	// д����

	// [out] buff : 
	// [in/out] size : in, the size of buff.
	//                 out, the size of readed.
	virtual bool Read(void * buff, int size);			// ������

	// [in] nDistanceToMove : The distance to move the file pointer.
	//          A positive value for lDistanceToMove moves the file
	//          pointer forward in the file, and a negative value
	//          moves the file pointer backward.
	// [in] dwMoveMethod : Starting point for the file pointer move. 
	//          This parameter can be one of the following values:
	//               FILE_BEGIN
	//               FILE_CURRENT
	//               FILE_END
	virtual bool SetPointer(int nDistanceToMove, DWORD dwMoveMethod);

	// return : If the function fails, the returned value is -1.
	virtual int  GetSize();

	// [in] pszPathName : the file's path name.
	// [in] dwCreationDisposition : Can be one of the following,
	//                CREATE_NEW
	//                CREATE_ALWAYS
	//                OPEN_EXISTING
	//                OPEN_ALWAYS
	//                TRUNCATE_EXISTING 
	virtual bool Create(const char* pszPathName, DWORD dwCreationDisposition);
	virtual void Close();
};

#endif
