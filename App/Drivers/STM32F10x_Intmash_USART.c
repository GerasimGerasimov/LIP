#include "STM32F10x_Intmash_USART.h"
#include "ramdata.h"

//��������� �������� ��������
const u32 USARTbaudRate[8]={
    4800,//0 
    9600,//1 
    19200,//2 
    57600,//3 
    115200,//4
    230400,//5  
    460800,//6  
    921600//7 
};

/*Name: UsartDriverInit
	������������� �������� (USART � DMA) ���������� �� ��������� Intmash_Usart *UserUsartStr
Arg:
	Intmash_Usart *UserUsartStr - ��������� ������������ ��������, �� ������� ����� �������� ��� �������������
Ret:
Comment:
����� ������� ���� ������� ���������� ������� � ���������������� ���������, 
��. �������� ���������
*/
void UsartDriverInit (Intmash_Usart *UserUsartStr)
{
  DMA_InitTypeDef DMA_InitStructure;
  USART_InitTypeDef USART_InitStructure;  
  //���� ������������� ���������� ������� ������, ���� ������� �������� ��������� 
  DMA_DeInit(UserUsartStr->DMAy_StreamRX);
  DMA_DeInit(UserUsartStr->DMAy_StreamRX); 
  USART_DeInit(UserUsartStr->USARTx);
  //������� ���������� ��� �� ������ � ������

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(UserUsartStr->USARTx->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(UserUsartStr->DMAy_StreamRX, &DMA_InitStructure);//USART2_Rx
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_Init(UserUsartStr->DMAy_StreamTX, &DMA_InitStructure);//USART2_Tx  
  //����� ���������� ��� ���� � �������� ���  

  USART_InitStructure.USART_BaudRate = UserUsartStr->USART_BaudRate;  
  //if (UserUsartStr->USART_StopBits < 4) USART_InitStructure.USART_StopBits = (u16)UserUsartStr->USART_StopBits << 12;
  //else USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_StopBits = UserUsartStr->USART_StopBits;
  //if (UserUsartStr->USART_Parity == 2) USART_InitStructure.USART_Parity = USART_Parity_Even;//����� 
  //else if (UserUsartStr->USART_Parity == 1) USART_InitStructure.USART_Parity = USART_Parity_Odd;//�������
  //else USART_InitStructure.USART_Parity = USART_Parity_No; //��� �������� ��������
  USART_InitStructure.USART_Parity = UserUsartStr->USART_Parity;
  
  if (USART_InitStructure.USART_Parity == USART_Parity_No)USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  else USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UserUsartStr->USARTx, &USART_InitStructure);
  
  UserUsartStr->USARTx->CR1 |=  USART_CR1_RE;//��������� �������
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TE;//��������� ���������� 
  UserUsartStr->USARTx->CR1 |=  USART_CR1_UE;//��������� UART2   
}



/*Name: UsartTransmit
	������� ��������� �������� �� �������� ������ 
Arg:
	Intmash_Usart *UserUsartStr - ��������� ������������ ��������
	u8* Buffer - ��������� �� ����� ������������ ������ (������ �����)
	u8 Cnt - ���������� ������������ ���� 
Ret:
Comment:
	
*/
void UsartTransmit(Intmash_Usart *UserUsartStr, u8* Buffer, u8 Cnt) 
{//��������� DMA �� �������� ������ � UART
  //SetDIRToTX;
  if (UserUsartStr->PolarityDIR == POLARITY_HI) GPIO_SetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  else if (UserUsartStr->PolarityDIR == POLARITY_LOW) GPIO_ResetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  
  DMA_Cmd(UserUsartStr->DMAy_StreamTX, DISABLE);
  //UserUsartStr->DMAy_StreamTX->CR &= ~(uint32_t)DMA_SxCR_EN;//�������� DMA ��� ��������� ������� � ���������
  UserUsartStr->USARTx->SR  &=  ~USART_SR_TC;   //�������� ���� ��������� �������� 
  DMA_ClearFlag(UserUsartStr->DMA_FLAGS_TX);//�������� ����� ������ ���, ��� ����� �� ��������  
  UserUsartStr->DMAy_StreamTX->CNDTR = Cnt;//������� ���� ���������
  UserUsartStr->DMAy_StreamTX->CMAR = (uint32_t)Buffer;
  UserUsartStr->USARTx->CR3 |=  USART_CR3_DMAT;
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TE;   //��������� ����������
  //UserUsartStr->DMAy_StreamTX->CR |= (uint32_t)DMA_SxCR_EN;//������� DMA
  DMA_Cmd(UserUsartStr->DMAy_StreamTX, ENABLE);
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TCIE; //������� ���������� �� ��������� ��������
  
} 

