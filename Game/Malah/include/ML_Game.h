/*
 * FILE: ML_Game.h
 *
 * DESCRIPTION: Main class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_GAME_H_
#define _ML_GAME_H_

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


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLGame
//
///////////////////////////////////////////////////////////////////////////

class CMLGame : public AGame
{
public:		//	Types

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLGame)

	CMLGame();
	virtual ~CMLGame() {}

public:		//	Attributes

public:		//	Operations

	virtual int		Run();		//	Run game

	virtual bool	InitRainSnow();
	virtual bool	ReleaseRainSnow();
	virtual bool	Reset();

	bool		TextOut(int iFont, int x, int y, char* szText, A3DCOLOR color, bool bShade);	//	Text out
	bool		DebugTextOut(int x, int y, char* szText);	//	Text out debu information

	bool		PlayA3DModelAction(A3DModel* pModel, char* szName, bool bDiscardOld,
								   bool bStopSFX=true, bool bStopGFX=true);	//	Play A3DModel action

	A3DGraphicsFX*	LoadGFX(int iResID, A3DVECTOR3* pvPos=NULL);			//	Load GFX from file
	void			ReleaseGFX(A3DGraphicsFX* pGFX);						//	Release GFX
	bool			PlayGFX(int iResID, A3DVECTOR3& vPos, float fScale, A3DVECTOR3* pvDir=NULL);		//	Play GFX
	bool			PlayGFX(char* szFile, A3DVECTOR3& vPos, float fScale, A3DVECTOR3* pvDir=NULL);	//	Play GFX

	A3DModel*	LoadA3DModel(int iResID);			//	Load mox model
	A3DModel*	LoadA3DModel(char* szModelFile);	//	Load mox model
	void		ReleaseA3DModel(A3DModel* pModel);	//	Release mox model
	A3DVECTOR3	GetHangPointPosAndDir(A3DModel* pModel, char* szName, A3DVECTOR3* pvDir);	//	Get hang point's position and direction

	void		Play2DSound(int iResID, bool bImmEffect, bool bLoop, bool bCanFade, int iType=0);
	void		Play2DSound(int iResID, bool bLoop, int iType=0);
	void		Stop2DSound(int iResID, int iType=0);
	void		Pause2DSound(int iResID, int iType=0);
	void		Play3DSound(int iResID, A3DVECTOR3& vPos, float fMinDist=20.0f, float fMaxDist=100.0f, int iType=0);
	void		Play3DSound(int iResID, A3DVECTOR3& vPos, float fMinDist, float fMaxDist, 
							bool bImmEffect, bool bForce2D, bool bCanFade, int iType);

	void		EnableAlphaTest(bool bEnable, A3DCMPFUNC CompFunc, DWORD dwRefValue);
	void		ResetEnvironment();		//	Reset environment

	//	Create 3D sound from file
	AM3DSoundBuffer* Create3DSound(int iResID, float fMin=20.0f, float fMax=100.0f);
	//	Release 3D sound buffer
	void Release3DSound(AM3DSoundBuffer* pSndBuf);
	//	Play 3D sound buffer
	void Play3DSound(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos, bool bLoop=false);
	void Play3DSound(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos, A3DVECTOR3& vVel, bool bLoop=false);
	//	Stop 3D sound buffer
	void Stop3DSound(AM3DSoundBuffer* pSndBuf);
	void Pause3DSound(AM3DSoundBuffer* pSndBuf, bool bPause, bool bLoop);	//	Pause 3D sound
	//	Update 3D sound position
	void Update3DSoundPos(AM3DSoundBuffer* pSndBuf, A3DVECTOR3& vPos);

	A3DLightning*	GetA3DLightning()	{	return m_pLightning;	}

protected:	//	Attributes

	AM3DSoundBufferMan*	m_p3DSoundMan;	//	AM3D sound manager
	A3DLightning*		m_pLightning;	//	Lightning object

protected:	//	Operations

	virtual bool	InitA3DTools();		//	Initialize A3D tools
	virtual bool	InitGameCore();		//	Initialize game core

	bool		InitFonts();			//	Initalize fonts
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_GAME_H_
