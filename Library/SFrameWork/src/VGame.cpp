/*
 * FILE: VGame.cpp
 *
 * DESCRIPTION: virtual game object
 *
 * CREATED BY: Hedi, 2002/7/29
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VWorld.h"
#include "VKeyBoardDevice.h"
#include "VKeyBoardTranslator.h"
#include "VGame.h"

// global game object pointer;
VGame				*g_pGame;

VGame::VGame()
{
	m_pA3DEngine		= NULL;
	m_pA3DDevice		= NULL;
	m_pA3DCamera		= NULL;
	m_pA3DViewport		= NULL;
	m_pA3DDirLight		= NULL;

	m_bAutoHideCursor	= true;
	m_pCursor			= NULL;

	m_dwTicks			= 0;
	m_bEmbedded			= false;
	m_bActive			= true;
	m_pVWorld			= NULL;

	m_nInputDeviceNum	= 0;
	memset(m_pInputDevices, 0, sizeof(VInputDevice *) * GAME_MAX_INPUT_DEVICE);
	m_pMsgQueue			= NULL;

	m_hInstance			= NULL;
	m_hDeviceWnd		= NULL;

	memset(m_szClassName, 0, sizeof(m_szClassName));
	memset(m_szWndTitle, 0, sizeof(m_szWndTitle));

	// Below parameters can be modified at derived class's constructor
	m_nWidth			= 640;
	m_nHeight			= 480;
	m_bWindowed			= true;

	m_bPacked			= true;

	_tcscpy(m_szClassName, _T("VGame"));
	_tcscpy(m_szWndTitle, _T("Virtual Game Frame Application"));
}

VGame::~VGame()
{
}

bool VGame::InitEmbedded(HINSTANCE hInstance, HWND hWndDevice, bool bPacked)
{
	m_hDeviceWnd	= hWndDevice;
	m_bWindowed		= true;
	m_bEmbedded		= true;

	RECT rectClient;

	GetClientRect(hWndDevice, &rectClient);
	m_nWidth	= rectClient.right - rectClient.left;
	m_nHeight	= rectClient.bottom - rectClient.top;

	return Init(hInstance, bPacked);
}

bool VGame::Init(HINSTANCE hInstance, bool bPacked)
{
	// Take down the window's parameter;
	m_hInstance = hInstance;
	m_bPacked	= bPacked;

	// Create error log object;
	char	szLogFile[MAX_PATH];
	char	szHello[1024];
	sprintf(szLogFile, "%s.log", m_szClassName);
	sprintf(szHello, "Virtual Game [%s]'s error log", m_szClassName);

	m_pErrLog = new ALog;
	if( NULL == m_pErrLog )
		return false;
	if( !m_pErrLog->Init(szLogFile, szHello) )
		return false;

	if( !InitWindow() )
	{
		m_pErrLog->Log("VGame::Init(), Call InitWindow() fail!");
		return false;
	}

	if( !InitEngine() )
	{
		m_pErrLog->Log("VGame::Init(), Call InitEngine() fail!");
		return false;
	}

	if( !ShowSplash() )
	{
		m_pErrLog->Log("VGame::Init(), ShowSplash() fail!");
		return false;
	}

	if( !InitWorld() )
	{
		m_pErrLog->Log("VGame::Init(), Call InitWorld() fail!");
		return false;
	}

	A3DRECT rect;

	rect.left = 0; rect.right = 32;
	rect.top = 0; rect.bottom = 32;
	m_pCursor = new A3DCursor();

	if( !m_pCursor->Init(m_pA3DDevice, "Cursor.tga", 32, 32, NULL, 1, &rect, A3DPOINT2(0, 0)) )
	{
		m_pCursor->Release();
		delete m_pCursor;
		m_pCursor = NULL;
		m_pErrLog->Log("VGame::Init(), Can not create device cursor!");
	}

	m_pA3DDevice->SetCursor(m_pCursor);
	m_pA3DDevice->ShowCursor(true);
	return true;
}

bool VGame::Release()
{
	m_pA3DDevice->SetCursor(NULL);

	if( m_pCursor )
	{
		m_pCursor->Release();
		delete m_pCursor;
		m_pCursor = NULL;
	}
	ReleaseWorld();

	ReleaseEngine();

	ReleaseWindow();
	
	if( m_pErrLog )
	{
		m_pErrLog->Release();
		delete m_pErrLog;
		m_pErrLog = NULL;
	}
	return true;
}

bool VGame::RunEmbedded()
{
	// First do precache;
	if( m_dwTicks == 0 )
	{
		/*
		if( !PrecacheResources() )
		{
			ErrLog("VGame::Run(), call precache fail!");
			return false;
		}*/

		m_nLastTickTime = A3DCounter::GetMicroSecondNow();
	}

	if( !Tick() )
		return false;

	if( !Render() )
		return false;

	return true;
}

