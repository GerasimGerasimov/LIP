#pragma once

#include "BaseWindow.h"
#include "Segment.h"

#include <vector>
#include <memory>

// 7-сегментный индикатор
class Win7Segment : public BaseWindow
{
private:
	 
	std::vector<std::unique_ptr<BaseWindow>> Segments;
	std::vector<RECT> LocationSegments;

	void fillLocationSegment();
	RECT border;
	int segmentWeight;
public:
	Win7Segment(Parameter param);

	//void on1(HDC hdc);
	void createSegment();
	//int getWidth() const;
	//int getHeight() const;
	//void turnOff();
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