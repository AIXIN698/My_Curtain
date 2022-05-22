 #include "gpio.h"


void GPIO_X_Init(void)
{
/* key Initialization code begin to */
	
	/* KEY initialize K1 K2 code to */
	
	GPIO_InitTypeDef GPIO_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_Structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Structure.GPIO_Pin = KEY_GPIO_Pin_14|KEY_GPIO_Pin_15;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_GPIOC, &GPIO_Structure);
	
	/* KEY initialize K1 K2 code END */
	
	/* KEY initialize K1 K2 code to */
	
	GPIO_Structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Structure.GPIO_Pin = KEY_GPIO_Pin_0|KEY_GPIO_Pin_1;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_GPIOA, &GPIO_Structure);
	
	/* KEY initialize K1 K2 code END */
		
	/* LED initialize LED_Task code to */
	
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Structure.GPIO_Pin = LED_GPIO_Pin_13;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_GPIOC, &GPIO_Structure);
	/* PC13 LED Initialize the output high level */
	GPIO_SetBits(KEY_GPIOC, LED_GPIO_Pin_13);  
	/* LED initialize LED_Task code END */
	
/* key Initialization code END */
	
}









