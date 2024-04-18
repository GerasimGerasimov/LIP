#pragma once

#include "framework.h"

#include <vector>

#define MAX_LOADSTRING 100

namespace MainWindow
{
	extern WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
	extern WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void setData(std::vector<uint8_t> &vecData);
	void close();
	void TimeStart(TIMERPROC proc);
	extern UINT IDT_TIMER1;
};

