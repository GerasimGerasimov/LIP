#include "Router.h"
#include "Resources/InternalResources.h"

#include "OutStream.h"


Router::Router() {
	currentPage = InternalResources::getInstance().getItemStringByName((char*)"Page1");
	lip::cout << currentPage;
	page.setIndication(currentPage);
}

Router& Router::getInstance() {
	static Router router;
	return router;
}

void Router::ProcessMessage(TMessage* m) {
}

void Router::setTask(Router::Task task){

}