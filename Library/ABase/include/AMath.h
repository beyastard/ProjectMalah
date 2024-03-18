/*
 * FILE: AMath.h
 *
 * DESCRIPTION: ������ѧ����
 *
 * CREATED BY: ��Զ��, 2002/8/19
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */


#ifndef __AMATH__H__
#define __AMATH__H__

#include <math.h>
#include "abasea3d.h"

// ��һ���������Ƿ������Ϊ��0
int	IsFloatZero(float __f);

// ����������ת��Ϊˮƽת��(0~2PIE)�ʹ�ֱת��(-PIE/2~PIE/2)��
// __angleh��__anglev�ֱ��ǽǶȵ�ˮƽ��ֵ����ֱ��ֵ
// __htotal��__vtotal�ֱ���ˮƽԲ�ܺ���ֱ��԰�ܵĵȷָ���
int	Dir2TwoAngle(float __dx, float __dy, float __dz
	, int *__angleh, int *__anglev, int __htotal, int __vtotal);
int	TwoAngle2Dir(int __angleh, int __anglev, int __htotal, int __vtotal
	, float *__dx, float *__dy, float *__dz);
int	TwoAngle2Dir(float __angleh, float __anglev
	, float *__dx, float *__dy, float *__dz);
int	NormalizeDir(float *__dx, float *__dy, float *__dz);

// ˮƽ�Ƕ�(0~2PIE)����ֱ�Ƕ�(-PIE/2~PIE/2)
int	TwoAngle2Int(float __fh, float __fv, int *__ih, int *__iv, int __htotal, int __vtotal);
int	Int2TwoAngle(int __ih, int __iv, int __htotal, int __vtotal, float *__fh, float *__fv);

//ˮƽΪ��������ֱΪ(-PIE/2~PIE/2)
int	DirAngle2Vector(float __dx, float __dz, float __av, float *__dirx, float *__diry, float *__dirz);
int	Vector2DirAngle(float __dirx, float __diry, float __dirz, float *__dx, float *__dz, float *__av);
// ����������ɽǶ�[0,2PIE)������-1��ʾû�нǶ�
int	Dir2Degree(float __dx, float __dz, float *__degree);
int	Degree2Dir(float __degree, float *__dx, float *__dz);
// ���[1,360]��0��ʾû�нǶ�
int	Dir2IntDegree(float __dx, float __dz, int *__degree);
//����-1��ʾ����(x1,z1)���������(x,z)��ƫ��������ƫ
int GetPointWay(float x,float z,float x1,float z1);
// ��ֵ
float	GetInterpolation(float f, float f1, float f2, float v1, float v2);
A3DVECTOR3 UpProduct(A3DVECTOR3 vecDir);

#endif  //__AMATH__H__