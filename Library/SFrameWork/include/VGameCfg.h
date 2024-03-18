/*
 * FILE: VGameCfg.h
 *
 * DESCRIPTION: virtual game config object
 *
 * CREATED BY: Hedi, 2002/9/19
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VGAMECFG_H_
#define _VGAMECFG_H_

#include <Windows.h>

enum CFG_VAR_TYPE
{
	CVT_INT			= 0,
	CVT_STRING		= 1,
	CVT_FLOAT		= 2,
	CVT_UNKNOWN		= 0xffff
};

class VGameCfg
{
protected:
	typedef struct _CFG_VAR
	{
		char			szVarName[32];
		LPBYTE			pData;
		CFG_VAR_TYPE	type;
		int				nDataLen;
	} CFG_VAR;

	int					m_nVarNum;
	CFG_VAR *			m_pVarTable;

public:
	VGameCfg();
	~VGameCfg();

	virtual bool Load(char * szFileName);
	virtual bool Destroy();
	virtual bool Save(char * szFileName);

	// Function to retrieve a variable from config
	virtual bool GetVariables(char * szVarName, CFG_VAR_TYPE type, LPBYTE pBuffer, int nBufferLen);
};

#endif//_VGAMECFG_H_