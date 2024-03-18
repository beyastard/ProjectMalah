/*
 * FILE: AUnit.h
 *
 * DESCRIPTION: ���о����߼������Ķ���Ļ���
 *
 * CREATED BY: ��Զ��, 2002/8/9
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */

#ifndef __AUNIT__H__
#define __AUNIT__H__

#include "AIDObject.h"
#include "AData.h"
#include "ABaseA3D.h"
#include "AUnitInWorldInterface.h"

struct AMSG;
typedef AMSG *PAMSG;
class AEvent;
class AGame;
//class AListT;

#define VERIFY_OBJECT_TYPE(pObject, typeClass) \
	(typeClass*)VerifyObjectType(pObject, CLASS_INFO(typeClass), __FILE__, __LINE__)

void* VerifyObjectType(AObject* pObject,
	AClassInfo* pInfo, LPCSTR pszFile, int nLine);

class AUnit : public AIDObject  
{
public:
	DECLARE_ASERIAL(AUnit)
protected:

	AGame *			m_pAGame;
	unsigned long	m_iTicks;

	// By JiangLi
	// ����һ��������AData�Ķ������������ж�д����
	// �Ĳ������ͱ���������������������ʵ����ݶ���
	virtual AData* CreateData();
	// ������д�뵽���ݶ����ͨ������һ�������ʱ����Ҫִ����
	// �����������������Ӧ�����ܹ�������ȫ�ָ�����ġ�
	virtual bool OnWriteData(AData* pData);
	// �����ݶ����ж�ȡ���ݡ���������п�����Դ������������Դ��
	// �ڴ��е����ݣ�Ҳ�п����Ǹ������ݵĹ��̡�������ò�Ҫ����
	// ����������������ʼ���Ĺ��̡�
	virtual bool OnReadData(AData* pData);
public:
	virtual bool IsDel();
	virtual bool IsDead();

	AUnit();
	virtual ~AUnit();
	virtual bool Init(AData *_pData,AGame * _pAGame);
	virtual bool Load(AArchive &ar,AGame * __pAGame);
	virtual bool Save(AArchive &ar);
	virtual int  OnMsg(PAMSG pMsg);
	virtual bool LogicRun(float __TimeSpan);
	virtual bool Release();
	virtual bool BindGame(AGame * m_pG);
	virtual bool BindFinished();	
	unsigned long& AccessTickCount() { return m_iTicks;}
	inline AGame* GetAGame() { return m_pAGame;}
};

#endif // __AIDOBJECT__H__