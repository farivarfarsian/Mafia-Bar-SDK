#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tuple>
#include <string>

namespace MafiaBar
{
	namespace SDK
	{
		class Console
		{
		public:
			Console() = default;
			Console(const Console&) = delete;
			Console& operator=(const Console&) = delete;
			void CreateConsole(const int x, const int y, const int Width, const int Height, const HWND& ParentHandle)
			{
				this->ConsoleHandle = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_CLIENTEDGE | WS_EX_CONTROLPARENT, "Edit", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT |
					ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL, x, y, Width, Height, ParentHandle, NULL, NULL, NULL);
				ChangeFont(8, "Segoe UI Light");
			}
			void CreateConsole()
			{
				if (AllocConsole() == FALSE) { MessageBoxA(GetForegroundWindow(), "Console allocation function have failed\n Try to use GetLastError() function to found out what the problem is", "Console Allocation Error", MB_ICONERROR); }
				FILE* fp;
				freopen_s(&fp, "CONIN$", "r", stdin);
				freopen_s(&fp, "CONOUT$", "w", stdout);
				freopen_s(&fp, "CONOUT$", "w", stderr);
			}
			void DestroyConsole() { DestroyWindow(ConsoleHandle); }
			void SetTextColor(int r, int g, int b)
			{
				this->r = r;
				this->g = g;
				this->b = b;
			}
			void ChangeFont(int Font_Size, const char* Font_Name) const
			{
				HFONT hFont = CreateFontA(0, Font_Size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Font_Name);
				SendMessageA(ConsoleHandle, WM_SETFONT, (WPARAM)hFont, 0);
			}
		public:
			Console& operator<<(const std::string& message)
			{
				int TextLen = SendMessageA(ConsoleHandle, WM_GETTEXTLENGTH, 0, 0);
				SendMessageA(ConsoleHandle, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
				SendMessageA(ConsoleHandle, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
				return *this;
			}
			Console& operator<<(int message)
			{
				int TextLen = SendMessageA(ConsoleHandle, WM_GETTEXTLENGTH, 0, 0);
				SendMessageA(ConsoleHandle, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
				SendMessageA(ConsoleHandle, EM_REPLACESEL, FALSE, (LPARAM)std::to_string(message).c_str());
				return *this;
			}
			Console& operator<<(double message)
			{
				int TextLen = SendMessageA(ConsoleHandle, WM_GETTEXTLENGTH, 0, 0);
				SendMessageA(ConsoleHandle, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
				SendMessageA(ConsoleHandle, EM_REPLACESEL, FALSE, (LPARAM)std::to_string(message).c_str());
				return *this;
			}
			Console& operator<<(float message)
			{
				int TextLen = SendMessageA(ConsoleHandle, WM_GETTEXTLENGTH, 0, 0);
				SendMessageA(ConsoleHandle, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
				SendMessageA(ConsoleHandle, EM_REPLACESEL, FALSE, (LPARAM)std::to_string(message).c_str());
				return *this;
			}
			Console& operator<<(const char* message)
			{
				int TextLen = SendMessageA(ConsoleHandle, WM_GETTEXTLENGTH, 0, 0);
				SendMessageA(ConsoleHandle, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
				SendMessageA(ConsoleHandle, EM_REPLACESEL, FALSE, (LPARAM)message);
				return *this;
			}
			Console& operator<<(Console& (ConsoleObject)(Console&)) { return ConsoleObject(*this); }
		public:
			void ShowConsole() const { ShowWindow(ConsoleHandle, SW_SHOW); }
			void HideConsole() const { ShowWindow(ConsoleHandle, SW_HIDE); }
			void ClearConsole() const { SetWindowTextA(ConsoleHandle, "\0"); }
		public:
			const HWND GetConsoleHandle() const { return ConsoleHandle; }
			std::tuple<int, int, int> GetColors() const { return std::make_tuple(r, g, b); }
			const int GetConsoleTextLength() const { return GetWindowTextLengthA(ConsoleHandle); }
			char* GetAllConsoleText() const
			{
				int length = GetConsoleTextLength() + 1;
				char* message = (char*)_malloca(length * sizeof(char));
				GetWindowTextA(ConsoleHandle, message, length);
				return message;
			}
		private:
			HWND ConsoleHandle = nullptr;
			int r = 255, g = 255, b = 255;
		} console;
	}


	SDK::Console& endl(SDK::Console& console)
	{
		int TextLen = SendMessageA(console.GetConsoleHandle(), WM_GETTEXTLENGTH, 0, 0);
		SendMessageA(console.GetConsoleHandle(), EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
		SendMessageA(console.GetConsoleHandle(), EM_REPLACESEL, FALSE, (LPARAM)"\n");
		return console;
	}
	SDK::Console& hide(SDK::Console& console)
	{
		console.HideConsole();
		return console;
	}
	SDK::Console& show(SDK::Console& console)
	{
		console.ShowConsole();
		return console;
	}
	SDK::Console& clear(SDK::Console& console)
	{
		console.ClearConsole();
		return console;
	}
}