#include "key.h"

/**
 * @name: get_Key_Scan
 * @description: ��������ֵ��ȡ����
 * @param {uint8_t : ��������ֵ}
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
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		Delay_ms(30);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// �ް�������
}

