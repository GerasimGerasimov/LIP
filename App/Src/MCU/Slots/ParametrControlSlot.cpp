#include "ParametrControlSlot.h"
#include "DevicePollManager/DevicePollManager.h"
#include "DevicePollManager/Devices.h"
#include "Slots/HandlerSlotRead.h"
#include "ini/parser.h"
#include "ini/IniResources.h"
#include "Resources/InternalResources.h"
#include "Parameters/Parameter.h"
#include "Parameters/IniString.h"
#include "Slots/SlotHandlerType.h"

#define DEVICE 0
#define SECTION 1
#define NAME 2
#define TYPE 3

#define COMAND_READ 3

//очистить индикатор
void ParametrControlSlot::clear(){
    parameter.Device = "";
    parameter.Section = "";
    parameter.Name = "";
    if(parameter.resources){
        delete parameter.resources;
        parameter.resources = nullptr;
    }
    slot->clearFlags();
    slot->setFlag(Slot::StateFlags::SKIP_SLOT);
}

void ParametrControlSlot::createReadCmd(){
    std::string RegHexAddr = parameter.resources->getRegHexAddr();
    const u8 DevAddr = Devices::getInstance().getDevNetWorkAddr(parameter.Device);
    slot->TimeOut = 1000;
    std::vector<u8> comand;
    comand.reserve(6);
    comand.push_back(DevAddr);
    comand.push_back(COMAND_READ);
    u16 addr = (u16)std::stoul(RegHexAddr, nullptr, 16);
    comand.push_back((u8)(addr >> 8) & 0x00FF);
    comand.push_back((u8)(addr & 0x00FF));
    u8 sizeByte = parameter.resources->getSizeByte();
    comand.push_back(0);
    comand.push_back(sizeByte / 2);
    slot->addcmd(comand);
}

bool ParametrControlSlot::setIsignal(){
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

std::string ParametrControlSlot::getValueStr(){
    TSlotHandlerArsg args = {&slot->InputBuf[0], slot->InputBufValidBytes, slot->StartAddrOffset, slot->LastAddrOffset};
    std::string value = parameter.resources->getValue(args, "");
    return value;
}

ParametrControlSlot::ParametrControlSlot(){
    slot = new Slot;
    stopSlot();
    slot->onData = HandlerSlotRead::parseSlotRead;
    DevicePollManager::getInstance().addSlot(slot);
}

ParametrControlSlot::~ParametrControlSlot(){
    delete slot;
    slot = nullptr;
    DevicePollManager::getInstance().deleteSlot();
}

bool ParametrControlSlot::setParameter(std::string& param){
    clear();
    if(param == ""){
        return false;
    }
    std::vector<std::string> page = Parser::splitString("/", param);
    parameter.Device = page[DEVICE];
    parameter.Section = IniResources::getSection(page[SECTION]);
    parameter.Name = page[NAME];
    parameter.type = (page[TYPE] == "RW") ? Type::RW : Type::R;

    if(setIsignal()){
        createReadCmd();
        startSlot();
        return true;
    }
    else{
        return false;// !errorParsing
    }
}

void ParametrControlSlot::stopSlot(){
    slot->setFlag(Slot::StateFlags::SKIP_SLOT);
    slot->resetFlag(Slot::StateFlags::COMPLETE_READ);
}

void ParametrControlSlot::startSlot(){
    slot->resetFlag(Slot::StateFlags::SKIP_SLOT);
}

void ParametrControlSlot::setFlag(Slot::StateFlags newFlag){
    slot->setFlag(newFlag);
}

void ParametrControlSlot::resetFlag(Slot::StateFlags delFlag){
    slot->resetFlag(delFlag);
}

bool ParametrControlSlot::isStateFlag(Slot::StateFlags isFlag){
    return slot->isStateFlag(isFlag);
}
