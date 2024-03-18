/*
 * FILE: ML_ManPlayer.cpp
 *
 * DESCRIPTION: Player manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/18
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManPlayer.h"
#include "ML_ManagerDef.h"
#include "ML_TransPlayerKeyboard.h"
#include "ML_TransPlayerMouse.h"
#include "ML_GamePlay.h"

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
//	Implement CMLManPlayer
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManPlayer, CMLManBase, 1)

CMLManPlayer::CMLManPlayer() : CMLManBase()
{
	m_managerId		= MANID_PLAYER;
	m_pKBTrans		= NULL;
	m_pMouseTrans	= NULL;
	m_dwTimeCnt		= 0;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManPlayer::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	if (!(m_pKBTrans = new CMLTransPlayerKeyboard))
	{
		ADEBUG_TRACE("Failed to create player mouse translator");
		return false;
	}

	m_pKBTrans->BindGame(pGame);

	if (!(m_pMouseTrans = new CMLTransPlayerMouse))
	{
		ADEBUG_TRACE("Failed to create player keyboard translator");
		return false;
	}

	m_pMouseTrans->BindGame(pGame);

	return true;
}

//	Release object
bool CMLManPlayer::Release()
{
	if (m_pKBTrans)
	{
		delete m_pKBTrans;
		m_pKBTrans = NULL;
	}

	if (m_pMouseTrans)
	{
		delete m_pMouseTrans;
		m_pMouseTrans = NULL;
	}

	return CMLManBase::Release();
}

/*	Add a player

	Return true for success, otherwise return false.

	pPlayer: player object's address
*/
bool CMLManPlayer::AddPlayer(CMLPlayer* pPlayer)
{
	m_PlayerList.AddTail(pPlayer);
	return true;
}

/*	Get player object by ID

	Return player object's address, otherwise return NULL.

	iID: player's ID
*/
CMLPlayer* CMLManPlayer::GetPlayerByID(int iID)
{
	AListPosition pos = m_PlayerList.GetHeadPosition();

	while (pos)
	{
		CMLPlayer* pPlayer = m_PlayerList.GetNext(pos);
		if (pPlayer->GetId() == iID)
			return pPlayer;
	}

	return NULL;
}

/*	Remove player from manager

	Return player object's address, otherwise return NULL.

	iID: player's ID
*/
CMLPlayer* CMLManPlayer::RemovePlayer(int iID)
{
	AListPosition pos = m_PlayerList.GetHeadPosition();

	while (pos)
	{
		CMLPlayer* pPlayer = m_PlayerList.GetAt(pos);
		if (pPlayer->GetId() == iID)
		{
			m_PlayerList.RemoveAt(pos);
			return pPlayer;
		}

		m_PlayerList.GetNext(pos);
	}

	return NULL;
}

//	Message handler
int	CMLManPlayer::OnMsg(AMSG* pMSG)
{
	MLMSG* pMLMsg = (MLMSG*)pMSG;

	if (pMLMsg->idObject == m_managerId)
		return ProcessMsg(pMLMsg) ? 1 : 0;

	CMLPlayer* pPlayer = GetPlayerByID(pMLMsg->idObject);
	if (pPlayer)
		pPlayer->OnMsg(pMLMsg);

	return 1;
}

/*	Handle a message

	Return true for success, otherwise return false.

	pMsg: message will be handled
*/
bool CMLManPlayer::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

