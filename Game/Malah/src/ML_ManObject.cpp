/*
 * FILE: ML_ManObject.cpp
 *
 * DESCRIPTION: General object manager class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_ManObject.h"
#include "ML_ManagerDef.h"
#include "ML_Route.h"
#include "ML_Helicopter.h"
#include "ML_Attacker.h"
#include "ML_FixedObject.h"
#include "ML_Cannon.h"
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
//	Implement CMLManObject
//
///////////////////////////////////////////////////////////////////////////

IMPLEMENT_ASERIAL(CMLManBase, AManager, 1)
IMPLEMENT_ASERIAL(CMLManObject, CMLManBase, 1)

CMLManObject::CMLManObject() : CMLManBase()
{
	m_managerId = MANID_OBJECT;

	m_dwTimeCnt		= 0;
	m_pCurCannon	= NULL;
	m_iNumCopter	= 0;
	m_pCannonCamera	= NULL;
	m_pCannonView	= NULL;
}

/*	Initialize object

	Return true for success, otherwise return false.

	pGame: game object's address
*/
bool CMLManObject::Init(AGame* pGame)
{
	if (!CMLManBase::Init(pGame))
	{
		ADEBUG_TRACE("Failed to call CMLManBase::Init");
		return false;
	}

	assert(m_pAGame);

	return true;
}

//	Release object
bool CMLManObject::Release()
{
	return CMLManBase::Release();
}

//	Message handler
int	CMLManObject::OnMsg(AMSG* pMSG)
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
bool CMLManObject::ProcessMsg(MLMSG* pMsg)
{
	return true;
}

//	Load game mission
bool CMLManObject::LoadMissionResources(AArchive& ar)
{
	//	Load all fixed objects
	int i, iNumObject;

	//	Load number of route
	if (!ar.Read(&iNumObject, sizeof (iNumObject)))
		return false;

	for (i=0; i < iNumObject; i++)
	{
		CMLFixedObject* pObject = new CMLFixedObject;
		if (!pObject)
		{
			assert(0);
			ADEBUG_TRACE("Not enough memory!");
			return false;
		}
	
		if (!pObject->Load(ar))
			return false;

		m_FixedObjList.AddTail(pObject);
	}

	return true;
}

//	Release mission resources
void CMLManObject::ReleaseMissionResources()
{
	//	Release all fixed objects
	AListPosition pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		pObject->Release();
		delete pObject;
	}

	m_FixedObjList.RemoveAll();
}

//	Logic tick
bool CMLManObject::LogicRun(float fTimeSpan)
{
	BeginLogicCount();
	
	m_dwTimeCnt += ML_ConvertTimeSpan(fTimeSpan);

	//	Large scaled objects
	AListPosition thePos, pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		pObject->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pObject->IsDead())
		{
			if (m_pCurCannon == pObject)
				m_pCurCannon = NULL;
			else if (pObject->GetId() == ID_SUPP_COPTER)
				m_iNumCopter--;

			pObject->Release();
			delete pObject;
			m_LargeList.RemoveAt(thePos);
		}
	}

	//	Bullet shells
	pos = m_ShellList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLUnit* pObject = (CMLUnit*)m_ShellList.GetNext(pos);
		pObject->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pObject->IsDead())
		{
			pObject->Release();
			m_ShellPool.FreeObject((CMLBulletShell*)pObject);
			m_ShellList.RemoveAt(thePos);
		}
	}

	//	Fixed objects
	pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		thePos = pos;
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		pObject->LogicRun(fTimeSpan, m_dwTimeCnt);

		if (pObject->IsDead())
			pObject->EnableRender(false);
	}

	EndLogicCount();

	return true;
}

//	Create helicopter
bool CMLManObject::CreateHelicopter(CMLRoute* pRoute)
{
	CMLHelicopter* pCopter = new CMLHelicopter;
	if (!pCopter || !pCopter->Init(pRoute))
	{
		ADEBUG_TRACE("Failed to create helicopter");
		return false;
	}

	m_iNumCopter++;
	m_LargeList.AddTail(pCopter);

	return true;
}

//	Create attacker
bool CMLManObject::CreateAttacker(A3DVECTOR3& vAttackPos)
{
	CMLAttacker* pAttacker = new CMLAttacker;
	if (!pAttacker || !pAttacker->Init(vAttackPos))
	{
		ADEBUG_TRACE("Failed to create attacker");
		return false;
	}

	m_LargeList.AddTail(pAttacker);

	return true;
}

