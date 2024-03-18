/*
 * FILE: ML_Utility.cpp
 *
 * DESCRIPTION: Utilities used in game Malah
 *
 * CREATED BY: Duyuxin, 2002/12/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "A3DTypes.h"
#include "A3DFuncs.h"
#include "A3DMathUtility.h"
#include <stdio.h>
#include <assert.h>
#include "ML_Utility.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////

A3DVECTOR3		g_vOrigin(0.0f);
A3DVECTOR3		g_vAxisX(1.0f, 0.0f, 0.0f);
A3DVECTOR3		g_vAxisY(0.0f, 1.0f, 0.0f);
A3DVECTOR3		g_vAxisZ(0.0f, 0.0f, 1.0f);

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement
//
///////////////////////////////////////////////////////////////////////////

/*	Check whether specified file exist.

	Return true specified file exist, otherwise return false.

	szFile: file's full name
*/
bool MLF_FileExist(const char* szFile)
{
	if (!szFile || !szFile[0])
		return false;

	FILE* fp = fopen(szFile, "rb");
	if (!fp)
		return false;

	fclose(fp);
	return true;
}

/*	Change file's extension

	szFile (in, out): file name.
	szNewExt: new extension of file
*/
void MLF_ChangeExtension(char* szFile, char* szNewExt)
{
	char* pTemp = strrchr(szFile, '.');
	if (!pTemp)
	{
		strcat(szFile, ".");
		strcat(szFile, szNewExt);
	}
	else
	{
		strcpy(pTemp+1, szNewExt);
	}
}

/*	Get a random float value in [fMin, fMax].

	Return random float value.

	fMin, fMax: Range of random value
*/
float MLA_Random(float fMin, float fMax)
{
	float fDelta = fMax - fMin;
	float fRand	 = (float)rand() / RAND_MAX;

	return fRand * fDelta + fMin;
}

/*	Get a random int value in [iMin, iMax].

	Return random float value.

	fMin, fMax: Range of random value
*/
int MLA_Random(int iMin, int iMax)
{
	int iDelta	= iMax - iMin;
	int iRand	= rand() % (iDelta + 1);

	return iRand + iMin;
}

//	Get a random float value in [-fMax, -fMin] or [fMin, fMax].
float MLA_RandomSign(float fMin, float fMax)
{
	float fDelta = fMax - fMin;
	float fRand	 = (float)rand() / RAND_MAX;

	float fResult = fRand * fDelta + fMin;
	if (rand() & 1)
		return -fResult;
	else
		return fResult;
}

//	Get a random integer value in [-iMax, -iMin] or [iMin, iMax].
int MLA_RandomSign(int iMin, int iMax)
{
	int iDelta	= iMax - iMin;
	int iRand	= rand() % (iDelta + 1);

	int iResult = iRand + iMin;
	if (rand() & 1)
		return -iResult;
	else
		return iResult;
}

/*	Check whether a probability happen.

	Return true if probability happen, otherwise return false.

	iPercent: probability represented in percent (0 -- 100).
*/
bool MLA_PercentProbability(int iPercent)
{
	int iRand = rand() % 100;
	return iRand < iPercent ? true : false;
}

/*	Check whether a probability happen.

	Return true if probability happen, otherwise return false.

	fPercent: probability represented in percent (0.00 -- 100.00).
*/
bool MLA_PercentProbability(float fPercent)
{
	int iRand = rand() % 10000;
	return iRand < (int)(fPercent * 1000) ? true : false;
}

/*	Generate a normalized vector whose x, z is random value and y is 0.0f

	Return degree the vector rotate around y axis (x axis is 0 degree)

	vVec (out): used to receive result vector
*/
int MLA_GenRandomVectorH(A3DVECTOR3& vVec)
{
	assert(g_pA3DMathUtility);

	int iDegree = MLA_Random(0, 359);

	vVec.x = g_pA3DMathUtility->COS((float)iDegree);
	vVec.z = g_pA3DMathUtility->SIN((float)iDegree);
	vVec.y = 0.0f;

	return iDegree;
}

A3DVECTOR3 MLA_GetRandomVector()
{
	assert(g_pA3DMathUtility);

	A3DVECTOR3 vVec;
	float fScale;

	int iDegree = MLA_Random(0, 179);
	vVec.y = g_pA3DMathUtility->SIN((float)iDegree);
	fScale = g_pA3DMathUtility->COS((float)iDegree);

	iDegree = MLA_Random(0, 359);
	vVec.x = g_pA3DMathUtility->COS((float)iDegree) * fScale;
	vVec.z = g_pA3DMathUtility->SIN((float)iDegree) * fScale;

	return vVec;
}

//	Calculate reverse square root of a number. Below code was got from Quake III
//	source code.
float MLA_ReverseSqrt(float fNum)
{
	static float fThreeHalfs = 1.5f;

	long i;
	float x2, y;

	x2 = fNum * 0.5f;
	y  = fNum;
	i  = *(long*) &y;				//	Evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);		//	What the fuck?
	y  = *(float*) &i;
	y  = y * (fThreeHalfs - (x2 * y * y));	//	1st iteration
//	y  = y * (fThreeHalfs - (x2 * y * y));	//	2nd iteration, this can be removed

	return y;
}

/*	Calculate view coordinate's up and right axis

	vDir: view's normalized direction
	pvUp (out): used to receive view's up direction, can be NULL.
	pvRight (out): used to receive view's right direction, can be NULL
*/
void MLA_GetViewUpAndRight(A3DVECTOR3& vDir, A3DVECTOR3* pvUp, A3DVECTOR3* pvRight)
{
	A3DVECTOR3 vRight = CrossProduct(g_vAxisY, vDir);
	float fLen = Normalize(vRight, vRight);

	if (fLen < 1e-6f)
		vRight = Normalize(CrossProduct(g_vAxisX, vDir));
	
	if (pvRight)
		*pvRight = vRight;

	if (pvUp)
		*pvUp = CrossProduct(vDir, vRight);
}

/*	Get a random positionn in specified disk.

	Return random position.

	vCenter: disk's center position.
	vUp: up direction of disk's coordinates
	vRight: right direction of disk's coordinates
	fOutRad: sphere's outer radius.
	fInRad: sphere's inner radius.
*/
A3DVECTOR3 MLA_GetRandomPosInDisk(A3DVECTOR3& vCenter, A3DVECTOR3& vUp, A3DVECTOR3& vRight, 
								  float fOutRad, float fInRad/* 0.0f */)
{
	A3DVECTOR3 vPos, vDir;

	int iDegree = MLA_Random(0, 359);
	vDir.x = g_pA3DMathUtility->COS((float)iDegree);
	vDir.y = g_pA3DMathUtility->SIN((float)iDegree);
	vDir.z = 0.0f;

	vDir = vDir * MLA_Random(fInRad, fOutRad);
	vPos = vCenter + vUp * vDir.y + vRight * vDir.x;

	return vPos;
}



