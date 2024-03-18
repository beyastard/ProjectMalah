/*
 * FILE: Malah.cpp
 *
 * DESCRIPTION: Main file of game Malah
 *
 * CREATED BY: Duyuxin, 2002/10/16
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_Global.h"
#include "ML_MainFrame.h"
#include "ML_Strings.h"
#include "AFI.h"
#include "resource.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


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

CMDLINEPARAMS	g_CmdParams;
CMLMainFrame	g_MainFrame;

///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////

void _ParseCommandLine(char* szCmdLine);
void _DefaultCmdParams();
bool _InitGameApp();
bool _CheckCDROM();
LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////
//
//	Implement 
//
///////////////////////////////////////////////////////////////////////////

//	WinMain
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//	Parse command line
	_ParseCommandLine(lpCmdLine);

	//	Do some initial work
	if (!_InitGameApp())
	{
		ADebug::Msg(0, "Failed to load string file!", __FILE__, __LINE__);
		return -1;
	}

	GUID guid;
	HRESULT hCreateGuid = CoCreateGuid(&guid);

	HCURSOR hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEICON));

	if (!g_MainFrame.Create("Malah Game", guid, 0, 0, BASERENDER_WIDTH, BASERENDER_HEIGHT, hInstance, lpCmdLine, nCmdShow, _WndProc))
	{
		MessageBox(NULL, "Failed to create main Window", "Error", MB_OK);
		return -1;
	}

	g_MainFrame.ShowWindow(nCmdShow);

	//	Build Game initialize structure
	INITAGAMESTRUCT GameInit;
	memset(&GameInit, 0, sizeof (GameInit));

	GameInit.InitA3DEngine.hWnd			= g_MainFrame.m_hWnd;
	GameInit.InitA3DEngine.bIsWindow	= !g_CmdParams.iFullScreen;
	GameInit.InitA3DEngine.hInstance	= hInstance;
	GameInit.InitA3DEngine.nWidth		= g_MainFrame.GetClientWidth();
	GameInit.InitA3DEngine.nHeight		= g_MainFrame.GetClientHeight();

	if (!g_theGame.Init(&GameInit))
		return -1;

	g_theGame.Run();

	g_theGame.Release();

	CloseFilePackage();
	AFileMod_Finalize();

	return 0;
}

/*	Parse program's command line

	szCmdLine: command line string
*/
void _ParseCommandLine(char* szCmdLine)
{
	char* szCmd;

	//	Set default value for command line parameters
	_DefaultCmdParams();

	if ((szCmd = strstr(szCmdLine, "rendersize:")))
		sscanf(szCmd, "rendersize:%dx%d", &g_CmdParams.iRenderWid, &g_CmdParams.iRenderHei);

	if ((szCmd = strstr(szCmdLine, "fullscreen:")))
		sscanf(szCmd, "fullscreen:%d", &g_CmdParams.iFullScreen);

	if ((szCmd = strstr(szCmdLine, "startmission:")))
	{
		sscanf(szCmd, "startmission:%d", &g_CmdParams.iStartMission);
		g_CmdParams.iStartMission--;
		CLAMPVALUE(g_CmdParams.iStartMission, 0, NUM_GAMELEVEL-1);
	}
}

//	Fill command line parameters to default value
void _DefaultCmdParams()
{
	memset(&g_CmdParams, 0, sizeof (g_CmdParams));

	g_CmdParams.iFullScreen	= 0;
	g_CmdParams.iRenderWid	= BASERENDER_WIDTH;
	g_CmdParams.iRenderHei	= BASERENDER_HEIGHT;
}

/*	Do some initialzations, include setting work directory, parse ini file etc.

	Return true for success, otherwise return false.
*/
bool _InitGameApp()
{
	AFileMod_Initialize();

	//	Set app's path as work directory
	GetModuleFileName(NULL, g_szWorkDir, MAX_PATH);
	strcpy(g_szWorkDir, g_szBaseDir);
	strcat(g_szWorkDir, "\\");
	char* pTemp = strrchr(g_szWorkDir, '\\');
	*(pTemp + 1) = '\0';

	strcpy(g_szIniFile, g_szWorkDir);
	strcat(g_szIniFile, "Malah.ini");

	AFileMod_SetBaseDir(g_szWorkDir);

	char szFile[MAX_PATH];

	//	Need to check CD-ROM ?
	//sprintf(szFile, "%s%s", g_szWorkDir, "Data\\disc.cfg");
	//if (MLF_FileExist(szFile))
	//{
	//	if (!_CheckCDROM())
	//	{
	//	#ifdef LANGUAGE_CHINESE
	//		MessageBox(NULL, "请先插入游戏光盘再运行游戏。", "提示", MB_OK);
	//	#else
	//		MessageBox(NULL, "Please insert game disc first.", "Note", MB_OK);
	//	#endif
	//
	//		return false;
	//	}
	//}

	//	Get general set file name
	GetPrivateProfileString("PathFile", "GeneralSetFile", "Data\\genset.cfg", g_szGenSetFile, MAX_PATH, g_szIniFile);

	//	Open file package if it exists
	GetPrivateProfileString("PathFile", "PackageFile", "null", szFile, MAX_PATH, g_szIniFile);

	if (_stricmp(szFile, "null"))
	{
		if (!OpenFilePackage(szFile))
		{
			MessageBox(NULL, "Failed to open file package", "Error", MB_OK);
			return false;
		}
	}

	//	Load strings used in game
	if (!g_MLStrings.LoadStrings("Data\\Strings.txt"))
	{
		ADebug::Msg(0, "Failed to load string file!", __FILE__, __LINE__);
		return false;
	}

	return true;
}

LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//	First send to AGame object, then to AMainFrame if AGame doesn't handle it
	if (g_theGame.WndProc(hWnd, message, wParam, lParam))
		return 0;
	
	switch (message)
	{
/*	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:

			if (HIWORD(wParam) == BN_CLICKED)
			{
				::PostMessage(hWnd,WM_CLOSE,0,0);
				break;
			}

			break;
		}

		break;
	}
*/
	case WM_SETCURSOR:
		
		SetCursor(NULL);
		break;

	default:
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//	Check whether our CD is in CD-ROM ?
bool _CheckCDROM()
{
	char szFile[256];

	for (int iDriver=1; iDriver < 26; iDriver++)
	{
		sprintf(szFile, "%c:", 'A'+iDriver-1);
		UINT uType = GetDriveType(szFile);

		if (uType == DRIVE_CDROM)
		{
			//	Read the file Data\disc.id
			strcat(szFile, "data_cn\\Data\\disc.cfg");

			FILE* fp = fopen(szFile, "rb");
			if (!fp)
				continue;

			char szId[256];
			if (!fgets(szId, 256, fp))
			{
				fclose(fp);
				continue;
			}

			fclose(fp);

			if (!strcmp(szId, "EpieGame-Disc-ID:HoChiMinhTrail"))
				return true;
		}
	}

	return false;
}


