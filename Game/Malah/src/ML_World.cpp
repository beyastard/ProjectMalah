/*
 * FILE: ML_World.cpp
 *
 * DESCRIPTION: World class for Malah game
 *
 * CREATED BY: Duyuxin, 2002/11/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_World.h"
#include "ML_Scene.h"

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
//	Implement CMLWorld
//
///////////////////////////////////////////////////////////////////////////

CMLWorld::CMLWorld() : AWorld()
{
	m_pDirLight = NULL;
}

bool CMLWorld::Release()
{
	m_pDirLight = NULL;

	return AWorld::Release();
}

//	Create scene
bool CMLWorld::CreateScene(const PAWORLD_INIT_SCENE& __pDatum)
{
	DestroyScene();

	CMLScene* pMLScene = new CMLScene;
	if (!pMLScene)
	{
		ADebug::Msg(0, "CMLWorld::CreateScene, Failed to create scene object");
		return false;
	}

	m_pScn = pMLScene;
	m_pScn->BindDevice(m_pA3DDevice);

	return m_pScn->Create(__pDatum);
}

//	Change light
bool CMLWorld::ChangeLight(const PAWORLD_INIT_ENVIRONMENT __pDatum)
{
	//	Change ambient light
	m_pA3DDevice->SetAmbient(__pDatum->colorAmbient);

	//	Change directional light
	if (m_pDirLight)
	{
		A3DLIGHTPARAM Params = m_pDirLight->GetLightparam();

		Params.Diffuse		= ColorRGBAToColorValue(__pDatum->colorDirectional);
		Params.Direction 	= Normalize(__pDatum->vecLightDir);

		m_pDirLight->SetLightParam(&Params);
	}

	return true;
}

bool CMLWorld::ChangeVSSFile(const char* szVSSFile)
{
	if (!m_pScn)
		return false;

	CMLScene* pMLScene = (CMLScene*)m_pScn;

	return pMLScene->LoadVSSData(szVSSFile);
}


