#ifndef _Usart_H
#define _Usart_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Timer.h"
#include <string.h>
#include "LED.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM.h"
#include "Delay.h"
#include "Key.h"

//#define USART2_MAX_RECV_LEN		400					//最大接收缓存字节数
//#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数
//#define USART2_RX_EN 			1					      //0,不接收;1,接收.

extern char USART1_RX_BUF[100];				//":MSG.\n"
extern uint8_t USART1_RX_STA;
//extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
//extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
//extern uint16_t USART2_RX_STA;

void Uart1_Init(void);
//void Uart2_Init(uint32_t Bound);
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);
void Serial_SendString(USART_TypeDef* USARTx,char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);
int fputc(int ch, FILE *f);
void Serial_Printf(char *format, ...);
void BlueToothMatch(void);

#endif
