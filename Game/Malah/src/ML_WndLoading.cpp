/*
 * FILE: ML_WndLoading.cpp
 *
 * DESCRIPTION: Loading interface for Malah game
 *
 * CREATED BY: Duyuxin, 2003/2/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "ML_Global.h"
#include "ML_WndLoading.h"
#include "ML_GamePlay.h"
#include "ML_WndManager.h"
#include "AButton.h"
#include <jexception.h>

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
//	Implement CMLLoadButton
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLLoadButton, AButton, 1)

bool CMLLoadButton::PointIn(const APOINT& pt)
{
	return AButton::PointIn(pt);
}

bool CMLLoadButton::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	APOINT ptAb;
	GetAbsolutePos(ptAb);
	ARECT rArea(ptAb.x, ptAb.y, m_rArea.GetWidth(), m_rArea.GetHeight());
	UINT uColor;

	AWC::SetCurrentFont(FONT_FONT2_16);

	if (m_bHovered)
	{
		if (m_bPushed)
			uColor = 0xFFDD8888;
		else
			uColor = 0xFFFFAAAA;
	}
	else
		uColor = 0xFFFF0000;
	
	AWC::DrawText(ptAb, "LOAD GAME", -1, uColor);

	return true;
}

bool CMLLoadButton::OnClick()
{
	J_EXCEPTION_RECORD(0);
	return ((CMLWndLoading*)m_pParent)->OnLoadingClick();
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLLoadButton
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndLoading, AWnd, 0x0100)

CMLWndLoading::CMLWndLoading()
{
	m_bVisible	= false;
}

CMLWndLoading::~CMLWndLoading()
{
}

bool CMLWndLoading::OnLoadingClick()
{
	J_EXCEPTION_RECORD(0);

/*	AEnyaGameLoadingCallback Callback;
	Callback.SetValue(this, (AEnyaGameLoadingCallback::AMethodType)OnLoadingUpdate);

	ALoadFile Stream(Callback);
	if (Stream.Create("data/test.sav", OPEN_EXISTING)== false)
	{
		assert(0);
		return false;
	}

	AArchive ar(&Stream, true);
	AEnyaGamePlay* pGamePlay = (AEnyaGamePlay*)m_pAGame->GetAGamePlay();
	pGamePlay->Reset();

	if (!pGamePlay->Load(ar))
	{
		ADEBUG_TRACE("Cannnot load game play!");
		return false;
	}

	((AEnyaWC*)AWC::GetWC())->SetActiveStatus(0);
*/
	return true;
}

bool CMLWndLoading::OnCreate()
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::OnCreate())
		return false;

	ARECT rbtnLoading(700, 570, 800, 600);
/*	if (!m_btnLoading.Create(this, rbtnLoading))
	{
		ADEBUG_TRACE("Cannot create loading button.");
		return false;
	}
*/
	AWndIniFile	IniFile;
	if (!IniFile.Init("Ini\\MainMenu.ini"))
	{
		ADEBUG_TRACE1("Cannot read ini file <%s>.", "MainMenu.ini");
		return false;
	}

	m_wndMainMenu.ShowWindow(true);

	if (!m_wndMainMenu.LoadFromIni(this, IniFile, "main_menu"))
	{
		ADEBUG_TRACE("Cannot load main menu.");
		return false;
	}

	IniFile.Save();
	IniFile.Release();

/*	m_wndVideoHead.Create(this, m_rArea);
	
	if (!m_wndVideoHead.Open("\\\\Jiangli\\incoming\\ffxi.avi"))
		return true;
	
	m_wndVideoHead.ShowWindow(true);
	m_wndVideoHead.Start();
	m_wndVideoHead.m_wnOnVideoStop.SetValue(this, (AWndNotify::AMethodType)OnHeadVideoStop);
*/	
	return true;
}

bool CMLWndLoading::OnHeadVideoStop(AWnd* pWnd)
{
	J_EXCEPTION_RECORD(0);
	return m_wndMainMenu.ShowWindow(true);
}

bool CMLWndLoading::ProcessMouseMsg(AWndMouseMsg& Msg)
{
	J_EXCEPTION_RECORD(0);
	return AWnd::ProcessMouseMsg(Msg);
}

void CMLWndLoading::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWnd::OnDestroy();
}

bool CMLWndLoading::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (bShow)
		m_wndMainMenu.LoadSetting();

	return true;
}

int CMLWndLoading::OnLoadingUpdate(float fPercent)
{
	return 1;
}

bool CMLWndLoading::OnDraw()
{
//	AWC::DrawRectangle(m_rArea, 0xFFAAAADF);
	return true;
}

