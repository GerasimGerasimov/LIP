#include "IndicatorContainer.h"

//TODO взможно сделать обработку без static
//int IndicatorContainer::indent = 10;
//RECT IndicatorContainer::border = { 10, 10, 10, 10 };

std::vector<uint8_t> IndicatorContainer::splitData(std::vector<uint8_t>::iterator& iterator, int byte){
    std::vector<uint8_t> newData(byte);
    for(int i = 0; i < byte; ++i){
        newData[i] = *iterator;
        ++iterator;
    }
    return newData;
}

IndicatorContainer::IndicatorContainer(Parameter param) : BaseWindow(param){
    border = {10, 10, 10, 10};
    indent = 10;
    heightIndicator = 0;// param.rect.bottom - param.rect.top - border.bottom - border.top;
    widthIndicator = 0;// (param.rect.right - param.rect.left + (count - 1) * indent) / count - border.left - border.right;
}

void IndicatorContainer::setData(std::vector<uint8_t>& vecData){
    //итератор для сохранения используемого элемента
    std::vector<uint8_t>::iterator iterator = vecData.begin();
    
    //данные приходят в обратном порядке
    //для переворачивания данных обратно - отправляются данные начиная в с последнего индиккатора
    for(auto n = Indicators.rbegin(); n != Indicators.rend(); ++n){
        int byteCount = (*n)->getByteIndication();
        std::vector<uint8_t> dataIndicator;
        dataIndicator = splitData(iterator, byteCount);
        (*n)->setData(dataIndicator);
    }
}
