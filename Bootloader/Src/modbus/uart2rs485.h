#ifndef __UART2RS485_H
#define __UART2RS485_H
#include    "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

    extern void uart2rs485_init (void);
    extern void uart2rs485_ReInit (void);
    extern void TxDMA1Ch7 (void);
    extern u8 U2_SwCNT (void);
    void TIM1_user_U2(void); 
    extern void usart2DMA_init (void);
    
#ifdef __cplusplus
}
#endif

#endif
