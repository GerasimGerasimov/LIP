//modbus rtu интерфейс организующий доступ к параметрам процессора через
//RS485 интерфейс
//Используется:
//UART2 (RX, TX, DIR, скорость задается уставкой)
//Timer1 (отсчёт временных интервалов Modbus RTU)


#include "uart2rs485.h"
#include "bastypes.h"
#include "mbtypes.h"
#include "flashdata.h"
#include "ramdata.h"
#include "modbus.h"
#include "stm32f10x.h"
#include "DEFINES.h"

//TClient uart2data;
void RxDMA1Ch6 (void);//настройка DMA на чтение данных из UART
void TxDMA1Ch7 (u8 b_size, u8 *buf_out);//настройка DMA на передачу данных в UART
void U2SetTimer(unsigned int Delay);  //зарядка таймера на подождать перед отправкой данных

#define SetDIR2ToRX    GPIO_WriteBit(GPIOA, GPIO_Pin_0,  (BitAction)(0));
#define SetDIR2ToTX    GPIO_WriteBit(GPIOA, GPIO_Pin_0,  (BitAction)(1));

#define U2RXBUFFSIZE  255 //размер буфера приёмника

u8 U2_RX_DATA_READY = 0;//флаг приёма пакета не ждем
u8 U2_TX_WAIT = 0;//флаг отправки пакета ждем!!!

 
const u32 U2BPS[]={  
  4800,// 0 
  9600,// 1 
 19200,// 2 
 57600,// 3 
115200,// 4
230400// 5

};

void uart2rs485_init (void){
  SetDIR2ToTX;//драйвер RS485 на отправку
  //uart2data.DevAddr = FLASH_DATA.MODBUS2.b[0];//адрес устройства в сети модбас
  //uart2data.BPS = FLASH_DATA.MODBUS2.b[1];
  //uart2data.Idx = 0;//буфер начать с начала
  //uart2data.TXCount = 0;
  //uart2data.ClntTimeOut = 200;//200мкс
  //uart2data.ID = 2;//номер интерфейса
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = U2BPS[FLASH_DATA.MODBUS2.b[1]];
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART2->CR1 |=  USART_CR1_RE;//разрешить приёмник
  USART2->CR1 |=  USART_CR1_TE;//разрешить передатчик
  USART2->CR1 |=  USART_CR1_UE;//разрешить UART2
  //U2_TX_WAIT = 0;
  //U2_RX_DATA_READY = 0;
  //RxDMA1Ch6();//настройка DMA на чтение данных из UART
}

//сравнить BPS и DEVADDR для UART2 если отличаются, то сделать повторнуюю инициализацию

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
  SetDIR2ToTX;//переключаю драйвер на передачу, дернули ногой

  //зарядка таймера
  TIM1->CR1 &= ~TIM_CR1_CEN;//остановить таймер 
  TIM1->SR &= ~TIM_FLAG_CC2;//сбросить флаг прерывания
  TIM1->CCR2 = Delay + TIM1->CNT;
  TIM1->CR1 |= TIM_CR1_CEN;//TIMER1_START;
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);//ждем когда пауза сработает.
} */

/* void TIM1_user_U2(void)
{  
  TIM1->CR1 &= ~TIM_CR1_CEN;//остановить таймер
  TIM1->SR &= ~TIM_FLAG_CC2;//сбросить флаг прерывания
  TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE);//
  TIM1->CR1 |= TIM_CR1_CEN;//TIMER1_START;
  TxDMA1Ch7();  //настройка и запуск дма
} */

/* void U2_ResetModbusTimerForWaitTransmit(void)
{
  //тут подождать 
  SetDIR2ToTX;//переключаю драйвер на передачу, дернули ногой
  TxDMA1Ch7();  //настройка и запуск дма
  U2_TX_WAIT = 1;
} */

/* u8 U2_SwCNT (void)
{
  
  if (U2_TX_WAIT) return 0; 
  if (U2_RX_DATA_READY !=0) 
  {
    
    U2_RX_DATA_READY = 0;
    //uart2data.OnCallBackReceive = &U2_ResetModbusTimerForWaitTransmit;
    //декодирование команды
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
      RxDMA1Ch6();//восстановлением работы приёмника
  }
  return 0;
} */

void usart2DMA_init (u8 *buf_in)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_DeInit(DMA1_Channel6);//на всякимй случай
  DMA_DeInit(DMA1_Channel7);//на всякимй случай
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);//источник - регистр данных UART
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) buf_in;//приёмник - мой буфер (размер 256 байт)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//направление из переферии в память (буфер)
  DMA_InitStructure.DMA_BufferSize = 0;//сколько байт отправить
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//адрес переферии не инкрементируется
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//адрес (ссылка на буфер) инкрементируется
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//размер данных переферии БАЙТ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//размер данных буфера БАЙТ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//по заполнению буфера DMA останавливается
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure); 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//направление из переферии в память (буфер)
  DMA_Init(DMA1_Channel6, &DMA_InitStructure); 
}


