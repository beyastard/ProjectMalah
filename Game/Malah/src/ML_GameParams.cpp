/*
 * FILE: ML_GameParams.cpp
 *
 * DESCRIPTION: Handle game parameters of game Malah that are described in text file.
 *
 * CREATED BY: Duyuxin, 2003/1/4
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "ML_ABase.h"
#include "ML_GameParams.h"

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


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CMLGameParams
//
///////////////////////////////////////////////////////////////////////////

CMLGameParams::CMLGameParams()
{
}

/*	load game parameters from text file

	Return true for success, otherwise return false.

	szFile: parameter file's full pathfile
*/
bool CMLGameParams::Load(const char* szFile)
{
	AScriptFile Script;
	AFileImage FileImage;
	
	if (!FileImage.Open((char*)szFile, AFILE_OPENEXIST | AFILE_BINARY))
	{
		ADEBUG_TRACE("Failed to open parameter file.");
		return false;
	}

	if (!Script.Open(&FileImage))
	{
		ADEBUG_TRACE("Failed to open script file.");
		return false;
	}

	FileImage.Close();

	//	Clear config information
	memset(&m_Params, 0, sizeof (m_Params));

	if (!ParseFile(&Script))
	{
		ADEBUG_TRACE("Failed to parse script file.");
		return false;
	}

	Script.Close();

	return true;
}

/*	Parse AI config file.

	Return true for success, otherwise return false.

	pFile: AScriptFile object
*/
bool CMLGameParams::ParseFile(AScriptFile* pFile)
{
	while (pFile->GetNextToken(true))
	{
		int iSection = -1;

		if (!_stricmp(pFile->m_szToken, "sec:AI"))
			iSection = 0;
		else if (!_stricmp(pFile->m_szToken, "sec:weapon"))
			iSection = 1;
		else if (!_stricmp(pFile->m_szToken, "sec:supply"))
			iSection = 2;
		else if (!_stricmp(pFile->m_szToken, "sec:others"))
			iSection = 3;
		else if (!_stricmp(pFile->m_szToken, "sec:enemyweapon"))
			iSection = 4;

		if (iSection < 0)
			continue;

		if (!pFile->MatchToken("{", false))
		{
			ADEBUG_TRACE("Failed to match {");
			return false;
		}
		
		while (pFile->PeekNextToken(true))
		{
			if (!_stricmp(pFile->m_szToken, "}"))
			{
				pFile->GetNextToken(true);	//	Skip '{'
				break;
			}
			
			switch (iSection)
			{
			case 0:	ParseAIParameter(pFile);		break;
			case 1:	ParseWeaponParameter(pFile);	break;
			case 2:	ParseSupplyParameter(pFile);	break;
			case 3:	ParseOthersParameter(pFile);	break;
			case 4:	ParseEWParameter(pFile);		break;
			}
		}
	}

	return true;
}

