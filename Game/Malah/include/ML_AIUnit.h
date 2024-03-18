/*
 * FILE: ML_AIUnit.h
 *
 * DESCRIPTION: AI unit class for game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/30
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_AIUNIT_H_
#define _ML_AIUNIT_H_

#include "ML_EventPt.h"
#include "ML_Unit.h"

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
//	Class CMLAIUnit
//
///////////////////////////////////////////////////////////////////////////

class CMLAIUnit : public CMLUnit
{
public:		//	Types

	//	Strategy
	enum
	{
		STRATEGY_NONE = 0,		//	None
		STRATEGY_WAIT,			//	Wait for appearing
		STRATEGY_DOEVENT,		//	Doing event on event point
		STRATEGY_RUNING,		//	Runing to next event point
		STRATEGY_DYING,			//	Dying
		STARTEGY_BEDEAD,		//	Has been dead
	};

	//	States
	enum
	{
		STATE_NONE = 0,			//	None
		STATE_STAND,			//	Stand
		STATE_RUN,				//	Run to next event point
		STATE_RUNSHOT,			//	Shot when runing
		STATE_ONEVENTPT,		//	Be on event point
		STATE_DYING,			//	Normal dying
		STATE_CATCHFIRE,		//	Catch fire
		STATE_DIEFLY,			//	Die fly
	};

	//	Sub-states
	enum
	{
		SUBSTATE_NONE = 0,		//	None
		SUBSTATE_RUN,			//	Run
		SUBSTATE_STAND,			//	Stand
		SUBSTATE_SQUAT,			//	Squat
		SUBSTATE_STANDSHOT,		//	Stand shot
		SUBSTATE_SQUATSHOT,		//	Squat shot
		SUBSTATE_JUMP,			//	Jump
		SUBSTATE_WAITANIM,		//	Waiting animation
	};

	//	Pose
	enum
	{
		POSE_STAND	= 0,	//	Stand
		POSE_SQUAT	= 1,	//	Squat
		POSE_RUN	= 2		//	Run
	};

	//	Actions
	enum
	{
		//	Image model actions
		ACT_JUMP_RIGHT = 0,			//	��ǹ��Ծ_��
		ACT_JUMP,					//	��ǹ��Ծ_��
		ACT_JUMP_LEFT,				//	��ǹ��Ծ_��
		ACT_CHARGE_SHOT,			//	������
		ACT_CHARGE_RIGHT45,			//	��ǹ���_��45��
		ACT_CHARGE_RIGHT90,			//	��ǹ���_��90��
		ACT_CHARGE,					//	��ǹ���_��0��
		ACT_CHARGE_LEFT45,			//	��ǹ���_��45��
		ACT_CHARGE_LEFT90,			//	��ǹ���_��90��
		ACT_DIE_BACKWARD0,			//	��_���˱�����
		ACT_DIE_BACKWARD1,			//	��_����ˤ
		ACT_DIE_BACKWARD2,			//	��_��������
		ACT_DIE_STRUGGLE,			//	��_Ť������
		ACT_DIE_FORWARD0,			//	��_ǰ�屻����
		ACT_DIE_FORWARD1,			//	��_ǰ�˵���
		ACT_DIE_FLY,				//	��_ը��
		ACT_DIE_FLY_LEFT,			//	ը���ڿ�_��
		ACT_DIE_FLY_RIGHT,			//	ը���ڿ�_��
		ACT_DIE_FLYFALL_LEFT,		//	ը�����_��
		ACT_DIE_FLYFALL_RIGHT,		//	ը�����_��
		ACT_STAND_SHOT,				//	վ�����
		ACT_STAND_REAMMO,			//	���_վ�ϵ�
		ACT_STAND_AIM,				//	վ����׼
		ACT_ONFIRE_DIE,				//	�Ż�_����
		ACT_ONFIRE,					//	�Ż�_����
		ACT_ONFIRE_RUN,				//	�Ż�_�Ӵ�
		ACT_SQUAT_SHOT_RIGHT45,		//	����_��45��
		ACT_SQUAT_SHOT_RIGHT80,		//	����_��80��
		ACT_SQUAT_SHOT,				//	����_��0��
		ACT_SQUAT_SHOT_LEFT45,		//	����_��45��
		ACT_SQUAT_SHOT_LEFT80,		//	����_��80��
		ACT_SQUAT_REAMMO_RIGHT,		//	���ϵ�_��
		ACT_SQUAT_REAMMO,			//	���ϵ�_��
		ACT_SQUAT_REAMMO_LEFT,		//	���ϵ�_��
		ACT_SQUAT_AIM_RIGHT45,		//	������45����׼
		ACT_SQUAT_AIM_RIGHT90,		//	������90����׼
		ACT_SQUAT_AIM,				//	������0����׼
		ACT_SQUAT_AIM_LEFT45,		//	������45����׼
		ACT_SQUAT_AIM_LEFT90,		//	������80����׼

		//	Mox model actions
		MOXACT_CHARGE,				//	���
		MOXACT_STAB,				//	��ɱ
		MOXACT_STAND,				//	վ��
		MOXACT_DIE0,				//	���� 0
		MOXACT_DIE1,				//	���� 1
		MOXACT_DIE2,				//	���� 2

		ACT_NUM,					//	Action number
		ACT_REMAIN					//	Remain current action
	};

public:		//	Constructor and Destructor

	CMLAIUnit();
	virtual ~CMLAIUnit();

public:		//	Attributes

	const static char*	m_aActions[NUM_AI_ID][ACT_NUM];		//	Image model actions

public:		//	Operations

	bool			Init(bool bNewObj, int iAIID, CMLRoute* pRoute, DWORD dwCol);	//	Initialize object
	virtual bool	Release();			//	Release object

	virtual void	SetPos(A3DVECTOR3& vPos);			//	Set position
	void			SetNoHeightPos(A3DVECTOR3& vPos);	//	Set position without height info.
	
	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);	//	Logic tick
	virtual bool	Render(A3DViewport* pViewport);		//	Render routine
	virtual bool	TickAnimation();					//	Tick animation

	void		UpdateAction();		//	Update action
	void		OutputDebugInfo();	//	Output debug information

	bool		RayTrace(MLRAYTRACE* pTrace);				//	Ray trace routine
	void		Strike(CMLBullet* pBullet);					//	Strike AI
	bool		Explode(CMLBullet* pBullet, bool bFire);	//	Explode

	void		SetCamera(A3DCamera* pCamera)	{	m_pCamera = pCamera;	}

protected:	//	Attributes

	bool			m_bUseMoxModel;		//	true, use mox model
	A3DImgModel*	m_pImgModel;		//	AI's image model
	A3DCamera*		m_pCamera;			//	Camera used to update image model
	A3DModel*		m_pMoxModel;		//	Mox model
	A3DGraphicsFX*	m_pGFX;				//	GFX object
	CMLRoute*		m_pRoute;			//	Route object this AI belongs to

	EVENTINTERATOR	m_EtInterator;		//	Event interator
	int				m_iCurEventPt;		//	Current event point
	A3DVECTOR3		m_vxzPos;			//	AI's position on xz plane
	A3DVECTOR3		m_vLastEPPos;		//	Last event position's position
	A3DVECTOR3		m_vNextEPPos;		//	Target position on next event point
	A3DVECTOR3		m_vLinkerDir;		//	Direction of link line between two event point
	float			m_fLinkerLen;		//	Length of link line between two event point
	A3DVECTOR3		m_vCurTarget;		//	Current target position
	A3DVECTOR3		m_vCurMoveDir;		//	Current move direction
	bool			m_bLastRunSeg;		//	Last running segment
	int 			m_iYawFlag;			//	Yaw flag
	bool			m_bRushTo;			//	true, rush to next event point

	float			m_fAISpeed;			//	Speed of this AI
	int				m_iAIMSInterval;	//	AI move shot inteval time
	int				m_iAISSInterval;	//	AI stand shot inteval time
	int				m_iAICSInterval;	//	AI creep shot inteval time
	int				m_iAIStabInterval;	//	Stab interval
	int				m_iLife;			//	Life value
	int				m_iAIAttackCopter;	//	AI attack copter posibility

	int				m_iStrategy;		//	Current strategy
	int				m_iState;			//	Current state
	int				m_iSubState;		//	Current Sub-state
	int				m_iAction;			//	Animation action
	int				m_iFlyAction;		//	Die fly action

	float			m_fTimeSpan;		//	Time spane of this logic tick
	int				m_iTimeSpan;		//	Time spane of this logic tick
	DWORD			m_dwTimeCnt;		//	time counter of this logic tick
	bool			m_bNewEvent;		//	New event flag
	int				m_iEventTimeCnt;	//	Event time counter
	int				m_iActTimeCnt;		//	Action time count
	int				m_iWaitAnimCnt;		//	Wait animation count
	int				m_iWaitReason;		//	Wait animation reason
	int				m_iCheckPosCnt;		//	Time counter used to check position

	bool			m_bInPreserve;		//	true, in preserve area
	bool			m_bCatchFire;		//	Catch fire flag

protected:	//	Operations

	bool		LogicRun_Wait();		//	Logic tick when waiting
	bool		LogicRun_DoEvent();		//	Logic tick when doing events
	bool		LogicRun_Runing();		//	Logic tick when running
	bool		LogicRun_Dying();		//	Logic tick when dying

	void		InitStates();					//	Initialize states
	void		SetoutToNextEventPt();			//	Setout to next event point
	void		SetoutToNextRunTarget();		//	Setout to next running target position
	void		BeginEventPoint(bool bNewPos);	//	Begin event point
	void		Fire(int iPose);				//	Fire function
	void		RocketManFire(int iPose);		//	Rocket man fire
	void		Stab();							//	Stab
	void		Shift(float fMin, float fMax);	//	Shift position
	void		OnDie(int iDieFrom);			//	On die
	bool		DoHitCheck();					//	Do hit check ?
	void		MoveFireOnBody(bool bStand);	//	Move fire
	
	float		GetDirDegree(A3DVECTOR3& vDir, int* piSide, float fFace=0.17365f);	//	Get direction degree for image model
	void		SelectDirAction(int iAction, A3DVECTOR3& vDir);			//	Select proper direction action
	void		SelectDyingAction(bool bFly, A3DVECTOR3* pDir);			//	Select dying action
	void		SelectCatchFireAction(float fDist);						//	Select catch fire action
	bool		ReachCurrentTarget(A3DVECTOR3& vDestPos);				//	Reach current position ?
	A3DVECTOR3	GetAIToPlayerDir(A3DVECTOR3* pvRight, float* pfDist);	//	Get the direction from AI to player
	int			AttenuateDamage(int iDamage, A3DVECTOR3& vDest);		//	Attenuate damage
	int			AttenuatePrecise(int iPrecise, A3DVECTOR3& vDest);		//	Attenuate precise
	float		GetAIToPlayerDist();									//	Get distance between AI and player

	void		DoEvent(bool bNew);				//	Do event on event point
	bool		DoEvent_StandShot();			//	Footman stand shot
	bool		DoEvent_CreepShot();			//	Footman creep shot
	bool		DoEvent_ShiftShot();			//	Footman shift shot
	bool		DoEvent_UseMoxModel();			//	Change to mox model
	bool		DoEvent_JumpForward();			//	Jump forward
	bool		DoEvent_Stab();					//	Stab
	bool		DoEvent_Stand();				//	Stand
	bool		DoEvent_Creep();				//	Creep
	bool		DoEvent_RocketStandShot();		//	Rocket man stand shot
	bool		DoEvent_RocketCreepShot();		//	Rocket man creep shot
	bool		DoEvent_RocketShiftShot();		//	Rocket man shift shot

	void		DoState_Run(bool bNew);			//	Run
	void		DoState_RunShot(bool bNew);		//	Run state
	void		DoState_Dying(bool bNew);		//	Dying
	void		DoState_CatchFire(bool bNew);	//	Catch fire
	void		DoState_DieFly(bool bNew);		//	Die fly
	bool		DoState_BeDead(bool bNew);		//	Be dead

	void		PlayAction(int iAction);		//	Play action

	inline void	WaitAnimation(int iFrame, int iReason=0);	//	Begin to wait animation
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

void CMLAIUnit::WaitAnimation(int iFrame, int iReason)
{
	m_iSubState		= SUBSTATE_WAITANIM;
	m_iWaitAnimCnt	= iFrame;
	m_iWaitReason	= iReason;
}


#endif	//	_ML_AIUNIT_H_

