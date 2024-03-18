/*
 * FILE: ML_ManImage.cpp
 *
 * DESCRIPTION: 2D iamge picture manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/8
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManImage.h"
#include "ML_ManagerDef.h"
#include "ML_GamePlay.h"
#include "ML_Weapon.h"
#include "ML_Player.h"

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
//	Implement CMLManImage
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManImage, CMLManBase, 1)

CMLManImage::CMLManImage() : CMLManBase()
{
	m_managerId		= MANID_IMAGE;

	m_dwTimeCnt		= 0;
	m_fScaleX		= 1.0f;
	m_fScaleY		= 1.0f;
	m_dwDrawFlags	= IMGDRAW_ALL;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManImage::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	memset(m_aImages, 0, sizeof (m_aImages));

	return true;
}

//	Release object
bool CMLManImage::Release()
{
	ReleaseAllImages();
	return CMLManBase::Release();
}

/*	Load image which has only one item

	Return true for success, otherwise return false.

	dwSlot: slot will be used by the image
	szFile: image file name
	iWidth, iHeight: iamge size
	iNumElem: element number used to initialize image buffer
*/
bool CMLManImage::LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, int iNumElem/* 0 */)
{
	A3DRECT Rect(0, 0, iWidth, iHeight);
	return LoadImage(dwSlot, szFile, iWidth, iHeight, 1, &Rect, iNumElem);
}

/*	Load image from file

	Return true for success, otherwise return false.

	dwSlot: slot will be used by the image
	szFile: image file name
	iWidth, iHeight: iamge size
	iNumItem: number of image item in this image
	aRects: rectanges for image item
*/
bool CMLManImage::LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight,
							int iNumItem, A3DRECT* aRects, int iNumElem/* 0 */)
{
	A3DDevice* pA3DDevice = m_pAGame->GetA3DEngine()->GetA3DDevice();

	IMAGE* pSlot = &m_aImages[dwSlot];
	if (pSlot->pImage)
	{
		ADEBUG_TRACE1("Slot %d has been used by other image", dwSlot);
		return false;
	}

	A2DSprite* pImage = new A2DSprite;
	if (!pImage)
	{
		ADEBUG_TRACE("Failed to create A2DSprite object");
		return false;
	}

	if (!pImage->Init(pA3DDevice, szFile, iWidth, iHeight, 0, iNumItem, aRects))
	{
		delete pImage;
		ADEBUG_TRACE1("Failed to initialize A2DSprite object from file %s", szFile);
		return false;
	}

	pSlot->pImage		= pImage;
	pSlot->iNumItem		= iNumItem;
	pSlot->iWidth		= iWidth;
	pSlot->iHeight		= iHeight;
	pSlot->iCurItem		= -1;
	pSlot->pImageBuf	= NULL;

	if (iNumElem)
	{
		if (!(pSlot->pImageBuf = new A2DSpriteBuffer))
		{
			ADEBUG_TRACE("Failed to new A2DSpriteBuffer");
		}
		else if (!pSlot->pImageBuf->Init(pA3DDevice, pSlot->pImage, iNumElem))
		{
			delete pSlot->pImageBuf;
			pSlot->pImageBuf = NULL;
			ADEBUG_TRACE("Failed to initialize A2DSpriteBuffer");
		}
	}

	return true;
}

