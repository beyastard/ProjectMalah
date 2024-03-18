/*
 * FILE: VGameCfg.cpp
 *
 * DESCRIPTION: virtual game config object
 *
 * CREATED BY: Hedi, 2002/9/19
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VGameCfg.h"
#include "VGame.h"

extern VGame *		g_pGame;

static char * s_var_type_name[] = 
{
	"INT",
	"STRING",
	"FLOAT"
};

static CFG_VAR_TYPE GetTypeByLabel(char * szLabel)
{
	for(int i=0; i<sizeof(s_var_type_name) / sizeof(char *); i++)
	{
		if( _stricmp(s_var_type_name[i], szLabel) == 0 )
			return (CFG_VAR_TYPE) i;
	}

	return CVT_UNKNOWN;
}

VGameCfg::VGameCfg()
{
	m_nVarNum		= 0;
	m_pVarTable		= NULL;
}

VGameCfg::~VGameCfg()
{
	Destroy();
}

bool VGameCfg::Load(char * szFileName)
{
	if( m_nVarNum )
		Destroy();

	FILE * fpConfig;

	fpConfig = fopen(szFileName, "rt");
	if( NULL == fpConfig )
	{
		g_pGame->ErrLog("VGameCfg::Load(), Can not open file %s", szFileName);
		return false;
	}

	// Every 3 lines consist of a variable definition
	char szLine[1024];
	char szTrimLine[1024];
	while(fgets(szLine, 1024, fpConfig))
	{
		// First trim and skip empty line and comment line;
		APath_TrimPath(szLine, szTrimLine);
		if( !szTrimLine[0] || szTrimLine[1] == '#' )
			continue;

		m_nVarNum ++;

		m_pVarTable = (CFG_VAR *) realloc(m_pVarTable, m_nVarNum * sizeof(CFG_VAR));
		if( NULL == m_pVarTable )
		{
			g_pGame->ErrLog("VGameCfg::Load(), Not enough memory!");
			return false;
		}

		CFG_VAR		var = m_pVarTable[m_nVarNum - 1];
		strncpy(var.szVarName, szTrimLine, 32);

		// variable type
		fgets(szLine, 1024, fpConfig);
		APath_TrimPath(szLine, szTrimLine);
		CFG_VAR_TYPE	type = GetTypeByLabel(szTrimLine);
		if( type == CVT_UNKNOWN )
		{
			g_pGame->ErrLog("VGameCfg::Load(), Unknown type [%s]", szTrimLine);
			return false;
		}
		var.type = type;

		// variable data
		fgets(szLine, 1024, fpConfig);
		APath_TrimPath(szLine, szTrimLine);
		int		nValue;
		float	vValue;

		switch(type)
		{
		case CVT_INT:
			nValue = atoi(szTrimLine);
			var.pData = (LPBYTE) malloc(sizeof(int));
			if( var.pData == NULL )
			{
				g_pGame->ErrLog("VGameCfg::Load(), Not enough memory!");
				return false;
			}
			memcpy(var.pData, &nValue, sizeof(int));
			break;
		case CVT_STRING:
			var.pData = (LPBYTE) malloc(strlen(szTrimLine) + 1);
			if( var.pData == NULL )
			{
				g_pGame->ErrLog("VGameCfg::Load(), Not enough memory!");
				return false;
			}
			strcpy((char *)var.pData, szTrimLine);
			break;
		case CVT_FLOAT:
			vValue = float(atof(szTrimLine));
			var.pData = (LPBYTE) malloc(sizeof(float));
			if( var.pData == NULL )
			{
				g_pGame->ErrLog("VGameCfg::Load(), Not enough memory!");
				return false;
			}
			memcpy(var.pData, &vValue, sizeof(float));
			break;
		}

	}

	fclose(fpConfig);
	return true;
}

bool VGameCfg::Save(char * szFileName)
{
	return true;
}

bool VGameCfg::Destroy()
{
	if( m_pVarTable )
	{
		for(int i=0; i<m_nVarNum; i++)
		{
			CFG_VAR var = m_pVarTable[i];

			if( var.pData )
				free(var.pData);
		}
		free(m_pVarTable);

		m_nVarNum	= 0;
		m_pVarTable = NULL;
	}

	return true;
}

bool VGameCfg::GetVariables(char * szVarName, CFG_VAR_TYPE type, LPBYTE pBuffer, int nBufferLen)
{
	for(int i=0; i<m_nVarNum; i++)
	{
		CFG_VAR var = m_pVarTable[i];

		if( _stricmp(szVarName, var.szVarName) == 0 && var.type == type )
		{
			memcpy(pBuffer, var.pData, min(nBufferLen, var.nDataLen));
			return true;
		}
	}

	return false;
}
