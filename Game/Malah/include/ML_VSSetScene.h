/*
 * FILE: ML_VSSetScene.h
 *
 * DESCRIPTION: VSSet scene class used in game Malah.
 *
 * CREATED BY: duyuxin, 2002/7/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _ML_VSSETSCENE_H_
#define _ML_VSSETSCENE_H_

#include "A3DVSSScene.h"

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
//	Class CMLVSSetScene
//
///////////////////////////////////////////////////////////////////////////

class CMLVSSetScene : public A3DVSSScene
{
public:		//	Types

	struct VIEWAREA
	{
		int		iSecion;		//	Section
		BYTE*	pVSSBytes;		//	VSS bytes
	};

public:		//	Constructors and Destructors

	CMLVSSetScene();
	virtual ~CMLVSSetScene() {}

public:		//	Attributes

public:		//	Operations

	virtual bool Init(A3DDevice* pA3DDevice);	//	Initialize object
	virtual bool Release();		//	Release resources

	virtual	bool UpdateVisibleSets(A3DViewport* pViewport);		//	Update visible set

	bool		LoadVSSFile(char* szFile);		//	Load VSS file

protected:	//	Attributes

	int			m_iRouteIdx;		//	Route index
	VIEWAREA	m_aViewAreas[4];	//	Viea areas
	BYTE*		m_pMergedBuf;		//	VSS merged buffer
	int			m_iNumVSSBytes;		//	Length of umcompressed VSS bytes

	int			m_iSampleDegree;	//	Sample degree
	int			m_iPeriod;			//	Sample period
	int			m_iNumSection;		//	Number of section

protected:	//	Operations

	void		CalcViewArea(A3DViewport* pViewport, int aSects[]);	//	Calculate view area
	void		MergeVSSBytes();	//	Merge VSS bytes
	void		GetVisSurfIndices(int* pIdxBuf, int* piNumVisSuf);	//	Get visible surfaces indices
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


#endif	//	_ML_VSSETSCENE_H_

