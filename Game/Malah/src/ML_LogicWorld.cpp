/*
 * FILE: ML_LogicWorld.cpp
 *
 * DESCRIPTION: Logic world class for Malah game
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_LogicWorld.h"
#include "ML_SkySet.h"
#include "ML_EnvSet.h"
#include "ML_EnvSchemeSet.h"
#include "ML_GamePlay.h"
#include "ML_ManPlayer.h"
#include "ML_ManBullet.h"
#include "ML_ManAI.h"
#include "ML_ManObject.h"

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
//	Implement CMLLogicWorld
//
///////////////////////////////////////////////////////////////////////////

CMLLogicWorld::CMLLogicWorld() : CMLWorld()
{
	m_pCurScheme	= NULL;
	m_dwAmbient		= 0xffffffff;
	m_dwDiffuse		= 0xffffffff;
}

bool CMLLogicWorld::Release()
{
	ReleaseMissionResources();

	return CMLWorld::Release();
}

//	Change scene file
bool CMLLogicWorld::ChangeSceneFile(CMLSceneData* pData)
{
	AWORLD_INIT_SCENE SceneData;
	pData->ToKernelDatum(&SceneData);

	if (!CreateScene(&SceneData))
	{
		ADEBUG_TRACE("Failed to create scene.");
		return false;
	}

	return true;
}

//	Change sky
bool CMLLogicWorld::ChangeSky(CMLEnvScheme* pScheme, CMLSceneData* pData)
{
	AUString strName = pScheme->GetSkyName();
	CMLSkySet* pSkySet = GETGENERALSET->GetSkyDataSet();
	CMLSky* pSky = NULL;

	if (strName.IsEmpty())
	{
		//	Select sky randomly
		int iNum = pSkySet->GetSkyNum();
		if (!iNum)
			return true;	//	Not sky can be selected

		int iSel = rand() % iNum;
		pSky = pSkySet->SearchSky(iSel);
	}
	else
	{
		pSky = pSkySet->SearchSky(strName);
	}

	if (!pSky)
	{
		ADEBUG_TRACE("Failed to find proper sky.");
		return false;
	}

	AWORLD_INIT_SKY SkyData;
	pSky->ToKernelDatum(&SkyData);

	if (!CreateSky(&SkyData))
	{
		ADEBUG_TRACE("Failed to create sky.");
		return false;
	}

	return true;
}

//	Change environment
bool CMLLogicWorld::ChangeEnvironment(CMLEnvScheme* pScheme, CMLSceneData* pData)
{
	AUString strName = pScheme->GetEnvName();
	CMLEnvSet* pEnvSet = GETGENERALSET->GetEnvDataSet();
	CMLEnvironment* pEnv = NULL;

	if (strName.IsEmpty())
	{
		//	Select environment randomly
		int iNum = pEnvSet->GetEnvNum();
		if (!iNum)
			return true;	//	No environment can be selected

		int iSel = rand() % iNum;
		pEnv = pEnvSet->SearchEnv(iSel);
	}
	else
	{
		pEnv = pEnvSet->SearchEnv(strName);
	}

	if (!pEnv)
	{	
		ADEBUG_TRACE("Failed to find proper environment");
		return false;
	}

	AWORLD_INIT_ENVIRONMENT EnvData;
	pEnv->ToKernelDatum(&EnvData);

	m_pA3DDevice->SetFogEnable(true);

	if (!CreateEnvironment(&EnvData))
	{
		ADEBUG_TRACE("Failed to create environment.");
		return false;
	}

	//	Change light
	if (!ChangeLight(&EnvData))
	{
		ADEBUG_TRACE("Failed to change light.");
		return false;
	}

	m_dwAmbient = EnvData.colorAmbient;
	m_dwDiffuse = EnvData.colorDirectional;

	return true;
}

//	Change environment scheme
bool CMLLogicWorld::ChangeEnvScheme(CMLSceneData* pData)
{
	AUString strName = pData->GetSchemeName();
	CMLEnvSchemeSet* pSchemeSet = GETGENERALSET->GetEnvSchemeSet();

	m_pCurScheme = NULL;

	if (strName.IsEmpty())
	{
		//	Select scheme randomly
		int iNum = pSchemeSet->GetEnvSchemeNum();
		if (!iNum)
			return true;	//	Not scheme can be selected

		int iSel = rand() % iNum;
		m_pCurScheme = pSchemeSet->SearchEnvScheme(iSel);
	}
	else
	{
		m_pCurScheme = pSchemeSet->SearchEnvScheme(strName);
	}

	if (!m_pCurScheme)
	{
		ADEBUG_TRACE("Not find proper environment scheme.");
		return false;
	}

	if (!ChangeSky(m_pCurScheme, pData))
	{
		ADEBUG_TRACE("Failed to change sky.");
		return false;
	}

	if (!ChangeEnvironment(m_pCurScheme, pData))
	{
		ADEBUG_TRACE("Failed to change environment.");
		return false;
	}

	return true;
}

/*	Load game mission

	Return true for success, otherwise return false.

	ar: mission archive
*/
bool CMLLogicWorld::LoadMissionResources(AArchive& ar)
{
	//	Release old mission resources
	ReleaseMissionResources();

	if (!m_MapData.Init())
	{
		ADEBUG_TRACE("Failed to load initialize map data object.");
		return false;
	}

	//	Load map data
	if (!m_MapData.Load(ar))
	{
		ADEBUG_TRACE("Failed to load mission data.");
		return false;
	}

	CMLSceneData* pSceneData = m_MapData.GetSceneData();

	//	Implement scene file
	ChangeSceneFile(pSceneData);

	//	Implement environment scheme
	ChangeEnvScheme(pSceneData);

	//	Implement vss file
	if (!pSceneData->GetVSSFile().IsEmpty())
	{
		char szFile[MAX_PATH];
		sprintf(szFile, "%s%s", g_szWorkDir, pSceneData->GetVSSFile());

		if (ChangeVSSFile(szFile))
			ForceRenderAll(false);
		else
			ForceRenderAll(true);
	}
	else
		ForceRenderAll(true);

	return true;
}

