// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: A3DAssistTools.h
// Creator: Wei Hua (κ��)
// ���ڼ�Ӧ�õĹ��߼����������ʼ���󴴽��ö���

#ifndef	__A3DASSISTTOOLS_H__
#define	__A3DASSISTTOOLS_H__

////////////////////////////////////////////////////////////////////
// ���ܺ���
////////////////////////////////////////////////////////////////////
// ��������ͨ�ִ���html��ʽ�ִ���ת��
int	CmnToHtml(char *__html, int __size, char *__cmn);
int	HtmlToCmn(char *__cmn, int __size, char *__html);
// ���õ�ǰʱ�䣬����ȷ��һ�����嵥Ԫ�����ʹ��ʱ��
// ֻ��Ҫ��LogicRun���Լ������Լ���Tick�Ϳ�����
// ���������ȡ����
// void	SetCurTickToA3DAssistTools(int nTick);

////////////////////////////////////////////////////////////////////
// A3DAssistTools�ӿڶ���
////////////////////////////////////////////////////////////////////
#include <A3DTypes.h>
class	A3DEngine;

#define	AATS_HTMLSTR_MAX_LEN				1024

// ����ע����ͨ����
typedef struct	__A3DFONT_INFO_T
{
	char	*pszName;
	int		nHeight;
	int		nFlag;
	int		nBufNum;							// A3DString�Ļ������
	int		nStrLen;							// ����ִ�����
	int		nSetBufNum;							// A3DStringSet�Ļ������
	int		nSetStrLen;							// ����ִ�����
}A3DFONT_INFO_T;
// ����ע��LCDͼƬ����
typedef struct	__A3DLCD_INFO_T
{
	char	*pszChars;							// ��������ַ��б�
	char	*pszCharGraphFile;					// ������ַ���Ӧ��һ�еȿ�Сͼ�ļ�
	int		nRow;								// ͼ������
	int		nCol;								// ͼ������
	int		nUnitW;								// ÿ��Сͼ��
	int		nUnitH;								// ÿ��Сͼ��
}A3DLCD_INFO_T;

