#ifndef __UART1_H_
#define __UART1_H_
#include "gpio.h"
#include "oled.h"
#include "stdio.h"

extern uint8_t CW_Flag;
extern uint8_t REV_Flag;

void UARE1_Init(void);            //����1��ʼ������
void USARE1_detection(void);     //����1���ݼ�⺯��

#endif
