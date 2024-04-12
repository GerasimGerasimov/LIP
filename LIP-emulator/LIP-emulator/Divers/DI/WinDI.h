#pragma once

#include "BaseWindow.h"

#include <vector>
#include <memory>
#include <map>

class WinDI : public BaseObject
{
private:
	std::vector<HWND> ButtonDI;
	
	void createButtonDI();
	const int countDI = 16;
	RECT rectButton;
	int widthButton;
	int heightButton;
	void create();
public:
	WinDI(Parameter param);
};

