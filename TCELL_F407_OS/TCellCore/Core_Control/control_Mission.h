#ifndef _CONTROL_MISSION_H_
#define _CONTROL_MISSION_H_

#include "sys_Platform.h"
#include "control_Config.h"
#include "status_Aircraft.h"

/*������Ч��״̬*/
typedef enum
{
	CONTROL_MISSION_SWITCH_DISAVA = 0, /*��Ч*/
	CONTROL_MISSION_SWITCH_AVA    = 1, /*��Ч*/
}CONTROL_MISSION_SWITCH_STATUS;

/*����ʱ״̬*/
typedef enum
{
	CONTROL_MISSION_COUNTDOWN_FINISH = 0,	/*����ʱ����*/	
	CONTROL_MISSION_COUNTDOWN_START  = 1,	/*����ʱ��ʼ*/
}CONTROL_MISSION_COUNTDOWN_STATUS;

/*��ʼ���ʹ��״̬*/
typedef enum
{
	CONTROL_MISSION_START_CHECK_DISABLE = 0,  /*ʧ�ܼ��*/	
	CONTROL_MISSION_START_CHECK_ENABLE  = 1,  /*ʹ�ܿ�ʼ���*/
}CONTROL_MISSION_START_CHECK_STATUS;

typedef struct
{
	volatile REMOT_SWITCH_EDGE_AVA_STATUS       HIGH_EDGE_AVA_STATUS;  /*������Ч״̬*/
	volatile REMOT_SWITCH_EDGE_AVA_STATUS       LOW_EDGE_AVA_STATUS;   /*������Ч״̬*/
	volatile CONTROL_MISSION_COUNTDOWN_STATUS   COUNTDOWN_STATUS;	   /*����ʱ״̬*/
	volatile CONTROL_MISSION_START_CHECK_STATUS CHECK_STATUS;		   /*��ʼ���ʹ��״̬*/
	vu8				  			  		        edge_cnt;  		       /*��������*/
	SimulateWatchDog					        count_down;			   /*����ʱ*/
}Control_Fly_Mission_Switch_Motion;

/*ι�����⹷*/
void feed_control_fly_mission_check_dog(u8 _10msFoc, SimulateWatchDog *mission_check_dog);

/*��ȡ�����⹷���״̬*/
SIM_WATCH_DOG_STATUS get_control_fly_mission_dog_status(SimulateWatchDog *mission_check_dog);

/*=== ���úͻ�ȡ���� ===*/
/*����״̬����ʼ��*/
void control_fly_mission_machine_reset(Uav_Status *uavStatus, UAV_FLY_MISSION TARG_MISSION);

/*�������*/
void control_fly_mission_clear(Uav_Status *uavStatus, UAV_FLY_MISSION TARG_MISSION);

/*���õ�ǰ����*/
void control_fly_mission_set(Uav_Status *uavStatus, UAV_FLY_MISSION SET_MISSION, UAV_MISSION_ATTITUDE_CTRL_STATUS ATTITUDE_CTRL_STATUS);

/*��ȡ��ǰ����*/
UAV_FLY_MISSION control_fly_mission_get(void);

/*=== �������� ===*/
/*һ�����*/
void control_fly_mission_onekey_fly(Uav_Status *uavStatus);

/*һ��/ʧ������/����*/
void control_fly_mission_gohome(void);

/*=== ��Ҫ������+������������� ===*/
typedef enum
{
	MISSION_OPFLOW_FOLLOW_POINT = 2,	/*����׷��*/
	MISSION_OPFLOW_FOLLOW_LINE  = 4,	/*����ѭ��*/
}MISSION_ULTR_OPFLOW_NBR;

/*������*/
UAV_FLY_MISSION control_fly_mission_check_ultr_opflow_pos(Uav_Status *uavStatus, u32 checkPeriodMS);

/*����׷��*/
void control_fly_mission_ultr_opflow_follow_point(void);

/*����ѭ��*/
void control_fly_mission_ultr_opflow_follow_line(void);

/*=== ��ҪGPS+��ѹ�ƶ�������� ===*/
typedef enum
{
	MISSION_GPS_WRITE_POINT = 2,	/*GPS��¼��ǰ��*/
	MISSION_GPS_PATROL_SKY  = 4,	/*GPS����¼��Ѳ��*/
}MISSION_BERO_GPS_NBR;

/*������*/
UAV_FLY_MISSION control_fly_mission_check_bero_gps_pos(Uav_Status *uavStatus, u32 checkPeriodMS);

/*GPS��¼��ǰ����*/
void control_fly_mission_bero_gps_write_pos(Uav_Status *uavStatus);

/*GPSѲ��*/
void control_fly_mission_bero_gps_patrol_sky(Uav_Status *uavStatus);

/*=== ��ҪOPENMV������ ===*/

#endif
