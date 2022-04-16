/*
-------------------Window Console-------------------
-----------------------main.cpp-----------------------
--For Testing Console Class
*/
#include "../Source/Console.h"

HWND hwnd = NULL;
const char* WndClassName = "WindowConsoleProgramTest";
const int Width  = 800;
const int Height = 600;

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int MessageLoop();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nShowCmd)
{
	if(InitializeWindow(hInstance, nShowCmd, Width, Height, true) == false)
	{
		MessageBoxA(NULL, "Window Initialization - Failed", "Error", MB_OK | MB_ICONERROR);
		Sleep(600);
		exit(-1);
	}

	//Testing Console
	MafiaBar::SDK::console.CreateConsole(0, (Height / 2) + 20, Width - 15, Height / 2, hwnd);
	MafiaBar::SDK::console << "Hi" << MafiaBar::endl << "HelloWorld" << MafiaBar::endl << "Yes" << MafiaBar::endl << "Haha" << MafiaBar::endl;

	MessageLoop();

	return 0;
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;	
	wc.hInstance = hInstance;	
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassExA(&wc))
	{
		MessageBoxA(NULL, "Error registering window", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowExA(
		NULL,
		WndClassName,
		"Window Title",	
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (hwnd == NULL)	
	{
		MessageBoxA(NULL, "Error creating window","Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hwnd, ShowWnd);

	return true;
}

int MessageLoop()
{
	MSG msg;
	while(true)	
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
			if (msg.message == WM_QUIT) { break; }
            TranslateMessage(&msg);	
            DispatchMessage(&msg);
        }
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(std::get<0>(MafiaBar::SDK::console.GetColors()), std::get<1>(MafiaBar::SDK::console.GetColors()), std::get<2>(MafiaBar::SDK::console.GetColors())));
			return (LRESULT)GetStockObject(BLACK_BRUSH);
		}
		case WM_SIZE:
		{
			MoveWindow(MafiaBar::SDK::console.GetConsoleHandle(), 0, (HIWORD(lParam) / 2) + 20, LOWORD(lParam) - 2, HIWORD(lParam) / 2, TRUE);
			MafiaBar::SDK::console.ShowConsole();
			break;
		}

	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}
