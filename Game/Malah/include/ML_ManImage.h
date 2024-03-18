/*
 * FILE: ML_ManImage.h
 *
 * DESCRIPTION: 2D iamge picture manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/8
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef	_ML_MANIMAGE_H_
#define _ML_MANIMAGE_H_

#include "ML_MsgDef.h"
#include "ML_ManBase.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Image draw flags
#define IMGDRAW_NONE		0			//	Don't draw any image
#define IMGDRAW_POST		0x0001		//	Post cursor
#define IMGDRAW_HEALTH		0x0002		//	Health value
#define IMGDRAW_AMMO		0x0004		//	Ammo number
#define IMGDRAW_SUPPCNT		0x0008		//	Support counter
#define IMGDRAW_SCORE		0x0010		//	Score number
#define IMGDRAW_TIMER		0x0020		//	Timer
#define IMGDRAW_KILLED		0x0040		//	Killed people number
#define IMGDRAW_MISSBRIEF	0x0080		//	Mission brief
#define IMGDRAW_MISSICON	0x0100		//	Mission icons
#define IMGDRAW_ALL			0xffff		//	Draw all iamges

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
//	Class CMLManImage
//
///////////////////////////////////////////////////////////////////////////

class CMLManImage : public CMLManBase
{
public:		//	Types

	enum
	{
		IMG_PISTOLPOST = 0,	//	Pistol post
		IMG_RIFLEPOST,		//	Rifle post
		IMG_MACHINEPOST,	//	Machine gun post
		IMG_RPGPOST,		//	RPG post
		IMG_LIFE,			//	Life
		IMG_BLOOD,			//	Blood
		IMG_PISTOL,			//	Pistol
		IMG_RIFLE,			//	Rifle
		IMG_MORTAR,			//	Mortar
		IMG_MACHINE,		//	Machine gun
		IMG_RPG,			//	RPG
		IMG_CANNON,			//	Cannon
		IMG_ATTACKER,		//	Attacker
		IMG_AMMONUM,		//	Ammo number
		IMG_AMMOUNLMT,		//	Ammo unlimited icon
		IMG_SUPPLY,			//	Supply icons
		IMG_SUPPLYTIME,		//	Supply time
		IMG_SUPPLYFULL,		//	Supply full
		IMG_KILL,			//	Kill icon
		IMG_TIMENUM,		//	Time number
		IMG_SCORENUM,		//	Score number
		IMG_UNITS,			//	Unit icons
		NUM_IMAGE,			//	Number of image
	};

	//	Images
	typedef struct _IMAGE
	{
		A2DSprite*	pImage;		//	Image
		int			iNumItem;	//	Number of image item
		int			iCurItem;	//	Current item
		int			iWidth;		//	Image width
		int			iHeight;	//	Image height

		A2DSpriteBuffer*	pImageBuf;	//	Image buffer

	} IMAGE, *PIMAGE;

public:		//	Constructor and Destructor

	DECLARE_ASERIAL(CMLManImage)

	CMLManImage();
	virtual ~CMLManImage() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Init(AGame* pGame);				//	Initialize object
	virtual bool	Release();						//	Release object

	virtual bool	LogicRun(float fTimeSpan);		//	Logic tick
	virtual int		OnMsg(AMSG* pMSG);				//	Message handler
	virtual bool	Draw();							//	Draw rouinne

	virtual bool	OnStartGamePlay();				//	Called when begin game playing
	virtual bool	OnEndGamePlay();				//	Called when end game playing
	virtual bool	OnBeginMission(int iMission);	//	Called when being mission
	virtual bool	OnResetMission();				//	Called when reset mission
	virtual bool	OnEndMission();					//	Called when end mission

	void		SetDrawFlags(DWORD dwFlags)		{	m_dwDrawFlags = dwFlags;	}
	DWORD		GetDrawFlags()					{	return m_dwDrawFlags;		}
	
	inline void	ShowCursor(bool bShow);		//	Show or hide cursor

	inline void	SetScaleFactor(float fScaleX, float fScaleY);		//	Set scale factor
	inline void	GetScaleFactor(float* pfScaleX, float* pfScaleY);	//	Get scale factor

protected:	//	Attributes

	DWORD		m_dwTimeCnt;	//	Time counter
	float		m_fScaleX;		//	Scale factor of x
	float		m_fScaleY;		//	Scale factor of y
	DWORD		m_dwDrawFlags;	//	Draw flags

	IMAGE		m_aImages[NUM_IMAGE];	//	Image array

protected:	//	Operations

	virtual bool	ProcessMsg(MLMSG* pMsg);	//	Handle a message

	bool		LoadAllImages();				//	Load all images
	void		ReleaseImage(DWORD dwSlot);		//	Release speicfied image
	void		ReleaseAllImages();				//	Release all images

	bool		LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, 
						  int iNumItem, A3DRECT* aRects, int iNumElem=0);	//	Load image from file
	bool		LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, int iNumElem=0);	//	Load image which has only one item
	void		DrawToBack(DWORD dwSlot, int x, int y, int iItem=0, bool bFixX=false, bool bFixY=false);	//	Draw image item to backbuffer
	void		DrawToBackWithoutScale(DWORD dwSlot, int x, int y, int iItem=0);	//	Draw image item to backbuffer without scale
	void		GetImageItemSize(DWORD dwSlot, int iItem, int* piWid, int* piHei);	//	Get image item size

	int			DrawNumber(DWORD dwSlot, int iNum, int x, int y, bool bFixX, bool bFixY, int iFixLen=0);	//	Draw number
	int			DrawNumberWithoutScale(DWORD dwSlot, int iNum, int x, int y, int iFixLen=0);
	void		DrawProcess(DWORD dwSlot, int iSlash, int iCur, int iMax, int x, int y);

	void		DrawPost();				//	Draw post
	void		DrawAmmo();				//	Draw ammo
	void		DrawHealthPoint();		//	Draw health points
	void		DrawSupportCounter();	//	Draw support counter
	void		DrawScore();			//	Draw score
	void		DrawTimer();			//	Draw timer
	void		DrawKilled();			//	Draw kill record
	void		DrawMissionBrief();		//	Draw mission brief
	void		DrawMissionIcons();		//	Draw mission icons
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

/*	Set scale factor

	fScaleX: scale factor of x.
	fScaleY: scale factor of y.
*/
void CMLManImage::SetScaleFactor(float fScaleX, float fScaleY)
{
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
}

/*	Get scale factor

	pfScaleX (out): receive scale factor of x. may be NULL
	pfScaleY (out): receive scale factor of y. may be NULL
*/
void CMLManImage::GetScaleFactor(float* pfScaleX, float* pfScaleY)
{
	if (pfScaleX)	*pfScaleX = m_fScaleX;
	if (pfScaleY)	*pfScaleY = m_fScaleY;
}

//	Show or hide cursor
void CMLManImage::ShowCursor(bool bShow)
{
	if (bShow)
		m_dwDrawFlags |= IMGDRAW_POST;
	else
		m_dwDrawFlags &= ~IMGDRAW_POST;
}


#endif	//	_ML_MANIMAGE_H_

