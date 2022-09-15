// WindowsProject1.cpp : Определяет точку входа для приложения.
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
std::map<HWND, POINT> prev_sec, prev_min, prev_hour;

COLORREF color_green = RGB(0, 204, 102);
COLORREF color_red = RGB(255, 0, 0);
int x_cur_pos = 0, y_cur_pos = 0;
const double PI = 3.141592653589793238462643383;
double coef_sec = 0.9, coef_min = 0.7, coef_hour = 0.5, coef_R = 0.9;

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
    RECT client_rect;
    SYSTEMTIME st;
    GetSystemTime(&st);

   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd1 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd1)
   {
      return FALSE;
   }

   color[hWnd1] = color_red;
   text_coord[hWnd1] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);

   ShowWindow(hWnd1, nCmdShow);
   UpdateWindow(hWnd1);


   hWnd2 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd2)
   {
       return FALSE;
   }

   color[hWnd2] = color_red;
   text_coord[hWnd2] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);

   ShowWindow(hWnd2, nCmdShow);
   UpdateWindow(hWnd2);

   SetTimer(hWnd1, 1, 1000, NULL);
   SetTimer(hWnd2, 1, 1000, NULL);


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
        // если сменился х или у, обновлять участок с текстом
        temp_x = LOWORD(lParam);
        temp_y = HIWORD(lParam);
        if (x_cur_pos != temp_x || y_cur_pos != temp_y)
        {
            x_cur_pos = temp_x;
            y_cur_pos = temp_y;

            RECT min_area;
            min_area.left = 0;
            min_area.top = 0;
            min_area.right = 100;
            min_area.bottom = 30;

            if (type == 1)
            {
                text_coord[hWnd1] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);
                text_coord[hWnd2] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);
            }
            else
            {
                if (hWnd == hWnd1)
                {
                    text_coord[hWnd1] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);
                    text_coord[hWnd2] = L"x: 0, y: 0";
                }
                else
                {
                    text_coord[hWnd2] = L"x: " + to_wstring(x_cur_pos) + L", y: " + to_wstring(y_cur_pos);
                    text_coord[hWnd1] = L"x: 0, y: 0";
                }
            }
            InvalidateRect(hWnd1, &min_area, true);
            InvalidateRect(hWnd2, &min_area, true);

        }
        break;
    case WM_TIMER:

        // обновлять только область с часами
        RECT min_area;
        GetClientRect(hWnd, &client_rect);
        temp_x = client_rect.right / 2;
        temp_y = client_rect.bottom / 2;
        min_area.left = temp_x - (coef_R + 0.1) * client_rect.bottom;
        min_area.top = temp_y - (coef_R + 0.1) * client_rect.bottom;
        min_area.right = temp_x + (coef_R + 0.1) * client_rect.bottom;
        min_area.bottom = temp_y - (coef_R + 0.1) * client_rect.bottom;
        InvalidateRect(hWnd, &min_area, true);

        break;
    case WM_PAINT:
        {
        SYSTEMTIME st;
        GetLocalTime(&st);

        GetClientRect(hWnd, &client_rect);

        double lenght0 = coef_R * client_rect.bottom / 2;
        // время в секундах, с учетом, что в одном круге:
        double now_sec = st.wSecond;                                         // 60 положений секундной стрелки
        double now_min = st.wMinute*60.0;                                    // 60*60 положений минутной стрелки
        double now_hour =( st.wHour >= 12 ? st.wHour - 12 : st.wHour)*3600.0;// 12*60*60 положений часовой стрелки
        temp_x = client_rect.right / 2;
        temp_y = client_rect.bottom / 2;
        
        // смещение от центра в полярных координатах, длина стрелки (lenght0 * coef), угол с учетом возможных положений и переводится в радианы
        prev_hour[hWnd].x = temp_x + lenght0 * coef_hour * cos((now_hour / (120.0) - 90.0) * PI / 180.0);
        prev_hour[hWnd].y = temp_y + lenght0 * coef_hour * sin((now_hour / (120.0) - 90.0) * PI / 180.0);
        prev_min[hWnd].x = temp_x + lenght0 * coef_min * cos((now_min / 10.0 - 90.0) * PI / 180.0);
        prev_min[hWnd].y = temp_y + lenght0 * coef_min * sin((now_min / 10.0 - 90.0) * PI / 180.0);
        prev_sec[hWnd].x = temp_x + lenght0 * coef_sec * cos((now_sec * 6.0 -90.0) * PI / 180.0);
        prev_sec[hWnd].y = temp_y + lenght0 * coef_sec * sin((now_sec * 6.0 -90.0) * PI / 180.0);

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
              
            POINT point;
            
            HPEN pen = CreatePen(PS_SOLID, 3.0, color[hWnd]);
            auto old_pen = SelectObject(hdc, pen);
           
            // контур часов
            Ellipse(hdc, temp_x - coef_R * temp_y, temp_y - coef_R * temp_y, temp_x + coef_R * temp_y, temp_y + coef_R * temp_y);
           
            // стрелки - начало в центре (temp_x, temp_y), коорддинаты конца в (prev_*[hWnd].x, prev_*[hWnd].y)
            MoveToEx(hdc, temp_x, temp_y, &point);
            LineTo(hdc, prev_hour[hWnd].x, prev_hour[hWnd].y);
            MoveToEx(hdc, temp_x, temp_y, &point);
            LineTo(hdc, prev_min[hWnd].x, prev_min[hWnd].y);
            MoveToEx(hdc, temp_x, temp_y, &point);
            LineTo(hdc, prev_sec[hWnd].x, prev_sec[hWnd].y);

            // кружок в центре
            Ellipse(hdc, temp_x - 0.1*coef_R * temp_y, temp_y - 0.1 * coef_R * temp_y, temp_x + 0.1 * coef_R * temp_y, temp_y + 0.1 * coef_R * temp_y);

            SelectObject(hdc, old_pen);
            DeleteObject(pen);

            TextOut(hdc, 0, 0, text_coord[hWnd].c_str(), text_coord[hWnd].length());

            // вывод текущего времени !таймер обновляет только область часов, надо кликать для смены секунд
            //wstring time = to_wstring(st.wHour) + L" : " + to_wstring(st.wMinute) + L" : " + to_wstring(st.wSecond);
            //TextOut(hdc, 0, client_rect.bottom/2, time.c_str(), time.length());

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); //"Убиваем" таймер
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
