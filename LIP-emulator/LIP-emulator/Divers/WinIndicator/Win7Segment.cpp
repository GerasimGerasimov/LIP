#include "Win7Segment.h"


void Win7Segment::createSegment() {
    /*HWND hs = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD  | WS_BORDER, 10, 8, 45, 4, hwnd,
        NULL, NULL, NULL);*/
    BaseWindow::Parameter param{ hwnd, {10, 8, 55, 12 } };

    Segments.push_back(new Segment(param));
    param.rect.top = 40;
    param.rect.bottom = 44;
    Segments.push_back(new Segment(param));
    param.rect.top = 76;
    param.rect.bottom = 80;
    Segments.push_back(new Segment(param));
    param.rect.left = 4;
    param.rect.right = 8;
    param.rect.top = 12;
    param.rect.bottom = 38;
    Segments.push_back(new Segment(param));
    param.rect.top = 46;
    param.rect.bottom = 74;
    Segments.push_back(new Segment(param));
    param.rect.left = 57;
    param.rect.right = 61;
    Segments.push_back(new Segment(param));
    param.rect.top = 12;
    param.rect.bottom = 38;
    Segments.push_back(new Segment(param));
    param.rect.top = 78;
    param.rect.left = 59;
    param.rect.right = 63;
    param.rect.bottom = 82;
    Segments.push_back(new Segment(param));

    for (const auto& n : Segments) {
        n->init();
    }
}

Win7Segment::Win7Segment(Parameter param) : BaseWindow(param){

}

void Win7Segment::init() {
    BaseWindow::init();
    //createSegment();
}

void Win7Segment::on1(HDC hdc) {
    Segments[0]->turnOn(hdc);
    Segments[1]->turnOff(hdc);
}



