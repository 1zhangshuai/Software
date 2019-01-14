#ifndef _CONTROL_CONFIG_H_
#define _CONTROL_CONFIG_H_

#include "sys_Platform.h"
#include "attitude_Aircraft.h"

/*======= 0.x,y,z�����ٶ�/�Ƕ����� =======*/
/*z�����ٶ�����*/
#define CTRL_HEIGHT_CLIMB_UP_MAX_SPEED     			   (150)         /*������������ٶ�(150cm/s):600~1000*/
#define CTRL_HEIGHT_CLIMB_DOWN_MAX_SPEED    		   (80)          /*������������ٶ�(150cm/s):0~400*/
/*z������ٶ�����*/
#define CTRL_HEIGHT_CLIMB_UP_MAX_ACCELERATION          (300)         /*��������������ٶ�(300cm/s^2)*/
#define CTRL_HEIGHT_CLIMB_DOWN_MAX_ACCELERATION   	   (200)         /*��������������ٶ�(150cm/s^2)*/

/*x,y����,�ٶ�����*/
#define CTRL_HORIZONTAL_MAX_MOVE_SPEED		           (200) 		 /*ˮƽ������������ٶ�(200cm/s)*/
/*x,y����,�Ƕ�����*/
#define REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX              (30)			 /*ң��PITCH��ROLL��������г̽Ƕ�*/
#define REMOT_YAW_ANGLE_EXPECT_MAX  	               (150)		 /*ң��YAW��������г�:+150~-150*/
#define GIMBAL_PITCH_ANGLE_EXPECT_MAX              	   (90)			 /*�������̨PITCH��ROLL��������г�:+90~-90*/
#define GIMBAL_YAW_ANGLE_EXPECT_MAX  	               (150)		 /*�������̨YAW��������г�:+150~-150*/
#define REMOT_ANGLE_DEADZONE_EXCEPT		               (450)		 /*1000~1450, 1550~2000,����֮�����Ч��Χ:450*/

/*x,y����,�Ƕȹ��󱣻�,�Զ��رյ��*/
#define CTRL_HORIZONTAL_ANGLE_OVER_CHECK			   (SYS_ENABLE)
#define CTRL_HORIZONTAL_ANGLE_SAFE_MAX				   (60)			 /*���������б�Ƕ�*/

/*======= 1.Altitude Hold(��ֱ����) =======*/
#define CTRL_HEIGHT_YAW_SPEED_FEEDFORWARD			   (SYS_ENABLE)  /*��ֱ�ٶ�ǰ��������*/
#define CTRL_HEIGHT_POS_CONTROL_ACC_STATUS             (SYS_ENABLE)  /*������������ģʽ:����ֱ�߶�λ��+��ֱ�ٶ�+��ֱ���ٶ�:SYS_ENABLE(1)����,SYS_DISABLE(0)�ر�*/
#define CTRL_HEIGHT_ONEKEY_TAKEOFF_HEIGHT			   (150)		 /*һ�������ظ߶�(cm),��ֵӦ�ڳ�������Χ(200cm)��*/


/*======= 2.Position Hold(ˮƽ����) =======*/
#define CTRL_HORIZONTAL_SENSOR_MODE_REMOT_EXPECT_ANGLE (SYS_DISABLE) /*ˮƽ����ģʽ�£����ֱ�Ӹ���̬������*/
#define CTRL_HORIZONTAL_SENSOR_MODE_REMOT_EXPECT_SPEED (SYS_ENABLE)  /*ˮƽ����ģʽ�£����ֱ�Ӹ���̬�����ٶ�*/

/*======= 3.����,���,��½������� =======*/
/*����*/
#define CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS        (100)  		 /*����������������,5ms*100=500ms*/
#define CTRL_IDEL_TRANSITION_INC_PERIOD_TICKS          (2)   		 /*���ٵ������ʱ��,10*5=50ms*/
#define CTRL_IDEL_THROTTLE_VALUE			           (1150) 		 /*���ŵ���,ȡ�ӽ���ת����ֵ����*/