//	Load all images
bool CMLManImage::LoadAllImages()
{
	//	Load posts ...
	LoadImage(IMG_PISTOLPOST, "InGame\\ColtM1911CrossHair.tga", 37, 37);
	LoadImage(IMG_RIFLEPOST, "InGame\\M16CrossHair.tga", 25, 25);
	LoadImage(IMG_MACHINEPOST, "InGame\\MiniGunCrossHair.tga", 39, 39);
	LoadImage(IMG_RPGPOST, "InGame\\MiniGunCrossHair.tga", 39, 39);

	m_aImages[IMG_PISTOLPOST].pImage->SetLocalCenterPos(18, 18);
	m_aImages[IMG_RIFLEPOST].pImage->SetLocalCenterPos(13, 13);
	m_aImages[IMG_MACHINEPOST].pImage->SetLocalCenterPos(19, 19);
	m_aImages[IMG_RPGPOST].pImage->SetLocalCenterPos(19, 19);

	int i, j;
	A3DRECT aRects[64];

	//	Life and blood
	LoadImage(IMG_LIFE, "InGame\\Life.tga", 112, 106);

	for (i=0; i < 3; i++)
		aRects[i] = A3DRECT(0, i*2, 12, i*2+2);

	LoadImage(IMG_BLOOD, "InGame\\LifeTime.bmp", 12, 6, 3, aRects, 35);

	//	Weapon icon
	LoadImage(IMG_PISTOL, "InGame\\Weapon_ColtM1911.tga", 75, 53);
	LoadImage(IMG_MORTAR, "InGame\\Weapon_Mortar.tga", 106, 123);
	LoadImage(IMG_MACHINE, "InGame\\Weapon_MiniGun.tga", 149, 51);
	LoadImage(IMG_CANNON, "InGame\\Weapon_Howitzer.tga", 134, 138);

#ifdef LANGUAGE_CHINESE
	LoadImage(IMG_RIFLE, "InGame\\Weapon_M16.tga", 130, 53);
	LoadImage(IMG_RPG, "InGame\\Weapon_RPG7.tga", 179, 51);
	LoadImage(IMG_ATTACKER, "InGame\\Weapon_F4.tga", 148, 88);
#else
	LoadImage(IMG_RIFLE, "InGame\\Weapon_M16.tga", 132, 46);
	LoadImage(IMG_RPG, "InGame\\Weapon_RPG7.tga", 179, 51);
	LoadImage(IMG_ATTACKER, "InGame\\Weapon_F4.tga", 121, 97);
#endif

	//	Ammo number
	for (i=0; i < 10; i++)
		aRects[i] = A3DRECT(i*26, 0, i*26 + 26, 30);

	LoadImage(IMG_AMMONUM, "InGame\\AmmoNum.tga", 260, 30, 10, aRects, 10);
	LoadImage(IMG_AMMOUNLMT, "InGame\\AmmoUnlimited.tga", 41, 30);

	//	Supply icons
	for (i=0; i < 3; i++)
	{
		for (j=0; j < 2; j++)
			aRects[i*2+j] = A3DRECT(j*90, i*80, j*90+90, i*80+80);
	}

	LoadImage(IMG_SUPPLY, "InGame\\Calling.tga", 180, 240, 6, aRects, 3);
	LoadImage(IMG_SUPPLYTIME, "InGame\\CallingTime.tga", 1, 6, 80);
	LoadImage(IMG_SUPPLYFULL, "InGame\\CallingTimeUp.tga", 47, 6, 3);

	//	Score number
	for (i=0; i < 11; i++)
		aRects[i] = A3DRECT(i*19, 0, i*19 + 19, 23);

	LoadImage(IMG_SCORENUM, "InGame\\KilledCounting.tga", 209, 23, 11, aRects, 30);

	//	Killed icons
	for (i=0; i < 3; i++)
		aRects[i] = A3DRECT(i*23, 0, i*23 + 23, 27);

	LoadImage(IMG_KILL,	"InGame\\KilledNew.tga", 69, 27, 3, aRects, 3);

	//	Time number
	for (i=0; i < 11; i++)
		aRects[i] = A3DRECT(i*33, 0, i*33 + 33, 50);

	LoadImage(IMG_TIMENUM, "InGame\\TimeCounting.tga", 363, 50, 11, aRects, 10);

	//	Unit icons
	for (i=0; i < 3; i++)
	{
		aRects[i]	= A3DRECT(i*69, 0, i*69+69, 39);
		aRects[i+3]	= A3DRECT(207+i*30, 0, 207+i*30+30, 39);
	}

	aRects[6] = A3DRECT(297, 0, 330, 39);
	aRects[7] = A3DRECT(330, 0, 399, 39);

	LoadImage(IMG_UNITS, "InGame\\CountingKilled.tga", 399, 39, 8, aRects, 10);

	return true;
}

