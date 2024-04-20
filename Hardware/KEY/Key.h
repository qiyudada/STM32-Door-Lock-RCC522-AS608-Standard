#ifndef __KEY_H
#define __KEY_H

//#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键0
//#define KEY1_PRES	1
#define KAngle 90
#include <stdio.h>

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键11
#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP 

extern uint8_t OPassword[4];

void Key_Init(void);
uint8_t Key_GetNum(void);
void PassMatch(uint8_t *OPass);
uint8_t SKey_GetNum(void);
u8 KEY_Scan(u8 mode);

#endif
