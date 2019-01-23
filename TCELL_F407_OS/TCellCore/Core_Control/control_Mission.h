#ifndef _CONTROL_MISSION_H_
#define _CONTROL_MISSION_H_

#include "sys_Platform.h"
#include "control_Config.h"
#include "status_Aircraft.h"

/*�������������Ч��״̬*/
typedef enum
{
	MISSION_CLEAR_SWITCH_DISABLE = 0, /*��Ч*/
	MISSION_CLEAR_SWITCH_ENABLE  = 1, /*��Ч*/
}MISSION_CLEAR_SWITCH_STATUS;

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

/*��������������*/
typedef enum
{	
	/*NULL*/
	UAV_FLY_MISSION_NULL                = 00,   /*û������*/
	
	/*һ��*/
	UAV_FLY_MISSION_ONEKEY_FLY			= 01,	/*һ�����*/
	UAV_FLY_MISSION_ONEKEY_LAND_HOME    = 02,	/*һ������/����*/
	
	/*������+��������ģʽ��*/
	UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT = 06,	/*����׷��*/
	UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE  = 07,	/*����Ѳ��*/
	
	/*��ѹ��+GPS����ģʽ��*/
	UAV_FLY_MISSION_GPS_WRITE_POS		= 10,   /*GPSд�뵱ǰ��λ*/
	UAV_FLY_MISSION_GPS_CLEAR_ALL_POS	= 11,   /*������е�λ*/	
	UAV_FLY_MISSION_GPS_PATROL_SKY      = 12,	/*GPSѲ��*/
}UAV_FLY_MISSION;

/*������Ч״̬*/
typedef enum
{
	UAV_MISSION_DISABLE = 0,	/*������Ч*/	
	UAV_MISSION_ENABLE  = 1,	/*������Ч*/
}UAV_MISSION_ENABLE_STATUS;

/*������Ŀ�������״̬*/
typedef enum
{
	UAV_MISSION_TARG_SET_NO = 0, /*Ŀ���δ����*/
	UAV_MISSION_TARG_SET_OK = 1, /*Ŀ���������*/
}UAV_MISSION_TARG_SET_STATUS;	

/*������Ŀ��㵽��״̬*/
typedef enum
{
	UAV_MISSION_TARG_REACH_NO = 0, /*Ŀ���δ����*/
	UAV_MISSION_TARG_REACH_OK = 1, /*Ŀ����ѵ���*/
}UAV_MISSION_TARG_REACH_STATUS;

/*�����Ƿ�����ִ��*/
typedef enum
{
	UAV_MISSION_EXECUTE_NOT = 0, /*ûִ��*/
	UAV_MISSION_EXECUTE_YES = 1, /*����ִ��*/
}UAV_MISSION_EXECUTE_STATUS;

typedef struct
{
	UAV_MISSION_ENABLE_STATUS        ENABLE_STATUS;
	UAV_MISSION_TARG_SET_STATUS      TARG_SET_STATUS;
	UAV_MISSION_TARG_REACH_STATUS    TARG_REACH_STATUS;
	UAV_MISSION_EXECUTE_STATUS       EXECUTE_STATUS;
}Uav_Mission_Status;

/*һ������*/
typedef struct
{
	Uav_Mission_Status FixedHeightFly;
	Uav_Mission_Status LandHome;
}Uav_Onekey_Mission;

/*д���λ����*/
typedef struct
{
	UAV_MISSION_TARG_SET_STATUS   SET_STATUS; /*���״̬*/
	Vector3f_Nav                  Pos;    /*��*/
}Uav_Limitless_Pos;

typedef struct
{
	vu16 			   index; 	   											/*λ�õ��˳��*/
	vu16			   posTotalNbr;											/*д�����ܸ���*/
	Uav_Limitless_Pos  Limitless_Pos[MISSION_LIMITLESS_POS_WRITE_MAX_NBR];  /*λ�õ�*/
}Uav_Write_Gps_Mission;

