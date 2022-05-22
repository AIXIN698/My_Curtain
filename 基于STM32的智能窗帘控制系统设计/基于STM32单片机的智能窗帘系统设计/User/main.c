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

 * 项目名称:   智能窗帘控制系统
 * 项目描述:   由按键K1~K4控制整个系统运行，OLED菜单显示，BH1750模块采集光强数据，ESP8266-01S进行与上位机数据交互，语音智能模块;
 * 项目功能;   1、手动控制模式     2、光照智能模式     3、语音智能模式     4、远程控制模式
 * 项目开发:   AIXIN698
 * 开发日期:   2022.5.21

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

	OLED_Clear();                       //清屏（全黑）
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	OLED_WR_Byte(0x27,OLED_CMD);        //水平向左或者右滚动 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);        //起始页 0
	OLED_WR_Byte(0x07,OLED_CMD);        //滚动时间间隔
	OLED_WR_Byte(0x04,OLED_CMD);        //终止页 2
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);        //虚拟字节
	Test_MainPage();
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动	
	
	//FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作  
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作 将DEVICE_INFO_ADDRESS里边的数据读出存放至FlashRBuff数组
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
 * @description: 主界面滚动测试函数
 * @param {void}
 * @return {void}
 */
void Test_MainPage(void)
{

	OLED_ShowString(12,0,"Smart Curtain",16);
	OLED_Show_Str(0,3,"智能窗帘控制系统",16);
	OLED_ShowString(0,6,"K1 Pick mode",16);
	
}

/**
 * @name: Test_MainPage_Close
 * @description: 主界面滚动关闭函数
 * @param {void}
 * @return {void}
 */
void Test_MainPage_Close(void)
{
	OLED_Clear();
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	Test_MainPage();
}

/**
 * @name: Test_MainPage_Open
 * @description: 主界面滚动开启函数
 * @param {void}
 * @return {void}
 */
void Test_MainPage_Open(void)
{
	OLED_Clear();
	OLED_ShowString(12,0,"Smart Curtain",16);
	OLED_Show_Str(0,3,"智能窗帘控制系统",16);
	OLED_ShowString(0,6,"K1 Pick mode",16);
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
}





