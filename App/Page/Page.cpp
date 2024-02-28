#include "Page.h"

#include "Indicator/LIP_5Nx.h"
#include "DMAIndicator.h"

Page::Page(){
    Indicator *indicator;
    int sizeSegment = 0;
    for(int i = 0; i < 3; ++i){
        indicator = new LIP_5Nx;
        ListIndicators.push_back(indicator);
        sizeSegment += indicator->getDataSize();
    }
    str = "11111";
    str2 = "12.770";
    str3 = "vvd.11";
    bufferData.setSizeBuffer(sizeSegment);
    bufferSender.setSizeBuffer(sizeSegment);
    update();
    bufferSender = bufferData;
    DMAIndicator::getInstance().setMemoryBaseAddr(bufferSender.getAddrBuffer());
}

Page::~Page(){
    for(auto& n : ListIndicators){
        delete n;
    }
}

void Page::update(){
    std::vector<uint8_t> res = ListIndicators[0]->getValue(str);
    bufferData.addData(res);
    res = ListIndicators[1]->getValue(str2);
    bufferData.addData(res);
    res = ListIndicators[2]->getValue(str3);
    bufferData.addData(res);
    //Indicator::bringOutValue();
}