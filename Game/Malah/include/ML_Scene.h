/*
 * FILE: ML_Scene.h
 *
 * DESCRIPTION: Scene class for Malah game
 *
 * CREATED BY: Duyuxin, 2002/11/28
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_ML_SCENE_H_
#define _ML_SCENE_H_

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
//	Class CMLScene
//
///////////////////////////////////////////////////////////////////////////

class CMLScene : public AScene
{
public:		//	Types

public:		//	Constructor and Destructor

	CMLScene();
	virtual ~CMLScene() {}

public:		//	Attributes

public:		//	Operations

	virtual bool	Create(const PAWORLD_INIT_SCENE __pDatum);
	virtual bool	Release();

	virtual bool	UpdateVisibleSets(A3DViewport* pCurrentViewport);

	bool	LoadVSSData(const char* szVSSFile);		//	Load VSS data

protected:	//	Attributes

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_SCENE_H_

