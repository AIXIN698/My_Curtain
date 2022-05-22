#include "stm32f10x.h" 
#include "delay.h"
#include "gpio.h"
#include "oled.h"
#include "key.h"
#include "tasking.h"
#include "bh1750.h"
#include "flash.h"
#include "uart1.h"
#include "stdio.h"
#include "string.h"

/*

 * ��Ŀ����:   ���ܴ�������ϵͳ
 * ��Ŀ����:   �ɰ���K1~K4��������ϵͳ���У�OLED�˵���ʾ��BH1750ģ��ɼ���ǿ���ݣ�ESP8266-01S��������λ�����ݽ�������������ģ��;
 * ��Ŀ����;   1���ֶ�����ģʽ     2����������ģʽ     3����������ģʽ     4��Զ�̿���ģʽ
 * ��Ŀ����:   AIXIN698
 * ��������:   2022.5.21

*/

/* KEY value */
uint16_t get_key_value; 
/* Global variable of motor forward rotation */
uint8_t CW_Flag;
/* Motor reverses global variable */
uint8_t REV_Flag;
/* Curtain opening flag bit */
uint8_t Curtain_Open_flag = 1;
/* Curtain closing flag bit */
uint8_t Curtain_Close_flag = 1;
/* Pattern to add */
uint8_t Pattern_Add_flag = 1;
/* Pattern to minus */
uint8_t Pattern_Minus_flag = 0;
/* Light value */
float Light = 0;
/* Curtain status data Store address to flash */
#define DEVICE_INFO_ADDRESS  0x800c000
/* Flash Write to the data cache */
uint8_t FlashWBuff [255];
/* Flash Reads the data cache */
uint16_t FlashRBuff [255];
/* Data written to the Flash   */
uint16_t FlashTest = 0;


void Test_MainPage(void);
void Test_MainPage_Close(void);
void Test_MainPage_Open(void);
int main(void)
{
	Delay_init();
	UARE1_Init();
	GPIO_X_Init();	
	OLED_Init();
	Step_Motor_GPIO_Init();
	BH1750_Init();

	OLED_Clear();                       //������ȫ�ڣ�
	OLED_WR_Byte(0x2E,OLED_CMD);        //�رչ���
	OLED_WR_Byte(0x27,OLED_CMD);        //ˮƽ��������ҹ��� 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0x00,OLED_CMD);        //��ʼҳ 0
	OLED_WR_Byte(0x07,OLED_CMD);        //����ʱ����
	OLED_WR_Byte(0x04,OLED_CMD);        //��ֹҳ 2
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0xFF,OLED_CMD);        //�����ֽ�
	Test_MainPage();
	OLED_WR_Byte(0x2F,OLED_CMD);        //��������	
	
	//FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // д����  
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // ������ ��DEVICE_INFO_ADDRESS��ߵ����ݶ��������FlashRBuff����
	while(1)
	{
		//OLED_ShowInt32Num(0,0,(uint32_t)FlashRBuff[0],2,16);
		get_key_value = KEY_Scan(0);
		if(get_key_value == 1)      Mode_Selection();
		else if(get_key_value == 2)   Test_MainPage_Close();	
		else if(get_key_value == 3)	  Test_MainPage_Open();
			
	}
}

/**
 * @name: Test_MainPage
 * @description: ������������Ժ���
 * @param {void}
 * @return {void}
 */
void Test_MainPage(void)
{

	OLED_ShowString(12,0,"Smart Curtain",16);
	OLED_Show_Str(0,3,"���ܴ�������ϵͳ",16);
	OLED_ShowString(0,6,"K1 Pick mode",16);
	
}

/**
 * @name: Test_MainPage_Close
 * @description: ����������رպ���
 * @param {void}
 * @return {void}
 */
void Test_MainPage_Close(void)
{
	OLED_Clear();
	OLED_WR_Byte(0x2E,OLED_CMD);        //�رչ���
	Test_MainPage();
}

/**
 * @name: Test_MainPage_Open
 * @description: �����������������
 * @param {void}
 * @return {void}
 */
void Test_MainPage_Open(void)
{
	OLED_Clear();
	OLED_ShowString(12,0,"Smart Curtain",16);
	OLED_Show_Str(0,3,"���ܴ�������ϵͳ",16);
	OLED_ShowString(0,6,"K1 Pick mode",16);
	OLED_WR_Byte(0x2F,OLED_CMD);        //��������
}





