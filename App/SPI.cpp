#include "SPI.h"
#include "ramdata.h"

SPI::SPI(){
    ++RAM_DATA.counter1;
}

void SPI::spi_send(uint8_t data){
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET){
      
    };  // ждём пока данные уйдут
    SPI_I2S_SendData(SPI1,data);
    ++RAM_DATA.counter2;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){
      ++RAM_DATA.counter3;
    };
}

uint8_t SPI::spi_receve(){

}