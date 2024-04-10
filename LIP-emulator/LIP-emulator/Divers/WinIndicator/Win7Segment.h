#pragma once

#include "BaseWindow.h"
#include "Segment.h"

#include "vector"

class Win7Segment : public BaseWindow
{
private:
	std::vector<Segment*> Segments;
	
public:
	Win7Segment(Parameter param);
	void init() override;
	void on1(HDC hdc);
	void createSegment();
};