/*Name: UsartRecieve
	������� ��������� �������� �� ����� ������ 
Arg:
	Intmash_Usart *UserUsartStr - ��������� ������������ ��������
	u8* Buffer - ��������� �� ����� ����������� ������ (���� ����������)
Ret:
Comment:
	
*/
void UsartRecieve (Intmash_Usart *UserUsartStr, u8* Buffer) 
{//��������� DMA �� ������ ������ �� UART
 // SetDIRToRX;//����������� �� ����? ��������, ��� ���� ������ ���������
  if (UserUsartStr->PolarityDIR == POLARITY_HI) GPIO_ResetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  else if (UserUsartStr->PolarityDIR == POLARITY_LOW) GPIO_SetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  
  //UserUsartStr->DMAy_StreamRX->CR &= ~(uint32_t)DMA_SxCR_EN;//�������� DMA ��� ��������� ������� � ���������
  DMA_Cmd(UserUsartStr->DMAy_StreamRX, DISABLE);
  DMA_ClearFlag(UserUsartStr->DMA_FLAGS_RX);//�������� ����� ������ ���, ��� ����� �� �������� 
  UserUsartStr->USARTx->CR3 |=  USART_CR3_DMAR;
  UserUsartStr->DMAy_StreamRX->CNDTR = URXBUFFSIZE;
  UserUsartStr->DMAy_StreamRX->CMAR = (uint32_t)Buffer;
  //UserUsartStr->DMAy_StreamRX->CR |= (uint32_t)DMA_SxCR_EN;//������� DMA
  DMA_Cmd(UserUsartStr->DMAy_StreamRX, ENABLE);
  UserUsartStr->USARTx->CR1 |=  USART_CR1_IDLEIE;//��������� ���������� �� ����� ������
  UserUsartStr->USARTx->CR1 |=  USART_CR1_RE;//��������� �������
}

/*Name: UsartTxRxFinish
	������� ��������� ������/�������� ������ 
Arg:
	Intmash_Usart *UserUsartStr - ��������� ������������ ��������
Ret:
	0 - ����� ��������� �������� ������
	���������� �������� ���� - ����� ��������� ������ ������ (u8)
Comment:
���������� ����� ��������� ������ ��� �������� ������, 
� ���������� ������������� USART 
	������: 	
		void USART1_IRQHandler(void)
		{
		  UsartTxRxFinish(&USERusart);
		}
*/
u8 UsartTxRxFinish(Intmash_Usart *UserUsartStr)
{
  static u32 IIR;
  IIR = UserUsartStr->USARTx->SR;
    if ((IIR & USART_SR_TC) && (UserUsartStr->USARTx->CR1 & USART_CR1_TCIE)) // �������� �������� (��������� ���� ��������� ������� � ����)
      {   
        UserUsartStr->USARTx->SR  &=  ~USART_SR_TC;   //�������� ���� ��������� ��������
        UserUsartStr->USARTx->CR1 &=  ~USART_CR1_TCIE;//��������� ���������� �� ��������� ��������
        UserUsartStr->USARTx->CR3 &=  ~USART_CR3_DMAT;//��������� UART-�� ���������� �� DMA
        //UserUsartStr->DMAy_StreamTX->CR &= ~(uint32_t)DMA_SxCR_EN;//��������� DMA �����������         
        DMA_Cmd(UserUsartStr->DMAy_StreamTX, DISABLE);
        return 0; //��������� �����������
      }
    if ((IIR & USART_SR_IDLE) & (UserUsartStr->USARTx->CR1 & USART_CR1_IDLEIE)) // ����� ������� ��� ����� ���������� ����� � 1 IDLE ����
      {
        UserUsartStr->USARTx->DR; //����� ����� IDLE
        UserUsartStr->USARTx->CR1 &=  ~USART_CR1_RE;    //��������� �������
        UserUsartStr->USARTx->CR1 &=  ~USART_CR1_IDLEIE;//��������� ���������� �� ����� ������
        UserUsartStr->USARTx->CR3 &=  ~USART_CR3_DMAR;  //��������� DMA RX
        //UserUsartStr->DMAy_StreamRX->CR &= ~(uint32_t)DMA_SxCR_EN;//��������� DMA �� ����   
        DMA_Cmd(UserUsartStr->DMAy_StreamRX, DISABLE);

        u8* arr = (u8*)UserUsartStr->DMAy_StreamRX->CMAR;
        RAM_DATA.data32[2] = (u32)arr;
        RAM_DATA.data[1] = arr[0];
        RAM_DATA.data[2] = arr[1];
        RAM_DATA.data[3] = arr[2];
        RAM_DATA.data[4] = arr[3];
        RAM_DATA.data[5] = arr[4];
        RAM_DATA.data[6] = arr[5];
        RAM_DATA.data[7] = arr[6];

        ++RAM_DATA.counter[1];
        return (URXBUFFSIZE - (u8)UserUsartStr->DMAy_StreamRX->CNDTR);//���-�� �������� ����
      }
    return 0;
}
