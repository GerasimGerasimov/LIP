#include "Router.h"
#include "Resources/InternalResources.h"
#include "DMAIndicator.h"

#include "OutStream.h"


Router::Router() {
	currentPage = InternalResources::getInstance().getItemStringByName("Page1");

	page.setIndication(currentPage);
	bufferData.setSizeBuffer(page.getSizeSegment());
	page.setBuffer(&bufferData);
	DMAIndicator::getInstance().setMemoryBaseAddr(bufferData);
}

Router& Router::getInstance() {
	static Router router;
	return router;
}

void Router::ProcessMessage(TMessage* m) {
}

void Router::setTask(Router::Task task){

}

void Router::update() {
	if (page.update()) {
		DMAIndicator::getInstance().DMAstart(bufferData.getSize());
	}
	
}