void TxDMA1Ch7 (u8 b_size, u8 *buf_out) {//настройка DMA на передачу данных в UART
  SetDIR2ToTX;
  DMA1_Channel7->CCR  &= ~DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, DISABLE);//отключаю DMA для получения доступа к регистрам
  DMA1_Channel7->CNDTR = b_size;//сколько байт отправить
  DMA1_Channel7->CMAR = (uint32_t)buf_out;
  USART2->SR  &=  ~USART_SR_TC;   //сбросить флаг окончания передачи
  USART2->CR3 |=  USART_CR3_DMAT;
  DMA1->IFCR |= DMA_IFCR_CTCIF7 | DMA_IFCR_CGIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CTEIF7;//очищу все флаги прерываний 
  USART2->CR1 |=  USART_CR1_TE;   //разрешить передатчик
  DMA1_Channel7->CCR  |= DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, ENABLE);//включаю DMA... и он начинает из буфера выкидывать данные на ТХ
  USART2->CR1 |=  USART_CR1_TCIE; //разрешу прерывания по окончанию передачи
} 

void RxDMA1Ch6 (void) {//настройка DMA на чтение данных из UART

  SetDIR2ToRX;//включить драйвер на приём
  DMA1_Channel6->CCR  &= ~DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, DISABLE);//отключаю DMA для получения доступа к регистрам
  DMA1_Channel6->CNDTR = U2RXBUFFSIZE;//256 байт размер принимающего буфера
  USART2->CR3 |=  USART_CR3_DMAR;
  DMA1->IFCR |= DMA_IFCR_CTCIF6 | DMA_IFCR_CGIF6 | DMA_IFCR_CHTIF6 | DMA_IFCR_CTEIF6;//очищу все флаги прерываний 
  DMA1_Channel6->CCR  |= DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, ENABLE);//включаю DMA... и он начинает складывать поступающие данные в заданный буфер
  USART2->CR1 |=  USART_CR1_IDLEIE;//разрешить прерывания по приёму данных
  USART2->CR1 |=  USART_CR1_RE;//разрешить приёмник
}

u8 TxRx1Finish(void)
{  
  u32 IIR = USART2->SR;
    if ((IIR & USART_SR_TC) && (USART2->CR1 & USART_CR1_TCIE)) // Передача окончена (последний байт полностью передан в порт)
      { 
        USART2->SR  &=  ~USART_SR_TC;   //сбросить флаг окончания передачи
        USART2->CR1 &=  ~USART_CR1_TCIE;//запретить прерывание по окончании передачи
        USART2->CR3 &=  ~USART_CR3_DMAT;//запретить UART-ту передавать по DMA
        DMA1_Channel7->CCR  &= ~DMA_CCR7_EN;//DMA_Cmd(DMA1_Channel7, DISABLE);//выключить DMA передатчика
        //переключить на приём
        //RxDMA1Ch6();//настройка DMA на чтение данных из UART
        //U2_TX_WAIT = 0;
        ++RAM_DATA.counter[1];
        return 0;
      }
    if ((IIR & USART_SR_IDLE) & (USART2->CR1 & USART_CR1_IDLEIE)) // Между байтами при приёме обнаружена пауза в 1 IDLE байт
      {         
        IIR = USART2->DR; //сброс флага IDLE
        //IIR = USART2->SR;
        USART2->CR1 &=  ~USART_CR1_RE;    //запретить приёмник
        USART2->CR1 &=  ~USART_CR1_IDLEIE;//запретить прерывания по приёму данных
        USART2->CR3 &=  ~USART_CR3_DMAR;  //запретить DMA RX
        DMA1_Channel6->CCR  &= ~DMA_CCR6_EN;//DMA_Cmd(DMA1_Channel6, DISABLE);//выключить DMA на приём
        //uart2data.Idx = (u8)(U2RXBUFFSIZE - DMA1_Channel6->CNDTR);//кол-во принятых байт
        //U2_RX_DATA_READY = 1;//выставляю флаг основному циклу что пакет данных принят
        //U2_TX_WAIT = 0;//нет ожидания передачи
        ++RAM_DATA.counter[3];
        return (U2RXBUFFSIZE - (u8)DMA1_Channel6->CNDTR);//кол-во принятых байт
      }
      return 0;
}






