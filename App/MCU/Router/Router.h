#pragma once

#include <string>

class Page;
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
	Page* page;
	Page* BackPage;
	void ProcessMessage(TMessage* m);
	void setTask(Task task);
};

