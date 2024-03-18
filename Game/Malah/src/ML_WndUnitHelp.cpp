/*
 * FILE: ML_WndUnitHelp.cpp
 *
 * DESCRIPTION: Unit help interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/3/21
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "A2DSprite.h"
#include "AWndSprite.h"

#include "ML_WndUnitHelp.h"
#include "ML_WndManager.h"
#include "ML_Gameplay.h"
#include "ML_Resources.h"
#include "jexception.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


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
//	Implement CMLWndUnitHelp
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndUnitHelp, AWnd, 1)

CMLWndUnitHelp::CMLWndUnitHelp()
{
	m_pBackimage	= NULL;
	m_bCanBeFocus	= true;
	m_iNumBtPerLine	= 4;
	m_iNumBtLine	= 4;
	m_bObserve		= false;
	m_pMoxModel		= NULL;
	m_pCamera		= NULL;
	m_pViewport		= NULL;
	m_iCurUnit		= -1;

	m_UnitDesc.iNumLine = 0;
}

CMLWndUnitHelp::~CMLWndUnitHelp()
{
}

class A3DAssistTools_I;

bool CMLWndUnitHelp::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

void CMLWndUnitHelp::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndUnitHelp::Redraw()
{
	AWnd::Redraw();

	if (m_bVisible && m_bObserve)
		RenderUnit();

	return true;
}

bool CMLWndUnitHelp::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	APOINT ptAb;
	GetAbsolutePos(ptAb);

	if (m_pBackimage)
	{
		if (!AWC::DrawSprite(m_pBackimage, ptAb))
		{
			ADEBUG_TRACE("Cannot draw to back.");
		}
	}

	return true;
}

void CMLWndUnitHelp::RenderUnit()
{
	if (!m_pCamera || !m_pViewport || !m_pMoxModel)
		return;

	A3DViewport* pOldView = g_theGame.GetA3DViewport();
	
	//	Use our own viewport and camera
	m_pViewport->Active();
	m_pViewport->ClearDevice();

	//	Rotate object
	float fRad = DEG2RAD(45.0f) * GETMLGAMEPLAY->GetTimeSpan() / 1000.0f;
	
	m_pMoxModel->RotateYRelative(fRad);
	m_pMoxModel->Render(m_pViewport);
	
	//	We should render all model GFX on our own
	AList* pGFXList = m_pMoxModel->GetGFXList();
	ALISTELEMENT* pGFXNode = pGFXList->GetFirst();
	while (pGFXNode != pGFXList->GetTail())
	{
		A3DMODEL_GFX_RECORD* pGFXRecord = (A3DMODEL_GFX_RECORD*)pGFXNode->pData;
		pGFXRecord->pGFX->Render(m_pViewport);
		pGFXNode = pGFXNode->pNext;
	}
	
	g_theGame.GetA3DEngine()->FlushVertexCollector(m_pViewport);
	
	//	Restore old viewport
	pOldView->Active();

	//	Draw description text
	if (!m_UnitDesc.iNumLine)
		return;

	int x = m_rcBtArea.left + 8;
	int y = m_rcBtArea.bottom - (m_UnitDesc.iNumLine-1) * 16 - 20;
	int iFont = ML_GetAppropriateFont(16);

	//	Draw name
	g_theGame.TextOut(iFont, x, y, m_UnitDesc.aLines[0], A3DCOLORRGB(255, 0, 0), false);
	y += 20;

	iFont = ML_GetAppropriateFont(12);
	for (int i=1; i < m_UnitDesc.iNumLine; i++)
	{
		g_theGame.TextOut(iFont, x, y, m_UnitDesc.aLines[i], A3DCOLORRGB(255, 255, 0), false);
		y += 16;
	}
}

bool CMLWndUnitHelp::LoadFromIni(AWnd* pParent, AWndIniFile& IniFile, const char* pszSection)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadFromIni(pParent, IniFile, pszSection))
		return false;
	
/*	const char* pszBackimage = NULL;
	if (!IniFile.ReadString(pszSection, "image_back", pszBackimage))
	{
		ADEBUG_TRACE("cannot read back image.");
		return false;
	}

	m_strBackimage	= pszBackimage;
*/
	//	Load buttons
	if (!IniFile.ReadRect(pszSection, "button_area", m_rcBtArea))
	{
		ADEBUG_TRACE("Cannot load button area.");
		return false;
	}

	int iWid, iHei, iHSpace, iVSpace;
	ARECT rcBtSize;

	if (!IniFile.ReadRect(pszSection, "button_size", rcBtSize))
	{
		iWid = m_rcBtArea.GetWidth() / m_iNumBtPerLine;
		iHei = m_rcBtArea.GetHeight() / m_iNumBtLine;
		iHSpace = iVSpace = 0;
	}
	else
	{
		iWid = rcBtSize.right;
		iHei = rcBtSize.bottom;
		iHSpace = (m_rcBtArea.GetWidth() - m_iNumBtPerLine * iWid) / (m_iNumBtPerLine-1);
		iVSpace = (m_rcBtArea.GetHeight() - m_iNumBtLine * iHei) / (m_iNumBtLine - 1);
	}

	char szEnter[40];
	const char* pszImage = NULL;

	for (int i=0; i < NUM_BUTTON; i++)
	{
		int x = m_rcBtArea.left + (i % m_iNumBtPerLine) * (iWid + iHSpace);
		int y = m_rcBtArea.top + (i / m_iNumBtPerLine) * (iHei + iVSpace);

		sprintf(szEnter, "button_image%d", i);
		if (!IniFile.ReadString(pszSection, szEnter, pszImage))
			continue;

		ARECT rcButton(x, y, x + iWid, y + iHei);
		m_aButtons[i].SetIniInfo(this, pszImage, rcButton);

		m_aButtons[i].m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndUnitHelp::OnBtnUnit);
	}

	//	Previous
	if (!m_btnPrev.LoadFromIni(this, IniFile, "wnd_unithelp::button_prev"))
	{
		ADEBUG_TRACE("Cannot create previous button.");
		return false;
	}

	m_btnPrev.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndUnitHelp::OnBtnClickPrev);
	m_btnPrev.EnableWnd(false);

	//	Next
	if (!m_btnNext.LoadFromIni(this, IniFile, "wnd_unithelp::button_next"))
	{
		ADEBUG_TRACE("Cannot create next button.");
		return false;
	}

	m_btnNext.m_wnOnClick.SetValue(this, (AWndNotify::AMethodType)&CMLWndUnitHelp::OnBtnClickNext);
	m_btnNext.EnableWnd(false);

	return true;
}

