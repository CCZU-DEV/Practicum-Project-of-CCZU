// Win_learn.cpp : Defines the entry point for the application.
//

#include "StdAfx.h"
#include "resource.h"
#include <math.h>

#define MAX_LOADSTRING 100

//��������
#define PI (acos(-1.0))
const double n=90.0;
const int gridsize=12;//�����С

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

//��������
POINT apt[7200];//����ɢ��
int left,right,pos;
int cxClient,cyClient,midyClient;//���洰�ڴ�С
int sinsize;//���Һ�������
HPEN GreenPen,gridPen;//��ɫ����

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Set(HWND, UINT, WPARAM, LPARAM);

//��������
void init(HWND hWnd);//�Զ���ĳ�ʼ������
void next();//���������Ӳ����㵱ǰ����λ��

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN_LEARN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	DialogBox(hInst, (LPCTSTR)IDD_Set, NULL, (DLGPROC)Set);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WIN_LEARN);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
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
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WIN_LEARN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= CreateSolidBrush(RGB(0,0,0));//ʹ�ú�ɫ����
	wcex.lpszMenuName	= (LPCSTR)IDC_WIN_LEARN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, cxClient, cyClient, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   init(hWnd);//�Զ���ĳ�ʼ������


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
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
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	int xpos,ypos;

	short Delta;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_TIMER:
			//��ʱ����Ϣ�����ݲ�ͬ��ʱ����Ϣ��������
			switch (wParam)
			{
			case 1:
				next();//ѹ��������
				break;
			case 2:
				InvalidateRect(hWnd,NULL,TRUE);//ˢ�½���
				break;
			}
			break;
		case WM_MOUSEWHEEL:
			//����м�����
			Delta = HIWORD(wParam);
			if (Delta>0)
			{
				//���ϻ�������м������Һ������ȱ��
				while (Delta>0)
				{
					sinsize+=10;
					Delta-=120;
					if (sinsize>midyClient) sinsize=midyClient;
				}
			}
			else if (Delta<0)
			{
				//���»�������м������Һ������ȱ�С
				while (Delta<0)
				{
					sinsize-=10;
					Delta+=120;
					if (sinsize<1) sinsize=1;
				}
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			//��������
			SelectObject (hdc,gridPen);
			for (xpos=pos;xpos<=cxClient;xpos+=gridsize)
			{
				MoveToEx(hdc,xpos,0,NULL);
				LineTo(hdc,xpos,cyClient);
			}
			for (ypos=0;ypos<=cyClient;ypos+=gridsize)
			{
				MoveToEx(hdc,0,ypos,NULL);
				LineTo(hdc,cxClient,ypos);
			}
			//ʹ����ɫ����
			SelectObject (hdc,GreenPen);
			//����ɢ��������Һ���
			Polyline (hdc,apt+left,right-left);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

LRESULT CALLBACK Set(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[100];
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
			{
				
				GetDlgItemText(hDlg,IDC_EDIT_H,str,100);
				cyClient=atoi(str);
				GetDlgItemText(hDlg,IDC_EDIT_W,str,100);
				cxClient=atoi(str);
			}
			if (LOWORD(wParam) == IDOK )//|| LOWORD(wParam) == IDCANCEL) 
			{
				
				EndDialog(hDlg,LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

void init(HWND hWnd)
{
	int i;
	midyClient=cyClient/2;//��������м�λ��
	//��ʼ������λ��
	pos=gridsize-cxClient%gridsize;
	//��ʼ�����Һ�������
	sinsize=100;
	//��ʼ������λ��
	left=3600-1-cxClient;right=3600;
	for (i=left;i<right;i++)
	{
		apt[i].x=i-left;
		apt[i].y=midyClient;
	}
	next();
	//��ʼ������
	GreenPen=CreatePen (PS_SOLID, 1, RGB (0, 255, 0)) ;
	gridPen=CreatePen (PS_SOLID, 1, RGB (32, 128, 32)) ;
	//���ü�ʱ��
	SetTimer(hWnd,1,10,NULL);
	SetTimer(hWnd,2,10,NULL);
}

void next()
{
	int i;
	if (right==7200)
	{
		for (i=left;i<right;i++)
		{
			apt[i-left]=apt[i];
		}
		right=right-left;
		left=0;
	}
	//ѹ��������
	apt[right].x=cxClient+1;
	apt[right].y=(int)(midyClient-sinsize*sin(PI*right/n));
	left++;right++;
	//�������ӵ�ǰλ��
	for (i=left;i<right;i++) apt[i].x--;
	//��������ǰλ��
	pos--;
	if (pos==0)
	{
		pos=gridsize-cxClient%gridsize;
	}
}