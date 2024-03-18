/*
 * FILE: ML_GamePlaySwitch.cpp
 *
 * DESCRIPTION: Game play switches routines for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/10
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_GamePlay.h"
#include "ML_WndManager.h"
#include "ML_ManAI.h"
#include "ML_ManRoute.h"

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
//	Implement CMLGamePlay
//
///////////////////////////////////////////////////////////////////////////

/*	Set game switch

	iSwitch: switch ID.
	dwValue: new value for specified switch.
*/
void CMLGamePlay::SetGameSwitch(int iSwitch, DWORD dwValue)
{
	switch (iSwitch)
	{
	case SWITCH_UNDEAD:

		m_GameSwitches.bUndead = dwValue ? true : false;
		break;

	case SWITCH_LOCKSYSMOUSE:

		m_GameSwitches.bLockSysMouse = dwValue ? true : false;
		break;

	case SWITCH_UNLIMITEDPISTOL:

		m_GameSwitches.bUnlimitedPistol = dwValue ? true : false;
		break;

	case SWITCH_UNLIMITEDRIFLE:

		m_GameSwitches.bUnlimitedRifle = dwValue ? true : false;
		break;

	case SWITCH_UNLIMITEDMORTAR:

		m_GameSwitches.bUnlimitedMortar = dwValue ? true : false;
		break;

	case SWITCH_UNLIMITEDMACHINE:

		m_GameSwitches.bUnlimitedMachine = dwValue ? true : false;
		break;

	case SWITCH_UNLIMITEDRPG:

		m_GameSwitches.bUnlimitedRPG = dwValue ? true : false;
		break;

	case SWITCH_ADJUSTGUNPOS:

		m_GameSwitches.bAdjustGunPos = dwValue ? true : false;
		break;
	}
}

/*	Get game switch

	Return value for specified switch.

	iSwitch: switch ID.
*/
DWORD CMLGamePlay::GetGameSwitch(int iSwitch)
{
	DWORD dwValue;

	switch (iSwitch)
	{
	case SWITCH_UNDEAD:

		dwValue = m_GameSwitches.bUndead ? 1 : 0;
		break;

	case SWITCH_LOCKSYSMOUSE:

		dwValue = m_GameSwitches.bLockSysMouse ? 1 : 0;
		break;

	case SWITCH_UNLIMITEDPISTOL:

		dwValue = m_GameSwitches.bUnlimitedPistol ? 1 : 0;
		break;

	case SWITCH_UNLIMITEDRIFLE:

		dwValue = m_GameSwitches.bUnlimitedRifle ? 1 : 0;
		break;

	case SWITCH_UNLIMITEDMORTAR:

		dwValue = m_GameSwitches.bUnlimitedMortar ? 1 : 0;
		break;

	case SWITCH_UNLIMITEDMACHINE:

		dwValue = m_GameSwitches.bUnlimitedMachine ? 1 : 0;
		break;

	case SWITCH_UNLIMITEDRPG:

		dwValue = m_GameSwitches.bUnlimitedRPG ? 1 : 0;
		break;

	case SWITCH_ADJUSTGUNPOS:

		dwValue = m_GameSwitches.bAdjustGunPos ? 1 : 0;
		break;
	}

	return dwValue;
}

//	Print message on console
void CMLGamePlay::PrintOnConsole(const char* szMsg, ...)
{
	static char szStr[512];

	va_list vaList;

	va_start(vaList, szMsg);
	vsprintf(szStr, szMsg, vaList);
	va_end(vaList);

	CMLWndManager* pInterface = GetWndManager();
	if (pInterface)
		pInterface->AddConsoleString(szStr);
}

/*	Parse console command.

	Return true if command was parsed successfully, otherwise return false.

	szCmd: command line string
*/
bool CMLGamePlay::ParseConsoleCmd(const char* szCmd)
{
	if (!szCmd || !szCmd[0])
		return false;

	if ((szCmd[0] == 'd' || szCmd[0] == 'D') && szCmd[1] == '_')
	{
		if (ParseDebugCmd(szCmd))
			return true;
	}
	else if ((szCmd[0] == 's' || szCmd[0] == 'S') && szCmd[1] == '_')
	{
		if (ParseSecretCmd(szCmd))
			return true;
	}
	else if (ParseUserCmd(szCmd))
		return true;

	PrintOnConsole("Unknown command: %s", szCmd);

	return false;
}

//	Dispose user command
bool CMLGamePlay::ParseUserCmd(const char* szCmd)
{
	bool bRet = true;
	bool bPrintfCmd = true;

	if (!_stricmp(szCmd, "exit"))
	{
		MLMSG Msg;
		Msg.dwMsg	 = MSG_EXITGAME;
		Msg.dwParam1 = EXIT_CODE_NORMAL;
		OnMsg_Exit(&Msg);
	}
	else if (!_stricmp(szCmd, "fps_on"))
		g_theGame.GetA3DEngine()->SetShowFPSFlag(true);
	else if (!_stricmp(szCmd, "fps_off"))
		g_theGame.GetA3DEngine()->SetShowFPSFlag(false);
	else if (!_stricmp(szCmd, "vss_on"))
		GetLogicWorld()->ForceRenderAll(false);
	else if (!_stricmp(szCmd, "vss_off"))
		GetLogicWorld()->ForceRenderAll(true);
	else
	{
		bRet = false;
		bPrintfCmd = false;
	}

	if (bPrintfCmd)
		PrintOnConsole(szCmd);

	return bRet;
}

