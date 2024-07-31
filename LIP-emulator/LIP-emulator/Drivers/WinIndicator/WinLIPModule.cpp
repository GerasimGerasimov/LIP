#include "WinLIPModule.h"
#include "ini/parser.h"
#include "WinLip_SwitchStatus_8.h"

WinLIPModule::WinLIPModule(Parameter param, std::vector<std::string> Config, RECT newBorder) : IndicatorContainer(param){
    Configuration = Config;
    border = newBorder;
    int countHeight = 0; //TODO пока только 5N индикатороы count = Configuration.size();
    std::map<std::string, std::function<void(std::string& Type)>> parseHandler;
    auto lambdaHeight = [&countHeight](std::string&){
        ++countHeight;
        };
    parseHandler[i5N] = lambdaHeight;
    parseHandler[iSwitchStatus] = lambdaHeight;
    Parser::parseConfigurarion(Configuration, parseHandler);

    if(countHeight > 0){
        widthIndicator = param.rect.right - param.rect.left - border.left - border.right;
        heightIndicator = (param.rect.bottom - param.rect.top + (countHeight - 1) * indent) / countHeight - border.bottom - border.top;

        createSegmentHeight();
    }
}

void WinLIPModule::insertRectParamHeight(BaseWindow::Parameter &param){
    param.rect.top += heightIndicator + indent;
    param.rect.bottom += heightIndicator + indent;
}

void WinLIPModule::createSegmentHeight(){
    BaseWindow::Parameter param;
    param.parrent = hwnd;
    param.rect.left = border.left;
    param.rect.top = border.top;
    param.rect.right = width - border.right;
    param.rect.bottom = param.rect.top + heightIndicator;

    auto lambdaCreateSegment5N = [this, &param](std::string&){
        Indicators.push_back(std::make_unique<WinLIP_5Nx>(param));
        insertRectParamHeight(param);
    };

    auto lambdaCreateSegmentSwitchStatus = [this, &param](std::string&){
        Indicators.push_back(std::make_unique<WinLip_SwitchStatus_8>(param));
        insertRectParamHeight(param);
    };
    std::map<std::string, std::function<void(std::string& Type)>> parseHandler;
    parseHandler[i5N] = lambdaCreateSegment5N;
    parseHandler[iSwitchStatus] = lambdaCreateSegmentSwitchStatus;
    Parser::parseConfigurarion(Configuration, parseHandler);
}

//RECT WinLIPModule::getRect(int CountIndicator) {
//	RECT rect;
//	rect.left = 0;
//	rect.top = 0;
//	rect.right = widthChildIndicator + border.left + border.right;
//	rect.bottom = border.top + border.bottom + CountIndicator * heightChildIndicator + (CountIndicator - 1) * indent;
//	return rect;
//}
