#include "Page.h"
#include "Indicator/LIP_5Nx.h"
#include "DMAIndicator.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"
//#include "ramdata.h"

#include <sstream>
#include <iomanip>

Page* pageFunction;

void Page::init() {
    std::string Config = InternalResources::getInstance().getItemStringByName("Config");
    Configuration = Parser::splitString(" ", Config);
    for (const auto& ind : Configuration) {
        if (ind == "5N") {
            ListIndicators.push_back(new LIP_5Nx);
        }
    }
}

Page::Page(){

    init();
    for (const auto& ind : ListIndicators) {
        sizeSegment += ind->getDataSize();
    }
    //str = "56789";
    //str2 = "12.7.7.0";
    //str3 = "vvd.1A";
    //bufferData.setSizeBuffer(sizeSegment);
    //bufferSender.setSizeBuffer(sizeSegment);
    //DMAIndicator::getInstance().setMemoryBaseAddr(bufferSender);
    //DMAIndicator::getInstance().DMAstart(sizeSegment);
    //pageFunction = this;
}

Page::~Page(){
    for(auto& n : ListIndicators){
        delete n;
    }
}

bool Page::update(){
    for (const auto& ind : ListIndicators) {
        if (ind->update()) {
            ind->getValue();//TODO запись в буффер
        }
    }
    return false;
}

//получить общее колличество байт индикаторов
uint16_t Page::getByteIndicators() {
    uint16_t sizeSegment = 0;
    for (const auto& ind : ListIndicators) {
        sizeSegment += ind->getDataSize();
    }
    return sizeSegment;
}

//установить новую страницу
void Page::setIndication(std::string page) {
    std::vector<std::string> newIndication = Parser::splitString("\r\n", page);
    for (int ind = 0; ind < ListIndicators.size(); ++ind) {
        ListIndicators[ind]->setParameter(newIndication[ind]);
    }
}

uint16_t Page::getSizeSegment() {
    return sizeSegment;
}

//TDOD для DMA
void updateDMA(){
    pageFunction->update();
}