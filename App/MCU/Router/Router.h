#pragma once

#include "Page/Page.h"

#include <string>

class TMessage;

//содержит Page и управляет страницами
class Router
{
private:
	Router();
	Router(const Router&) = delete;
	Router& operator=(const Router&) = delete;
	Router(const Router&&) = delete;
	Router& operator=(const Router&&) = delete;
	//Buffer bufferSender;
	Buffer bufferData;
public:
	struct Task {
		bool isDone;
		std::string NextPage;
		void* props;
	};

	static Router& getInstance();
	Page page;
	std::string currentPage;
	void ProcessMessage(TMessage* m);
	void setTask(Task task);
	void update();
};

