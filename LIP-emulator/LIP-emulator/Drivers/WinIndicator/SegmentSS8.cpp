#include "SegmentSS8.h"

void SegmentSS8::setColor(HBRUSH brush){
    RECT newRect{0, 0, width, height};
    FillRect(hdc, &newRect, brush);
}

void SegmentSS8::createSegment(){
    hwnd = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, rect.left, rect.top, width, height, Parrent,
        NULL, NULL, NULL);
}

SegmentSS8::SegmentSS8(Parameter param) : BaseObject(param){
    createSegment();

    brushEnable = CreateSolidBrush(RGB(255, 255, 0));
    brushDisable = CreateSolidBrush(RGB(255, 0, 0));
    brushDisableAlarm = CreateSolidBrush(RGB(0, 255, 0));
    brushOff = CreateSolidBrush(RGB(0, 0, 0));

    hdc = GetDC(hwnd);
}

void SegmentSS8::setData(std::vector<uint8_t>& vecData){
    switch(vecData[0]){
    case 0:
        setColor(brushOff);
        break;
    case 1:
        setColor(brushDisable);
        break;
    case 2:
        setColor(brushDisableAlarm);
        break;
    case 3:
        setColor(brushEnable);
        break;
    }
}
