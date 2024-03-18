/*
 * FILE: AData.h
 *
 * DESCRIPTION: A class which is the base of all the data objects;
 *
 * CREATED BY: JiangLi, 2002/08/08
 *             ����ӯ 2002/8/20 properies manager & analyze
 *
 * HISTORY: 
 *
 * Copyright (c) 2001~2002 Epie-Game, All Rights Reserved.	
 */

#ifndef	__ADATA_H__
#define	__ADATA_H__

#include "AIDObject.h"
#include "AProperty.h"
#include "AUString.h"
#include "AUList.h"
#include "stddef.h"

// AObject -> AIDObject -> class AData;

// ��ʼ����̬���Լ��ϣ��������ڹ��캯���г��֣�
#define INITIALIZE_STATICPROPERIES(classname) 

// ���徲̬���Լ�����ʼ�㣬������������������
#define STATICPROPERIES_BEGIN(base, classname) \
	virtual int ProperiesCount() \
	{ \
		int count = 0;\
		classname::Properies(&count, NULL, 0);\
		return base::ProperiesCount() + count;\
	}\
	virtual AProperty * Properies(int index)\
	{\
		AProperty * pProp = base::Properies(index);\
		if(pProp) return pProp;\
		index -= base::ProperiesCount();\
		return classname::Properies(NULL, NULL, index);\
	}\
	virtual AProperty * Properies(AUString name)\
	{\
		AProperty * pProp = base::Properies(name);\
		if(pProp) return pProp;\
		return classname::Properies(NULL, name, 0);\
	}\
	struct _staticproperies\
	{\
		AProperty * properies(int * pCount, const char * pName, int index)\
		{\
			static AProperty * pProps[] = \
			{

// ���徲̬�����ս�㣬����������
#define STATICPROPERIES_END(classname) \
			};\
			static int count = sizeof(pProps) / sizeof(AProperty *);\
			if(pCount) {*pCount = count;return NULL;}\
			if(pName)\
			{\
				for(int a = 0;a < count;a ++)\
					if(strcmp(pProps[a]->GetName(), pName) == 0) \
						return pProps[a];\
				return NULL;\
			}\
			if(index >= 0 && index < count) return pProps[index];\
			return NULL;\
		}\
		~_staticproperies()\
		{\
			int count = 0;\
			properies(&count, NULL, 0);\
			for(int a = 0;a < count;a ++)\
			{\
				AProperty * pProp = properies(NULL, NULL, a);\
				delete pProp;\
			}\
		}\
	};\
	friend struct _staticproperies;\
	static AProperty * Properies(int * pCount, const char * pName, int index)\
	{\
		static classname::_staticproperies properies;\
		return properies.properies(pCount, pName, index);\
	}

// ����������Ŀ�����������������������ͣ�����ָ�룬����ֵ���ϣ�����ֵ��Χ�����ԵĴ���ʽ���ļ�������Ѱλ�ã��������ֵΪ�ļ����ӵĻ���
#define STATICPROPERTY(classname, name, prop_type, prop_data, prop_set, prop_range, prop_way, prop_path) \
			new APropertyTemplate<prop_type>(name, offsetof(classname, prop_data), prop_set, prop_range, prop_way, prop_path),


/*
  ���͵����Զ��������֣�

  ������
  STATICPROPERTY("property", int, m_int_value, NULL, NULL, WAY_DEFAULT, NULL)

  �����͵�ָ������ģʽ
  STATICPROPERTY("property", A3DCOLOR, m_a3c_value, NULL, NULL, WAY_COLOR, NULL)

  �ļ�����
  STATICPROPERTY("property", AUString, m_str_skytexture, NULL, NULL, WAY_FILENAME, "texture\\sky")

  ���ڴ���ʽ�Ķ��壬���£�
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
*/

class AData : public AIDObject
{
public:

	DECLARE_ASERIAL(AData)

public:

	// collection resources;
	virtual void CollectionResourceRec(AListT<AUString> & resources);

	void SetName(AUString name);
	AUString GetName();

	virtual bool Save(AArchive &ar);
	virtual bool Load(AArchive &ar);

	// ���Ը���
	int GetPropertyCount()
	{
		return ProperiesCount();
	}

	// ��������
	AProperty * GetProperty(int index)			// ʹ����Ż������
	{
		return Properies(index);
	}

	AProperty * GetProperty(AUString name)		// ʹ�����ƻ������
	{
		return Properies(name);
	}

	AData();
	virtual ~AData();

protected:

	AUString				m_str_name;			// ��������

	virtual int ProperiesCount()
	{
		int count = 0;
		AData::Properies(&count, NULL, 0);
		return /* AObject::ProperiesCount() + */ count;
	}

	virtual AProperty * Properies(int index)
	{
		return AData::Properies(NULL, NULL, index);
	}

	virtual AProperty * Properies(AUString name)
	{
		return AData::Properies(NULL, name, 0);
	}

	struct _staticproperies
	{
		AProperty * properies(int * pCount, const char * pName, int index)
		{
			static AProperty * pProps[] = 
			{
				new APropertyTemplate<AUString>("����", offsetof(AData, m_str_name), NULL, NULL, WAY_DEFAULT, NULL),
			};

			static int count = sizeof(pProps) / sizeof(AProperty *);
			if(pCount) {*pCount = count;return NULL;}
			if(pName)
			{
				for(int a = 0;a < count;a ++)
					if(strcmp(pProps[a]->GetName(), pName) == 0) 
						return pProps[a];
				return NULL;
			}
			if(index >= 0 && index < count) return pProps[index];
			return NULL;
		}

		~_staticproperies()
		{
			int count = 0;
			properies(&count, NULL, 0);
			for(int a = 0;a < count;a ++)
			{
				AProperty * pProp = properies(NULL, NULL, a);
				delete pProp;
			}
		}
	};
	friend struct _staticproperies;
	static AProperty * Properies(int * pCount, const char * pName, int index)
	{
		static AData::_staticproperies properies;
		return properies.properies(pCount, pName, index);
	}
};

#endif