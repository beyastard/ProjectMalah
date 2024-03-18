// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: A3DAssistCache.h
// Creator: Wei Hua (κ��)
// ͨ�û���

#ifndef	__A3DASSISTCACHE_H__
#define	__A3DASSISTCACHE_H__

#include "APlatform.h"
#include "AStringArray.h"
#include "ABinString.h"
#include <hashtab.h>

class	A3DASSISTCACHEUNIT
{
public:
	bool	m_bTaken;						// �õ�Ԫ�ѱ�ռ��
	int		m_nUseCount;					// �ܹ�ʹ�õĴ���
	int		m_nLastUsedTick;				// ׷��һ��ʹ�õ�ʱ��
	void	*m_pExtra;						// ��������
	int		m_nProperty;					// ��������
	inline int		Cmp(A3DASSISTCACHEUNIT &Unit)
	{
		int	rst;
		rst	= m_nLastUsedTick - Unit.m_nLastUsedTick;
		if( rst == 0 )
		{
			return	m_nUseCount - Unit.m_nUseCount;
		}
		return	rst;
	}
};

class	A3DAssistCache
{
protected:
	int		m_nTotal;
	A3DASSISTCACHEUNIT	*m_pUnits;
protected:
	virtual bool	StillInUse(int nIdx)		{return false;}	// ������ʵ�ֱ�ʾ�����Ԫ�Ƿ���ʹ����
public:
	static unsigned int	m_nCurTick;								// �����������д
public:
	A3DAssistCache();
	virtual	~A3DAssistCache();
	bool	Init(int nTotal);
	bool	Release();
	void	SetLastUsedTick(int nIdx);
	void	SetLastUsedTick(int nIdx, int nTick);
	void	Reset(int nIdx);
	void	ResetAll();
	int		GetMEUU(bool bForce = true);	// ���һ�����ʹ�õĵ�Ԫ����(Most Earlist Unused Unit)
											// ���߿���û�б�ʹ�õĵ�Ԫ����
};

// ���ַ��������Դ�Ļ���
////////////////////////////////////////////////////////////////////
// A3DAssistCache_StrNoDup
////////////////////////////////////////////////////////////////////
template <class _HashStrTab, class _StrArray>
class	A3DAssistCache_StrNoDup	: public A3DAssistCache
{
protected:
	_HashStrTab				* m_pTab;
	abase::pair<int *,bool>	m_srst;
	_StrArray				m_asa;
public:
	A3DAssistCache_StrNoDup() : m_srst(NULL, false)
	{
		m_pTab	= NULL;
	}
	~A3DAssistCache_StrNoDup()
	{
	}
	bool	Init(int nTotal, int nStrLen)
	{
		bool	bval;

		if( !m_asa.Init(nTotal, nStrLen) )
		{
			return	false;
		}
		
		bval	= A3DAssistCache::Init(nTotal);
		if( !bval )
		{
			return	bval;
		}
		m_pTab	= new _HashStrTab(nTotal);
		if( !m_pTab )
		{
			return	false;
		}

		return	true;
	}
	bool	Release()
	{
		m_asa.Release();

		if( m_pTab )
		{
			delete	m_pTab;
			m_pTab	= NULL;
		}
		A3DAssistCache::Release();

		return	true;
	}
	void	Reset(int nIdx)
	{
		bool	bval;
		if( m_pUnits[nIdx].m_pExtra )
		{
			bval	= m_pTab->erase((char *)m_pUnits[nIdx].m_pExtra);
		}
		A3DAssistCache::Reset(nIdx);
	}
	void	ResetAll()
	{
		m_pTab->clear();
		A3DAssistCache::ResetAll();
	}
	// ����pszStr��Ӧ�����������û�з���-1��Ȼ���ϲ����GetMEUU�õ�һ������ʹ�õ�������
	int		SearchString(char *pszStr)
	{
		m_srst	= m_pTab->get(pszStr);
		if( m_srst.second )
		{
			return	*m_srst.first;
		}
		return	-1;
	}
	// ���ԭ��nIdx���ж�����Ҫ�������ص����ж���(�磺hash�е��ִ�)
	int		RegisterString(int nIdx, char *pszStr)
	{
		if( nIdx<0 )
		{
			return	-1;
		}
		if( m_pUnits[nIdx].m_bTaken )
		{
			Reset(nIdx);
		}
		if( !m_asa.StrCpy(nIdx, pszStr) )
		{
			return	-1;
		}
		if( !m_pTab->put(m_asa.GetUnit(nIdx), nIdx) )
		{
			return	-1;
		}
		m_pUnits[nIdx].m_pExtra	= m_asa.GetUnit(nIdx);
		m_pUnits[nIdx].m_bTaken	= true;

		return	0;
	}
};
// ��ͨ�ִ�
struct HASHSTR
{
	typedef const char * LPCSTR;
	const char * _reference;
	HASHSTR()						{}
	HASHSTR(const char * _ref)		{ _reference = _ref; }
	HASHSTR(char * _ref)			{ _reference = _ref; }
	inline operator LPCSTR() const	{ return _reference; }
	inline bool operator ==(const HASHSTR &rhs) const
	{ return (!strcmp(_reference, rhs)); }
};
typedef	abase::hashtab<int, HASHSTR, abase::_hash_function>	AAC_StrTab;
// �����������ݵ��ִ�
struct HASHBINSTR
{
	typedef const char * LPCSTR;
	const char * _reference;
	HASHBINSTR()					{}
	HASHBINSTR(const char * _ref)	{ _reference = _ref; }
	HASHBINSTR(char * _ref)			{ _reference = _ref; }
	inline operator LPCSTR() const	{ return _reference; }
	inline bool operator ==(const HASHBINSTR &rhs) const
	{
		return !BinStrCmp(_reference, rhs._reference);
	}
};
struct _binstr_hash_function
{
	_binstr_hash_function() {}
	_binstr_hash_function(const _binstr_hash_function & hf)	{}
	inline unsigned long operator()(const char *s) const
	{
		return	BinStrHash(s);
	}
};
typedef	abase::hashtab<int, HASHBINSTR, _binstr_hash_function>	AAC_BinStrTab;
// ������Ӧ����
typedef		A3DAssistCache_StrNoDup<AAC_StrTab, AStringArray>		A3DAssistCache_String;
typedef		A3DAssistCache_StrNoDup<AAC_BinStrTab, ABinStringArray>	A3DAssistCache_BinString;

////////////////////////////////////////////////////////////////////
// A3DAssistCache_StrDup
////////////////////////////////////////////////////////////////////
class	A3DAssistCache_StrDup		: public A3DAssistCache
{
protected:
	AStringArray	m_asa;
protected:
	// ����true��ʾ�Ѿ����˱���ʼ���ĵ�λ��*pnMEUU�з��ص��ǵ�λ�����
	// ����false��ʾ��Ҫ����*pnMEUU���³�ʼ����λ��ʹ��
	bool	SearchAndAct(char *szStr, int *pnMEUU, bool bFindOld);	// �������StillInUse�������ҵ�һ���������³�ʼ���ĵ�λ����
	void	Reset(int nIdx);
	// �����ж�Ĭ��Ϊֻ����MEUU����ʹ��˳��
	virtual bool	StillInUse(int nIdx)		{return true;}
public:
	A3DAssistCache_StrDup();
	~A3DAssistCache_StrDup();
	bool	Init(int nTotal, int nStrLen);
	bool	Release();
	bool	ResetAll();
};
#endif