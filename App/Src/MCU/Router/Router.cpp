#include "Router.h"
#include "Resources/InternalResources.h"
#include "DMAIndicator.h"
#include "Message/Message.h"
#include "ini/parser.h"
#include "Indicator/ControlSlot.h"

//#include "OutStream.h"
#include "ramdata.h"

Router::Router(){
    currentPage = 0; 
    std::string pages = InternalResources::getInstance().getItemStringByName("Pages");
    setIndicatorSlots();
    Pages = Parser::splitString("/", pages);
    setPage();
    page.setAppIndicatorSlots(IndicatorSlots);
    bufferData.setSizeBuffer(page.getSizeSegment());
    page.setBuffer(&bufferData);
    DMAIndicator::getInstance().setMemoryBaseAddr(bufferData);
}

void Router::setPage(){
    std::string setStartPage = InternalResources::getInstance().getItemStringByName(Pages[currentPage].c_str());
    page.setIndication(setStartPage);
    page.setAppIndicatorSlots(IndicatorSlots);
}

void Router::setIndicatorSlots(){
    std::string registers = InternalResources::getInstance().getItemStringByName("SLOTS");
    std::vector<std::string> regSlot = Parser::splitString("/", registers);
    for(const auto& n : regSlot){
        std::string str = InternalResources::getInstance().getItemStringByName(n.c_str());
        if(str != ""){
            ControlSlot* newIndicatorSlot = new ControlSlot;
            newIndicatorSlot->setParameter(str);
            IndicatorSlots[n] = newIndicatorSlot;
        }
    }
}

bool Router::updateIndicatorSlots(){
    for(const auto& slot : IndicatorSlots){
        if(slot.second->update()){
            ++countUpdateSlot;
        }
    }
    if(countUpdateSlot == IndicatorSlots.size()){
        return true;
    }
    return false;
}

void Router::stopIndicatorSlots(){
    for(auto& slot : IndicatorSlots){
        slot.second->stopSlot();
    }
}

void Router::startIndicatorSlots(){
    for(auto& slot : IndicatorSlots){
        slot.second->startSlot();
    }
}

Router& Router::getInstance(){
    static Router router;
    return router;
}

void Router::ProcessMessage(TMessage* m){
    if(m->event == Event::KEYBOARD){
        switch(m->p1){
        case 1:
            if(currentPage > 0){
                --currentPage;
                setPage();
            }
            break;
        case 2:
            if(currentPage < Pages.size() - 1){
                ++currentPage;
                setPage();
            }
            break;
        default:
            break;
        }
    }
    page.ProcessMessage(m);
}

void Router::setTask(Router::Task task){

}

void Router::update(){
    if(bufferData.getStatus() == Buffer::Status::EMPTY){
        updateIndicatorSlots();
        if(page.update()){
            bufferData.setFillStatus();
            page.stopSlot();
            stopIndicatorSlots();
            countUpdateSlot = 0;
        }
    }
}

void Router::setEmptyBufferStatus(){
    bufferData.setEmptyStatus();
    startIndicatorSlots();
    page.startSlot();
}

bool Router::isFillBuffer(){
    return (bufferData.getStatus() == Buffer::Status::FILL);
}

uint16_t Router::getBufferSize(){
    return bufferData.getSize();
}

ControlSlot* Router::getAppSlot(std::string name){
    if(IndicatorSlots.count(name)){
        return IndicatorSlots.at(name);
    }
    return nullptr;
}

Router::~Router(){
    for(auto& slot : IndicatorSlots){
        delete slot.second;
        slot.second = nullptr;
    }
}
