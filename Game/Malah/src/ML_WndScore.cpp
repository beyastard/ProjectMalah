/*
 * FILE: ML_WndScore.cpp
 *
 * DESCRIPTION: Scoreboard interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "ML_WndScore.h"
#include "ML_GamePlay.h"
#include "ML_WndManager.h"
#include "AWndSprite.h"
#include "jexception.h"

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

extern char		g_szSettingFile[];

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////

void DrawBackRect(RECT rArea)
{
	int outw	= 4;
	int inw		= 2;
	int margin	= 5;
	UINT	uColor		= 0x800E1019;
	UINT	uLineColor	= 0xFFFF8000;

	APOINT	v0(rArea.left, rArea.top + outw);
	APOINT	v1(rArea.left + outw, rArea.top);
	APOINT	v2(rArea.right - outw, rArea.top);
	APOINT	v3(rArea.right, rArea.top + outw);
	APOINT	v4(rArea.right, rArea.bottom-outw);
	APOINT	v5(rArea.right - outw, rArea.bottom);
	APOINT	v6(rArea.left + outw, rArea.bottom);
	APOINT	v7(rArea.left, rArea.bottom-outw);

	AWC::DrawTriangle(v0, v1, v2, uColor);
	AWC::DrawTriangle(v0, v2, v7, uColor);

	AWC::DrawTriangle(v7, v2, v3, uColor);
	AWC::DrawTriangle(v7, v3, v6, uColor);

	AWC::DrawTriangle(v6, v3, v4, uColor);
	AWC::DrawTriangle(v6, v4, v5, uColor);

	rArea.left	+= margin;
	rArea.right	-= margin;
	rArea.top	+= margin;
	rArea.bottom-= margin;
	APOINT	p0(rArea.left, rArea.top + inw);
	APOINT	p1(rArea.left + inw, rArea.top);
	APOINT	p2(rArea.right - inw, rArea.top);
	APOINT	p3(rArea.right, rArea.top + inw);
	APOINT	p4(rArea.right, rArea.bottom-inw);
	APOINT	p5(rArea.right - inw, rArea.bottom);
	APOINT	p6(rArea.left + inw, rArea.bottom);
	APOINT	p7(rArea.left, rArea.bottom-inw);

	AWC::DrawLine(p0, p1, uLineColor);
	AWC::DrawLine(p1, p2, uLineColor);
	AWC::DrawLine(p2, p3, uLineColor);
	AWC::DrawLine(p3, p4, uLineColor);

	AWC::DrawLine(p4, p5, uLineColor);
	AWC::DrawLine(p5, p6, uLineColor);
	AWC::DrawLine(p6, p7, uLineColor);
	AWC::DrawLine(p7, p0, uLineColor);
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLWndScore
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndScore, AWnd, 1)

CMLWndScore::CMLWndScore():
m_pBackimage	(NULL),
m_nPlayerCount	(0),
m_ptListStart	(0, 0),
m_ptListStep	(0, 0)
{
	for (int i = 0; i < MAX_PLAYER; i ++)
		m_aPlayers[i].m_nScore = 0;

	m_bVisible			= false;
	m_bKeyPreview		= true;
	m_bCanBeFocus		= true;
	m_bAutoLoadRes		= false;
	m_nInsertScorePos	= -1;
}

CMLWndScore::~CMLWndScore()
{
}

void CMLWndScore::OnInputName(AUString& strName)
{
	J_EXCEPTION_RECORD(0);
//	m_aPlayers[m_nInsertScorePos].m_strName = strName;
//	m_wndNameInput.ShowWindow(false);
}

bool CMLWndScore::OnCreate()
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::OnCreate())
		return false;

	return true;
}

void CMLWndScore::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndScore::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	APOINT ptAb;
	GetAbsolutePos(ptAb);

	if (m_pBackimage)
	{
		if(!AWC::DrawSprite(m_pBackimage, ptAb))
		{
			ADEBUG_TRACE("Cannot draw to back.");
		}
	}
		
	int i;
	APOINT ptPlayerOut(m_ptListStart);
	for (i = 0; i < m_nPlayerCount; i++)
	{
		ptPlayerOut.x = m_ptListStart.x;
		if (i == m_nInsertScorePos)
		{
			ARECT r(ptPlayerOut.x-2, ptPlayerOut.y, ptPlayerOut.x + 212, ptPlayerOut.y + 20);
			AWC::DrawRectangle(r, A3DCOLORRGB(0, 128, 128));
		}

		int nLength	= m_aPlayers[i].m_strName.GetLength();
		if (nLength > 0)
			AWC::DrawText(ptPlayerOut, m_aPlayers[i].m_strName, -1, 0xFFFF0000);
		
		ptPlayerOut.x += m_ptListStep.x;

		char szScore[64];
		sprintf(szScore, "%d", m_aPlayers[i].m_nScore);
		APOINT scoresize;
		AWC::GetTextExtent(szScore, -1, scoresize);
		ptPlayerOut.x	-= scoresize.x;
		AWC::DrawText(ptPlayerOut, szScore, -1, 0xFFFFFF00);

		ptPlayerOut.y += m_ptListStep.y;
	}

	return true;
}

bool CMLWndScore::OnOkClick(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	AWndIniFile IniList;
	if (!IniList.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot open setting file.");
		return false;
	}

	AUString pszPlayerSec = "player_list";
	IniList.WriteInt(pszPlayerSec, "count_player", m_nPlayerCount);

	int i;
	char szKeyBuf[64];
	for (i=0; i < m_nPlayerCount; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "player_%d", i);
		IniList.WriteString(pszPlayerSec, szKeyBuf, m_aPlayers[i].m_strName);
	
		sprintf(szKeyBuf, "score_%d", i);
		IniList.WriteInt(pszPlayerSec, szKeyBuf, m_aPlayers[i].m_nScore);
	}

	IniList.Save();
	IniList.Release();
	
	ShowWindow(false);

	if (m_bAutoLoadRes)
	{
		MLMSG Msg;
		
		Msg.idManager	= -1;
		Msg.dwMsg		= MSG_EXITGAME;
		Msg.dwParam1	= 100;
		
		GETMLGAMEPLAY->OnMsg((AMSG*)&Msg);
	}

	return true;
}

bool CMLWndScore::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);
	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
	{
		return false;
	}

	const char* pszBackimage = NULL;
	if (!IniFile.ReadString(pszSection, "image_back", pszBackimage))
	{
		ADEBUG_TRACE("cannot read back image.");
		return false;
	}

	m_strBackimage = pszBackimage;

	if (!m_btnOk.LoadFromIni(this, IniFile, "wnd_score::button_back"))
	{
		ADEBUG_TRACE("cannot load button wnd_score::button_back");
		return false;
	}

	m_btnOk.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndScore::OnOkClick);

	if (!IniFile.ReadPoint(pszSection, "point_list_start", m_ptListStart))
	{
		ADEBUG_TRACE("Cannot read point_list_start");
		return false;
	}

	if (!IniFile.ReadPoint(pszSection, "point_list_step", m_ptListStep))
	{
		ADEBUG_TRACE("Cannot read point_list_step");
		return false;
	}

	return true;
}

//	Load score list
bool CMLWndScore::LoadScoreList()
{
	AWndIniFile IniList;
	if (!IniList.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot create setting list file.");
		return false;
	}

	AUString pszPlayerSec = "player_list";
	if (!IniList.ReadInt(pszPlayerSec, "count_player", m_nPlayerCount))
	{
		ADEBUG_TRACE("Cannot read player list count.");
		return false;
	}

	ADebug::Msg(0,"PlayerCount %d in %s ",m_nPlayerCount,pszPlayerSec);

	if (m_nPlayerCount < 0)
		m_nPlayerCount	= 0;
	else if (m_nPlayerCount > MAX_PLAYER)
		m_nPlayerCount	= MAX_PLAYER;
	
	int  i;
	char szKeyBuf[64];
	for (i=0; i < m_nPlayerCount; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "player_%d", i);
		if (!IniList.ReadString(pszPlayerSec, szKeyBuf, pszName))
		{
			m_nPlayerCount	= 0;
			ADEBUG_TRACE1("Cannot read palyer name %d.", i);
			break;
		}

		m_aPlayers[i].m_strName	= pszName;

		sprintf(szKeyBuf, "score_%d", i);
		if (!IniList.ReadInt(pszPlayerSec, szKeyBuf, m_aPlayers[i].m_nScore))
		{
			m_nPlayerCount	= 0;
			ADEBUG_TRACE1("Cannot read player score %d.", i);
			break;
		}
	}

	QueueScore(m_aPlayers, m_nPlayerCount);

	IniList.Release();

	return true;
}

bool CMLWndScore::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());

	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
		m_rArea.GetHeight(), 0, 1, &rSprite);

	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}

	return true;
}

bool CMLWndScore::FreeRes()
{
	J_EXCEPTION_RECORD(0);

	bool bRet = true;

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

	return bRet && AWnd::FreeRes();
}

bool CMLWndScore::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
	{
		LoadScoreList();

		if (m_bAutoLoadRes)
		{
			LoadRes();

			int nScore = GETMLGAMEPLAY->GetUserData()->iScore;

			if (m_nPlayerCount == 0)
				m_nInsertScorePos = 0;
			else
			{
				int i;
				for (i=0; i < m_nPlayerCount; i++)
				{
					if (m_aPlayers[i].m_nScore < nScore)
						break;
				}
				
				if (i >= m_nPlayerCount)
					m_nInsertScorePos = m_nPlayerCount;
				else
					m_nInsertScorePos = i;
			}
			
			if (m_nInsertScorePos <= MAX_PLAYER-1 && nScore)
			{ 
				APlayerNode nodeTemp;
				nodeTemp.m_nScore	= nScore;
				nodeTemp.m_strName	= GETMLGAMEPLAY->GetUserData()->strName;
				InsertScore(m_nInsertScorePos, &nodeTemp);
			}
		}
		else
			m_nInsertScorePos = -1;
	}
	else if (m_bAutoLoadRes)
	{
		FreeRes();
	}

	return true;
}

bool CMLWndScore::QueueScore(APlayerNode* aPlayers, int iNumPlayer)
{
	APlayerNode nodeTemp;

	for (int i=0; i < iNumPlayer-1; i++)
	{
		for (int j = i+1; j < iNumPlayer; j++)
		{
			if (aPlayers[i].m_nScore < aPlayers[j].m_nScore)
			{
				nodeTemp.m_nScore		= aPlayers[i].m_nScore;
				nodeTemp.m_strName		= aPlayers[i].m_strName;
				aPlayers[i].m_nScore	= aPlayers[j].m_nScore;
				aPlayers[i].m_strName	= aPlayers[j].m_strName;
				aPlayers[j].m_nScore	= nodeTemp.m_nScore;
				aPlayers[j].m_strName	= nodeTemp.m_strName;
			}
		}
	}

	return true;
}

//insert score at index,index base on 0
bool CMLWndScore::InsertScore(int index,APlayerNode * pNode)
{
	if(index<0 || index> m_nPlayerCount || index >MAX_PLAYER-1)
		return false;

	int count = m_nPlayerCount - index;
	for(int i=0;i<count;i++)
	{
		if (m_nPlayerCount-i<=MAX_PLAYER-1)
			m_aPlayers[m_nPlayerCount-i] = m_aPlayers[m_nPlayerCount-i-1];
	}
	
	m_aPlayers[index] = *pNode;

	m_nPlayerCount++;
	if(m_nPlayerCount>=MAX_PLAYER)
		m_nPlayerCount = MAX_PLAYER;

	return true;
}

void CMLWndScore::SavePlayerScore(const char* szName, int iScore)
{
	if (!iScore)
		return;

	AWndIniFile IniList;
	if (!IniList.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot create setting file.");
		return;
	}

	int iNumPlayer;

	AUString pszPlayerSec = "player_list";
	if (!IniList.ReadInt(pszPlayerSec, "count_player", iNumPlayer))
	{
		ADEBUG_TRACE("Cannot read player list count.");
		return;
	}

	if (iNumPlayer < 0)
		iNumPlayer = 0;
	else if (iNumPlayer > MAX_PLAYER)
		iNumPlayer = MAX_PLAYER;
	
	APlayerNode aPlayers[MAX_PLAYER];

	int i, iMin = -1;
	bool bInsert = !iNumPlayer ? true : false;
	char szKeyBuf[64];

	for (i=0; i < iNumPlayer; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "player_%d", i);
		
		IniList.ReadString(pszPlayerSec, szKeyBuf, pszName);
		aPlayers[i].m_strName = pszName;
		
		sprintf(szKeyBuf, "score_%d", i);
		IniList.ReadInt(pszPlayerSec, szKeyBuf, aPlayers[i].m_nScore);
		
		if (iMin < 0 || aPlayers[i].m_nScore < aPlayers[iMin].m_nScore)
			iMin = i;
		
		if (aPlayers[i].m_nScore < iScore)
			bInsert = true;
	}

	if (bInsert)
	{
		if (iNumPlayer == MAX_PLAYER)
		{
			aPlayers[iMin].m_strName = szName;
			aPlayers[iMin].m_nScore  = iScore;
		}
		else
		{
			aPlayers[iNumPlayer].m_strName = szName;
			aPlayers[iNumPlayer].m_nScore  = iScore;
			iNumPlayer++;
		}
	}

	QueueScore(aPlayers, iNumPlayer);

	IniList.WriteInt(pszPlayerSec, "count_player", iNumPlayer);

	for (i=0; i < iNumPlayer; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "player_%d", i);
		IniList.WriteString(pszPlayerSec, szKeyBuf, aPlayers[i].m_strName);
		
		sprintf(szKeyBuf, "score_%d", i);
		IniList.WriteInt(pszPlayerSec, szKeyBuf, aPlayers[i].m_nScore);
	}
		
	IniList.Save();
	IniList.Release();
}

