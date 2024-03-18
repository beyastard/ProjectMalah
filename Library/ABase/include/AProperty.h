/*
 * FILE: AProperty.h
 *
 * DESCRIPTION: 
 *	ʵ�����Զ����������ͽӿڶ���
 *		ARange;
 *		ASet;
 *		AProperty;
 *		ARangeTemplate;
 *		ASetTemplate;
 *		ASetElement;
 *		APropertyTemplate;
 *
 * CREATED BY: ����ӯ, 2002/8/7
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */


#ifndef _APROPERTY_H__
#define _APROPERTY_H__
#pragma warning ( disable : 4786 )
#include <assert.h>
#include "AVariant.h"
#include "AUString.h"
#include "stdarg.h"

// Ϊʵ�ִ���򻯶������
#define EXTERN_DEFINESET(type, name) extern ASetTemplate<type> name;
#define	DEFINE_SETBEGIN(type, name) ASetTemplate<type> name(
#define DEFINE_SETELEMENT(type, name, value) ASetTemplate<type>::ASetElement<type>(name, value),
#define DEFINE_SETEND(type) ASetTemplate<type>::ASetElement<type>());

// �༭���Դ����Ե��Ƽ���ʽ
enum 
{
	WAY_DEFAULT,		// ȱʡ��ʽ���ɱ༭���Զ����AVariant�����Ͳ�����
	WAY_BOOLEAN,		// ����������ʽ
	WAY_INTEGER,		// ���ʹ���ʽ
	WAY_FLOAT,			// ���㴦��ʽ
	WAY_STRING,			// �ִ�����ʽ
	WAY_FILENAME,		// �ļ���
	WAY_COLOR,			// ��ɫֵ
	WAY_VECTOR,			// ����ֵ
	WAY_BINARY,			// �����ƿ�
	WAY_OBJECT,			// ����
	WAY_UNITID,			// ����ID
	WAY_PATHID,			// ·��ID
	WAY_STRID,			// (�ĵ���)�ַ�����ԴID
	WAY_SFXID,			// (�ĵ���)��Ч��ԴID
	// ����ʽ˵��
	WAY_READONLY	= (1 << 31),// ����ֻ������
};

/////ARange�ӿ�
class ARange
{
public:

	virtual AVariant GetMaxValue() = 0;
	virtual AVariant GetMinValue() = 0;
	virtual bool IsInRange(AVariant value) = 0;
};

/////ASet�ӿ�
class ASet
{
public:

	virtual int GetCount() = 0;
	virtual AUString GetNameByIndex(int index) = 0;
	virtual AVariant GetValueByIndex(int index) = 0;
	virtual AVariant GetValueByName(AUString szName) = 0;
	virtual AUString GetNameByValue(AVariant value) = 0;
	virtual int FindValue(AVariant value) = 0;
	virtual int FindName(AUString szName) = 0;

};

/////AProperty�ӿ�
class AProperty
{
public:

	virtual const char * GetName() = 0;
	virtual const char * GetPath() = 0;
	virtual ASet * GetPropertySet() = 0;
	virtual ARange * GetPropertyRange() = 0;
	virtual int GetWay() = 0;
	virtual void SetWay(int way) = 0;
	virtual AVariant GetValue(void * pBase) = 0;
	virtual void SetValue(void * pBase, AVariant value) = 0;
	virtual ~AProperty() {};
};

/////ARangeTemplate��
template <class TYPE> class ARangeTemplate : public ARange
{
public:

	ARangeTemplate(TYPE min, TYPE max) :m_min(min), m_max(max) {}
	AVariant GetMaxValue() {return AVariant(m_max);}
	AVariant GetMinValue() {return AVariant(m_min);}

	bool IsInRange(AVariant value)
	{
		if(TYPE(value) < m_min && TYPE(value) > m_max)
		{
			return false;
		}

		return true;
	}

private:
	
	TYPE m_min;
	TYPE m_max;
};

