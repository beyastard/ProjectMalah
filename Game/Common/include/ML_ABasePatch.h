/*
 * FILE: ML_ABasePatch.h
 *
 * DESCRIPTION: This is patch for class ASkyInit, AEnvironment and ASceneInit
 *				in ABase. That two ugly classes don't implement class-serial
 *				strictly.
 *
 * CREATED BY: Duyuxin, 2002/11/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _ML_ABASEPATCH_H_
#define _ML_ABASEPATCH_H_

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
//	Class CMLSky
//
///////////////////////////////////////////////////////////////////////////

class CMLSky : public ASkyInit
{
public:		//	Types

	DECLARE_ASERIAL(CMLSky)

public:		//	Constructor and Destructor

	CMLSky() : ASkyInit()
	{
		m_int_skytype = 2;
	}

	virtual ~CMLSky() {}

public:		//	Attributes

public:		//	Operations

	bool Init()
	{
		//	Remove name's readonly flag
		AProperty* pProp = Properies("名称");
		if (pProp)
			pProp->SetWay(pProp->GetWay() & ~WAY_READONLY);

		//	Set readonly property to sky type
		if ((pProp = Properies("天空类型")))
			pProp->SetWay(pProp->GetWay() | WAY_READONLY);

		return true;
	}

	//	Load sky set
	virtual bool Load(AArchive& ar)
	{
		if (!AData::Load(ar))
			return false;

		if (!ASkyInit::Load(ar))
			return false;

		return true;
	}

	//	Save sky set
	virtual bool Save(AArchive& ar)
	{
		if (!AData::Save(ar))
			return false;

		if (!ASkyInit::Save(ar))
			return false;

		return true;
	}

protected:	//	Attributes

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEnvironment
//
///////////////////////////////////////////////////////////////////////////

class CMLEnvironment : public AEnvironmentInit
{
public:		//	Types

	DECLARE_ASERIAL(CMLEnvironment)

public:		//	Constructor and Destructor

	CMLEnvironment() {}
	virtual ~CMLEnvironment() {}

public:		//	Attributes

public:		//	Operations

	bool Init()
	{
		//	Remove name's readonly flag
		AProperty* pProp = Properies("名称");
		if (pProp)
			pProp->SetWay(pProp->GetWay() & ~WAY_READONLY);

		return true;
	}

	//	Load sky set
	virtual bool Load(AArchive& ar)
	{
		if (!AData::Load(ar))
			return false;

		if (!AEnvironmentInit::Load(ar))
			return false;

		return true;
	}

	//	Save sky set
	virtual bool Save(AArchive& ar)
	{
		if (!AData::Save(ar))
			return false;

		if (!AEnvironmentInit::Save(ar))
			return false;

		return true;
	}

protected:	//	Attributes

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLEnvScheme
//
///////////////////////////////////////////////////////////////////////////

class CMLEnvScheme : public AData
{
public:		//	Types

	DECLARE_ASERIAL(CMLEnvScheme)

public:		//	Constructor and Destructor

	CMLEnvScheme() {}
	virtual ~CMLEnvScheme() {}

public:		//	Attributes

public:		//	Operations

	void		SetSkyName(AUString strSky)		{	m_strSky = strSky;	}
	void		SetEnvName(AUString strEnv)		{	m_strEnv = strEnv;	}

	AUString&	GetSkyName()		{	return m_strSky;	}
	AUString&	GetEnvName()		{	return m_strEnv;	}

	//	Load sky set
	virtual bool Load(AArchive& ar)
	{
		if (!AData::Load(ar))
			return false;

		if (!ar.ReadString(m_strSky))
			return false;

		if (!ar.ReadString(m_strEnv))
			return false;

		return true;
	}

	//	Save sky set
	virtual bool Save(AArchive& ar)
	{
		if (!AData::Save(ar))
			return false;

		if (!ar.WriteString(m_strSky))
			return false;

		if (!ar.WriteString(m_strEnv))
			return false;

		return true;
	}

protected:	//	Attributes

	AUString	m_strSky;		//	Sky's reference
	AUString	m_strEnv;		//	Environment's reference

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Class CMLSceneData
//
///////////////////////////////////////////////////////////////////////////

class CMLSceneData : public ASceneInit
{
public:		//	Types

	DECLARE_ASERIAL(CMLSceneData)

public:		//	Constructor and Destructor

	CMLSceneData() {}
	virtual ~CMLSceneData() {}

public:		//	Attributes

	STATICPROPERIES_BEGIN(ASceneInit, CMLSceneData)
		STATICPROPERTY(CMLSceneData, "VSS 文件", AUString, m_strVSSFile, NULL, NULL, WAY_FILENAME, NULL)
	STATICPROPERIES_END(CMLSceneData)

public:		//	Operations

	void		SetSchemeName(AUString& strName)	{	m_strScheme = strName;	}
	AUString&	GetSchemeName()						{	return m_strScheme;		}
	void		SetVSSFile(AUString& strVSS)		{	m_strVSSFile = strVSS;	}
	AUString&	GetVSSFile()						{	return m_strVSSFile;	}

	virtual bool Load(AArchive& ar);	//	Load sky set
	virtual bool Save(AArchive& ar);	//	Save sky set

protected:	//	Attributes

	AUString	m_strScheme;	//	Scheme's reference
	AUString	m_strVSSFile;	//	VSS file's name

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_ML_ABASEPATCH_H_