bool CMLWndUnitHelp::LoadRes()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::LoadRes())
		return false;
	
/*	ARECT rSprite(0, 0, m_rArea.GetWidth(), m_rArea.GetHeight());
	m_pBackimage = AWndSpriteMan::CreateSprite(m_strBackimage, m_rArea.GetWidth(), 
											   m_rArea.GetHeight(), 0, 1, &rSprite);
	if (!m_pBackimage)
	{
		ADEBUG_TRACE1("Cannot load image <%s>", m_strBackimage);
	}
*/
	CreateCameraAndView();

	return true;
}

bool CMLWndUnitHelp::FreeRes()
{
	J_EXCEPTION_RECORD(0);

	bool bRet = true;

	if (m_pBackimage)
	{
		AWndSpriteMan::ReleaseSprite(m_pBackimage);
		m_pBackimage = NULL;
	}

	ReleaseCameraAndView();

	return bRet && AWnd::FreeRes();
}

bool CMLWndUnitHelp::OnBtnUnit(AWnd* pWnd)
{
	int i;
	for (i=0; i < NUM_BUTTON; i++)
	{
		if (&m_aButtons[i] == pWnd)
			break;
	}

	if (i >= NUM_BUTTON)
		return false;

	ObserveUnit(true, i);

	if (!i)
	{
		m_btnPrev.EnableWnd(false);
		m_btnNext.EnableWnd(true);
	}
	else if (i == NUM_BUTTON-1)
	{
		m_btnPrev.EnableWnd(true);
		m_btnNext.EnableWnd(false);
	}
	else
	{
		m_btnPrev.EnableWnd(true);
		m_btnNext.EnableWnd(true);
	}

	return true;
}

