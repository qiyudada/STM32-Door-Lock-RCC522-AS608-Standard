#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	EXTI_InitTypeDef EXTI_InitStructure;			//声明外部中断优先级配置结构体
//	NVIC_InitTypeDef NVIC_InitStructure;			//声明中断优先级配置结构体
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	//按键1的中断优先级配置
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				/* 配置NVIC为优先级组1 */
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		/* 配置中断源：按键1 */
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* 配置抢占优先级 */
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			/* 配置子优先级 */
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* 使能中断通道 */
//    NVIC_Init(&NVIC_InitStructure);	
//    //按键2的中断优先级配置
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				/* 配置NVIC为优先级组1 */
//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn	; 	/* 配置中断源：按键2 */  
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/* 配置抢占优先级 */
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			/* 配置子优先级 */
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* 使能中断通道 */
//  	NVIC_Init(&NVIC_InitStructure);


//		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1); /* 选择EXTI的信号源 */
//    EXTI_InitStructure.EXTI_Line = EXTI_Line1;						/* 选择EXIT事件线 */
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;						/* EXTI为中断模式 */
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;					/* 上升沿中断 */
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;	/* 使能中断 */	
//    EXTI_Init(&EXTI_InitStructure);
//		
//		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11); /* 选择EXTI的信号源 */
//    EXTI_InitStructure.EXTI_Line = EXTI_Line11;						/* 选择EXIT事件线 */
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;						/* EXTI为中断模式 */
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;					/* 上升沿中断 */
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;	/* 使能中断 */	
//    EXTI_Init(&EXTI_InitStructure);
			
}

static uint8_t KeyNum=0;
uint8_t Key_GetNum(void)
{
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}

uint8_t SKey_GetNum(void)
{
	uint8_t SKeyNum=0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		SKeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		SKeyNum = 2;
	}
	
	return SKeyNum;
}

static uint8_t Password[4];
void PassMatch(uint8_t *OPass)
{
	uint8_t Kflag=0;
	uint8_t i=0;
	int P=0;
	
	OLED_Clear();
	
	while(1)
	{
		while(1)
		{
			KeyNum=Key_GetNum();
			Delay_ms(10);
			if(KeyNum!=0)
			{
				break;
			}
		}
		if(KeyNum!=0 &&Kflag ==0)
		{
			OLED_Clear();
			Kflag=1;
			i++;
			OLED_ShowString(28,0,"PASSWORD:",OLED_8X16);
			OLED_ShowNum(16+i*16,32,KeyNum,1,OLED_6X8);
			OLED_Update();
			Delay_ms(100);
		}
		if(Kflag==1)
		{
			Password[i-1]=KeyNum;
			Kflag=0;
			KeyNum=0;
		}	
		if(i>=4)
		{
			Kflag=3;
			break;
		}
	}		
	while(Kflag==3)
	{
		for(int j=0;j<4;j++)
		{
			if(Password[j] == OPass[j])
			{
				P++;
			}
			else
			{
				P=-1;
				break;
			}
		}
		if(P==4)
		{
			LED1_OFF();
			LED2_OFF();
			LED3_ON();
			Delay_ms(500);
			Servo_SetAngle(KAngle);
			Delay_ms(100);
			OLED_Clear();
			OLED_ShowString(20, 28, "Door Open!", OLED_8X16);
			OLED_Update();
			Delay_ms(3000);
			KeyNum=0;
			break;
		}
		else if(P == -1)
		{
			LED2_OFF();
			LED3_OFF();
			LED1_ON();
			OLED_Clear();
			OLED_ShowString(6,16, "Password error!", OLED_8X16);
			OLED_ShowString(22,32, "Try again", OLED_8X16);
			OLED_Update();
			Delay_ms(3000);
//			OLED_Clear();
//			OLED_Update();
			break;
		}
	}
		
}
	
/****Key0--PB1
*****Key1--PB11
*****WK_up--PB10*/

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		Delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0){
			return KEY0_PRES;
		}
		else if(KEY1==0){
			return KEY1_PRES;
		}
		else if(WK_UP==1){
			return WKUP_PRES; 
		}
	}
	else if
		(KEY0==1&&KEY1==1&&WK_UP==0)
		key_up=1; 	     
	return 0;// 无按键按下
}

 

