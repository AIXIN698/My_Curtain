#ifndef __FLASH_H_
#define __FLASH_H_

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define FLASH_SIZE 64          //所选MCU的FLASH容量大小(单位为K)

#if FLASH_SIZE<256
  #define SECTOR_SIZE           1024    //字节
#else 
  #define SECTOR_SIZE           2048    //字节
#endif

void FLASH_ReadMoreData(uint32_t startAddress,uint16_t *readData,uint16_t countToRead);
uint16_t FLASH_ReadHalfWord(uint32_t address);
uint32_t FLASH_ReadWord(uint32_t address);
void FLASH_WriteMoreData(uint32_t startAddress,uint16_t *writeData,uint16_t countToWrite);

#endif
