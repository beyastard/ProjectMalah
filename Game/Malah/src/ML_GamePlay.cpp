/*
 * FILE: ML_GamePlay.cpp
 *
 * DESCRIPTION: Game play class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_GamePlay.h"
#include "ML_Resources.h"
#include "ML_ManagerDef.h"
#include "ML_WndManager.h"
#include "ML_Weapon.h"
#include "ML_MainFrame.h"

#include "ML_ManMessage.h"
#include "ML_ManPlayer.h"
#include "ML_ManAI.h"
#include "ML_ManRoute.h"
#include "ML_ManBullet.h"
#include "ML_ManImage.h"
#include "ML_ManObject.h"
#include "ML_TransKeyboard.h"
#include "ML_Interface.h"
#include "ML_WndScore.h"

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

extern CMLMainFrame		g_MainFrame;
extern char				g_szDefUserName[];
extern char				g_szSettingFile[];

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

IMPLEMENT_ASERIAL(CMLGamePlay, AGamePlay, 1)

CMLGamePlay::CMLGamePlay() : AGamePlay()
{
	m_nActive		= GPS_NOTACTIVE;
	m_dwFrameCnt	= 0;
	m_iViewMode		= VIEWMODE_PLAYER;
	m_iMission		= -1;
	m_iSndTimeCnt	= 0;
	m_bRain			= false;
	m_bSnow			= false;
	m_bLightning	= false;
	m_idThunderSnd	= -1;
	m_idEnvSnd		= -1;
	m_bEnterGame	= false;
	m_bSoundPause	= false;
	m_iMissionEnd	= 0;

	m_szMissionFile[0] = '\0';

	memset(m_aManagers, 0, sizeof (m_aManagers));
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLGamePlay::Init(AGame* pGame)
{
	//	Set default user configs and game switches
	ML_DefaultUserConfigs(&m_UserConfigs);
	ML_DefaultGameSwitches(&m_GameSwitches);

	//	Set default user data
	DefaultUserData();

	//	Call SetIsResetAGameRes here so that GamePlay::Init won't release
	//	evironment effect (rain and snow) !!
	SetIsResetAGameRes(false);

	if (!AGamePlay::Init(pGame))
	{
		ADEBUG_TRACE("Failed to initialize game play object");
		return false;
	}

	if (!CreateMLWorld())
	{
		ADEBUG_TRACE("Failed to initialize malah world oject");
		return false;
	}

	if (!CreateManagers())
	{
		ADEBUG_TRACE("Failed to create manangers");
		return false;
	}

	//	Load all global data and set. Do this before CreateInterface()
	if (!LoadAllGlobalSets())
	{
		ADEBUG_TRACE("Failed to load global data and set");
		return false;
	}

	//	Create interface. Do this after LoadAllGlobalSets()
	if (!CreateInterface())
	{
		ADEBUG_TRACE("Failed to create interface");
		return false;
	}

	return true;
}

//	Release object
bool CMLGamePlay::Release()
{
	m_GeneralSet.Release();
	m_LvlFileMgr.Release();

	memset(m_aManagers, 0, sizeof (m_aManagers));

	return AGamePlay::Release();
}

/*	Create managers

	Return true for success, otherwise return false.
*/
bool CMLGamePlay::CreateManagers()
{
	struct _MANINFO
	{
		AClassInfo* pClassInfo;
		DWORD		dwMask;
	};
	
	_MANINFO aManInfos[NUM_MANAGER] = 
	{
		{	CLASS_INFO(CMLManMessage),	MASK_MESSAGEMAN		},
		{	CLASS_INFO(CMLManPlayer),	MASK_PLAYERMAN		},
		{	CLASS_INFO(CMLManRoute),	MASK_ROUTEMAN		},
		{	CLASS_INFO(CMLManBullet),	MASK_BULLETMAN		},
		{	CLASS_INFO(CMLManAI),		MASK_AIMAN			},
		{	CLASS_INFO(CMLManImage),	MASK_IMAGEMAN		},
		{	CLASS_INFO(CMLManObject),	MASK_OBJECTMAN		},
	};

	int i;

	for (i=0; i < NUM_MANAGER; i++)
	{
		if (!(GetBits() & aManInfos[i].dwMask))
			continue;

		//	Create mananger
		AClassInfo* pClassInfo = aManInfos[i].pClassInfo;

		//	Get object creating object
		CREATEMETHOD lpfnCreateObject = pClassInfo->GetCreateMethod();
		if (!lpfnCreateObject)
		{
			ADEBUG_TRACE("Object create funciton is NULL");
			return false;
		}
		
		//	Create manager object
		AManager* pMan = (AManager*)lpfnCreateObject();
		if (!pMan)
		{
			ADEBUG_TRACE("Failed to create manager object");
			return false;
		}
		
		if (!pMan->Init(m_pAGame))
		{
			ADEBUG_TRACE("Failed to initialize manager object");
			return false;
		}

		AddManPtr(pMan);
	}

	//	Prepare pointers for convenience. 
	//	Don't put these lines into above loop
	m_aManagers[MANID_MESSAGE]	= (CMLManBase*)GetManagerById(MANID_MESSAGE);
	m_aManagers[MANID_PLAYER]	= (CMLManBase*)GetManagerById(MANID_PLAYER);
	m_aManagers[MANID_AI]		= (CMLManBase*)GetManagerById(MANID_AI);
	m_aManagers[MANID_ROUTE]	= (CMLManBase*)GetManagerById(MANID_ROUTE);
	m_aManagers[MANID_BULLET]	= (CMLManBase*)GetManagerById(MANID_BULLET);
	m_aManagers[MANID_IMAGE]	= (CMLManBase*)GetManagerById(MANID_IMAGE);
	m_aManagers[MANID_OBJECT]	= (CMLManBase*)GetManagerById(MANID_OBJECT);

	//	Put image manager to render tail
	SetLastDrawMan(m_aManagers[MANID_IMAGE]);

	return true;
}

