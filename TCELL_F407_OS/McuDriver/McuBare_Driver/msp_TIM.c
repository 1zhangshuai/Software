#include "msp_TIM.h"
#include "msp_GPIO.h"

/**** TIM PWM(�����ȵ���) ****/
/*MultiPwm*/
void msp_TimMultiPwmOut_Init(TimMultiPwmOut *timPwmOut)
{
	GPIO_InitTypeDef 	    GPIO_InitStruct	;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef       TIM_OCInitStruct;	
	
	/*GPIO & TIM Channel config*/
	if (timPwmOut->CH1_Pin != 0)
	{
		/*GPIO config*/
		GPIO_InitStruct.GPIO_Pin   = timPwmOut->CH1_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;	
		GPIO_Init(timPwmOut->CH1_GPIO, &GPIO_InitStruct);
		
		/*AF*/
		GPIO_PinAFConfig(timPwmOut->CH1_GPIO, timPwmOut->CH1_PinSource, timPwmOut->GPIO_AF);
		
		/*Tim Channle config*/
		TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;   /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2*/
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
		TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High; /*TIM����Ƚϼ��Ը�*/
		TIM_OCInitStruct.TIM_Pulse       = 0;					  /*��ʼ��0*/			
		TIM_OC1Init(timPwmOut->Tim, &TIM_OCInitStruct); /*����Tָ���Ĳ�����ʼ������TIMxOC1*/
		
		/*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/
		TIM_OC1PreloadConfig(timPwmOut->Tim, TIM_OCPreload_Enable);		
	}
	
	if (timPwmOut->CH2_Pin != 0)
	{
		/*GPIO config*/
		GPIO_InitStruct.GPIO_Pin   = timPwmOut->CH2_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;	
		GPIO_Init(timPwmOut->CH2_GPIO, &GPIO_InitStruct);
		
		/*AF*/
		GPIO_PinAFConfig(timPwmOut->CH2_GPIO, timPwmOut->CH2_PinSource, timPwmOut->GPIO_AF);
		
		/*Tim Channle config*/
		TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;   /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2*/
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
		TIM_OCInitStruct.TIM_Pulse       = 0;					  /*��ʼ��0*/		
		TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High; /*TIM����Ƚϼ��Ը�*/
		TIM_OC2Init(timPwmOut->Tim, &TIM_OCInitStruct); /*����Tָ���Ĳ�����ʼ������TIMxOC2*/
		
		/*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/
		TIM_OC2PreloadConfig(timPwmOut->Tim, TIM_OCPreload_Enable);		
	}

	if (timPwmOut->CH3_Pin != 0)
	{
		/*GPIO config*/
		GPIO_InitStruct.GPIO_Pin   = timPwmOut->CH3_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;	
		GPIO_Init(timPwmOut->CH3_GPIO, &GPIO_InitStruct);
		
		/*AF*/
		GPIO_PinAFConfig(timPwmOut->CH3_GPIO, timPwmOut->CH3_PinSource, timPwmOut->GPIO_AF);
		
		/*Tim Channle config*/
		TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;   /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2*/
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
		TIM_OCInitStruct.TIM_Pulse       = 0;					  /*��ʼ��0*/		
		TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High; /*TIM����Ƚϼ��Ը�*/
		TIM_OC3Init(timPwmOut->Tim, &TIM_OCInitStruct); /*����Tָ���Ĳ�����ʼ������TIMxOC3*/
		
		/*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/
		TIM_OC3PreloadConfig(timPwmOut->Tim, TIM_OCPreload_Enable);			
	}

	if (timPwmOut->CH4_Pin != 0)
	{
		/*GPIO config*/
		GPIO_InitStruct.GPIO_Pin   = timPwmOut->CH4_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;	
		GPIO_Init(timPwmOut->CH4_GPIO, &GPIO_InitStruct);
		
		/*AF*/
		GPIO_PinAFConfig(timPwmOut->CH4_GPIO, timPwmOut->CH4_PinSource, timPwmOut->GPIO_AF);
		
		/*Tim Channle config*/
		TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;   /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2*/
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
		TIM_OCInitStruct.TIM_Pulse       = 0;					  /*��ʼ��0*/
		TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High; /*TIM����Ƚϼ��Ը�*/
		TIM_OC4Init(timPwmOut->Tim, &TIM_OCInitStruct); /*����Tָ���Ĳ�����ʼ������TIMxOC3*/
		
		/*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/
		TIM_OC4PreloadConfig(timPwmOut->Tim, TIM_OCPreload_Enable);			
	}	
	
	/*Tim config*/
	TIM_TimeBaseInitStruct.TIM_Period            = timPwmOut->Period;		/*�趨�������Զ���װֵ*/
	TIM_TimeBaseInitStruct.TIM_Prescaler         = timPwmOut->Prescaler;	/*Ԥ��Ƶ��*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision     = timPwmOut->ClockDivision;/*��ʱ�ӷָ�:TDTS = Tck_tim*/
	TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;		/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;				        /*�ظ��Ƚϴ��������¼�*/
	TIM_TimeBaseInit(timPwmOut->Tim, &TIM_TimeBaseInitStruct);
	
	/*ʹ��TIMx_CHx��CCR2�ϵ�Ԥװ�ؼĴ���*/
	TIM_ARRPreloadConfig(timPwmOut->Tim, ENABLE);
	
	/*ʹ�ܶ�ʱ��*/
	TIM_Cmd(timPwmOut->Tim, ENABLE);

	/*��ʼ������ʱ�����pwm,����Ǹ߼���ʱ�����е�,���pwm�����*/
//	TIM_CtrlPWMOutputs(timPwmOut->Tim, ENABLE);
	
	/*��ʼ��PWM���������,�����Ч�г���Сֵ(ͣתֵ)*/
	msp_TimMultiPwmOut_SetPluse(timPwmOut, MSP_TIM_CH1, ESC_MIN_PULSE_ZERO_SPEED_VALUE);
	msp_TimMultiPwmOut_SetPluse(timPwmOut, MSP_TIM_CH2, ESC_MIN_PULSE_ZERO_SPEED_VALUE);
	msp_TimMultiPwmOut_SetPluse(timPwmOut, MSP_TIM_CH3, ESC_MIN_PULSE_ZERO_SPEED_VALUE);
	msp_TimMultiPwmOut_SetPluse(timPwmOut, MSP_TIM_CH4, ESC_MIN_PULSE_ZERO_SPEED_VALUE);	
}

/*PWMʩ��*/
void msp_TimMultiPwmOut_SetPluse(TimMultiPwmOut *timPwmOut, MSP_TIM_CHANNLE TIM_CHANNLE, u16 Pulse)
{
	switch(TIM_CHANNLE)
	{
		case MSP_TIM_CH1:
		{
			TIM_SetCompare1(timPwmOut->Tim, Pulse);
		}break;
		
		case MSP_TIM_CH2:
		{
			TIM_SetCompare2(timPwmOut->Tim, Pulse);		
		}break;

		case MSP_TIM_CH3:
		{
			TIM_SetCompare3(timPwmOut->Tim, Pulse);		
		}break;
		
		case MSP_TIM_CH4:
		{
			TIM_SetCompare4(timPwmOut->Tim, Pulse);		
		}break;

		default:break;
	}
}

/*SinglePwm*/
void msp_TimSinglePwmOut_Init(TimSinglePwmOut *timPwmOut)
{
	GPIO_InitTypeDef 	    GPIO_InitStruct	;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef       TIM_OCInitStruct;	
	
	/*GPIO & TIM Channel config*/
	/*GPIO config*/
	GPIO_InitStruct.GPIO_Pin   = timPwmOut->Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_Init(timPwmOut->GPIO, &GPIO_InitStruct);
		
	/*AF*/
	GPIO_PinAFConfig(timPwmOut->GPIO, timPwmOut->PinSource, timPwmOut->GPIO_AF);
		
	/*Tim Channle config*/
	TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;   /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2*/
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
	TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High; /*TIM����Ƚϼ��Ը�*/
	TIM_OCInitStruct.TIM_Pulse       = 0;					  /*��ʼ��0*/			
	TIM_OC1Init(timPwmOut->Tim, &TIM_OCInitStruct); /*����Tָ���Ĳ�����ʼ������TIMxOC1*/
		
	/*ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���*/
	TIM_OC1PreloadConfig(timPwmOut->Tim, TIM_OCPreload_Enable);		
	
	/*Tim config*/
	TIM_TimeBaseInitStruct.TIM_Period            = timPwmOut->Period;		/*�趨�������Զ���װֵ*/
	TIM_TimeBaseInitStruct.TIM_Prescaler         = timPwmOut->Prescaler;	/*Ԥ��Ƶ��*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision     = timPwmOut->ClockDivision;/*��ʱ�ӷָ�:TDTS = Tck_tim*/
	TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;		/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;				        /*�ظ��Ƚϴ��������¼�*/
	TIM_TimeBaseInit(timPwmOut->Tim, &TIM_TimeBaseInitStruct);
	
	/*ʹ��TIMx_CHx��CCR2�ϵ�Ԥװ�ؼĴ���*/
	TIM_ARRPreloadConfig(timPwmOut->Tim, ENABLE);
	
	/*ʹ�ܶ�ʱ��*/
	TIM_Cmd(timPwmOut->Tim, ENABLE);

	/*��ʼ������ʱ�����pwm,����Ǹ߼���ʱ�����е�,���pwm�����*/
//	TIM_CtrlPWMOutputs(timPwmOut->Tim, ENABLE);
	
	/*��ʼ��PWM���������,�����Ч�г���Сֵ(ͣתֵ)*/
	msp_TimSinglePwmOut_SetPluse(timPwmOut, timPwmOut->CHANNLE, 0);
}

void msp_TimSinglePwmOut_SetPluse(TimSinglePwmOut *timPwmOut, MSP_TIM_CHANNLE TIM_CHANNLE, u16 Pulse)
{
	switch(TIM_CHANNLE)
	{
		case MSP_TIM_CH1:
		{
			TIM_SetCompare1(timPwmOut->Tim, Pulse);
		}break;
		
		case MSP_TIM_CH2:
		{
			TIM_SetCompare2(timPwmOut->Tim, Pulse);		
		}break;

		case MSP_TIM_CH3:
		{
			TIM_SetCompare3(timPwmOut->Tim, Pulse);		
		}break;
		
		case MSP_TIM_CH4:
		{
			TIM_SetCompare4(timPwmOut->Tim, Pulse);		
		}break;

		default:break;
	}
}
/*******************************/

/*=== PWM_In���벶�� ===*/
void msp_TimPwmIn_Init(TimPwmIn *timPwmIn)
{
	GPIO_InitTypeDef 	    GPIO_InitStruct	;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef       TIM_ICInitStruct;
	
	/*GPIO & TIM Channel config*/
	if (timPwmIn->CH1_Pin != 0)
	{
		/*GPIO config*/
		GPIO_InitStruct.GPIO_Pin   = timPwmIn->CH1_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;	
		GPIO_Init(timPwmIn->CH1_GPIO, &GPIO_InitStruct);
		
		/*AF*/
		GPIO_PinAFConfig(timPwmIn->CH1_GPIO, timPwmIn->CH1_PinSource, timPwmIn->GPIO_AF);
		
		/*Tim Channle config*/
		TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1;			 /*ѡ������� IC1ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising;	 /*�����ز���*/
		TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; /*ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 /*���������Ƶ,����Ƶ*/
		TIM_ICInitStruct.TIM_ICFilter    = 0x00;				  	 /*���������˲��� ���˲�*/
		TIM_ICInit(timPwmIn->Tim, &TIM_ICInitStruct);
		
		/*��������ж�,����CC1IE�����ж�*/
		TIM_ITConfig(timPwmIn->Tim, TIM_IT_Update | TIM_IT_CC1, ENABLE);		
	}
	
	if (timPwmIn->CH2_Pin != 0)
	{
		/*GPIO config*/		
		GPIO_InitStruct.GPIO_Pin  = timPwmIn->CH2_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;			
		GPIO_Init(timPwmIn->CH2_GPIO, &GPIO_InitStruct);

		/*AF*/
		GPIO_PinAFConfig(timPwmIn->CH2_GPIO, timPwmIn->CH2_PinSource, timPwmIn->GPIO_AF);		

		/*Tim Channle config*/
		TIM_ICInitStruct.TIM_Channel     = TIM_Channel_2;			 /*ѡ������� IC1ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising;	 /*�����ز���*/
		TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; /*ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 /*���������Ƶ,����Ƶ*/
		TIM_ICInitStruct.TIM_ICFilter    = 0x00;				  	 /*���������˲��� ���˲�*/
		TIM_ICInit(timPwmIn->Tim, &TIM_ICInitStruct);	

		/*��������ж�,����CC1IE�����ж�*/
		TIM_ITConfig(timPwmIn->Tim, TIM_IT_Update | TIM_IT_CC2, ENABLE);
	}

	if (timPwmIn->CH3_Pin != 0)
	{
		/*GPIO config*/		
		GPIO_InitStruct.GPIO_Pin  = timPwmIn->CH3_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;			
		GPIO_Init(timPwmIn->CH3_GPIO, &GPIO_InitStruct);

		/*AF*/
		GPIO_PinAFConfig(timPwmIn->CH3_GPIO, timPwmIn->CH3_PinSource, timPwmIn->GPIO_AF);		

		/*Tim Channle config*/
		TIM_ICInitStruct.TIM_Channel     = TIM_Channel_3;			 /*ѡ������� IC1ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising;	 /*�����ز���*/
		TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; /*ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 /*���������Ƶ,����Ƶ*/
		TIM_ICInitStruct.TIM_ICFilter    = 0x00;				  	 /*���������˲��� ���˲�*/
		TIM_ICInit(timPwmIn->Tim, &TIM_ICInitStruct);		

		/*��������ж�,����CC1IE�����ж�*/
		TIM_ITConfig(timPwmIn->Tim, TIM_IT_Update | TIM_IT_CC3, ENABLE);		
	}

	if (timPwmIn->CH4_Pin != 0)
	{
		/*GPIO config*/		
		GPIO_InitStruct.GPIO_Pin   = timPwmIn->CH4_Pin;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;		
		GPIO_Init(timPwmIn->CH4_GPIO, &GPIO_InitStruct);

		/*AF*/
		GPIO_PinAFConfig(timPwmIn->CH4_GPIO, timPwmIn->CH4_PinSource, timPwmIn->GPIO_AF);		

		/*Tim Channle config*/
		TIM_ICInitStruct.TIM_Channel     = TIM_Channel_4;			 /*ѡ������� IC1ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising;	 /*�����ز���*/
		TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; /*ӳ�䵽TI1��*/
		TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 /*���������Ƶ,����Ƶ*/
		TIM_ICInitStruct.TIM_ICFilter    = 0x00;				  	 /*���������˲��� ���˲�*/
		TIM_ICInit(timPwmIn->Tim, &TIM_ICInitStruct);
		
		/*��������ж�,����CC1IE�����ж�*/
		TIM_ITConfig(timPwmIn->Tim, TIM_IT_Update | TIM_IT_CC4, ENABLE);		
	}	
	
	/*Tim config*/
	TIM_TimeBaseInitStruct.TIM_Period        = timPwmIn->Period;		/*�趨�������Զ���װֵ*/
	TIM_TimeBaseInitStruct.TIM_Prescaler     = timPwmIn->Prescaler;		/*Ԥ��Ƶ��*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision = timPwmIn->ClockDivision;	/*��ʱ�ӷָ�:TDTS = Tck_tim*/
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;		/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseInit(timPwmIn->Tim, &TIM_TimeBaseInitStruct);
	
	/*ʹ�ܶ�ʱ��*/
	TIM_Cmd(timPwmIn->Tim, ENABLE);
}

/*=== TIM ALARM(��ʱ��) ===*/
void msp_TimAlarm_Init(TimAlarm *timAlarm)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/*TIM��ʱ����������*/
	TIM_TimeBaseStructure.TIM_Prescaler     = timAlarm->Prescaler;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = timAlarm->Period;
	TIM_TimeBaseStructure.TIM_ClockDivision = timAlarm->ClockDivision;
	TIM_TimeBaseInit(timAlarm->Tim, &TIM_TimeBaseStructure);
	
	/*����жϸ��±�־λ*/
	TIM_ClearFlag(timAlarm->Tim, TIM_FLAG_Update);

	/*ʹ��ָ����TIM�ж�,��������ж�*/
	TIM_ITConfig(timAlarm->Tim, TIM_IT_Update, ENABLE );  
	
	/*ʹ��TIM*/
	TIM_Cmd(timAlarm->Tim, ENABLE);
}
/*******************************/