void CMLWndUnitHelp::ShowUnitButtons(bool bShow)
{
	for (int i=0; i < NUM_BUTTON; i++)
		m_aButtons[i].ShowWindow(bShow);

	m_btnPrev.EnableWnd(!bShow);
	m_btnNext.EnableWnd(!bShow);
}

void CMLWndUnitHelp::ObserveUnit(bool bOpen, int iUnit)
{
	if (!bOpen)
	{
		//	Show all buttons
		ShowUnitButtons(true);
		m_bObserve = false;
		return;
	}

	//	Hide all buttons
	ShowUnitButtons(false);
	m_bObserve = true;

	if (!m_pCamera || !m_pViewport)
		return;

	//	Load model
	if (iUnit != m_iCurUnit || !m_pMoxModel)
	{
		g_theGame.ReleaseA3DModel(m_pMoxModel);

		int iModel;
		switch (iUnit)
		{
		case BT_PISTOL:		iModel = RES_MOX_HIGHPISTOL;	break;
		case BT_RIFLE:		iModel = RES_MOX_HIGHRIFLE;		break;
		case BT_MORTAR:		iModel = RES_MOX_HIGHMORTAR;	break;
		case BT_MACHINE:	iModel = RES_MOX_HIGHMACHINE;	break;
		case BT_RPG:		iModel = RES_MOX_HIGHRPG;		break;
		case BT_CANNON:		iModel = RES_MOX_CANNON;		break;
		case BT_ATTACKER:	iModel = RES_MOX_ATTACKER;		break;
		case BT_COPTER:		iModel = RES_MOX_COPTER;		break;
		case BT_FOOTMAN:	iModel = RES_MOX_FOOTMAN;		break;
		case BT_GUERILLA:	iModel = RES_MOX_GUERILLA;		break;
		case BT_ROCKET:		iModel = RES_MOX_ROCKETMAN;		break;
		case BT_TRUCK:		iModel = RES_MOX_TRUCK;			break;
		case BT_TANK:		iModel = RES_MOX_TANK;			break;
		case BT_ARMORED:	iModel = RES_MOX_ARMORED;		break;
		case BT_MIG21:		iModel = RES_MOX_MIG21;			break;
		case BT_FANTAN:		iModel = RES_MOX_FANTAN;		break;
		default:
			return;
		}

		if (!(m_pMoxModel = g_theGame.LoadA3DModel(iModel)))
			return;

		m_iCurUnit = iUnit;
	}

	//	Reset model's orientation
	m_pMoxModel->SetDirAndUp(g_vAxisZ, g_vAxisY);

	//	Adjust camera's position base on model's size
	A3DAABB aabb = m_pMoxModel->GetModelAABB();
	float fRadius = Magnitude(aabb.Extents) * 2.0f;

	A3DVECTOR3 vDir = Normalize(A3DVECTOR3(0.0f, -0.5f, -2.0f));
	m_pCamera->SetDirAndUp(vDir, g_vAxisY);

	float fScaleY = 0.6f;
	switch (iUnit)
	{
	case BT_PISTOL:		fScaleY = 0.3f;		break;
	case BT_MORTAR:		fScaleY = 0.2f;		break;
	case BT_FOOTMAN:	
	case BT_GUERILLA:	
	case BT_ROCKET:		
		
		fScaleY = 1.0f;
		m_pMoxModel->PlayActionByName("[³å·æ]", true);
		break;
	}

	m_pCamera->SetPos(-vDir * fRadius + g_vAxisY * (aabb.Extents.y * fScaleY));

	//	Load unit description
	LoadUnitDesc(iUnit);
}

