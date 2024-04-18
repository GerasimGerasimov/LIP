#include "Router.h"


Router::Router() {
	CurrentPage = nullptr;
	BackPage = nullptr;
}

Router& Router::getInstance() {
	static Router router;
	return router;
}

void Router::ProcessMessage(TMessage* m) {
}

void Router::setTask(Router::Task task){

}