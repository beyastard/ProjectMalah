/*
 * FILE: AMatter.h
 *
 * DESCRIPTION: AMatter
 *
 * CREATED BY: JiangLi, 2002/08/14
 *
 * HISTORY: 
 *
 * Copyright (c) 2001~2002 Epie-Game, All Rights Reserved.	
 */

#ifndef	__AMATTER_H__
#define	__AMATTER_H__
/*
#include "AModelUnit.h"
#include "AMatterData.h"

// AObject
	// AIDObject
		// AUnit
			// AVisualUnit
				// AModelUnit
					class AMatter;
						class AMatterCanBeShoted;
						class AMatterCanBePicked;

class AMatter : public AModelUnit
{
public:
	DECLARE_ASERIAL(AMatter)
public:
	AMatter();
	virtual ~AMatter();

protected:
	AMATTER_KERNEL_DATUM	m_matter_dat;
protected:

	virtual AModelUnitData* CreateData();
	virtual bool OnWriteData(AModelUnitData* pData);
	virtual bool OnReadData(AModelUnitData* pData);
	
public:
	virtual bool OnShot(AModelUnit *pShotInfo);
	virtual bool Release();
	virtual bool Render(A3DViewport * pCurrentViewport);
	virtual int  OnMsg(PAMSG pMsg);
};

// �ܹ������е���Ʒ���������Ժ󲥷Ŷ�������������
// ����Ʒ��������Ʒ����һ���ļ��ʣ��ܹ��Ӷ����ѡ
// ��Ʒ�����ѡȡ���������Ժ�Ӧ�û���һ���ķ�����
// �����п�����Ҫ������Ϣ������ߡ�
class AMatterCanBeShoted : public AMatter
{
public:
	DECLARE_ASERIAL(AMatterCanBeShoted)

protected:

	// �������Ժ��������Ʒ
	class AShotGenerate
	{
	public:
		//AMODELUNIT_TYPE	m_Type;		// ��Ʒ����
		int				m_nProb;	// ��������
		int				m_nDelay;	// ������Ʒ���ӳ�
		///INITMODELUNIT	m_Init;		// ��ʼ����
		union	// ��ͬ������Ʒ�ĳ�ʼ������
		{
			int		m_nReserve;
		};
	};

	AShotGenerate*	m_pShotGen;
	UINT			m_uCountGen;

public:
	AMatterCanBeShoted();
	virtual ~AMatterCanBeShoted();

	virtual bool Release();
	virtual bool OnShot(AModelUnit *pShotInfo);
};

// �ܹ���ʰȡ����Ʒ��ʰȡ�Ժ������Ҫ����������
// ��ʰȡ�Ժ󣬰���Ʒ����͸�ShotInfo�Ĺ�������
class AMatterCanBePicked : public AMatter
{
public:
	DECLARE_ASERIAL(AMatterCanBePicked)
public:
	AMatterCanBePicked();
	virtual ~AMatterCanBePicked();

	virtual bool Release();
	virtual bool OnShot(AModelUnit *pShotInfo);
};
*/
#endif	// __AMATTER_H__