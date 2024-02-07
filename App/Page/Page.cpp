#include "Page.h"

#include "Indicator/LIP_5Nx.h"

Page::Page(){
    Indicator *indicator;
    for(int i = 0; i < 3; ++i){
        indicator = new LIP_5Nx;
        List.push_back(indicator);
    }
    str = "01234";
    str2 = "56789";
    str3 = "ABCDE";
}

Page::~Page(){
    for(auto& n : List){
        delete n;
    }
}

void Page::update(){
    List[0]->setValue(str3.c_str());
    List[1]->setValue(str2.c_str());
    List[2]->setValue(str.c_str());
    Indicator::bringOutValue();
}