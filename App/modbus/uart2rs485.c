//modbus rtu ��������� ������������ ������ � ���������� ���������� �����
//RS485 ���������
//������������:
//UART2 (RX, TX, DIR, �������� �������� ��������)
//Timer1 (������ ��������� ���������� Modbus RTU)


#include "uart2rs485.h"
#include "bastypes.h"
#include "mbtypes.h"
#include "flashdata.h"
#include "ramdata.h"
#include "modbus.h"
#include "stm32f10x.h"
#include "DEFINES.h"

//TClient uart2data;
void RxDMA1Ch6 (void);//��������� DMA �� ������ ������ �� UART
void TxDMA1Ch7 (u8 b_size, u8 *buf_out);//��������� DMA �� �������� ������ � UART
void U2SetTimer(unsigned int Delay);  //������� ������� �� ��������� ����� ��������� ������

#define SetDIR2ToRX    GPIO_WriteBit(GPIOA, GPIO_Pin_0,  (BitAction)(0));
#define SetDIR2ToTX    GPIO_WriteBit(GPIOA, GPIO_Pin_0,  (BitAction)(1));

#define U2RXBUFFSIZE  255 //������ ������ ��������

u8 U2_RX_DATA_READY = 0;//���� ����� ������ �� ����
u8 U2_TX_WAIT = 0;//���� �������� ������ ����!!!

 
const u32 U2BPS[]={  
  4800,// 0 
  9600,// 1 
 19200,// 2 
 57600,// 3 
115200,// 4
230400// 5

};

void uart2rs485_init (void){
  SetDIR2ToTX;//������� RS485 �� ��������
  //uart2data.DevAddr = FLASH_DATA.MODBUS2.b[0];//����� ���������� � ���� ������
  //uart2data.BPS = FLASH_DATA.MODBUS2.b[1];
  //uart2data.Idx = 0;//����� ������ � ������
  //uart2data.TXCount = 0;
  //uart2data.ClntTimeOut = 200;//200���
  //uart2data.ID = 2;//����� ����������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = U2BPS[FLASH_DATA.MODBUS2.b[1]];
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART2->CR1 |=  USART_CR1_RE;//��������� �������
  USART2->CR1 |=  USART_CR1_TE;//��������� ����������
  USART2->CR1 |=  USART_CR1_UE;//��������� UART2
  //U2_TX_WAIT = 0;
  //U2_RX_DATA_READY = 0;
  //RxDMA1Ch6();//��������� DMA �� ������ ������ �� UART
}

//�������� BPS � DEVADDR ��� UART2 ���� ����������, �� ������� ���������� �������������

/* void uart2rs485_ReInit (void){ 
  
  USART_InitTypeDef USART_InitStructure;
  
  if (uart2data.BPS != FLASH_DATA.MODBUS2.b[1]) 
  {
    USART_InitStructure.USART_BaudRate = U2BPS[FLASH_DATA.MODBUS2.b[1]];
    uart2data.BPS = FLASH_DATA.MODBUS1.b[1];
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
  }
  if (uart2data.DevAddr !=FLASH_DATA.MODBUS2.b[0])
  {
    uart2data.DevAddr = FLASH_DATA.MODBUS2.b[0];
  }
} */


/* void U2SetTimer(unsigned int Delay)
{
  SetDIR2ToTX;//���������� ������� �� ��������, ������� �����

  //������� �������
  TIM1->CR1 &= ~TIM_CR1_CEN;//���������� ������ 
  TIM1->SR &= ~TIM_FLAG_CC2;//�������� ���� ����������
  TIM1->CCR2 = Delay + TIM1->CNT;
  TIM1->CR1 |= TIM_CR1_CEN;//TIMER1_START;
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);//���� ����� ����� ���������.
} */

/* void TIM1_user_U2(void)
{  
  TIM1->CR1 &= ~TIM_CR1_CEN;//���������� ������
  TIM1->SR &= ~TIM_FLAG_CC2;//�������� ���� ����������
  TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE);//
  TIM1->CR1 |= TIM_CR1_CEN;//TIMER1_START;
  TxDMA1Ch7();  //��������� � ������ ���
} */

/* void U2_ResetModbusTimerForWaitTransmit(void)
{
  //��� ��������� 
  SetDIR2ToTX;//���������� ������� �� ��������, ������� �����
  TxDMA1Ch7();  //��������� � ������ ���
  U2_TX_WAIT = 1;
} */

/* u8 U2_SwCNT (void)
{
  
  if (U2_TX_WAIT) return 0; 
  if (U2_RX_DATA_READY !=0) 
  {
    
    U2_RX_DATA_READY = 0;
    //uart2data.OnCallBackReceive = &U2_ResetModbusTimerForWaitTransmit;
    //������������� �������
    if (command_decode(&uart2data)) 
    {      
      if (uart2data.TXCount != 0)
      {
        U2SetTimer(uart2data.ClntTimeOut);
        U2_TX_WAIT = 1;
      }
      return 1;
    }
      //U2_TX_WAIT = 0;
      RxDMA1Ch6();//��������������� ������ ��������
  }
  return 0;
} */

