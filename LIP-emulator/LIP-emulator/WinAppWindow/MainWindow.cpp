#include "MainWindow.h"
#include "LIP-emulator.h"
#include "Divers/WinIndicator/WinLIP_5Nx.h"

HINSTANCE MainWindow::hInst = NULL;   // текущий экземпляр
WinLIP_5Nx* MainWindow::indicator = nullptr;

ATOM MainWindow::MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIPEMULATOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LIPEMULATOR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // Сохранить маркер экземпляра

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }
    static BaseWindow::Parameter param;
    param.parrent = hWnd;
    param.rect.left = 100;
    param.rect.top = 100;
    param.rect.right = 510;
    param.rect.bottom = 210;
    indicator = new WinLIP_5Nx(param);
    
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//const int ID_BUTTON9(3900);

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //TCHAR greeting[] = _T("Hello, LIP desctop.");
    
    switch (message) {
    case WM_CREATE:
        /*CreateWindow(L"button", L"--< КНОПКА >--", BS_PUSHBUTTON |
            WS_VISIBLE | WS_CHILD | WS_TABSTOP, 60, 60, 120, 25, hWnd,
            (HMENU)ID_BUTTON9, NULL, NULL);*/
        
        
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        indicator->createIndicator();
        //indicator->on1(hdc);
        //TextOut(hdc, 100, 100, greeting, _tcslen(greeting));
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

INT_PTR MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
