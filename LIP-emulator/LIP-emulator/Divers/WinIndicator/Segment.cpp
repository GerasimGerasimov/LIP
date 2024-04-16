#include "Segment.h"

void Segment::setColor(HBRUSH brush) {
	RECT newRect{ 0, 0, width, height };
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	FillRect(hdc, &newRect, brush);
	EndPaint(hwnd, &ps);
}

Segment::Segment(Parameter param) : BaseWindow(param){
	
	
	brushOn = CreateSolidBrush(RGB(0, 255, 0));
	brushOff = CreateSolidBrush(RGB(0, 0, 0));

	hdc = GetDC(hwnd);
}

void Segment::setData(std::vector<uint8_t>& vecData) {
	if (vecData[0] == 0) {
		setColor(brushOff);
	}
	else {
		setColor(brushOn);
	}
}

