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

#include "ALog.h"
#include "ATime.h"
#include "APath.h"
#include <IO.h>

void SafeCreateDir(char * szDir)
{
	if( _access(szDir, 0) == -1 )
	{
		CreateDirectory(szDir, NULL);
	}
}

char ALog::m_szLogDir[MAX_PATH] = "Logs";

ALog::ALog()
{
	m_pFile = NULL;
}

ALog::~ALog()
{
}

bool ALog::Init(char * szLogFile, char * szHelloMsg, bool bAppend)
{
	WORD nYear, nMonth, nDay, nDate, nHour, nMinute, nSecond;
	char szLogPath[MAX_PATH];

	SafeCreateDir(m_szLogDir);

	sprintf(szLogPath, "%s\\%s", m_szLogDir, szLogFile);
	if( bAppend )
		m_pFile = fopen(szLogPath, "at");
	else
		m_pFile = fopen(szLogPath, "wt");
	if( NULL == m_pFile ) 
		return true; 

	ATime_GetSystemTime(&nYear, &nMonth, &nDay, &nDate, &nHour, &nMinute, &nSecond);
	fprintf(m_pFile, "%s\nCreated(or opened) on %02d/%02d/%04d %02d:%02d:%02d\n\n", szHelloMsg,
		nDay, nMonth, nYear, nHour, nMinute, nSecond);
	return true;
}

bool ALog::Release()
{
	LogString("Log file closed successfully!");

	if( m_pFile )
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return true;
}

bool ALog::Log(char * fmt, ...)
{
	char szErrorMsg[2048];
	va_list args_list;


    va_start(args_list, fmt);	   
    vsprintf(szErrorMsg, fmt, args_list);
	va_end(args_list);

	return LogString(szErrorMsg);
}

bool ALog::LogString(char * szString)
{
	if( !m_pFile ) return true;

	WORD nYear, nMonth, nDay, nDate, nHour, nMinute, nSecond;
    ATime_GetSystemTime(&nYear, &nMonth, &nDay, &nDate, &nHour, &nMinute, &nSecond);

    fprintf(m_pFile, "[%02d:%02d:%02d] %s\n", nHour, nMinute, nSecond, szString);
	fflush(m_pFile);
	return true;
}

void ALog::SetLogDir(char * szLogDir)
{
	strncpy(m_szLogDir, szLogDir, MAX_PATH);

	if( m_szLogDir[strlen(m_szLogDir) - 1] == '\\' )
		m_szLogDir[strlen(m_szLogDir) - 1] = '\0';

	SafeCreateDir(m_szLogDir);
}