//	Parse secret command
bool CMLGamePlay::ParseSecretCmd(const char* szCmd)
{
	static bool bEnable = true;
	if (!bEnable)
		return false;

	bool bRet = true;
	bool bPrintfCmd = true;

	if (!_stricmp(szCmd, "s_undead_on"))
		SetGameSwitch(SWITCH_UNDEAD, 1);
	else if (!_stricmp(szCmd, "s_undead_off"))
		SetGameSwitch(SWITCH_UNDEAD, 0);
	else if (!_stricmp(szCmd, "s_full_ammo_on"))
	{
		SetGameSwitch(SWITCH_UNLIMITEDPISTOL, 1);
		SetGameSwitch(SWITCH_UNLIMITEDRIFLE, 1);
		SetGameSwitch(SWITCH_UNLIMITEDMORTAR, 1);
		SetGameSwitch(SWITCH_UNLIMITEDMACHINE, 1);
		SetGameSwitch(SWITCH_UNLIMITEDRPG, 1);
	}
	else if (!_stricmp(szCmd, "s_full_ammo_off"))
	{
		SetGameSwitch(SWITCH_UNLIMITEDPISTOL, 0);
		SetGameSwitch(SWITCH_UNLIMITEDRIFLE, 0);
		SetGameSwitch(SWITCH_UNLIMITEDMORTAR, 0);
		SetGameSwitch(SWITCH_UNLIMITEDMACHINE, 0);
		SetGameSwitch(SWITCH_UNLIMITEDRPG, 0);
	}
	else if (!_stricmp(szCmd, "s_i_like_pistol"))
		SetGameSwitch(SWITCH_UNLIMITEDPISTOL, 1);
	else if (!_stricmp(szCmd, "s_i_dislike_pistol"))
		SetGameSwitch(SWITCH_UNLIMITEDPISTOL, 0);
	else if (!_stricmp(szCmd, "s_i_like_rifle"))
		SetGameSwitch(SWITCH_UNLIMITEDRIFLE, 1);
	else if (!_stricmp(szCmd, "s_i_dislike_rifle"))
		SetGameSwitch(SWITCH_UNLIMITEDRIFLE, 0);
	else if (!_stricmp(szCmd, "s_i_like_mortar"))
		SetGameSwitch(SWITCH_UNLIMITEDMORTAR, 1);
	else if (!_stricmp(szCmd, "s_i_dislike_mortar"))
		SetGameSwitch(SWITCH_UNLIMITEDMORTAR, 0);
	else if (!_stricmp(szCmd, "s_i_like_machinegun"))
		SetGameSwitch(SWITCH_UNLIMITEDMACHINE, 1);
	else if (!_stricmp(szCmd, "s_i_dislike_machinegun"))
		SetGameSwitch(SWITCH_UNLIMITEDMACHINE, 0);
	else if (!_stricmp(szCmd, "s_i_like_rpg"))
		SetGameSwitch(SWITCH_UNLIMITEDRPG, 1);
	else if (!_stricmp(szCmd, "s_i_dislike_rpg"))
		SetGameSwitch(SWITCH_UNLIMITEDRPG, 0);
	else if (!_stricmp(szCmd, "s_pass"))
		HandleMissionEnding(true);
	else if (!_stricmp(szCmd, "s_to_die"))
		HandleMissionEnding(false);
	else if (!_stricmp(szCmd, "s_all_tasks"))
		m_UserData.iMaxLevel = m_GameParams.GetParams()->iMaxMission;
	else
	{
		bRet = false;
		bPrintfCmd = false;
	}

	if (bPrintfCmd)
		PrintOnConsole(szCmd);

	return bRet;
}

