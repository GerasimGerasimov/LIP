#include "LIP_5Nx.h"
#include "DevicePollManager/Slot.h"
#include"DevicePollManager/DevicePollManager.h"
#include "ini/parser.h"
#include "Resources/InternalResources.h"
#include "DevicePollManager/Devices.h"
#include "ini/IniResources.h"
#include "IniString.h"
#include "Parameter.h"
#include "crc16.h"
#include "Slots/HandlerSlotRead.h"
#include "OutStream.h"

#define DEVICE 0
#define SECTION 1
#define NAME 2
#define TYPE 3

LIP_5Nx::LIP_5Nx(){
    DataSize = 5;
    slot = new Slot;
    slot->Flags |= static_cast<u16>(Slot::StateFlags::SKIP_SLOT);
    slot->onData = HandlerSlotRead::parseSlotRead;
    DevicePollManager::getInstance().addSlot(slot);
}

//получить список байт на отправку в SPI
std::vector<uint8_t> LIP_5Nx::getValue(std::string& data) {
    std::vector<uint8_t> result;
    result.reserve(DataSize);
    for (auto i = data.rbegin(); i != data.rend(); ++i) {
        if (*i == '.') {
            dot = true;
            continue;
        }
        uint8_t symbol = getChar(*i);
        result.push_back(symbol);
    }


    return result;
}

//установить новый параметр
void LIP_5Nx::setParameter(std::string param) {
    if (param == "") {
        clear();
        return;
    }
    std::vector<std::string> page = Parser::splitString("/", param);
    parameter.Device = page[DEVICE];
    parameter.Section = IniResources::getSection(page[SECTION]);
    parameter.Name = page[NAME];
    lip::cout << parameter.Device << "\r\n" << parameter.Section << "\r\n" << parameter.Name << "\r\n";
    setIsignal();
    if (page[TYPE] == "RW") {
        parameter.type = Type::RW;
    }
    else {
        parameter.type = Type::R;
    }
}

const char LIP_5Nx::ASCIITable[96] = {
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
uint8_t LIP_5Nx::getChar(char symbol){
    if(symbol <= 0x20){
        symbol = 0;
    }
    else{
        symbol -= 0x20;
    }



    uint8_t result;

    if(typeKathode){
        result = ASCIITable[symbol];
    }
    else{
        result = ~ASCIITable[symbol];
    }

    if (dot) {
        dot = false;
        result += 128;
    }
    return result;
}

//очистить индикатор
void LIP_5Nx::clear() {
    parameter.Device = "";
    parameter.Section = "";
    parameter.Name = "";
}

void LIP_5Nx::setIsignal() {
    slot->Flags |= static_cast<u16>(Slot::StateFlags::SKIP_SLOT);
    std::string dev = Devices::getInstance().getSourceOfDev(parameter.Device.c_str());
    ItemLimits item = InternalResources::getInstance().getItemLimitsByName(dev.c_str());
    IniParser::getInstance().setRoot(item.RootOffset, item.Size);
    if (IniParser::getInstance().setSectionToRead(parameter.Section.c_str())) {
        TSectionReadResult readChar{ NULL, 0 };
        std::string readResult;
        size_t pos;
        do {
            readResult = "";
            readChar = IniParser::getInstance().getNextTagChar();
            readResult.append(readChar.tag, readChar.result);
            if (readResult == "")return;
            pos = readResult.find(parameter.Name);
        } while (pos == std::string::npos);

        lip::cout << readResult << "\r\n";
        pos = readResult.find('=');
        std::string number = readResult.substr(0, pos);
        lip::cout << number << "\r\n";
        ISignal* s = IniString::getSignal(dev, parameter.Section, readChar.tag, readChar.result);
        parameter.resources = dynamic_cast<Parameter*>(s);
        createReadCmd();
        slot->Flags &= ~(static_cast<u16>(Slot::StateFlags::SKIP_SLOT));
    }
}

void LIP_5Nx::createReadCmd() {
    std::string RegHexAddr = parameter.resources->getRegHexAddr();
    const u8 DevAddr = Devices::getInstance().getDevNetWorkAddr(parameter.Device);
    slot->TimeOut = 500;
    std::vector<u8> comand(6);
    u8 count = 0;
    comand[count++] = DevAddr;
    comand[count++] = 3;
    u16 addr = (u16)std::stoul(RegHexAddr, nullptr, 16);
    comand[count++] = (u8)(addr >> 8) & 0x00FF;
    comand[count++] = (u8)(addr & 0x00FF);
    u8 sizeByte = parameter.resources->getSizeByte();
    comand[count++] = 0;
    comand[count++] = sizeByte / 2;
    //count += 2;
    //FrameEndCrc16(comand.data(), count);
    slot->addcmd(comand);
}
