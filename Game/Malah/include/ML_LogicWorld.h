/*
 * FILE: ML_LogicWorld.h
 *
 * DESCRIPTION: Logic world class for Malah game
 *
 * CREATED BY: Duyuxin, 2002/12/17
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_LOGICWORLD_H_
#define _ML_LOGICWORLD_H_

#include "ML_World.h"
#include "ML_MapData.h"

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

class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLLogicWorld
//
///////////////////////////////////////////////////////////////////////////

class CMLLogicWorld : public CMLWorld
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLLogicWorld();
	virtual ~CMLLogicWorld() {}

public:		//	Attributes

public:		//	Operations

	virtual	bool	Release();

	virtual bool	Render(A3DViewport* pCurrentViewport);			//	Render solid
	virtual bool	RenderAlpha(A3DViewport* pCurrentViewport);		//	Render alpha

	virtual bool	LoadMissionResources(AArchive& ar);		//	Load game mission
	virtual void	ReleaseMissionResources();				//	Release mission resources

	float		GetHeightOfPos(A3DVECTOR3& vPos, A3DVECTOR3* pvNormal=NULL);	//	Get height of a position

	bool		RayTrace(int iBulletID, MLRAYTRACE* pTrace);	//	Ray trace routine
	bool		RayTrace_AI(MLRAYTRACE* pTrace);				//	AI ray trace routine
	bool		RayTrace_Player(MLRAYTRACE* pTrace);			//	Player ray trace routine
	bool		RayTrace_Copter(MLRAYTRACE* pTrace);			//	Helicopter trace routine
	bool		RayTrace_EnemyWeapon(MLRAYTRACE* pTrace);		//	Enemy plane trace routine
	bool		RayTrace_FixedObjects(MLRAYTRACE* pTrace);		//	Fixed objects trace routine

	void		Explode(CMLBullet* pBullet, bool bFire);		//	Bullet explode

	CMLMapData*		GetMapData()	{	return &m_MapData;		}
	CMLEnvScheme*	GetCurScheme()	{	return m_pCurScheme;	}
	DWORD			GetAmbient()	{	return m_dwAmbient;		}
	DWORD			GetDiffuse()	{	return m_dwDiffuse;		}

protected:	//	Attributes

	CMLMapData		m_MapData;		//	Map data
	CMLEnvScheme*	m_pCurScheme;	//	Current environment scheme
	DWORD			m_dwAmbient;	//	Current ambient color
	DWORD			m_dwDiffuse;	//	Diffuse of directional light

protected:	//	Operations

	bool		ChangeSceneFile(CMLSceneData* pData);	//	Change scene file
	bool		ChangeEnvScheme(CMLSceneData* pData);	//	Change environment scheme
	bool		ChangeSky(CMLEnvScheme* pScheme, CMLSceneData* pData);			//	Change sky
	bool		ChangeEnvironment(CMLEnvScheme* pScheme, CMLSceneData* pData);	//	Change environment
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_LOGICWORLD_H_

