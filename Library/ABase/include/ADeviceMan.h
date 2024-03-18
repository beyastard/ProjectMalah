// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: ADeviceMan.h
// Creator: Wei Hua (κ��)
// �豸������

#ifndef	__ADEVICEMAN_H__
#define	__ADEVICEMAN_H__
#include "Aulist.h"

class	ADevice;
class	AWinMsgDevice;

class	ADeviceMan
{
protected:
	AListT<ADevice *>	* m_pDeviceList;						// ������豸�б�
public:
	ADeviceMan();
	~ADeviceMan();
	bool	AddDevice(ADevice *pADevice);			// �����豸��AGameFrame������ɾ����Ҳ����ֱ���ǳ�Ա��������Ϊһ�㲻�ö�̬���ɣ���
	bool	RemoveDevice(ADevice *pADevice);
	bool	RemoveAllDevice();
	bool	DealIn();								// �ֳ�In/Outд��ԭ���ǳ������������ȼ�����롢�����߼���Ȼ�����
	bool	DealOut();
};

class	AWinMsgDeviceGrp
{
protected:
	AListT<AWinMsgDevice *>	* m_pDeviceList;						// ������豸�б�
public:
	AWinMsgDeviceGrp();
	~AWinMsgDeviceGrp();
	bool	AddDevice(AWinMsgDevice *pAWMDevice);
	bool	RemoveDevice(AWinMsgDevice *pAWMDevice);
	bool	RemoveAllDevice();
	bool	FilterWinMsg(void *pWinMsg);
};

#endif