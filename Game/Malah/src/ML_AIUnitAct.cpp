/*
 * FILE: ML_AIUnitAct.cpp
 *
 * DESCRIPTION: AI unit actions for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/6
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_AIUnit.h"

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
//	Implement CMLAIUnit
//
///////////////////////////////////////////////////////////////////////////

//	Actions
const char* CMLAIUnit::m_aActions[NUM_AI_ID][ACT_NUM] = 
{
	{	//	Footman's actions
		"�ֿ�_��ǹ���_��45��",		//	ACT_JUMP_RIGHT,
		"�ֿ�_��ǹ���_��0��",		//	ACT_JUMP,				
		"�ֿ�_��ǹ���_��45��",		//	ACT_JUMP_LEFT,			
		"�ֿ�_������",			//	ACT_CHARGE_SHOT,		
		"�ֿ�_��ǹ���_��45��",		//	ACT_CHARGE_RIGHT45,		
		"�ֿ�_��ǹ���_��45��",		//	ACT_CHARGE_RIGHT90,		
		"�ֿ�_��ǹ���_��0��",		//	ACT_CHARGE,				
		"�ֿ�_��ǹ���_��45��",		//	ACT_CHARGE_LEFT45,		
		"�ֿ�_��ǹ���_��45��",		//	ACT_CHARGE_LEFT90,		
		"�ֿ�_��_���˱�����",		//	ACT_DIE_BACKWARD0,		
		"�ֿ�_��_����ˤ",			//	ACT_DIE_BACKWARD1,		
		"�ֿ�_��_��������",			//	ACT_DIE_BACKWARD2,		
		"�ֿ�_��_Ť������",			//	ACT_DIE_STRUGGLE,		
		"�ֿ�_��_ǰ�屻����",		//	ACT_DIE_FORWARD0,		
		"�ֿ�_��_ǰ�˵���",			//	ACT_DIE_FORWARD1,		
		"�ֿ�_��_ը��",				//	ACT_DIE_FLY,	
		"�ֿ�_ը���ڿ�_��",			//	ACT_DIE_FLY_LEFT
		"�ֿ�_ը���ڿ�_��",			//	ACT_DIE_FLY_RIGHT
		"�ֿ�_ը�����_��",			//	ACT_DIE_FLYFALL_LEFT
		"�ֿ�_ը�����_��",			//	ACT_DIE_FLYFALL_RIGHT
		"�ֿ�_վ�����",			//	ACT_STAND_SHOT,			
		"�ֿ�_վ����׼",			//	ACT_STAND_REAMMO,		
		"�ֿ�_վ����׼",			//	ACT_STAND_AIM,			
		"�ֿ�_�Ż𵹱�",			//	ACT_ONFIRE_DIE,			
		"�ֿ�_�Ż�����",			//	ACT_ONFIRE,				
		"�ֿ�_�Ż��Ӵ�",			//	ACT_ONFIRE_RUN,			
		"�ֿ�_�������",			//	ACT_SQUAT_SHOT_RIGHT45,	
		"�ֿ�_�������",			//	ACT_SQUAT_SHOT_RIGHT80,	
		"�ֿ�_�������",			//	ACT_SQUAT_SHOT,			
		"�ֿ�_�������",			//	ACT_SQUAT_SHOT_LEFT45,	
		"�ֿ�_�������",			//	ACT_SQUAT_SHOT_LEFT80,	
		"�ֿ�_������׼",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"�ֿ�_������׼",			//	ACT_SQUAT_REAMMO,		
		"�ֿ�_������׼",			//	ACT_SQUAT_REAMMO_LEFT,	
		"�ֿ�_������׼",			//	ACT_SQUAT_AIM_RIGHT45,	
		"�ֿ�_������׼",			//	ACT_SQUAT_AIM_RIGHT90,	
		"�ֿ�_������׼",			//	ACT_SQUAT_AIM,			
		"�ֿ�_������׼",			//	ACT_SQUAT_AIM_LEFT45,	
		"�ֿ�_������׼",			//	ACT_SQUAT_AIM_LEFT90,	

		//	Mox model actions
		"[���]",					//	MOXACT_CHARGE,
		"[��ɱ]",					//	MOXACT_STAB,		
		"[վ��]",					//	MOXACT_STAND,
		"[����A]",					//	MOXACT_DIE0,
		"[����B]",					//	MOXACT_DIE1,
		"[����C]",					//	MOXACT_DIE2,
	},
	{	//	Guerilla's actions
		"����_��������",			//	ACT_JUMP_RIGHT,
		"����_��������",			//	ACT_JUMP,				
		"����_��������",			//	ACT_JUMP_LEFT,			
		"����_��������",			//	ACT_CHARGE_SHOT,		
		"����_��������",			//	ACT_CHARGE_RIGHT45,		
		"����_��������",			//	ACT_CHARGE_RIGHT90,		
		"����_��������",			//	ACT_CHARGE,				
		"����_��������",			//	ACT_CHARGE_LEFT45,		
		"����_��������",			//	ACT_CHARGE_LEFT90,		
		"����_����_��غ���ˤ",		//	ACT_DIE_BACKWARD0,		
		"����_����_��غ���ˤ",		//	ACT_DIE_BACKWARD1,		
		"����_����_��غ���ˤ",		//	ACT_DIE_BACKWARD2,		
		"����_����",				//	ACT_DIE_STRUGGLE,		
		"����_����_ǰ��Զ����",		//	ACT_DIE_FORWARD0,		
		"����_����_ǰ��Զ����",		//	ACT_DIE_FORWARD1,		
		"����_ը��",				//	ACT_DIE_FLY,			
		"����_ը��",				//	ACT_DIE_FLY_LEFT,		
		"����_ը��",				//	ACT_DIE_FLY_RIGHT,		
		"����_ը��",				//	ACT_DIE_FLYFALL_LEFT,	
		"����_ը��",				//	ACT_DIE_FLYFALL_RIGHT,	
		"����_վ�����",			//	ACT_STAND_SHOT,			
		"����_վ�����",			//	ACT_STAND_REAMMO,		
		"����_վ�����",			//	ACT_STAND_AIM,			
		"����_����",				//	ACT_ONFIRE_DIE,			
		"����_�Ż�����",			//	ACT_ONFIRE,				
		"����_�Ż�����",			//	ACT_ONFIRE_RUN,			
		"����_վ�����",			//	ACT_SQUAT_SHOT_RIGHT45,	
		"����_վ�����",			//	ACT_SQUAT_SHOT_RIGHT80,	
		"����_վ�����",			//	ACT_SQUAT_SHOT,			
		"����_վ�����",			//	ACT_SQUAT_SHOT_LEFT45,	
		"����_վ�����",			//	ACT_SQUAT_SHOT_LEFT80,	
		"����_վ�����",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"����_վ�����",			//	ACT_SQUAT_REAMMO,		
		"����_վ�����",			//	ACT_SQUAT_REAMMO_LEFT,	
		"����_վ�����",			//	ACT_SQUAT_AIM_RIGHT45,	
		"����_վ�����",			//	ACT_SQUAT_AIM_RIGHT90,	
		"����_վ�����",			//	ACT_SQUAT_AIM,			
		"����_վ�����",			//	ACT_SQUAT_AIM_LEFT45,	
		"����_վ�����",			//	ACT_SQUAT_AIM_LEFT90,
		
		//	Mox model actions
		NULL,						//	MOXACT_CHARGE,
		NULL,						//	MOXACT_STAB,		
		NULL,						//	MOXACT_STAND,
		NULL,						//	MOXACT_DIE0,
		NULL,						//	MOXACT_DIE1,
		NULL,						//	MOXACT_DIE2,
	},		
	{	//	Rocket's a"ctions
		"���_�����������_��45��",	//	ACT_JUMP_RIGHT,
		"���_�����������_��0��",	//	ACT_JUMP,				
		"���_�����������_��45��",	//	ACT_JUMP_LEFT,			
		"���_�����������_��0��",	//	ACT_CHARGE_SHOT,		
		"���_�����������_��45��",	//	ACT_CHARGE_RIGHT45,		
		"���_�����������_��45��",	//	ACT_CHARGE_RIGHT90,		
		"���_�����������_��0��",	//	ACT_CHARGE,				
		"���_�����������_��45��",	//	ACT_CHARGE_LEFT45,		
		"���_�����������_��45��",	//	ACT_CHARGE_LEFT90,		
		"�ֿ�_��_���˱�����",		//	ACT_DIE_BACKWARD0,		
		"�ֿ�_��_����ˤ",			//	ACT_DIE_BACKWARD1,		
		"�ֿ�_��_��������",			//	ACT_DIE_BACKWARD2,		
		"�ֿ�_��_Ť������",			//	ACT_DIE_STRUGGLE,		
		"�ֿ�_��_ǰ�屻����",		//	ACT_DIE_FORWARD0,		
		"�ֿ�_��_ǰ�˵���",			//	ACT_DIE_FORWARD1,		
		"�ֿ�_��_ը��",				//	ACT_DIE_FLY,			
		"�ֿ�_ը���ڿ�_��",			//	ACT_DIE_FLY_LEFT
		"�ֿ�_ը���ڿ�_��",			//	ACT_DIE_FLY_RIGHT
		"�ֿ�_ը�����_��",			//	ACT_DIE_FLYFALL_LEFT
		"�ֿ�_ը�����_��",			//	ACT_DIE_FLYFALL_RIGHT
		"[վ������ϵ�]",			//	ACT_STAND_SHOT,			
		"���_վ�ϵ�",				//	ACT_STAND_REAMMO,		
		"���_վ����׼",			//	ACT_STAND_AIM,			
		"�ֿ�_�Ż𵹱�",			//	ACT_ONFIRE_DIE,			
		"�ֿ�_�Ż�����",			//	ACT_ONFIRE,				
		"�ֿ�_�Ż��Ӵ�",			//	ACT_ONFIRE_RUN,			
		"[��������ϵ�_��]",		//	ACT_SQUAT_SHOT_RIGHT45,	
		"[��������ϵ�_��]",		//	ACT_SQUAT_SHOT_RIGHT80,	
		"[��������ϵ�_��]",		//	ACT_SQUAT_SHOT,			
		"[��������ϵ�_��]",		//	ACT_SQUAT_SHOT_LEFT45,	
		"[��������ϵ�_��]",		//	ACT_SQUAT_SHOT_LEFT80,	
		"���_���ϵ�_��",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"���_���ϵ�_��",			//	ACT_SQUAT_REAMMO,		
		"���_���ϵ�_��",			//	ACT_SQUAT_REAMMO_LEFT,	
		"���_������45����׼",		//	ACT_SQUAT_AIM_RIGHT45,	
		"���_������45����׼",		//	ACT_SQUAT_AIM_RIGHT90,	
		"���_������0����׼",		//	ACT_SQUAT_AIM,			
		"���_������45����׼",		//	ACT_SQUAT_AIM_LEFT45,	
		"���_������45����׼",		//	ACT_SQUAT_AIM_LEFT90,
		
		//	Mox model actions
		NULL,						//	MOXACT_CHARGE,
		NULL,						//	MOXACT_STAB,		
		NULL,						//	MOXACT_STAND,
		NULL,						//	MOXACT_DIE0,
		NULL,						//	MOXACT_DIE1,
		NULL,						//	MOXACT_DIE2,
	},
};
