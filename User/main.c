#include "main.h"

/************Pin Planning****************************************************
LED:red PC13 
	white PC14 
	green PC15

KEY: PB1 PB11

OLED: SCL PB8 
      SDA PB9(soft)

BlueTooth:(usart1) TXD PA10 
		   			RXD PA9

RC522:(SPI2)  SDA PB12 (NSS)
			  SCL(SCK) PB13
			  MOSI PB15
			  MISO PB14
			  IRQ  PB3
			  RES  PB4

Sonicware: Trig PA0
		  echo PA1

FingerPrint:(usart2) TX PA3
			RX PA2
			TCH PA6
			UA 3v3
Servo: PWM PB0
	   VCC +5V

Serial:(usart1) TX PA10
				RX PA9
					
**This is a Reference,if involved infringement of copyright,please advise*/
/*************************************************************************/

int main(void)
{
/****************************INIT****************************************/
/***************************OLED*****************************************/
	OLED_Init();
/***************************LED*****************************************/	
	LED_Init();
/***************************Key*****************************************/
	Key_Init();	//按键初始化 
/***************************USART*****************************************/ 
	Uart1_Init();//设置了115200(Bluetooth/Serial)
	usart2_init(usart2_baund);
/***************************TIM*****************************************/		
	TIM4_Int_Init(9999,7199);//1s(Usart 2)
	Timer2_Init();
/***************************HCSR04*****************************************/	
	HCSR04_Init();//UltraSonicWare use TIM2
/***************************LCD*****************************************/	
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	// float t=0; // a value for lcd
/***************************RCC522*****************************************/
	RC522_Init();//Open sensor card detector
	Card_Type1[0]=0x04;
	Card_Type1[1]=0x00;
//	Delay_init();	//延时函数初始化	
	PcdResetON();//复位启动
/***************************Servo*****************************************/
	Servo_Init();//servo use TIM3
	LED2_ON();//white light
	Delay_ms(100);
	Servo_SetAngle(OAngle);//Original Angle
	Delay_ms(100);
/***************************OLEDUpdate*****************************************/
	OLED_Clear();
	OLED_Update();// Updata Oled for snow
/***************************AS608*****************************************/	
	PS_StaGPIO_Init();
	// u8 ensure;
	/*Remind user on pc usart1*/
	Serial_Printf("AS608 Fingerprint module test\r\n");
	Serial_Printf("Author: Praymerry\r\n");
	Serial_Printf("Connect with AS608.....\r\n");

	while(PS_HandShake(&AS608Addr))//与AS608模块握手
	{
		OLED_Clear();
		OLED_ShowString(25,16,"Initing...",OLED_8X16);
		OLED_Update();
		Serial_Printf("Cannot connect with AS608!\r\n");
		Delay_ms(1000);
		Serial_Printf("Try to connect again....\r\n");
		Delay_ms(1000);	  
	}
		OLED_Clear();
		OLED_ShowString(25,16,"Initing OK!",OLED_8X16);
		OLED_Update();
		Delay_ms(1000);
		Serial_Printf("Connect Success!\r\n");//look at pc
/***************************************Function area**********************************/	
	while(1)
	{
		if((SKeyNum=SKey_GetNum())!=0)//first press to enter Key control
		{
			OLED_Clear();
			OLED_ShowString(24,0,"Keep Input",OLED_8X16);
			OLED_ShowString(16,32,">>UnLocking<<",OLED_8X16);
			OLED_Update();
			KeyNum=0;
			Delay_ms(1000);
			while(1)
			{
				KeyNum=0;
				task=1;
				if((KeyNum=Key_GetNum())!=0)
				{
					break;
				}
			}	
		}
		else if(USART1_RX_STA == 1)// Secret is  :Praymerry.
		{
			OLED_Clear();
			OLED_ShowString(14,16,">>BlueTooth<<",OLED_8X16);
			OLED_ShowString(20,32,"Verifing...",OLED_8X16);
			OLED_Update();
			Delay_ms(1000);
			task=2;
		}
		else if(MI_OK==PcdRequest(0x52, Card_Type1))
		{
			OLED_Clear();
			OLED_ShowString(24,16,">>NFC<<",OLED_8X16);
			OLED_ShowString(20,32,"Verifing...",OLED_8X16);
			OLED_Update();
			Delay_ms(1000);
			task=3;
		}
		else if(PS_Sta !=0)
		{
			
				OLED_Clear();
				OLED_ShowString(14,16,">>Finger<<",OLED_8X16);
				OLED_Update();
				task=4;	
			
		}
		else if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0))
		{
			OLED_Clear();
			OLED_ShowString(14,16,">>AddFinger<<",OLED_8X16);
			OLED_Update();
			task=5;	
		}
		//Assignment Choose
		switch(task)
		{
			case 1 :PassMatch(OPassword);//The way of  using Key unlock door
					task=0;
//					while((T=HCSR04_GetValue())<20);//sonic ware value(if people doesn't close to door )
			        break;
			case 2 :BlueToothMatch();
							task=0;
//							while((T=HCSR04_GetValue())<20);
							break;
			
			case 3 :NFCMatch();
							task=0;
//							while((T=HCSR04_GetValue())<20);
							break;
			case 4 :FrMatch();
							task=0;
// 							while((T=HCSR04_GetValue())<20);
							break;
			case 5 : Add_FR();
							task=0;
							break;				

			default : LED1_OFF();//default state	
							LED2_ON();
							LED3_OFF();
							Servo_SetAngle(OAngle);
							OLED_Clear();
							OLED_ShowString(36,8,"Welcome",OLED_8X16);
							OLED_ShowString(55,24,"To",OLED_8X16);
							OLED_ShowString(30,40,"Wonderland",OLED_8X16);
							OLED_Update();
							break;
		}
		 
	 	
	}
	
}
/***************************************Example**********************************/
/*********************************************************************************/
/*finger print model*/
/***outside while*/