bool VGame::Run()
{
	// First do precache;
	if( !PrecacheResources() )
	{
		ErrLog("VGame::Run(), call precache fail!");
		return false;
	}

	m_nLastTickTime = A3DCounter::GetMicroSecondNow();

	while( true )
	{
		if( !Tick() )
			break;

		if( !Render() )
			break;

		if( !WindowMessageTick() )
			break;
	}

	ShowEnd();
	return true;
}

bool VGame::InitWindow()
{
	// If the game object is init as embedded, we will not need to create device window
	if( m_bEmbedded )
		return true;

	// First register the window;
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon		= LoadIcon(GetModuleHandle(NULL), _T("IDI_MYICON"));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= m_szClassName;
	wcex.hIconSm		= wcex.hIcon;

	RegisterClassEx(&wcex);

	// Then create the window
	if( GetSystemMetrics(SM_CXSCREEN) <= m_nWidth && GetSystemMetrics(SM_CYSCREEN) <= m_nHeight )
	{
		m_hDeviceWnd = CreateWindow(m_szClassName, m_szWndTitle, WS_POPUP,
			0, 0, m_nWidth, m_nHeight, NULL, NULL, m_hInstance, NULL);

		if( !m_hDeviceWnd )
		{
			m_pErrLog->Log("VGame::InitWindow(), Create window fail!");
			return false;
		}
	}
	else
	{
		m_hDeviceWnd = CreateWindow(m_szClassName, m_szWndTitle, WS_OVERLAPPEDWINDOW,
		  0, 0, m_nWidth, m_nHeight, NULL, NULL, m_hInstance, NULL);

		if( !m_hDeviceWnd )
		{
			m_pErrLog->Log("VGame::InitWindow(), Create window fail!");
			return false;
		}

		RECT rectWnd, rectClient;
							  
		GetWindowRect(m_hDeviceWnd, &rectWnd);
		GetClientRect(m_hDeviceWnd, &rectClient);

		// Resize the client area;
		SetWindowPos(m_hDeviceWnd, NULL, 0, 0, m_nWidth + rectWnd.right - rectWnd.left - rectClient.right + rectClient.left, 
		   m_nHeight + rectWnd.bottom - rectWnd.top - rectClient.bottom + rectClient.top, SWP_NOMOVE | SWP_NOZORDER);

		// Center the window on the screen;
		GetWindowRect(m_hDeviceWnd, &rectWnd);
		SetWindowPos(m_hDeviceWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - rectWnd.right + rectWnd.left) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - rectWnd.bottom + rectWnd.top) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	ShowWindow(m_hDeviceWnd, SW_SHOW);
	UpdateWindow(m_hDeviceWnd);

	return true;
}

