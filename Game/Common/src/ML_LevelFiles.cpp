/*
 * FILE: ML_LevelFiles.cpp
 *
 * DESCRIPTION: Level file manager class used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/10
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Database.h"
#include "ML_LevelFiles.h"
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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLLevelFiles
//
///////////////////////////////////////////////////////////////////////////

CMLLevelFiles::CMLLevelFiles()
{	
	m_CurPos = NULL;
}

//	Release resources
void CMLLevelFiles::Release()
{
	AListPosition pos = m_FileList.GetHeadPosition();
	while (pos)
	{
		MLLEVELFILE* ptr = m_FileList.GetNext(pos);
		delete ptr;
	}

	m_FileList.RemoveAll();
}

/*	Load data from file

	Return true for success, otherwise return false.

	szFile: script file's name
*/
bool CMLLevelFiles::Load(const char* szFile)
{
	//	Release old data
	Release();

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

	while (Script.PeekNextToken(true))
		ParseLevelFile(&Script);

	Script.Close();

	return true;
}

/*	Save data to file

	Return true for success, otherwise return false.

	szFile: script file's name
*/
bool CMLLevelFiles::Save(const char* szFile)
{
	FILE* fp = fopen(szFile, "wr+");
	if (!fp)
	{
		ADebug::Msg(0, "Failed to create file.", __FILE__, __LINE__);
		return false;
	}

	fprintf(fp, "//  ************* Malah level description file ***************\n\n");
	fprintf(fp, "/*\nfrom\tto\tmission file\n*/\n");

	AListPosition pos = m_FileList.GetHeadPosition();
	while (pos)
	{
		MLLEVELFILE* ptr = m_FileList.GetNext(pos);
		fprintf(fp, "%d\t%d\t%s\n", ptr->iStart, ptr->iEnd, (const char*)ptr->strFile);
	}

	fclose(fp);

	return true;
}

/*	Parse a level file

	Return true for success, otherwise return false.

	pFile: script file object's address
*/
bool CMLLevelFiles::ParseLevelFile(AScriptFile* pFile)
{
	MLLEVELFILE* pLevelFile = new MLLEVELFILE;

	//	Read start level
	pFile->GetNextToken(true);
	pLevelFile->iStart = atoi(pFile->m_szToken);

	//	Read end level
	pFile->GetNextToken(false);
	pLevelFile->iEnd = atoi(pFile->m_szToken);

	//	Read mission file name
	pFile->GetNextToken(false);
	pLevelFile->strFile	= pFile->m_szToken;

	pFile->SkipLine();

	//	Add to list
	m_FileList.AddTail(pLevelFile);

	return true;
}

/*	Get level file

	Return true for success, otherwise return false.

	iLvl: level index.
	pInfo (out): used to receive level file information, cannot be NULL
*/
bool CMLLevelFiles::GetLevelFile(int iLvl, MLLEVELFILE* pInfo)
{
	if (iLvl < 0 || iLvl >= NUM_GAMELEVEL)
		return false;

	AListPosition pos = m_FileList.GetHeadPosition();
	while (pos)
	{
		MLLEVELFILE* ptr = m_FileList.GetNext(pos);

		if (iLvl >= ptr->iStart && iLvl <= ptr->iEnd)
		{
			*pInfo = *ptr;
			return true;
		}
	}

	return false;
}

/*	Get level file

	Return true for success, otherwise return false.

	iLvl: level index.
	pstrFile (out): used to receive mission file name for specified level, 
					cannot be NULL
*/
bool CMLLevelFiles::GetLevelFile(int iLvl, AUString* pstrFile)
{
	MLLEVELFILE Info;
	if (!GetLevelFile(iLvl, &Info))
		return false;

	*pstrFile = Info.strFile;
	return true;
}


