#ifndef __UART1_H_
#define __UART1_H_
#include "gpio.h"
#include "oled.h"
#include "stdio.h"

extern uint8_t CW_Flag;
extern uint8_t REV_Flag;

void UARE1_Init(void);            //串口1初始化函数
void USARE1_detection(void);     //串口1数据检测函数

#endif
