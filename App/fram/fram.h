/**
  ******************************************************************************
  * @file    fram.h
  * @author  IMD, Erlina
  * @version V1.0.0
  * @date    18-06-2013
  * @brief   ���� �������� �����������, ������� � �������������� ������� ��� 
  * ������ � ������ ���������� �� Fram
  ******************************************************************************
  * @�������:
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
#include "stm32f10x.h"//���������� StdPeriph_Driver

// ������� ��� ������ � FRAM
#define FREAD 0x03      //Read Status Register 0 - 255
#define FWRITE 0x02     //Write Status Register
#define FREAD_bkp 0x0B  //Read Status Register 256- 511
#define FWRITE_bkp 0x0A  //Write Status Register

#define WREN 0x06       //Set Write Enable Latch
#define WRDI 0x04       //Write Disable 	
#define RDSR 0x05       //Read Status Register
#define WRSR 0x01       //Write Status Register

/* ���� ������������  ����������� � ������� ��� �������� ���������� FRAM
  ******************************************************************************
  * Fram_SIO - ����� ������ 
  * Fram_SCK - ����� ������������ 
  * Fram_CS - Chip select  
  * 
  * Fram_SIO_IN - ��������� ����� ������
  * MACROS_on - ��������� ������ � 1
  * MACROS_off - ��������� ������ � 0
  * 
  * ������� ������������� ������������� ��� ������� ���������� 
  * (������� �� ����, ����� ��������������� ������������, 
  * �� ����� ���� ����� ������ �������� ������� ����������, ������ �����)
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

//������ �����������
#define delay 30 //�������� 250�� =5
#define delay_sck 30 //�������� ��� �����

#define fram_sector 0 //�������� ������
#define fram_bkp 1 //���������
#define FramTmpBufferSize 256

u8 ReadSR(void);

extern void fram_pulse(u8 Delay);//������������ ������ ����
void fram_wr_byte(u8 Data_byt);//�������� ������ �����, ������ � ������� ������������ �����
void fram_wr_byte(u8 Data_byt); //������ ������ �����, ������������ ����
void Fram_CS_Up(void);
void Fram_CS_Down(void);
void Fram_sio_mode(u8 mode);

extern void fram_wr_massive(u8 *massive, u16 n, u8 Addr, u8 Mem_half);
extern void fram_rd_massive(u8 *massive, u16 n, u8 Addr, u8 Mem_half);

extern u8 Fram_Buffer[FramTmpBufferSize];
extern void check_fram (void);