/*	Create malah world object

	Return true for success, otherwise return false.
*/
bool CMLGamePlay::CreateMLWorld()
{
	CMLLogicWorld* pWorld = new CMLLogicWorld;
	if (!pWorld)
	{
		ADEBUG_TRACE("Not enough memory.");
		return false;
	}

	pWorld->BindEngine(m_pAGame->GetA3DEngine());
	pWorld->SetA3DLight(m_pAGame->GetA3DLight());

	SetAWorld(pWorld);

	return true;
}

/*	Create interface

	Return true for success, otherwise return false.
*/
bool CMLGamePlay::CreateInterface()
{
	if (!(m_pAInterface = new CMLWndManager))
	{
		ADEBUG_TRACE("Failed to create window manager");
		return false;
	}

	AWC_INIT awc_param;
	memset(&awc_param, 0, sizeof (awc_param));
	awc_param.pGame = m_pAGame;

	if (!m_pAInterface->Init(awc_param))
	{
		ADEBUG_TRACE("Failed to initialize interface");
		return false;
	}

	return true;
}

//	Load all global sets needed by game
bool CMLGamePlay::LoadAllGlobalSets()
{
	char szPathFile[MAX_PATH];
	sprintf(szPathFile, "%s%s", g_szWorkDir, g_szGenSetFile);

	//	Load general set
	if (!m_GeneralSet.Init())
	{
		ADEBUG_TRACE("Failed to initialize general set object");
		return false;
	}

	if (!m_GeneralSet.Load(szPathFile))
	{
		ADEBUG_TRACE("Failed to load general set.");
		return false;
	}

	//	Load level file descriptions
	sprintf(szPathFile, "%s%s", g_szWorkDir, RES_GetOtherFile(RES_LEVELDESCFILE));
	if (!m_LvlFileMgr.Load(szPathFile))
	{
		ADEBUG_TRACE("Failed to load level-file description file.");
		m_LvlFileMgr.Release();
		return false;
	}

	//	Load logic parameters from file
	sprintf(szPathFile, "%s%s", g_szWorkDir, RES_GetOtherFile(RES_PARAMETERFILE));
	if (!m_GameParams.Load(szPathFile))
	{
		ADEBUG_TRACE("Failed to load game parameters from file.");
		return false;
	}

	return true;
}

/*	Post message

	Return true for success, otherwise return false.

	pMsg: message will be post.
*/
bool CMLGamePlay::PostMessage(MLMSG* pMsg)
{
	if (!m_aManagers[MANID_MESSAGE] || !(GetBits() & MASK_MESSAGEMAN))
		return false;

	pMsg->dwFrameCnt = m_dwFrameCnt;

	return GetMessageMan()->AddMsg(pMsg);
}

//	Message handler
int	CMLGamePlay::OnMsg(PAMSG pMsg)
{
	MLMSG* pMLMsg = (MLMSG*)pMsg;

	switch (pMLMsg->dwMsg)
	{
	case MSG_EXITGAME:
	
		OnMsg_Exit(pMLMsg);
		break;

	case MSG_INTERFACE:

		OnMsg_Interface(pMLMsg);
		break;

	case MSG_MISSIONBRIEF:
		
		OnMsg_MissionBrief(pMLMsg);
		break;
	}

	return 1;
}

//	Handle exit message
void CMLGamePlay::OnMsg_Exit(MLMSG* pMsg)
{
	if (m_bEnterGame)
		EndGamePlay();

	if (pMsg->dwParam1 == EXIT_CODE_NORMAL)		//	Exit game directly
	{
		m_exitCode = pMsg->dwParam1;
	}
	else if (pMsg->dwParam1 == 100)		//	Exit to main menu
	{
		SetActiveStatus(GPS_NOTACTIVE);
		m_pAInterface->SetActiveStatus(MLIG_LOADING);
	}

	return;
}

//	Handle interface message
void CMLGamePlay::OnMsg_Interface(MLMSG* pMsg)
{
	if (!m_pAInterface)
		return;

	PauseSounds(true);

	SetActiveStatus(pMsg->dwParam2);
	m_pAInterface->SetActiveStatus(pMsg->dwParam1);
}

//	Handle mission brief message
void CMLGamePlay::OnMsg_MissionBrief(MLMSG* pMsg)
{
	CMLManImage* pImgMan = GetImageMan();
	if (!pImgMan)
		return;

	if (pImgMan->GetDrawFlags() & IMGDRAW_MISSBRIEF)
		pImgMan->SetDrawFlags(pImgMan->GetDrawFlags() & ~IMGDRAW_MISSBRIEF);
	else
		pImgMan->SetDrawFlags(pImgMan->GetDrawFlags() | IMGDRAW_MISSBRIEF);
}

//	Tick run
bool CMLGamePlay::Run(int& __nRet, float fTimeSpan)
{
	m_fTimeSpan = fTimeSpan;

	m_dwFrameCnt++;

	if (fTimeSpan > 80.0f)
		fTimeSpan = 80.0f;

	return AGamePlay::Run(__nRet, fTimeSpan);
}

/*	Set active status

	Return previous status.

	iStatus: active status
*/
int	CMLGamePlay::SetActiveStatus(int iStatus)
{
	int iOld = AGamePlay::SetActiveStatus(iStatus);

	switch (iStatus)
	{
	case GPS_NORMALPLAY:
	
		if (m_iViewMode == VIEWMODE_PLAYER)
			GetPlayerMan()->BindTrans(TBT_MOUSEANDKB);
	
		break;

	case GPS_POPCONSOLE:

		if (m_iViewMode == VIEWMODE_PLAYER)
			GetPlayerMan()->BindTrans(TBT_ONLYMOUSE);

		break;
	}

	return iOld;
}

/*	Get local player

	Return local player object's address for success, otherwise return NULL
*/
CMLPlayer* CMLGamePlay::GetLocalPlayer()
{
	CMLPlayer* pPlayer = GetPlayerMan()->GetPlayerByID(ID_LOCALPLAYER);
	assert(pPlayer);
	return pPlayer;
}

bool CMLGamePlay::SetKeySetting(int iCommand, int iValue, int iPos)
{
	CMLManPlayer* pPlayerMan = GetPlayerMan();
	if (!pPlayerMan)
		return false;

	CMLTransKeyboard* pKeyBoard = (CMLTransKeyboard*)(pPlayerMan->GetTransKeyboard());
	return pKeyBoard ? pKeyBoard->SetKeySetting(iCommand, iValue, iPos) : false;
}

/*	Start a game playing

	iLevel: mission's level, which is also mission's index
*/
bool CMLGamePlay::StartGamePlay(int iLevel)
{
	ResetUserData();

	//	Load setting
	LoadSetting();

	//	Preload resources
	PreloadResources();
	
	//	Load mission resources
	if (!LoadMissionResources(iLevel))
	{
		ADEBUG_TRACE("Failed to load mission resources.");
		return false;
	}

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] && !m_aManagers[i]->OnStartGamePlay())
		{
			ADEBUG_TRACE1("Failed call manager(%d)->OnStartGamePlay!", i);
			return false;
		}
	}

	//	Begin mission
	if (!BeginMission(iLevel))
	{
		ADEBUG_TRACE("Failed to begin mission.");
		return false;
	}

	SetActiveStatus(CMLGamePlay::GPS_NORMALPLAY);

	m_bEnterGame = true;

	return true;
}

//	End game playing and ready to return interface module
bool CMLGamePlay::EndGamePlay()
{
	//	End current mission
	EndMission();

	//	Release mission resources
	ReleaseMissionResources();

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] && !m_aManagers[i]->OnEndGamePlay())
		{
			ADEBUG_TRACE1("Failed call manager(%d)->OnEndGamePlay!", i);
			return false;
		}
	}

	m_szMissionFile[0] = '\0';

	if (m_pAInterface)
		((CMLWndManager*)m_pAInterface)->FreeInGameInterface();

	//	Save setting
	SaveSetting();

	//	Close all light and fog effect
	g_theGame.ResetEnvironment();

	//	Reset resources
	g_theGame.Reset();

	m_bEnterGame = false;

	return true;
}

/*	Load specified game mission's resources

	Return values:

		0, failure.
		1, load a new mission resources
		2, use current mission resources

	iLevel: mission's level, which is also mission's index
*/
int CMLGamePlay::LoadMissionResources(int iLevel)
{
	//	Get mission file's name
	AUString strLevelFile;
	if (!m_LvlFileMgr.GetLevelFile(iLevel, &strLevelFile))
	{
		ADEBUG_TRACE1("Failed to get mission %d file name.", iLevel);
		return 0;
	}

	//	Load mission brief
	LoadMissionBrief(iLevel);

	//	If requested mission file is same as current one, just return true
	if (m_szMissionFile[0] && !strcmp(m_szMissionFile, (const char*)strLevelFile))
		return 2;

	//	Release old resources
	ReleaseMissionResources();

	//	Load mission file ...
	char szFile[MAX_PATH];
	sprintf(szFile, "%s%s", g_szWorkDir, (const char*)strLevelFile);

	//	Open file
	AUFile File;
	if (!File.Create(strLevelFile, OPEN_EXISTING))
	{
		ADEBUG_TRACE1("Failed to open file %s.", strLevelFile);
		return 0;
	}

	AArchive ar(&File, false);

	//	Load scene resources
	if (!((CMLLogicWorld*)m_pAWorld)->LoadMissionResources(ar))
	{
		ADEBUG_TRACE("Failed to call world's LoadMissionResources!");
		return 0;
	}

	//	Load route resources
	if (m_aManagers[MANID_ROUTE] && !(GetRouteMan()->LoadMissionResources(ar)))
	{
		ADEBUG_TRACE("Failed to call route's LoadMissionResources!");
		return 0;
	}

	//	Load object resources
	if (m_aManagers[MANID_OBJECT] && !(GetObjectMan()->LoadMissionResources(ar)))
	{
		ADEBUG_TRACE("Failed to call object's LoadMissionResources!");
		return 0;
	}

	//	Save mission file name
	strcpy(m_szMissionFile, strLevelFile);

	//	Set ambient color for lightning
	if (g_theGame.GetA3DLightning())
		g_theGame.GetA3DLightning()->SetAmbient(GetLogicWorld()->GetAmbient());
	
	return 1;
}

//	Release mission resource
void CMLGamePlay::ReleaseMissionResources()
{
	//	Handle world
	GetLogicWorld()->ReleaseMissionResources();

	//	Handle managers
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->ReleaseMissionResources();
	}
}

/*	Reset mission

	Return true for success, otherwise return false.
*/
bool CMLGamePlay::ResetMission()
{
	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] && !m_aManagers[i]->OnResetMission())
		{
			ADEBUG_TRACE1("Failed call manager(%d)->OnResetMission!", i);
			return false;
		}
	}

	m_Mission.ResetMission();

	SelectEnvironmentEffect();

	m_iTimeCnt		= 0;
	m_bSoundPause	= false;
	m_iMissionEnd	= 0;

	//	Clear kill number every scene
	m_UserData.iNumPeople	= 0;
	m_UserData.iNumVehicle	= 0;
	m_UserData.iNumPlanes	= 0;

	return true;
}

//	Begin current game mission
bool CMLGamePlay::BeginMission(int iLevel)
{
	assert(iLevel >= 0);

	m_iMission		= iLevel;
	m_iTimeCnt		= 0;
	m_bSoundPause	= false;
	m_iMissionEnd	= 0;

	//	Clear kill number every scene
	m_UserData.iNumPeople	= 0;
	m_UserData.iNumVehicle	= 0;
	m_UserData.iNumPlanes	= 0;

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] && !m_aManagers[i]->OnBeginMission(m_iMission))
		{
			ADEBUG_TRACE1("Failed call manager(%d)->OnBeginMission!", i);
			return false;
		}
	}

	CMLMissionData* pMission = m_GeneralSet.GetMissionSet()->GetMissionData(iLevel);
	m_Mission.BeginMission(pMission);

	//	Begin snow and rain
	SelectEnvironmentEffect();

	return true;
}

//	End current game mission
bool CMLGamePlay::EndMission()
{
	if (m_iMission < 0)
		return false;

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i] && !m_aManagers[i]->OnEndMission())
		{
			ADEBUG_TRACE1("Failed call manager(%d)->OnEndMission!", i);
			return false;
		}
	}

	//	Stop environment effect
	StopEnvironmentEffect();

	return true;
}

//	Logic run
bool CMLGamePlay::LogicRun(float __TimeSpan)
{
	int iTimeSpan = ML_ConvertTimeSpan(__TimeSpan);
	m_iTimeCnt += iTimeSpan;
	m_iSndTimeCnt += iTimeSpan;

	//	Time to update sound ?
	m_bUpdateSound = false;
	if (m_iSndTimeCnt >= 150)
	{
		m_bUpdateSound	= true;
		m_iSndTimeCnt	= 0;
	}

	bool bRet = AGamePlay::LogicRun(__TimeSpan);

	if (m_aManagers[MANID_AI])
		GetAIMan()->UpdateActions();

	//	Mission completed ?
	if (m_iMission >= 0 && !m_iMissionEnd)
	{
		int iRet = m_Mission.MissionCompleted(m_iTimeCnt);
		if (iRet)
		{
			m_iMissionEnd = iRet;
			m_iEndTimeCnt = 0;
		}
	}

	//	Time to do lightning strike ?
	if (m_bLightning && m_iTimeCnt >= m_iNextLightning)
		LightningStrike(m_iTimeCnt);

	if (m_bLightning && m_iTimeCnt >= m_iNextThunder)
	{
		m_idThunderSnd = RES_S2D_THUNDER1 + MLA_Random(0, 5);
		g_theGame.Play2DSound(m_idThunderSnd, false);
		m_iNextThunder = m_iNextLightning + MLA_Random(1500, 3000);
	}

	//	Mission end ?
	if (m_iMissionEnd)
	{	
		if ((m_iEndTimeCnt += iTimeSpan) >= 3000)	//	Wait 3 seconds
			HandleMissionEnding(m_iMissionEnd == 1 ? true : false);
	}

	return bRet;
}

//	Tick animation
bool CMLGamePlay::TickAnimation(float fTimeSpan)
{
	if (GetActiveStatus() == GPS_NOTACTIVE)
	{
		if (m_pAInterface)
			m_pAInterface->TickAnimation(fTimeSpan);
		
		return true;
	}

	AGamePlay::TickAnimation(fTimeSpan);

	if (m_bLightning && g_theGame.GetA3DLightning())
		g_theGame.GetA3DLightning()->TickAnimation();

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->TickAnimation();
	}

	return true;
}

//	After render
void CMLGamePlay::AfterRender()
{
	//	Render cannon video
	CMLManObject* pObjMan = GetObjectMan();
	if (pObjMan)
		pObjMan->RenderCannon(m_pAGame->GetA3DViewport());

	//	Output debug information here ...
	char szMsg[256];

	//	Output weapon's postion and direction that are relative to camera
	if (GetActiveStatus() != GPS_NOTACTIVE && GetGameSwitch(SWITCH_ADJUSTGUNPOS))
	{
		CMLWeapon* pWeapon;
		CMLPlayer* pPlayer = GetLocalPlayer();
		if (pPlayer && (pWeapon = pPlayer->GetCurrentWeapon()))
		{
			A3DVECTOR3 v = pWeapon->GetRelativePos();
			sprintf(szMsg, "relative pos: %f, %f, %f", v.x, v.y, v.z);
			g_theGame.DebugTextOut(10, 10, szMsg);

			v = pWeapon->GetRelativeDir();
			sprintf(szMsg, "relative dir: %f, %f, %f", v.x, v.y, v.z);
			g_theGame.DebugTextOut(10, 25, szMsg);
		}
	}
}

/*	Reset display mode.

	Return true for success, otherwise return false.

	iWidth, iHeight: sizeof of new display mode.
	iColotBit: color bit of new display mode.
*/
bool CMLGamePlay::ResetScreen(int iWidth, int iHeight, int iColorBit)
{
	if (iWidth == m_pAGame->GetClientWidth() &&
		iHeight == m_pAGame->GetClientHeight() && 
		iColorBit == m_pAGame->GetColorDepth())
		return true;
	
	m_pAGame->SetClientWidth(iWidth);
	m_pAGame->SetClientHeight(iHeight);
	m_pAGame->SetColorDepth(iColorBit);

	if (m_aManagers[MANID_IMAGE])
		((CMLManImage*)m_aManagers[MANID_IMAGE])->SetScaleFactor((float)iWidth / BASERENDER_WIDTH, (float)iHeight / BASERENDER_HEIGHT);
	
	DWORD dwFlags = SDM_WIDTH | SDM_HEIGHT;
	A3DFORMAT fmtTarget;
	
	switch (iColorBit)
	{
	case -1: 
		
		fmtTarget = A3DFMT_UNKNOWN;
		break;
		
	case 16:
		
		fmtTarget = m_pAGame->GetA3DEngine()->GetA3DDevice()->GetTarget16();
		dwFlags	 |= SDM_FORMAT;
		break;
		
	case 32:
		
		fmtTarget = m_pAGame->GetA3DEngine()->GetA3DDevice()->GetTarget32();
		dwFlags	 |= SDM_FORMAT;
		break;
	}
	
	//	Set new display mode to engine
	if (!m_pAGame->GetA3DEngine()->SetDisplayMode(iWidth, iHeight, fmtTarget,
											m_pAGame->GetIsWindow(), dwFlags))
	{
		ADEBUG_TRACE("Failed to set display mode");
		assert(0);
		return false;
	}
	
	g_MainFrame.MoveWindow(0, 0, iWidth, iHeight);

	A3DVIEWPORTPARAM ViewParams;
	memset(&ViewParams, 0, sizeof (ViewParams));
	ViewParams.MaxZ		= 1.0f;
	ViewParams.Width	= iWidth;
	ViewParams.Height	= iHeight;

	m_pAGame->GetA3DViewport()->SetParam(&ViewParams);

	//	Set size for interface

	//	Set size for loading... window

	return true;
}

//	Pass current mission
void CMLGamePlay::PassMission()
{
	//	End current mission
	EndMission();

	int iMission = m_iMission + 1;

	//	Load resources
	if (!LoadMissionResources(iMission))
	{
		ADEBUG_TRACE1("Failed to load mission %d resource", iMission);
		return;
	}

	BeginMission(iMission);
}

/*	Replay current mission

	bFail: true, replay mission after failure
*/
void CMLGamePlay::ReplayMission(bool bFail)
{
	if (bFail)
	{
		//	Save player's score
		USERDATA* pUserData = GETMLGAMEPLAY->GetUserData();
		CMLWndScore::SavePlayerScore(pUserData->strName, pUserData->iScore);

		ResetUserData();
	}

	//	End current mission
	EndMission();

	//	Reset
	ResetMission();
}

//	Default user playing data
void CMLGamePlay::DefaultUserData()
{
	m_UserData.strName		= g_szDefUserName;
	m_UserData.iScore		= 0;
	m_UserData.iNumPeople	= 0;
	m_UserData.iNumVehicle	= 0;
	m_UserData.iNumPlanes	= 0;
	m_UserData.iMaxLevel	= 0;
}

//	Reset user data
void CMLGamePlay::ResetUserData()
{
	m_UserData.iScore		= 0;
	m_UserData.iNumPeople	= 0;
	m_UserData.iNumVehicle	= 0;
	m_UserData.iNumPlanes	= 0;
}

//	Handle mission ending
void CMLGamePlay::HandleMissionEnding(bool bPass)
{
	MLMSG Msg;
	memset(&Msg, 0, sizeof (Msg));

	Msg.idManager	= -1;
	Msg.dwMsg		= MSG_INTERFACE;
	Msg.dwParam2	= CMLGamePlay::GPS_NOTACTIVE;

	if (bPass)
	{
		int iNextMission = m_iMission + 1;
		if (iNextMission > m_GameParams.GetParams()->iMaxMission)
		{
			//	Finished all mission
			m_UserData.iMaxLevel = m_GameParams.GetParams()->iMaxMission;

			Msg.dwParam1 = MLIG_VICTORY;
			OnMsg((AMSG*)&Msg);
		}
		else
		{
			if (iNextMission > m_UserData.iMaxLevel)
				m_UserData.iMaxLevel = iNextMission;

			Msg.dwParam1 = MLIG_PASS;
			OnMsg((AMSG*)&Msg);
		}
	}
	else	//	Failure
	{
		if (m_iMission > m_UserData.iMaxLevel)
			m_UserData.iMaxLevel = m_iMission;

		Msg.dwParam1 = MLIG_NOTPASS;
		OnMsg((AMSG*)&Msg);
	}
}

//	Load setting
bool CMLGamePlay::LoadSetting()
{
	AWndIniFile	IniFile;
	if (!IniFile.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot read ini file setting.ini");
		return false;
	}

	//	Load user name
	const char* pszName = NULL;
	if (!IniFile.ReadString("record", "username", pszName))
		m_UserData.strName = g_szDefUserName;
	else
		m_UserData.strName = pszName;

	//	Load maximum level
	int iLevel;
	if (!IniFile.ReadInt("record", "maxlevel", iLevel))
		m_UserData.iMaxLevel = 0;
	else
		m_UserData.iMaxLevel = iLevel;

	IniFile.Release();

	//	Apply system setting ...
	A3DTEXTURE_QUALITY aTextureQuality[] =
	{
		A3DTEXTURE_QUALITY_LOW,
		A3DTEXTURE_QUALITY_MEDIUM,
		A3DTEXTURE_QUALITY_HIGH,
	};

	g_pA3DConfig->SetTextureQuality(aTextureQuality[m_UserConfigs.iTexQuality]);
	g_pA3DConfig->SetFlagTrilinear(m_UserConfigs.iTextureFilter == 1 ? true : false);

	return true;
}

//	Save setting
bool CMLGamePlay::SaveSetting()
{
	AWndIniFile	IniFile;
	if (!IniFile.Init(g_szSettingFile))
	{
		ADEBUG_TRACE("Cannot load setting inifile.");
		return false;
	}

	IniFile.WriteInt("record", "maxlevel", m_UserData.iMaxLevel);
	ADEBUG_TRACE1("Maximum available mission: %d", m_UserData.iMaxLevel);
//	IniFile.WriteString("record", "username", m_UserData.strName);

	IniFile.Save();
	IniFile.Release();

	return true;
}

//	Select environment effect
void CMLGamePlay::SelectEnvironmentEffect()
{
	m_idEnvSnd		= -1;
	m_idThunderSnd	= -1;

	CMLEnvScheme* pScheme = GetLogicWorld()->GetCurScheme();
	if (!pScheme)
		return;

	if (!m_UserConfigs.bWeather)
		return;

	if (pScheme->GetName().FindString("_ÏÂÓê") >= 0)
	{
		if (g_theGame.GetA3DRain())
		{
			g_theGame.GetA3DRain()->StartRain();
			RainStart(true);
		}

		m_idEnvSnd = RES_S2D_RAIN1 + MLA_Random(0, 2);
		g_theGame.Play2DSound(m_idEnvSnd, true, SND2DTYPE_ENV);

		if (pScheme->GetName().FindString("_ÉÁµç") >= 0)
		{
			if (g_theGame.GetA3DLightning())
			{
				m_bLightning		= true;
				m_iNextLightning	= MLA_Random(15000, 40000);
				m_iNextThunder		= m_iNextLightning + MLA_Random(1500, 3000);
			}
		}
	}
	else
	{
		if (pScheme->GetName().FindString("_ÏÂÑ©") >= 0)
		{
			if (g_theGame.GetA3DSnow())
			{
				g_theGame.GetA3DSnow()->StartSnow();
				SnowStart(true);
			}
		}

		if (pScheme->GetName().FindString("_Ò¹Íí") >= 0)
		{
			m_idEnvSnd = RES_S2D_NIGHT1;
			g_theGame.Play2DSound(m_idEnvSnd, true, SND2DTYPE_ENV);
		}
		else	//	In sunny day
		{
			m_idEnvSnd = RES_S2D_SUNNY1 + MLA_Random(0, 2);
			g_theGame.Play2DSound(m_idEnvSnd, true, SND2DTYPE_ENV);
		}
	}
}

