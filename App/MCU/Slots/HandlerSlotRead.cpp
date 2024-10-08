#include "HandlerSlotRead.h"
#include "Helpers/utils.h"
#include "ramdata.h"

enum class ModbusReadRegister
{
    CMD_POS = 1,
    ANSW_BYTES_POS = 2,
    ANSW_DATA_POS = 3
};

bool isReadCmdGotAnError(u8 cmdcode){
    return (bool)((cmdcode & 0x80) != 0);
}

void HandlerSlotRead::parseSlotRead(Slot* slot, u8* reply){
    try{
        if(slot->RespondLenghtOrErrorCode <= 0) throw (int)RespondErrorCodes::LNK;
        if(slot->isStateFlag(Slot::StateFlags::CRC_ERR)) throw (int)RespondErrorCodes::CRCE;
        if(isReadCmdGotAnError(reply[(u8)ModbusReadRegister::CMD_POS])) throw (int)RespondErrorCodes::CMD;
        u8 regs_count = reply[(u8)ModbusReadRegister::ANSW_BYTES_POS] >> 1;
        Utils::swp_copy_u16((u8*)&reply[(u8)ModbusReadRegister::ANSW_DATA_POS],
            (u16*)slot->InputBuf.data(),
            regs_count);
        RAM_DATA.data[0] = reply[0];
        RAM_DATA.data[1] = reply[1];
        RAM_DATA.data[2] = reply[2];
        RAM_DATA.data[3] = reply[3];
        RAM_DATA.data[4] = reply[4];
        RAM_DATA.data[5] = reply[5];
        //RAM_DATA.data[6] = reply[6];
        //RAM_DATA.data[7] = reply[7];
        slot->InputBufValidBytes = regs_count;
        slot->setFlag(Slot::StateFlags::DATA_VALID);
        slot->setFlag(Slot::StateFlags::COMPLETE_READ);
        slot->resetFlag(Slot::StateFlags::NO_VALID);
        
    }
    catch(int e){
        
        slot->InputBufValidBytes = 0;
        slot->resetFlag(Slot::StateFlags::DATA_VALID);
        slot->setFlag(Slot::StateFlags::NO_VALID);
    }
}
