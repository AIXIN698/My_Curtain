#ifndef __OLED_H_
#define __OLED_H_

#include "stm32f10x.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据 


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_8)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_11)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_11)


//OLED控制用函数
void OLED_Hardware_Init(void);
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Draw(uint8_t byte);

void OLED_ShowChar(uint16_t x,uint16_t y, uint8_t num, uint8_t size);
void OLED_ShowString(uint16_t x,uint16_t y,char *p, uint8_t size);
void OLED_ShowInt32Num(uint16_t x,uint16_t y, int32_t num, uint8_t len, uint8_t size);
void OLED_DrawFont16(uint16_t x, uint16_t y, char *s);
void OLED_DrawFont32(uint16_t x, uint16_t y, char *s);
void OLED_Show_Str(uint16_t x, uint16_t y, char *str,uint8_t size);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif

