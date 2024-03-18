/*
 * FILE: ML_GamePlay.h
 *
 * DESCRIPTION: Game play class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_GAMEPLAY_H_
#define _ML_GAMEPLAY_H_

#include "ML_Global.h"
#include "ML_LevelData.h"
#include "ML_GameParams.h"
#include "ML_GeneralSet.h"
#include "ML_LevelFiles.h"
#include "ML_MsgDef.h"
#include "ML_ManagerDef.h"
#include "ML_Mission.h"

#include "ML_LogicWorld.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

#define GETMLGAMEPLAY		((CMLGamePlay*)g_theGame.GetAGamePlay())
#define GETGENERALSET		GETMLGAMEPLAY->GetGeneralSet()
#define GETMAPDATA			GETMLGAMEPLAY->GetLogicWorld()->GetMapData()

#define GETPLAYERILDVALUE(iILD, iLevel) \
	GETMLGAMEPLAY->GetGeneralSet()->GetActorDataSet()->GetILD((iILD))->GetAt((iLevel))

#define GETAIILDVALUE(iILD, iLevel) \
	GETMLGAMEPLAY->GetGeneralSet()->GetAIDataSet()->GetILD((iILD))->GetAt((iLevel))

#define GETAIFLDVALUE(iFLD, iLevel) \
	GETMLGAMEPLAY->GetGeneralSet()->GetAIDataSet()->GetFLD((iFLD))->GetAt((iLevel))

#define INCSTATISTIC(iID, iNum, dwFlag) \
	GETMLGAMEPLAY->GetMission()->IncStatistic((iID), (iNum), (dwFlag))

#define GETCURUSERCONFIGS	GETMLGAMEPLAY->GetCurUserConfigs()
#define GETGAMELOGICPARAMS	GETMLGAMEPLAY->GetGameParams()
#define GETGAMESWITCH(i)	GETMLGAMEPLAY->GetGameSwitch((i))
#define GETFRAMECOUNT		GETMLGAMEPLAY->GetFrameCount()
#define POSTMESSAGE(pMsg) 	GETMLGAMEPLAY->PostMessage((pMsg))

#define MAXNUM_BRIEFLINE	10

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

class CMLManBase;
class CMLWndManager;
class CMLManPlayer;
class CMLManRoute;
class CMLManAI;
class CMLManMessage;
class CMLManBullet;
class CMLManImage;
class CMLManObject;
class CMLPlayer;

//	User's playing data
struct USERDATA
{
	AUString	strName;		//	Name
	int			iScore;			//	Score
	int			iNumPeople;		//	Number of killed people
	int			iNumVehicle;	//	Number of destroyed vehicles
	int			iNumPlanes;		//	Number of destroyed planes
	int			iMaxLevel;		//	Maximum level
};

//	Missoin brief
struct MISSIONBRIEF
{
	int			iNumLine;		//	Brief line
	AUString	aLines[MAXNUM_BRIEFLINE];	//	Brief strings
};

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLGamePlay
//
///////////////////////////////////////////////////////////////////////////

class CMLGamePlay : public AGamePlay
{
public:		//	Types

	//	View mode
	enum
	{
		VIEWMODE_PLAYER	= 0
	};

	//	Game player active states
	enum
	{
		GPS_NOTACTIVE	= 0,		//	Not active
		GPS_NORMALPLAY 	= 1,		//	Normal play
		GPS_POPCONSOLE	= 2,		//	Pop console
	};

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLGamePlay)

	CMLGamePlay();
	virtual ~CMLGamePlay() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);		//	Initialize object
	virtual bool	Release();				//	Release object

	virtual int		OnMsg(PAMSG pMsg);					//	Message handler
	virtual bool	Run(int& __nRet, float fTimeSpan);	//	Tick run
	virtual int		SetActiveStatus(int iActive);		//	Set active status
	virtual bool	TickAnimation(float fTimeSpan);		//	Tick animation

	bool			ResetScreen(int iWidth, int iHeight, int iColorBit);	//	Reset display mode
	bool			SetKeySetting(int iCommand, int iValue, int iPos);		//	Set key setting

	bool			StartGamePlay(int iLevel);	//	Start game playing
	bool			EndGamePlay();				//	End game playing
	bool			PostMessage(MLMSG* pMsg);	//	Post message
	CMLPlayer*		GetLocalPlayer();			//	Get local player
	void			RenderLightning(A3DViewport* pView);	//	Render lightning

	void			PassMission();				//	Pass current mission
	void			ReplayMission(bool bFail);	//	Replay current mission
	void			PauseSounds(bool bPause);	//	Pause sounds in game

	void			SetGameSwitch(int iSwitch, DWORD dwValue);	//	Set game switch
	DWORD			GetGameSwitch(int iSwitch);					//	Get game switch
	bool			ParseConsoleCmd(const char* szCmd);			//	Parse console command

	inline int		AddUserScore(int iScore);		//	Add user score
	inline int		AddKilledPeopleNum(int iNum);	//	Add number of killed people
	inline int		AddKilledVehicleNum(int iNum);	//	Add number of destroyed vehicle
	inline int		AddKilledPlaneNum(int iNum);	//	Add number of destroyed plane

	CMLGeneralSet*	GetGeneralSet()			{	return &m_GeneralSet;		}
	USERCONFIGS*	GetCurUserConfigs()		{	return &m_UserConfigs;		}
	DWORD			GetFrameCount()			{	return m_dwFrameCnt;		}
	int				GetCurrentMission()		{	return m_iMission;			}
	bool			UpdateSound()			{	return m_bUpdateSound;		}
	CMLMission*		GetMission()			{	return &m_Mission;			}
	USERDATA*		GetUserData()			{	return &m_UserData;			}
	float			GetTimeSpan()			{	return m_fTimeSpan;			}
	MISSIONBRIEF*	GetMissionBrief()		{	return &m_MissionBrief;		}

	CMLManPlayer*	GetPlayerMan()			{	return (CMLManPlayer*)m_aManagers[MANID_PLAYER];	}
	CMLManRoute*	GetRouteMan()			{	return (CMLManRoute*)m_aManagers[MANID_ROUTE];		}
	CMLManAI*		GetAIMan()				{	return (CMLManAI*)m_aManagers[MANID_AI];			}
	CMLManBullet*	GetBulletMan()			{	return (CMLManBullet*)m_aManagers[MANID_BULLET];	}
	CMLManObject*	GetObjectMan()			{	return (CMLManObject*)m_aManagers[MANID_OBJECT];	}
	CMLManMessage*	GetMessageMan()			{	return (CMLManMessage*)m_aManagers[MANID_MESSAGE];	}
	CMLManImage*	GetImageMan()			{	return (CMLManImage*)m_aManagers[MANID_IMAGE];		}

	CMLLogicWorld*		GetLogicWorld()		{	return (CMLLogicWorld*)m_pAWorld;		}
	GAMELOGICPARAMS*	GetGameParams()		{	return m_GameParams.GetParams();		}
	CMLWndManager*		GetWndManager()		{	return (CMLWndManager*)m_pAInterface;	}

protected:	//	Attributes

	bool		m_bEnterGame;		//	Enter game flag
	int			m_iViewMode;		//	View mode
	DWORD		m_dwFrameCnt;		//	Frame counter
	int			m_iMission;			//	Current mission index
	int			m_iTimeCnt;			//	Level time counter
	int			m_iSndTimeCnt;		//	Sound update time counter
	bool		m_bUpdateSound;		//	Update sound flag
	float		m_fTimeSpan;		//	Time span
	bool		m_bSoundPause;		//	Flag of sound pause

	bool		m_bLightning;		//	true, play lightning
	int			m_iNextLightning;	//	Next lightning's time
	int			m_iNextThunder;		//	Next thunder's time
	int			m_idEnvSnd;			//	Environment sound's ID
	int			m_idThunderSnd;		//	Thunder sound's ID

	int			m_iMissionEnd;		//	Mission end flag. 1, pass; 0, not end; -1, failed
	int			m_iEndTimeCnt;		//	Time counter after mission end

	char		m_szMissionFile[MAX_PATH];	//	Current mission file

	CMLGeneralSet	m_GeneralSet;	//	General data set
	CMLLevelFiles	m_LvlFileMgr;	//	Level file manager
	USERCONFIGS		m_UserConfigs;	//	Current user's configs
	CMLGameParams	m_GameParams;	//	Game logic parameters
	GAMESWITCHES	m_GameSwitches;	//	Game dynamic switches
	CMLMission		m_Mission;		//	Mission
	USERDATA		m_UserData;		//	User playing data

	CMLManBase*		m_aManagers[NUM_MANAGER];	//	Manager pointers for convenience
	MISSIONBRIEF	m_MissionBrief;				//	Mission brief

protected:	//	Operations

	virtual bool	LogicRun(float __TimeSpan);	//	Logic run
	virtual void	AfterRender();				//	After render

	bool		CreateManagers();			//	Create managers
	bool		CreateMLWorld();			//	Create malah world object
	bool		CreateInterface();			//	Create interface
	bool		LoadAllGlobalSets();		//	Load all global sets
	void		DefaultUserData();			//	Default user data
	void		ResetUserData();			//	Reset user data

	bool		LoadSetting();				//	Load setting
	bool		SaveSetting();				//	Save setting
	void		PreloadResources();			//	Preload resources

	void		LoadMissionBrief(int iLevel);	//	Load mission brief

	void		PrintOnConsole(const char* szMsg, ...);	//	Print message on console
	void		OnMsg_Exit(MLMSG* pMsg);				//	Handle exit message
	void		OnMsg_Interface(MLMSG* pMsg);			//	Handle interface message
	void		OnMsg_MissionBrief(MLMSG* pMsg);		//	Handle mission brief message

	int			LoadMissionResources(int iLevel);		//	Load game mission resource
	void		ReleaseMissionResources();				//	Release mission resource
	bool		ResetMission();							//	Reset mission
	bool		BeginMission(int iLevel);				//	Begin current game mission
	bool		EndMission();							//	End current game mission

	void		HandleMissionEnding(bool bPass);		//	Handle mission ending
	void		SelectEnvironmentEffect();				//	Select environment effect
	void		StopEnvironmentEffect();				//	Stop environment effect
	void		LightningStrike(int iTimeCnt);			//	Ligthning strike

	bool		ParseSecretCmd(const char* szCmd);		//	Parse secret command
	bool		ParseUserCmd(const char* szCmd);		//	Parse user command
	bool		ParseDebugCmd(const char* szCmd);		//	Parse debug command
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

/*	Add user score
	
	Return result score.
*/
int	CMLGamePlay::AddUserScore(int iScore)
{
	m_UserData.iScore += iScore;
	if (m_UserData.iScore < 0)
		m_UserData.iScore = 0;

	return m_UserData.iScore;
}

//	Add number of killed people
int	CMLGamePlay::AddKilledPeopleNum(int iNum)
{
	m_UserData.iNumPeople += iNum;
	if (m_UserData.iNumPeople < 0)
		m_UserData.iNumPeople = 0;

	return m_UserData.iNumPeople;
}

//	Add number of destroyed vehicle
int	CMLGamePlay::AddKilledVehicleNum(int iNum)
{
	m_UserData.iNumVehicle += iNum;
	if (m_UserData.iNumVehicle < 0)
		m_UserData.iNumVehicle = 0;

	return m_UserData.iNumVehicle;
}

//	Add number of destroyed plane
int	CMLGamePlay::AddKilledPlaneNum(int iNum)
{
	m_UserData.iNumPlanes += iNum;
	if (m_UserData.iNumPlanes < 0)
		m_UserData.iNumPlanes = 0;

	return m_UserData.iNumPlanes;
}


#endif	//	_ML_GAMEPLAY_H_
