// WinStep1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include<stdio.h>
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
struct Customer
{
	char Name[32];
	int Month,Day;
};
char Star[][7]={"Ħ����","ˮƿ��","˫����","������","��ţ��","˫����","��з��","ʨ����","��Ů��","�����","��Ы��","������"};
char Month[][7]={"һ��","����","����","����","����","����","����","����","����","ʮ��","ʮһ��","ʮ����"};
struct Customer MyCustomer;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_WINSTEP1, szWindowClass, MAX_LOADSTRING);
	strcpy(szTitle,"�ҵ�Ϳѻ����");
	strcpy(szWindowClass,"��ó121�ෲ��");
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINSTEP1);

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

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WINSTEP1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_WINSTEP1;
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

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
LRESULT CALLBACK InputCustomer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			int i;
			for(i=0;i<12;i++)
				SendDlgItemMessage(hDlg,IDC_MONTH,CB_ADDSTRING,0,(LPARAM)Month[i]);
			SetDlgItemText(hDlg,IDC_NAME,MyCustomer.Name);
			SendDlgItemMessage(hDlg,IDC_MONTH,CB_SETCURSEL,MyCustomer.Month-1,0);
			SetDlgItemInt(hDlg,IDC_DAY,MyCustomer.Day,FALSE);
		}
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK)
			{
				GetDlgItemText(hDlg,IDC_NAME,MyCustomer.Name,32);
				MyCustomer.Month=SendDlgItemMessage(hDlg,IDC_MONTH,CB_SETCURSEL,0,0)+1;
				MyCustomer.Day=GetDlgItemInt(hDlg,IDC_DAY,NULL,FALSE);
			}
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
	case WM_LBUTTONDBLCLK:
		if(DialogBox(hInst,(LPCTSTR)IDD_INPUT_CUSTOMER,hWnd,(DLGPROC)InputCustomer)==IDOK)
			InvalidateRect(hWnd,NULL,TRUE);
		break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_SHOW_TIME:
				{
					char tms[20];
					SYSTEMTIME tm;
					GetLocalTime(&tm);
					sprintf(tms,"������%dʱ%d��",tm.wHour,tm.wMinute);
					MessageBox(hWnd,tms,"������",MB_OK);
					return 0;
				}
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
		case WM_PAINT:
			{
			     hdc = BeginPaint(hWnd, &ps);
			     /*{char *p1="���Ͼ������£���������׽�̸Ц���軹��";
			     char *p2="���������£�ֻҪ�ϵ��ʡ�";
			     char *p3="�������������죬�����";
				 RECT    r1={10,10,1024,90};
			     HFONT   hfont=CreateFont(48,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
				     DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,
				     "��̴իë������");
			     HGDIOBJ  oldf=SelectObject(hdc,hfont);
			     HPEN   hpen=CreatePen(PS_DASH,1,RGB(75,110,12));
			     HGDIOBJ oldp=SelectObject(hdc,hpen);
			     COLORREF  oldColor=SetTextColor(hdc,RGB(127,0,0));
			     Rectangle(hdc,r1.left,r1.top,r1.right,r1.bottom);
			     DrawText(hdc,p1,-1,&r1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			     r1.top+=100,r1.bottom+=100;Rectangle(hdc,r1.left,r1.top,r1.right,r1.bottom);
			     DrawText(hdc,p2,-1,&r1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			     r1.top+=100,r1.bottom+=100;Rectangle(hdc,r1.left,r1.top,r1.right,r1.bottom);
			     DrawText(hdc,p3,-1,&r1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			     SelectObject(hdc,oldf);
			     SelectObject(hdc,oldp);
			     DeleteObject(hfont);
				 DeleteObject(hpen);}*/
           if(MyCustomer.Month>0)
			{
				RECT  r1={10,10,1024,90};
				char  Name[128];
				int i;
				sprintf(Name,"��ӭ����%s���ѣ�����������",MyCustomer.Name);
				i=MyCustomer.Month-1;
				if(MyCustomer.Day>19)
					i++;
				if(i>=12)
					i=0;
				HFONT   hfont=CreateFont(80,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
				DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,
				"Ҷ���Ѽ������");
				HGDIOBJ  oldf=SelectObject(hdc,hfont);
				DrawText(hdc,Name,-1,&r1,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
				HFONT   hfont2=CreateFont(180,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
				DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,
				"Ҷ���Ѽ������");
				SelectObject(hdc,hfont2);
				r1.top+=100,r1.bottom+=300;
				DrawText(hdc,Star[i],-1,&r1,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
				SelectObject(hdc,oldf);
				DeleteObject(hfont2);
				DeleteObject(hfont);
			}
			     EndPaint(hWnd, &ps);
			}
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
