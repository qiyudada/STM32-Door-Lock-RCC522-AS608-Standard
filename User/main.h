#ifndef _Main_H
#define _Main_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Usart.h"
#include "Timer.h"
#include <stdio.h>
#include <string.h>
#include "AS608.h"
#include "HCSR04.h"
#include "Servo.h"
#include "PWM.h"
#include "LCD.h"
#include "LCD_init.h"
#include "spi.h"
#include "dma.h"
#include "OLED_Data.h"
#include "pic.h"
#include "RC522.h"
#include "usart2.h"
#include "stmflash.h"

//#define usart2_bound  57600

uint16_t T;//超声波
//uint8_t add;
//extern uint8_t Pre_flag;
//extern uint8_t Add_flag;
//// int m=0;
//// int mark;
//// int mark1;
//extern int Continue;
uint16_t TIM2_Time;
uint8_t KeyNum;//按键值
uint8_t SKeyNum;//first key value
float OAngle= 0;//舵机角度
static uint8_t OPassword[4]={2,2,1,2};

uint8_t Card_Type1[2];  
uint8_t Card_ID[4]; 
uint8_t Card_KEY[6] = {0xff,0xff,0xff,0xff,0xff,0xff};    //{0x11,0x11,0x11,0x11,0x11,0x11};   //密码
uint8_t Card_Data[16];
uint8_t status;
uint8_t task=0;
uint16_t cardType;

#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改

//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={0x17,0x23,0x6f,0x60,0,0};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)
#define FLASH_SAVE_ADDR  0X0802C124 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.

extern SysPara AS608Para;//指纹模块AS608参数
extern u16 ValidN;//模块内有效指纹个数
// u8** kbd_tbl;

#endif