/*	Draw image item to backbuffer considing scale factor

	dwSlot: image slot.
	x, y: image's local center position on screen. x, y will also are be effected
		  by scale factors
	iItem: iamge item index
	bFixX: fix x
	bFixY: fix y
*/
void CMLManImage::DrawToBack(DWORD dwSlot, int x, int y, int iItem/* 0 */,
							 bool bFixX/* false */, bool bFixY/* false */)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pImage = &m_aImages[dwSlot];
	assert(iItem >= 0 && iItem < pImage->iNumItem);

	if (iItem != pImage->iCurItem)
	{
		pImage->pImage->SetCurrentItem(iItem);
		pImage->iCurItem = iItem;
	}

	pImage->pImage->SetScaleX(m_fScaleX);
	pImage->pImage->SetScaleY(m_fScaleY);

	if (!bFixX)
		x = (int)(x * m_fScaleX);

	if (!bFixY)
		y = (int)(y * m_fScaleY);
	
	if (pImage->pImageBuf)
		pImage->pImage->DrawToBuffer(pImage->pImageBuf, x, y);
	else
		pImage->pImage->DrawToBack(x, y);
}

/*	Draw image item to backbuffer without scale.

	dwSlot: image slot.
	x, y: image's local center position on screen. x, y will also are be effected
		  by scale factors
	iItem: iamge item index
*/
void CMLManImage::DrawToBackWithoutScale(DWORD dwSlot, int x, int y, int iItem/* 0 */)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pImage = &m_aImages[dwSlot];
	assert(iItem >= 0 && iItem < pImage->iNumItem);

	if (iItem != pImage->iCurItem)
	{
		pImage->pImage->SetCurrentItem(iItem);
		pImage->iCurItem = iItem;
	}

	pImage->pImage->SetScaleX(1.0f);
	pImage->pImage->SetScaleY(1.0f);

	if (pImage->pImageBuf)
		pImage->pImage->DrawToBuffer(pImage->pImageBuf, x, y);
	else
		pImage->pImage->DrawToBack(x, y);
}

/*	Release speicfied image

	dwSlot: image's slot.
*/
void CMLManImage::ReleaseImage(DWORD dwSlot)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pImage = &m_aImages[dwSlot];

	if (pImage->pImage)
	{
		pImage->pImage->Release();
		delete pImage->pImage;
		pImage->pImage = NULL;
	}

	if (pImage->pImageBuf)
	{
		pImage->pImageBuf->Release();
		delete pImage->pImageBuf;
		pImage->pImageBuf = NULL;
	}

	pImage->iNumItem = 0;
	pImage->iCurItem = -1;
}

//	Release all images
void CMLManImage::ReleaseAllImages()
{
	int i;
	for (i=0; i < NUM_IMAGE; i++)
	{
		IMAGE* pImage = &m_aImages[i];
		
		if (pImage->pImage)
		{
			pImage->pImage->Release();
			delete pImage->pImage;
		}

		if (pImage->pImageBuf)
		{
			pImage->pImageBuf->Release();
			delete pImage->pImageBuf;
		}
	}

	memset(m_aImages, 0, sizeof (m_aImages));
}

/*	Get image item size

	dwSlot: image slot.
	iItem: item index.
	piWid (out): used to receive image item's width
	piHei (out): used to receive image item's height.
*/
void CMLManImage::GetImageItemSize(DWORD dwSlot, int iItem, int* piWid, int* piHei)
{
	if (dwSlot >= NUM_IMAGE)
	{
		assert(0);
		return;
	}

	IMAGE* pImage = &m_aImages[dwSlot];

	assert(iItem >= 0 && iItem < pImage->iNumItem);

	A2DSpriteItem* pItem = pImage->pImage->GetItem(iItem);
	A3DRECT Rect = pItem->GetRect();

	if (piWid)
		*piWid = Rect.right - Rect.left;

	if (piHei)
		*piHei = Rect.bottom - Rect.top;
}

