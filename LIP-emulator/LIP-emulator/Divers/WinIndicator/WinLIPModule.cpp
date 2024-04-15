#include "WinLIPModule.h"

WinLIPModule::WinLIPModule(Parameter param, int CountIndicator, RECT newBorder) : IndicatorContainer(param) {
	count = CountIndicator;
	border = newBorder;
	widthIndicator = param.rect.right - param.rect.left - border.left - border.right;
	heightIndicator = (param.rect.bottom - param.rect.top + (count - 1) * indent) / count - border.bottom - border.top;
	
	createSegment();
}

//TODO брать из списка индикаторов
void WinLIPModule::createSegment() {
	BaseWindow::Parameter param;
	param.parrent = hwnd;
	param.rect.left = border.left;
	param.rect.top = border.top;
	param.rect.right = width - border.right;
	param.rect.bottom = param.rect.top + heightIndicator;
	for (int i = 0; i < count; ++i) {
		Indicators.push_back(std::make_unique<WinLIP_5Nx>(param));
		param.rect.top += heightIndicator + indent;
		param.rect.bottom += heightIndicator + indent;
	}
}

void WinLIPModule::setData(std::vector<uint8_t>) {

}

//RECT WinLIPModule::getRect(int CountIndicator) {
//	RECT rect;
//	rect.left = 0;
//	rect.top = 0;
//	rect.right = widthChildIndicator + border.left + border.right;
//	rect.bottom = border.top + border.bottom + CountIndicator * heightChildIndicator + (CountIndicator - 1) * indent;
//	return rect;
//}
