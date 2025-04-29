#include "Router.h"
#include "Resources/InternalResources.h"
#include "DMAIndicator.h"
#include "Message/Message.h"
#include "ini/parser.h"

//#include "OutStream.h"


Router::Router(){
    currentPage = 0; 
    std::string pages = InternalResources::getInstance().getItemStringByName("Pages");
    std::string registers = InternalResources::getInstance().getItemStringByName("REGISTERS");
    Pages = Parser::splitString("/", pages);
    setPage();
    bufferData.setSizeBuffer(page.getSizeSegment());
    page.setBuffer(&bufferData);
    DMAIndicator::getInstance().setMemoryBaseAddr(bufferData);
}

void Router::setPage(){
    std::string setStartPage = InternalResources::getInstance().getItemStringByName(Pages[currentPage].c_str());
    page.setIndication(setStartPage);
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
        if(page.update()){
            bufferData.setFillStatus();
            page.stopSlot();
        }
    }
}

void Router::setEmptyBufferStatus(){
    bufferData.setEmptyStatus();
    page.startSlot();
}

bool Router::isFillBuffer(){
    return (bufferData.getStatus() == Buffer::Status::FILL);
}

uint16_t Router::getBufferSize(){
    return bufferData.getSize();
}