//	Release mission resources
void CMLLogicWorld::ReleaseMissionResources()
{
	DestroySky();
	DestroyEnvironment();
	DestroyScene();

	m_MapData.Release();
}

//	Render solid
bool CMLLogicWorld::Render(A3DViewport* pCurrentViewport)
{
	if (!CMLWorld::Render(pCurrentViewport))
		return false;

	//	Render lightning
	GETMLGAMEPLAY->RenderLightning(pCurrentViewport);
	
	//	Render player, weapon etc.
	CMLManPlayer* pPlayerMan = GETMLGAMEPLAY->GetPlayerMan();
	if (pPlayerMan)
		pPlayerMan->Render(pCurrentViewport);

	//	Render general objects, helicopter, attacker...
	CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
	if (pObjectMan)
		pObjectMan->Render(pCurrentViewport);

	//	Render all AI
	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	if (pAIMan)
		pAIMan->Render(pCurrentViewport);

	//	Render bullet
	CMLManBullet* pBulletMan = GETMLGAMEPLAY->GetBulletMan();
	if (pBulletMan)
		pBulletMan->Render(pCurrentViewport);
	
	g_theGame.EnableAlphaTest(true, A3DCMP_GREATEREQUAL, 80);
	g_theGame.GetA3DEngine()->FlushMeshCollector(pCurrentViewport);
	g_theGame.EnableAlphaTest(false, A3DCMP_GREATEREQUAL, 0);

	return true;
}

//	Render alpha
bool CMLLogicWorld::RenderAlpha(A3DViewport* pCurrentViewport)
{
	g_theGame.EnableAlphaTest(true, A3DCMP_GREATEREQUAL, 8);

	if (!CMLWorld::RenderAlpha(pCurrentViewport))
		return false;

	g_theGame.EnableAlphaTest(false, A3DCMP_GREATEREQUAL, 0);

	return true;
}

/*	Get height of a position

	Return height of specified position.

	vPos: position given in 3D space
	pvNormal (out): used to receive normal
*/
float CMLLogicWorld::GetHeightOfPos(A3DVECTOR3& vPos, A3DVECTOR3* pvNormal/* NULL */)
{
	A3DVECTOR3 vStart(vPos.x, 500.0f, vPos.z);
	A3DVECTOR3 vDelta(0.0f, -1000.0f, 0.0f);
	RAYTRACE Trace;
	float fHeight = 0.0f;
	
	if (RayTraceSence(vStart, vDelta, 1.0f, &Trace))
	{
		fHeight = Trace.vPoint.y;

		if (pvNormal)
			*pvNormal = Trace.vNormal;
	}

	return fHeight;
}