class	A3DAssistTools
{
public:
	virtual			~A3DAssistTools()										{;}
	virtual void	SetSystemTickcount(unsigned long nT)					= 0;	// ����ϵͳ��ǰ��ʱ�䣨���룩������������fade ��Ӧ�������߼�ÿ�λ��ʱ���ʱ���������
	virtual void	AddSystemTickcount(unsigned long nT)					= 0;	// �ۼӵ�ǰ��ʱ���������룩������������fade ��Ӧ�������߼�ÿ�λ��ʱ���ʱ���������
	virtual bool	LogicRun()												= 0;	// ������Ƶ�һЩ������ҪTick
	// ����
	virtual bool	ResetResource()											= 0;	// ����������Դ��һ����һ����Ϸ�׶ν���ʱ��
	// �������
	virtual bool	F_RegisterFont(int nFont, A3DFONT_INFO_T *pInfo)		= 0;	// ע�ᡣ�ڳ���ʼǰȷ���ܹ���Ҫ��Щ���壬Ȼ��һ��ȫ��ע�ᡣ
	virtual bool	F_RegisterColor(int nColor, A3DCOLOR acolor)			= 0;	// ע��html�л�ʹ�õ�����ɫ��
	virtual bool	F_SetCurFont(int nFont, int nNewSize = 0)				= 0;// ����������Ҫʹ�õ�2D���塣
	virtual	int		F_GetCurFont()											= 0;
	virtual int		F_GetFont(int nFont)									= 0;
	virtual bool	F_SetCur3DFont(int nFont, float fRatio = 0)				= 0;	// ����������Ҫʹ�õ�3D���塣
	virtual bool	F_GetTextExtent(char *szText, int *pnW, int *pnH)		= 0;	// ��õ�ǰ�����£�ָ���ִ�����Ļ����ߴ�
	virtual bool	F_TextOut(int nX, int nY, char *szText, A3DCOLOR color, bool bShade)= 0;	// ����Ļָ��λ�����ָ����ɫ���ִ�
	virtual bool	F_HtmlTextOut(int nX, int nY, char *szText, A3DCOLOR color, bool bShade)=0;	// ����ִ��п���������html��ʽ�ı�ĳһ�ε�������ɫ
	virtual bool	F_TextOut3D(A3DVECTOR3 vecPos, char *szText, A3DCOLOR color)		= 0;	// ����ά�����һ��λ������ִ�
	//		���б���ַ������
	//		nX, nY����ʼ����
	virtual bool	F_TextOut_Begin()										= 0;
	//		nDX����������
	virtual bool	F_TextOut_AddString(int nDX, A3DCOLOR color, char *szText)			= 0;
	//		bShade��ʾ�Ƿ���Ӱ�ӱ�
	virtual bool	F_TextOut_End(int nX, int nY, bool bShade)				= 0;
	// LCD�ַ����
	virtual bool	LCD_RegisterFont(int nLCDFont, A3DLCD_INFO_T *pInfo)	= 0;	// ע�ᡣ����ע�����LCD���塣
	virtual bool	LCD_GetTextExtent(char *szText, int *nW, int *nH)		= 0;	// ���������LCD�ִ������Ļ����ߴ�
	virtual bool	LCD_SetCurFont(int nLCDFont, float fRatio = 0)			= 0;	// ����������Ҫʹ�õ�LCD���塣
	virtual bool	LCD_SetSep(int nSep)									= 0;	// �����ּ��
	virtual bool	LCD_TextOut(int nX, int nY, char *szText, A3DCOLOR color)			= 0;	// ��ָ��λ�����ָ����ɫ��LCD���ӵ��ִ�
	// �������
	virtual bool	S_SndInit(int * n2DBufTypeBuf,int n2DBufTypeCount, int n2DDupBuf,
							  int* n3DBufTypeBuf, int n3DBufTypeCount, int nFNLen)		= 0;	// ��ʼ�����������nFNLen���ļ����Դ󳤶ȣ�
	virtual void	S_SetJustLoad(bool bSet)								= 0;	// ��������ֻload����
	virtual bool	S_2DSndPlay(int type,char *szSndFile, bool bImmEffect, bool bLoop, bool bCanFade)	= 0;
	virtual bool	S_2DSndStop(int type,char *szSndFile)							= 0;
	virtual bool	S_2DSndPause(int type,char *szSndFile)							= 0;
	virtual bool	S_2DSndIsStopped(int type,char *szSndFile)						= 0;
	virtual bool	S_2DDupSndPlay(char *szSndFile, bool bImmEffect, bool bCanFade)		= 0;	// ����һ�������ж���
	virtual bool	S_3DSndPlay(int iType, char *szSndFile, A3DVECTOR3 &vecPos, float fMinDist, float fMaxDist
					, bool bImmEffect, bool bForce2D, bool bCanFade)		= 0;
	virtual void	S_SetVolume(unsigned int nVol)							= 0;	// ����
	virtual void	S_2DSndFadeIn(int type,int ms)									= 0;	// ����֮��ms�����ڵ�������Ҫfade��2D��������fadein
	virtual void	S_2DSndFadeOut(int type)										= 0;	// ��ǰ���ڲ��ŵ�������Ҫfade��2D��������fadeout
	virtual void	S_3DSndFadeIn(int iType, int ms)								= 0;	// ����֮��ms�����ڵ�������Ҫfade��2D��������fadein
	virtual void	S_3DSndFadeOut(int iType)										= 0;	// ��ǰ���ڲ��ŵ�������Ҫfade��2D��������fadeout
	// ��������
	virtual bool	BM_Play(char *szSndFile)								= 0;
	virtual bool	BM_Stop()												= 0;
	virtual bool	BM_Pause(bool bPause)									= 0;
	virtual bool	BM_IsStopped()											= 0;
	virtual void	BM_SetLoop(bool bLoop)									= 0;
	virtual bool	BM_GetLoop()											= 0;
	virtual void	BM_SetVolume(unsigned int nVol)							= 0;	// ����
	virtual unsigned int	BM_GetVolume()									= 0;
	virtual bool	BM_FadeOut(float fOT, char *szNextFile = NULL, float fIT = 0)	= 0;
	virtual bool	BM_FadeIn(char *szNextFile, float fIT)					= 0;
																					// ������ǰ���֣�Ȼ������һ������
};

// ��NewA3DAssistTools�����ڳ�ʼ���ö���
typedef struct	__A3DASSISTTOOLS_INFO_T
{
	A3DEngine	*pA3DEngine;
}A3DASSISTTOOLS_INFO_T;
A3DAssistTools *	NewA3DAssistTools(A3DASSISTTOOLS_INFO_T *pInfo);

#endif