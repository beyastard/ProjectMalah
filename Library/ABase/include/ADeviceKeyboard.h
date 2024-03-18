// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: ADeviceKeyboard.h
// Creator: Wei Hua (κ��)
// �����豸

#ifndef	___ADEVICEKEYBOARD_H__
#define	___ADEVICEKEYBOARD_H__

#include "ADevice.h"

#define	MYVK_UNDO				0x1A
#define	MYVK_REDO				0x19

class	ADeviceKeyboard		: public ADevice1vs1, public AWinMsgDevice
{
public:
	// ~ ���ڸ��������������������������ֻҪʵ����������ͻᱻ����
	// ����ָ��������İ���״̬
	virtual bool	VK_FirstPress(int nVK)					= 0;
	virtual bool	VK_LastPress(int nVK)					= 0;
	virtual bool	VK_Pressing(int nVK)					= 0;
	// ���WM_CHAR�Ŀɼ��ִ�(�����ִ�����)
	virtual int		GetChars(char *pszChars, int nSize)		= 0;
	// ���WM_KEYDOWN�Ĺ��ܼ��ִ�(�����ִ�����)
	virtual int		GetKeys(char *pszChars, int nSize)		= 0;
};
ADeviceKeyboard	* NewADeviceKeyboard();

#endif