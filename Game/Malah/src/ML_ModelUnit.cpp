/*
 * FILE: ML_ModelUnit.cpp
 *
 * DESCRIPTION: Model unit base class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/4/24
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_Global.h"
#include "ML_ModelUnit.h"

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
//	Implement CMLModelUnit
//
///////////////////////////////////////////////////////////////////////////

CMLModelUnit::CMLModelUnit() : CMLUnit()
{
	m_pMoxModel	= NULL;
	m_bSort		= true;
	m_bCollect	= false;
}

//	Release resources
bool CMLModelUnit::Release()
{
	if (m_pMoxModel)
	{
		g_theGame.ReleaseA3DModel(m_pMoxModel);
		m_pMoxModel = NULL;
	}

	return CMLUnit::Release();
}

//	Render routine
bool CMLModelUnit::Render(A3DViewport* pViewport)
{
	if (m_bRender && m_pMoxModel)
		m_pMoxModel->Render(pViewport, m_bSort, m_bCollect);

	return true;
}

//	Tick animation
bool CMLModelUnit::TickAnimation()
{
	if (m_pMoxModel)
		m_pMoxModel->TickAnimation();

	return true;
}

//	Pause sound
void CMLModelUnit::PauseSound(bool bPause)
{
	if (m_pMoxModel)
		m_pMoxModel->PauseAllSFX(bPause);
}

//	Stop sound
void CMLModelUnit::StopSound()
{
	if (m_pMoxModel)
		m_pMoxModel->StopAllSFX();
}

//	Set position
void CMLModelUnit::SetPos(A3DVECTOR3& vPos)
{
	CMLUnit::SetPos(vPos);

	if (m_pMoxModel)
		m_pMoxModel->SetPos(vPos);
}

/*	Play model action

	Return true if action was played, otherwise return false.

	szName: action's name
	bDiscardOld: true, stop old action immediately.
	bStopSFX: true, stop old SFX
	bStopGFX: true, stop old GFX
*/
bool CMLModelUnit::PlayModelAction(char* szName, bool bDiscardOld, bool bStopSFX/* true */,
								   bool bStopGFX/* true */)
{
	if (!m_pMoxModel)
		return false;

	return g_theGame.PlayA3DModelAction(m_pMoxModel, szName, bDiscardOld, bStopSFX, bStopGFX);
}
