/*
 * FILE: ML_BulletShell.h
 *
 * DESCRIPTION: Bullet shell class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/24
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_BULLETSHELL_H_
#define _ML_BULLETSHELL_H_

#include "ML_ModelUnit.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


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


///////////////////////////////////////////////////////////////////////////
//
//	Class CMLBulletShell
//
///////////////////////////////////////////////////////////////////////////

class CMLBulletShell : public CMLModelUnit
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLBulletShell();
	virtual ~CMLBulletShell() {}

public:		//	Attributes

public:		//	Operations

	bool			Init(int iID, A3DVECTOR3& vPos, A3DVECTOR3& vVel);	//	Initialize object
	virtual bool	Release();		//	Release resources

	virtual bool	LogicRun(float fTimeSpan, DWORD dwTimeCnt);		//	Logic tick

protected:	//	Attributes

	A3DVECTOR3	m_vRotateAxis;	//	Rotate axis
	float		m_fRadian;		//	Rotate radian

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_BULLETSHELL_H_

