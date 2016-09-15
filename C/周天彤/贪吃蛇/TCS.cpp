// TCS.cpp : Defines the entry point for the application.
//

#include "StdAfx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

//��������
int map[100][100];//��ͼ
int headx,heady;//��ͷλ��
int dir;//�ߵ��˶�����
int game;//��Ϸ������־
int succ;//��Ϸʤ����־

int tmp=0;

//��������
void init(HWND hWnd);//��ʼ����Ϸ
void go();//���ж�һ��
void apple();//������ƻ��
void draw(HWND hWnd);//��ͼ
void play(HWND hWnd,HDC hdc);//��ʾ��Ϸ���
void paint(HWND hWnd,HDC hdc);//������Ϸ����


// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_TCS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TCS);

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TCS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TCS;
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
      CW_USEDEFAULT, 0, 616, 658, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   //��Ϸ��ʼ��
   init(hWnd);

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
				go();//���ж�һ��
				InvalidateRect(hWnd,NULL,TRUE);//ˢ�½���
				break;
			case 2:
				apple();//������ƻ��
				InvalidateRect(hWnd,NULL,TRUE);
				break;
			}
			break;
		case WM_KEYDOWN:
			//��ȡ���̰�����ֵ
			if (wParam==VK_F5)
			{
				//F5�����¿�ʼ
				KillTimer(hWnd,1);
				KillTimer(hWnd,2);
				init(hWnd);
				InvalidateRect(hWnd,NULL,TRUE);
			}
			if (wParam==VK_RIGHT)
			{
				//�Ҽ�ͷ���޸ķ���
				if (dir==2) break;
				dir=0;
				InvalidateRect(hWnd,NULL,TRUE);
			}
			if (wParam==VK_DOWN)
			{
				//�¼�ͷ���޸ķ���
				if (dir==3) break;
				dir=1;
				InvalidateRect(hWnd,NULL,TRUE);
			}
			if (wParam==VK_LEFT)
			{
				//���ͷ���޸ķ���
				if (dir==0) break;
				dir=2;
				InvalidateRect(hWnd,NULL,TRUE);
			}
			if (wParam==VK_UP)
			{
				//�ϼ�ͷ���޸ķ���
				if (dir==1) break;
				dir=3;
				InvalidateRect(hWnd,NULL,TRUE);
			}
			break;
		case WM_PAINT:
			draw(hWnd);
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

void init(HWND hWnd)//��ʼ����Ϸ
{
	int i,j,k;

	//��ʼ����ͼ���ѵ�ͼ��������Ϊǽ������ط�Ϊ�յء�
	for (i=0;i<100;i++)
	{
		for (j=0;j<100;j++)
		{
			if (i==0||j==0||i==99||j==99) map[i][j]=-1;
			else map[i][j]=0;
		}
	}

	//����ڵ�ͼ�м������ͷλ��
	headx=rand()%50+24;
	heady=rand()%50+24;

	//��ʼ��������ߵ��˶�����
	dir=0;
	map[headx][heady]=3;
	map[headx-1][heady]=2;
	map[headx-2][heady]=1;

	//���������ͼ�м��ǽ
	/*
	for (k=0;k<10;k++)
	{
		i=0;j=0;
		while (map[i][j]!=0)
		{
			i=rand()%98+1;
			j=rand()%98+1;
		}
		map[i][j]=-1;
	}
	*/

	//�������ƻ��
	for (k=0;k<20;k++)
	{
		i=0;j=0;
		while (map[i][j]!=0)
		{
			i=rand()%98+1;
			j=rand()%98+1;
		}
		map[i][j]=-2;
	}

	//���ü�ʱ��
	SetTimer(hWnd,1,200,NULL);
	SetTimer(hWnd,2,20000,NULL);

	//����Ϸ��־����Ϊ��ʼ
	game=1;
}

