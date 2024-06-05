#include "STM32F0xx_Intmash_USART.h"

//доступные скорости передачи
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
	Инициализация драйвера (USART и DMA) значениями из структуры Intmash_Usart *UserUsartStr
Arg:
	Intmash_Usart *UserUsartStr - структура конфигурации драйвера, из который берем значения для инициализации
Ret:
Comment:
Перед вызовом этой функции необходимо создать и инициализировать структуру, 
см. описание структуры
*/
void UsartDriverInit (Intmash_Usart *UserUsartStr)
{
  DMA_InitTypeDef DMA_InitStructure;
  USART_InitTypeDef USART_InitStructure;  
  //если инициализация происходит посреди работы, надо сначала сбросить настройки 
  DMA_DeInit(UserUsartStr->DMAy_StreamRX);
  DMA_DeInit(UserUsartStr->DMAy_StreamTX);
  USART_DeInit(UserUsartStr->USARTx);
  //сначала настраиваю ДМА на работу с УАРТом

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(UserUsartStr->USARTx->RDR);
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
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(UserUsartStr->USARTx->TDR);//источник - регистр данных UART TDR
  DMA_Init(UserUsartStr->DMAy_StreamTX, &DMA_InitStructure);//USART2_Tx  
  //потом настраиваю сам УАРТ и запускаю его  

  USART_InitStructure.USART_BaudRate = UserUsartStr->USART_BaudRate;  
  //if (UserUsartStr->USART_StopBits < 4) USART_InitStructure.USART_StopBits = (u16)UserUsartStr->USART_StopBits << 12;
  //else USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_StopBits = UserUsartStr->USART_StopBits;
  //if (UserUsartStr->USART_Parity == 2) USART_InitStructure.USART_Parity = USART_Parity_Even;//четны 
  //else if (UserUsartStr->USART_Parity == 1) USART_InitStructure.USART_Parity = USART_Parity_Odd;//нечетны
  //else USART_InitStructure.USART_Parity = USART_Parity_No; //нет контроля четности
  USART_InitStructure.USART_Parity = UserUsartStr->USART_Parity;
  
  if (USART_InitStructure.USART_Parity == USART_Parity_No)USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  else USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UserUsartStr->USARTx, &USART_InitStructure);
  USART_DMACmd(UserUsartStr->USARTx, USART_DMAReq_Rx, ENABLE);
  USART_DMACmd(UserUsartStr->USARTx, USART_DMAReq_Tx, ENABLE);

  UserUsartStr->USARTx->CR1 |=  USART_CR1_RE;//разрешить приёмник
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TE;//разрешить передатчик 
  UserUsartStr->USARTx->CR1 |=  USART_CR1_UE;//разрешить UART2   
}



/*Name: UsartTransmit
	функция настройки драйвера на передачу данных 
Arg:
	Intmash_Usart *UserUsartStr - структура конфигурации драйвера
	u8* Buffer - указатель на буфер передаваемых данных (откуда брать)
	u8 Cnt - количество передаваемых байт 
Ret:
Comment:
	
*/
void UsartTransmit(Intmash_Usart *UserUsartStr, u8* Buffer, u8 Cnt) 
{//настройка DMA на передачу данных в UART
  //SetDIRToTX;
  if (UserUsartStr->PolarityDIR == POLARITY_HI) GPIO_SetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  else if (UserUsartStr->PolarityDIR == POLARITY_LOW) GPIO_ResetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  
  DMA_Cmd(UserUsartStr->DMAy_StreamTX, DISABLE);//отключаю DMA для получения доступа к регистрам
  USART_ClearFlag(UserUsartStr->USARTx, USART_ICR_TCCF);//сбросить флаг окончания передачи
  DMA_ClearFlag(UserUsartStr->DMA_FLAGS_TX);//почистим флаги стрима ДМА, без этого не работает  
  UserUsartStr->DMAy_StreamTX->CNDTR = Cnt;//сколько байт отправить
  UserUsartStr->DMAy_StreamTX->CMAR = (uint32_t)Buffer;
  UserUsartStr->USARTx->CR3 |=  USART_CR3_DMAT;
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TE;   //разрешить передатчик

  DMA_Cmd(UserUsartStr->DMAy_StreamTX, ENABLE);//включаю DMA
  UserUsartStr->USARTx->CR1 |=  USART_CR1_TCIE; //разрешу прерывания по окончанию передачи
  
} 

