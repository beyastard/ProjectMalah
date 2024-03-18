/*
 * FILE: ML_Strings.cpp
 *
 * DESCRIPTION: Outside strings used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/19
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_ABase.h"
#include "ML_Strings.h"
#include "AScriptFile.h"
#include "AFileImage.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////

CMLStrings	g_MLStrings;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLStrings
//
///////////////////////////////////////////////////////////////////////////

CMLStrings::CMLStrings()
{
	memset(m_aStrings, 0, sizeof (m_aStrings));
}

CMLStrings::~CMLStrings()
{
	for (int i=0; i < MAXNUM_STRINGS; i++)
	{
		if (m_aStrings[i])
			delete m_aStrings[i];
	}
}

/*	Load strings

	szFile: name of file which contains all strings
*/
bool CMLStrings::LoadStrings(const char* szFile)
{
	AScriptFile Script;
	AFileImage FileImage;
	
	if (!FileImage.Open((char*)szFile, AFILE_OPENEXIST | AFILE_BINARY))
	{
		ADebug::Msg(0, "Failed to open config file.", __FILE__, __LINE__);
		return false;
	}

	if (!Script.Open(&FileImage))
	{
		ADebug::Msg(0, "Failed to open script file.", __FILE__, __LINE__);
		return false;
	}

	FileImage.Close();

	while (Script.GetNextToken(true))
	{
		//	Get string's index
		int iIndex = atoi(Script.m_szToken);
		assert(iIndex >= 0 && iIndex < MAXNUM_STRINGS);

		//	Check string conflict
		if (m_aStrings[iIndex])
		{
			ADebug::Msg(0, "String conflict !", __FILE__, __LINE__);
			delete m_aStrings[iIndex];
		}

		//	Get string
		Script.GetNextToken(false);
		m_aStrings[iIndex] = new AUString(Script.m_szToken);

		Script.SkipLine();
	}

	Script.Close();

	return true;
}



