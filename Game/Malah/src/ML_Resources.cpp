/*
 * FILE: ML_Resources.cpp
 *
 * DESCRIPTION: Resources operations for game Malah
 *
 * CREATED BY: Duyuxin, 2003/1/2
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include <string.h>
#include <assert.h>
#include "ML_Language.h"
#include "ML_Resources.h"

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

const char* l_aResNames [] =
{
	"¸Ö¿ø_Ê¿±ø",		//	RES_IM_FOOTMAN
	"¶·óÒ_Ê¿±ø",		//	RES_IM_GUERILLA
	"»ð¼ý_Ê¿±ø",		//	RES_IM_ROCKETMAN
};

const char* l_aOtherResFiles [] =
{
	"ImgModels\\ÌùÍ¼×é.imd",		//	RES_IM_AIGROUPFILE
	"Missions\\LvlFiles.txt",		//	RES_LEVELDESCFILE		
	"Data\\Parameters.txt",			//	RES_PARAMETERFILE		
};

const char* l_aModelFiles[] = 
{
#ifdef LANGUAGE_CHINESE

	"ÊÖÇ¹\\ÊÖÇ¹.mod",								//	RES_MOX_PISTOL:		
	"Thompson³å·æÇ¹\\Thompson³å·æÇ¹.mod",			//	RES_MOX_RIFLE:				
	"ÆÈ»÷ÅÚ\\ÆÈ»÷ÅÚ.mod",							//	RES_MOX_MORTAR:		
	"×ªÂÖ»úÇ¹\\×ªÂÖ»úÇ¹.mod",						//	RES_MOX_MACHINE:
	"Bazooka»ð¼ýÍ²\\Bazooka»ð¼ýÍ².mod",				//	RES_MOX_RPG
	"ÈÕ±¾Â½¾ü\\ÈÕ±¾Â½¾ü.mod",						//	RES_MOX_FOOTMAN:		
	"ÈÕ±¾ÇÇÃñ\\ÈÕ±¾ÇÇÃñ¹í×Ó.mod",					//	RES_MOX_GUERILLA:		
	"ÈÕ±¾»ð¼ý±ø\\ÈÕ±¾»ð¼ý¹í×Ó.mod",					//	RES_MOX_ROCKETMAN:		
	"RPG7»ð¼ýÍ²\\RPG7_GUN.mod",						//	RES_MOX_ROCKETBULLET:	
	"Õ½»ú_P38\\P-38Lig.mod",						//	RES_MOX_ATTACKER:		
	"M101Áñµ¯ÅÚ\\M101Áñµ¯ÅÚ.mod",					//	RES_MOX_CANNON:		
	"Ö±Éý»ú_BellH13\\BellH13.mod",					//	RES_MOX_COPTER:		
	"Ö±Éý»ú_BellH13\\BellH13_»÷ÖÐ×¹Âä.mod",			//	RES_MOX_COPTERCRASH:	
	"Ö±Éý»ú_BellH13\\BellH13_²Ðº¡.mod",				//	RES_MOX_COPTERWRECK:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\ÆÈ»÷ÅÚ_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_MTLOCALIZER:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\È¼ÉÕµ¯_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_ATLOCALIZER:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\Áñµ¯ÅÚ_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_CNLOCALIZER:	
	"×Óµ¯ÀàÄ£ÐÍ\\ÊÖÇ¹µ¯¿Ç.mod",						//	RES_MOX_PISTOLSHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\M-16Ç¹µ¯¿Ç.mod",					//	RES_MOX_RIFLESHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\×ªÂÖ»úÇ¹µ¯¿Ç.mod",					//	RES_MOX_MACHINESHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\×ªÂÖ»úÇ¹±»²ðµ¯Á´.mod",				//	RES_MOX_MACHINECHAIN:	
	"Õ½»ú_Zero\\zero.mod",							//	RES_MOX_MIG21:			
	"Õ½»ú_kyofu\\kyofu.mod",						//	RES_MOX_FANTAN:	
	"ÔË±ø³µ\\¾üÓÃÔË±ø³µ.mod",						//	RES_MOX_TRUCK
	"Õ½³µ_97Ê½Ì¹¿Ë\\97Ê½Ì¹¿Ë.mod",					//	RES_MOX_TANK	
	"Õ½³µ_×°¼×³µ\\×°¼×³µ.mod",						//	RES_MOX_ARMORED
	"ÔË±ø³µ\\¾üÓÃÔË±ø³µ»÷»Ù.mod",					//	RES_MOX_TRUCKWRECKAGE
	"Õ½³µ_97Ê½Ì¹¿Ë\\97Ê½Ì¹¿Ë»÷»Ù.mod",				//	RES_MOX_TANKWRECKAGE
	"Õ½³µ_×°¼×³µ\\×°¼×³µ_²Ðº¡.mod",					//	RES_MOX_ARMOREDWRECKAGE
	"¾«ÖÂÎäÆ÷_ÊÖÇ¹\\ÊÖÇ¹.mod",						//	RES_MOX_HIGHPISTOL
	"ÎäÆ÷_Thompson³å·æÇ¹\\Thompson³å·æÇ¹.mod",		//	RES_MOX_HIGHRIFLE
	"¾«ÖÂÎäÆ÷_ÆÈ»÷ÅÚ\\ÆÈ»÷ÅÚ.mod",					//	RES_MOX_HIGHMORTAR
	"¾«ÖÂÎäÆ÷_×ªÂÖ»úÇ¹\\×ªÂÖ»úÇ¹.mod",				//	RES_MOX_HIGHMACHINE
	"ÎäÆ÷_Bazooka»ð¼ýÍ²\\Bazooka»ð¼ýÍ².mod",		//	RES_MOX_HIGHRPG

#else

	"ÊÖÇ¹\\ÊÖÇ¹.mod",								//	RES_MOX_PISTOL:		
	"M-16Ç¹\\Ö÷¹Û_M16.mod",							//	RES_MOX_RIFLE:				
	"ÆÈ»÷ÅÚ\\ÆÈ»÷ÅÚ.mod",							//	RES_MOX_MORTAR:		
	"×ªÂÖ»úÇ¹\\×ªÂÖ»úÇ¹.mod",						//	RES_MOX_MACHINE:
	"RPG7»ð¼ýÍ²\\Ö÷¹Û_RPG7»ð¼ýÍ².mod",				//	RES_MOX_RPG
	"¸Ö¿øÔ½¹²\\¸Ö¿øÔ½¹².mod",						//	RES_MOX_FOOTMAN:		
	"¶·óÒÔ½¹²\\¶·óÒÔ½¹².mod",						//	RES_MOX_GUERILLA:		
	"»ð¼ýÔ½¹²\\»ð¼ýÔ½¹².mod",						//	RES_MOX_ROCKETMAN:		
	"RPG7»ð¼ýÍ²\\RPG7_GUN.mod",						//	RES_MOX_ROCKETBULLET:	
	"Õ½»ú_F4-Phantom\\F4-Phantom.mod",				//	RES_MOX_ATTACKER:		
	"M101Áñµ¯ÅÚ\\M101Áñµ¯ÅÚ.mod",					//	RES_MOX_CANNON:		
	"Õ½»ú_UH1E\\Ö±Éý»ú_UH1E.mod",					//	RES_MOX_COPTER:		
	"Õ½»ú_UH1E\\Ö±Éý»ú_UH1E_»÷ÖÐ×¹Âä.mod",			//	RES_MOX_COPTERCRASH:	
	"Õ½»ú_UH1E\\Ö±Éý»ú_UH1E_×¹Âä²Ðº¡.mod",			//	RES_MOX_COPTERWRECK:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\ÆÈ»÷ÅÚ_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_MTLOCALIZER:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\È¼ÉÕµ¯_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_ATLOCALIZER:	
	"ÅÚ»ðÎ»ÖÃ±ê¼Ç\\Áñµ¯ÅÚ_ÅÚ»ðÎ»ÖÃ±ê¼Ç.mod",		//	RES_MOX_CNLOCALIZER:	
	"×Óµ¯ÀàÄ£ÐÍ\\ÊÖÇ¹µ¯¿Ç.mod",						//	RES_MOX_PISTOLSHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\M-16Ç¹µ¯¿Ç.mod",					//	RES_MOX_RIFLESHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\×ªÂÖ»úÇ¹µ¯¿Ç.mod",					//	RES_MOX_MACHINESHELL:	
	"×Óµ¯ÀàÄ£ÐÍ\\×ªÂÖ»úÇ¹±»²ðµ¯Á´.mod",				//	RES_MOX_MACHINECHAIN:	
	"Õ½»ú_Mig21\\mig21.mod",						//	RES_MOX_MIG21:			
	"Õ½»ú_Ç¿5\\Ç¿5.mod",							//	RES_MOX_FANTAN:	
	"ÔË±ø³µ\\¾üÓÃÔË±ø³µ.mod",						//	RES_MOX_TRUCK
	"Õ½³µ_T54\\T54.mod",							//	RES_MOX_TANK	
	"Õ½³µ_PT76\\PT76.mod",							//	RES_MOX_ARMORED
	"ÔË±ø³µ\\¾üÓÃÔË±ø³µ»÷»Ù.mod",					//	RES_MOX_TRUCKWRECKAGE
	"Õ½³µ_T54\\T54_²Ðº¡01.mod",						//	RES_MOX_TANKWRECKAGE
	"Õ½³µ_PT76\\PT76_²Ðº¡01.mod",					//	RES_MOX_ARMOREDWRECKAGE
	"¾«ÖÂÎäÆ÷_ÊÖÇ¹\\ÊÖÇ¹.mod",						//	RES_MOX_HIGHPISTOL
	"¾«ÖÂÎäÆ÷_M16\\M16.mod",						//	RES_MOX_HIGHRIFLE
	"¾«ÖÂÎäÆ÷_ÆÈ»÷ÅÚ\\ÆÈ»÷ÅÚ.mod",					//	RES_MOX_HIGHMORTAR
	"¾«ÖÂÎäÆ÷_×ªÂÖ»úÇ¹\\×ªÂÖ»úÇ¹.mod",				//	RES_MOX_HIGHMACHINE
	"¾«Ï¸ÎäÆ÷_RPG7\\¾«Ï¸ÎäÆ÷_RPG7.mod",				//	RES_MOX_HIGHRPG

#endif	//	LANGUAGE_CHINESE
};

const char* l_aGFXFiles[] = 
{
	"»÷ÖÐ_ÈËÌåÑª»¨.gfx",		//	RES_GFX_HITBLOOD:		
	"ÊÖÇ¹¿ª»ð.gfx",				//	RES_GFX_GUNFIRELIGHT:	
	"»ð¼ýÍ²·¢Éä.gfx",			//	RES_GFX_ROCKETLAUNCH:	
	"»ð¼ýµ¯·ÉÐÐ_Low.gfx",		//	RES_GFX_ROCKETFLY0	
	"»ð¼ýµ¯·ÉÐÐ_Middle.gfx",	//	RES_GFX_ROCKETFLY1
	"»ð¼ýµ¯·ÉÐÐ.gfx",			//	RES_GFX_ROCKETFLY2
	"»ð¼ýµ¯±¬Õ¨.gfx",			//	RES_GFX_ROCKETHIT
	"±¬Õ¨_È¼ÉÕ_15fps.gfx",		//	RES_GFX_FIREBALL0:		
	"±¬Õ¨_È¼ÉÕ.gfx",			//	RES_GFX_FIREBALL1:		
	"È¼ÉÕµ¯±¬Õ¨.gfx",			//	RES_GFX_FIREBALL2:		
	"È¼ÉÕ²ÐÁô.gfx",				//	RES_GFX_FIREREMAINS:	
	"±¬Õ¨_»ðÃç_01.gfx",			//	RES_GFX_BODYFIRE:		
	"±¬Õ¨_ÍÁÑÌ_01.gfx",			//	RES_GFX_CANNONEXPLODE:	
	"±¬Õ¨_ÍÁÑÌ.gfx",			//	RES_GFX_MORTEREXPLODE:	
	"±¬Õ¨_·É»úËé¿é.gfx",		//	RES_GFX_COPTEREXPLODE:				
	"±¬Õ¨_·É»úËéÆ¬.gfx",		//	RES_GFX_COPTERSHIVER:	
	"Ö±Éý»ú¿ª»ð.gfx",			//	RES_GFX_COPTERFIRE
	"Ö÷¹Û_µ¶ÅüÑªºÛ_ºá.gfx",		//	RES_GFX_STABHURT0:		
	"Ö÷¹Û_µ¶ÅüÑªºÛ_Êú.gfx",		//	RES_GFX_STABHURT1:		
	"Ö÷¹Û_±¬Õ¨»÷ÖÐ.gfx",		//	RES_GFX_ROCKETHURT:	
	"»÷ÖÐ_µØÃæ.gfx",			//	RES_GFX_GUNHITGROUD:	
	"¸Ö¿øÅ×³ö.gfx",				//	RES_GFX_THROWNHELMET:	
	"²½Ç¹Å×³ö.gfx",				//	RES_GFX_THROWNGUN:		
	"»ð¼ýÍ²Å×³ö.gfx",			//	RES_GFX_THROWBAZOOKA:	
	"¶·óÒÅ×³ö.gfx",				//	RES_GFX_THROWNBBHAT:	
	"¸Ö¿øÕ¨·É.gfx",				//	RES_GFX_BLOWNHELMET
	"²½Ç¹Õ¨·É.gfx",				//	RES_GFX_BLOWNGUN	
	"»ð¼ýÍ²Õ¨·É.gfx",			//	RES_GFX_BLOWNBAZOOKA
	"¶·óÒÕ¨·É.gfx",				//	RES_GFX_BLOWNNBBHAT
	"±¬Õ¨_ÄàµØ.gfx",			//	RES_GFX_EXPLODEMUD
	"±¬Õ¨_ÄàµØ´øµ¯¿Ó.gfx",		//	RES_GFX_EXPLODEMUDHOLD
	"±¬Õ¨_²ÐÁôµ¯¿Ó_Ð¡.gfx",		//	RES_GFX_BOMBHOLE0
	"±¬Õ¨_²ÐÁôµ¯¿Ó_´ó.gfx",		//	RES_GFX_BOMBHOLE1
	"±¬Õ¨_Õ½»úËé¿é_Ç¿5.gfx",	//	RES_GFX_FANTANEXPLODE
	"±¬Õ¨_Õ½»úËé¿é_Mig21.gfx",	//	RES_GFX_MIG21EXPLODE
	"Ì¹¿ËÅÚ¹ÜÕ¨·É.gfx",			//	RES_GFX_TANKGUNTHROW
	"×°¼×³µÅÚ¹ÜÕ¨·É.gfx",		//	RES_GFX_ARMOREDGUNTHROW
	"Áñµ¯ÅÚ¿ª»ð.gfx",			//	RES_GFX_TANKFIRE
	"Õ½³µÎ²ÑÌ_²»Ñ­»·.gfx",		//	RES_GFX_TANKSMOKE
	"±¬Õ¨_È¼ÉÕ01.gfx",			//	RES_GFX_TANKEXPLODE
	"Ð§¹û»ð»¨.gfx",				//	RES_GFX_SPARK
};

const char* l_aSoundFiles[] = 
{
	//	2D sounds ...
	"ColtM1911_fire.wav",		//	RES_S2D_PISTOLFIRE
	"M16A2_fire.wav",			//	RES_S2D_RIFLEFIRE
	"MortarFire.wav",			//	RES_S2D_MORTARFIRE
	"MiniGun_Fire_.wav",		//	RES_S2D_MACHINEFIRE
	"MiniGun_Run.wav",			//	RES_S2D_MACHINERUN
	"RPGRocket_fire.wav",		//	RES_S2D_RPGFIRE
	"HowitzerFire.wav",			//	RES_S2D_CANNONFIRE
	"BulletWhizz_01.wav",		//	RES_S2D_BULLETWHIZZ1
	"BulletWhizz_02.wav",		//	RES_S2D_BULLETWHIZZ2
	"BulletWhizz_03.wav",		//	RES_S2D_BULLETWHIZZ3
	"BulletWhizz_04.wav",		//	RES_S2D_BULLETWHIZZ4
	"BulletWhizz_05.wav",		//	RES_S2D_BULLETWHIZZ5
	"BulletWhizz_06.wav",		//	RES_S2D_BULLETWHIZZ6
	"BulletWhizz_07.wav",		//	RES_S2D_BULLETWHIZZ7
	"BulletWhizz_08.wav",		//	RES_S2D_BULLETWHIZZ8
	"BulletWhizz_09.wav",		//	RES_S2D_BULLETWHIZZ9
	"BulletWhizz_10.wav",		//	RES_S2D_BULLETWHIZZ10
	"BulletWhizz_11.wav",		//	RES_S2D_BULLETWHIZZ11
	"BulletWhizz_12.wav",		//	RES_S2D_BULLETWHIZZ12
	"Bulletway01.wav",			//	RES_S3D_BULLETDROP1
	"Bulletway02.wav",			//	RES_S3D_BULLETDROP2
	"Bulletway03.wav",			//	RES_S3D_BULLETDROP3
	"Bulletway04.wav",			//	RES_S3D_BULLETDROP4
	"Bulletway05.wav",			//	RES_S3D_BULLETDROP5
	"Bulletway06.wav",			//	RES_S3D_BULLETDROP6
	"Bulletway07.wav",			//	RES_S2D_BULLETDROP7
	"Bulletway08.wav",			//	RES_S2D_BULLETDROP8
	"Bulletway09.wav",			//	RES_S2D_BULLETDROP9
	"Bulletway10.wav",			//	RES_S2D_BULLETDROP10
	"Bulletway11.wav",			//	RES_S2D_BULLETDROP11
	"Bulletway12.wav",			//	RES_S2D_BULLETDROP12
	"Weather_LightRain.wav",	//	RES_S2D_RAIN1
	"Weather_LightRain02.wav",	//	RES_S2D_RAIN2
	"Weather_MediumRain.wav",	//	RES_S2D_RAIN3
	"Thunder_1.wav",			//	RES_S2D_THUNDER1
	"Thunder_2.wav",			//	RES_S2D_THUNDER2
	"Thunder_3.wav",			//	RES_S2D_THUNDER3
	"Thunder_4.wav",			//	RES_S2D_THUNDER4
	"Thunder_5.wav",			//	RES_S2D_THUNDER5
	"Thunder_6.wav",			//	RES_S2D_THUNDER6
	"Ambience_ForestDay04.wav",	//	RES_S2D_SUNNY1
	"Ambience_ForestDay05.wav",	//	RES_S2D_SUNNY2
	"Ambience_ForestDay06.wav",	//	RES_S2D_SUNNY3
	"Ambience_ForestNight01.wav",	//	RES_S2D_NIGHT1
	"weapon_NoAmmo.wav",		//	RES_S2D_NOAMMO
	"PushButton.wav",			//	RES_S2D_PUSHBUTTON
	"AmmoFull.wav",				//	RES_S2D_SUPPLYFINISH

	//	Voice sounds ...
	"Voice\\artillery01.wav",	//	RES_S2D_CANNONCALLING,		
	"Voice\\artillery12.wav",	//	RES_S2D_CANNONDENY,			
	"Voice\\artillery11.wav",	//	RES_S2D_CANNONAGREE,		
	"Voice\\heli01.wav",		//	RES_S2D_COPTERCALLING,		
	"Voice\\heli12.wav",		//	RES_S2D_COPTERDENY,			
	"Voice\\heli11.wav",		//	RES_S2D_COPTERAGREE,		
	"Voice\\heli13.wav",		//	RES_S2D_COPTERDOWN,	
	"Voice\\napalm01.wav",		//	RES_S2D_ATTACKERCALLING,	
	"Voice\\napalm12.wav",		//	RES_S2D_ATTACKERDENY,		
	"Voice\\napalm11.wav",		//	RES_S2D_ATTACKERAGREE,		
	"Voice\\win01.wav",			//	RES_S2D_MISSIONCOMPLETED1,	
	"Voice\\win02.wav",			//	RES_S2D_MISSIONCOMPLETED2,	
	"Voice\\win03.wav",			//	RES_S2D_MISSIONCOMPLETED3,	
	"Voice\\loss01.wav",		//	RES_S2D_MISSIONFAILED1,		
	"Voice\\loss02.wav",		//	RES_S2D_MISSIONFAILED2,		
	"Voice\\loss03.wav",		//	RES_S2D_MISSIONFAILED3,		
	"Voice\\hit01.wav",			//	RES_S2D_HITENEMYPLANE1,		
	"Voice\\hit02.wav",			//	RES_S2D_HITENEMYPLANE2,		
	"Voice\\hit03.wav",			//	RES_S2D_HITENEMYPLANE3,		
	"Voice\\hit11.wav",			//	RES_S2D_HITENEMYVEHICLE1,	
	"Voice\\hit12.wav",			//	RES_S2D_HITENEMYVEHICLE2,	
	"Voice\\hit13.wav",			//	RES_S2D_HITENEMYVEHICLE3,	

	//	3D sounds ...
	"MortarWhizz.wav",			//	RES_S3D_MORTARWHIZZ
	"MortarHit.wav",			//	RES_S3D_MORTARHIT
	"FireBombExplosion.wav",	//	RES_S3D_FIREBOMBEXPLODE
	"FireBombBurning.wav",		//	RES_S3D_FIREBOMBFIRE	
	"FireBombExtinguish.wav",	//	RES_S3D_FIREBOMBEXTINCT
	"RPGRocket_fire.wav",		//	RES_S3D_ROCKETFIRE	
	"RPGRocket_Hit.wav",		//	RES_S3D_ROCKETHIT	
	"RPGRocket_Fly.wav",		//	RES_S3D_ROCKETFLY
	"HowitzerHit_02.wav",		//	RES_S3D_CANNONHIT1		
	"HowitzerHit_04.wav",		//	RES_S3D_CANNONHIT2		
	"HowitzerHit_05.wav",		//	RES_S3D_CANNONHIT3		
	"HowitzerHit_06.wav",		//	RES_S3D_CANNONHIT4		
	"AK47_fire.wav",			//	RES_S3D_AK47FIRE1
	"AK47SemiBurst_fire.wav",	//	RES_S3D_AK47FIRE2
	"AK47Burst_fire.wav",		//	RES_S3D_AK47FIRE3
	"HueyHeliLoop.wav",			//	RES_S3D_COPTERFLY
	"HueyM60Fire.wav",			//	RES_S3D_COPTERFIRE
	"HueyRocketHurt_01.wav",	//	RES_S3D_COPTEREXPLODE1
	"HueyRocketHurt_02.wav",	//	RES_S3D_COPTEREXPLODE2
	"HueyRocketHurt_03.wav",	//	RES_S3D_COPTEREXPLODE3
	"HueyHeliCrash_01.wav",		//	RES_S3D_COPTERCRASH1
	"HueyHeliCrash_02.wav",		//	RES_S3D_COPTERCRASH2
	"HueyHeliCrash_03.wav",		//	RES_S3D_COPTERCRASH3
	"HueyHeliCrash_04.wav",		//	RES_S3D_COPTERCRASH4
	"HueyHeliCrash_05.wav",		//	RES_S3D_COPTERCRASH5
	"HueyHurt_01.wav",			//	RES_S3D_COPTERHURT1
	"HueyHurt_02.wav",			//	RES_S3D_COPTERHURT2
	"HueyHurt_03.wav",			//	RES_S3D_COPTERHURT3
	"HueyEngineDown.wav",		//	RES_S3D_COPTERFLYDOWN
	"VietNamRush_01.wav",		//	RES_S3D_AIRUSH1
	"VietNamRush_02.wav",		//	RES_S3D_AIRUSH2
	"VietNamRush_03.wav",		//	RES_S3D_AIRUSH3
	"VietNamRush_04.wav",		//	RES_S3D_AIRUSH4
	"VietNamRush_05.wav",		//	RES_S3D_AIRUSH5
	"VietNamRush_06.wav",		//	RES_S3D_AIRUSH6
	"VietNamRush_07.wav",		//	RES_S3D_AIRUSH7
	"VietNamRush_08.wav",		//	RES_S3D_AIRUSH8
	"VietNamRush_09.wav",		//	RES_S3D_AIRUSH9
	"VietNamRush_10.wav",		//	RES_S3D_AIRUSH10
	"VietNamRush_11.wav",		//	RES_S3D_AIRUSH11
	"VietNamRush_12.wav",		//	RES_S3D_AIRUSH12
	"VietNamDeathBurn_01.wav",	//	RES_S3D_AIBURNDEATH1
	"VietNamDeathBurn_02.wav",	//	RES_S3D_AIBURNDEATH2
	"VietNamDeathBurn_03.wav",	//	RES_S3D_AIBURNDEATH3
	"VietNamDeathBurn_04.wav",	//	RES_S3D_AIBURNDEATH4
	"VietNamDeathBurn_05.wav",	//	RES_S3D_AIBURNDEATH5
	"VietNamDeathBurn_06.wav",	//	RES_S3D_AIBURNDEATH6
	"VietNamDeathBurn_07.wav",	//	RES_S3D_AIBURNDEATH7
	"VietNamDeath01.wav",		//	RES_S3D_AIDEATH1
	"VietNamDeath02.wav",		//	RES_S3D_AIDEATH2
	"VietNamDeath03.wav",		//	RES_S3D_AIDEATH3
	"VietNamDeath04.wav",		//	RES_S3D_AIDEATH4
	"VietNamDeath05.wav",		//	RES_S3D_AIDEATH5
	"VietNamDeath06.wav",		//	RES_S3D_AIDEATH6
	"VietNamBlastDeath01.wav",	//	RES_S3D_AIBLASTDEATH1
	"VietNamBlastDeath02.wav",	//	RES_S3D_AIBLASTDEATH2
	"VietNamBlastDeath03.wav",	//	RES_S3D_AIBLASTDEATH3
	"VietNamBlastDeath04.wav",	//	RES_S3D_AIBLASTDEATH4
	"VietNamBlastDeath05.wav",	//	RES_S3D_AIBLASTDEATH5
	"VietnamShout01.wav",		//	RES_S3D_AISTABSHOUT1
	"VietnamShout02.wav",		//	RES_S3D_AISTABSHOUT2
	"VietnamShout03.wav",		//	RES_S3D_AISTABSHOUT3
	"VietnamShout04.wav",		//	RES_S3D_AISTABSHOUT4
	"VietnamShout05.wav",		//	RES_S3D_AISTABSHOUT5
	"BulletHitG1st1.wav",		//	RES_S3D_BULLETHITMUD1
	"BulletHitG1st3.wav",		//	RES_S3D_BULLETHITMUD2
	"BulletHitG1st4.wav",		//	RES_S3D_BULLETHITMUD3
	"BulletHitG1st5.wav",		//	RES_S3D_BULLETHITMUD4
	"BulletHitG1st6.wav",		//	RES_S3D_BULLETHITMUD5
	"BulletHitG2st1.wav",		//	RES_S3D_BULLETHITMUD6
	"BulletHitG2st2.wav",		//	RES_S3D_BULLETHITMUD7
	"BulletHitG2st3.wav",		//	RES_S3D_BULLETHITMUD8
	"BulletHitG2st4.wav",		//	RES_S3D_BULLETHITMUD9
	"BulletHitG2st5.wav",		//	RES_S3D_BULLETHITMUD10
	"BulletHitG2st6.wav",		//	RES_S3D_BULLETHITMUD11
	"BulletHitG3st1.wav",		//	RES_S3D_BULLETHITMUD12
	"BulletHitG3st2.wav",		//	RES_S3D_BULLETHITMUD13
	"BulletHitG3st3.wav",		//	RES_S3D_BULLETHITMUD14
	"BulletHitG3st4.wav",		//	RES_S3D_BULLETHITMUD15
	"BulletHitG3st5.wav",		//	RES_S3D_BULLETHITMUD16
	"BulletHitG3st6.wav",		//	RES_S3D_BULLETHITMUD17
	"BulletHitG3st7.wav",		//	RES_S3D_BULLETHITMUD18
	"BulletHitG3st8.wav",		//	RES_S3D_BULLETHITMUD19
	"BulletHitG3st9.wav",		//	RES_S3D_BULLETHITMUD20
	"BulletHitG3st10.wav",		//	RES_S3D_BULLETHITMUD21
	"BulletHitBody_01.wav",		//	RES_S3D_BULLETHITBODY1
	"BulletHitBody_02.wav",		//	RES_S3D_BULLETHITBODY2
	"BulletHitBody_03.wav",		//	RES_S3D_BULLETHITBODY3
	"BulletHitBody_04.wav",		//	RES_S3D_BULLETHITBODY4
	"BulletHitBody_05.wav",		//	RES_S3D_BULLETHITBODY5
	"BulletHitHuey_01.wav",		//	RES_S3D_BULLETHITPLANE1
	"BulletHitHuey_02.wav",		//	RES_S3D_BULLETHITPLANE2
	"BulletHitHuey_03.wav",		//	RES_S3D_BULLETHITPLANE3
	"BulletHitHuey_04.wav",		//	RES_S3D_BULLETHITPLANE4
	"BulletHitHuey_05.wav",		//	RES_S3D_BULLETHITPLANE5
	"BulletHitHuey_06.wav",		//	RES_S3D_BULLETHITPLANE6
	"BulletHitHuey_07.wav",		//	RES_S3D_BULLETHITPLANE7
	"BulletHitHuey_08.wav",		//	RES_S3D_BULLETHITPLANE8
	"BulletHitHuey_09.wav",		//	RES_S3D_BULLETHITPLANE9
	"BulletHitHuey_10.wav",		//	RES_S3D_BULLETHITPLANE10
	"BulletHitHuey_11.wav",		//	RES_S3D_BULLETHITPLANE11
	"BulletHitHuey_12.wav",		//	RES_S3D_BULLETHITPLANE12
	"BulletHitTank01.wav",		//	RES_S3D_BULLETHITTANK1
	"BulletHitTank02.wav",		//	RES_S3D_BULLETHITTANK2
	"BulletHitTank03.wav",		//	RES_S3D_BULLETHITTANK3
	"BulletHitTank04.wav",		//	RES_S3D_BULLETHITTANK4
	"BulletHitTank05.wav",		//	RES_S3D_BULLETHITTANK5
	"HowitzerWhizz_01.wav",		//	RES_S2D_CANNONWHIZZ1
	"HowitzerWhizz_02.wav",		//	RES_S2D_CANNONWHIZZ2
	"HowitzerWhizz_03.wav",		//	RES_S2D_CANNONWHIZZ3
	"BayonetHit01.wav",			//	RES_S3D_STAB0
	"BayonetHit02.wav",			//	RES_S3D_STAB1
	"BayonetHit03.wav",			//	RES_S3D_STAB2
	"Mig21MiniGun.wav",			//	RES_S3D_MIG21FIRE
	"Mig21RocketsFire.wav",		//	RES_S3D_FANTANFIRE
	"Tank_Fire01.wav",			//	RES_S3D_TANKFIRE1
	"Tank_Fire02.wav",			//	RES_S3D_TANKFIRE2
	"TankDestroy.wav",			//	RES_S3D_TANKEXPLODE
	"TankRestartEngine.wav",	//	RES_S3D_TANKSTARTUP
	"TankRunning.wav",			//	RES_S3D_TANKRUNNING
	"Pt76Destroy.wav",			//	RES_S3D_ARMOREDEXPLODE
	"Pt76Running.wav",			//	RES_S3D_ARMOREDRUNNING
};

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement
//
///////////////////////////////////////////////////////////////////////////

const char* RES_GetModelFile(int iID)
{
	assert(iID >= 0 && iID < NUM_RESMOXFILE);
	return l_aModelFiles[iID];
}

const char* RES_GetGFXFile(int iID)
{
	assert(iID >= 0 && iID < NUM_RESGFXFILE);
	return l_aGFXFiles[iID];
}

const char* RES_GetOtherFile(int iID)
{
	assert(iID >= 0 && iID < NUM_RESOTHERFILE);
	return l_aOtherResFiles[iID];
}

const char* RES_GetSoundFile(int iID)
{
	assert(iID >= 0 && iID < NUM_RESSOUNDFILE);
	return l_aSoundFiles[iID];
}

const char* RES_GetName(int iID)
{
	assert(iID >= 0 && iID < NUM_RESNAME);
	return l_aResNames[iID];
}

