/*
 * FILE: ML_Scene.cpp
 *
 * DESCRIPTION: Scene class for Malah game
 *
 * CREATED BY: Duyuxin, 2002/11/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_Scene.h"
#include "ML_VSSetScene.h"

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
//	Implement CMLScene
//
///////////////////////////////////////////////////////////////////////////

CMLScene::CMLScene() : AScene()
{
}

bool CMLScene::Create(const PAWORLD_INIT_SCENE __pDatum)
{
	if (!BindFinished())
	{
		ADebug::Msg(0, "CMLScene::Create, Not bind device!");
		return false;
	}

	Release();

	CMLVSSetScene* pVSSScene = new CMLVSSetScene;
	if (!pVSSScene)
	{
		ADebug::Msg(0, "CMLScene::Create, Not enough memory to create Scene!");
		return false;
	}

	if (!pVSSScene->Init(m_pA3DDevice))
	{
		delete pVSSScene;
		ADebug::Msg(0, "CMLScene::Create, Error init A3DScene!");
		return false;
	}

	if (__pDatum->szModFile[0])
	{
		if (!pVSSScene->Import(__pDatum->szModFile))
		{
			delete pVSSScene;
			ADebug::Msg(0, "CMLScene::Create, Error load A3DScene!");
			return false;
		}
	}

	if (__pDatum->szBSPFile[0])
	{
		if (!pVSSScene->SetBSPFile(__pDatum->szBSPFile))
		{	
			delete pVSSScene;
			ADebug::Msg(0, "CMLScene::Create, Error load BSP!");
			return false;
		}
	}
	
	if (__pDatum->szESPFile[0])
	{
		if (!(m_pA3DESP = new A3DESP))
		{
			delete pVSSScene;
			ADebug::Msg(0, "CMLScene::Create, Not enough memory to create ESP!");
			return false;
		}

		if (!m_pA3DESP->Load(__pDatum->szESPFile))
		{
			delete pVSSScene;
			delete m_pA3DESP;
			m_pA3DESP = NULL;
			ADebug::Msg(0, "CMLScene::Create, Load ESP File Fail!");
			return false;
		}
	}
	
	pVSSScene->ForceToRenderAll(true);

	m_pA3DScene = pVSSScene;
	m_pOldScene = pVSSScene;
	m_bIBL		= false;

	return true;
}

bool CMLScene::Release()
{
	return AScene::Release();
}

/*	Load VSS data

	Return true for success, otherwise return false.

	szVSSFile: VSS file name
*/
bool CMLScene::LoadVSSData(const char* szVSSFile)
{
	if (!m_pA3DScene)
		return false;

	CMLVSSetScene* pVSSScene = (CMLVSSetScene*)m_pA3DScene;

	return pVSSScene->LoadVSSFile((char*)szVSSFile);
}

bool CMLScene::UpdateVisibleSets(A3DViewport* pCurrentViewport)
{
	if (m_pA3DScene)
		m_pA3DScene->UpdateVisibleSets(pCurrentViewport);

	return true;
}

