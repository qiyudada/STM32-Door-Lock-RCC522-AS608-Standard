#ifndef __SPI_H
#define __SPI_H			  	 
#include "sys.h"


void SPI1_Init(void);
void SPI2_Init(void);

int32_t SPI2_WriteByte(uint16_t TxData);
int32_t SPI2_WriteNBytes(uint8_t *p_TxData,uint32_t sendDataNum);
int32_t SPI2_ReadNBytes(uint8_t *p_RxData,uint32_t readDataNum);

#endif




