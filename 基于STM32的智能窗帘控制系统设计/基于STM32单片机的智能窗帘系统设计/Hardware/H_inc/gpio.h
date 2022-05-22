#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

/* KEY macro definition GPIOC KEY code begin to*/

#define KEY_GPIO_Pin_14     GPIO_Pin_14
#define KEY_GPIO_Pin_15     GPIO_Pin_15
#define KEY_GPIOC     GPIOC

/* KEY macro definition GPIOC KEY code END */

/* KEY macro definition GPIOA KEY code begin to*/

#define KEY_GPIO_Pin_0     GPIO_Pin_0
#define KEY_GPIO_Pin_1     GPIO_Pin_1
#define KEY_GPIOA     GPIOA

/* KEY macro definition GPIOA KEY code END */

/* LED macro definition GPIOC LED code begin to */

#define LED_GPIO_Pin_13    GPIO_Pin_13

#define LED_1   PCout(13)

/* LED macro definition GPIOC LED code END */

/* Get key value macro definition code begin to */

#define KEY1  GPIO_ReadInputDataBit(KEY_GPIOC,GPIO_Pin_14)//读取按键0
#define KEY2  GPIO_ReadInputDataBit(KEY_GPIOC,GPIO_Pin_15)//读取按键1
#define KEY3  GPIO_ReadInputDataBit(KEY_GPIOA,GPIO_Pin_0)//读取按键2 
#define KEY4  GPIO_ReadInputDataBit(KEY_GPIOA,GPIO_Pin_1)//读取按键3

/* Get key value macro definition code END */



void GPIO_X_Init(void);

#endif
