#include "ParametrControlSlotXN.h"

#define TYPE 3
#define DECIMAL 4

void ParametrControlSlotXN::setProps(std::vector<std::string>& page){
    ParametrControlSlot::setProps(page);
    type = (page[TYPE] == "RW") ? Type::RW : Type::R;
    numDecimal = std::stoi(page[DECIMAL]);
}

unsigned short ParametrControlSlotXN::getNumDecimal(){
    return numDecimal;
}
