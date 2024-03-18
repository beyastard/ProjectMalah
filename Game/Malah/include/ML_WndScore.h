/*
 * FILE: ML_WndScore.h
 *
 * DESCRIPTION: Scoreboard interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#ifndef	_ML_WNDSCORE_H_
#define	_ML_WNDSCORE_H_

#include "AWnd.h"
#include "ML_Button.h"
#include "AEditPlus.h"
#include "AUString.h"

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

class A2DSprite;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLWndScore
//
///////////////////////////////////////////////////////////////////////////

class CMLWndScore : public AWnd
{
public:		//	Types

	DECLARE_ASERIAL(CMLWndScore)

	enum
	{
		MAX_PLAYER = 10,
	};

	class APlayerNode
	{
	public:

		AUString	m_strName;
		int			m_nScore;
	};

public:		//	Constructor and Destructor

	CMLWndScore();
	virtual ~CMLWndScore();

public:		//	Attributes

public:		//	Operations

	virtual bool	LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection);
	virtual bool	LoadRes();
	virtual bool	FreeRes();

	virtual void	OnInputName(AUString& strName);
	virtual bool	ShowWindow(bool bShow);

	bool		LoadScoreList();	//	Load score list
	void		EnableAutoLoadRes(bool bAuto)	{	m_bAutoLoadRes = bAuto;	}

	static void		SavePlayerScore(const char* szName, int iScore);
	static bool		QueueScore(APlayerNode* aPlayers, int iNumPlayer);

protected:	//	Attributes

	CMLButton		m_btnOk;
	AUString		m_strBackimage;
	A2DSprite*		m_pBackimage;
	bool			m_bAutoLoadRes;		//	Auto load/release resource when show/hide window

	APlayerNode		m_aPlayers[MAX_PLAYER];
	int				m_nPlayerCount;
	APOINT			m_ptListStart;
	APOINT			m_ptListStep;
	int				m_nInsertScorePos;

protected:	//	Operations

	virtual bool	OnCreate();
	virtual void	OnDestroy();
	virtual bool	OnDraw();
	virtual bool	OnOkClick(AWnd* pWnd);

	bool	IsEmptyName(char *name);
	bool	RemoveScore(int index);
	bool	InsertScore(int index,APlayerNode * pNode);
};


#endif	// _ML_WNDSCORE_H_