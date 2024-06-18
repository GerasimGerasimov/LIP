#include "WinLIPModule.h"
#include "ini/parser.h"

WinLIPModule::WinLIPModule(Parameter param, std::vector<std::string> Config, RECT newBorder) : IndicatorContainer(param){
    Configuration = Config;
    border = newBorder;
    int count = 0; //TODO пока только 5N индикатороы count = Configuration.size();

    Parser::parseConfigurarion(Configuration, 
        [&count](std::string&){
            ++count;
        });

    if(count > 0){
        widthIndicator = param.rect.right - param.rect.left - border.left - border.right;
        heightIndicator = (param.rect.bottom - param.rect.top + (count - 1) * indent) / count - border.bottom - border.top;

        createSegment();
    }
}

void WinLIPModule::createSegment(){
    BaseWindow::Parameter param;
    param.parrent = hwnd;
    param.rect.left = border.left;
    param.rect.top = border.top;
    param.rect.right = width - border.right;
    param.rect.bottom = param.rect.top + heightIndicator;

    auto lambdaCreateSegment = [this, &param](std::string&){
        Indicators.push_back(std::make_unique<WinLIP_5Nx>(param));
        param.rect.top += heightIndicator + indent;
        param.rect.bottom += heightIndicator + indent;
    };

    Parser::parseConfigurarion(Configuration, lambdaCreateSegment);
}

//RECT WinLIPModule::getRect(int CountIndicator) {
//	RECT rect;
//	rect.left = 0;
//	rect.top = 0;
//	rect.right = widthChildIndicator + border.left + border.right;
//	rect.bottom = border.top + border.bottom + CountIndicator * heightChildIndicator + (CountIndicator - 1) * indent;
//	return rect;
//}
