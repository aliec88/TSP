// ImproveTSP.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ImproveTSP.h"
#include "GATsp.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
CGATsp* g_tsp;

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMPROVETSP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMPROVETSP));

	// Main message loop:
	bool bDone=true;
	while(bDone)
	{
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				bDone=false;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (g_tsp->Started())
		{
			g_tsp->Epoch();
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
		}
	}
	delete g_tsp;
	UnregisterClass(szWindowClass,hInstance);
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMPROVETSP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IMPROVETSP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      GetSystemMetrics(SM_CXSCREEN)/2-WINDOW_WIDTH/2, GetSystemMetrics(SM_CYSCREEN)/2-WINDOW_HEIGHT/2,
	  WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static int xClient;
	static int yClinet;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;
	static HMENU hMenu;
	switch (message)
	{
	case  WM_CREATE:
		{
			srand((unsigned)time(NULL));
			RECT rect;
			GetClientRect(hWnd,&rect);
			xClient=rect.right;
			yClinet=rect.bottom;
			hdcBackBuffer=CreateCompatibleDC(NULL);
			HDC dc=GetDC(hWnd);
			hBitmap=CreateCompatibleBitmap(dc,xClient,yClinet);
			hOldBitmap=(HBITMAP)SelectObject(hdcBackBuffer,hBitmap);
			ReleaseDC(hWnd,dc);
			g_tsp=new CGATsp();
			hMenu=GetMenu(hWnd);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_CRO_PMX:
			g_tsp->SetCrossoverType(PMX);
			CheckMenuRadioItem(hMenu,IDM_CRO_PMX,IDM_CRO_PBX,IDM_CRO_PMX,MF_BYCOMMAND);
			break;
		case IDM_CRO_OBX:
			g_tsp->SetCrossoverType(OBX);
			CheckMenuRadioItem(hMenu,IDM_CRO_PMX,IDM_CRO_PBX,IDM_CRO_OBX,MF_BYCOMMAND);
			break;
		case IDM_CRO_PBX:
			g_tsp->SetCrossoverType(PBX);
			CheckMenuRadioItem(hMenu,IDM_CRO_PMX,IDM_CRO_PBX,IDM_CRO_PBX,MF_BYCOMMAND);
			break;
		case IDM_MUTATE_DM:
			g_tsp->SetMutationType(DM);
			CheckMenuRadioItem(hMenu,IDM_MUTATE_EM,IDM_MUTATE_DM,IDM_MUTATE_DM,MF_BYCOMMAND);
			break;
		case IDM_MUTATE_EM:
			g_tsp->SetMutationType(EM);
			CheckMenuRadioItem(hMenu,IDM_MUTATE_EM,IDM_MUTATE_DM,IDM_MUTATE_EM,MF_BYCOMMAND);
			break;
		case IDM_MUTATE_IM:
			g_tsp->SetMutationType(IM);
			CheckMenuRadioItem(hMenu,IDM_MUTATE_EM,IDM_MUTATE_DM,IDM_MUTATE_IM,MF_BYCOMMAND);
			break;
		case IDM_MUTATE_SM:
			g_tsp->SetMutationType(SM);
			CheckMenuRadioItem(hMenu,IDM_MUTATE_EM,IDM_MUTATE_DM,IDM_MUTATE_SM,MF_BYCOMMAND);
			break;
		case IDM_SCALE_NONE:
			g_tsp->SetScaleType(NONE);
			CheckMenuRadioItem(hMenu,IDM_SCALE_NONE,IDM_SCALE_BOLTZMAN,IDM_SCALE_NONE,MF_BYCOMMAND);
			break;
		case IDM_SCALE_RANK:			
			g_tsp->SetScaleType(RANK);
			CheckMenuRadioItem(hMenu,IDM_SCALE_NONE,IDM_SCALE_BOLTZMAN,IDM_SCALE_RANK,MF_BYCOMMAND);
			break;
		case IDM_SCAL_SIGMA:			
			g_tsp->SetScaleType(SIGMA);
			CheckMenuRadioItem(hMenu,IDM_SCALE_NONE,IDM_SCALE_BOLTZMAN,IDM_SCAL_SIGMA,MF_BYCOMMAND);
			break;
		case IDM_SCALE_BOLTZMAN:			
			g_tsp->SetScaleType(BOLTZMAN);
			CheckMenuRadioItem(hMenu,IDM_SCALE_NONE,IDM_SCALE_BOLTZMAN,IDM_SCALE_BOLTZMAN,MF_BYCOMMAND);
			break;
		case IDM_SELECT_ROULETTE:
			g_tsp->SetSus(false);
			CheckMenuRadioItem(hMenu,IDM_SUS_ON,IDM_SUS_OFF,IDM_SUS_OFF,MF_BYCOMMAND);
			g_tsp->SetSelectType(ROULETTE);
			CheckMenuRadioItem(hMenu,IDM_SELECT_ROULETTE,IDM_SELECT_SUS,IDM_SELECT_ROULETTE,MF_BYCOMMAND);
			break;
		case IDM_SELECT_TOURNAMENT:
			g_tsp->SetSus(false);
			CheckMenuRadioItem(hMenu,IDM_SUS_ON,IDM_SUS_OFF,IDM_SUS_OFF,MF_BYCOMMAND);
			g_tsp->SetSelectType(TOURNAMENT);
			CheckMenuRadioItem(hMenu,IDM_SELECT_ROULETTE,IDM_SELECT_SUS,IDM_SELECT_TOURNAMENT,MF_BYCOMMAND);
			break;
		case IDM_SELECT_ALTTOURNAMENT:
			g_tsp->SetSus(false);
			CheckMenuRadioItem(hMenu,IDM_SUS_ON,IDM_SUS_OFF,IDM_SUS_OFF,MF_BYCOMMAND);
			g_tsp->SetSelectType(ALT_TOURNAMENT);
			CheckMenuRadioItem(hMenu,IDM_SELECT_ROULETTE,IDM_SELECT_SUS,IDM_SELECT_ALTTOURNAMENT,MF_BYCOMMAND);
			break;
		case IDM_SUS_ON:
			g_tsp->SetSus(true);
			CheckMenuRadioItem(hMenu,IDM_SUS_ON,IDM_SUS_OFF,IDM_SUS_ON,MF_BYCOMMAND);
			CheckMenuRadioItem(hMenu,IDM_SELECT_ROULETTE,IDM_SELECT_SUS,IDM_SELECT_SUS,MF_BYCOMMAND);
			break;
		case IDM_SUS_OFF:
			g_tsp->SetSus(false);
			CheckMenuRadioItem(hMenu,IDM_SUS_ON,IDM_SUS_OFF,IDM_SUS_OFF,MF_BYCOMMAND);
			g_tsp->SetSelectType(TOURNAMENT);
			CheckMenuRadioItem(hMenu,IDM_SELECT_ROULETTE,IDM_SELECT_SUS,IDM_SELECT_TOURNAMENT,MF_BYCOMMAND);
		case IDM_ELITISM_OFF:
			g_tsp->SetElitism(false);
			CheckMenuRadioItem(hMenu,IDM_ELITISM_ON,IDM_ELITISM_OFF,IDM_ELITISM_OFF,MF_BYCOMMAND);
			break;
		case IDM_ELITISM_ON:
			g_tsp->SetElitism(true);
			CheckMenuRadioItem(hMenu,IDM_ELITISM_ON,IDM_ELITISM_OFF,IDM_ELITISM_ON,MF_BYCOMMAND);
			break;
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdcBackBuffer,0,0,xClient,yClinet,NULL,0,0,WHITENESS);
		g_tsp->Rander(hdcBackBuffer,xClient,yClinet);
		BitBlt(hdc,0,0,xClient,yClinet,hdcBackBuffer,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_RETURN:
				{					
					g_tsp->Run();
				}
				break;
			case VK_SPACE:
				{
					g_tsp->Stop();
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			SelectObject(hdcBackBuffer,hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(hdcBackBuffer);
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