bool VGame::InitEngine()
{
	AFileMod_Initialize();
	if( m_bPacked )
	{
		OpenFilePackage("scene.pck");
	}

	//Init Engine;	
	m_pA3DEngine = new A3DEngine();
	if( NULL == m_pA3DEngine )
	{
		m_pErrLog->Log("VGame::InitEngine(), new A3DEngine fail!");
		return false;
	}
	
	A3DDEVFMT devFmt;
	ZeroMemory(&devFmt, sizeof(devFmt));
	devFmt.bWindowed	= m_bWindowed;
	devFmt.nWidth		= m_nWidth;
	devFmt.nHeight		= m_nHeight;
	if( m_nDepth == 16 )
		devFmt.fmtTarget	= A3DFMT_X1R5G5B5;
	else
		devFmt.fmtTarget	= A3DFMT_X8R8G8B8;

	if( !m_pA3DEngine->Init(m_hInstance, m_hDeviceWnd, &devFmt, A3DDEVTYPE_HAL) )
	{
		m_pErrLog->Log("VGame::InitEngine(), call A3DEngine::Init() fail!");
		return false;
	}

	m_pA3DDevice = m_pA3DEngine->GetA3DDevice();

	//Set Render Parameters;
	m_pA3DCamera = new A3DCamera();
	if( NULL == m_pA3DCamera )
	{
		m_pErrLog->Log("VGame::InitEngine(), new A3DCamera fail!");
		return false;
	}

	if( !m_pA3DCamera->Init(m_pA3DDevice, DEG2RAD(60.0f), 2.0f, 10000.0f, m_nWidth * 1.0f / m_nHeight) )
	{
		m_pErrLog->Log("VGame::InitEngine(), call A3DCamera::Init() fail!");
		return false;
	}
						    
	m_pA3DCamera->SetPos(A3DVECTOR3(0.0f, 2.0f, -20.0f));
	m_pA3DCamera->SetDirAndUp(A3DVECTOR3(0.0f, 0.0f, 1.0f), A3DVECTOR3(0.0f, 1.0f, 0.0f));

	m_pA3DCamera->SetAM3DSoundDevice(m_pA3DEngine->GetAMSoundEngine()->GetAM3DSoundDevice());
	// Viewport;
	if( !m_pA3DDevice->CreateViewport(&m_pA3DViewport, 0, 0, m_nWidth, m_nHeight, 0.0f, 1.0f, true, true, A3DCOLORRGBA(0, 0, 0, 255)) )
		return false;

	m_pA3DViewport->SetCamera(m_pA3DCamera);
	m_pA3DEngine->AddViewport(m_pA3DViewport, NULL);

	// Light;
	m_pA3DEngine->GetA3DLightMan()->CreateDirecionalLight(1, &m_pA3DDirLight, A3DVECTOR3(0.0f, 0.0f, 0.0f),
		A3DVECTOR3(-1.0f, -0.2f, 1.0f), A3DCOLORVALUE(0.9f, 0.9f, 0.9f, 1.0f), A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f));
	m_pA3DDevice->SetAmbient(A3DCOLORRGB(60, 60, 60));

	m_pMsgQueue = new VMsgQueue();
	if( !m_pMsgQueue->Init() )
	{
		ErrLog("VGame::InitEngine(), Init the Message Queue fail!");
		return false;
	}
	return true;
}

// This is used for base class, so the derived class should not call this
bool VGame::InitWorld()
{
	m_pVWorld = new VWorld();

	if( !m_pVWorld->Init(this) )
	{
		ErrLog("VGame::InitWorld(), Init the world fail!");
		return false;
	}

	VKeyBoardDevice * pKeyBoard;
	pKeyBoard = new VKeyBoardDevice();

	if( !pKeyBoard->Init(m_hDeviceWnd, this, m_pMsgQueue) )
	{
		ErrLog("VGame::InitWorld(), Init the key board device fail!");
		return false;
	}

	m_pInputDevices[m_nInputDeviceNum ++] = pKeyBoard;

	VKeyBoardTranslator * pKeyTranslator;
	pKeyTranslator = new VKeyBoardTranslator();
	if( !pKeyTranslator->Init(this, m_pMsgQueue) )
		return false;

	pKeyBoard->ConnectTranslator(pKeyTranslator);
	
	return true;
}

bool VGame::ReleaseWorld()
{
	m_pInputDevices[0]->Release();
	delete m_pInputDevices[0];
	m_pInputDevices[0] = NULL;
	m_nInputDeviceNum = 0;

	if( m_pVWorld )
	{
		m_pVWorld->Release();

		delete m_pVWorld;
		m_pVWorld = NULL;
	}

	return true;
}