void go()//���ж�һ��
{
	tmp=tmp+6;
	int i,j;
	int nextx,nexty;//��������һ����λ��

	//��������һ����λ��
	if (dir==0) 
	{
		nextx=headx+1;
		nexty=heady;
	}
	if (dir==1) 
	{
		nextx=headx;
		nexty=heady+1;
	}
	if (dir==2) 
	{
		nextx=headx-1;
		nexty=heady;
	}
	if (dir==3) 
	{
		nextx=headx;
		nexty=heady-1;
	}

	//��������һ������λ�ý����ж�
	if (map[nextx][nexty]>0)
	{
		//ײ���Լ�����Ϸ����
		//����Ϸ��־����Ϊ����
		game=0;
		//��Ϸʧ��
		succ=0;
	}
	if (map[nextx][nexty]==0)
	{
		//û���κζ���������ǰ��һ��
		for (i=1;i<99;i++)
			for (j=1;j<99;j++)
				if (map[i][j]>0) map[i][j]--;
		map[nextx][nexty]=map[headx][heady]+1;
		
		//������ͷλ��
		headx=nextx;
		heady=nexty;
	}
	if (map[nextx][nexty]==-1)
	{
		//����ǽʱ������Ϊһ����ײǽ�󳤶�Ϊ�㣬��Ϸʧ��
		if (map[headx][heady]==1)
		{
			game=0;
			succ=0;
		}
		//����ǽ�����ȼ�һ
		for (i=1;i<99;i++)
			for (j=1;j<99;j++)
				if (map[i][j]>0) map[i][j]--;
		
	}
	if (map[nextx][nexty]==-2)
	{
		//�Ե�ƻ��
		map[nextx][nexty]=map[headx][heady]+1;

		//������ͷλ��
		headx=nextx;
		heady=nexty;

		if (map[headx][heady]>=50)
		{
			//����Ϸ��־����Ϊ����
			game=0;
			//��Ϸʤ��
			succ=1;
		}
	}
}
void apple()//������ƻ��
{
	int i,j;

	i=0;j=0;
	while (map[i][j]!=0)
	{
		i=rand()%98+1;
		j=rand()%98+1;
	}
	map[i][j]=-2;
}

void draw(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);

	//��Ϸ����
	if (game==0)
	{
		play(hWnd,hdc);
	}

	//��Ϸ������
	if (game==1)
	{
		paint(hWnd,hdc);
	}

	EndPaint(hWnd, &ps);
}

void play(HWND hWnd,HDC hdc)//��ʾ��Ϸ���
{
	//��Ϸ�����������Ϸ���
	if (succ==0) TextOut(hdc,260,250,"Game  Over",10);
	if (succ==1) TextOut(hdc,270,250,"You  Win",8);
}

void paint(HWND hWnd,HDC hdc)//������Ϸ����
{
	int i,j;

	RECT rect; 
	//���������ɫ����
	HBRUSH black,red,blue,green;
	black=CreateSolidBrush(RGB(0,0,0));
	red=CreateSolidBrush(RGB(255,0,0));
	blue=CreateSolidBrush(RGB(0,0,255));
	green=CreateSolidBrush(RGB(0,255,0));

	//����map��ͼ���л���
	for (i=0;i<100;i++)
		for (j=0;j<100;j++)
		{
			SetRect(&rect,i*6,j*6,i*6+6,j*6+6);
			if (map[i][j]>0)
			{
				FillRect(hdc,&rect,green);
			}
			if (map[i][j]==-1)
			{
				FillRect(hdc,&rect,red);
			}
			if (map[i][j]==-2)
			{
				FillRect(hdc,&rect,blue);
			}
		}

	//������
	if (dir==0) SetRect(&rect,headx*6+2,heady*6+2,headx*6+9,heady*6+3);
	if (dir==1) SetRect(&rect,headx*6+2,heady*6+2,headx*6+3,heady*6+9);
	if (dir==2) SetRect(&rect,headx*6-3,heady*6+2,headx*6+3,heady*6+3);
	if (dir==3) SetRect(&rect,headx*6+2,heady*6-3,headx*6+3,heady*6+3);
	FillRect(hdc,&rect,red);
}
