#ifndef __TASKING_H_
#define __TASKING_H_
#include "oled.h"
#include "gpio.h"
#include "key.h"
#include "step.h"
#include "bh1750.h"
#include "flash.h"
#include "uart1.h"
#include "stdio.h"
#include "string.h"

#define DEVICE_INFO_ADDRESS  0x800c000

/* KEY value */
extern uint16_t get_key_value; 
/* Global variable of motor forward rotation */
extern uint8_t CW_Flag;
/* Motor reverses global variable */
extern uint8_t REV_Flag;
/* Curtain opening flag bit */
extern uint8_t Curtain_Open_flag;
/* Curtain closing flag bit */
extern uint8_t Curtain_Close_flag;
/* Pattern to add */
extern uint8_t Pattern_Add_flag;
/* Pattern to minus */
extern uint8_t Pattern_Minus_flag;
/* Light value */
extern float Light;
/* Flash Write to the data cache */
extern uint8_t FlashWBuff [255];
/* Flash Reads the data cache */
extern uint16_t FlashRBuff [255];
/* Data written to the Flash   */
extern uint16_t FlashTest;
/* The serial port receives data variables */
extern char USARTReceBuff[8]; 
extern uint8_t usart_falg;

void KEY_SCAN(void);
void Mode_Selection(void);   // ģʽѡ��
void Mode_Selection_Display(uint8_t mode);   // ģʽ�˵���ʾ
void Mode_Confirm(uint8_t mode);   // ģʽȷ��
void Manual_Mode(void);   // �ֶ�����ģʽ
void Lighting_Intelligent(void);   // ��������ģʽ
void Voice_Intelligent(void);   // ��������ģʽ
void Long_Range_Control(void);   // Զ�̿���ģʽ
void System_Setup(void);   // ϵͳ����

void System_Confirm(uint8_t mode);   // ����ѡ����
void Reset_System_Function(void);   // ����ϵͳ����
#endif
