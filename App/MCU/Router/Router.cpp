#include "Router.h"
#include "Resources/InternalResources.h"

#include "OutStream.h"


Router::Router() {
	currentPage = InternalResources::getInstance().getItemStringByName("Page2");
	//lip::cout << currentPage;
	page.setIndication(currentPage);
	bufferData.setSizeBuffer(page.getSizeSegment());
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

	}
}
