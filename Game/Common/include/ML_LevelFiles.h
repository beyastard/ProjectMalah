/*
 * FILE: ML_LevelFiles.h
 *
 * DESCRIPTION: Level file manager class used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/10
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_LEVELFILES_H_
#define _ML_LEVELFILES_H_

#include "AUString.h"
#include "AScriptFile.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

struct MLLEVELFILE
{
	int			iStart;		//	Start level
	int			iEnd;		//	End level
	AUString	strFile;	//	File name
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	CMLLevelFiles
//
///////////////////////////////////////////////////////////////////////////

class CMLLevelFiles
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLLevelFiles();
	virtual ~CMLLevelFiles() {}

public:		//	Attributes

public:		//	Operations

	void		Release();		//	Release resources

	bool		Load(const char* szFile);		//	Load data from file
	bool		Save(const char* szFile);		//	Save data to file

	bool		GetLevelFile(int iLvl, MLLEVELFILE* pInfo);		//	Get level file
	bool		GetLevelFile(int iLvl, AUString* pstrFile);		//	Get level file

	inline MLLEVELFILE*	GetFirstLvlFile();		//	Get first level file
	inline MLLEVELFILE*	GetNextLvlFile();		//	Get next level file

	inline void	AddLvlFile(MLLEVELFILE* pLvlFile);	//	Add a level file
	
protected:	//	Attributes

	AListPosition	m_CurPos;		//	current position

	AListT<MLLEVELFILE*>	m_FileList;		//	File list

protected:	//	Operations

	bool		ParseLevelFile(AScriptFile* pFile);		//	Parse a level file
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Get first level file
MLLEVELFILE* CMLLevelFiles::GetFirstLvlFile()
{
	m_CurPos = m_FileList.GetHeadPosition();
	if (m_CurPos)
		return m_FileList.GetNext(m_CurPos);
	else
		return NULL;
}

//	Get next level file
MLLEVELFILE* CMLLevelFiles::GetNextLvlFile()
{
	if (!m_CurPos)
		return NULL;

	return m_FileList.GetNext(m_CurPos);
}

//	Add a level file
void CMLLevelFiles::AddLvlFile(MLLEVELFILE* pLvlFile)
{
	m_FileList.AddTail(pLvlFile);
}


#endif	//	_ML_LEVELFILES_H_



