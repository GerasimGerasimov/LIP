/**
  ******************************************************************************
  * @brief   Файл содержит определения, макросы и декларирование функций для 
  * работы с ключом параметров на Fram
  ******************************************************************************
  * @Заметки:
  *
  * 
  * 
  * 
  * 
  * 
  *
  * 
  ******************************************************************************
  */
#include "stm32f10x.h"//библиотека StdPeriph_Driver

// команды для работы с FRAM
#define FREAD 0x03      //Read Status Register 0 - 255
#define FWRITE 0x02     //Write Status Register
#define FREAD_bkp 0x0B  //Read Status Register 256- 511
#define FWRITE_bkp 0x0A  //Write Status Register

#define WREN 0x06       //Set Write Enable Latch
#define WRDI 0x04       //Write Disable 	
#define RDSR 0x05       //Read Status Register
#define WRSR 0x01       //Write Status Register

/* Ниже представлены  определения и макросы для сигналов управления FRAM
  ******************************************************************************
  * Fram_SIO - линия данных 
  * Fram_SCK - линия тактирования 
  * Fram_CS - Chip select  
  * 
  * Fram_SIO_IN - состояние линии данных
  * MACROS_on - выставить сигнал в 1
  * MACROS_off - выставить сигнал в 0
  * 
  * Макросы прописываются индивидуально для каждого приложения 
  * (зависит от того, какой микроконтроллер используется, 
  * на какие пины каких портов посажены сигналы управления, смотри схему)
  ******************************************************************************
  */
#define Fram_SIO GPIO_Pin_14 
#define Fram_SCK GPIO_Pin_15 
#define Fram_CS  GPIO_Pin_13 

#define Fram_SIO_IN GPIOB->IDR & Fram_SIO
#define Fram_SIO_on GPIOB->BSRR = GPIO_Pin_14
#define Fram_SCK_on GPIOB->BSRR = GPIO_Pin_15 
#define Fram_CS_on  GPIOB->BSRR = GPIO_Pin_13  

#define Fram_SIO_off GPIOB->BRR = GPIO_Pin_14 
#define Fram_SCK_off GPIOB->BRR = GPIO_Pin_15 
#define Fram_CS_off  GPIOB->BRR = GPIO_Pin_13  

//другие определения
#define delay 30 //задержка 250нс =5
#define delay_sck 30 //задержка для клока

#define fram_sector 0 //основной сектор
#define fram_bkp 1 //резервный
#define FramTmpBufferSize 256

u8 ReadSR(void);

extern void fram_pulse(u8 Delay);//тактирование одного бита
void fram_wr_byte(u8 Data_byt);//передача одного байта, данные в функцию отправляются сразу
void fram_wr_byte(u8 Data_byt); //чтение одного байта, возвращается байт
void Fram_CS_Up(void);
void Fram_CS_Down(void);
void Fram_sio_mode(u8 mode);

extern void fram_wr_massive(u8 *massive, u16 n, u8 Addr, u8 Mem_half);
extern void fram_rd_massive(u8 *massive, u16 n, u8 Addr, u8 Mem_half);

extern u8 Fram_Buffer[FramTmpBufferSize];
extern void check_fram (void);

