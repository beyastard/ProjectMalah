/*
 * FILE: ALog.h
 *
 * DESCRIPTION: Class used to manipulate the log file
 *
 * CREATED BY: Hedi, 2001/12/3
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ALOG_H_
#define _ALOG_H_

#include "ACPlatform.h"

class ALog
{
private:
	FILE *			m_pFile;
	static char		m_szLogDir[MAX_PATH];

protected:
public:
	ALog();
	~ALog();

	// Init a log file
	//		szLogFile	will be the logs path
	//		szHelloMsg	is the hello message in the log
	//		bAppend		is the flag to append at the end of the log file
	bool Init(char * szLogFile, char * szHelloMsg, bool bAppend=false);

	// Release the log file
	//		this call will close the log file pointer and write a finished message
	bool Release();

	// Output a variable arguments log message;
	bool Log(char * fmt, ...);

	// Output a string as a log message;
	bool LogString(char * szString);

	static void SetLogDir(char * szLogDir);
	static char * GetLogDir() { return m_szLogDir; }
};

typedef ALog * PALog;

#endif
