#include "MainWindow.h"
#include "LIP-emulator.h"
#include "Divers/WinIndicator/WinLIPModule.h"

HINSTANCE MainWindow::hInst = NULL;   // текущий экземпляр
WinLIPModule* MainWindow::indicator = nullptr;

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
        CW_USEDEFAULT, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }
    
    
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    static BaseWindow::Parameter param;
    param.parrent = hWnd;
    int countIndicator = 2;
    int indent = 10;
    RECT rectModule = WinLIPModule::getRect(countIndicator);
    rectModule.left += indent;
    rectModule.right += indent;
    rectModule.top += indent;
    rectModule.bottom += indent;
    param.rect = rectModule;
    indicator = new WinLIPModule(param, countIndicator);

    //indicator->turnOff();
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
        //indicator->turnOff();
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SHOWWINDOW:

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
