/*
 * FILE: ML_ModelUnit.h
 *
 * DESCRIPTION: Model unit base class for game Malah
 *
 * CREATED BY: Duyuxin, 2003/4/24
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_MODELUNIT_H_
#define _ML_MODELUNIT_H_

#include "ML_Unit.h"

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
//	Class CMLModelUnit
//
///////////////////////////////////////////////////////////////////////////

class CMLModelUnit : public CMLUnit
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLModelUnit();
	virtual ~CMLModelUnit() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Release();		//	Release resources

	virtual bool	Render(A3DViewport* pViewport);					//	Render routine
	virtual bool	TickAnimation();								//	Tick animation

	virtual void	PauseSound(bool bPause);	//	Pause sound
	virtual void	StopSound();				//	Stop sound

	virtual void	SetPos(A3DVECTOR3& vPos);		//	Set position

protected:	//	Attributes

	A3DModel*	m_pMoxModel;	//	A3D model
	bool		m_bSort;		//	Need sort flag
	bool		m_bCollect;		//	Need collect flag
	
protected:	//	Operations

	bool	PlayModelAction(char* szName, bool bDiscardOld, bool bStopSFX=true,
							bool bStopGFX=true);	//	Play model action
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_UNIT_H_

