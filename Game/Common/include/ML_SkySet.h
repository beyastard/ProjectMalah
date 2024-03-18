/*
 * FILE: ML_SkySet.h
 *
 * DESCRIPTION: Sky data set in game Malah
 *
 * CREATED BY: Duyuxin, 2002/11/15
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_SKYSET_H_
#define _ML_SKYSET_H_

#include "ML_ABasePatch.h"

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
//	Class CMLSkySet
//
///////////////////////////////////////////////////////////////////////////

class CMLSkySet : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLSkySet)

public:		//	Constructor and Destructor

	CMLSkySet();
	virtual ~CMLSkySet() {}

public:		//	Attributes

public:		//	Operations

	bool			Init();			//	Initialize object
	virtual bool	Release();		//	Release object

	virtual bool	Load(AArchive& ar);		//	Load sky set
	virtual bool	Save(AArchive& ar);		//	Save sky set

	void		AddSky(CMLSky* pSky);			//	Add a sky
	void		DelSky(CMLSky* pSky);			//	Delete a sky
	CMLSky*		SearchSky(AUString& strName);	//	Search a sky by name
	CMLSky*		SearchSky(int iIndex);			//	Search a sky by index

	int			GetSkyNum()		{	return m_SkyList.GetCount();	}

	inline CMLSky*	GetFirstSky();	//	Get first sky
	inline CMLSky*	GetNextSky();	//	Get next sky

protected:	//	Attributes

	AListPosition	m_CurPos;		//	current position

	AListT<CMLSky*>	m_SkyList;		//	Sky list
	
protected:	//	Operations

	void		ReleaseAllSkys();	//	Release all skys
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

//	Get first sky
CMLSky* CMLSkySet::GetFirstSky()
{
	m_CurPos = m_SkyList.GetHeadPosition();
	if (m_CurPos)
		return m_SkyList.GetNext(m_CurPos);
	else
		return NULL;
}

//	Get next sky
CMLSky* CMLSkySet::GetNextSky()
{
	if (!m_CurPos)
		return NULL;

	return m_SkyList.GetNext(m_CurPos);
}



#endif	//	_ML_SKYSET_H_



