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

	if (!_stricmp(pFile->m_szToken, "AI攻击直升机的半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIShotCopterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI向前跳跃的距离值"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIJumpDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI射击时间间隔正负偏差"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIShotInterRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI产生间隔时间正负偏差"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIGenInterRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI奔跑速度正负偏差"))
	{
		pFile->GetNextToken(false);
		m_Params.iAISpeedRange = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI死后尸体保留时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iAICorpseTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI跑动中连续射击的概率"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIFusilladePercent = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI火箭弹飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIRocketSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "伤害值和准确度衰减近距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIAttenuationNear = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "伤害值和准确度衰减远距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fAIAttenuationFar = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI抛出枪和钢盔的概率"))
	{
		pFile->GetNextToken(false);
		m_Params.iAIThrowGunPercentage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "AI小范围移动射击的移动范围"))
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

	if (!_stricmp(pFile->m_szToken, "手枪射击间隔时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iPistolInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "步枪单发射击间隔时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iRifleSingleInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "步枪连发射击间隔时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iRifleStrafeInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "迫击炮射击间隔时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iMortarInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "转轮机枪射击间隔时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iMachineGunInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "火箭筒射击时间间隔"))
	{
		pFile->GetNextToken(false);
		m_Params.iRPGInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "转轮机枪的散射范围"))
	{
		pFile->GetNextToken(false);
		m_Params.iMachineGunDither = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "迫击炮爆炸内半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "迫击炮爆炸外半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "迫击炮的到达时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iMortarArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "迫击炮落点最近水平距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fMortarMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "步枪狙击模式伤害倍率"))
	{
		pFile->GetNextToken(false);
		m_Params.fRifleSingleDamage = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "步枪狙击镜放大倍数"))
	{
		pFile->GetNextToken(false);
		m_Params.fRifleMagnifyScale = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "火箭筒爆炸范围"))
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

	if (!_stricmp(pFile->m_szToken, "直升机到达的时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iCopterArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "炮火支援的到达时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iCannonArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "攻击机到达时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iAttackerArriveTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "炮弹爆炸内半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "炮弹爆炸外半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "燃烧弹爆炸内半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballInnerRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "炮弹落点最近水平距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fCannonMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "燃烧弹落点最近水平距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballMinDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "燃烧弹爆炸外半径"))
	{
		pFile->GetNextToken(false);
		m_Params.fFireballOuterRadius = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "直升机的飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterFlyHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "直升机的补给高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterSupplyHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "直升机的飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "直升机补给停留时间"))
	{
		pFile->GetNextToken(false);
		m_Params.iCopterHangTime = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "直升机被击中后下落的速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fCopterDropSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "攻击机的飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fAttackerHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "攻击机的飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fAttackerSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "投掷的燃烧弹个数"))
	{
		pFile->GetNextToken(false);
		m_Params.iFireballNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "燃烧弹覆盖距离长度"))
	{
		pFile->GetNextToken(false);
		m_Params.iFireballBombLen = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "支援炮火齐射次数"))
	{
		pFile->GetNextToken(false);
		m_Params.iCannonFireTimes = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "支援炮火每次齐射炮弹数"))
	{
		pFile->GetNextToken(false);
		m_Params.iNumCannonBomb = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "支援炮火落点误差半径"))
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

	if (!_stricmp(pFile->m_szToken, "米格21飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21Speed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21最低飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21MinHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21最高飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21MaxHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21发射子弹个数"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21BulletNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21最小伤害值"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21MinDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21最大伤害值"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21MaxDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21射击时间间隔"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21FireInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21开火距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21FireDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21射击距离长度"))
	{
		pFile->GetNextToken(false);
		m_Params.fMIG21FireLen = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "米格21生命值"))
	{
		pFile->GetNextToken(false);
		m_Params.iMIG21Life = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN最低飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanMinHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN最高飞行高度"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanMaxHeight = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN火箭弹飞行速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanRocketSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN发射火箭个数"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanRocketNum = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN射击准确度"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanPrecise = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN火箭弹的伤害值"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN射击时间间隔"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanFireInterval = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN开火距离"))
	{
		pFile->GetNextToken(false);
		m_Params.fFantanFireDist = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN生命值"))
	{
		pFile->GetNextToken(false);
		m_Params.iFantanLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "卡车移动速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fTruckSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "坦克移动速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fTankSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "装甲车移动速度"))
	{
		pFile->GetNextToken(false);
		m_Params.fArmoredSpeed = (float)atof(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "卡车生命值"))
	{
		pFile->GetNextToken(false);
		m_Params.iTruckLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "坦克生命值"))
	{
		pFile->GetNextToken(false);
		m_Params.iTankLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "装甲车生命值"))
	{
		pFile->GetNextToken(false);
		m_Params.iArmoredLife = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "坦克伤害值"))
	{
		pFile->GetNextToken(false);
		m_Params.iTankDamage = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "装甲车伤害值"))
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

	if (!_stricmp(pFile->m_szToken, "可使用任务数"))
	{
		pFile->GetNextToken(false);
		m_Params.iMaxMission = atoi(pFile->m_szToken) - 1;
	}
	else if (!_stricmp(pFile->m_szToken, "步枪兵分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreFootman = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "游击队分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreGuerilla = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "火箭筒分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreRocket = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "卡车分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreTruck = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "坦克分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreTank = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "装甲车分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreArmored = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "MIG21分值"))
	{
		pFile->GetNextToken(false);
		m_Params.iScoreMIG21 = atoi(pFile->m_szToken);
	}
	else if (!_stricmp(pFile->m_szToken, "FANTAN分值"))
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

