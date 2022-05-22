#include "tasking.h"


uint8_t curtain_open_flag = 1;
uint8_t curtain_close_flag = 1;

/**
 * @name: KEY_SCAN
 * @description: 按键扫描处理函数
 * @param {void}
 * @return {void}
 */
void KEY_SCAN(void)
{
	/* Gets the key return value */
	get_key_value = get_Key_Scan();
		if((get_key_value == 1 || CW_Flag == 1) && Curtain_Open_flag == 1){		
			LED_1 = 0;
			Curtain_Open_flag = 0;
			Curtain_Close_flag = 1;
			OLED_Show_Str(32,6,"打开窗帘",16);	
			/* Motor forward */
			motorNcircle(1,1);
			CW_Flag = 0;
		}		
		else if((get_key_value == 2 || REV_Flag == 1) && Curtain_Close_flag == 1){
			LED_1 = 1;
			Curtain_Close_flag = 0;
			Curtain_Open_flag = 1;
			OLED_Show_Str(32,6,"关闭窗帘",16);
			/* APRS */
			motorNcircle(1,2);
			REV_Flag = 0;
		}
		else if(get_key_value == 3){
		
			LED_1 = 0;
			Delay_ms(500);
			LED_1 = 1;
			Delay_ms(500);
		}
		else if(get_key_value == 4){
		
			LED_1 = 0;	
			Delay_ms(1000);
			LED_1 = 1;
			Delay_ms(1000);
		}
}

/**
 * @name: Mode_Selection
 * @description: 模式选择函数
 * @param {void}
 * @return {void}
 */
void Mode_Selection(void)
{
	uint8_t mode_flag = 1;
	uint8_t get_mode_sum = 0;
	OLED_Clear();
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	while(mode_flag)
	{
		/* Gets the key return value */
		get_mode_sum = KEY_Scan(0);
		if(get_mode_sum == 1){
			Pattern_Add_flag++;
			
		}else if(get_mode_sum == 2){
			Pattern_Add_flag--;
			
		}else if(get_mode_sum == 3){
			// 这里为确认键 确认选择该模式
			Mode_Confirm(Pattern_Add_flag);
		}
		
		if(Pattern_Add_flag>5)
			Pattern_Add_flag = 1;
		if(Pattern_Add_flag<1)
			Pattern_Add_flag = 5;
		//OLED_ShowInt32Num(0,0,Pattern_Add_flag,1,16);   // 按键调试
		Mode_Selection_Display(Pattern_Add_flag);
		
		if(get_mode_sum == 4){
			// 这里为退出键 点击退出模式选择界面
			OLED_Clear();
			
			OLED_ShowString(12,0,"Smart Curtain",16);
			OLED_Show_Str(0,3,"智能窗帘控制系统",16);
			OLED_ShowString(0,6,"K1 Pick mode",16);
			OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
			
			mode_flag = 0;
			Pattern_Add_flag = 1;
		}
	}
}

/**
 * @name: Mode_Selection_Display
 * @description: 模式选择显示函数
 * @param {uint8_t mode :对应显示的模式编号 }
 * @return {void}
 */
void Mode_Selection_Display(uint8_t mode)
{
	switch(mode)
	{
		case 1:  
			OLED_Show_Str(16,1,"手动控制模式",16);
			OLED_ShowString(0,4," K3 ON    K4 OFF",16);
			OLED_ShowString(0,6,"<<- K1    K2 ->>",16);
			break;
		case 2:	 OLED_Show_Str(16,1,"光照智能模式",16);
			OLED_ShowString(0,4," K3 ON    K4 OFF",16);
			OLED_ShowString(0,6,"<<- K1    K2 ->>",16);
			break;
		case 3:	 
			OLED_Show_Str(16,1,"语音智能模式",16);
			OLED_ShowString(0,4," K3 ON    K4 OFF",16);
			OLED_ShowString(0,6,"<<- K1    K2 ->>",16);
			break;
		case 4:	 
			OLED_Show_Str(16,1,"远程控制模式",16);
			OLED_ShowString(0,4," K3 ON    K4 OFF",16);
			OLED_ShowString(0,6,"<<- K1    K2 ->>",16);		
			break;
		case 5:	 
			OLED_Show_Str(16,1,"系统设置模式",16);
			OLED_ShowString(0,4," K3 ON    K4 OFF",16);
			OLED_ShowString(0,6,"<<- K1    K2 ->>",16);	
			break;
		default:
			break;
	}
}


