#include "WinLIP_5Nx.h"


WinLIP_XNx::WinLIP_XNx(Parameter param, uint8_t dataSize) : IndicatorContainer(param){
    count = dataSize;
    heightIndicator = param.rect.bottom - param.rect.top - border.bottom - border.top;
    widthIndicator = (param.rect.right - param.rect.left + (count - 1) * indent) / count - border.left - border.right;
    createIndicator();
}

void WinLIP_XNx::createIndicator(){
    BaseWindow::Parameter param;
    param.parrent = hwnd;
    param.rect.left = border.left;
    param.rect.top = border.top;
    param.rect.right = param.rect.left + widthIndicator;
    param.rect.bottom = height - border.bottom;

    for(int i = 0; i < count; ++i){
        Indicators.push_back(std::make_unique< Win7Segment>(param));
        param.rect.left += widthIndicator + indent;
        param.rect.right += widthIndicator + indent;
    }
}

int WinLIP_XNx::getByteIndication(){
    return count;
}

int WinLIP_XNx::getDesirWidth(){
    return desirWidth;
}

int WinLIP_XNx::getDesirHeight(){
    return desirHeight;
}