bool VGame::ReleaseEngine()
{
	if( m_pA3DDirLight )
	{
		m_pA3DDirLight->Release();
		delete m_pA3DDirLight;
		m_pA3DDirLight = NULL;
	}

	if( m_pA3DViewport )
	{
		m_pA3DViewport->Release();
		delete m_pA3DViewport;
		m_pA3DViewport = NULL;
	}

	if( m_pA3DCamera )
	{
		m_pA3DCamera->Release();
		delete m_pA3DCamera;
		m_pA3DCamera = NULL;
	}

	if( m_pA3DEngine )
	{
		m_pA3DEngine->Release();
		delete m_pA3DEngine;
		m_pA3DEngine = NULL;
	}

	if( m_bPacked )
	{
		CloseFilePackage();
	}
	AFileMod_Finalize();
	return true;
}

bool VGame::ReleaseWindow()
{
	if( m_bEmbedded )
		return true;

	if( m_hDeviceWnd )
	{
		DestroyWindow(m_hDeviceWnd);
		m_hDeviceWnd = NULL;
	}

	return true;
}

bool VGame::Tick()
{
	__int64 nTimeNow = A3DCounter::GetMicroSecondNow();

	if( nTimeNow - m_nLastTickTime > 33333 )
	{
		m_nLastTickTime = nTimeNow;
		m_pA3DEngine->TickAnimation();
		m_pA3DCamera->UpdateEar();
		if( m_pVWorld )
			m_pVWorld->TickAnimation();
	}

	// Tick input here;
	for(int i=0; i<m_nInputDeviceNum; i++)
	{
		if( m_pInputDevices[i] )
		{
			if( !m_pInputDevices[i]->Tick(m_pVWorld->GetTickTimeSpan()) )
			{
				ErrLog("VGame::Tick(), Call input device tick fail!");
				return false;
			}				
		}
	}

	// Process the game message
	if( !GameMessageProcess() )
		return false;

	if( m_pVWorld )
		m_pVWorld->Tick();

	m_dwTicks ++;
	return true;
}

bool VGame::RenderContent(A3DViewport * pCurrentViewport)
{
	if( !m_pVWorld->RenderBeforeEngineScene(pCurrentViewport) )
		return false;

	if( !m_pA3DEngine->RenderScene(pCurrentViewport) )
		return false;

	if( !m_pVWorld->RenderAfterEngineScene(pCurrentViewport) )
		return false;

	return true;
}

bool VGame::Render()
{
	if( !m_pA3DEngine->BeginRender() )
		return false;

	if( !m_pA3DViewport->Active() )
		return false;

	if( !m_pA3DViewport->ClearDevice() )
		return false;

	if( !RenderContent(m_pA3DViewport) ) 
		return false;

	if( !m_pA3DEngine->EndRender() )
		return false;

	if( !m_pA3DEngine->Present() )
		return false;

	return true;
}

bool VGame::ResizeDevice(int nWidth, int nHeight)
{
	if( !m_pA3DEngine || !m_pA3DViewport || !m_bWindowed )
		return true;

	A3DVIEWPORTPARAM param;
	param.X = 0;
	param.Y = 0;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	param.Width = nWidth;
	param.Height = nHeight;
	m_pA3DViewport->SetParam(&param);
	m_pA3DEngine->SetDisplayMode(nWidth, nHeight, (A3DFORMAT) NULL, 0, SDM_WIDTH | SDM_HEIGHT);
	m_pA3DCamera->SetProjectionParam(0.0f, 0.0f, 0.0f, m_nWidth * 1.0f / m_nHeight);

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	if( m_pVWorld )
	{
		GAMEMSG msg;
		msg.dwMsgID = GAMEMSG_RESIZE;
		m_pVWorld->OnMsg(msg, NULL);
	}
	return true;
}

