#include "WinLIP_5Nx.h"


WinLIP_5Nx::WinLIP_5Nx(Parameter param) : BaseWindow(param) {
	border.left = 10;
	border.top = 10;
	border.right = 10;
	border.bottom = 10;
	indent = 10;
	heightIndicator = param.rect.bottom - param.rect.top - border.bottom - border.top;
	widthIndicator = (param.rect.right - param.rect.left + (count - 1) * indent) / count - border.left - border.right;
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
		
		Indicators[i]->createSegment();
		param.rect.left +=  widthIndicator + indent;
		param.rect.right += widthIndicator + indent;
	}



}

