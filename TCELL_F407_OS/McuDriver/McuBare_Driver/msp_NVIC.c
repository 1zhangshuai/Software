#include "msp_NVIC.h"
#include "sys_McuInit.h"

void msp_NVIC_Init(MSP_NVIC_GROUP mspNvicGroup)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(mspNvicGroup);		/*�������2*/
		
	/*Systick��ʱ��*/
#ifndef PLATFORM_RTOS__RT_THREAD
	NVIC_SetPriority(SysTick_IRQn, 0);
#endif

	/*ExecutePeriod����ִ�е�ʱ��ϵͳ��ʱ��*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimExecutePeriod.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;   /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	/*TIM3��ʱ��PWM���*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimMultiPwmOut.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;   /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	#if defined(REMOTE_DATA_RECV__PWM)	
	/*TIM4��ʱ��PWM���벶��(ң�����ݽ���)*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimPwmIn_Attitude.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;   /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	/*TIM1��ʱ��PWM���벶��(ң�����ݽ���)*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimPwmIn_Switch.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;   /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
	
	#if defined(REMOTE_DATA_RECV__PPM)
	/*�ⲿ�ж�(ң�����ݽں�PPM)*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sPPMRecvExti.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;   /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	#endif
	
	/*USER ���ڿ��� �ж�(RX)*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sDebugUart.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;    /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	/*GPS ���ڿ��� �ж�(RX)*/
#if defined(HW_CUT__USE_GPS)
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sGpsUart.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;    /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
#endif	
	NVIC_Init(&NVIC_InitStructure);		

	/*ULTR ���ڿ��� �ж�(RX)*/
#if defined(HW_CUT__USE_ULTR)	
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sUltrUart.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;    /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
#endif	

	/*OPTICALFLOW ���ڿ��� �ж�(RX)*/
#if defined(HW_CUT__USE_OPTICFLOW)	
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sOpticalFlowUart.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    /*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;    /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
#endif	
	
	/*Task_Main���ȶ�ʱ��*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimAlarmTaskMain.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	/*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;	/*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*Task_Assist���ȶ�ʱ��*/
	NVIC_InitStructure.NVIC_IRQChannel                   = g_sTimAlarmTaskAssist.NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	/*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;	/*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