//	Load unit description text
bool CMLWndUnitHelp::LoadUnitDesc(int iUnit)
{
	AWndIniFile IniFile;
	if (!IniFile.Init("Ini\\UnitHelp.ini"))
	{
		ADEBUG_TRACE("Cannot create unithelp file.");
		return false;
	}

	char szSect[50];
	sprintf(szSect, "unit_%d", iUnit);

	if (!IniFile.ReadInt(szSect, "count_line", m_UnitDesc.iNumLine))
		return false;

	if (!m_UnitDesc.iNumLine)
		return true;

	if (m_UnitDesc.iNumLine > MAX_DESCLINE)
		m_UnitDesc.iNumLine	= MAX_DESCLINE;

	int  i;
	char szKeyBuf[64];

	for (i=0; i < m_UnitDesc.iNumLine; i++)
	{
		const char* pszName	= NULL;
		sprintf(szKeyBuf, "line_%d", i);

		if (!IniFile.ReadString(szSect, szKeyBuf, pszName))
		{
			m_UnitDesc.iNumLine = 0;
			break;
		}

		m_UnitDesc.aLines[i] = pszName;
	}

	IniFile.Release();

	return true;
}

bool CMLWndUnitHelp::CreateCameraAndView()
{
	A3DDevice* pA3DDevice = g_theGame.GetA3DEngine()->GetA3DDevice();
	
	//	Create camera
	if (!(m_pCamera = new A3DCamera()))
	{
		ADEBUG_TRACE("Failed to create camera object!");
		return false;
	}
	
	if (!m_pCamera->Init(pA3DDevice, DEG2RAD(56.0f), 0.5f, 300.0f))
	{
		ADEBUG_TRACE("Failed to initialize camera.");
		return false;
	}
	
	m_pCamera->SetPos(g_vOrigin);
	m_pCamera->SetDirAndUp(g_vAxisZ, g_vAxisY);
	
	//	Create cannon viewport
	int iWidth  = m_rcBtArea.GetWidth();
	int iHeight = m_rcBtArea.GetHeight();

	if (!pA3DDevice->CreateViewport(&m_pViewport, m_rcBtArea.left, m_rcBtArea.top,
					iWidth, iHeight, 0.0f, 1.0f, true, true, 0))
	{
		ADEBUG_TRACE("Failed to create viewport.");
		return false;
	}
	
	m_pViewport->SetCamera(m_pCamera);

	return true;
}

void CMLWndUnitHelp::ReleaseCameraAndView()
{
	if (m_pViewport)
	{
		m_pViewport->Release();
		delete m_pViewport;
		m_pViewport = NULL;
	}

	if (m_pCamera)
	{
		m_pCamera->Release();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	g_theGame.ReleaseA3DModel(m_pMoxModel);
	m_pMoxModel = NULL;
}

bool CMLWndUnitHelp::OnMouseDown(const APOINT& ptClick)
{
	if (m_bObserve && 
		ptClick.x >= m_rcBtArea.left && ptClick.x <= m_rcBtArea.right &&
		ptClick.y >= m_rcBtArea.top && ptClick.y <= m_rcBtArea.bottom)
	{
		ObserveUnit(false, 0);
		return true;
	}

	return AWnd::OnMouseDown(ptClick);
}

bool CMLWndUnitHelp::OnBtnClickPrev(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	if (!m_iCurUnit)
		return true;

	ObserveUnit(true, m_iCurUnit-1);

	if (!m_iCurUnit)
		m_btnPrev.EnableWnd(false);
	else
		m_btnPrev.EnableWnd(true);

	m_btnNext.EnableWnd(true);

	return true;
}

bool CMLWndUnitHelp::OnBtnClickNext(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);

	if (m_iCurUnit == NUM_BUTTON-1)
		return true;

	ObserveUnit(true, m_iCurUnit+1);

	if (m_iCurUnit == NUM_BUTTON-1)
		m_btnNext.EnableWnd(false);
	else
		m_btnNext.EnableWnd(true);

	m_btnPrev.EnableWnd(true);

	return true;
}

bool CMLWndUnitHelp::ShowWindow(bool bShow)
{
	if (!bShow)
		ObserveUnit(false, 0);

	return AWnd::ShowWindow(bShow);
}

//	Tick animation
bool CMLWndUnitHelp::TickAnimation(float fTimeSpan)
{
	if (m_bObserve && m_pMoxModel)
		m_pMoxModel->TickAnimation();

	return true;
}

