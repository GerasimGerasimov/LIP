// LIP-emulator.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "LIP-emulator.h"
#include "threads/MainThread.h"
#include "MainWindow.h"

#include <vector> //TODO проверка работы индикаторов

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, MainWindow::szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LIPEMULATOR, MainWindow::szWindowClass, MAX_LOADSTRING);
    MainWindow::MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!MainWindow::InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIPEMULATOR));

    MSG msg;
    
    //TODO проверка работы индикаторов
    int byteCount = 15;
    std::vector<uint8_t> vecData(byteCount);
    for (int i = 0; i < byteCount; ++i) {
        vecData[i] = i;
    }
    MainWindow::setData(vecData);
    //MainThread::createMCU();
    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //MainThread::closeMCU();
    return (int) msg.wParam;
}
