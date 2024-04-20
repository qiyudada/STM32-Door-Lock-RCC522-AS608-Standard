#include "spi.h"
#include "stm32f10x.h"                  // Device header



void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//只发送模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//波特率预分频值：波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);//初始化SPI
	SPI_Cmd(SPI1, ENABLE);//使能SPI
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;//sck mosi miso
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//NSS信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//波特率预分频值：波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(SPI2,&SPI_InitStructure);//初始化SPI
	
	SPI_SSOutputCmd(SPI2,ENABLE);
	SPI_Cmd(SPI2, ENABLE);//使能SPI
}

/**
  * @brief  写1字节数据到SPI总线
  * @param  SPIx 需要使用的SPI
  * @param  TxData 写到总线的数据
  * @retval 数据发送状态
  *		@arg 0 数据发送成功
  * 	@arg -1 数据发送失败
  */
int32_t SPI2_WriteByte(uint16_t TxData)
{
	uint8_t retry=0;				 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0)			//等待发送区空	
	{
		retry++;
		if(retry>200)return -1;
	}			  
	SPI2->DR=TxData;	 	  				//发送一个byte 
	retry=0;
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0)				//等待接收完一个byte  
	{
		retry++;
		if(retry>200)return -1;
	}  
	SPI2->DR;						    
	return 0;          				//返回收到的数据
}

/**
  * @brief  向SPI总线写多字节数据
  * @param  SPIx 需要使用的SPI
  * @param  p_TxData 发送数据缓冲区首地址
  * @param	sendDataNum 发送数据字节数
  * @retval 数据发送状态
  *		@arg 0 数据发送成功
  * 	@arg -1 数据发送失败
  */
int32_t SPI2_WriteNBytes(uint8_t *p_TxData,uint32_t sendDataNum)
{
	int retry=0;
	while(sendDataNum--){
		while((SPI2->SR&SPI_I2S_FLAG_TXE)==0)			//等待发送区空	
		{
			retry++;
			if(retry>20000)return -1;
		}	
		SPI2->DR=*p_TxData++;		//发送一个byte
		retry=0;
		while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0) 				//等待接收完一个byte  
		{
			SPI2->SR = SPI2->SR;
			retry++;
			if(retry>20000)return -1;
		} 
		SPI2->DR;
	}
	return 0;
}
/**
  * @brief  从SPI总线读取多字节数据
  * @param  SPIx 需要使用的SPI
  * @param  p_RxData 数据储存地址
  * @param	readDataNum 读取数据字节数
  * @retval 数据读取状态
  *		@arg 0 数据读取成功
  * 	@arg -1 数据读取失败
  */
int32_t SPI2_ReadNBytes(uint8_t *p_RxData,uint32_t readDataNum)
{
	int retry=0;
	while(readDataNum--){
		SPI2->DR = 0xFF;
		while(!(SPI2->SR&SPI_I2S_FLAG_TXE)){
			retry++;
			if(retry>20000)return -1;
		}
		retry = 0;
		while(!(SPI2->SR&SPI_I2S_FLAG_RXNE)){
			retry++;
			if(retry>20000)return -1;
		}
		*p_RxData++ = SPI2->DR;
	}	
	return 0;
}