/*�Ƿ�����ͬʱִ�����������̬����*/
typedef enum
{
	UAV_MISSION_ATTITUDE_CTRL_ENABLE  = 1, /*����ͬʱִ�����������̬����*/
	UAV_MISSION_ATTITUDE_CTRL_DISABLE = 0, /*��ֹͬʱִ�����������̬����*/
}UAV_MISSION_ATTITUDE_CTRL_STATUS;

typedef struct
{
	/*��ǰ��������*/
	UAV_FLY_MISSION    			     CURRENT_FLY_MISSION;
	
	/*�ϴη�������*/
	UAV_FLY_MISSION    				 LAST_FLY_MISSION;
	
	/*�Ƿ�����ͬʱִ����̬����*/
	UAV_MISSION_ATTITUDE_CTRL_STATUS ATTITUDE_CTRL_STATUS;
	
	/*�������������Ч��״̬*/
	MISSION_CLEAR_SWITCH_STATUS      CLEAR_SWITCH_STATUS;
	
	/*һ������*/
	Uav_Onekey_Mission 			     Onekey_Mission;
	
	/*д���λ����*/
	Uav_Write_Gps_Mission 		     Write_Gps_Mission;
}Uav_Fly_Mission;


/*ι�����⹷*/
void feed_control_fly_mission_check_dog(u8 _10msFoc, SimulateWatchDog *mission_check_dog);

/*��ȡ�����⹷���״̬*/
SIM_WATCH_DOG_STATUS get_control_fly_mission_dog_status(SimulateWatchDog *mission_check_dog);

/*=== ���úͻ�ȡ���� ===*/
/*����״̬����ʼ��*/
void control_fly_mission_machine_reset(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION TARG_MISSION);

/*�������*/
void control_fly_mission_clear(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION TARG_MISSION);

/*�����һ������,���õ�ǰ����*/
void control_fly_mission_set(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION SET_MISSION, UAV_MISSION_ATTITUDE_CTRL_STATUS ATTITUDE_CTRL_STATUS);

/*��ȡ��ǰ����*/
UAV_FLY_MISSION control_fly_mission_get(void);

/*=== �������� ===*/
/*һ�����*/
void control_fly_mission_onekey_fly(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*һ��/ʧ������/����*/
void control_fly_mission_auto_gohome(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus, fp32 controlDeltaT);

/*=== ��Ҫ������+������������� ===*/
typedef enum
{
	MISSION_OPFLOW_FOLLOW_POINT = 2,	/*����׷��*/
	MISSION_OPFLOW_FOLLOW_LINE  = 4,	/*����ѭ��*/
}MISSION_ULTR_OPFLOW_NBR;

/*������*/
UAV_FLY_MISSION control_fly_mission_check_ultr_opflow_pos(Uav_Fly_Mission *uav_fly_mission, u32 checkPeriodMS);

/*����׷��*/
void control_fly_mission_ultr_opflow_follow_point(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*����ѭ��*/
void control_fly_mission_ultr_opflow_follow_line(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*=== ��ҪGPS+��ѹ�ƶ�������� ===*/
typedef enum
{
	MISSION_GPS_WRITE_LIMITLESS_POS     = 2,	/*GPS��¼��ǰ��*/
	MISSION_GPS_PATROL_SKY              = 4,	/*GPS����¼��Ѳ��*/
	MISSION_GPS_CLEAR_ALL_LIMITLESS_POS = 6,	/*������м�¼��*/	
}MISSION_BERO_GPS_NBR;

/*������*/
UAV_FLY_MISSION control_fly_mission_check_bero_gps_pos(Uav_Fly_Mission *uav_fly_mission, u32 checkPeriodMS);

/*GPS��¼��ǰ����*/
void control_fly_mission_bero_gps_write_pos(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*GPS������м�¼������*/
void control_fly_mission_bero_gps_clear_all_pos(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*GPSѲ��*/
void control_fly_mission_bero_gps_patrol_sky(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus);

/*=== ��ҪOPENMV������ ===*/


extern Uav_Fly_Mission gs_Uav_Fly_Mission;

#endif
