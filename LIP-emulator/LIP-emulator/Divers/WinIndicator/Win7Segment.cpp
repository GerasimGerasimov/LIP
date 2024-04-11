#include "Win7Segment.h"


void Win7Segment::createSegment() {

    BaseWindow::Parameter param;
    param.parrent = hwnd;
    for (const auto& n : LocationSegments) {
        param.rect = n;
        Segments.push_back(std::make_unique<Segment>(param));
    }

}

int Win7Segment::getWidth() const {
    return width;
}

int Win7Segment::getHeight() const {
    return height;
}

void Win7Segment::fillLocationSegment() {
    RECT rectSegment = {0};
    rectSegment.left = border.left;
    rectSegment.top = border.top;
    rectSegment.right = width - border.right;
    rectSegment.bottom = rectSegment.top + segmentWeight;
    LocationSegments.push_back(rectSegment);
    rectSegment.left = width - border.right;
    rectSegment.top = border.top;
    rectSegment.right = rectSegment.left + segmentWeight;
    rectSegment.bottom = (height / 2);
    LocationSegments.push_back(rectSegment);
    rectSegment.left = width - border.right;
    rectSegment.top = (height / 2);
    rectSegment.right = rectSegment.left + segmentWeight;
    rectSegment.bottom = height - border.bottom;
    LocationSegments.push_back(rectSegment);
    rectSegment.left = border.left;
    rectSegment.top = height - border.bottom - segmentWeight;
    rectSegment.right = width - border.right;
    rectSegment.bottom = rectSegment.top + segmentWeight;
    LocationSegments.push_back(rectSegment);
    rectSegment.left = border.left - segmentWeight;
    rectSegment.top = (height / 2);
    rectSegment.right = rectSegment.left + segmentWeight;
    rectSegment.bottom = height - border.bottom;
    LocationSegments.push_back(rectSegment);
    rectSegment.left = border.left - segmentWeight;
    rectSegment.top = border.top;
    rectSegment.right = rectSegment.left + segmentWeight;
    rectSegment.bottom = (height / 2);
    LocationSegments.push_back(rectSegment);
    rectSegment.left = border.left;
    rectSegment.top = (height / 2.0) - (segmentWeight / 2);
    rectSegment.right = width - border.right;
    rectSegment.bottom = (height / 2.0) + (segmentWeight / 2);
    LocationSegments.push_back(rectSegment);
    rectSegment.left = width - border.right + segmentWeight * 2;
    rectSegment.top = height - border.bottom - segmentWeight;
    rectSegment.right = rectSegment.left + segmentWeight;
    rectSegment.bottom = rectSegment.top + segmentWeight;
    LocationSegments.push_back(rectSegment);
}

Win7Segment::Win7Segment(Parameter param) : BaseWindow(param){
    segmentWeight = 4;
    border.left = 10;
    border.top = 8;
    border.right = 18;
    border.bottom = 10;
    fillLocationSegment();
}

void Win7Segment::on1(HDC hdc) {
    Segments[0]->turnOn(hdc);
    
}



