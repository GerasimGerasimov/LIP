#include "Segment.h"

Segment::Segment(Parameter param) : BaseWindow(param){
	
	
	brushOn = CreateSolidBrush(RGB(0, 255, 0));
	brushOff = CreateSolidBrush(RGB(255, 0, 0));

	hdc = GetDC(param.parrent);
	//turnOff();
}

void Segment::turnOn(HDC Hdc) {
	//hdc = GetDC(this->hwnd);
	//if (!hdc) {
	//	return;
	//}
	FillRect(Hdc, &rect, brushOn);
}

void Segment::turnOff() {
	RECT newRect{ 0, 0, height, width };
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	HDC newHDC = GetDC(Parrent);
	FillRect(newHDC, &rect, brushOff);
	EndPaint(hwnd, &ps);
}

