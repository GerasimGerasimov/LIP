#ifndef __UART1RS485_H
#define __UART1RS485_H
#include    "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

    extern void uart1rs485_init (void);
    extern void uart1rs485_ReInit (void);
    extern void TxDMA1Ch4 (void);
    extern u8 U1_SwCNT (void);
    extern void TIM1_user_U1(void); 
    extern void usart1DMA_init (void);
    
#ifdef __cplusplus
}
#endif

#endif