/////ASetTemplate��
template <class TYPE> class ASetTemplate : public ASet
{
public:

	// �ڽ�Ԫ����
	template <class TYPE> class ASetElement
	{
	public:

		ASetElement()
		{
			m_szName.Empty();
		}

		ASetElement(AUString szName, TYPE data)
		{
			m_szName	= szName;
			m_data		= data;
		}

		ASetElement(ASetElement & e)
		{
			m_szName	= e.m_szName;
			m_data		= e.m_data;
		}

		AUString GetName() {return m_szName;}
		TYPE GetData() {return m_data;}

	private:

		AUString m_szName;
		TYPE		 m_data;
	};

protected:

	int					m_iCount;
	ASetElement<TYPE> * m_pElements;

public:

	// ���캯��
	ASetTemplate(ASetElement<TYPE> first, ...)
	{
		va_list elements, elements2;
		int iCount = 0;
		ASetElement<TYPE> element = first;
		va_start(elements, first);
		while(!element.GetName().IsEmpty())
		{
			iCount ++;
			element = va_arg(elements, ASetElement<TYPE>);
		}
		va_end(elements);

		assert(iCount > 0);
		m_pElements = new ASetElement<TYPE>[iCount];
		assert(m_pElements);

		int i = 0;
		element = first;
		va_start(elements2, first);
		while(!element.GetName().IsEmpty())
		{
			m_pElements[i] = element;
			element = va_arg(elements2, ASetElement<TYPE>);
			i ++;
		}
		va_end(elements2);
		m_iCount = iCount;
	}

	~ASetTemplate()
	{
		if(m_pElements)
		{
			delete []m_pElements;
		}
	}

	//Ԫ�ظ���
	int GetCount()
	{
		return m_iCount;
	}

	// ����������ȡ����
	AUString GetNameByIndex(int index)
	{
		assert(m_pElements);
		assert(index >= 0 && index < m_iCount);
		return m_pElements[index].GetName();
	}

	// ����������ȡֵ
	AVariant GetValueByIndex(int index)
	{
		assert(m_pElements);
		assert(index >= 0 && index < m_iCount);
		return AVariant(m_pElements[index].GetData());
	}

	// �������Ʋ�ֵ
	AVariant GetValueByName(AUString szName)
	{
		assert(m_pElements);
		assert(szName);

		for(int index = 0;index < m_iCount;index ++)
		{
			if(strcmp(szName, m_pElements[index].GetName()) == 0)
			{
				return AVariant(m_pElements[index].GetData());
			}
		}

		return AVariant();	//δ�ҵ�Ԫ�ص�����£�����һ��������������ΪAVT_INVALIDTYPE��
	}

	// ����ֵ������
	AUString GetNameByValue(AVariant value)
	{
		assert(m_pElements);

		for(int index = 0;index < m_iCount;index ++)
		{
			if(m_pElements[index].GetData() == (TYPE)value)
			{
				return m_pElements[index].GetName();
			}
		}

		return AUString();
	}

	// ��ֵ����
	int FindValue(AVariant value)
	{
		assert(m_pElements);
		for(int index = 0;index < m_iCount;index ++)
		{
			if(m_pElements[index].GetData() == (TYPE)value)
			{
				return index;
			}
		}

		return -1;
	}

	// ����������
	int FindName(AUString szName)
	{
		assert(m_pElements);
		for(int index = 0;index < m_iCount;index ++)
		{
			if(m_pElements[index].GetName() == szName)
			{
				return index;
			}
		}

		return -1;
	}
};

template <class TYPE> class APropertyTemplate : public AProperty  
{
public:
	
	APropertyTemplate(
		const char * szName,			// ������
		int iOffset,					// ������ƫ����
		ASet * pSet = NULL,				// ����
		ARange * pRange = NULL,			// ��Χ
		int way = WAY_DEFAULT,			// ���ԵĴ���ʽ
		const char * szPath = NULL)		// ·�����ļ����ӵ���Ѱλ�ã�ΪNULLʱ������ִ�г���ʼ��·�����£�
	{
		assert(szName);
		m_szName	= szName;
		m_iOffset	= iOffset;
		m_pRange	= pRange;
		m_pSet		= pSet;
		m_iWay		= way;
		if(szPath) m_szPath = szPath;
	}

	AVariant GetValue(void * pBase)
	{
		assert(pBase);
		char * pData = (char *)pBase;
		return AVariant(*(TYPE*)(pData + m_iOffset));
	}

	void SetValue(void * pBase, AVariant value)
	{
		assert(pBase);
		char * pData = (char *)pBase;
		*(TYPE*)(pData + m_iOffset) = (TYPE)value;
	}

	const char * GetName()
	{
		return m_szName;
	}

	const char * GetPath()
	{
		return m_szPath;
	}

	ASet * GetPropertySet()
	{
		return m_pSet;
	}

	ARange * GetPropertyRange()
	{
		return m_pRange;
	}

	int GetWay()
	{
		return m_iWay;
	}

	void SetWay(int way)
	{
		m_iWay = way;
	}

private:

	AUString			m_szName;		// ������
	AUString			m_szPath;		// ·��(������Ϊ�ļ�����ʱ�������ļ�����Ѱ λ��)
	int					m_iOffset;		// ����ƫ����
	int					m_iWay;			// ���ԵĴ���ʽ
	ARange *			m_pRange;		// ����ֵ�ķ�Χ
	ASet *				m_pSet;			// ����ֵ�ļ���
};
#endif
