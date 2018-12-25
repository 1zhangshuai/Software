#ifndef _CONTROL_AIRCRAFT_H_
#define _CONTROL_AIRCRAFT_H_

#include "sys_Platform.h"
#include "math_Function.h"
#include "bsp_BoardLib.h"
#include "attitude_Aircraft.h"
#include "remot_DataAnaly.h"
#include "status_Aircraft.h"
#include "ahrs_Caculation.h"
#include "sins_Strapdown.h"
#include "bsp_BoardLib.h"

/*����/����/����(����)*/
#include "control_Config.h"

/*����ϵͳ������ȡ���*/
typedef enum
{
	CTRL_SYSTEM_PARA_INIT_SUCC = 0, /*��ʼ���ɹ�*/
	CTRL_SYSTEM_PARA_INIT_FAIL = 1, /*��ʼ��ʧ��*/
}CTRL_SYSTEM_PARA_INIT_STATUS;

/*�����㷨��ʼ��״̬*/
typedef struct
{
	/*PID�㷨*/
	volatile CTRL_SYSTEM_PARA_INIT_STATUS pid;
	
	/*ADRC�㷨*/
	volatile CTRL_SYSTEM_PARA_INIT_STATUS adrc;
}CTRL_SysStatus;

/*���������й�����,�˶�����*/
typedef enum
{
	/*��ֱ����*/
	CTRL_AIRCRAFT_MOVE_VER_UP    = 0, /*��ֱ�����˶�*/
	CTRL_AIRCRAFT_MOVE_VER_HOLD  = 1, /*��ֱ��������*/
	CTRL_AIRCRAFT_MOVE_VER_DOWN  = 2, /*��ֱ�����˶�*/
	
	/*ˮƽ����*/	
	CTRL_AIRCRAFT_MOVE_HOR_FRONT = 3, /*ˮƽ��ǰ�˶�*/
	CTRL_AIRCRAFT_MOVE_HOR_BACK  = 4, /*ˮƽ����˶�*/
	CTRL_AIRCRAFT_MOVE_HOR_LEFT  = 5, /*ˮƽ�����˶�*/	
	CTRL_AIRCRAFT_MOVE_HOR_RIGHT = 6, /*ˮƽ�����˶�*/	
	CTRL_AIRCRAFT_MOVE_HOR_HOLD  = 7, /*ˮƽ��������*/
}CTRL_AIRCRAFT_MOVE_TREND;

/*�Զ��������ܿ���*/
typedef enum
{
	CTRL_AIRCRAFT_GO_HOME_DISABLE = 0, /*�������Զ�����*/
	CTRL_AIRCRAFT_GO_HOME_ENABLE  = 1, /*�����Զ�����*/
}CTRL_AIRCRAFT_GO_HOME_STATUS;

/*�Զ���������״̬*/
typedef enum
{
	CTRL_AIRCRAFT_GO_HOME_SET    = 1, /*�Զ�����������*/
	CTRL_AIRCRAFT_GO_HOME_NOTSET = 0, /*�Զ�����δ����*/
}CTRL_AIRCRAFT_GO_HOME_SWITCH;

/*���PWM���ֵ*/
typedef struct
{
	u16 channle1;
	u16 channle2;	
	u16 channle3;	
	u16 channle4;
	u16 channle5;
	u16 channle6;	
	u16 channle7;	
	u16 channle8;	
}MotorPWMOutput;

/*�������*/
typedef struct
{
	/*������״̬*/
    u16 			        	 	      heightHoldThrottle;       /*�߶ȱ�������(���ڶ���)*/
	u16					     		      throttleUpDownJudgeValue;	/*���Ÿ����ƻ��������ж���*/
	u16					     		      ctrlThrottle;				/*��������(��ң����������,��PWM�������)*/	
	u16					     		      lastCtrlThrottle;			/*�ϴ�����(��ң����������,��PWM�������)*/	
	u16					     		      throttleOutput;			/*�����������*/
	Remot_Expect_Angle     		          RemotExpectAngle;			/*ң��ֵת�����Ƕ�ֵ*/
	ImuAttitude			     		      RemotExpectAutoAngle;		/*ң���������ȸ�����,�����*/
	GimbalAngle              		  	  GimbalExpectAngle;		/*�������̨�����Ƕ�*/
	volatile CTRL_AIRCRAFT_MOVE_TREND 	  AIRCRAFT_VER_MOVE_TREND;	/*��������ֱ�����˶�����*/
	volatile CTRL_AIRCRAFT_MOVE_TREND     AIRCRAFT_HOR_MOVE_TREND;	/*������ˮƽ�����˶�����*/
	volatile CTRL_AIRCRAFT_GO_HOME_STATUS GO_HOME_STATUS;			/*�Զ�����ʹ��״̬*/
	volatile CTRL_AIRCRAFT_GO_HOME_SWITCH GO_HOME_SET;				/*�Զ���������״̬*/
	
	/*���PWM���*/
	MotorPWMOutput		 	 		      CurMotorPwmOutput;		/*��ǰ���PWM���*/
	MotorPWMOutput	     	 		      LastMotorPwmOutput;		/*�ϴε��PWM���*/
}ControlAircraft;


/*�����㷨������ʼ��*/
SYS_RETERR ctrl_autocontrol_para_init(CTRL_SysStatus *sysStatus);

/*�������Զ�����ϵͳ*/
void ctrl_auto_control_system_dp(void);

/*�߶�,ˮƽ����ģʽѡ��*/
void ctrl_Control_Mode_Select(AircraftStatus *aircraftStatus);

/*��̬������: �Ƕ��⻷+���ٶ��ڻ�*/
void ctrl_Attitude_Control_Dp(void);
/*�Ƕ��⻷����*/
void ctrl_Attitude_Angle_Control_Dp(void);
/*���ٶ��ڻ�����*/
void ctrl_Attitude_Gyro_Control_Dp(void);

/*����������: ����+����&����*/
void ctrl_MainLeading_Control_Dp(void);

/*������ǲ���*/
void ctrl_Throttle_DipAngle_Compensate(void);

/*�Կ�ϵͳ���������*/
void ctrl_auto_control_system_output(void);

extern CTRL_SysStatus g_sCtrlSysStatus;
extern CTRL_SysStatus *g_psCtrlSysStatus;

extern ControlAircraft g_sControlAircraft;
extern ControlAircraft *g_psControlAircraft;

#endif
