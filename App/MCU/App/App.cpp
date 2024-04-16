#include "App.h"

App::App() {
}

App& App::getInstance() {
	static App app;
	return app;
}
