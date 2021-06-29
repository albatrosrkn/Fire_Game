#include <windows.h>  
#include "chmat.h"
#include <stdio.h>
#include <thread>
#include <iostream>

#define IDI_ICON1 101
#define IDC_OPEN	3000
#define IDC_BUTTON1 3001
#define IDC_BUTTON2 3002
#define IDC_BUTTON3 3003

HINSTANCE hInst;   // current instance
HWND     hWnd, hEdit,HButton1, HButton2, HButton3, Hmainbmp;    //parent window
int keypressed;

LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "GENERIC32";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		900, 600,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);
	

	if (!hWnd)
		return(FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}
//   !!!!!!!!!!!!!!!!!!!Global definition makes it easier!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
CHMAT m(300, 300, CHMAT_INT);
HANDLE hTimerQueue = NULL;
int boxlocation = 10, shoot = 103, count = 0, sides = 0, shootx = 0;
VOID CALLBACK SlidingBox(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	for (int i = 0; i < 10; i++)
	{
		m(boxlocation + i, 100) = 0;
		m(boxlocation + i, 101) = 0;
		m(boxlocation + i, 102) = 0;
		m(boxlocation + i, 103) = 0;
		m(boxlocation + i, 104) = 0;
		m(boxlocation + i, 105) = 0;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
	switch(keypressed)
	{
	case 37: {
		sides = 1;
		boxlocation -= 2;
		break;
	}
	case 39: {
		sides = 2;
		boxlocation += 2;
		break;
	}

	case 32: {
		
		if (sides == 1) boxlocation -= 2;
		if (sides == 2) boxlocation += 2;
		if (count == 0) shootx = boxlocation;
		count = 1;
			 break;
	}
	default: count=0;
	}
	//Bound Check
	if (boxlocation > 300 || boxlocation<0) {
		SetWindowText(hEdit, "Out of bounds");
  		exit(-1);
	}
	for (int i = 0; i < 10; i++)
	{
		m(boxlocation + i, 100) = 0xffffff;
		m(boxlocation + i, 101) = 0xffffff;
		m(boxlocation + i, 102) = 0xffffff;
		m(boxlocation + i, 103) = 0xffffff;
		m(boxlocation + i, 104) = 0xffffff;
		m(boxlocation + i, 105) = 0xffffff;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
	for (int i = 0; i < 4; i++)
	{
		if (count == 0) break;
		if (shoot > 299)
		{
			shoot = 103;
			count = 0;
			break;
		}
		m(shootx, shoot + i) = 0;
		m(shootx, shoot + i) = 0;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
	if (count == 1) shoot += 3;
	for (int i = 0; i < 4; i++)
	{
		if (count == 0) break;
		if (shoot > 299)
		{
			shoot = 102;
			count = 0;
			break;
		}
		m(shootx, shoot + i) = 0xffffff;
		m(shootx, shoot + i) = 0xffffff;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		
	case WM_CREATE:
	{
		
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", "", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 735, 5, 150, 100, hWnd, NULL, hInst, NULL);
		HButton1 = CreateWindowEx(NULL, "BUTTON", "START", WS_CHILD | WS_VISIBLE | SS_CENTER,
			2, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		HButton2 = CreateWindowEx(NULL, "BUTTON", "BUTTON2", WS_CHILD | WS_VISIBLE | SS_CENTER,
			82, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		HButton3 = CreateWindowEx(NULL, "BUTTON", "STOP", WS_CHILD | WS_VISIBLE | SS_CENTER,
			162, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON3, hInst, NULL);
		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 600, 500, hWnd, NULL, hInst, NULL);
		
	}
	break;
	case WM_KEYDOWN:
	{
		keypressed = (int)wParam;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDC_OPEN:
		{
			OPENFILENAME ofn;

			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
		}
		break;
		case IDC_BUTTON1:
		{
			//Starting Timer
			HANDLE hTimer = NULL;
			
			hTimerQueue = CreateTimerQueue();
			if (NULL == hTimerQueue)
			{
				break;
			}
			//starts 1000 ms later calls thread every 30 m
			CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)SlidingBox, NULL, 1000, 30, 0);
		}
		break;
		case IDC_BUTTON2:
		{
			
			m = 0xff00;
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			SetWindowText(hEdit, "Green matrix created!");

		}
		break;
		case IDC_BUTTON3:
		{
			//deletes timer
			DeleteTimerQueue(hTimerQueue);
		}
		break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}

