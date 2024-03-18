/*
 * FILE: AModelUnitTypes.h
 *
 * DESCRIPTION: ����������AModelUnit�����Ͷ�����������һ��Ψһ������
 *          ��־��ÿ���һ���µ�����Ҳ��Ҫ�������ö�����������һ��
 *          �µ�ö�ٳ�����
 *
 * CREATED BY: JiangLi, 2002-8-15
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2002 Epie-Game, All Rights Reserved.
 */

#ifndef	__AMODELUNITTYPES_H__
#define	__AMODELUNITTYPES_H__

enum AMODELUNIT_TYPE
{
	// AMatter ���ࣺBy JiangLi
	// ͨ�����ö�����п����ò��ϣ�д����������������Ρ�
	AMATTER_CANBESHOTED,	// ����һ��ͨ���ָ࣬ͨ���Ա��������Ʒ��
		AMATTER_CHEST,		// ���ӣ����Ա����顣������AMATTER_CANBESHOTED
	AMATTER_CANBEPICKED,	// ����һ��ͨ���ָ࣬ͨ���Ա�ʰȡ����Ʒ��
		AMATTER_WEAPON,		// ����������ǹе�����񵯣��ӵ��������µȡ�
		AMATTER_MEDICINE,	// ҩƷ�����������ָ����ⶾ��������ж����ȡ�
};

#endif	// __AMODELUNITTYPES_H__