//	Message handler
int	CMLManImage::OnMsg(AMSG* pMSG)
{
	MLMSG* pMLMsg = (MLMSG*)pMSG;

	if (pMLMsg->idObject == m_managerId)
		return ProcessMsg(pMLMsg) ? 1 : 0;

	return 1;
}

/*	Handle a message

	Return true for success, otherwise return false.

	pMsg: message will be handled
*/
bool CMLManImage::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

//	Called when begin game playing
bool CMLManImage::OnStartGamePlay()
{
	if (!LoadAllImages())
	{
		ADEBUG_TRACE("Failed to load images");
		return false;
	}

	return true;
}

//	Called when end game playing
bool CMLManImage::OnEndGamePlay()
{
	ReleaseAllImages();
	return true;
}

//	Called when being mission
bool CMLManImage::OnBeginMission(int iMission)
{
	m_dwTimeCnt = 0;

	return true;
}

//	Called when reset mission
bool CMLManImage::OnResetMission()
{
	m_dwTimeCnt = 0;

	return true;
}

//	Called when end mission
bool CMLManImage::OnEndMission()
{
	return true;
}

//	Logic tick
bool CMLManImage::LogicRun(float fTimeSpan)
{
	BeginLogicCount();
	
	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	EndLogicCount();

	return true;
}

//	Draw rouinne
bool CMLManImage::Draw()
{
	if (GETMLGAMEPLAY->GetActiveStatus() != CMLGamePlay::GPS_NORMALPLAY)
		return true;

	if (m_dwDrawFlags & IMGDRAW_POST)
		DrawPost();

	if (m_dwDrawFlags & IMGDRAW_AMMO)
		DrawAmmo();

	if (m_dwDrawFlags & IMGDRAW_HEALTH)
		DrawHealthPoint();

	if (m_dwDrawFlags & IMGDRAW_SUPPCNT)
		DrawSupportCounter();

	if (m_dwDrawFlags & IMGDRAW_SCORE)
		DrawScore();

	if (m_dwDrawFlags & IMGDRAW_TIMER)
		DrawTimer();

	if (m_dwDrawFlags & IMGDRAW_KILLED)
		DrawKilled();

	if (m_dwDrawFlags & IMGDRAW_MISSBRIEF)
		DrawMissionBrief();

	if (m_dwDrawFlags & IMGDRAW_MISSICON)
		DrawMissionIcons();

	//	Flush image buffer
	for (int i=0; i < NUM_IMAGE; i++)
	{
		IMAGE* pSlot = &m_aImages[i];
		if (pSlot->pImageBuf)
			pSlot->pImageBuf->FlushBuffer(m_pAGame->GetA3DViewport());
	}

	return true;
}

/*	Draw number

	Return number of digits were drawn.
*/
int CMLManImage::DrawNumber(DWORD dwSlot, int iNum, int x, int y, bool bFixX, bool bFixY, int iFixLen/* 0 */)
{
	char szNum[40];
	_itoa(iNum, szNum, 10);

	int iWid, iHei;
	GetImageItemSize(dwSlot, 0, &iWid, &iHei);

	int iCount = 0;

	if (iFixLen)
	{
		int iSkip = iFixLen - strlen(szNum);
		for (int i=0; i < iSkip; i++, iCount++)
		{
			DrawToBack(dwSlot, x, y, 0, bFixX, bFixY);
			x += iWid;
		}
	}

	for (int i=0; szNum[i] != '\0'; i++, iCount++)
	{
		int iItem = szNum[i] - '0';
		DrawToBack(dwSlot, x, y, iItem, bFixX, bFixY);
		x += iWid;
	}

	return iCount;
}

