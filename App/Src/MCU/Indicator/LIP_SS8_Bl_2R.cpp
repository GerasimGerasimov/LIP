#include "LIP_SS8_Bl_2R.h"
#include "Message/Message.h"
#include "Indicator/ControlSlot.h"
#include "Router/Router.h"
#include "ini/parser.h"
#include "ramdata.h"

#define LED_SIZE 8
#define REG_SIZE 16

#define SHIFT_GREEN 1
#define SHIFT_RED 2

#define SYMBOL_BLINK '*'

void LIP_SS8_Bl_2R::setColorFunction(){
    ColorFunction[COLOR::BLACK] = resetLED;
    ColorFunction[COLOR::RED] = setRed;
    ColorFunction[COLOR::GREEN] = setGreen;
    ColorFunction[COLOR::YELLOW] = setYellow;
}

//Установка состояния светодиода по умолчанию (нет сигнала)
void LIP_SS8_Bl_2R::setBaseSetting(std::vector<std::string>& ConfigOption){
    if(ConfigOption.size() > 1){
        BaseSettingIndicator = getColorSettingByString(ConfigOption[1]);
    }
}

//Получаем цвет по символу из ресурсов
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

//При смене страницы
void LIP_SS8_Bl_2R::clear(){
    for(auto& row : SlotsControlList){
        // Удаляем содержимое под-векторов
        row.second.erase(row.second.begin(), row.second.end());
    }

    // Удаляем пустые под-векторы
    SlotsControlList.erase(SlotsControlList.begin(), SlotsControlList.end());
}

//Чтение данных из слотов и установка битов для цвета
unsigned short LIP_SS8_Bl_2R::updateColorLedState(std::vector<IndicatorSlot>& tagsFunction){
     unsigned short result = 0;
    ControlSlot* controlSlot;
    std::vector<std::pair<unsigned char, unsigned char>>* tempTagByte;
    unsigned short number;
    std::string data;
    for(auto& n : (tagsFunction)){
        controlSlot = n.SlotControl;
        if(controlSlot->getnoValid() || controlSlot->isErrorParsing()){
            //Нет связи или не правильно описаны ресурсы
            data = "0";
        }
        else{
            //Получаем значение регистра
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
unsigned short LIP_SS8_Bl_2R::setColorPriorityLED(std::vector<std::pair<ColorSettingLED, unsigned short>>& switchLED){
    unsigned short res = 0;

    for(int i = 0; i < LED_SIZE; ++i){ //Привязано к светодиодам
        unsigned short switchCountLED = 0;
        //Вначале массива switchLED более высокий приоритет
        for(auto& n : switchLED){
            if(n.second & (1 << i)){
                setColorBlinkLED(n.first, i, res);
                //Если нашёлся светодиод, дальше не смотрим - текущий приоритет выше
                break;
            }
            ++switchCountLED;
        }
        if(switchCountLED == switchLED.size()){
            //Если не найден включённый сигнал, включается цвет по умолчанию
            setColorBlinkLED(BaseSettingIndicator, i, res);
        }
    }

    return res;
}

//Установка цвета на светодиод, в зависимости от мигания
void LIP_SS8_Bl_2R::setColorBlinkLED(ColorSettingLED& colorSetting, unsigned short indicator, unsigned short& result){
    ColorFunction.at(colorSetting.CurrentColor)(indicator, result);
    if(colorSetting.Blink){
        if(BlinkIndicator){
            resetLED(indicator, result);
        }
    }
}

//Получает цвет и мигание светодиода по строке из ресурсов
LIP_SS8_Bl_2R::ColorSettingLED LIP_SS8_Bl_2R::getColorSettingByString(std::string& setting){
    ColorSettingLED resultSetting;
    resultSetting.CurrentColor = getColorByChar(setting[0]);
    if(setting.size() > 1){
        if(setting[1] == SYMBOL_BLINK){
            resultSetting.Blink = true;
        }
    }
    return resultSetting;
}

LIP_SS8_Bl_2R::LIP_SS8_Bl_2R(std::vector<std::string>& ConfigOption){
    DataSize = 2;
    setColorFunction();
    setBaseSetting(ConfigOption);
    
}

std::vector<uint8_t> LIP_SS8_Bl_2R::getValue(){
    std::vector<std::pair<ColorSettingLED, unsigned short>> switchLED;
    switchLED.reserve(SlotsControlList.size());
    for(auto& slotsContro : SlotsControlList){
        switchLED.emplace_back(slotsContro.first, updateColorLedState(slotsContro.second));
    }
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
        for(auto& tag : func.second){
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
        BlinkIndicator = !BlinkIndicator;
    }
}

//Установка параметров на старте или при изменении страницы
void LIP_SS8_Bl_2R::setParameter(std::string& param){
    clear();
    //Разделение на включение разных цветов по приоритетам
    //                      Цвет         Теги
    std::vector<std::pair<std::string, std::string>> functionLed = Parser::splitString2DelimInPrev("(", ")", param);
    for(auto& func : functionLed){
        std::pair<ColorSettingLED, std::vector<IndicatorSlot>> slotsFunction;
        //Получение настройки цвета по ресурсам
        slotsFunction.first = getColorSettingByString(func.first);
        //Получение списка тегов для данного цвета
        std::vector<std::string> tags = Parser::splitString(" ", func.second);
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
                        indSlot.TagByte.emplace_back(curByte, i);
                    }
                }
                else{
                    // Побитовое заполнение
                    for(auto& j : spltVecBytes){
                        // Разделение на бит из регистра и светодиод который надо включить
                        std::vector<std::string> spltPair = Parser::splitString(".", j);
                        // R(S4:0.0/1.1/2.2/3.3)
                        // цвет(Слот:Бит из регистра.Номер светодиода)
                        indSlot.TagByte.emplace_back(stoi(spltPair[0]), stoi(spltPair[1]));
                    }
                }
            }
            slotsFunction.second.push_back(std::move(indSlot));
        }
        SlotsControlList.push_back(std::move(slotsFunction));
    }
}

//Установка указателей используемых слотов на существующие слоты
void LIP_SS8_Bl_2R::setAppSlots(std::map<std::string, ControlSlot*>& indSlots){
    for(auto& func : SlotsControlList){
        for(auto& n : func.second){
            if(indSlots.count(n.Tag)){
                n.SlotControl = indSlots.at(n.Tag);
            }
        }
    }
}

void LIP_SS8_Bl_2R::stopSlot(){
    updating = false;
}
