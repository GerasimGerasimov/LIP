#include "LIP_5Nx.h"

#define TYPE 0
#define DECIMAL 1

#define INDICATOR_XN_SIZE_OPTION 3
#define INDICATOR_XN_TYPE_ANODE 1
#define INDICATOR_XN_DATA_SIZE 2

LIP_XNx::LIP_XNx(std::vector<std::string>& ConfigOption){
    if(ConfigOption.size() == INDICATOR_XN_SIZE_OPTION){
        setTypeAnode(ConfigOption[INDICATOR_XN_TYPE_ANODE] == "A");
        DataSize = std::stoi(ConfigOption[INDICATOR_XN_DATA_SIZE]);
    }
}

//получить список байт на отправку в SPI
std::vector<uint8_t> LIP_XNx::getValue(){
    
    std::string data;
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID)){
        data = connectErrorStr;
    }
    else{
        data = parametrControl.isErrorParsing() ? parseErrorStr : parametrControl.getValueStr();
        transformNumDecimal(data);
        transformSizeSring(data);
    }
    std::vector<uint8_t> result;
    result.reserve(DataSize);
    for(auto i = data.rbegin(); i != data.rend(); ++i){
        if(*i == '.'){
            dot = true;
            continue;
        }
        uint8_t symbol = getChar(*i);
        result.push_back(symbol);
    }


    return result;
}

bool LIP_XNx::update(){
    if(parametrControl.update()){
        updating = true;
        return true;
    }
    updating = false;
    return false;
}

void LIP_XNx::setParameter(std::string& param){
    if(parametrControl.setParameter(param)){
        std::vector<std::string>* optionParam = parametrControl.getOption();
        if(optionParam){
            type = (optionParam->at(TYPE) == "RW") ? Type::RW : Type::R;
            numDecimal = std::stoi(optionParam->at(DECIMAL));
        }
    }
}

void LIP_XNx::stopSlot(){
    parametrControl.stopSlot();
    updating = false;
}

void LIP_XNx::startSlot(){
    parametrControl.startSlot();
}

const std::string LIP_XNx::parseErrorStr = "-----";
const std::string LIP_XNx::connectErrorStr = " . . . . .";

const char LIP_XNx::ASCIITable[96] = {
    0x00, 0x86, 0x22, 0x49, 0x2D, 0x6B, 0x53, 0x46,
    0x70, 0x0F, 0x63, 0x46, 0x80, 0x40, 0x80, 0x52,
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x09, 0x58, 0x58, 0x48, 0x4C, 0xA7,
    0x5D, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x7B,
    0x74, 0x30, 0x0E, 0x75, 0x38, 0x55, 0x37, 0x5C,
    0x73, 0x67, 0x49, 0x6D, 0x78, 0x1C, 0x3E, 0x7E,
    0x76, 0x6E, 0x5B, 0x02, 0x64, 0x39, 0x23, 0x08,
    0x02, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71, 0x7B,
    0x74, 0x30, 0x0E, 0x75, 0x38, 0x55, 0x54, 0x5C,
    0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x3E, 0x7E,
    0x76, 0x6E, 0x5B, 0x46, 0x30, 0x70, 0x41, 0x00
};

//получить ASCII символ для индикации
uint8_t LIP_XNx::getChar(char symbol){

    symbol = (symbol <= 0x20) ? 0 : symbol - 0x20;

    uint8_t result = typeAnode ? ~ASCIITable[symbol] : ASCIITable[symbol];

    if(dot){
        dot = false;
        result += 128;
    }
    return result;
}

//изменение строки под необходимый размер байт
void LIP_XNx::transformSizeSring(std::string& data){
    size_t pos = data.find('.');
    uint8_t size = DataSize;
    if(pos != std::string::npos && pos < size){
        ++size;
    }
    if(data.size() < size){
        u8 insertSize = size - data.size();
        std::string insertStr(insertSize, ' ');
        data.insert(0, insertStr);
    }
    else if(data.size() > size){
        u8 deleteSize = data.size() - size;
        data.erase(data.length() - deleteSize);
    }
}

void LIP_XNx::transformNumDecimal(std::string& data){
    size_t dotPos = data.find('.');

    if(dotPos == std::string::npos){
        if(numDecimal > 0){
            data += "." + std::string(numDecimal, '0');
        }
        return;
    }

    if(numDecimal == 0){
        // Удаляем точку и всё после неё
        data = data.substr(0, dotPos);
        return;
    }

    size_t fractionLen = data.size() - dotPos - 1;

    if(fractionLen == numDecimal){
        return; // Ничего не нужно менять
    }
    else if(fractionLen > numDecimal){
        data = data.substr(0, dotPos + 1 + numDecimal); // Обрезаем
    }
    else{
        data += std::string(numDecimal - fractionLen, '0'); // Дополняем
    }
}

LIP_XNx::~LIP_XNx(){}
