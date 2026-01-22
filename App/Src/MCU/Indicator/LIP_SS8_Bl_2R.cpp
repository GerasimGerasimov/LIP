#include "LIP_SS8_Bl_2R.h"
#include "Message/Message.h"
#include "Indicator/ControlSlot.h"
#include "Router/Router.h"
#include "ini/parser.h"
#include "ramdata.h"

#define LED_SIZE 8
#define REG_SIZE 16

#define SWITCH_ON 0
#define SWITCH_ALARM 1
#define ISOL_WRN 2
#define ISOL_ALARM 3

#define SHIFT_GREEN 1
#define SHIFT_RED 2

#define SHIFT_BASE 1
#define SHIFT_ALARM 2

#define SYMBOL_BLINK '*'

void LIP_SS8_Bl_2R::setColorFunction(){
    ColorFunction[COLOR::BLACK] = resetLED;
    ColorFunction[COLOR::RED] = setRed;
    ColorFunction[COLOR::GREEN] = setGreen;
    ColorFunction[COLOR::YELLOW] = setYellow;
}

void LIP_SS8_Bl_2R::setBaseSetting(std::vector<std::string>& ConfigOption){
    if(ConfigOption.size() > 1){
        if(ConfigOption[1][1] == SYMBOL_BLINK){
            BaseSettingIndicator.Blink = true;
        }
        BaseSettingIndicator.CurrentColor = getColorByChar(ConfigOption[1][0]);
    }
}

LIP_SS8_Bl_2R::COLOR LIP_SS8_Bl_2R::getColorByChar(char symbol){
    COLOR result;
    switch(symbol){
    case 'B':
        result = COLOR::BLACK;
        break;
    case 'R':
        result = COLOR::RED;
        break;
    case 'G':
        result = COLOR::GREEN;
        break;
    case 'Y':
        result = COLOR::YELLOW;
        break;
    default:
        result = COLOR::BLACK;
        break;
    }
    return result;
}

// Цвета устанавливаются в регистр по светодиодам 
// Установка красного цвета
void LIP_SS8_Bl_2R::setRed(unsigned short indicator, unsigned short& result){
    result &= ~(SHIFT_GREEN << (indicator * 2));
    result |= SHIFT_RED << (indicator * 2);
}

// Установка зелёного цвета
void LIP_SS8_Bl_2R::setGreen(unsigned short indicator, unsigned short& result){
    result |= SHIFT_GREEN << (indicator * 2);
    result &= ~(SHIFT_RED << (indicator * 2));
}

// Установка жёлтого цвета (красный + зелёный)
void LIP_SS8_Bl_2R::setYellow(unsigned short indicator, unsigned short& result){
    result |= SHIFT_GREEN << (indicator * 2);
    result |= SHIFT_RED << (indicator * 2);
}

// Сброс цвета (чёрный/светодиод не светит)
void LIP_SS8_Bl_2R::resetLED(unsigned short indicator, unsigned short& result){
    result &= ~(SHIFT_GREEN << (indicator * 2));
    result &= ~(SHIFT_RED << (indicator * 2));
}

void LIP_SS8_Bl_2R::clear(){
    for(auto& row : SlotsControlList){
        // Удаляем содержимое под-векторов
        row.erase(row.begin(), row.end());
    }

    // Удаляем пустые под-векторы
    SlotsControlList.erase(SlotsControlList.begin(), SlotsControlList.end());
}

//Чтение данных из слотов и установка битов для цвета
unsigned short LIP_SS8_Bl_2R::updateColorLedState(std::vector<IndicatorSlot>* tagsFunction){
    unsigned short result = 0;
    ControlSlot* controlSlot;
    std::vector<std::pair<unsigned char, unsigned char>>* tempTagByte;
    unsigned short number;
    std::string data;
    for(auto& n : (*tagsFunction)){
        controlSlot = n.SlotControl;
        if(controlSlot->getnoValid() || controlSlot->isErrorParsing()){
            data = "0";
        }
        else{
            data = controlSlot->getValueStr();
        }

        number = stoul(data);

        tempTagByte = &(n.TagByte);  //Указатель для оптимизации
        for(int i = 0; i < tempTagByte->size(); ++i){
            if(number & (1 << (*tempTagByte)[i].first)){
                result |= (1 << (*tempTagByte)[i].second);
            }
        }
    }
    return result;
}

//Установка цвета на светодиод в зависимости от приоритета
unsigned short LIP_SS8_Bl_2R::setColorPriorityLED(unsigned short switchLED[]){
    unsigned short res;

    for(int i = 0; i < LED_SIZE; ++i){ //Привязано к светодиодам
    //Определение приоритетов
        setPriorityLED(switchLED, i);

        switch(Priority){
        case LIP_SS8_Bl_2R::PRIORITY::ALARM:
            //Мигание зелёным
            if(Blink){
                setGreen(i, res);
            }
            else{
                resetLED(i, res);
            }
            break;
        case LIP_SS8_Bl_2R::PRIORITY::ISOLATION_ALARM:
            if(Blink){
                setYellow(i, res);
            }
            else{
                resetLED(i, res);
            }
            break;
        case LIP_SS8_Bl_2R::PRIORITY::ISOLATION_WARNING:
            setYellow(i, res);
            break;
        case LIP_SS8_Bl_2R::PRIORITY::SWICH:
            setRed(i, res);
            break;
        case LIP_SS8_Bl_2R::PRIORITY::BASE:
            //setGreen(i, res);
            ColorFunction.at(BaseSettingIndicator.CurrentColor)(i, res);
            break;
        }
    }

    return res;
}

