#ifndef __STEP_H_
#define __STEP_H_
#include "stm32f10x.h"


void Step_Motor_GPIO_Init(void);
void SetMotor(unsigned char InputData);
void motorNcircle(int n,uint8_t position);

#endif
