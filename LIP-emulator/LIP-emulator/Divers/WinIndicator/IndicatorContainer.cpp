#include "IndicatorContainer.h"

//TODO взможно сделать обработку без static
int IndicatorContainer::indent = 10;
RECT IndicatorContainer::border = { 10, 10, 10, 10 };

IndicatorContainer::IndicatorContainer(Parameter param) : BaseWindow(param) {
	border = { 10, 10, 10, 10 };
	heightIndicator = 0;// param.rect.bottom - param.rect.top - border.bottom - border.top;
	widthIndicator = 0;// (param.rect.right - param.rect.left + (count - 1) * indent) / count - border.left - border.right;
}
