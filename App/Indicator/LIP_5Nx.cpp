#include "LIP_5Nx.h"
#include "ramdata.h"
#include "spi.h"


LIP_5Nx::LIP_5Nx(){
    ++RAM_DATA.counter2;
    CountSegment = 5;
}

void LIP_5Nx::setValue(uint8_t data[]){
    for(u8 i = CountSegment; i > 0; --i){
        SPI::getInstance().spi_send(~(data[i-1]));
    }
}