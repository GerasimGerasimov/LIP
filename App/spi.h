#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"//библиотека STM

#ifdef __cplusplus
extern "C" void spi_send(uint8_t data);
#else
extern void spi_send(uint8_t data);
#endif
extern uint16_t spi_receve(void);


    
#endif