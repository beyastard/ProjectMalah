/*
 * FILE: ML_ManAI.h
 *
 * DESCRIPTION: AI manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/31
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANAI_H_
#define _ML_MANAI_H_

#include "ObjectPool.h"
#include "ML_AIUnit.h"
#include "ML_ManBase.h"

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

class CMLRoute;
class CMLBullet;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLManAI
//
///////////////////////////////////////////////////////////////////////////

class CMLManAI : public CMLManBase
{
public:		//	Types

	//	Index of AI
	enum
	{
		IDX_FOOTMAN		= ID_AI_FOOTMAN - ID_FIRSTAI,
		IDX_GUERILLA	= ID_AI_GUERILLA - ID_FIRSTAI,
		IDX_ROCKET		= ID_AI_ROCKET - ID_FIRSTAI
	};

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManAI)

	CMLManAI();
	virtual ~CMLManAI() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);			//	Initialize object
	virtual bool	Release();					//	Release object

	virtual bool	LogicRun(float fTimeSpan);	//	Logic tick
	virtual int		OnMsg(AMSG* pMSG);			//	Message handler

	virtual bool	TickAnimation();				//	Tick animation
	virtual bool	Render(A3DViewport* pViewport);	//	Render routines
	void			UpdateActions();				//	Update AI actions

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	bool		CreateAI(CMLRoute* pRoute);					//	Create a AI on route
	bool		RayTrace(MLRAYTRACE* pTrace);				//	Ray trace routine
	bool		Explode(CMLBullet* pBullet, bool bFire);	//	Bullet explode

	bool		CreateEnemyPlane(int iID, A3DVECTOR3& vDir);	//	Create a enemy plane
	bool		CreateEnemyVehicle(int iID, CMLRoute* pRoute, int iWaitTime);	//	Create a enemy vehicle
	bool		EnemyWeaponRayTrace(MLRAYTRACE* pTrace);	//	Enemy plane ray trace routine

	inline A3DImgModel*		GetDefImageModel(int iAIID);	//	Get default image model

	void		OutputDebugInfo();	//	Output debug information

	int			GetCurAINum()		{	return m_AIList.GetCount();		}
	int			GetCurEWNum()		{	return m_EWList.GetCount();		}
	DWORD		GetImageColor()		{	return m_dwImageColor;			}
	int			GetEnemyPlaneNum()	{	return m_iNumPlane;				}

protected:	//	Attributes

	A3DImageModelMan*	m_pImageMan;	//	Image model manager

	int			m_iIMGroupAI;		//	AI image model group index
	DWORD		m_dwTimeCnt;		//	Time counter
	int			m_iMIG21Time;		//	Time counter for MIG-21
	int			m_iMIG21Num;		//	Number counter for MIG-21
	int			m_iFantanTime;		//	Time counter for Fantan
	int			m_iFantanNum;		//	Number counter for Fantan

	DWORD		m_dwImageColor;		//	Image model's color	
	int			m_iNumPlane;		//	Current number of enemy plane
	
	//	Used for debug
	int			m_iNumAIDead;		//	Dead AI number
	int			m_iNumAIGen;		//	Generated AI number

	A3DImgModel*			m_aDefModels[NUM_AI_ID];	//	Default image model
	CObjectPool<CMLAIUnit>	m_aAIPool[NUM_AI_ID];		//	AI allocated pool
	AListT<CMLUnit*>		m_AIList;					//	AI list
	AListT<CMLUnit*>		m_EWList;					//	Enemy weapon list

protected:	//	Operations

	virtual bool	ProcessMsg(MLMSG* pMsg);	//	Handle a message

	bool		InitObjectPools();			//	Initialize object pools
	void		ReleaseObjectPools();		//	Release all object pools
	void		ReleaseAllAI();				//	Release all AI
	bool		LoadAIImageModels();		//	Load AI image models
	void		ReleaseAIImageModels();		//	Release AI image models
	void		CalcImageColor();			//	Adjust image model's color

	void		GeneratePlanes(float fTimeSpan);	//	Generate planes
	void		ResetMissionData();					//	Reset mission data

	CObjectPool<CMLAIUnit>*	GetAIPool(int iID)	{	return &m_aAIPool[iID - ID_FIRSTAI];	}
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

/*	Get default image model

	Return specified image model for success, otherwise return NULL.

	iAIID: AI's ID
*/
A3DImgModel* CMLManAI::GetDefImageModel(int iAIID)
{
	assert(ISAIID(iAIID));
	return m_aDefModels[iAIID - ID_FIRSTAI];
}


#endif	//	_ML_MANAI_H_

