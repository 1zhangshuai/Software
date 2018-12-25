#ifndef _CONTROL_ALTHOLD_H_
#define _CONTROL_ALTHOLD_H_

#include "sys_Platform.h"
#include "control_Config.h"

/*�����ƶ�����λ������ϵ*/
typedef enum
{
	/*��������״̬*/
	CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD	  = 0,  /*����λ����λ����*/
	CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD   = 1,	/*����λ�������Ϸ�*/
	CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD = 2,	/*����λ�������·�*/
	
	/*��ϱ��κ��ϴε�״̬*/
	CONTROL_THROTTLE_PUSH_DOWM_TO_MIDDEAD     = 3,  /*�����Ƶ���λ����*/
	CONTROL_THROTTLE_PUSH_DOWM_OVER_MIDDEAD   = 4,  /*�����ƹ���λ����*/
	CONTROL_THROTTLE_PUSH_UP_TO_MIDDEAD	      = 5,  /*�����Ƶ���λ����*/
	CONTROL_THROTTLE_PUSH_UP_OVER_MIDDEAD     = 6,  /*�����ƹ���λ����*/
	CONTROL_THROTTLE_PUSH_UPDOWN_DEADBAND     = 7,  /*������λ����λ����*/
	CONTROL_THROTTLE_PUSH_UPDOWN_UPBAND       = 8,  /*������λ����λ��*/
	CONTROL_THROTTLE_PUSH_UPDOWN_DOWMBAND     = 9,  /*������λ����λ��*/	
}CONTROL_THROTTLE_PUSH;

/*�����ƶ�����λ������ϵ*/
typedef struct
{
	volatile CONTROL_THROTTLE_PUSH LAST_STATUS;	/*�ϴ������������״̬*/
	volatile CONTROL_THROTTLE_PUSH CUR_STATUS;	/*���������������״̬*/
	volatile CONTROL_THROTTLE_PUSH REAL_STATUS;	/*��ʵ�����������״̬(����ϴκͱ�������״̬)*/
}ControlThrottlePush;

/*��ȡ���������λ�����ƶ����*/
CONTROL_THROTTLE_PUSH altitude_Get_Throttle_Relative_MidDead(ControlThrottlePush *controlThrottlePush);

/*��ֱ�߶�(����)������*/
void vertical_Control_AltHold(fp32 controlDeltaT);


extern fp32 g_HeightYawSpeedFeedforwardOutput;   /*��ֱ�ٶ�ǰ�����������*/
extern fp32 g_HeightYawSpeedFeedforwardRate;   	 /*��ֱ�ٶ�ǰ��������,APM����Ϊ1��0.45*/
extern fp32 g_HeightYawSpeedFeedforwardDelta;    /*��ֱ�����ٶȱ仯��*/
extern fp32 g_CurHeightYawSpeedExpect;   		 /*������ֱ�ٶ�����*/
extern fp32 g_LastHeightYawSpeedExpect;			 /*�ϴ���ֱ�ٶ�����*/

#endif