/**
 * @name: Mode_Confirm
 * @description: 模式确认函数
 * @param {void}
 * @return {void}
 */
void Mode_Confirm(uint8_t mode)
{

	switch(mode)
	{
	
		case 1: Manual_Mode();   // 手动模式
			break;
		case 2: Lighting_Intelligent();   // 光照智能模式
			break;
		case 3: Voice_Intelligent();   // 语音智能模式
			break;
		case 4: Long_Range_Control();   // 远程控制模式
			break;
		case 5: System_Setup();   // 设置页面
		default:
			break;
	}
}

/**
 * @name: Manual_Mode
 * @description: 手动控制模式函数
 * @param {void}
 * @return {void}
 */
void Manual_Mode(void)
{
	uint8_t Manual_run_flag = 1;
	uint8_t Manual_get_key_sum = 0;
	OLED_Clear();
	OLED_Show_Str(16,0,"手动控制模式",16);
	OLED_Show_Str(0,6,"窗帘状态:",16);
	OLED_ShowString(6,3,"K1 ON    K2 OFF",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	   // 判断DEVICE_INFO_ADDRESS地址中也就是FlashRBuff[0]中的值,
	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);   // 11 为窗帘打开状态, 22 为窗帘关闭状态
	usart_falg = 0;
	while(Manual_run_flag)
	{
		
		if(Manual_get_key_sum == 1 || Manual_get_key_sum == 2) // 判断如果按键1或按键2触发,则读一次Flash中指定地址的数据
			FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
		
		Manual_get_key_sum = KEY_Scan(0);
		if(Manual_get_key_sum == 1 && curtain_open_flag == 1 && FlashRBuff[0] == 22){		
			// 打开窗帘
			OLED_Clear();
			OLED_Show_Str(16,0,"手动控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"打开中",16);
			OLED_ShowString(6,3,"<<<<< ON >>>>>",16);
			motorNcircle(1,1);	
			OLED_Show_Str(78,6,"已打开",16);
			FlashTest = 11;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作
			curtain_open_flag = 0;
			curtain_close_flag = 1;
		}else if(Manual_get_key_sum == 2 && curtain_close_flag == 1 && FlashRBuff[0] == 11){
		    // 关闭窗帘
			OLED_Clear();
			OLED_Show_Str(16,0,"手动控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"关闭中",16);	
			OLED_ShowString(6,3,">>>>> OFF <<<<<",16);
			motorNcircle(1,2);
			OLED_Show_Str(78,6,"已关闭",16);
			FlashTest = 22;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作			
			curtain_open_flag = 1;
			curtain_close_flag = 0;
		}else if(Manual_get_key_sum == 3){
			
		}else if(Manual_get_key_sum == 4){
			// 退至上一级菜单
			Manual_run_flag = 0;
			usart_falg = 1;
			OLED_Clear();
		}
	}
}

/**
 * @name: Lighting_Intelligent
 * @description: 光照智能模式函数
 * @param {void}
 * @return {void}
 */
void Lighting_Intelligent(void)
{
	uint8_t t = 0;
	uint8_t light_run_flag = 1;
	uint8_t light_get_key_sum = 0;
	//uint32_t light_value = 0;
	OLED_Clear();
	OLED_Show_Str(16,0,"光照智能模式",16);
	OLED_Show_Str(0,6,"窗帘状态:",16);
	OLED_ShowString(6,3,"Lux: ",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	
	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);	
	usart_falg = 0;	
	while(light_run_flag)
	{
		if(Light > 300 || Light < 80) // 判断如果按键1或按键2触发,则读一次Flash中指定地址的数据
		FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
		/* ----------------------------------------------------- */
		if(t%10==0)			//每100ms读取一次
		{		
			if (!i2c_CheckDevice(BH1750_Addr))
			{
				Light = LIght_Intensity();              //获取光照强度
			}		
			OLED_ShowInt32Num(45,3,Light,3,16);
			
		}
		Delay_ms(10);
		t++;
		if(t == 50){
			t = 0;
			printf("{Light:%d}\r\n",(int)Light);
		}
		/* ----------------------------------------------------- */
		
		if(Light > 300 && curtain_open_flag == 1 && FlashRBuff[0] == 22){
			// 打开窗帘
			OLED_Clear();
			usart_falg = 0;	
			OLED_Show_Str(16,0,"光照智能模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"打开中",16);
			OLED_ShowString(6,3,"<<<<< ON >>>>>",16);
			motorNcircle(1,1);
			OLED_Show_Str(78,6,"已打开",16);
			FlashTest = 11;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作
			OLED_Clear();
			OLED_Show_Str(16,0,"光照智能模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_ShowString(6,3,"Lux: ",16);
			OLED_Show_Str(78,6,"已打开",16);
			usart_falg = 1;	
			curtain_open_flag = 0;
			curtain_close_flag = 1;
		}else if(Light < 80 && curtain_close_flag == 1 && FlashRBuff[0] == 11){
			// 关闭窗帘
			OLED_Clear();
			usart_falg = 0;	
			OLED_Show_Str(16,0,"光照智能模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"关闭中",16);
			OLED_ShowString(6,3,">>>>> OFF <<<<<",16);
			motorNcircle(1,2);
			OLED_Show_Str(78,6,"已关闭",16);
			FlashTest = 22;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作
			OLED_Clear();
			OLED_Show_Str(16,0,"光照智能模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_ShowString(6,3,"Lux: ",16);
			OLED_Show_Str(78,6,"已关闭",16);
			usart_falg = 1;				
			curtain_open_flag = 1;
			curtain_close_flag = 0;
		}
		
		
		light_get_key_sum = KEY_Scan(0);
		if(light_get_key_sum == 1){
		
		}else if(light_get_key_sum == 2){
		
		}else if(light_get_key_sum == 3){
		
		}else if(light_get_key_sum == 4){
			// 退至上一级菜单
			light_run_flag = 0;
			OLED_Clear();
		}
	}
	
}

/**
 * @name: Voice_Intelligent
 * @description: 语音智能模式函数
 * @param {void}
 * @return {void}
 */
void Voice_Intelligent(void)
{
	uint8_t voice_run_flag = 1;
	uint8_t voice_get_key_sum = 0;
	OLED_Clear();
	OLED_Show_Str(16,0,"语音智能模式",16);
	OLED_Show_Str(0,6,"窗帘状态:",16);
	OLED_Show_Str(0,3,"小创为您服务!",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	
	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);
	usart_falg = 0;	
	while(voice_run_flag)
	{
		voice_get_key_sum = KEY_Scan(0);
		if(voice_get_key_sum == 1){
		
		}else if(voice_get_key_sum == 2){
		
		}else if(voice_get_key_sum == 3){
		
		}else if(voice_get_key_sum == 4){
			// 退至上一级菜单
			usart_falg = 1;
			voice_run_flag = 0;
			OLED_Clear();
		}
	}
	
}

/**
 * @name: Long_Range_Control
 * @description: 远程控制模式函数
 * @param {void}
 * @return {void}
 */
void Long_Range_Control(void)
{

	uint8_t long_range_run_flag = 1;
	uint8_t long_range_get_key_sum = 0;
	OLED_Clear();
	OLED_Show_Str(16,0,"远程控制模式",16);
	OLED_Show_Str(0,6,"窗帘状态:",16);
	OLED_ShowString(0,3,"APP data:",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	
	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);
	usart_falg = 1;	
	while(long_range_run_flag)
	{
		//OLED_ShowInt32Num(0,0,(uint32_t)FlashRBuff[0],2,16);
		//OLED_ShowInt32Num(0,2,curtain_open_flag,2,16);
		//OLED_ShowInt32Num(0,5,(uint32_t)USARTReceBuff[3],5,16);
		
		USARE1_detection();   // 串口1数据监测函数
		
		if(USARTReceBuff[3] != 0x00){ // 判断如果串口接收到了数据,则读一次Flash中指定地址的数据
			FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
		}
		if(USARTReceBuff[3] == 0x41 && curtain_open_flag == 1 && FlashRBuff[0] == 22){
			// 打开窗帘
			OLED_Clear();
			usart_falg = 0;	
			OLED_Show_Str(16,0,"远程控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"打开中",16);
			OLED_ShowString(6,3,"<<<<< ON >>>>>",16);
			motorNcircle(1,1);
			OLED_Show_Str(78,6,"已打开",16);
			FlashTest = 11;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作
			OLED_Clear();
			OLED_Show_Str(16,0,"远程控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_ShowString(0,3,"APP data:",16);
			OLED_ShowString(80,3,"0x00",16);
			OLED_ShowInt32Num(96,3,(int)USARTReceBuff[3],1,16);
			OLED_Show_Str(78,6,"已打开",16);
			USARTReceBuff[3] = 0x00;
			usart_falg = 1;	
			curtain_open_flag = 0;
			curtain_close_flag = 1;
		}else if(USARTReceBuff[3] == 0x42 && curtain_close_flag == 1 && FlashRBuff[0] == 11){
			// 关闭窗帘
			OLED_Clear();
			usart_falg = 0;	
			OLED_Show_Str(16,0,"远程控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_Show_Str(78,6,"关闭中",16);
			OLED_ShowString(6,3,">>>>> OFF <<<<<",16);
			motorNcircle(1,2);
			OLED_Show_Str(78,6,"已关闭",16);
			FlashTest = 22;
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest,sizeof(FlashTest));   // 写操作	
			OLED_Clear();
			OLED_Show_Str(16,0,"远程控制模式",16);
			OLED_Show_Str(0,6,"窗帘状态:",16);
			OLED_ShowString(0,3,"APP data:",16);
			OLED_ShowString(80,3,"0x00",16);
			OLED_ShowInt32Num(96,3,(int)USARTReceBuff[3],1,16);
			OLED_Show_Str(78,6,"已关闭",16);			
			USARTReceBuff[3] = 0x00;	
			usart_falg = 1;				
			curtain_open_flag = 1;
			curtain_close_flag = 0;
		}
		
		long_range_get_key_sum = KEY_Scan(0);
		if(long_range_get_key_sum == 1){
		
		}else if(long_range_get_key_sum == 2){
		
		}else if(long_range_get_key_sum == 3){
		
		}else if(long_range_get_key_sum == 4){
			// 退至上一级菜单
			usart_falg = 0;	
			long_range_run_flag = 0;
			OLED_Clear();
		}
	}
}

/**
 * @name: System_Setup
 * @description: 系统设置函数
 * @param {void}
 * @return {void}
 */
uint8_t set_value = 0;
void System_Setup(void)
{
	uint8_t set_run_flag = 1;
	uint8_t set_get_key_sum = 0;
	uint8_t set_sum_key = 0;
	OLED_Clear();
	OLED_Show_Str(32,0,"系统设置",16);
	OLED_Show_Str(20,3,"重置系统状态",16);
//	OLED_Show_Str(0,3,"小创为您服务!",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	
	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);	
	while(set_run_flag)
	{
		set_get_key_sum = KEY_Scan(0);
		if(set_get_key_sum == 1){
			set_sum_key = 1;
			set_value+=3;
			if(set_value >= 9)set_value = 3;
		}else if(set_get_key_sum == 2){
			set_sum_key = 1;
			set_value-=3;
			if(set_value <= 0)set_value = 6;
		}else if(set_get_key_sum == 3){
			System_Confirm(set_value);
		}else if(set_get_key_sum == 4){
			// 退至上一级菜单
			set_run_flag = 0;
			OLED_Clear();
		}
		
		if(set_value == 3 && set_sum_key){
			set_sum_key = 0;
			OLED_Clear();
			OLED_Show_Str(0,set_value,"→",16);
			OLED_Show_Str(32,0,"系统设置",16);
			OLED_Show_Str(20,3,"重置系统状态",16);
		}else if(set_value == 6 && set_sum_key){
			set_sum_key = 0;
			OLED_Clear();
			OLED_Show_Str(0,set_value,"→",16);
			OLED_Show_Str(32,0,"系统设置",16);
			OLED_Show_Str(20,3,"重置系统状态",16);
		}
		
	}
	
}


/**
 * @name: System_Confirm
 * @description: 设置确认函数
 * @param {void}
 * @return {void}
 */
void System_Confirm(uint8_t mode)
{

	switch(mode)
	{
	
		case 3: Reset_System_Function();
			break;
		case 6: 
			break;
		case 9: 
			break;
		case 12: 
			break;
		case 15: 
		default:
			break;
	}
}

/**
 * @name: Reset_System_Function
 * @description: 系统重置函数
 * @param {void}
 * @return {void}
 */
void Reset_System_Function(void)
{
	uint8_t res_run_flag = 1;
	uint8_t res_get_key_sum = 0;
	/* Data written to the Flash   */
	uint16_t FlashTest_Res = 11;   
	OLED_Clear();
	OLED_Show_Str(6,1,"是否要重置系统?",16);
	OLED_ShowString(0,4,"yes k2    no k3",16);
	FLASH_ReadMoreData(DEVICE_INFO_ADDRESS,FlashRBuff,sizeof(FlashRBuff));   // 读操作
//	if(FlashRBuff[0] == 11)   		OLED_Show_Str(78,6,"已打开",16);	
//	else if(FlashRBuff[0] == 22)    OLED_Show_Str(78,6,"已关闭",16);	
	while(res_run_flag)
	{
		res_get_key_sum = KEY_Scan(0);
		if(res_get_key_sum == 1){
			
		}else if(res_get_key_sum == 2){
			OLED_Clear();
			OLED_Show_Str(8,3,"系统正在重置中",16);
			Delay_ms(1000);
			Delay_ms(1000);
			Delay_ms(1000);
			FLASH_WriteMoreData(DEVICE_INFO_ADDRESS,&FlashTest_Res,sizeof(FlashTest));   // 写操作   系统重置后默认窗帘状态为打开状态
			OLED_Clear();
			OLED_Show_Str(24,3,"系统已重置",16);
			Delay_ms(1000);
			Delay_ms(1000);
			Delay_ms(1000);
			OLED_Show_Str(0,set_value,"→",16);
			OLED_Show_Str(32,0,"系统设置",16);
			OLED_Show_Str(20,3,"重置窗帘状态",16);
			break;
		}else if(res_get_key_sum == 3){
			res_run_flag = 0;
			OLED_Clear();
			OLED_Clear();
			OLED_Show_Str(0,set_value,"→",16);
			OLED_Show_Str(32,0,"系统设置",16);
			OLED_Show_Str(20,3,"重置窗帘状态",16);
		
		}else if(res_get_key_sum == 4){
			// 退至上一级菜单
			res_run_flag = 0;
			OLED_Clear();
			OLED_Clear();
			OLED_Show_Str(0,set_value,"→",16);
			OLED_Show_Str(32,0,"系统设置",16);
			OLED_Show_Str(20,3,"重置窗帘状态",16);
		}
	}
}


