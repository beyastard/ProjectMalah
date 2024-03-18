/*
 * FILE: ML_Utility.h
 *
 * DESCRIPTION: Utilities used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_UTILITY_H_
#define _ML_UTILITY_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Macros used to implement 16.16 fix point calculation
#define	INTTOFIX16(x)		(((int)(x)) << 16)
#define FIX16TOINT(x)		(((x) + 0x00008000) >> 16)
#define FIX16TOINTCEIL(x)	(((x) + 0x0000ffff) >> 16)
#define FIX16TOINTFLOOR(x)	((x) >> 16)
#define FLOATTOFIX16(x)		((int)((x) * 65536.0f + 0.5f))
#define FIX16TOFLOAT(x)		((x) / 65536.0f)

#define FIX16MUL(M1, M2)	(int)(((__int64)(M1) * (M2) + 0x00008000) >> 16)
#define FIX16DIV(D1, D2)	(int)(((__int64)(D1) << 16) / (D2))

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

extern A3DVECTOR3		g_vOrigin;
extern A3DVECTOR3		g_vAxisX;
extern A3DVECTOR3		g_vAxisY;
extern A3DVECTOR3		g_vAxisZ;

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////

bool MLF_FileExist(const char* szFile);
void MLF_ChangeExtension(char* szFile, char* szNewExt);

float MLA_Random(float fMin, float fMax);
int MLA_Random(int iMin, int iMax);
float MLA_RandomSign(float fMin, float fMax);
int MLA_RandomSign(int iMin, int iMax);
bool MLA_PercentProbability(int iPercent);
bool MLA_PercentProbability(float fPercent);
int MLA_GenRandomVectorH(A3DVECTOR3& vVec);
A3DVECTOR3 MLA_GetRandomVector();
float MLA_ReverseSqrt(float fNum);
void MLA_GetViewUpAndRight(A3DVECTOR3& vDir, A3DVECTOR3* pvUp, A3DVECTOR3* pvRight);
A3DVECTOR3 MLA_GetRandomPosInDisk(A3DVECTOR3& vCenter, A3DVECTOR3& vUp, A3DVECTOR3& vRight, float fOutRad, float fInRad=0.0f);

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Round float to int
inline int MLA_SecureFloatToInt(float fValue)
{
	if (fValue < 0.0f)
		return (int)(fValue - 0.5f);
	else
		return (int)(fValue + 0.5f);
}

//	Calculate square root for specified float number
inline float MLA_Sqrt(float fNum)
{
	return 1.0f / MLA_ReverseSqrt(fNum);
}

//	Calculate absolute value for specified float number. Below code was got from
//	Quake III source code
inline float MLA_fabs(float fNum)
{
	int iTmp = *(int*) &fNum;
	iTmp &= 0x7FFFFFFF;
	return *(float*) &iTmp;
}

inline bool	MLA_Percent50()
{
	return (rand() & 1) ? true : false;
}

/*	Get a random positionn in specified sphere.

	Return random position.

	vCenter: sphere's center position.
	fOutRad: sphere's outer radius.
	fInRad: sphere's inner radius.
*/
inline A3DVECTOR3 MLA_GetRandomPosInSphere(A3DVECTOR3& vCenter, float fOutRad, float fInRad/* 0.0f */)
{
	A3DVECTOR3 vPos = MLA_GetRandomVector();
	vPos = vCenter + vPos * MLA_Random(fInRad, fOutRad);
	return vPos;
}


#endif	//	_ML_UTILITY_H_