/*Name: UsartRecieve
	функция настройки драйвера на прием данных 
Arg:
	Intmash_Usart *UserUsartStr - структура конфигурации драйвера
	u8* Buffer - указатель на буфер принимаемых данных (куда складывать)
Ret:
Comment:
	
*/
void UsartRecieve (Intmash_Usart *UserUsartStr, u8* Buffer) 
{//настройка DMA на чтение данных из UART
 // SetDIRToRX;//переключить на приём? надеемся, что нога успеет дернуться
  if (UserUsartStr->PolarityDIR == POLARITY_HI) GPIO_ResetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  else if (UserUsartStr->PolarityDIR == POLARITY_LOW) GPIO_SetBits(UserUsartStr->GPIOxDIR, UserUsartStr->GPIO_PinDIR);
  
  DMA_Cmd(UserUsartStr->DMAy_StreamRX, DISABLE);//отключаю DMA для получения доступа к регистрам
  DMA_ClearFlag(UserUsartStr->DMA_FLAGS_RX);//почистим флаги стрима ДМА, без этого не работает 
  UserUsartStr->USARTx->CR3 |=  USART_CR3_DMAR;
  UserUsartStr->DMAy_StreamRX->CNDTR = URXBUFFSIZE;
  UserUsartStr->DMAy_StreamRX->CMAR = (uint32_t)Buffer;

  DMA_Cmd(UserUsartStr->DMAy_StreamRX, ENABLE);//включаю DMA
  UserUsartStr->USARTx->CR1 |=  USART_CR1_IDLEIE;//разрешить прерывания по приёму данных
  UserUsartStr->USARTx->CR1 |=  USART_CR1_RE;//разрешить приёмник
}

/*Name: UsartTxRxFinish
	функция окончания приема/передачи данных 
Arg:
	Intmash_Usart *UserUsartStr - структура конфигурации драйвера
Ret:
	0 - после окончания передачи данных
	количество принятых байт - после окончания приема данных (u8)
Comment:
вызывается после окончания приема или передачи данных, 
в прерывании используемого USART 
	пример: 	
		void USART1_IRQHandler(void)
		{
		  UsartTxRxFinish(&USERusart);
		}
*/
u8 UsartTxRxFinish(Intmash_Usart *UserUsartStr)
{
  static u32 IIR;
  IIR = UserUsartStr->USARTx->ISR;
    if ((IIR & USART_ISR_TC) && (UserUsartStr->USARTx->CR1 & USART_CR1_TCIE)) // Передача окончена (последний байт полностью передан в порт)
      {   
        USART_ClearFlag(UserUsartStr->USARTx, USART_ICR_TCCF);//сбросить флаг окончания передачи 
        USART_ClearITPendingBit(UserUsartStr->USARTx, USART_CR1_TCIE);//запретить прерывание по окончании передачи
        UserUsartStr->USARTx->CR3 &=  ~USART_CR3_DMAT;//запретить UART-ту передавать по DMA     
        DMA_Cmd(UserUsartStr->DMAy_StreamTX, DISABLE);//выключить DMA передатчика  
        return 0; //сообщение отправилось
      }
    if ((IIR & USART_ISR_IDLE) & (UserUsartStr->USARTx->CR1 & USART_CR1_IDLEIE)) // Между байтами при приёме обнаружена пауза в 1 IDLE байт
      {
        USART_ClearITPendingBit(UserUsartStr->USARTx, USART_CR1_IDLEIE | USART_CR1_RE);//запретить прерывания по приёму данных, запретить приёмник
        USART_ClearFlag(UserUsartStr->USARTx, USART_ICR_IDLECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_PECF | USART_ICR_ORECF);//сброс флага IDLE и флагов ошибок
        UserUsartStr->USARTx->CR3 &=  ~USART_CR3_DMAR;  //запретить DMA RX
        DMA_Cmd(UserUsartStr->DMAy_StreamRX, DISABLE);//выключить DMA на приём  

        return (URXBUFFSIZE - (u8)UserUsartStr->DMAy_StreamRX->CNDTR);//кол-во принятых байт
      }
    return 0;
}