/*	Ray trace routine

	Return true if ray collide something, otherwise return false.
	
	iBulletID: bullet's ID
	pTrace (out): used to receive result information
*/
bool CMLLogicWorld::RayTrace(int iBulletID, MLRAYTRACE* pTrace)
{
	assert(pTrace);
	pTrace->Trace.fFraction = 1.0f;

	MLRAYTRACE Trace;
	Trace.vStart	= pTrace->vStart;
	Trace.vVel		= pTrace->vVel;
	Trace.fTime		= pTrace->fTime;

	switch (iBulletID)
	{
	case ID_BULLET_MORTAR:
	case ID_BULLET_CANNON:	

		//	These kinds of bullet shouldn't use RayTrace

		return false;

	case ID_BULLET_FIREBALL:

		//	Trace ESP model
		if (RayTraceSence(Trace.vStart, Trace.vVel, Trace.fTime, &Trace.Trace) &&
			Trace.Trace.fFraction < pTrace->Trace.fFraction)
		{
			pTrace->iObjectType = MLTRACE_ESPMODEL;
			pTrace->Trace		= Trace.Trace;
		}

		break;

	case ID_BULLET_PISTOL:
	case ID_BULLET_RIFLE:
	case ID_BULLET_MACHINE:
	case ID_BULLET_RPG:
	{
		//	Trace ESP model
		if (RayTraceSence(Trace.vStart, Trace.vVel, Trace.fTime, &Trace.Trace) &&
			Trace.Trace.fFraction < pTrace->Trace.fFraction)
		{
			pTrace->iObjectType = MLTRACE_ESPMODEL;
			pTrace->Trace		= Trace.Trace;
		}

		//	Trace AI
		if (RayTrace_AI(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace = Trace;

		//	Trace enemy planes
		if (RayTrace_EnemyWeapon(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace = Trace;

		//	Trace fixed objects
		if (RayTrace_FixedObjects(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace = Trace;

		break;
	}
	case ID_BULLET_ROCKET:
	case ID_BULLET_FANTAN:
	case ID_BULLET_ARMORED:
	{
		//	Trace ESP model
		if (RayTraceSence(Trace.vStart, Trace.vVel, Trace.fTime, &Trace.Trace) &&
			Trace.Trace.fFraction < pTrace->Trace.fFraction)
		{
			pTrace->iObjectType = MLTRACE_ESPMODEL;
			pTrace->Trace		= Trace.Trace;
		}

		//	Trace player
		if (RayTrace_Player(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace	= Trace;

		//	Trace helicopter
		if (RayTrace_Copter(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace = Trace;

		//	Trace fixed objects
		if (RayTrace_FixedObjects(&Trace) && Trace.Trace.fFraction < pTrace->Trace.fFraction)
			*pTrace = Trace;

		break;
	}
	}

	if (pTrace->Trace.fFraction < 1.0f)
		return true;

	pTrace->iObjectType = MLTRACE_NONE;
	return false;
}

//	Ray trace routine only check AI.
bool CMLLogicWorld::RayTrace_AI(MLRAYTRACE* pTrace)
{
	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	assert(pAIMan);

	if (pAIMan->RayTrace(pTrace))
		return true;

	return false;
}

//	Ray trace routine only check player
bool CMLLogicWorld::RayTrace_Player(MLRAYTRACE* pTrace)
{
	//	Treat player as a sphere
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();

	if (pPlayer->RayTrace(pTrace))
		return true;

	return false;
}

//	Helicopter trace routine
bool CMLLogicWorld::RayTrace_Copter(MLRAYTRACE* pTrace)
{
	CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
	assert(pObjectMan);

	if (pObjectMan->CopterRayTrace(pTrace))
		return true;

	return false;
}

//	Enemy plane trace routine
bool CMLLogicWorld::RayTrace_EnemyWeapon(MLRAYTRACE* pTrace)
{
	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	assert(pAIMan);

	if (pAIMan->EnemyWeaponRayTrace(pTrace))
		return true;

	return false;
}

//	Fixed objects trace routine
bool CMLLogicWorld::RayTrace_FixedObjects(MLRAYTRACE* pTrace)
{
	CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
	assert(pObjectMan);

	if (pObjectMan->FixedObjectRayTrace(pTrace))
		return true;

	return false;
}

//	Bullet explode
void CMLLogicWorld::Explode(CMLBullet* pBullet, bool bFire)
{
	//	AI
	CMLManAI* pAIMan = GETMLGAMEPLAY->GetAIMan();
	assert(pAIMan);
	pAIMan->Explode(pBullet, bFire);

	if (!bFire)
	{
		//	Fixed objects
		CMLManObject* pObjectMan = GETMLGAMEPLAY->GetObjectMan();
		assert(pObjectMan);
		pObjectMan->FixedObjectExplode(pBullet, bFire);
	}
}


