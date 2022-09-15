﻿// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject1.h"
#include <map>
#include<string>

using namespace std;
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hWnd2, hWnd1;

std::map<HWND, COLORREF> color;
std::map<HWND, wstring> text_coord;


COLORREF color_green = RGB(0, 204, 102);
COLORREF color_red = RGB(255, 0, 0);
int x = 0, y = 0;
char buf[100];
char buf2[100];

int type = 0; // 0 = a, 1 = b


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd1 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd1)
   {
      return FALSE;
   }

   color[hWnd1] = color_red;
   text_coord[hWnd1] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);

   ShowWindow(hWnd1, nCmdShow);
   UpdateWindow(hWnd1);


   hWnd2 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd2)
   {
       return FALSE;
   }

   color[hWnd2] = color_red;
   text_coord[hWnd2] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);

   ShowWindow(hWnd2, nCmdShow);
   UpdateWindow(hWnd2);



   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT client_rect;
    int temp_x = 0;
    int temp_y = 0;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_CHANGE_COORD:
                 type = type == 0 ? 1 : 0;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONUP:

       
        if (color[hWnd] == color_red)
            color[hWnd] = color_green;
        else
            color[hWnd] = color_red;

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);
        InvalidateRect(hWnd, &client_rect, true);

        break;

    case WM_MOUSEMOVE:
        temp_x = LOWORD(lParam);
        temp_y = HIWORD(lParam);
        if (x != temp_x || y != temp_y)
        {
            x = temp_x;
            y = temp_y;

            RECT min_area;
            min_area.left = 0;
            min_area.top = 0;
            min_area.right = 100;
            min_area.bottom = 30;

            if (type == 1)
            {
                text_coord[hWnd1] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);
                text_coord[hWnd2] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);
            }
            else
            {
                if (hWnd == hWnd1)
                {
                    text_coord[hWnd1] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);
                    text_coord[hWnd2] = L"x: 0, y: 0";
                }
                else
                {
                    text_coord[hWnd2] = L"x: " + to_wstring(x) + L", y: " + to_wstring(y);
                    text_coord[hWnd1] = L"x: 0, y: 0";
                }
            }
            InvalidateRect(hWnd1, &min_area, true);
            InvalidateRect(hWnd2, &min_area, true);

        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
              
            POINT point;
            GetClientRect(hWnd, &client_rect);

            HPEN pen = CreatePen(PS_SOLID, 5.0, color[hWnd]);
            auto old_pen = SelectObject(hdc, pen);
            MoveToEx(hdc, 0, 0, &point);
            LineTo(hdc, client_rect.right, client_rect.bottom);
            MoveToEx(hdc, 0, client_rect.bottom, &point);
            LineTo(hdc, client_rect.right, 0);
            SelectObject(hdc, old_pen);
            DeleteObject(pen);

            TextOut(hdc, 0, 0, text_coord[hWnd].c_str(), text_coord[hWnd].length());

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

// Обработчик сообщений для окна "О программе".
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
