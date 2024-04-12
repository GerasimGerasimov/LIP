#include "Segment.h"

Segment::Segment(Parameter param) : BaseWindow(param){
	
	
	brushOn = CreateSolidBrush(RGB(0, 255, 0));
	brushOff = CreateSolidBrush(RGB(0, 0, 0));

	hdc = GetDC(hwnd);
	turnOff();
}

void Segment::turnOn(HDC Hdc) {

	FillRect(Hdc, &rect, brushOn);
}

void Segment::turnOff() {
	RECT newRect{ 0, 0, width, height };
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	
	FillRect(hdc, &newRect, brushOff);
	EndPaint(hwnd, &ps);
}

