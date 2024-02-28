#include "Page.h"

#include "Indicator/LIP_5Nx.h"

Page::Page(){
    Indicator *indicator;
    int sizeSegment = 0;
    for(int i = 0; i < 3; ++i){
        indicator = new LIP_5Nx;
        ListIndicators.push_back(indicator);
        sizeSegment += indicator->getDataSize();
    }
    str = "2356.8";
    str2 = "qwert";
    str3 = "wasd.f";
    bufferData.setSizeBuffer(sizeSegment);
    bufferSender.setSizeBuffer(sizeSegment);
}

Page::~Page(){
    for(auto& n : ListIndicators){
        delete n;
    }
}

void Page::update(){

    bufferData.addData(ListIndicators[0]->getValue(str));
    bufferData.addData(ListIndicators[1]->getValue(str2));
    bufferData.addData(ListIndicators[2]->getValue(str3));
    Indicator::bringOutValue();
}