//Установка приоритетов в зависимости от включенных сигналов
void LIP_SS8_Bl_2R::setPriorityLED(unsigned short switchLED[], int LED){
    if(switchLED[0] & (1 << LED)){
        Priority = PRIORITY::ALARM;
    }
    else if(switchLED[1] & (1 << LED)){
        Priority = PRIORITY::ISOLATION_ALARM;
    }
    else if(switchLED[2] & (1 << LED)){
        Priority = PRIORITY::ISOLATION_WARNING;
    }
    else if(switchLED[3] & (1 << LED)){
        Priority = PRIORITY::SWICH;
    }
    else{
        Priority = PRIORITY::BASE;
    }
}

LIP_SS8_Bl_2R::LIP_SS8_Bl_2R(std::vector<std::string>& ConfigOption){
    DataSize = 2;
    setColorFunction();
    setBaseSetting(ConfigOption);
    
}

std::vector<uint8_t> LIP_SS8_Bl_2R::getValue(){
    unsigned short switchON = 0;
    switchON = updateColorLedState(&(SlotsControlList[SWITCH_ON]));

    unsigned short switchAlarm = 0;
    if(SlotsControlList.size() > 1){
        switchAlarm = updateColorLedState(&(SlotsControlList[SWITCH_ALARM]));
    }

    unsigned short isolationWarn = 0;
    unsigned short isolationAlarm = 0;
    
    if(SlotsControlList.size() > 2){
        isolationWarn = updateColorLedState(&(SlotsControlList[ISOL_WRN]));
        if(SlotsControlList.size() > 3){
            isolationAlarm = updateColorLedState(&(SlotsControlList[ISOL_ALARM]));
        }
    }

    //Массив сигналов в зависимости от приоритета
    unsigned short switchLED[4] = {switchAlarm, isolationAlarm, isolationWarn, switchON};

    std::vector<uint8_t> result(DataSize);
    unsigned short res = setColorPriorityLED(switchLED);
    result[1] = res;
    result[0] = res >> 8;
    return result;
}



bool LIP_SS8_Bl_2R::update(){
    if(updating){
        return false;
    }

    uint16_t countUpdateSlot = 0;
    uint16_t slotsSize = 0;

    for(auto& func : SlotsControlList){
        for(auto& tag : func){
            ++slotsSize;
            if(tag.SlotControl->getUpdate()){
                //Слот, привязанный к индикатору обновился
                ++countUpdateSlot;
            }
        }
    }

    if(countUpdateSlot == slotsSize){
        updating = true;
        return true;
    }

    updating = false;
    return false;
}

void LIP_SS8_Bl_2R::ProcessMessage(TMessage* m){
    //Установка состояния в мигающем светодиоде по таймеру
    if(m->event == Event::TIMER){
        Blink = !Blink;
    }
}

//Установка параметров на старте или при изменении страницы
void LIP_SS8_Bl_2R::setParameter(std::string& param){
    clear();
    //Разделение на включение разных цветов по приоритетам
    std::vector<std::string> functionLed = Parser::splitString2Delim("(", ")", param);
    for(auto& func : functionLed){
        std::vector<std::string> tags = Parser::splitString(" ", func);
        std::vector<IndicatorSlot> slotsFunction;
        //Поиск разных слотов для 1 цвета
        for(auto& n : tags){
            IndicatorSlot indSlot;
            //Разделение на слот и значения битов
            std::vector<std::string> spltTag = Parser::splitString(":", n);
            indSlot.Tag = spltTag[0];
            if(spltTag.size() > 1){
                //Разделение значений для каждого светодиода 
                std::vector<std::string> spltVecBytes = Parser::splitString("/", spltTag[1]);
                if(spltVecBytes[0] == "*"){
                    // Автозаполнение
                    // R(S4:*/0.1)
                    // цвет(Слот:*/Стартовый бит.Сколько пропускать)
                    // Заполнение происходит с 0 светодиода по порядку
                    // Если количество бит превысит количество бит в регистре, заполнение прекратится
                    std::vector<std::string> spltPair = Parser::splitString(".", spltVecBytes[1]);
                    u16 startByte = stoi(spltPair[0]);
                    u16 skipByte = stoi(spltPair[1]);
                    for(int i = 0; i < LED_SIZE; ++i){
                        u16 curByte = (skipByte + 1) * i + startByte;
                        if(curByte >= REG_SIZE){
                            break;
                        }
                        std::pair<unsigned char, unsigned char> bytes = {curByte, i};
                        indSlot.TagByte.push_back(bytes);
                    }
                }
                else{
                    // Побитовое заполнение
                    for(auto& j : spltVecBytes){
                        // Разделение на бит из регистра и светодиод который надо включить
                        std::vector<std::string> spltPair = Parser::splitString(".", j);
                        // R(S4:0.0/1.1/2.2/3.3)
                        // цвет(Слот:Бит из регистра.Номер светодиода)
                        std::pair<unsigned char, unsigned char> bytes = {stoi(spltPair[0]), stoi(spltPair[1])};
                        indSlot.TagByte.push_back(bytes);
                    }
                }
            }
            slotsFunction.push_back(indSlot);
        }
        SlotsControlList.push_back(slotsFunction);
    }
}

//Установка указателей используемых слотов на существующие слоты
void LIP_SS8_Bl_2R::setAppSlots(std::map<std::string, ControlSlot*>& indSlots){
    for(auto& func : SlotsControlList){
        for(auto& n : func){
            if(indSlots.count(n.Tag)){
                n.SlotControl = indSlots.at(n.Tag);
            }
        }
    }
}

void LIP_SS8_Bl_2R::stopSlot(){
    updating = false;
}
