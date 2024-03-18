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
		"钢盔_端枪冲锋_右45度",		//	ACT_JUMP_RIGHT,
		"钢盔_端枪冲锋_正0度",		//	ACT_JUMP,				
		"钢盔_端枪冲锋_左45度",		//	ACT_JUMP_LEFT,			
		"钢盔_冲锋射击",			//	ACT_CHARGE_SHOT,		
		"钢盔_端枪冲锋_右45度",		//	ACT_CHARGE_RIGHT45,		
		"钢盔_端枪冲锋_右45度",		//	ACT_CHARGE_RIGHT90,		
		"钢盔_端枪冲锋_正0度",		//	ACT_CHARGE,				
		"钢盔_端枪冲锋_左45度",		//	ACT_CHARGE_LEFT45,		
		"钢盔_端枪冲锋_左45度",		//	ACT_CHARGE_LEFT90,		
		"钢盔_死_后退被击中",		//	ACT_DIE_BACKWARD0,		
		"钢盔_死_后仰摔",			//	ACT_DIE_BACKWARD1,		
		"钢盔_死_后坐倒地",			//	ACT_DIE_BACKWARD2,		
		"钢盔_死_扭曲倒地",			//	ACT_DIE_STRUGGLE,		
		"钢盔_死_前冲被击中",		//	ACT_DIE_FORWARD0,		
		"钢盔_死_前扑倒地",			//	ACT_DIE_FORWARD1,		
		"钢盔_死_炸飞",				//	ACT_DIE_FLY,	
		"钢盔_炸飞腾空_左",			//	ACT_DIE_FLY_LEFT
		"钢盔_炸飞腾空_右",			//	ACT_DIE_FLY_RIGHT
		"钢盔_炸飞落地_左",			//	ACT_DIE_FLYFALL_LEFT
		"钢盔_炸飞落地_左",			//	ACT_DIE_FLYFALL_RIGHT
		"钢盔_站立射击",			//	ACT_STAND_SHOT,			
		"钢盔_站姿瞄准",			//	ACT_STAND_REAMMO,		
		"钢盔_站姿瞄准",			//	ACT_STAND_AIM,			
		"钢盔_着火倒毙",			//	ACT_ONFIRE_DIE,			
		"钢盔_着火身上",			//	ACT_ONFIRE,				
		"钢盔_着火逃窜",			//	ACT_ONFIRE_RUN,			
		"钢盔_跪姿射击",			//	ACT_SQUAT_SHOT_RIGHT45,	
		"钢盔_跪姿射击",			//	ACT_SQUAT_SHOT_RIGHT80,	
		"钢盔_跪姿射击",			//	ACT_SQUAT_SHOT,			
		"钢盔_跪姿射击",			//	ACT_SQUAT_SHOT_LEFT45,	
		"钢盔_跪姿射击",			//	ACT_SQUAT_SHOT_LEFT80,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_REAMMO,		
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_REAMMO_LEFT,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_AIM_RIGHT45,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_AIM_RIGHT90,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_AIM,			
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_AIM_LEFT45,	
		"钢盔_蹲姿瞄准",			//	ACT_SQUAT_AIM_LEFT90,	

		//	Mox model actions
		"[冲锋]",					//	MOXACT_CHARGE,
		"[刺杀]",					//	MOXACT_STAB,		
		"[站立]",					//	MOXACT_STAND,
		"[死亡A]",					//	MOXACT_DIE0,
		"[死亡B]",					//	MOXACT_DIE1,
		"[死亡C]",					//	MOXACT_DIE2,
	},
	{	//	Guerilla's actions
		"斗笠_冲锋中射击",			//	ACT_JUMP_RIGHT,
		"斗笠_冲锋中射击",			//	ACT_JUMP,				
		"斗笠_冲锋中射击",			//	ACT_JUMP_LEFT,			
		"斗笠_冲锋中射击",			//	ACT_CHARGE_SHOT,		
		"斗笠_冲锋中射击",			//	ACT_CHARGE_RIGHT45,		
		"斗笠_冲锋中射击",			//	ACT_CHARGE_RIGHT90,		
		"斗笠_冲锋中射击",			//	ACT_CHARGE,				
		"斗笠_冲锋中射击",			//	ACT_CHARGE_LEFT45,		
		"斗笠_冲锋中射击",			//	ACT_CHARGE_LEFT90,		
		"斗笠_打死_离地后仰摔",		//	ACT_DIE_BACKWARD0,		
		"斗笠_打死_离地后仰摔",		//	ACT_DIE_BACKWARD1,		
		"斗笠_打死_离地后仰摔",		//	ACT_DIE_BACKWARD2,		
		"斗笠_烧死",				//	ACT_DIE_STRUGGLE,		
		"斗笠_打死_前扑远倒地",		//	ACT_DIE_FORWARD0,		
		"斗笠_打死_前扑远倒地",		//	ACT_DIE_FORWARD1,		
		"斗笠_炸死",				//	ACT_DIE_FLY,			
		"斗笠_炸死",				//	ACT_DIE_FLY_LEFT,		
		"斗笠_炸死",				//	ACT_DIE_FLY_RIGHT,		
		"斗笠_炸死",				//	ACT_DIE_FLYFALL_LEFT,	
		"斗笠_炸死",				//	ACT_DIE_FLYFALL_RIGHT,	
		"斗笠_站立射击",			//	ACT_STAND_SHOT,			
		"斗笠_站立射击",			//	ACT_STAND_REAMMO,		
		"斗笠_站立射击",			//	ACT_STAND_AIM,			
		"斗笠_烧死",				//	ACT_ONFIRE_DIE,			
		"斗笠_着火身上",			//	ACT_ONFIRE,				
		"斗笠_着火身上",			//	ACT_ONFIRE_RUN,			
		"斗笠_站立射击",			//	ACT_SQUAT_SHOT_RIGHT45,	
		"斗笠_站立射击",			//	ACT_SQUAT_SHOT_RIGHT80,	
		"斗笠_站立射击",			//	ACT_SQUAT_SHOT,			
		"斗笠_站立射击",			//	ACT_SQUAT_SHOT_LEFT45,	
		"斗笠_站立射击",			//	ACT_SQUAT_SHOT_LEFT80,	
		"斗笠_站立射击",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"斗笠_站立射击",			//	ACT_SQUAT_REAMMO,		
		"斗笠_站立射击",			//	ACT_SQUAT_REAMMO_LEFT,	
		"斗笠_站立射击",			//	ACT_SQUAT_AIM_RIGHT45,	
		"斗笠_站立射击",			//	ACT_SQUAT_AIM_RIGHT90,	
		"斗笠_站立射击",			//	ACT_SQUAT_AIM,			
		"斗笠_站立射击",			//	ACT_SQUAT_AIM_LEFT45,	
		"斗笠_站立射击",			//	ACT_SQUAT_AIM_LEFT90,
		
		//	Mox model actions
		NULL,						//	MOXACT_CHARGE,
		NULL,						//	MOXACT_STAB,		
		NULL,						//	MOXACT_STAND,
		NULL,						//	MOXACT_DIE0,
		NULL,						//	MOXACT_DIE1,
		NULL,						//	MOXACT_DIE2,
	},		
	{	//	Rocket's a"ctions
		"火箭_扛着武器冲锋_右45度",	//	ACT_JUMP_RIGHT,
		"火箭_扛着武器冲锋_正0度",	//	ACT_JUMP,				
		"火箭_扛着武器冲锋_左45度",	//	ACT_JUMP_LEFT,			
		"火箭_扛着武器冲锋_正0度",	//	ACT_CHARGE_SHOT,		
		"火箭_扛着武器冲锋_右45度",	//	ACT_CHARGE_RIGHT45,		
		"火箭_扛着武器冲锋_右45度",	//	ACT_CHARGE_RIGHT90,		
		"火箭_扛着武器冲锋_正0度",	//	ACT_CHARGE,				
		"火箭_扛着武器冲锋_左45度",	//	ACT_CHARGE_LEFT45,		
		"火箭_扛着武器冲锋_左45度",	//	ACT_CHARGE_LEFT90,		
		"钢盔_死_后退被击中",		//	ACT_DIE_BACKWARD0,		
		"钢盔_死_后仰摔",			//	ACT_DIE_BACKWARD1,		
		"钢盔_死_后坐倒地",			//	ACT_DIE_BACKWARD2,		
		"钢盔_死_扭曲倒地",			//	ACT_DIE_STRUGGLE,		
		"钢盔_死_前冲被击中",		//	ACT_DIE_FORWARD0,		
		"钢盔_死_前扑倒地",			//	ACT_DIE_FORWARD1,		
		"钢盔_死_炸飞",				//	ACT_DIE_FLY,			
		"钢盔_炸飞腾空_左",			//	ACT_DIE_FLY_LEFT
		"钢盔_炸飞腾空_右",			//	ACT_DIE_FLY_RIGHT
		"钢盔_炸飞落地_左",			//	ACT_DIE_FLYFALL_LEFT
		"钢盔_炸飞落地_左",			//	ACT_DIE_FLYFALL_RIGHT
		"[站立射击上弹]",			//	ACT_STAND_SHOT,			
		"火箭_站上弹",				//	ACT_STAND_REAMMO,		
		"火箭_站姿瞄准",			//	ACT_STAND_AIM,			
		"钢盔_着火倒毙",			//	ACT_ONFIRE_DIE,			
		"钢盔_着火身上",			//	ACT_ONFIRE,				
		"钢盔_着火逃窜",			//	ACT_ONFIRE_RUN,			
		"[跪姿射击上弹_右]",		//	ACT_SQUAT_SHOT_RIGHT45,	
		"[跪姿射击上弹_右]",		//	ACT_SQUAT_SHOT_RIGHT80,	
		"[跪姿射击上弹_正]",		//	ACT_SQUAT_SHOT,			
		"[跪姿射击上弹_左]",		//	ACT_SQUAT_SHOT_LEFT45,	
		"[跪姿射击上弹_左]",		//	ACT_SQUAT_SHOT_LEFT80,	
		"火箭_跪上弹_右",			//	ACT_SQUAT_REAMMO_RIGHT,	
		"火箭_跪上弹_正",			//	ACT_SQUAT_REAMMO,		
		"火箭_跪上弹_左",			//	ACT_SQUAT_REAMMO_LEFT,	
		"火箭_蹲姿右45度瞄准",		//	ACT_SQUAT_AIM_RIGHT45,	
		"火箭_蹲姿右45度瞄准",		//	ACT_SQUAT_AIM_RIGHT90,	
		"火箭_蹲姿正0度瞄准",		//	ACT_SQUAT_AIM,			
		"火箭_蹲姿左45度瞄准",		//	ACT_SQUAT_AIM_LEFT45,	
		"火箭_蹲姿左45度瞄准",		//	ACT_SQUAT_AIM_LEFT90,
		
		//	Mox model actions
		NULL,						//	MOXACT_CHARGE,
		NULL,						//	MOXACT_STAB,		
		NULL,						//	MOXACT_STAND,
		NULL,						//	MOXACT_DIE0,
		NULL,						//	MOXACT_DIE1,
		NULL,						//	MOXACT_DIE2,
	},
};
