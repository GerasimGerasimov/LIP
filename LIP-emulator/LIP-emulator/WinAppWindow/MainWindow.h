#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class MainWindow
{
public:
	static HINSTANCE hInst;                                // текущий экземпляр
	WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
	WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

