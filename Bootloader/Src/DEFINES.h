#pragma once

#include "stm32f10x.h"//���������� STM
 
//��� ������ �� ���������
#define LED_RUN_ON   GPIOB->BRR = GPIO_Pin_5
#define LED_RUN_OFF  GPIOB->BSRR = GPIO_Pin_5
#define LED_RUN_ST   GPIOB->ODR & GPIO_Pin_5
#define LED_LINK1_ON   GPIOB->BRR = GPIO_Pin_6
#define LED_LINK1_OFF  GPIOB->BSRR = GPIO_Pin_6
#define LED_LINK1_ST   GPIOB->ODR & GPIO_Pin_6
#define LED_ALARM_ON   GPIOB->BRR = GPIO_Pin_7
#define LED_ALARM_OFF  GPIOB->BSRR = GPIO_Pin_7
#define LED_ALARM_ST   GPIOB->ODR & GPIO_Pin_7
#define LED_LINK2_ON   GPIOB->BRR = GPIO_Pin_8
#define LED_LINK2_OFF  GPIOB->BSRR = GPIO_Pin_8
#define LED_LINK2_ST   GPIOB->ODR & GPIO_Pin_8

//���
#define ADC1_DR_Address    ((u32)0x4001244C)

//#define Angle180 44928

#define SYNC_EXTI_LINE (1<<11)

#define TIM2_PERIOD     360 //200:kHz ������ ���
#define TIM2_CH_3_4_Q   108 //30% duty cycle

#define FLASH_PAGE_SIZE    ((u16)0x400)

#define SYNC_fail_cnt 10 //������� ��������� ������ ��� �������� 1= 10 ��