//	Create cannon
bool CMLManObject::CreateCannon(A3DVECTOR3& vBombPos)
{
	CMLCannon* pCannon = new CMLCannon;
	if (!pCannon || !pCannon->Init(vBombPos))
	{
		ADEBUG_TRACE("Failed to create cannon");
		return false;
	}

	m_LargeList.AddTail(pCannon);
	m_pCurCannon = pCannon;

	return true;
}

/*	Create bullet shell

	iID: bullet shell's ID.
	vPos: position.
	vVel: velocity
*/
bool CMLManObject::CreateBulletShell(int iID, A3DVECTOR3& vPos, A3DVECTOR3& vVel)
{
	CMLBulletShell* pShell = m_ShellPool.AllocObject();
	if (!pShell || !pShell->Init(iID, vPos, vVel))
	{
		ADEBUG_TRACE("Failed to create bullet shell");
		return false;
	}

	m_ShellList.AddTail(pShell);

	return true;
}

//	Tick animation
bool CMLManObject::TickAnimation()
{
	//	Large scaled objects
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		pObject->TickAnimation();
	}

	//	Bullet shells
	pos = m_ShellList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_ShellList.GetNext(pos);
		pObject->TickAnimation();
	}

	//	Fixed objects
	pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		pObject->TickAnimation();
	}

	return true;
}

//	Render routines
bool CMLManObject::Render(A3DViewport* pViewport)
{
	//	Large scaled objects
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		if (pObject->CanRender())
			pObject->Render(pViewport);
	}

	//	Bullet shells
	pos = m_ShellList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_ShellList.GetNext(pos);
		if (pObject->CanRender())
			pObject->Render(pViewport);
	}

	//	Fixed objects
//	g_theGame.EnableAlphaTest(true, A3DCMP_GREATEREQUAL, 80);

	pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		if (pObject->CanRender())
			pObject->Render(pViewport);
	}

//	g_theGame.EnableAlphaTest(false, A3DCMP_GREATEREQUAL, 0);

	return true;
}

//	Render cannon
bool CMLManObject::RenderCannon(A3DViewport* pViewport)
{
	if (!m_pCurCannon || !m_pCurCannon->CanRender())
		return true;

	return m_pCurCannon->Render(pViewport);
}

//	Release all large scaled objects
void CMLManObject::ReleaseAllLarge()
{
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		pObject->Release();
		delete pObject;
	}

	m_LargeList.RemoveAll();

	m_pCurCannon = NULL;
	m_iNumCopter = 0;
}

//	Release all shells
void CMLManObject::ReleaseAllShells()
{
	AListPosition pos = m_ShellList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_ShellList.GetNext(pos);
		pObject->Release();
		m_ShellPool.FreeObject((CMLBulletShell*)pObject);
	}

	m_ShellList.RemoveAll();
}

//	Release all objects
void CMLManObject::ReleaseAllObjects()
{
	ReleaseAllLarge();
	ReleaseAllShells();
}

//	Called when begin game playing
bool CMLManObject::OnStartGamePlay()
{
	if (!m_ShellPool.Init())
	{
		ADEBUG_TRACE("Failed to initialize bullet shell allocated pool");
		return false;
	}

	if (!CreateCannonWindow())
	{
		ADEBUG_TRACE("Failed to create cannon camera and viewport");
		return false;
	}

	return true;
}

//	Called when end game playing
bool CMLManObject::OnEndGamePlay()
{
	m_ShellPool.Release();
	ReleaseMissionResources();
	ReleaseCannonWindow();
	return true;
}

//	Called when being mission
bool CMLManObject::OnBeginMission(int iMission)
{
	m_dwTimeCnt = 0;

	return true;
}

//	Called when reset mission
bool CMLManObject::OnResetMission()
{
	m_dwTimeCnt = 0;

	//	Reset all fixed objects
	AListPosition pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		pObject->Reset();
	}

	return true;
}

//	Called when end mission
bool CMLManObject::OnEndMission()
{
	ReleaseAllObjects();
	return true;
}

/*	Select a random helicopter

	Return helicopter object's address if it was founded
*/
CMLHelicopter* CMLManObject::SelectRandomCopter()
{
	if (!m_iNumCopter)
		return NULL;

	int iCount=0, iSel = MLA_Random(0, m_iNumCopter-1);

	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		if (pObject->GetId() == ID_SUPP_COPTER)
		{
			if (iCount == iSel)
				return (CMLHelicopter*)pObject;

			iCount++;
		}
	}

	return NULL;
}

