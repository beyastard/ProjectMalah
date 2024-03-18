// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: ADeviceMouse.h
// Creator: Wei Hua (κ��)
// ����豸

#ifndef	__ADEVICEMOUSE_H__
#define	__ADEVICEMOUSE_H__

#include "ADevice.h"

#define	ADMOUSE_VK_LBUTTON		1
#define	ADMOUSE_VK_RBUTTON		2
#define	ADMOUSE_VK_MBUTTON		3

class	ADeviceMouse		: public ADevice1vs1, public AWinMsgDevice
{
public:
	// ~ ���ڸ��������������������������ֻҪʵ����������ͻᱻ����
	// ����ָ��������İ���״̬
	virtual bool	VK_FirstPress(int nVK)					= 0;
	virtual bool	VK_LastPress(int nVK)					= 0;
	virtual bool	VK_Pressing(int nVK)					= 0;
	virtual void	GetCursorPos(int *pnX, int *pnY)		= 0;
	virtual void	GetCursorDeltaPos(int *pnDX, int *pnDY)	= 0;
	virtual void	SetCursorRange(int nL, int nT, int nR, int nB)	= 0;
	virtual int		GetWheelRoll()							= 0;
};
#include "APlatform.h"
typedef struct	__ADEVICEMOUSE_INFO_T
{
	HWND		hWnd;
	HINSTANCE	hInst;
	bool		bWindow;			// ����ģʽ�����ÿ��Ƽ���
	// CursorRange
	int			nLeft;
	int			nTop;
	int			nRight;
	int			nBottom;
}ADEVICEMOUSE_INFO_T;
ADeviceMouse	* NewADeviceMouse(ADEVICEMOUSE_INFO_T *pInfo);

#endif