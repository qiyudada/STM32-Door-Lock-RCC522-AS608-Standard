#ifndef _Timer_H
#define _Timer_H

#include "stm32f10x.h" 
extern uint16_t TIM2_Time;

void Timer2_Init(void);
void TIM4_Int_Init(uint16_t arr,uint16_t psc);
void TIM3_Int_Init(u16 arr,u16 psc);

//extern uint8_t add;
#endif
