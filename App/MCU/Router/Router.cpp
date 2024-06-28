#include "Router.h"
#include "Resources/InternalResources.h"
#include "DMAIndicator.h"

//#include "OutStream.h"


Router::Router(){
    currentPage = InternalResources::getInstance().getItemStringByName("Page1");

    page.setIndication(currentPage);
    bufferData.setSizeBuffer(page.getSizeSegment());
    page.setBuffer(&bufferData);
    DMAIndicator::getInstance().setMemoryBaseAddr(bufferData);
}

Router& Router::getInstance(){
    static Router router;
    return router;
}

void Router::ProcessMessage(TMessage* m){}

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