void CMLManImage::DrawProcess(DWORD dwSlot, int iSlash, int iCur, int iMax, int x, int y)
{
	int iWid, iHei, iCount;
	GetImageItemSize(dwSlot, 0, &iWid, &iHei);

	iCount = DrawNumber(dwSlot, iCur, x, y, false, false);

	x += iCount * iWid;
	DrawToBack(dwSlot, x, y, iSlash);

	x += iWid;
	DrawNumber(dwSlot, iMax, x, y, false, false);
}

int	CMLManImage::DrawNumberWithoutScale(DWORD dwSlot, int iNum, int x, int y, int iFixLen/* 0 */)
{
	char szNum[40];
	_itoa(iNum, szNum, 10);

	int iWid, iHei;
	GetImageItemSize(dwSlot, 0, &iWid, &iHei);

	int iCount = 0;

	if (iFixLen)
	{
		int iSkip = iFixLen - strlen(szNum);
		for (int i=0; i < iSkip; i++, iCount++)
		{
			DrawToBackWithoutScale(dwSlot, x, y, 0);
			x += iWid;
		}
	}

	for (int i=0; szNum[i] != '\0'; i++, iCount++)
	{
		int iItem = szNum[i] - '0';
		DrawToBackWithoutScale(dwSlot, x, y, iItem);
		x += iWid;
	}

	return iCount;
}

//	Draw post
void CMLManImage::DrawPost()
{
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	if (!pPlayer)
		return;

	CMLWeapon* pWeapon = pPlayer->GetCurrentWeapon();
	if (!pWeapon)
		return;

	DWORD dwImage;
	switch (pWeapon->GetId())
	{
	case ID_WEAPON_PISTOL:	dwImage = IMG_PISTOLPOST;	break;
	case ID_WEAPON_RIFLE:	dwImage = IMG_RIFLEPOST;	break;
	case ID_WEAPON_MACHINE:	dwImage = IMG_MACHINEPOST;	break;
	case ID_WEAPON_RPG:		dwImage = IMG_RPGPOST;		break;

	case ID_WEAPON_MORTAR:
	case ID_WEAPON_ATTACKER:
	case ID_WEAPON_CANNON:
	default:
		return;
	}

	int x = GETCURUSERCONFIGS->iRenderWid >> 1;
	int y = GETCURUSERCONFIGS->iRenderHei >> 1;
	DrawToBackWithoutScale(dwImage, x, y);
}

//	Draw ammo
void CMLManImage::DrawAmmo()
{
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	if (!pPlayer)
		return;

	CMLWeapon* pWeapon = pPlayer->GetCurrentWeapon();
	if (!pWeapon)
		return;

	//	Draw weapon icon
	int iWid, iHei, iImage;

	switch (pWeapon->GetId())
	{
	case ID_WEAPON_PISTOL:		iImage = IMG_PISTOL;	break;
	case ID_WEAPON_RIFLE:		iImage = IMG_RIFLE;		break;
	case ID_WEAPON_MORTAR:		iImage = IMG_MORTAR;	break;
	case ID_WEAPON_MACHINE:		iImage = IMG_MACHINE;	break;
	case ID_WEAPON_RPG:			iImage = IMG_RPG;		break;
	case ID_WEAPON_CANNON:		iImage = IMG_CANNON;	break;
	case ID_WEAPON_ATTACKER:	iImage = IMG_ATTACKER;	break;
	default:
		return;
	}

	GetImageItemSize(iImage, 0, &iWid, &iHei);

	int x = (int)(400 * m_fScaleX);
	int y = BASERENDER_HEIGHT - iHei;

	DrawToBack(iImage, x, y, 0, true);

	//	Draw ammo number
	x += (int)(iWid * m_fScaleX);
	GetImageItemSize(IMG_AMMONUM, 0, &iWid, &iHei);
	y  = BASERENDER_HEIGHT - iHei;

	if (pWeapon->GetId() != ID_WEAPON_CANNON && pWeapon->GetId() != ID_WEAPON_ATTACKER)
	{
		if (pWeapon->UnlimitedAmmo())
			DrawToBack(IMG_AMMOUNLMT, x, y, 0, true);
		else
			DrawNumber(IMG_AMMONUM, pWeapon->GetBulletNum(), x, y, true, false);
	}
}

