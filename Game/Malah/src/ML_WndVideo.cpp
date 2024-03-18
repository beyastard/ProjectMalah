/*
 * FILE: ML_WndVideo.cpp
 *
 * DESCRIPTION: Video window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/4/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
*/

#include "AWC.h"
#include "AGame.h"
#include "jexception.h"
#include "ML_WndManager.h"
#include "ML_WndVideo.h"
#include "ML_Interface.h"

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
//	Implement CMLWndVideo
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndVideo, AWnd, 1)

CMLWndVideo::CMLWndVideo()
{
	m_bInited			= false;
	m_bCanBeFocus		= true;
	m_bVisible			= false;
	m_bClickToFinish	= true;
	m_bEscToFinish		= true;
}

CMLWndVideo::~CMLWndVideo()
{
}

bool CMLWndVideo::OnCreate()
{
	J_EXCEPTION_RECORD(0);
	
	if (!AWnd::OnCreate())
		return false;
	
	return true;
}

bool CMLWndVideo::OnKeyboardMsg(AWndKeyboardMsg& Msg, bool& bAccept)
{
	J_EXCEPTION_RECORD(0);
	
	if (!AWnd::OnKeyboardMsg(Msg, bAccept))
		return false;
	
	if (m_bEscToFinish)
	{
		bool bDown, bChanged;
		Msg.GetButtonStatus(AKB_ESC, bDown, bChanged);
		if (bDown && bChanged)
			ShowWindow(false);
	}

	if (m_VideoClip.IsStopped())
	{
		if(IsVisible())
			ShowWindow(false);
	}
	
	return true;
}

bool CMLWndVideo::OnMouseDown(const APOINT& ptClick)
{
	J_EXCEPTION_RECORD(0);

	if (!GetIsClickToFinish())
		return true;

	if (!AWnd::OnMouseDown(ptClick))
		return false;
	
	ShowWindow(false);

	return true;
}

void CMLWndVideo::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	Close();
	AWnd::OnDestroy();
}

bool CMLWndVideo::OnDraw()
{
	J_EXCEPTION_RECORD(0);

	if (!m_bInited)
		return true;
	
	m_VideoClip.Tick();
	m_VideoClip.DrawToBack();

	return true;
}

bool CMLWndVideo::ShowWindow(bool bShow)
{
	J_EXCEPTION_RECORD(0);

	if (!AWnd::ShowWindow(bShow))
		return false;
	
	if (!bShow)
	{
		Close();
		OnVideoStop();
	
		((CMLWndManager*)AWC::GetWC())->SetShowCursor(true);
	}
	else
	{
		bool bLoad = false;

		int iCurVideo = ((CMLWndManager*)AWC::GetWC())->GetCurVideo();
		if (iCurVideo == VIDEO_LOGO)
			bLoad = Open("Video\\logo.avi");

		if (!bLoad)
		{
			OnVideoStop();
			return true;
		}

		Start();

		((CMLWndManager*)AWC::GetWC())->SetShowCursor(false);
	}

	return true;
}

bool CMLWndVideo::Open(const char* pszFile)
{
	J_EXCEPTION_RECORD(0);

	A3DDevice* pAD = m_pAGame->GetA3DEngine()->GetA3DDevice();
	AMVideoEngine* pVE = m_pAGame->GetA3DEngine()->GetAMVideoEngine();

	if (!m_VideoClip.Init(pAD, pVE, const_cast<char*>(pszFile)))
	{
		m_VideoClip.Release();
		ADEBUG_TRACE1("Cannot open video file <%s>.", pszFile);
		return false;
	}

	m_VideoClip.SetAutoScaleFlags(AMVID_FIT_X | AMVID_FIT_Y | AMVID_FIT_KEEPCONSTRAINT);

	m_bInited = true;

	return true;
}

bool CMLWndVideo::Start()
{
	J_EXCEPTION_RECORD(0);

	if (!m_bInited)
		return false;
	
	AWC::GetWC()->SetFocus(this);
	m_pAGame->GetA3DEngine()->GetActiveViewport()->SetClearColor(A3DCOLORRGBA(0, 0, 0, 255));
	m_VideoClip.Start(false);

	return true;
}

bool CMLWndVideo::Stop()
{
	J_EXCEPTION_RECORD(0);
	
	if (!m_bInited)
		return false;
	
	m_VideoClip.Stop();

	return true;
}

void CMLWndVideo::Close()
{
	J_EXCEPTION_RECORD(0);

	if (!m_bInited)
		return;
	
	Stop();
	m_VideoClip.Release();
	m_bInited = false;
	
	return;
}

bool CMLWndVideo::OnVideoStop()
{
	J_EXCEPTION_RECORD(0);

	CMLWndManager* pWndMan = (CMLWndManager*)AWC::GetWC();

	if (pWndMan->GetCurVideo() == VIDEO_LOGO)
		pWndMan->SetActiveStatus(MLIG_LOADING);

	return true;
}

