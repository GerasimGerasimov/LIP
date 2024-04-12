#include "BaseObject.h"

BaseObject::BaseObject(Parameter param) {
	Parrent = param.parrent;
	rect.left = param.rect.left;
	rect.top = param.rect.top;
	rect.right = param.rect.right;
	rect.bottom = param.rect.bottom;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

BaseObject::~BaseObject() {}