//	Draw health points
void CMLManImage::DrawHealthPoint()
{
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	if (!pPlayer)
		return;

	int iWid, iHei;
	GetImageItemSize(IMG_LIFE, 0, &iWid, &iHei);

	int x = 0;
	int y = GETCURUSERCONFIGS->iRenderHei - iHei;
	DrawToBackWithoutScale(IMG_LIFE, x, y);

	int iLife = pPlayer->GetLife();
	int iMax  = pPlayer->GetMaxLife();
	int iNum  = (int)((float)iLife / iMax * 33 + 0.99999f);

	//	Move to blood slot's left-bottom
	x += 63;
	y += 73;

	int iItem;
	if (iNum >= 22)
		iItem = 0;
	else if (iNum >= 11)
		iItem = 1;
	else
		iItem = 2;

	for (int i=0; i < iNum; i++)
	{
		DrawToBackWithoutScale(IMG_BLOOD, x, y, iItem);
		y -= 2;
	}
}

//	Draw support counter
void CMLManImage::DrawSupportCounter()
{
	CMLPlayer* pPlayer = GETMLGAMEPLAY->GetLocalPlayer();
	if (!pPlayer)
		return;

	int iWid, iHei;
	GetImageItemSize(IMG_SUPPLY, 0, &iWid, &iHei);

	int x, y, tx, ty, iCur, iMax, iItem, i, iNum;

	x = GETCURUSERCONFIGS->iRenderWid - iWid;
	y = (GETCURUSERCONFIGS->iRenderHei - iHei * 3) / 2;

	//	Cannon ...
	iCur  = pPlayer->GetSupportCounter(ID_SUPP_CANNON, &iMax);
	iItem = iCur >= iMax ? 0 : 1;
	DrawToBackWithoutScale(IMG_SUPPLY, x, y, iItem);

	tx = x + 22;
	ty = y + 64;

	if (iCur >= iMax)
		DrawToBackWithoutScale(IMG_SUPPLYFULL, tx, ty);
	else
	{
		iNum = (int)((float)iCur / iMax * 46);
		for (i=0; i < iNum; i++, tx++)
		{
			if (i & 1)
				DrawToBackWithoutScale(IMG_SUPPLYTIME, tx, ty);
		}
	}

	//	Attacker ...
	y	 += iHei;
	iCur  = pPlayer->GetSupportCounter(ID_SUPP_ATTACKER, &iMax);
	iItem = iCur >= iMax ? 4 : 5;
	DrawToBackWithoutScale(IMG_SUPPLY, x, y, iItem);

	tx = x + 22;
	ty = y + 62;

	if (iCur >= iMax)
		DrawToBackWithoutScale(IMG_SUPPLYFULL, tx, ty);
	else
	{
		iNum = (int)((float)iCur / iMax * 46);
		for (i=0; i < iNum; i++, tx++)
		{
			if (i & 1)
				DrawToBackWithoutScale(IMG_SUPPLYTIME, tx, ty);
		}
	}

	//	Helicopter ... 
	y	 += iHei;
	iCur  = pPlayer->GetSupportCounter(ID_SUPP_COPTER, &iMax);
	iItem = iCur >= iMax ? 2 : 3;
	DrawToBackWithoutScale(IMG_SUPPLY, x, y, iItem);

	tx = x + 22;
	ty = y + 62;

	if (iCur >= iMax)
		DrawToBackWithoutScale(IMG_SUPPLYFULL, tx, ty);
	else
	{
		iNum = (int)((float)iCur / iMax * 46);
		for (i=0; i < iNum; i++, tx++)
		{
			if (i & 1)
				DrawToBackWithoutScale(IMG_SUPPLYTIME, tx, ty);
		}
	}
}

