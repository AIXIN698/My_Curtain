#include "stm32f10x.h" 
#include "UART1.h"

u8 USARTReceIn;         //�������ݽ��ձ���
u8 USARTReceFullFlag;   //�жϽ��ձ�־λ
u8 USARTReceBuff[8];    //���崮�����ݽ��ն�̬�������
u8 Res;                  //����1�������ݱ���


/**
 * @name: UARE1_Init
 * @description: ����1/�жϳ�ʼ������
 * @param {void}
 * @return {void}
 */
void UARE1_Init(void)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	
}

/**
 * @name: USART1_IRQHandler
 * @description: ����1�жϷ�����
 * @param {void}
 * @return {void}
 */
uint8_t usart_falg = 0;
void USART1_IRQHandler(void)
{
	
	//��������ݴ����ж�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		//���ж�
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);  
		if(usart_falg){
			//��ȡ�����жϽ��յ����ݸ���Res
			Res =USART_ReceiveData(USART1);	
			USART_SendData(USART1, Res);
			switch(USARTReceIn)
			{
				case 0:
					if(Res)   //���Res���յ����ݣ�������ݴ������� USARTReceBuff
						USARTReceBuff[USARTReceIn++] = Res;
					else      //����USARTReceIn ���������ж�
						USARTReceIn = 0;
					break;
				default:
					//���Res���յ����ݣ�������ݴ������� USARTReceBuff
					USARTReceBuff[USARTReceIn++] = Res;
					break;
			}
			//��� USARTReceIn �������ݴ��ڰ˴Σ����ʾһ֡���ݽ�����ɣ����жϽ��ձ�־λ��һ
			if(USARTReceIn >= 4)
			{
				USARTReceFullFlag = 1;	 
			}
		}
		
     }
}

/***************************************************************
 * ��  �ܣ�����1���ݼ�⺯��     USARE1_detection
 * ��  ������
 * ����ֵ����
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

		USARTReceFullFlag = 0;      //�رմ������ݱ�־λ
		USARTReceIn = 0;            //�������ݽ��ձ�������
	}
}

int fputc(int ch, FILE *f)//�ض�����printf���������
{
    USART_SendData(USART1, (uint8_t) ch);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return ch;
}


/*************************************************
 * @��������: USARTx_Send_Byte
 * @��������: ���ڵ����ֽ�
 * @�������: USARTx: ѡ��������ʹ�õĴ��ں�
 *           byte��  Ҫ���͵������׵�ַ
 * @�������: ��
 * @����ֵ  : ��
 * @����˵��: ��
 *************************************************/ 
void USARTx_Send_Byte(unsigned char byte) //���ڷ���һ���ֽ�
{
        USART_SendData(USART1, byte); //ͨ���⺯����������
        while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
        //�ȴ�������ɡ�  ��� USART_FLAG_TC �Ƿ���1��     
}
/*************************************************
 * @��������: UARTx_Send_Str
 * @��������: ���ڷ����ַ���
 * @�������: USARTx: ѡ��������ʹ�õĴ��ں�
 *           *s��    Ҫ���͵������׵�ַָ��
 * @�������: ��
 * @����ֵ  : ��
 * @����˵��: �����ַ�������Ӧ��ָ�뷽��
 *************************************************/ 
void UARTx_Send_Str(unsigned char *s)
{
        unsigned char i=0; //����һ���ֲ��������������ַ���
        while(s[i]!='\0') //ÿ���ַ�����β ������"\0" ��β��
        { 
                 /*ͨ���⺯����������*/
                USART_SendData(USART1,s[i]); 
                 /*�ȴ�������ɡ���� USART_FLAG_TC �Ƿ���1*/
                while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
                i++;   //i++һ��
        }
}
/*************************************************
 * @��������: USART_SendArray
 * @��������: ���ڷ��Ͷ��ֽ�
 * @�������: USARTx: ѡ��������ʹ�õĴ��ں�
 *            buf   ��Ҫ���͵������׵�ַ
 *            len   ��Ҫ���͵����鳤��
 * @�������: ��
 * @����ֵ  : ��
 * @����˵��: ��
 *************************************************/ 
void USART_SendArray(USART_TypeDef* USARTx, u8 *buf, u8 len)
{   
  u8 t;
  for(t=0;t<len;t++)//ѭ����������
    {
        //�ȴ����ͽ���        
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET){}; 
       //��������
        USART_SendData(USARTx,buf[t]); 
    }   
    //�ȴ����ͽ���     
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);      
}





