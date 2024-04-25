#include "SignalFactoty.h"
#include "ini/parser.h"
#include "TU8BIT.h"
#include "TU16BIT.h"
#include "TU32BIT.h"
#include "TS16Bit.h"
#include "TFloat.h"
#include "TBit.h"
#include "TPrmList.h"

const std::map<std::string, std::function<ISignal*(ISignal::PropsPointers)>> SignalFactoty::TypeToSignal = {
	{"TByte", [](ISignal::PropsPointers props) {return new TU8BIT(props); }},
	{"TFloat", [](ISignal::PropsPointers props) {return new TFloat(props); }},
	{"TInteger", [](ISignal::PropsPointers props) {return new TS16BIT(props); }},
	{"TBit", [](ISignal::PropsPointers props) {return new TBit(props); }},
	{"TWORD", [](ISignal::PropsPointers props) {return new TU16BIT(props); }},
	{"TPrmList", [](ISignal::PropsPointers props) {return new TPrmList(props); }},
	{"TDWORD", [](ISignal::PropsPointers props) {return new TU32BIT(props); }}
};

SignalFactoty::SignalFactoty() {
}

SignalFactoty& SignalFactoty::getInstance() {
    static SignalFactoty factory;
	return factory;
}

ISignal::PropsPointers SignalFactoty::getSignalProps(const char* dev, const char* source, const int srcLen) {
	ISignal::PropsPointers res = { nullptr, nullptr, nullptr, nullptr, nullptr };
	res.dev = const_cast<char*>(dev);
	/*отделить номер параметра со знаком "=" от значащей части параметра (те что разделены слэшем)*/
	char* src = const_cast<char*>(source);
	int size = srcLen;
	//строки будут с символом "слэш" в конце!
	if (IniParser::getInstance().isDelimiterSizeLimited('=', src, size) != -1) {//нашёл знак "="
		res.pName = src;
		if (IniParser::getInstance().isDelimiterSizeLimited('/', src, size) != -1) {
			res.pComment = src;
			if (IniParser::getInstance().isDelimiterSizeLimited('/', src, size) != -1) {
				res.pType = src;
				if (IniParser::getInstance().isDelimiterSizeLimited('/', src, size) != -1) {
					res.pOptional = src;
				}
			}
		}
	}
	return res;
}

Scale::Props SignalFactoty::getScaleProps(const char* source, const int srcLen) {
	Scale::Props res = { nullptr, nullptr, 0 };
	/*отделить номер параметра со знаком "=" от значащей части параметра (те что разделены слэшем)*/
	char* src = const_cast<char*>(source);
	int size = srcLen;
	res.pKey = src;//Name заканчивается знаком "="
	if (IniParser::getInstance().isDelimiterSizeLimited('=', src, size) != -1) {//нашёл знак "="
		res.pValue = src;//строка опций для vars считывается до /r/n/
		res.ValueSize = size;//остаток длины строки
	}
	return res;
}

ISignal* SignalFactoty::getSignal(const ISignal::PropsPointers& props) {
	int size = 100;
	char* _pType = props.pType;
	std::string pType = IniParser::getInstance().getElement('/', &_pType, size);
	ISignal* s = (TypeToSignal.count(pType))
		? TypeToSignal.at(pType)(props)
		: nullptr;
	return s;
}

ISignal* SignalFactoty::getScale(const Scale::Props& props) {
	/*TODO тут надо парсить переданную строку и генерить объекты vars а они бывают от простых
	  ключ = значение, до сложных типа списков*/
	return new Scale(props);
}
