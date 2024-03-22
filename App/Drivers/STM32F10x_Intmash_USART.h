#ifndef STM32F4xx_Intmash_USART_H
#define STM32F4xx_Intmash_USART_H

#include "stm32f10x.h"




#define URXBUFFSIZE  255 //������ ������ ��������
//�������� ��� ��������� DIR
#define WITHOUT_DIR  0 // ���� ��� ���, ������ �� ������
#define POLARITY_LOW 1 // ���� ������������, ��������� ������.
#define POLARITY_HI  2 // ���� ������������, ������ ������. 
/*
��������� ������������ ��������
��� ������� �������� - ���� ��������� � ���� �������������

//������ ��������� � �� �������������
Intmash_Usart USERusart; // ������� ���������
void InitUserUsartData(void) //������ ������������� ���������
{
  USERusart.USARTx = USART1;
  USERusart.USART_BaudRate = 115200;
  USERusart.DMAy_StreamRX = DMA2_Stream2;
  USERusart.DMAy_StreamTX = DMA2_Stream7;
  USERusart.DMA_channel = DMA_Channel_4;
  USERusart.DMA_FLAGS_RX = DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TCIF2;
  USERusart.DMA_FLAGS_TX = DMA_FLAG_FEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TCIF7;
  USERusart.GPIOxDIR = GPIOA;
  USERusart.GPIO_PinDIR = GPIO_Pin_8;
  USERusart.PolarityDIR = 2; //����������� ����������
}
*/

#ifdef __cplusplus
struct Intmash_Usart
  #else
typedef struct 
#endif
{

  USART_TypeDef* USARTx; //��������� �� ������������ USART
  DMA_Channel_TypeDef* DMAy_StreamTX; //��������� �� ����� DMA �� �������� ������ ����� USART
  DMA_Channel_TypeDef* DMAy_StreamRX; //��������� �� ����� DMA �� ����� ������ ����� USART

  u32 DMA_FLAGS_TX; //����� ����������, ���������� �� �������� ������
  u32 DMA_FLAGS_RX; //����� ����������, ���������� �� ����� ������
  u32 USART_BaudRate; //�������� ��������, �������� 9600
  GPIO_TypeDef* GPIOxDIR; //����, ��� �������� ������������ ��� ������������ ����������� �������� ������
  u16 GPIO_PinDIR; //����� ����, ������� ������������ ��� ������������ ����������� �������� ������
  u16 PolarityDIR; //2 - �����������, 1 - ���������, 0 - �� ������������
  u8 USART_StopBits; //���������� �������� ��� 0-1/1-0,5/2-2/3-1,5 
  u8 USART_Parity; // 0 - 0/ 2 - Even / 1- Odd
  
  #ifdef __cplusplus
};
  #else
} Intmash_Usart;  
#endif

  #ifdef __cplusplus
    extern "C" void UsartDriverInit (Intmash_Usart *UserUsartStr);
    extern "C" void UsartTransmit(Intmash_Usart *UserUsartStr, u8* Buffer, u8 Cnt);
    extern "C" void UsartRecieve (Intmash_Usart *UserUsartStr, u8* Buffer);
    extern "C" u8 UsartTxRxFinish(Intmash_Usart *UserUsartStr);
  #else
    extern void UsartDriverInit (Intmash_Usart *UserUsartStr);
    extern void UsartTransmit(Intmash_Usart *UserUsartStr, u8* Buffer, u8 Cnt);
    extern void UsartRecieve (Intmash_Usart *UserUsartStr, u8* Buffer);
    extern u8 UsartTxRxFinish(Intmash_Usart *UserUsartStr);
  #endif
    


extern const u32 USARTbaudRate[8];

#endif /*STM32F4xx_Intmash_USART_H */