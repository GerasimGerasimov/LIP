#include "Page.h"
#include "Indicator/LIP_5Nx.h"
#include "Indicator/LIP_SS8.h"
#include "Indicator/LIP_SS8_Bl_2R.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"
#include "Message/Message.h"

void Page::init(){
    std::string Config = InternalResources::getInstance().getItemStringByName("Config");
    Configuration = Parser::splitString(" ", Config);

    auto lambdaXN = [this](std::vector<std::string>& Config){
        std::string type;
        uint8_t num = 0;
        if(Config.size() == 3){
            type = Config[1];
            num = std::stoi(Config[2]);
        }
        ListIndicators.push_back(new LIP_XNx(num));
        bool typeA = (type == "A");
        ListIndicators.back()->setTypeAnode(typeA);
    };

    auto lambdaSS8 = [this](std::vector<std::string>& Config){
        ListIndicators.push_back(new LIP_SS8);
    };

    auto lambdaSS8_Bl_2R = [this](std::vector<std::string>& Config){
        ListIndicators.push_back(new LIP_SS8_Bl_2R);
    };
    std::map<std::string, std::function<void(std::vector<std::string>& Config)>> parseHandler;
    parseHandler[i5N] = lambdaXN;
    parseHandler[iSwitchStatus] = lambdaSS8;
    parseHandler[iSwitchStatusBlink2Reg] = lambdaSS8_Bl_2R;
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
    static uint16_t countFillBuffer = 0; //Проверка на заполнение всех слотов
    //Заполнение буфера в порядке расположения индикаторов
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
        //Все слоты заполнены
        countFillBuffer = 0;
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
void Page::setIndication(std::string& page){
    std::vector<std::string> newIndication = Parser::splitString("\r\n", page);
    for(int ind = 0; ind < ListIndicators.size(); ++ind){
        if(ind >= newIndication.size()){
            newIndication.push_back("");
        }
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

void Page::ProcessMessage(TMessage* m){
    for(const auto& ind : ListIndicators){
        ind->ProcessMessage(m);
    }
}

void Page::setAppIndicatorSlots(std::map<std::string, ControlIndicatorSlot*>& indSlots){
    for(const auto& ind : ListIndicators){
        ind->setAppSlots(indSlots);
    }
}
