#include "msp_EXTI.h"

/*�ⲿ�жϳ�ʼ��*/
void msp_EXTI_Init(MSP_Exti *exti)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	/*GPIO��ʼ��*/
	GPIO_InitStruct.GPIO_Pin = exti->Gpio.GPIO_Pin;
	/*�����ش���*/
	if (exti->EXTI_Trigger == EXTI_Trigger_Rising)
	{
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; /*����*/
	}
	/*�½��ش���*/
	else if (exti->EXTI_Trigger == EXTI_Trigger_Falling)
	{
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   /*����*/	
	}
	/*���±��ش���*/
	else if (exti->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
	{
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	/*����ģʽ*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_Init(exti->Gpio.GPIO, &GPIO_InitStruct);
	
	/*IO�ĸ��ù��ܱ���ʹ��*/
	SYSCFG_EXTILineConfig(exti->EXTI_PortSource, exti->EXTI_PinSource);
	
	/*�ⲿ�жϳ�ʼ��*/
	EXTI_InitStruct.EXTI_Line    = exti->EXTI_Line;
	EXTI_InitStruct.EXTI_Mode    = exti->EXTI_Mode;
	EXTI_InitStruct.EXTI_Trigger = exti->EXTI_Trigger;	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}
