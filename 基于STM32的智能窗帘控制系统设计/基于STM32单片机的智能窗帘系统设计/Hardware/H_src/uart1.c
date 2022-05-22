#include "stm32f10x.h" 
#include "UART1.h"

u8 USARTReceIn;         //串口数据接收变量
u8 USARTReceFullFlag;   //中断接收标志位
u8 USARTReceBuff[8];    //定义串口数据接收动态数组变量
u8 Res;                  //串口1接收数据变量


/**
 * @name: UARE1_Init
 * @description: 串口1/中断初始化函数
 * @param {void}
 * @return {void}
 */
void UARE1_Init(void)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
	
}

/**
 * @name: USART1_IRQHandler
 * @description: 串口1中断服务函数
 * @param {void}
 * @return {void}
 */
uint8_t usart_falg = 0;
void USART1_IRQHandler(void)
{
	
	//如果有数据触发中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		//清中断
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);  
		if(usart_falg){
			//读取串口中断接收的数据赋给Res
			Res =USART_ReceiveData(USART1);	
			USART_SendData(USART1, Res);
			switch(USARTReceIn)
			{
				case 0:
					if(Res)   //如果Res接收到数据，则把数据传入数组 USARTReceBuff
						USARTReceBuff[USARTReceIn++] = Res;
					else      //否则USARTReceIn 清零重新判断
						USARTReceIn = 0;
					break;
				default:
					//如果Res接收到数据，则把数据传入数组 USARTReceBuff
					USARTReceBuff[USARTReceIn++] = Res;
					break;
			}
			//如果 USARTReceIn 接收数据大于八次，则表示一帧数据接收完成，将中断接收标志位置一
			if(USARTReceIn >= 4)
			{
				USARTReceFullFlag = 1;	 
			}
		}
		
     }
}

/***************************************************************
 * 功  能：串口1数据检测函数     USARE1_detection
 * 参  数：无
 * 返回值：无
 * 
****************************************************************/

void USARE1_detection(void)
{
	
	if(USARTReceFullFlag)
	{
//		if((USARTReceBuff[0] == 0xAA && USARTReceBuff[1] == 0x55 && USARTReceBuff[6] == 0x55 && USARTReceBuff[7] == 0xAA)){
//		
//			if((USARTReceBuff[4] == 0x00 && USARTReceBuff[5] == 0x01)){
//				/* Start motor positive turn signal position */
//				CW_Flag = 1;
//				REV_Flag = 0;
//			}
//			else if((USARTReceBuff[4] == 0x00 && USARTReceBuff[5] == 0x02)){
//				/* Start motor reverse signal bit */
//				REV_Flag = 1;
//				CW_Flag = 0;
//			}
//		}
		if((USARTReceBuff[0] == 0x20 && USARTReceBuff[1] == 0x22)){
		
			if((USARTReceBuff[2] == 0x66 && USARTReceBuff[3] == 0x41)){			
				/* Start motor positive turn signal position */
				//CW_Flag = 1;
				//REV_Flag = 0;
			}else if(USARTReceBuff[2] == 0x66 && USARTReceBuff[3] == 0x42){
				/* Start motor reverse signal bit */
				//REV_Flag = 1;
				//CW_Flag = 0;
			}
		}
		//for(uint8_t i = 0; i < 4; i++)   USARTReceBuff[i] = 0x00;

		USARTReceFullFlag = 0;      //关闭串口数据标志位
		USARTReceIn = 0;            //串口数据接收变量清零
	}
}

int fputc(int ch, FILE *f)//重定向，让printf输出到串口
{
    USART_SendData(USART1, (uint8_t) ch);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return ch;
}


/*************************************************
 * @函数名称: USARTx_Send_Byte
 * @函数功能: 串口单个字节
 * @输入参数: USARTx: 选择发送数组使用的串口号
 *           byte：  要发送的数组首地址
 * @输出参数: 无
 * @返回值  : 无
 * @其它说明: 无
 *************************************************/ 
void USARTx_Send_Byte(unsigned char byte) //串口发送一个字节
{
        USART_SendData(USART1, byte); //通过库函数发送数据
        while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
        //等待发送完成。  检测 USART_FLAG_TC 是否置1；     
}
/*************************************************
 * @函数名称: UARTx_Send_Str
 * @函数功能: 串口发送字符串
 * @输入参数: USARTx: 选择发送数组使用的串口号
 *           *s：    要发送的数组首地址指针
 * @输出参数: 无
 * @返回值  : 无
 * @其它说明: 发送字符串函数应用指针方法
 *************************************************/ 
void UARTx_Send_Str(unsigned char *s)
{
        unsigned char i=0; //定义一个局部变量用来发送字符串
        while(s[i]!='\0') //每个字符串结尾 都是以"\0" 结尾的
        { 
                 /*通过库函数发送数据*/
                USART_SendData(USART1,s[i]); 
                 /*等待发送完成。检测 USART_FLAG_TC 是否置1*/
                while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
                i++;   //i++一次
        }
}
/*************************************************
 * @函数名称: USART_SendArray
 * @函数功能: 串口发送多字节
 * @输入参数: USARTx: 选择发送数组使用的串口号
 *            buf   ：要发送的数组首地址
 *            len   ：要发送的数组长度
 * @输出参数: 无
 * @返回值  : 无
 * @其它说明: 无
 *************************************************/ 
void USART_SendArray(USART_TypeDef* USARTx, u8 *buf, u8 len)
{   
  u8 t;
  for(t=0;t<len;t++)//循环发送数据
    {
        //等待发送结束        
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET){}; 
       //发送数据
        USART_SendData(USARTx,buf[t]); 
    }   
    //等待发送结束     
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);      
}





