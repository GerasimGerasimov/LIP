#include "Page.h"
#include "Indicator/LIP_5Nx.h"
#include "DMAIndicator.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"

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
    countFillBuffer = 0;
    pageBuffer = nullptr;
    init();
    for (const auto& ind : ListIndicators) {
        sizeSegment += ind->getDataSize();
    }

}

Page::~Page(){
    for(auto& n : ListIndicators){
        delete n;
    }
}

bool Page::update(){
    u16 offsetStart = 0;
    u16 offsetEnd = 0;
    for (int i = 0; i < ListIndicators.size(); ++i) {
        offsetStart = offsetEnd;
        offsetEnd += ListIndicators[i]->getDataSize();
        if (ListIndicators[i]->update()) {
            ++countFillBuffer;
            std::vector<uint8_t> resultData = ListIndicators[i]->getValue();
            pageBuffer->addData(resultData, offsetStart, offsetEnd);
        }
    }
    if (countFillBuffer == ListIndicators.size()) {
        countFillBuffer = 0;
        return true;
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

void Page::setBuffer(Buffer* newBuffer) {
    pageBuffer = newBuffer;
}

//TDOD для DMA
void updateDMA(){
    pageFunction->update();
}