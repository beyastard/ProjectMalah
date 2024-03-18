// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: A3DAssistSnd.h
// Creator: Wei Hua (κ��)
// Ϊ��Ч���Ļ���

#ifndef	__A3DASSISTSND_H__
#define	__A3DASSISTSND_H__

#include "A3DAssistCache.h"
#include <a3d.h>

// ����
#define	AA_SND_PROPERTY_CANFADE			0x01	// ���Ե��뵭��
#define	AA_SND_PROPERTY_NEEDFADETICK	0x02	// ��Ҫ����tick����

// ��������
bool	IsSndStopped(AMSoundBuffer *pAMSoundBuffer);

// ÿ��wavֻ��ͬʱһ��(OneSoundOnePlay)
class	A3DAssist2DOSOP	: public A3DAssistCache_String
{
protected:
	bool			m_bJustLoad;				// �Ƿ�ֻload������
	AMSoundBuffer	**m_ppAMSoundBuffer;
	A3DEngine		*m_pA3DEngine;
	unsigned long	m_nFadeInFinishTime;		// ������������ʾ�ڴ�ʱ��ǰ���ŵ�������Ҫfade������fadein
protected:
	bool	ClrSnd(int i);
	AMSoundBuffer	*SearchSndBuf(char *szSndFile);
	virtual bool	StillInUse(int nIdx);
protected:
	int		CheckAndSetSnd(char *szSndFile);
public:
	inline void	SetJustLoad(bool bSet)
	{ m_bJustLoad = bSet; }
public:
	A3DAssist2DOSOP();
	~A3DAssist2DOSOP();
	bool	Init(A3DEngine *pA3DEngine, int nTotal, int nStrLen);
	bool	Release();
	bool	LogicRun();							// �ṩfade�ȹ���
	bool	ResetAll();
	bool	Play(char *szSndFile, bool bImmEffect, bool bLoop, bool bCanFade);
	bool	Stop(char *szSndFile);
	bool	Pause(char *szSndFile);
	bool	IsStopped(char *szSndFile);
	bool	FadeIn(int ms);
	bool	FadeOut();
};

// wav�����ã�ֻ����ͬʱ���ڵĸ���
class	A3DAssist2DSnd		: public A3DAssistCache_StrDup
{
protected:
	bool			m_bJustLoad;				// �Ƿ�ֻload������
	AMSoundBuffer	**m_ppAMSoundBuffer;
	A3DEngine		*m_pA3DEngine;
	unsigned long	m_nFadeInFinishTime;		// ������������ʾ�ڴ�ʱ��ǰ���ŵ�������Ҫfade������fadein
protected:
	bool	ClrSnd(int i);
	virtual bool	StillInUse(int nIdx);
public:
	inline void	SetJustLoad(bool bSet)
	{ m_bJustLoad = bSet; }
public:
	A3DAssist2DSnd();
	~A3DAssist2DSnd();
	bool	Init(A3DEngine *pA3DEngine, int nTotal, int nStrLen);
	bool	Release();
	bool	LogicRun();							// �ṩfade�ȹ���
	bool	ResetAll();
	bool	Play(char *szSndFile, bool bImmEffect, bool bCanFade);		// ��Ϊ����ͣ�����Բ�����loop
	bool	FadeIn(int ms);
	bool	FadeOut();
};

class	A3DAssist3DSnd		: public A3DAssistCache_StrDup
{
protected:
	bool			m_bJustLoad;				// �Ƿ�ֻload������
	AM3DSoundBuffer	**m_ppAM3DSoundBuffer;
	A3DEngine		*m_pA3DEngine;
	unsigned long	m_nFadeInFinishTime;		// ������������ʾ�ڴ�ʱ��ǰ���ŵ�������Ҫfade������fadein
protected:
	bool	ClrSnd(int i);
	virtual bool	StillInUse(int nIdx);
public:
	inline void	SetJustLoad(bool bSet)
	{ m_bJustLoad = bSet; }
public:
	A3DAssist3DSnd();
	~A3DAssist3DSnd();
	bool	Init(A3DEngine *pA3DEngine, int nTotal, int nStrLen);
	bool	Release();
	bool	LogicRun();							// �ṩfade�ȹ���
	bool	ResetAll();
	bool	Play(char *szSndFile, A3DVECTOR3 &vecPos, float fMinDist, float fMaxDist, bool bImmEffect, bool bForce2D, bool bCanFade);	// ������Ϊ�˼ӿ촫���ٶ�
	bool	FadeIn(int ms);
	bool	FadeOut();
};

#endif