#include "Usart.h"

char USART1_RX_BUF[100];				//":MSG\r\n"
uint8_t USART1_RX_STA=0;
//uint16_t USART2_RX_STA=0; 
//u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
//u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];

void Uart1_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启usart1（用来串口调试信息）
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//GPIOA开启
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//串口中断开启
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

//void Uart2_Init(uint32_t Bound)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//GPIOA开启
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	USART_InitTypeDef USART_InitStructure;
//	USART_InitStructure.USART_BaudRate = Bound;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_Init(USART2, &USART_InitStructure);

//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);											//根据指定的参数初始化VIC寄存器
//	
//	USART_Cmd(USART2, ENABLE);
//	
//	USART2_RX_STA=0;
//}

void Serial_SendByte(USART_TypeDef* USARTx,uint8_t Byte)
{
	USART_SendData(USARTx, Byte);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx,Array[i]);
	}
}

void Serial_SendString(USART_TypeDef* USARTx,char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(USARTx,String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx,Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(USART1,ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(USART1,String);
}

void USART1_IRQHandler(void)//蓝牙模块
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			if (RxData == ':' && USART1_RX_STA == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '.')
			{
				RxState = 2;
			}
			else
			{
				USART1_RX_BUF[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				USART1_RX_BUF[pRxPacket] = '\0';
				USART1_RX_STA = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

//void USART2_IRQHandler(void)
//{
//	u8 res;	      
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
//	{	 
//		res =USART_ReceiveData(USART2);		 
//		if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//还可以接收数据
//		{
//				TIM_SetCounter(TIM4,0);      //计数器清空          			
//				if(USART2_RX_STA==0) 				//使能定时器4的中断 
//				{
//					TIM_Cmd(TIM4,ENABLE);//使能定时器4
//				}
//				USART2_RX_BUF[USART2_RX_STA++]=res;	//记录接收到的值	 
//			}
//		else 
//			{
//				USART2_RX_STA|=1<<15;				//强制标记接收完成
//			} 
//		}
//	}  		

void BlueToothMatch()
{
	if (strcmp(USART1_RX_BUF, "Praymerry") == 0)
	{
				LED1_OFF();
				LED2_OFF();
				LED3_ON();
//				Serial_SendString("LED1_ON_OK\n");//pc test
				Delay_ms(500);
				Servo_SetAngle(KAngle);
				Delay_ms(100);
				OLED_Clear();
				OLED_ShowString(20, 28, "Door Open!", OLED_8X16);
				OLED_Update();
				USART1_RX_STA = 0;
				Delay_ms(3000);
	}
	else 
	{
				LED2_OFF();
				LED3_OFF();
				LED1_ON();
				OLED_Clear();
				OLED_ShowString(6,16, "Password error!", OLED_8X16);
				OLED_ShowString(22,32, "Try again", OLED_8X16);
				OLED_Update();
				USART1_RX_STA = 0;
				Delay_ms(3000);
	}
}
	
