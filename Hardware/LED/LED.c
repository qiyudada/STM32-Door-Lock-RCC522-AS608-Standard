#include "stm32f10x.h"                  // Device header
#include "LED.h"

void LED_Init(void)//init
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
}

void LED3_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

void LED3_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
}

