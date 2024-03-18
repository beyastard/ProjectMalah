/*
 * FILE: ML_WndConsole.cpp
 *
 * DESCRIPTION: Console window for Malah game
 *
 * CREATED BY: Duyuxin, 2003/1/14
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_Interface.h"
#include "ML_WndConsole.h"
#include "ML_WndManager.h"
#include "ML_GamePlay.h"

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
//	Implement CMLWndConsole
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLWndConsole, AWndConsole, 1)

CMLWndConsole::CMLWndConsole()
{
	m_bVisible = false;
}

CMLWndConsole::~CMLWndConsole()
{
}

bool CMLWndConsole::Redraw()
{
	J_EXCEPTION_RECORD(0);
	return AWndConsole::Redraw();

/*	if (!IsVisible())
		return true;
	
	ADEBUG_TIME_TEST(OnDraw());
	ADEBUG_TIME_TEST(m_CommandInput.Redraw());
	ADEBUG_TIME_TEST(m_CommandList.Redraw());

	return true;
*/
}

bool CMLWndConsole::ShowWindow(bool bShow)
{
	if (!AWndConsole::ShowWindow(bShow))
		return false;
	
	m_CommandInput.SetSel();
	return true;
}

bool CMLWndConsole::OnInputEnter()
{
	J_EXCEPTION_RECORD(0);
	char szBuf[1024];
	int iStrLength = m_CommandInput.GetLine(0, szBuf, sizeof (szBuf));

	if (iStrLength > 0)
	{
		szBuf[iStrLength] = '\0';
		GETMLGAMEPLAY->ParseConsoleCmd(szBuf);

		m_CommandInput.SetSel();
		m_CommandHistory.AddString(szBuf);

		if (m_CommandHistory.GetLength() > 256)
			m_CommandHistory.RemoveAt(0);
		
		m_nHistoryIndex	= m_CommandHistory.GetLength();
	}

	return true;
}

bool CMLWndConsole::AddString(const char* szStr)
{
	J_EXCEPTION_RECORD(0);

	AddLine(szStr);
	return true;
}

bool CMLWndConsole::OnCreate()
{
	J_EXCEPTION_RECORD(0);
	if (!AWndConsole::OnCreate())
		return false;
	
	m_CommandList.SetScrollWidth(0);
	return true;
}

void CMLWndConsole::OnDestroy()
{
	J_EXCEPTION_RECORD(0);
	AWndConsole::OnDestroy();
}

bool CMLWndConsole::OnInputKey(AKeyboardButton akb, bool bDown)
{
	J_EXCEPTION_RECORD(0);
	if (!AWndConsole::OnInputKey(akb, bDown))
		return false;

	bool bControl = (0xF0 & GetKeyState(VK_CONTROL)) != 0;
	static bool bShow = false;

	switch (akb)
	{
	case AKB_UP:

		if (bDown)
		{
			if (bControl)
				m_CommandList.PageUp();
			else
				MoveHistory(-1);
		}

		break;

	case AKB_DOWN:

		if (bDown)
		{
			if (bControl)
				m_CommandList.PageDown();
			else
				MoveHistory(1);
		}

		break;

	case 192:

		if ((0xF0 & GetKeyState(192)) == 0)
		{
			if (bShow)
				((CMLWndManager*)CMLWndManager::GetWC())->SetActiveStatus(MLIG_NONE);

			bShow = !bShow;
		}

		break;

	case AKB_ESC:

		if ((0xF0 & GetKeyState(VK_ESCAPE)) == 0)
		{
			((CMLWndManager*)CMLWndManager::GetWC())->SetActiveStatus(MLIG_NONE);
			bShow = false;
		}

		break;
	}

	return true;
}

