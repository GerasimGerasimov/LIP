#include "Segment.h"

void Segment::setColor(HBRUSH brush){
    RECT newRect{0, 0, width, height};
    FillRect(hdc, &newRect, brush);
}

void Segment::createSegment(){
    hwnd = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, rect.left, rect.top, width, height, Parrent,
        NULL, NULL, NULL);
}

Segment::Segment(Parameter param) : BaseObject(param){
    createSegment();

    brushOn = CreateSolidBrush(RGB(0, 255, 0));
    brushOff = CreateSolidBrush(RGB(255, 255, 255));

    hdc = GetDC(hwnd);
}

void Segment::setData(std::vector<uint8_t>& vecData){
    if(vecData[0] == 0){
        setColor(brushOff);
    }
    else{
        setColor(brushOn);
    }
}

