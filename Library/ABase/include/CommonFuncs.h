// File		: CommonFuncs.h
// Creator	: Wei Hua
// ��Ҷ������õ�һЩͨ�ú���

#ifndef	__COMMONFUNCS_H__
#define	__COMMONFUNCS_H__

#include "A3DTypes.h"
#include "A3DFuncs.h"
// ��������
// ��ֵ
float	_GetInterpolation(float f, float f1, float f2, float v1, float v2);
// �ж�һ�����Ƿ�������
bool	_HaveChineseInStr(char *__str);
float	_GetHDistance(A3DVECTOR3 Pos1, A3DVECTOR3 Pos2, float fMaxVLen);
int		_StrTrimC(char *__str, char __c);
float	_RandBetweenFloat(float __r1, float __r2);
int		_RandBetweenInt(int __r1, int __r2);		// ���__r2 > __r1 , �����Χ�� __r1 �� __r2-1
bool	_IsPathExist(const char *__path, bool __searchpck = true);
int		_GetFileLen(const char *__file);
// ����CDROM��������Ŀ��__drives����CDROM�������̷���ɵ��ִ�
int		_GetLogicalCDROM(char *__drives, int __len);
// �ж���Щ�ļ��Ƿ��ڹ�����
bool	_HaveTheseFilesOnCDROM(int __fnum, char **__files);

#endif	// __COMMONFUNCS_H__