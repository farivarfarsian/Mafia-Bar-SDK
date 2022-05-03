#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>
#include <uxtheme.h>
#include <vssym32.h>
#pragma comment(lib, "uxtheme.lib")


typedef RECT WRECTANGLE;
typedef HWND WindowHandle;

/*------ Codes have been adopted from this reposity: https://github.com/grassator/win32-window-custom-titlebar ------*/

typedef struct
{
    RECT Close;
    RECT Maximize;
    RECT Minimize;
} TitleBarButtonRects;

typedef enum
{
    TitleBarHoveredButton_None,
    TitleBarHoveredButton_Minimize,
    TitleBarHoveredButton_Maximize,
    TitleBarHoveredButton_Close,
} TitleBarHoveredButton;

namespace MafiaBar
{
    namespace SDK
    {
        namespace WindowsHelper
        {
            #undef IsMaximized
            const bool IsMaximized(WindowHandle Handle)
            {
                WINDOWPLACEMENT placement = { 0 };
                placement.length = sizeof(WINDOWPLACEMENT);
                if (GetWindowPlacement(Handle, &placement)) {
                    return placement.showCmd == SW_SHOWMAXIMIZED;
                }
                return false;
            }
            const WRECTANGLE GetTitlebarRect(WindowHandle Handle)
            {
                SIZE title_bar_size = { 0 };
                const int top_and_bottom_borders = 2;
                HTHEME theme = OpenThemeData(Handle, L"WINDOW");
                UINT dpi = GetDpiForWindow(Handle);
                GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
                CloseThemeData(theme);

                int height = (int)((float)title_bar_size.cy * dpi / 96) + top_and_bottom_borders;

                RECT rect;
                GetClientRect(Handle, &rect);
                rect.bottom = rect.top + height;
                return rect;
            }
            const TitleBarButtonRects GetTitleBarButtonRects(WindowHandle Handle, const RECT& TitleBarRect)
            {
                UINT dpi = GetDpiForWindow(Handle);
                TitleBarButtonRects ButtonRects{};
                int button_width = (int)((float)47 * dpi / 96.0f);
                ButtonRects.Close = TitleBarRect;
                ButtonRects.Close.top += 0;
                ButtonRects.Close.left = ButtonRects.Close.right - button_width;
                ButtonRects.Maximize = ButtonRects.Close;
                ButtonRects.Maximize.left -= button_width;
                ButtonRects.Maximize.right -= button_width;
                ButtonRects.Minimize = ButtonRects.Maximize;
                ButtonRects.Minimize.left -= button_width;
                ButtonRects.Minimize.right -= button_width;
                return ButtonRects;
            }
            void CenterRectinRect(RECT* ToCenter, const RECT* OuterRect)
            {
                int to_width = ToCenter->right - ToCenter->left;
                int to_height = ToCenter->bottom - ToCenter->top;
                int outer_width = OuterRect->right - OuterRect->left;
                int outer_height = OuterRect->bottom - OuterRect->top;

                int padding_x = (outer_width - to_width) / 2;
                int padding_y = (outer_height - to_height) / 2;

                ToCenter->left = OuterRect->left + padding_x;
                ToCenter->top = OuterRect->top + padding_y;
                ToCenter->right = ToCenter->left + to_width;
                ToCenter->bottom = ToCenter->top + to_height;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------*/


