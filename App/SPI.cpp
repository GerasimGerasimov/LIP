#include "SPI.h"
#include "ramdata.h"


void SPI::spi_send(uint16_t data){
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET){
      
    };  // ждём пока данные уйдут
    SPI_I2S_SendData(SPI2,data);
    
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET){
      
    };
}

uint8_t SPI::spi_receve(){
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  // ждём пока данные появтся
    uint8_t received = SPI_I2S_ReceiveData(SPI2);
    return received;
}

SPI& SPI::getInstance(){
    static SPI spi;
    return spi;
}