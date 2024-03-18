/*
 * FILE: AVariant.h
 *
 * DESCRIPTION: ʵ������������AVariant
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY: By JiangLi��ʹ��ģ�幹�캯���Լ�ģ������ת��������Ӧ���ǲ����еġ�
 *               ������¶�������ز��֡�
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */


#ifndef _AVARIANT_H__
#define _AVARIANT_H__

#include "windows.h"
#include "a3dtypes.h"
#include "aobject.h"
#include "abinary.h"
#include "austring.h"
#include <assert.h>

#define DEFINEOPERATOR(type) 	operator type() {assert(m_iType != AVT_INVALIDTYPE);return *(type *)(&m_avData);}

class ASet;

class AVariant  
{
public:

	enum AVARTYPE
	{
		AVT_INVALIDTYPE,	// �Ƿ����ͣ�����Ĭ�Ϲ���,��ʱ�κ����ݳ�Ա��������
		AVT_BOOL,			// ������				m_avData.bV;
		AVT_CHAR,			// �ַ�					m_avData.cV;	
		AVT_UCHAR,			// �޷����ַ�			m_avData.ucV;	
		AVT_SHORT,			// ������				m_avData.sV;	
		AVT_USHORT,			// �޷��Ŷ�����			m_avData.usV;	
		AVT_INT,			// ����					m_avData.iV;	
		AVT_UINT,			// �޷�������			m_avData.uiV;	
		AVT_LONG,			// ������				m_avData.lV;	
		AVT_ULONG,			// �޷��ų�����			m_avData.ulV;	
		AVT_INT64,			// 64Bit����			m_avData.i64V;	
		AVT_UINT64,			// �޷���64Bit����		m_avData.ui64V;
		AVT_FLOAT,			// ������				m_avData.fV;	
		AVT_DOUBLE,			// ˫���ȸ�����			m_avData.dV;	
		AVT_POBJECT,		// ����ָ��				m_avData.paV;
		AVT_A3DVECTOR3,		// ����					m_avData.a3dvV;
		AVT_A3DMATRIX4,		// ����					m_avData.pa3dmV;
		AVT_STRING,			// �ַ���				m_avData.pstrV;
		AVT_PABINARY,		// �����ƿ�				m_avData.binV;
		AVT_PACLASSINFO,	// �മ�л���Ϣָ��		m_avData.paciV;
		AVT_PASET,			// ����ָ��				m_avData.pasetV;
	};


public:

	AVariant();
	AVariant(const AVariant & v);
	AVariant(bool bV);
	AVariant(char cV);
	AVariant(unsigned char ucV);
	AVariant(short sV);
	AVariant(unsigned short usV);
	AVariant(int iV);
	AVariant(unsigned int uiV);
	AVariant(long lV);
	AVariant(unsigned long ulV);
	AVariant(__int64 i64V);
	AVariant(unsigned __int64 ui64V);
	AVariant(float fV);
	AVariant(double dV);
	AVariant(AObject & pObject);
	AVariant(A3DVECTOR3 avec3);
	AVariant(A3DMATRIX4 amat4);
	AVariant(AUString & str);
	AVariant(const ABinary & binV);
	AVariant(AClassInfo * paciV);
	AVariant(ASet * pSet);

	int GetType() const {return m_iType;}

	DEFINEOPERATOR(bool)
	DEFINEOPERATOR(char)
	DEFINEOPERATOR(unsigned char)
	DEFINEOPERATOR(short)
	DEFINEOPERATOR(unsigned short)
	DEFINEOPERATOR(int)
	DEFINEOPERATOR(unsigned int)
	DEFINEOPERATOR(long)
	DEFINEOPERATOR(unsigned long)
	DEFINEOPERATOR(__int64)
	DEFINEOPERATOR(unsigned __int64)

	// ��float��ת�����������
	operator float()
	{
		assert(m_iType != AVT_INVALIDTYPE);
		if(m_iType == AVT_DOUBLE)
			return (float)m_avData.dV;
		if(m_iType == AVT_FLOAT)
			return m_avData.fV;
		return (float)(*(__int64 *)(&m_avData));
	}

	// ��double��ת�����������
	operator double()
	{
		assert(m_iType != AVT_INVALIDTYPE);
		if(m_iType == AVT_DOUBLE)
			return m_avData.dV;
		if(m_iType == AVT_FLOAT)
			return (double)m_avData.fV;
		return (double)(*(__int64 *)(&m_avData));
	}

	DEFINEOPERATOR(AObject *)
	DEFINEOPERATOR(A3DVECTOR3)
	DEFINEOPERATOR(A3DMATRIX4)
	operator AUString() {assert(m_iType == AVT_STRING);return *(m_avData.pstrV);}
	operator ABinary() {assert(m_iType != AVT_INVALIDTYPE);return *(m_avData.pbinV);}
	DEFINEOPERATOR(AClassInfo *)
	DEFINEOPERATOR(ASet *)

private:

	union AVARIANT
	{
		bool					bV;			// ������
		char					cV;			// �ַ�
		unsigned char			ucV;		// �޷����ַ�
		short					sV;			// ������
		unsigned short			usV;		// �޷��Ŷ�����
		int						iV;			// ����
		unsigned int			uiV;		// �޷�������
		long					lV;			// ������
		unsigned long			ulV;		// �޷��ų�����
		__int64					i64V;		// 64Bit����
		unsigned __int64		ui64V;		// �޷���64Bit����
		float					fV;			// ������
		double					dV;			// ˫���ȸ�����
		AObject *				paV;		// ����ָ��
		char					a3dvV[sizeof(A3DVECTOR3)];	// ����������
		char					a3dmV[sizeof(A3DMATRIX4)];	// ����������
		AUString *				pstrV;		// �ַ��� 
		const ABinary *			pbinV;		// �����ƿ����ָ��
		AClassInfo *			paciV;		// ���л���Ϣָ��
		ASet *					pasetV;		// ����ָ��
	} m_avData;

	int m_iType;	// ��������
};

#endif