//	Parse 'AI' section
bool CMLGameParams::ParseAIParameter(AScriptFile* pFile)
{
	//	Get command name
	pFile->GetNextToken(true);

	if (!_stricmp(pFile->m_szToken, "AI����ֱ�����İ뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIShotCopterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI��ǰ��Ծ�ľ���ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIJumpDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI���ʱ��������ƫ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIShotInterRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI�������ʱ������ƫ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIGenInterRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI�����ٶ�����ƫ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iAISpeedRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI����ʬ�屣��ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iAICorpseTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI�ܶ�����������ĸ���"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIFusilladePercent = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI����������ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIRocketSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�˺�ֵ��׼ȷ��˥��������"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIAttenuationNear = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�˺�ֵ��׼ȷ��˥��Զ����"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIAttenuationFar = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI�׳�ǹ�͸ֿ��ĸ���"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIThrowGunPercentage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AIС��Χ�ƶ�������ƶ���Χ"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIShiftMin = (float)atof(pFile->m_szToken);

		pFile->GetNextToken(false);
		m_Params.fAIShiftMax = (float)atof(pFile->m_szToken);
	}
	else
	{
		ADEBUG_TRACE1("Unknown parameter: %s", pFile->m_szToken);
	}

	pFile->SkipLine();
	return true;
}

//	Parse 'weapon' section
bool CMLGameParams::ParseWeaponParameter(AScriptFile* pFile)
{
	//	Get command name
	pFile->GetNextToken(true);

	if (!_stricmp(pFile->m_szToken, "��ǹ������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iPistolInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "��ǹ����������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iRifleSingleInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "��ǹ����������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iRifleStrafeInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�Ȼ���������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iMortarInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ת�ֻ�ǹ������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iMachineGunInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "���Ͳ���ʱ����"))
	{
		pFile->GetNextToken(false);
		m_Params.iRPGInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ת�ֻ�ǹ��ɢ�䷶Χ"))
	{
		pFile->GetNextToken(false);
		m_Params.iMachineGunDither = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�Ȼ��ڱ�ը�ڰ뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�Ȼ��ڱ�ը��뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�Ȼ��ڵĵ���ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iMortarArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�Ȼ���������ˮƽ����"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "��ǹ�ѻ�ģʽ�˺�����"))
	{
		pFile->GetNextToken(false);
		m_Params.fRifleSingleDamage = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "��ǹ�ѻ����Ŵ���"))
	{
		pFile->GetNextToken(false);
		m_Params.fRifleMagnifyScale = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "���Ͳ��ը��Χ"))
	{
		pFile->GetNextToken(false);
		m_Params.fRPGRadius = (float)atof(pFile->m_szToken);
	}
	else
	{
		ADEBUG_TRACE1("Unknown parameter: %s", pFile->m_szToken);
	}

	pFile->SkipLine();
	return true;
}

//	Parse 'supply' section
bool CMLGameParams::ParseSupplyParameter(AScriptFile* pFile)
{
	//	Get command name
	pFile->GetNextToken(true);

	if (!_stricmp(pFile->m_szToken, "ֱ���������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iCopterArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�ڻ�֧Ԯ�ĵ���ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iCannonArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "����������ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iAttackerArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�ڵ���ը�ڰ뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�ڵ���ը��뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ȼ�յ���ը�ڰ뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�ڵ�������ˮƽ����"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ȼ�յ�������ˮƽ����"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ȼ�յ���ը��뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ֱ�����ķ��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterFlyHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ֱ�����Ĳ����߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterSupplyHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ֱ�����ķ����ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ֱ��������ͣ��ʱ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iCopterHangTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ֱ���������к�������ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterDropSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�������ķ��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fAttackerHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�������ķ����ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fAttackerSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "Ͷ����ȼ�յ�����"))
	{
		pFile->GetNextToken(false);
		m_Params.iFireballNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "ȼ�յ����Ǿ��볤��"))
	{
		pFile->GetNextToken(false);
		m_Params.iFireballBombLen = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "֧Ԯ�ڻ��������"))
	{
		pFile->GetNextToken(false);
		m_Params.iCannonFireTimes = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "֧Ԯ�ڻ�ÿ�������ڵ���"))
	{
		pFile->GetNextToken(false);
		m_Params.iNumCannonBomb = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "֧Ԯ�ڻ�������뾶"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonFallRadius = (float)atof(pFile->m_szToken);
	}
	else
	{
		ADEBUG_TRACE1("Unknown parameter: %s", pFile->m_szToken);
	}

	pFile->SkipLine();
	return true;
}

//	Parse parameter in 'enemyweapon' section
bool CMLGameParams::ParseEWParameter(AScriptFile* pFile)
{
	//	Get command name
	pFile->GetNextToken(true);

	if (!_stricmp(pFile->m_szToken, "�׸�21�����ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21Speed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21��ͷ��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21MinHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21��߷��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21MaxHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21�����ӵ�����"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21BulletNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21��С�˺�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21MinDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21����˺�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21MaxDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21���ʱ����"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21FireInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21�������"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21FireDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21������볤��"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21FireLen = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�׸�21����ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21Life = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN�����ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN��ͷ��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanMinHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN��߷��и߶�"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanMaxHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN����������ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanRocketSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN����������"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanRocketNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN���׼ȷ��"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanPrecise = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN��������˺�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN���ʱ����"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanFireInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN�������"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanFireDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN����ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�����ƶ��ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fTruckSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "̹���ƶ��ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fTankSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "װ�׳��ƶ��ٶ�"))
	{
		pFile->GetNextToken(false);
		m_Params.fArmoredSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "��������ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iTruckLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "̹������ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iTankLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "װ�׳�����ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iArmoredLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "̹���˺�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iTankDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "װ�׳��˺�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iArmoredDamage = atoi(pFile->m_szToken);
	}
	else
	{
		ADEBUG_TRACE1("Unknown parameter: %s", pFile->m_szToken);
	}

	pFile->SkipLine();
	return true;
}

//	Parse 'others' section
bool CMLGameParams::ParseOthersParameter(AScriptFile* pFile)
{
	//	Get command name
	pFile->GetNextToken(true);

	if (!_stricmp(pFile->m_szToken, "��ʹ��������"))
	{
		pFile->GetNextToken(false);
		m_Params.iMaxMission = atoi(pFile->m_szToken) - 1;
	}
	else if (!_stricmp(pFile->m_szToken, "��ǹ����ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreFootman = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "�λ��ӷ�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreGuerilla = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "���Ͳ��ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreRocket = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "������ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreTruck = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "̹�˷�ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreTank = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "װ�׳���ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreArmored = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "MIG21��ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreMIG21 = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN��ֵ"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreFantan = atoi(pFile->m_szToken);
	}
	else
	{
		ADEBUG_TRACE1("Unknown parameter: %s", pFile->m_szToken);
	}

	pFile->SkipLine();
	return true;
}