/*���*/
#define CTRL_THROTTLE_START_TURN  				       (1100) 		 /*��ת��������������ǲ����ã�̫��ᵼ�¹�����*/
#define CTRL_THROTTLE_START_FLY_VALUE  			       (1250) 		 /*���������*/

/*��ͣ����*/
#define CTRL_THROTTLE_HOVER_ENABLE_STATUS			   (SYS_ENABLE)  /*Ĭ����ͣ����*/
#define CTRL_THROTTLE_HOVER_DEFAULT_VALUE			   (1450) 		 /*Ĭ����ͣ����*/


/*��½*/
#define CTRL_LAND_CHECK_THROTTLE_THRESHOLD_MIN		   (1150) 		 /*��½���������Сֵ*/

/*======= 4.����ϵͳ������������(ms) taskPeriod * cnt =======*/
/*��������ʱ��: PLATFORM_TASK_SCHEDULER_MIN_FOC_MS*/
/*�߶ȿ�������*/
#define CTRL_VERTICAL_POSITION_CONTROL_PERIOD	       (2)	/*��ֱλ�ÿ�������(10ms)*/
#define CTRL_VERTICAL_SPEED_CONTROL_PERIOD			   (1)	/*��ֱ�ٶȿ�������(5ms)*/
/*ˮƽ��������_GPS*/
#define CTRL_GPS_HORIZONTAL_POSITION_CONTROL_PERIOD	   (4)	/*ˮƽλ�ÿ�������(20ms)*/
#define CTRL_GPS_HORIZONTAL_SPEED_CONTROL_PERIOD	   (2)	/*ˮƽ�ٶȿ�������(10ms)*/
/*ˮƽ��������_OpticFlow*/
#define CTRL_OPFLOW_HORIZONTAL_POSITION_CONTROL_PERIOD (8)	/*ˮƽλ�ÿ�������(40ms)*/
#define CTRL_OPFLOW_HORIZONTAL_SPEED_CONTROL_PERIOD	   (4)	/*ˮƽ�ٶȿ�������(20ms)*/


/*======= ���� =======*/
#define CTRL_THROTTLE_TOP_VALUE						   (1000) /*�������ֵ*/
#define CTRL_THROTTLE_BOTTOM_VALUE					   (0)    /*��С����ֵ*/ 
#define REMOT_THROTTLE_DOWN_CROSSOVER	               (500 - REMOT_THROTTLE_MID_DEADZONE * 0.5)	/*���Ÿ˸��������ٶ�ʱ,���г��ٽ�ֵ*/
#define REMOT_THROTTLE_UP_CROSSOVER	   	               (500 + REMOT_THROTTLE_MID_DEADZONE * 0.5)	/*���Ÿ˸��������ٶ�ʱ,���г��ٽ�ֵ*/
#define REMOT_ANGLE_DEADZONE_BUTTOM 	               (REMOT_ANGLE_MID - REMOT_ANGLE_MID_DEADZONE * 0.5) /*1450*/
#define REMOT_ANGLE_DEADZONE_TOP    	               (REMOT_ANGLE_MID + REMOT_ANGLE_MID_DEADZONE * 0.5) /*1550*/

#include "control_AltHold.h"	/*����*/
#include "control_PosHold.h"    /*����*/
#include "control_Landing.h"    /*����/����*/
#include "control_Mission.h" 	/*��������*/
#include "sins_Strapdown.h"		/*�����ߵ������ں�*/

#include "sys_ControlMap.h"

#ifdef CONTROL_SYS__PID
	#include "pid_System.h"
	/*PID�����㷨*/
/*config*/
	#define PID_SYSTEM_CONFIG_USE_AVERAGE_FILTER		(SYS_DISABLE) /*��ֵ�˲�*/
#endif

#ifdef CONTROL_SYS__ADRC
	#include "adrc_System.h"	
	/*ADRC�����㷨*/
#endif

#endif
