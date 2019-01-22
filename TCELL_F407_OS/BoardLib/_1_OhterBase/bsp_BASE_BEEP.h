#ifndef _BSP_BASE_BEEP_H_
#define _BSP_BASE_BEEP_H_

#include "sys_Platform.h"
#include "sys_BoardMap.h"
#include "sys_McuInit.h"
#include "msp_TIM.h"

#define BEEP_LOUDNESS_LOW_DUTY_CYCLE		(1/4)
#define BEEP_LOUDNESS_MID_DUTY_CYCLE		(2/4)
#define BEEP_LOUDNESS_HIGH_DUTY_CYCLE		(3/4)

/*����ǿ��*/
typedef enum
{
	BEEP_LOUDNESS_NULL = 0, /*��*/
	BEEP_LOUDNESS_LOW  = 1, /*����*/
	BEEP_LOUDNESS_MID  = 2, /*����*/
	BEEP_LOUDNESS_HIGH = 3, /*����*/
}BEEP_LOUDNESS_TYPE;

/*������ʾ��ʽ*/
typedef enum
{
	BEEP_CIRCLE_NULL    = 0, /*��*/
	BEEP_CIRCLE_ONCE    = 1, /*һ��*/
	BEEP_CIRCLE_FOREVER = 2,  /*��Զ*/
}BEEP_CIRCLE_TYPE;	

typedef struct
{
	/*������ʾ*/
	BEEP_LOUDNESS_TYPE    LOUDNESS_TYPE; 	/*����ǿ��*/
	BEEP_CIRCLE_TYPE 	  CIRCLE_TYPE;  	/*ѭ����ʽ*/
	u32			     	  conTick5MS;       /*����ʱ��_5ms*/
}Beep_Work;

typedef struct
{
	TimSinglePwmOut *SinglePWM;
	Beep_Work		work;
}BSP_BEEP;

/*��ʼ��*/
SYS_RETSTATUS bsp_BEEP_Init(BSP_BEEP *beep);

extern BSP_BEEP g_sBeep;

#endif
