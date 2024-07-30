#include "Page.h"
#include "Indicator/LIP_5Nx.h"
#include "Indicator/LIP_SS8.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"

Page* pageFunction;

void Page::init(){
    std::string Config = InternalResources::getInstance().getItemStringByName("Config");
    Configuration = Parser::splitString(" ", Config);

    auto lambda5N = [this](std::string& Type){
        ListIndicators.push_back(new LIP_5Nx);
        bool typeA = (Type == "A");
        ListIndicators.back()->setTypeAnode(typeA);
    };

    auto lambdaSS8 = [this](std::string&){
        ListIndicators.push_back(new LIP_SS8);
    };
    std::map<std::string, std::function<void(std::string& Type)>> parseHandler;
    parseHandler[i5N] = lambda5N;
    parseHandler[iSwitchStatus] = lambdaSS8;
    Parser::parseConfigurarion(Configuration, parseHandler);
}

Page::Page(){
    pageBuffer = nullptr;
    init();
    for(const auto& ind : ListIndicators){
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
    uint16_t countFillBuffer = 0;
    for(int i = 0; i < ListIndicators.size(); ++i){
        offsetStart = offsetEnd;
        offsetEnd += ListIndicators[i]->getDataSize();
        if(ListIndicators[i]->update()){
            ++countFillBuffer;
            std::vector<uint8_t> resultData = ListIndicators[i]->getValue();
            pageBuffer->addData(resultData, offsetStart, offsetEnd);
        }
    }
    if(countFillBuffer == ListIndicators.size()){
        return true;
    }

    return false;
}

//получить общее колличество байт индикаторов
uint16_t Page::getByteIndicators(){
    uint16_t sizeSegment = 0;
    for(const auto& ind : ListIndicators){
        sizeSegment += ind->getDataSize();
    }
    return sizeSegment;
}

//установить новую страницу
void Page::setIndication(std::string page){
    std::vector<std::string> newIndication = Parser::splitString("\r\n", page);
    for(int ind = 0; ind < ListIndicators.size(); ++ind){
        ListIndicators[ind]->setParameter(newIndication[ind]);
    }
}

uint16_t Page::getSizeSegment(){
    return sizeSegment;
}

void Page::setBuffer(Buffer* newBuffer){
    pageBuffer = newBuffer;
}

void Page::stopSlot(){
    for(const auto& ind : ListIndicators){
        ind->stopSlot();
    }
}

void Page::startSlot(){
    for(const auto& ind : ListIndicators){
        ind->startSlot();
    }
}

//для DMA
void updateDMA(){
    pageFunction->update();
}