//	Draw score
void CMLManImage::DrawScore()
{
	USERDATA* pUserData = GETMLGAMEPLAY->GetUserData();
	m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(255, 180, 0));
	DrawNumber(IMG_SCORENUM, pUserData->iScore, 10, 10, false, false);
}

//	Draw timer
void CMLManImage::DrawTimer()
{
	CMLMission* pMission = GETMLGAMEPLAY->GetMission();

	//	No time limit
	if (!pMission->GetTimeLimit())
		return;

	int iTimeLeft = pMission->GetTimeLimit() - pMission->GetTimeCount();
	if (iTimeLeft < 0)
		iTimeLeft = 0;

	//	Convert ms to s
	iTimeLeft = (iTimeLeft + 999) / 1000;

	int iHour = iTimeLeft / 3600;
	int iMin  = (iTimeLeft - iHour * 3600) / 60;
	int iSec  = iTimeLeft - iHour * 3600 - iMin * 60;

	int iWid, iHei;
	GetImageItemSize(IMG_TIMENUM, 0, &iWid, &iHei);

	int x, y;
	if (iHour)
		x = (BASERENDER_WIDTH - iWid * 8) / 2;
	else
		x = (BASERENDER_WIDTH - iWid * 5) / 2;

	y = 0;

	//	Hour
	if (iHour)
	{
		DrawNumber(IMG_TIMENUM, iHour, x, y, false, false, 2);
		x += iWid * 2;

		DrawToBack(IMG_TIMENUM, x, y, 10);
		x += iWid;
	}

	//	Minute
	DrawNumber(IMG_TIMENUM, iMin, x, y, false, false, 2);
	x += iWid * 2;

	DrawToBack(IMG_TIMENUM, x, y, 10);
	x += iWid;

	//	Second
	DrawNumber(IMG_TIMENUM, iSec, x, y, false, false, 2);
}

//	Draw kill record
void CMLManImage::DrawKilled()
{
	char szNum[10];

	int iWid, iHei, iNumWid, iNumHei;

	GetImageItemSize(IMG_KILL, 0, &iWid, &iHei);
	GetImageItemSize(IMG_SCORENUM, 0, &iNumWid, &iNumHei);

	//	Set number color
	m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(255, 180, 0));

	USERDATA* pUserData = GETMLGAMEPLAY->GetUserData();

	//	Draw killed people icon and number
	_itoa(pUserData->iNumPeople, szNum, 10);
	int iLen = strlen(szNum);

	int x, y;
	x = BASERENDER_WIDTH - iLen * iNumWid - 10;
	y = 5;

	DrawNumber(IMG_SCORENUM, pUserData->iNumPeople, x, y, false, false);

	x -= iWid;
	DrawToBack(IMG_KILL, x, y, 0);

	//	Draw destroyed vehicle icon and number
	_itoa(pUserData->iNumVehicle, szNum, 10);
	iLen = strlen(szNum);

	x = BASERENDER_WIDTH - iLen * iNumWid - 10;
	y += iHei + 3;

	DrawNumber(IMG_SCORENUM, pUserData->iNumVehicle, x, y, false, false);

	x -= iWid;
	DrawToBack(IMG_KILL, x, y, 1);

	//	Draw destroyed plane icon and number
	_itoa(pUserData->iNumPlanes, szNum, 10);
	iLen = strlen(szNum);

	x = BASERENDER_WIDTH - iLen * iNumWid - 10;
	y += iHei + 3;

	DrawNumber(IMG_SCORENUM, pUserData->iNumPlanes, x, y, false, false);

	x -= iWid;
	DrawToBack(IMG_KILL, x, y, 2);
}

