/*
 * FILE: AGame.h
 *
 * DESCRIPTION: AI��Ļ���
 *
 * CREATED BY: Tom Zhou
 *
 * MODIFIER:  Wei Hua
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef __ACAMERA__H__
#define __ACAMERA__H__

#include "AbaseA3D.h"
#include "AUnit.h"

class AGame;

class ACamera	: public AUnit
{
protected:
	AGame *				m_pAGame;
	A3DVECTOR3			m_velocity;

	A3DVECTOR3			m_pos;
	A3DVECTOR3			m_dir;
	float				m_fSpeed;		// �ƶ��ٶ�
	float				m_fTurnSpeed;	// ת���ٶ�
	float				m_fDamp;		// ����ϵ��
	float				m_turn;
	float				m_look;

protected:
	
public:
	virtual bool LogicRun(float __TimeSpan);
	bool TurnAroundAxis(float vDeltaRad);
	ACamera();
	~ACamera();

	A3DCamera * GetA3DCamera(void);
	inline void SetMoveSpeed(float fSpeed)	{ m_fSpeed = fSpeed;}
	inline void IncMoveSpeed(float fDelta)	{ m_fSpeed += fDelta;}
	inline float GetMoveSpeed(void)			{ return	m_fSpeed;}
	inline void SetTurnSpeed(float fSpeed)	{ m_fTurnSpeed = fSpeed;}
	inline void IncTurnSpeed(float fDelta)	{ m_fTurnSpeed += fDelta;}
	inline float GetTurnSpeed(void)			{ return	m_fTurnSpeed;}
	inline void SetDamp(float fDamp)		{ m_fDamp = fDamp;}
	inline float GetDamp(void)				{ return	m_fDamp ;}

	bool Init(AGame * __pAGame);

	// __dir(0~360)
	bool Move(int __dir);
	// __dir(����)
	bool Lift(int __dir);
	// ֹͣ
	bool Stop();
	// ת��
	bool Turn(float h);
	bool Look(float v);
};

#endif //_ACAMERA__H__