void usart2DMA_init (u8 *buf_in)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_DeInit(DMA1_Channel6);//�� ������� ������
  DMA_DeInit(DMA1_Channel7);//�� ������� ������
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);//�������� - ������� ������ UART
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) buf_in;//������� - ��� ����� (������ 256 ����)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//����������� �� ��������� � ������ (�����)
  DMA_InitStructure.DMA_BufferSize = 0;//������� ���� ���������
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����� ��������� �� ����������������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����� (������ �� �����) ����������������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//������ ������ ��������� ����
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//������ ������ ������ ����
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//�� ���������� ������ DMA ���������������
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure); 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//����������� �� ��������� � ������ (�����)
  DMA_Init(DMA1_Channel6, &DMA_InitStructure); 
}


void TxDMA1Ch7 (u8 b_size, u8 *buf_out) {//��������� DMA �� �������� ������ � UART
  SetDIR2ToTX;
  DMA1_Channel7->CCR  &= ~DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, DISABLE);//�������� DMA ��� ��������� ������� � ���������
  DMA1_Channel7->CNDTR = b_size;//������� ���� ���������
  DMA1_Channel7->CMAR = (uint32_t)buf_out;
  USART2->SR  &=  ~USART_SR_TC;   //�������� ���� ��������� ��������
  USART2->CR3 |=  USART_CR3_DMAT;
  DMA1->IFCR |= DMA_IFCR_CTCIF7 | DMA_IFCR_CGIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CTEIF7;//����� ��� ����� ���������� 
  USART2->CR1 |=  USART_CR1_TE;   //��������� ����������
  DMA1_Channel7->CCR  |= DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, ENABLE);//������� DMA... � �� �������� �� ������ ���������� ������ �� ��
  USART2->CR1 |=  USART_CR1_TCIE; //������� ���������� �� ��������� ��������
} 

void RxDMA1Ch6 (void) {//��������� DMA �� ������ ������ �� UART

  SetDIR2ToRX;//�������� ������� �� ����
  DMA1_Channel6->CCR  &= ~DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, DISABLE);//�������� DMA ��� ��������� ������� � ���������
  DMA1_Channel6->CNDTR = U2RXBUFFSIZE;//256 ���� ������ ������������ ������
  USART2->CR3 |=  USART_CR3_DMAR;
  DMA1->IFCR |= DMA_IFCR_CTCIF6 | DMA_IFCR_CGIF6 | DMA_IFCR_CHTIF6 | DMA_IFCR_CTEIF6;//����� ��� ����� ���������� 
  DMA1_Channel6->CCR  |= DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, ENABLE);//������� DMA... � �� �������� ���������� ����������� ������ � �������� �����
  USART2->CR1 |=  USART_CR1_IDLEIE;//��������� ���������� �� ����� ������
  USART2->CR1 |=  USART_CR1_RE;//��������� �������
}

u8 TxRx1Finish(void)
{  
  u32 IIR = USART2->SR;
    if ((IIR & USART_SR_TC) && (USART2->CR1 & USART_CR1_TCIE)) // �������� �������� (��������� ���� ��������� ������� � ����)
      { 
        USART2->SR  &=  ~USART_SR_TC;   //�������� ���� ��������� ��������
        USART2->CR1 &=  ~USART_CR1_TCIE;//��������� ���������� �� ��������� ��������
        USART2->CR3 &=  ~USART_CR3_DMAT;//��������� UART-�� ���������� �� DMA
        DMA1_Channel7->CCR  &= ~DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, DISABLE);//��������� DMA �����������
        //����������� �� ����
        //RxDMA1Ch6();//��������� DMA �� ������ ������ �� UART
        //U2_TX_WAIT = 0;
        ++RAM_DATA.counter[1];
        return 0;
      }
    if ((IIR & USART_SR_IDLE) & (USART2->CR1 & USART_CR1_IDLEIE)) // ����� ������� ��� ����� ���������� ����� � 1 IDLE ����
      {         
        IIR = USART2->DR; //����� ����� IDLE
        //IIR = USART2->SR;
        USART2->CR1 &=  ~USART_CR1_RE;    //��������� �������
        USART2->CR1 &=  ~USART_CR1_IDLEIE;//��������� ���������� �� ����� ������
        USART2->CR3 &=  ~USART_CR3_DMAR;  //��������� DMA RX
        DMA1_Channel6->CCR  &= ~DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, DISABLE);//��������� DMA �� ����
        //uart2data.Idx = (u8)(U2RXBUFFSIZE - DMA1_Channel6->CNDTR);//���-�� �������� ����
        //U2_RX_DATA_READY = 1;//��������� ���� ��������� ����� ��� ����� ������ ������
        //U2_TX_WAIT = 0;//��� �������� ��������
        ++RAM_DATA.counter[3];
        return (U2RXBUFFSIZE - (u8)DMA1_Channel6->CNDTR);//���-�� �������� ����
      }
      return 0;
}