bool VGame::WindowMessageTick()
{
	MSG		msg;
	while( PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
	{
		GetMessage(&msg, NULL, NULL, NULL);
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

bool VGame::GameMessageProcess()
{
	if( !m_pMsgQueue )
		return true;

	GAMEMSG		msg;
	while( m_pMsgQueue->MsgQueueOut(&msg) )
	{
		if( !OnMsg(msg, 0) )
			return false;
	}

	return true;
}

bool VGame::SetCameraPos(const A3DVECTOR3& vecPos)
{
	m_pA3DCamera->SetPos(vecPos);

	return true;
}

bool VGame::SetCameraDirAndUp(const A3DVECTOR3& vecDir, const A3DVECTOR3& vecUp)
{
	m_pA3DCamera->SetDirAndUp(vecDir, vecUp);

	return true;
}
		 
LRESULT CALLBACK VGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) 
	{
		case WM_PAINT:
			HDC				hdc;
			PAINTSTRUCT		ps;
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;	

		case WM_ACTIVATEAPP:
			if( wParam )
				g_pGame->OnActivateApp(true);
			else
				g_pGame->OnActivateApp(false);
			break;

		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
			break;

		case WM_SETCURSOR:
			if( 1 )
				SetCursor(NULL);
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
			break;

		case WM_SIZE:
			if( wParam != SIZE_MINIMIZED && g_pGame )
			{
				g_pGame->ResizeDevice(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void VGame::ErrLog(char * szFmt, ...)
{
	if( m_pErrLog )
	{
		char buffer[2048];
		vsprintf(buffer, szFmt, (char*) ((&szFmt) + 1));
		m_pErrLog->Log("%s", buffer);
	}
}

bool VGame::SetAmbient(BYTE red, BYTE green, BYTE blue)
{
	m_pA3DDevice->SetAmbient(A3DCOLORRGB(red, green, blue));

	return true;
}

bool VGame::SetDirectionalColor(BYTE red, BYTE green, BYTE blue)
{
	A3DLIGHTPARAM lightParam;
	lightParam = m_pA3DDirLight->GetLightparam();
	lightParam.Diffuse = ColorRGBAToColorValue(A3DCOLORRGB(red, green, blue));
	m_pA3DDirLight->SetLightParam(&lightParam);

	return true;
}

bool VGame::SetDirectionalDir(A3DVECTOR3 vecDir)
{
	A3DLIGHTPARAM lightParam;
	lightParam = m_pA3DDirLight->GetLightparam();
	lightParam.Direction = vecDir;
	m_pA3DDirLight->SetLightParam(&lightParam);

	return true;
}

bool VGame::SetFogColor(BYTE red, BYTE green, BYTE blue)
{
	m_pA3DDevice->SetFogColor(A3DCOLORRGB(red, green, blue));

	return true;
}

bool VGame::SetFogStartAndEnd(float vStart, float vEnd)
{
	m_pA3DDevice->SetFogStart(vStart);
	m_pA3DDevice->SetFogEnd(vEnd);

	return true;
}

bool VGame::PrecacheResources()
{
	if( !m_pA3DEngine->PrecacheAllTextures(m_pA3DViewport) )
		return false;

	return true;
}

bool VGame::OnMsg(const GAMEMSG& msg, DWORD dwUserData)
{
	// First filter some game defined message;
	switch(msg.dwMsgID)
	{
	case GAMEMSG_EXIT:
		ErrLog("VGame::OnMsg(), Receive 'GAMEMSG_EXIT', now to exit...!");
		return false;

	case GAMEMSG_SHOWCURSOR:
		if( m_bAutoHideCursor )
			m_pA3DDevice->ShowCursor(true);
		break;

	case GAMEMSG_HIDECURSOR:
		if( m_bAutoHideCursor )
			m_pA3DDevice->ShowCursor(false);
		break;
	}

	if( m_pVWorld )
	{
		return m_pVWorld->OnMsg(msg, dwUserData);
	}

	return true;
}

bool VGame::OnActivateApp(bool bActive)
{
	m_bActive = bActive;

	for(int i=0; i<m_nInputDeviceNum; i++)
	{
		if( m_pInputDevices[i] )
			m_pInputDevices[i]->SetActive(bActive);
	}

	return true;
}

bool VGame::ShowSplash()
{
	return true;
}

bool VGame::ShowEnd()
{
	return true;
}

bool VGame::SetCursorType(int nType)
{
	return true;
}