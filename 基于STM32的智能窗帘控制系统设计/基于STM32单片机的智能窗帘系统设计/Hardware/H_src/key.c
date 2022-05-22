#include "key.h"

/**
 * @name: get_Key_Scan
 * @description: 按键返回值获取函数
 * @param {uint8_t : 按键返回值}
 * @return {void}
 */
uint8_t get_Key_Scan(void)
{
	if(KEY1 == 0){
		Delay_ms(10);
		if(KEY1 == 0){
		
			return 1;
		}
	}
	if(KEY2 == 0){
		Delay_ms(10);
		if(KEY2 == 0){
		
			return 2;
		}
	}
	if(KEY3 == 0){
		Delay_ms(10);
		if(KEY3 == 0){
		
			return 3;
		}
	}
	if(KEY4 == 0){
		Delay_ms(10);
		if(KEY4 == 0){
		
			return 4;
		}
	}
	return 0;
}


u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		Delay_ms(30);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}