// /*提醒用户语句*/
// 	Serial_Printf("AS608 Fingerprint module test\r\n");
// 	Serial_Printf("Author: Praymerry\r\n");
// 	Serial_Printf("Connect with AS608.....\r\n");
	
// 	while(PS_HandShake(&AS608Addr))//与AS608模块握手
// 	{
// 		Serial_Printf("Cannot connect with AS608!\r\n");
// 		Delay_ms(1000);
// 		Serial_Printf("Try to connect again....\r\n");
// 		Delay_ms(1000);	  
// 	}
// 	Serial_Printf("Connect Success!\r\n");
// 	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	
// 	if(ensure!=0x00)
// 	{
// 		OLED_Clear();
// 		OLED_ShowString(0,0,"error",OLED_6X8);
// 		OLED_Update();
// 		ShowErrMessage(ensure);//显示确认码错误信息	
// 		ensure=PS_ReadSysPara(&AS608Para);  //读AS608模块参数
// 	}		
// 	if(ensure==0x00)
// 	{
// 		OLED_Clear();
// 		OLED_ShowString(0,0,"ok",OLED_6X8);
// 		OLED_Update();
// 	}
// 	else
// 		OLED_Clear();
// 		OLED_ShowString(0,0,"ok but error",OLED_6X8);
// 		OLED_Update();
// 		ShowErrMessage(ensure);	
/******inside while *****/
// KeyNum=Key_GetNum();
// 		if(KeyNum)
// 		{
// 			if(KeyNum==KEY0_PRES)
// 				Del_FR();		//删指纹
// 			if(KeyNum==KEY1_PRES)
// 				Add_FR();		//录指纹									
// 		}
// 		OLED_Clear();
// 		if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
// 		{
// 			OLED_Clear();
// 			OLED_ShowString(0,0,"verify",OLED_6X8);
// 			OLED_Update();
// 			press_FR();//刷指纹			
// 		}

/*********************************************************************************/
//// /*servo drive*/
//		OLED_ShowString(16, 0, "Angle:",OLED_8X16);
//		KeyNum = Key_GetNum();
//		if (KeyNum == 1)
//		{
//			Angle += 30;
//			if (Angle > 180)
//			{
//				Angle = 0;
//			}
//		}
//		Servo_SetAngle(Angle);
//		OLED_ShowNum(16,48, Angle, 3,OLED_8X16);
//		OLED_Update();
/*********************************************************************************/
    /*RC522 test*///outside while()
 
//Serial_Printf("-----hello world!-----\r\n");
// 
//	Delay_init();
//	RC522_Init();
//	PcdReset();  //复位RC522
//	PcdAntennaOff();  //关闭天线
//	Delay_ms(100);
//	PcdAntennaOn();  //开启天线
//	
//	Serial_Printf("-----test start!-----\r\n");

// inside while()
//		Delay_ms(10);
//		if(MI_OK==PcdRequest(0x52, Card_Type1))  //寻卡函数，如果成功返回MI_OK  打印多次卡号
////		if(MI_OK==PcdRequest(0x26, Card_Type1))  //寻卡函数，如果成功返回MI_OK   打印1次卡号
//		{
//			uint16_t cardType = (Card_Type1[0]<<8)|Card_Type1[1];//读不同卡的类型
//			Serial_Printf("卡类型：(0x%04X)\r\n",cardType);  //"Card Type(0x%04X):"
//			switch(cardType){
//			case 0x4400:
//					Serial_Printf("Mifare UltraLight\r\n");
//					break;
//			case 0x0400:
//					Serial_Printf("Mifare One(S50)\r\n");
//					break;
//			case 0x0200:
//					Serial_Printf("Mifare One(S70)\r\n");
//					break;
//			case 0x0800:
//					Serial_Printf("Mifare Pro(X)\r\n");
//					break;
//			case 0x4403:
//					Serial_Printf("Mifare DESFire\r\n");
//					break;
//			default:
//					Serial_Printf("Unknown Card\r\n");
//					break;
//			}
//			
//			status = PcdAnticoll(Card_ID);//防冲撞 如果成功返回MI_OK
//			if(status != MI_OK){
//					Serial_Printf("Anticoll Error\r\n");
//			}else{
//					Serial_Printf("Serial Number:%02X%02X%02X%02X\r\n",Card_ID[0],Card_ID[1],Card_ID[2],Card_ID[3]);
//			}
//			
//			status = PcdSelect(Card_ID);  //选卡 如果成功返回MI_OK
//			if(status != MI_OK){
//					Serial_Printf("Select Card Error\r\n");
//			}
//			else
//				Serial_Printf("Select Card OK\r\n");
//			status = PcdHalt();  //卡片进入休眠状态
//			if(status != MI_OK){
//					Serial_Printf("PcdHalt Error\r\n");			
//			}
//			else
//			{
//					Serial_Printf("PcdHalt OK\r\n");	
//			}
//		}
/*********************************************************************************/

//     /*LCD test*/
//		LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);	
//		LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
//		LCD_ShowString(24,50,"LCD_H:",BLUE,WHITE,16,0);
//		LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
//		LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
//		t+=0.11;
//		LCD_ShowPicture(65,80,40,40,gImage_1);
/*********************************************************************************/
///*OLED test*/
///*在(16, 0)位置显示字符串"Hello World!"，字体大小为8*16点阵*/
//	OLED_ShowString(16, 0, "Hello World!", OLED_8X16);
//	/*在(0, 18)位置显示字符'A'，字体大小为6*8点阵*/
//	OLED_ShowChar(0, 18, 'A', OLED_6X8);
//	
//	/*在(16, 18)位置显示字符串"Hello World!"，字体大小为6*8点阵*/
//	OLED_ShowString(16, 18, "Hello World!", OLED_6X8);
//	
//	/*在(0, 28)位置显示数字12345，长度为5，字体大小为6*8点阵*/
//	OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
//	
//	/*在(40, 28)位置显示有符号数字-66，长度为2，字体大小为6*8点阵*/
//	OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
//	
//	/*在(70, 28)位置显示十六进制数字0xA5A5，长度为4，字体大小为6*8点阵*/
//	OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);
//		
//	OLED_Update();
/*********************************************************************************/
  
		

	/*Light up*/
//	LED1_ON();
//	LED2_ON();
//	LED3_ON();
	
	/*Ultrsonicware show*/
//	OLED_ShowString(1, 1, "distance:");
//	OLED_ShowString(1, 13, "cm");
//		T=HCSR04_GetValue();
//		OLED_ShowNum(1,10,T,3);
//		Delay_ms(100);