//	Stop environment effect
void CMLGamePlay::StopEnvironmentEffect()
{
	if (g_theGame.GetA3DRain())
		g_theGame.GetA3DRain()->Stop();

	if (g_theGame.GetA3DSnow())
		g_theGame.GetA3DSnow()->Stop();

	if (m_idEnvSnd >= 0)
		g_theGame.Stop2DSound(m_idEnvSnd, SND2DTYPE_ENV);

	if (m_idThunderSnd >= 0)
		g_theGame.Stop2DSound(m_idThunderSnd);

	m_idEnvSnd		= -1;
	m_idThunderSnd	= -1;

	m_bRain			= false;
	m_bSnow			= false;
	m_bLightning	= false;
}

//	Ligthning strike
void CMLGamePlay::LightningStrike(int iTimeCnt)
{
	if (!m_bLightning || !g_theGame.GetA3DLightning())
		return;

	float fx, fy, fz;

	fx = A3DRandGenerator::RandUniformFloat(-500.0f, 500.0f);
	fy = A3DRandGenerator::RandUniformFloat(-500.0f, 500.0f);
	fz = A3DRandGenerator::RandUniformFloat(1000.0f, 2000.0f);

	A3DCamera* pCamera = GETMLGAMEPLAY->GetLocalPlayer()->GetCamera();

	A3DVECTOR3 vFrom = pCamera->GetPos() + pCamera->GetDirH() * fz + 
					   pCamera->GetRight() * fx + A3DVECTOR3(0.0, 1000.0f + fy, 0.0);
	A3DVECTOR3 vDir	= pCamera->GetRight() + A3DVECTOR3(MLA_Random(-1.0f, 1.0f), MLA_Random(-1.0f, 0.0f), MLA_Random(-1.0f, 1.0f));

	g_theGame.GetA3DLightning()->ReCharge(vFrom, vDir, g_vAxisY);
	g_theGame.GetA3DLightning()->Strike();

	//	Next strike time
	m_iNextLightning = iTimeCnt + MLA_Random(15000, 40000);
}

//	Render lightning
void CMLGamePlay::RenderLightning(A3DViewport* pView)
{
	if (!m_bLightning || !g_theGame.GetA3DLightning())
		return;

	g_theGame.GetA3DLightning()->Render(pView);
}

//	Preload resources
void CMLGamePlay::PreloadResources()
{
	int i;

	A3DModel* pModel;
	for (i=0; i < NUM_RESMOXFILE; i++)
	{
		if (i == RES_MOX_GUERILLA || i == RES_MOX_ROCKETMAN)
			continue;

		if ((pModel = g_theGame.LoadA3DModel(i)))
			g_theGame.ReleaseA3DModel(pModel);
	}

	A3DGraphicsFX* pGFX;
	for (i=0; i < NUM_RESGFXFILE; i++)
	{
		if ((pGFX = g_theGame.LoadGFX(i)))
			g_theGame.ReleaseGFX(pGFX);
	}
}

//	Load mission brief
void CMLGamePlay::LoadMissionBrief(int iLevel)
{
	AWndIniFile	IniFile;
	if (!IniFile.Init("Ini\\MissionBrief.ini"))
	{
		ADEBUG_TRACE("Cannot read ini file MissionBrief.ini");
		return;
	}

	char szSec[20], szKey[20];
	sprintf(szSec, "mission_%d", iLevel+1);

	if (!IniFile.ReadInt(szSec, "count_line", m_MissionBrief.iNumLine))
	{
		m_MissionBrief.iNumLine = 0;
		ADEBUG_TRACE1("Failed to read count_line of mission brief %d", iLevel);
		IniFile.Release();
		return;
	}

	CLAMPVALUE(m_MissionBrief.iNumLine, 0, MAXNUM_BRIEFLINE);

	for (int i=0; i < m_MissionBrief.iNumLine; i++)
	{
		//	Load brief string
		sprintf(szKey, "line_%d", i);
		const char* pszBrief = NULL;

		if (!IniFile.ReadString(szSec, szKey, pszBrief))
			m_MissionBrief.aLines[i] = "";
		else
			m_MissionBrief.aLines[i] = pszBrief;
	}

	IniFile.Release();
}

//	Pause sounds in game
void CMLGamePlay::PauseSounds(bool bPause)
{
	if (m_bSoundPause == bPause)
		return;

	m_bSoundPause = bPause;

	for (int i=0; i < NUM_MANAGER; i++)
	{
		if (m_aManagers[i])
			m_aManagers[i]->PauseSound(bPause);
	}

	if (GetLocalPlayer())
		GetLocalPlayer()->StopFire();
}

