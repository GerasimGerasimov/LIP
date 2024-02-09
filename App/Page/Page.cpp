#include "Page.h"

#include "Indicator/LIP_5Nx.h"

Page::Page(){
    Indicator *indicator;
    for(int i = 0; i < 3; ++i){
        indicator = new LIP_5Nx;
        List.push_back(indicator);
    }
    str = "2356.8";
    str2 = "qwert";
    str3 = "wasd.f";
}

Page::~Page(){
    for(auto& n : List){
        delete n;
    }
}

void Page::update(){
    List[0]->setValue(str3);
    List[1]->setValue(str2);
    List[2]->setValue(str);
    Indicator::bringOutValue();
}