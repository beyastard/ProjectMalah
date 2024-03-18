// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: ADevice.h
// Creator: Wei Hua (κ��)
// �豸����

#ifndef	__ADEVICE_H__
#define	__ADEVICE_H__

#include "AUList.h"

class	ATrans;
class	AUListS;

////////////////////////////////////////////////////////////////////
// �����豸
////////////////////////////////////////////////////////////////////
class	ADevice
{
private:
	bool			m_bActive;
public:
	ADevice()
	{
		m_bActive	= true;
	}
	inline void	Activate(bool bSet)
	{ m_bActive = bSet; }
	inline bool	IsActivate()
	{ return m_bActive; }
	virtual			~ADevice()				{;}
	virtual bool	TranslateIn()			= 0;
	virtual bool	TranslateOut()			= 0;
	virtual bool	UpdateIn()				= 0;
	virtual bool	UpdateOut()				= 0;
};

class	ADevice1vs1	: public ADevice
{
protected:
	ATrans			* m_pTrans;				// �����ķ�����
public:
	ADevice1vs1();
	virtual			~ADevice1vs1();
	void	SetTrans(ATrans *pTrans);
	virtual bool	TranslateIn();
	virtual bool	TranslateOut();
};

class	ADevice1vsN	: public ADevice
{
protected:
	AListT<ATrans *>	*m_pTransList;			// �����ķ������б�
public:
	ADevice1vsN();
	virtual			~ADevice1vsN();
	bool	AddTrans(ATrans *pTrans);
	bool	RemoveTrans(ATrans *pTrans);
	virtual bool	TranslateIn();
	virtual bool	TranslateOut();
};

////////////////////////////////////////////////////////////////////
// �����豸�ӿ�
////////////////////////////////////////////////////////////////////
class	AWinMsgDevice						// ��Windows��Ϣ�������豸
{
public:
	virtual bool	FilterWinMsg(void *pWinMsg)	= 0;
};

#endif