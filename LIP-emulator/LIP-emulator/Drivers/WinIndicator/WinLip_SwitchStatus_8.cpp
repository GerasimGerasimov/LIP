#include "WinLip_SwitchStatus_8.h"
#include "SegmentSS8.h"

WinLip_SwitchStatus_8::WinLip_SwitchStatus_8(Parameter param) : IndicatorContainer(param){
    border.left = 20;
    indent = 30;
    heightIndicator = 20;
    widthIndicator = 20;
    createIndicator();
}

void WinLip_SwitchStatus_8::createIndicator(){
    BaseWindow::Parameter param;
    param.parrent = hwnd;
    param.rect.left = border.left;
    param.rect.top = border.top;
    param.rect.right = param.rect.left + widthIndicator;
    param.rect.bottom = param.rect.top + heightIndicator;
    for(int i = 0; i < line; ++i){
        for(int j = 0; j < count / line; ++j){
            Indicators.push_back(std::make_unique<SegmentSS8>(param));
            param.rect.left += widthIndicator + indent;
            param.rect.right += widthIndicator + indent;
        }
        param.rect.left = border.left;
        param.rect.right = param.rect.left + widthIndicator;
        param.rect.top += heightIndicator + indent;
        param.rect.bottom += heightIndicator + indent;
    }
}

int WinLip_SwitchStatus_8::getByteIndication(){
    return count >> 2; // count / 4
}

void WinLip_SwitchStatus_8::setData(std::vector<uint8_t>& vecData){
    if(vecData.size() != 2){
        return;
    }
    int bitLedBite = count / bitPerLED;
    for(int s = 0; s < line; ++s){
        for(int i = 0; i < count / line; ++i){
            int indexIndicator = (i + (s * (count / line)));
            int indexData = indexIndicator / (bitLedBite);
            std::vector<uint8_t> state(1);
            state[0] = ((vecData[indexData]) & (1 << (count - 1 - ((indexIndicator % bitLedBite) * 2)))) ? 1 : 0;
            state[0] |= ((vecData[indexData]) & (1 << (count - 1 - ((indexIndicator % bitLedBite) * 2 + 1)))) ? 2 : 0;
            int idIndicator = s * (bitLedBite) + i;
            Indicators[indexIndicator]->setData(state);
        }
    }
}

int WinLip_SwitchStatus_8::getDesirWidth(){
    return desirWidth;
}

int WinLip_SwitchStatus_8::getDesirHeight(){
    return desirHeight;
}
