#include "WinDI.h"

#include <string>
#include <sstream>

void WinDI::createButtonDI(){

    for(int i = 0; i < countDI; ++i){
        std::string nameDI = "DI_";
        std::stringstream ss;
        ss << i;
        nameDI += ss.str();
        HWND hwndDI = CreateWindowA("button", nameDI.c_str(), WS_VISIBLE | WS_CHILD /*| BS_AUTOCHECKBOX */| WS_TABSTOP | WS_BORDER | BS_PUSHBUTTON, rect.left, rect.top, widthButton, heightButton, Parrent,
            NULL, NULL, NULL);
        ButtonDI.push_back(hwndDI);
        OffsetRect(&rect, 0, heightButton);
    }
}

WinDI::WinDI(Parameter param) : BaseObject(param){
    widthButton = 65;
    heightButton = 25;
    width = widthButton;
    height = heightButton * countDI;
    createButtonDI();
}

int WinDI::getDIPush(HWND Param){
    for(int i = 0; i < ButtonDI.size(); ++i){
        if(Param == ButtonDI[i]){
            return (1 << i);
        }
    }
    return -1;
}