//	Draw mission brief
void CMLManImage::DrawMissionBrief()
{
	MISSIONBRIEF* pBrief = GETMLGAMEPLAY->GetMissionBrief();

	int i, iFont, iLineSpace;

	if (m_fScaleX > 0.99f)
	{
		iFont = ML_GetAppropriateFont(16);
		iLineSpace = 20;
	}
	else
	{
		iFont = ML_GetAppropriateFont(12);
		iLineSpace = 16;
	}

	int x = 10;
	int y = (int)(70 * m_fScaleY);

	for (i=0; i < pBrief->iNumLine; i++)
	{
		g_theGame.TextOut(iFont, x, y, pBrief->aLines[i], A3DCOLORRGB(0, 255, 0), true);
		y += iLineSpace;
	}
}

//	Draw mission icons
void CMLManImage::DrawMissionIcons()
{
	MLMISSIONDATA* pData = GETMLGAMEPLAY->GetMission()->GetMissionData();
	CMLMission::STATISTIC* pStatistics = GETMLGAMEPLAY->GetMission()->GetStatistics();

	int x, y, iWid, iHei;

	x = 3;
	y = 220;

	if (pData->iType == MISSION_KILL)
	{
		int i, aNums[6], aFlags[6];
	
		if ((aFlags[0] = pData->iNumTruck))
			aNums[0] = pData->iNumTruck - pStatistics->iNumDeadTruck;

		if ((aFlags[1] = pData->iNumArmored))
			aNums[1] = pData->iNumArmored - pStatistics->iNumDeadArmored;

		if ((aFlags[2] = pData->iNumTank))
			aNums[2] = pData->iNumTank - pStatistics->iNumDeadTank;

		if ((aFlags[3] = pData->iNumFootMan))
			aNums[3] = pData->iNumFootMan - pStatistics->iNumDeadFootman;

		if ((aFlags[4] = pData->iNumRocket))
			aNums[4] = pData->iNumRocket - pStatistics->iNumDeadRocket;

		if ((aFlags[5] = pData->iNumGuerilla))
			aNums[5] = pData->iNumGuerilla - pStatistics->iNumDeadGuerilla;

		m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(0, 255, 0));

		for (i=0; i < 6; i++)
		{
			if (!aFlags[i])
				continue;

			CLAMPMINVALUE(aNums[i], 0);

			GetImageItemSize(IMG_UNITS, i, &iWid, &iHei);
			DrawToBack(IMG_UNITS, x, y, i);

			DrawNumber(IMG_SCORENUM, aNums[i], x+iWid+3, i >= 2 ? y+5 : y, false, false);

			y += iHei;
		}
	}
	else if (pData->iType == MISSION_PRESERVE)
	{
		GetImageItemSize(IMG_UNITS, 6, &iWid, &iHei);
		DrawToBack(IMG_UNITS, x, y, 6, false, false);

		m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(255, 180, 0));

		x += iWid + 5;
		DrawProcess(IMG_SCORENUM, 10, pStatistics->iNumInPreserve, pData->iNumFootMan, x, y+8);
		y += iHei;
	}
	else if (pData->iType == MISSION_STOPTRUCK)
	{
		GetImageItemSize(IMG_UNITS, 0, &iWid, &iHei);
		DrawToBack(IMG_UNITS, x, y, 0);

		m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(0, 255, 0));

		x += iWid;
		DrawProcess(IMG_SCORENUM, 10, pStatistics->iNumEscTruck, pData->iNumTruck, x, y);
		y += iHei;
	}

	//	Copter number
	if (pData->iNumCopter)
	{
		x = 3;
		GetImageItemSize(IMG_UNITS, 7, &iWid, &iHei);
		DrawToBack(IMG_UNITS, x, y, 7, false, false);

		m_aImages[IMG_SCORENUM].pImage->SetColor(A3DCOLORRGB(255, 180, 0));

		x += iWid + 3;
		DrawProcess(IMG_SCORENUM, 10, pStatistics->iNumCopter, pData->iNumCopter, x, y+5);
	}
}

