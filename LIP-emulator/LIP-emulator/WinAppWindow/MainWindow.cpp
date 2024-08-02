#include "MainWindow.h"
#include "LIP-emulator.h"
#include "WinIndicator/WinLIPModule.h"
#include "DI/WinDI.h"
#include "OutStream.h"
#include "com_master_driver.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"
#include "Message/Message.h"

namespace MainWindow
{
    HWND hWnd;
    WinLIPModule* indicator;
    HINSTANCE hInst;                                // текущий экземпляр
    WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
    WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
    WinDI* winDI;
    UINT IDT_TIMER1 = 1;
    void createObject();
    void createIndicator();
    void createDI();
    void createOutText();
}

void MainWindow::createObject(){
    createIndicator();
    createDI();
    createOutText();
    ComMasterDriver::open();
}

ATOM MainWindow::MyRegisterClass(HINSTANCE hInstance){
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

void MainWindow::createIndicator(){
    std::string Config = InternalResources::getInstance().getItemStringByName((char*)"Config");
    std::vector<std::string> Configuration = Parser::splitString(" ", Config);
    BaseObject::Parameter param;
    param.parrent = hWnd;
    int countIndicator = 0; //TODO пока только 5N индикатороы countIndicator = Configuration.size();
    std::map<std::string, std::function<void(std::string& Type)>> parseHandler;
    auto lambda = [&countIndicator](std::string&){
        ++countIndicator;
    };
    parseHandler[i5N] = lambda;
    parseHandler[iSwitchStatus] = lambda;
    Parser::parseConfigurarion(Configuration, parseHandler);

    // вычисление размеров модуля с индикаторами
    int indent = 10;
    int diserWidthIndicator = 310;
    int diserHeightIndicator = 90;
    RECT borderIndicator{10, 10, 10, 10};
    RECT rectModule;
    rectModule.left = indent;
    rectModule.right = indent + diserWidthIndicator + borderIndicator.right + borderIndicator.left;
    rectModule.top = indent;
    rectModule.bottom = indent + borderIndicator.top + borderIndicator.bottom + countIndicator * diserHeightIndicator + (countIndicator - 1) * indent;
    param.rect = rectModule;
    indicator = new WinLIPModule(param, Configuration, borderIndicator);//TODO обработка отсутствия параметров индикаторов
}

BOOL MainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow){
    hInst = hInstance; // Сохранить маркер экземпляра

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);

    if(!hWnd){
        return FALSE;
    }



    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    createObject();

    //indicator->turnOff();
    return TRUE;
}

void MainWindow::createDI(){
    BaseObject::Parameter param;
    param.parrent = hWnd;
    param.rect = {500, 10, 0, 0}; //длина и ширина вычисляются самим объектом 
    winDI = new WinDI(param);
}

void MainWindow::createOutText(){
    HWND hwnd = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL, 620, 10, 285, 180, hWnd,
        NULL, NULL, NULL);
    lip::cout.setHwnd(hwnd);
}

//const int ID_BUTTON9(3900);

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    //TCHAR greeting[] = _T("Hello, LIP desctop.");

    switch(message){
    case WM_CREATE:
        /*CreateWindow(L"button", L"--< КНОПКА >--", BS_PUSHBUTTON |
            WS_VISIBLE | WS_CHILD | WS_TABSTOP, 60, 60, 120, 25, hWnd,
            (HMENU)ID_BUTTON9, NULL, NULL);*/


        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        keyBoardControlMCU(lParam);
        // Разобрать выбор в меню:
        switch(wmId){
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
        //indicator->getRect(5);
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

INT_PTR MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
    UNREFERENCED_PARAMETER(lParam);
    switch(message){
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void MainWindow::setData(std::vector<uint8_t>& vecData){
    indicator->setData(vecData);
}

void MainWindow::close(){
    if(winDI){
        delete winDI;
        winDI = nullptr;
    }
    if(indicator){
        delete indicator;
        indicator = nullptr;
    }
}

void MainWindow::TimeStart(TIMERPROC proc){
    SetTimer(hWnd, IDT_TIMER1, 100, proc);
}

void MainWindow::keyBoardControlMCU(LPARAM Param){
    int butDI = winDI->getDIPush(reinterpret_cast<HWND>(Param));
    switch(butDI){
    case 0:
        lip::cout << "push DI_0 \n";
        LipMessage::getInstance().send_message(Event::KEYBOARD, static_cast<u32>(KeyCodes::F1), 0);
        break;
    case 1:
        lip::cout << "push DI_1 \n";
        LipMessage::getInstance().send_message(Event::KEYBOARD, static_cast<u32>(KeyCodes::F2), 0);
        break;
    default:
        return;
    }
}