/*	Bind translator

	Return true for success, otherwise return false.

	iBindType: 
*/
bool CMLManPlayer::BindTrans(int iBindType)
{
	ATrans* pMouse=NULL, *pKB=NULL;

	switch (iBindType)
	{
	case TBT_NONE:

		pKB		= NULL;
		pMouse	= NULL;
		break;

	case TBT_ONLYKEYBOARD:

		pKB		= m_pKBTrans;
		pMouse	= NULL;
		break;

	case TBT_ONLYMOUSE:

		pKB		= NULL;
		pMouse	= m_pMouseTrans;
		break;

	case TBT_MOUSEANDKB:

		pKB		= m_pKBTrans;
		pMouse	= m_pMouseTrans;
		break;

	default:
		return false;
	}

	if (m_pAGame->GetADeviceKeyboard())
		m_pAGame->GetADeviceKeyboard()->SetTrans(pKB);

	if (m_pAGame->GetADeviceMouse())
		m_pAGame->GetADeviceMouse()->SetTrans(pMouse);

	return true;
}

//	Called when begin game playing
bool CMLManPlayer::OnStartGamePlay()
{
	//	Create player
	CMLPlayer* pPlayer = new CMLPlayer;
	if (!pPlayer)
	{
		ADEBUG_TRACE("Not enough memory.");
		return false;
	}

	if (!pPlayer->Init(GETMLGAMEPLAY->GetLogicWorld(), g_theGame.GetA3DCamera()))
	{
		ADEBUG_TRACE("Failed to initialize local player.");
		return false;
	}

	AddPlayer(pPlayer);

	if (!pPlayer->OnStartGamePlay())
	{
		ADEBUG_TRACE("Failed to call player->OnStartGamePlay.");
		return false;
	}

	return true;
}

//	Called when end game playing
bool CMLManPlayer::OnEndGamePlay()
{
	//	Handle player
	CMLPlayer* pPlayer = RemovePlayer(ID_LOCALPLAYER);
	if (pPlayer)
	{
		if (!pPlayer->OnEndGamePlay())
		{
			ADEBUG_TRACE("Failed call player->OnEndGamePlay!");
			return false;
		}
		
		pPlayer->Release();
		delete pPlayer;
	}

	return true;
}

//	Called when being mission
bool CMLManPlayer::OnBeginMission(int iMission)
{
	CMLPlayer* pPlayer = GetPlayerByID(ID_LOCALPLAYER);

	if (!pPlayer->OnBeginMission(iMission))
	{
		ADEBUG_TRACE("Failed call player->OnBeginMission!");
		return false;
	}

	m_dwTimeCnt = 0;

	return true;
}

//	Called when reset mission
bool CMLManPlayer::OnResetMission()
{
	CMLPlayer* pPlayer = GetPlayerByID(ID_LOCALPLAYER);

	if (!pPlayer->OnResetMission())
	{
		ADEBUG_TRACE("Failed call player->OnResetMission!");
		return false;
	}

	m_dwTimeCnt = 0;

	return true;
}

//	Called when end mission
bool CMLManPlayer::OnEndMission()
{
	CMLPlayer* pPlayer = GetPlayerByID(ID_LOCALPLAYER);

	if (!pPlayer->OnEndMission())
	{
		ADEBUG_TRACE("Failed call player->OnEndMission!");
		return false;
	}

	return true;
}

//	Logic tick
bool CMLManPlayer::LogicRun(float fTimeSpan)
{
	BeginLogicCount();

	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	AListPosition pos = m_PlayerList.GetHeadPosition();
	while (pos)
	{
		CMLPlayer* pPlayer = m_PlayerList.GetNext(pos);
		pPlayer->LogicRun(fTimeSpan, m_dwTimeCnt);
	}

	EndLogicCount();

	return true;
}

//	Tick animation
bool CMLManPlayer::TickAnimation()
{
	AListPosition pos = m_PlayerList.GetHeadPosition();
	while (pos)
	{
		CMLPlayer* pPlayer = m_PlayerList.GetNext(pos);
		pPlayer->TickAnimation();
	}

	return true;
}

//	Render routines
bool CMLManPlayer::Render(A3DViewport* pViewport)
{
	AListPosition pos = m_PlayerList.GetHeadPosition();
	while (pos)
	{
		CMLPlayer* pPlayer = m_PlayerList.GetNext(pos);
		pPlayer->Render(pViewport);
	}

	return true;
}