//	Helicopter ray trace routine
bool CMLManObject::CopterRayTrace(MLRAYTRACE* pTrace)
{
	MLRAYTRACE Trace;
	Trace.vStart	= pTrace->vStart;
	Trace.vVel		= pTrace->vVel;
	Trace.fTime		= pTrace->fTime;

	float fMinFrac = 1.0f;

	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		if (pObject->GetId() == ID_SUPP_COPTER)
		{
			if (((CMLHelicopter*)pObject)->RayTrace(&Trace) &&
				Trace.Trace.fFraction < fMinFrac)
			{
				fMinFrac = Trace.Trace.fFraction;
				*pTrace	 = Trace;
			}
		}
	}

	if (fMinFrac < 1.0f)
		return true;

	return false;
}

//	Fixed object ray trace
bool CMLManObject::FixedObjectRayTrace(MLRAYTRACE* pTrace)
{
	MLRAYTRACE Trace;
	Trace.vStart	= pTrace->vStart;
	Trace.vVel		= pTrace->vVel;
	Trace.fTime		= pTrace->fTime;

	float fMinFrac = 1.0f;

	AListPosition pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		if (pObject->RayTrace(&Trace) && Trace.Trace.fFraction < fMinFrac)
		{
			fMinFrac = Trace.Trace.fFraction;
			*pTrace	 = Trace;
		}
	}

	if (fMinFrac < 1.0f)
		return true;

	return false;
}

//	Bullet explode
bool CMLManObject::FixedObjectExplode(CMLBullet* pBullet, bool bFire)
{
	AListPosition pos = m_FixedObjList.GetHeadPosition();
	while (pos)
	{
		CMLFixedObject* pObject = (CMLFixedObject*)m_FixedObjList.GetNext(pos);
		pObject->Explode(pBullet, bFire);
	}

	return true;
}

//	Create cannon camera and viewport
bool CMLManObject::CreateCannonWindow()
{
	A3DDevice* pA3DDevice = g_theGame.GetA3DEngine()->GetA3DDevice();
	
	//	Create camera
	if (!(m_pCannonCamera = new A3DCamera()))
	{
		ADEBUG_TRACE("Failed to create camera object!");
		return false;
	}
	
	if (!m_pCannonCamera->Init(pA3DDevice, DEG2RAD(56.0f), 0.1f, 200.0f))
	{
		ADEBUG_TRACE("Failed to initialize camera.");
		Release();
		return false;
	}
	
	A3DVECTOR3 vPos = A3DVECTOR3(-6.0f, 1.5f, -4.5f);
	m_pCannonCamera->SetPos(vPos);
	
	vPos	= -vPos;
	vPos.y	= 1.0f;
	m_pCannonCamera->SetDirAndUp(Normalize(vPos), g_vAxisY);
	
	//	Create cannon viewport
	int iWidth  = (int)((float)GETCURUSERCONFIGS->iRenderWid / BASERENDER_WIDTH * CMLCannon::m_iViewWid + 0.5f);
	int iHeight = (int)((float)GETCURUSERCONFIGS->iRenderHei / BASERENDER_HEIGHT * CMLCannon::m_iViewHei + 0.5f);

	if (!pA3DDevice->CreateViewport(&m_pCannonView, 115, //	GETCURUSERCONFIGS->iRenderWid - iWidth, 
					GETCURUSERCONFIGS->iRenderHei-iHeight, iWidth, iHeight,
					0.0f, 1.0f, false, true, 0))
	{
		ADEBUG_TRACE("Failed to create viewport.");
		Release();
		return false;
	}
	
	m_pCannonView->SetCamera(m_pCannonCamera);

	return true;
}

//	Release cannon camera and viewport
void CMLManObject::ReleaseCannonWindow()
{
	if (m_pCannonView)
	{
		m_pCannonView->Release();
		delete m_pCannonView;
		m_pCannonView = NULL;
	}

	if (m_pCannonCamera)
	{
		m_pCannonCamera->Release();
		delete m_pCannonCamera;
		m_pCannonCamera = NULL;
	}
}

//	Get current friend plane number
int	CMLManObject::GetFriendPlaneNum()
{
	int iNum = 0;

	//	Large scaled objects
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		if (pObject->GetId() == ID_SUPP_COPTER ||
			pObject->GetId() == ID_SUPP_ATTACKER)
			iNum++;
	}

	return iNum;
}

//	Pause sound
void CMLManObject::PauseSound(bool bPause)
{
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		pObject->PauseSound(bPause);
	}
}

//	Stop sound
void CMLManObject::StopSound()
{
	AListPosition pos = m_LargeList.GetHeadPosition();
	while (pos)
	{
		CMLUnit* pObject = (CMLUnit*)m_LargeList.GetNext(pos);
		pObject->StopSound();
	}
}

