#pragma once

#include "BaseWindow.h"
#include "Segment.h"

#include "vector"

class Win7Segment : public BaseWindow
{
private:

	std::vector<Segment*> Segments;
	std::vector<RECT> LocationSegments;

	void fillLocationSegment();
	RECT border;
	int segmentWeight;
public:
	Win7Segment(Parameter param);
	void init() override;
	void on1(HDC hdc);
	void createSegment();
};

/*
	порядок расположения сегментов
		1
     _______
	|		|
  6 |		| 2
	|_______|
	|	7	|
  5 |		| 3
	|_______|  .8
		4

*/