/***** TIM ENCODER(������) *****/
void msp_TimEncoder_Init(MSP_TIM_RESOURCE mspTimResource)
{	
//	GPIO_InitTypeDef         GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef        TIM_ICInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
//	
//	/***** MOTOR B ENCODER *****/
//	GPIO_InitStruct.GPIO_Pin   = MSP_ENCODER_HB_1PIN | MSP_ENCODER_HB_2PIN;
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	/*Tim Base Init*/
//	TIM_TimeBaseStructure.TIM_Prescaler     = MSP_TIM2_ENCODER_PRESCALER;
//	TIM_TimeBaseStructure.TIM_Period        = MSP_TIM2_ENCODER_PERIOD;  
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  
//	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; 
//	TIM_TimeBaseInit(MSP_TIM2_ENCODER_TIMX, &TIM_TimeBaseStructure);
//	
//	/*Tim ������:TIM_ICPolarity_Rising�����ز���*/
//	TIM_EncoderInterfaceConfig(MSP_TIM2_ENCODER_TIMX, TIM_EncoderMode_TI12, \
//						       TIM_ICPolarity_Falling, TIM_ICPolarity_Falling); 
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = MSP_TIM2_ENCODER_ICFILTER; /*���˲���*/
//	TIM_ICInit(MSP_TIM2_ENCODER_TIMX, &TIM_ICInitStructure);

//	/***** MOTOR A ENCODER *****/
//	GPIO_InitStruct.GPIO_Pin   = MSP_ENCODER_HA_1PIN | MSP_ENCODER_HA_2PIN;
//	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
// 
//	/*Tim Base Init*/
//	TIM_TimeBaseStructure.TIM_Prescaler     = MSP_TIM4_ENCODER_PRESCALER;
//	TIM_TimeBaseStructure.TIM_Period        = MSP_TIM4_ENCODER_PERIOD;  
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶϵ��������Ƶ
//	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //���ϼ���ģʽ 
//	TIM_TimeBaseInit(MSP_TIM4_ENCODER_TIMX, &TIM_TimeBaseStructure);
//	
//	/*Tim ������:TIM_ICPolarity_Rising�����ز���*/
//	TIM_EncoderInterfaceConfig(MSP_TIM4_ENCODER_TIMX, TIM_EncoderMode_TI12, \
//							   TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = MSP_TIM4_ENCODER_ICFILTER; //���˲���
//	TIM_ICInit(MSP_TIM4_ENCODER_TIMX, &TIM_ICInitStructure);	
//	
//	/*�����ʱ��TIM2/4�жϱ�־λ*/
//	TIM_ClearFlag(MSP_TIM2_ENCODER_TIMX, TIM_FLAG_Update);
//	TIM_ClearFlag(MSP_TIM4_ENCODER_TIMX, TIM_FLAG_Update);
//	
//	/*ʹ�ܼ�����TIM2/TIM4*/
//	TIM_Cmd(MSP_TIM2_ENCODER_TIMX, ENABLE);
//	TIM_Cmd(MSP_TIM4_ENCODER_TIMX, ENABLE);	
}

MSP_EncoderNbr* msp_TimEncoder_GetEncoderNbr(MSP_TIM_RESOURCE mspTimResource)
{
	MSP_EncoderNbr *psEncoderNbr;
//	
//	psEncoderNbr->TargANbr = TIM_GetCounter(MSP_TIM2_ENCODER_TIMX);
//	psEncoderNbr->TargBNbr = TIM_GetCounter(MSP_TIM4_ENCODER_TIMX);
//	
//	TIM_SetCounter(MSP_TIM2_ENCODER_TIMX, 0);
//	TIM_SetCounter(MSP_TIM4_ENCODER_TIMX, 0);	
	
	return psEncoderNbr;
}
/*******************************/
