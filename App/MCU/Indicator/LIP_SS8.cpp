#include "LIP_SS8.h"
#include "DevicePollManager/Slot.h"
#include "DevicePollManager/DevicePollManager.h"
#include "DevicePollManager/Devices.h"
#include "Slots/HandlerSlotRead.h"
#include "ini/IniResources.h"
#include "ini/parser.h"
#include "Resources/InternalResources.h"
#include "Parameters/Parameter.h"
#include "Parameters/IniString.h"
#include "Slots/SlotHandlerType.h"

#define DEVICE 0
#define SECTION 1
#define NAME 2
#define TYPE 3

#define COMAND_READ 3

bool LIP_SS8::setIsignal(){
    std::string dev = Devices::getInstance().getSourceOfDev(parameter.Device.c_str());
    ItemLimits item = InternalResources::getInstance().getItemLimitsByName(dev.c_str());
    IniParser::getInstance().setRoot(item.RootOffset, item.Size);
    if(!IniParser::getInstance().setSectionToRead(parameter.Section.c_str())){  //если нет секции в .ini
        return false;
    }
    TSectionReadResult readChar{NULL, 0};
    std::string readResult;
    size_t pos;
    do{
        readResult = "";
        readChar = IniParser::getInstance().getNextTagChar();
        readResult.append(readChar.tag, readChar.result);
        if(readResult == ""){
            return false; //если .ini закончился и ничего не нашлось
        }
        pos = readResult.find(parameter.Name);
    } while(pos == std::string::npos);

    pos = readResult.find('=');
    std::string number = readResult.substr(0, pos);
    ISignal* s = IniString::getSignal(dev, parameter.Section, readChar.tag, readChar.result);
    parameter.resources = dynamic_cast<Parameter*>(s);
    slot->StartAddrOffset = parameter.resources->getAddr();
    return true;
}

void LIP_SS8::createReadCmd(){
    std::string RegHexAddr = parameter.resources->getRegHexAddr();
    const u8 DevAddr = Devices::getInstance().getDevNetWorkAddr(parameter.Device);
    slot->TimeOut = 50;
    std::vector<u8> comand(6);//TODO сделать динамический размер
    u8 count = 0;
    comand[count++] = DevAddr;
    comand[count++] = COMAND_READ;
    u16 addr = (u16)std::stoul(RegHexAddr, nullptr, 16);
    comand[count++] = (u8)(addr >> 8) & 0x00FF;
    comand[count++] = (u8)(addr & 0x00FF);
    u8 sizeByte = parameter.resources->getSizeByte();
    comand[count++] = 0;
    comand[count++] = sizeByte / 2;
    //count += 2;
    //FrameEndCrc16(comand.data(), count);//
    slot->addcmd(comand);
}

void LIP_SS8::clear(){
    parameter.Device = "";
    parameter.Section = "";
    parameter.Name = "";
    if(parameter.resources){
        delete parameter.resources;
        parameter.resources = nullptr;
    }
    slot->setFlag(Slot::StateFlags::SKIP_SLOT);
}

std::string LIP_SS8::getValueStr(){
    TSlotHandlerArsg args = {&slot->InputBuf[0], slot->InputBufValidBytes, slot->StartAddrOffset, slot->LastAddrOffset};
    std::string value = parameter.resources->getValue(args, "");
    return value;
}

LIP_SS8::LIP_SS8(){
    DataSize = 2;
    slot = new Slot;
    stopSlot();
    slot->onData = HandlerSlotRead::parseSlotRead;
    DevicePollManager::getInstance().addSlot(slot);
}

std::vector<uint8_t> LIP_SS8::getValue(){
    std::string data = getValueStr();
    unsigned short number;
    number = stoul(data);
    std::vector<uint8_t> result(DataSize);

    result[0] = number;
    result[1] = number >> 8;
    return result;
}

void LIP_SS8::setParameter(std::string param){
    clear();
    if(param == ""){
        return;
    }
    std::vector<std::string> page = Parser::splitString("/", param);
    parameter.Device = page[DEVICE];
    parameter.Section = IniResources::getSection(page[SECTION]);
    parameter.Name = page[NAME];
    parameter.type = (page[TYPE] == "RW") ? Type::RW : Type::R;

    if(setIsignal()){
        errorParsing = false;
        createReadCmd();
        startSlot();
    }
    else{
        errorParsing = true;
    }
}

bool LIP_SS8::update(){
    if(errorParsing){
        return true;
    }
    if(slot->isStateFlag(Slot::StateFlags::COMPLETE_READ)){
        return true;
    }
    return false;
}

void LIP_SS8::stopSlot(){
    slot->setFlag(Slot::StateFlags::SKIP_SLOT);
    slot->resetFlag(Slot::StateFlags::COMPLETE_READ);
}

void LIP_SS8::startSlot(){
    if(errorParsing){
        return;
    }
    slot->resetFlag(Slot::StateFlags::SKIP_SLOT);
}
