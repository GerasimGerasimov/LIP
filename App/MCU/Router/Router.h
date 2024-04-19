#pragma once

#include "Page/Page.h"

#include <string>

class TMessage;

class Router
{
private:
	Router();
	Router(const Router&) = delete;
	Router& operator=(const Router&) = delete;
	Router(const Router&&) = delete;
	Router& operator=(const Router&&) = delete;
public:
	struct Task {
		bool isDone;
		std::string NextPage;
		void* props;
	};

	static Router& getInstance();
	Page CurrentPage;

	void ProcessMessage(TMessage* m);
	void setTask(Task task);
};