//	Dispose debug command
bool CMLGamePlay::ParseDebugCmd(const char* szCmd)
{
	static bool bEnable = true;
	if (!bEnable)
		return false;

	bool bRet = true;
	bool bPrintfCmd = true;

	if (!_stricmp(szCmd, "d_adjustweapon_on"))
		SetGameSwitch(SWITCH_ADJUSTGUNPOS, 1);
	else if (!_stricmp(szCmd, "d_adjustweapon_off"))
		SetGameSwitch(SWITCH_ADJUSTGUNPOS, 0);
	else if (!_stricmp(szCmd, "d_lockmouse_on"))
		SetGameSwitch(SWITCH_LOCKSYSMOUSE, 1);
	else if (!_stricmp(szCmd, "d_lockmouse_off"))
		SetGameSwitch(SWITCH_LOCKSYSMOUSE, 0);
	else if (!_stricmp(szCmd, "d_statistics"))
	{
		bPrintfCmd = false;
		PrintOnConsole(szCmd);

		CMLMission::STATISTIC* pStatistics = m_Mission.GetStatistics();
		const CMLManRoute::AINUM* pTotalAI = GETMLGAMEPLAY->GetRouteMan()->GetAITotalGenNum();
		
		PrintOnConsole("---------------------------------------");

		char szStr[100];
		sprintf(szStr, "<= Killed people => F:%d, G:%d, R:%d", pStatistics->iNumDeadFootman,
				pStatistics->iNumDeadGuerilla, pStatistics->iNumDeadRocket);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Destroyed plane => %d", pStatistics->iNumDeadPlane);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Unit in preserved area => %d", pStatistics->iNumInPreserve);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Copter lost => %d", pStatistics->iNumCopter);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Time, AI Gen / Level => %d / %d (s)", GETAIILDVALUE(ILD_TIME_AIGEN, m_iMission), m_iTimeCnt / 1000);
		PrintOnConsole(szStr);

		CMLManAI* pAIMan = GetAIMan();
		sprintf(szStr, "<= Current => People:%d, Plane:%d, Vehicle:%d", pAIMan->GetCurAINum(),
				pAIMan->GetEnemyPlaneNum(), pAIMan->GetCurEWNum()-pAIMan->GetEnemyPlaneNum());
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Truck => Destroyed:%d, Esc:%d, Total:%d", pStatistics->iNumDeadTruck,
				pStatistics->iNumEscTruck, pTotalAI->iNumTruck);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Tank => Destroyed:%d, Esc:%d, Total:%d", pStatistics->iNumDeadTank,
				pStatistics->iNumEscTank, pTotalAI->iNumTank);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Armored Car => Destroyed:%d, Esc:%d, Total:%d", pStatistics->iNumDeadArmored,
				pStatistics->iNumEscArmored, pTotalAI->iNumArmored);
		PrintOnConsole(szStr);

		sprintf(szStr, "<= Potential unit => %s", GetRouteMan()->AIGenerationOver() ? "No" : "Yes");
		PrintOnConsole(szStr);

		PrintOnConsole("---------------------------------------");
	}
	else if (!_stricmp(szCmd, "d_logictime"))
	{
		bPrintfCmd = false;
		PrintOnConsole(szCmd);

		char szStr[100];
		DWORD dwTime = 0;

		sprintf(szStr, "Message manager %u", m_aManagers[MANID_MESSAGE]->GetLogicTime());
		dwTime += m_aManagers[MANID_MESSAGE]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Player manager %u", m_aManagers[MANID_PLAYER]->GetLogicTime());
		dwTime += m_aManagers[MANID_PLAYER]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "AI manager %u", m_aManagers[MANID_AI]->GetLogicTime());
		dwTime += m_aManagers[MANID_AI]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Route manager %u", m_aManagers[MANID_ROUTE]->GetLogicTime());
		dwTime += m_aManagers[MANID_ROUTE]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Bullet manager %u", m_aManagers[MANID_BULLET]->GetLogicTime());
		dwTime += m_aManagers[MANID_BULLET]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Image manager %u", m_aManagers[MANID_IMAGE]->GetLogicTime());
		dwTime += m_aManagers[MANID_IMAGE]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Object manager %u", m_aManagers[MANID_OBJECT]->GetLogicTime());
		dwTime += m_aManagers[MANID_OBJECT]->GetLogicTime();
		PrintOnConsole(szStr);

		sprintf(szStr, "Total time: %u", dwTime);
		PrintOnConsole(szStr);
	}
	else if (!_stricmp(szCmd, "d_renderall_on"))
		m_pAWorld->ForceRenderAll(true);
	else if (!_stricmp(szCmd, "d_renderall_off"))
		m_pAWorld->ForceRenderAll(false);
	else if (!_stricmp(szCmd, "d_aiinfo"))
		GetAIMan()->OutputDebugInfo();
	else if (!_stricmp(szCmd, "d_ambientcolor"))
	{
		bPrintfCmd = false;
		PrintOnConsole(szCmd);

		char szStr[50];
		sprintf(szStr, "Ambient color: %x", g_theGame.GetA3DEngine()->GetA3DDevice()->GetAmbientColor());
		PrintOnConsole(szStr);
	}
	else if (!_stricmp(szCmd, "d_directionlight"))
	{
		bPrintfCmd = false;
		PrintOnConsole(szCmd);

		A3DLIGHTPARAM Params = g_theGame.GetA3DLight()->GetLightparam();

		char szStr[100];
		sprintf(szStr, "On/Off: %s", g_theGame.GetA3DLight()->IsOn() ? "On" : "Off");
		PrintOnConsole(szStr);

		sprintf(szStr, "Direction: %f, %f, %f", Params.Direction.x, Params.Direction.y, Params.Direction.z);
		PrintOnConsole(szStr);

		sprintf(szStr, "Diffuse: %x", ColorValueToColorRGBA(Params.Diffuse));
		PrintOnConsole(szStr);
	}
	else
	{
		bRet = false;
		bPrintfCmd = false;
	}

	if (bPrintfCmd)
		PrintOnConsole(szCmd);

	return bRet;
}

