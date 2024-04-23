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
    std::string Config = InternalResources::getInstance().getItemStringByName((char*)"Config");
    Configuration = Parser::splitString(" ", Config);
    for (const auto& ind : Configuration) {
        if (ind == "5N") {
            ListIndicators.push_back(new LIP_5Nx);
        }
    }
}

Page::Page(){
    Indicator *indicator;
    int sizeSegment = 0;
    init();
    //for(int i = 0; i < 3; ++i){
    //    indicator = new LIP_5Nx;
    //    ListIndicators.push_back(indicator);
    //    sizeSegment += indicator->getDataSize();
    //}
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

void Page::update(){
    //bufferSender.swapStatus();
    //bufferData.swapStatus();
    //std::stringstream stream;

    //static unsigned short value1 = 0;
    //stream << std::setfill('0') << std::setw(5) << value1;
    // ++value1;
    //str = "";
    //stream >> str;
    //std::vector<uint8_t> res = ListIndicators[0]->getValue(str);
    //bufferData.addData(res);
    //res = ListIndicators[1]->getValue(str2);
    //bufferData.addData(res);
    //res = ListIndicators[2]->getValue(str3);
    //bufferData.addData(res);
    //bufferSender = bufferData;
    //Indicator::bringOutValue();
}

void updateDMA(){
    pageFunction->update();
}