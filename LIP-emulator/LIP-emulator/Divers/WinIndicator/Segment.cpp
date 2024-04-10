#include "Segment.h"

Segment::Segment(Parameter param) : BaseWindow(param){
	
	
	brushOn = CreateSolidBrush(RGB(0, 255, 0));
	brushOff = CreateSolidBrush(RGB(255, 0, 0));
	//hdc = nullptr;
}

void Segment::turnOn(HDC Hdc) {
	//hdc = GetDC(this->hwnd);
	//if (!hdc) {
	//	return;
	//}
	FillRect(Hdc, &rect, brushOn);
}

void Segment::turnOff(HDC Hdc) {
	HDC hdc = GetDC(hwnd);
	if (!hdc) {
		return;
	}
	FillRect(hdc, &rect, brushOff);
}

//void Segment::init() {
//	BaseWindow::init();
//	
//}
