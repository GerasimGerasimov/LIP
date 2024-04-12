#include "WinLIP_5Nx.h"


WinLIP_5Nx::WinLIP_5Nx(Parameter param) : IndicatorContainer(param) {
	heightIndicator = param.rect.bottom - param.rect.top - border.bottom - border.top;
	widthIndicator = (param.rect.right - param.rect.left + (count - 1) * indent) / count - border.left - border.right;
	createIndicator();
}

void WinLIP_5Nx::createIndicator() {
	width;
	height;
	BaseWindow::Parameter param;
	param.parrent = hwnd;
	param.rect.left = border.left;
	param.rect.top = border.top;
	param.rect.right = param.rect.left + widthIndicator;
	param.rect.bottom = height - border.bottom;

	for (int i = 0; i < count; ++i) {
		Indicators.push_back(std::make_unique< Win7Segment>(param));
		param.rect.left +=  widthIndicator + indent;
		param.rect.right += widthIndicator + indent;
	}



}

//void WinLIP_5Nx::turnOff() {
//	for (const auto& n : Indicators) {
//		n->turnOff();
//	}
//}

