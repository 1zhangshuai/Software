#ifndef _CONTROL_LANDING_H_
#define _CONTROL_LANDING_H_

#include "sys_Platform.h"
#include "control_Config.h"
#include "status_Aircraft.h"

/*���������HOME��ˮƽ����ּ�*/
#define CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_CM				(600)	/*����home��Զ,��ΪA���� cm*/	
#define CTRL_AUTO_GO_HOME_NEAR_DISTANCE_CM					(150)	/*����home��Զ,��ΪB���� cm*/	
#define CTRL_AUTO_GO_HOME_ARRIVE_DISTANCE_CM				(0)		/*����HOME��,��ΪC���� cm*/
	
/*���ݷ��������HOME��ˮƽ����,�����������ٶ�*/	
#define CTRL_AUTO_GO_HOME_A2B_FIRST_NAV_SPEED_CM_S			(100)	/*A->B,һ��Ѳ���ٶ� cm/s*/
#define CTRL_AUTO_GO_HOME_B2C_SECOND_NAV_SPEED_CM_S			(80)	/*B->C,����Ѳ���ٶ� cm/s*/
#define CTRL_AUTO_GO_HOME_NEARC_THIRD_NAV_SPEED_CM_S		(60)	/*C����,����Ѳ���ٶ� cm/s*/
#define CTRL_AUTO_GO_HOME_DEFAULT_NAV_SPEED_CM_S			(50)	/*Ĭ��Ѳ���ٶ� cm/s*/

/*���ݷ��������HOME��ˮƽ����,������������ȫ�߶Ⱥ��ٶ�*/
#define CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_SAFE_HEIGHT_CM	(800)	/*���������HOME��Զʱ�İ�ȫ�߶� cm*/
#define CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_CLIMB_CM_S		(80)	/*���������HOME��Զʱ����С�ڰ�ȫ�߶�,ԭ�������ٶ� cm*/
#define CTRL_AUTO_GO_HOME_ARRIVE_DESCEND_CM_S				(-40)	/*������HOME���Ϸ�ʱ,�Ͻ��ٶ� cm*/
#define CTRL_AUTO_GO_HOME_RAPID_DESCEND_CM_S				(-80)	/*�����½�������ʱ���ٶ� cm*/

/*��������ȫ�߶���ˮƽ����HOME��Ĺ���ʱ��*/
#define CTRL_AUTO_GO_HOME_CLIMB_THEN_CLOSE_TRANSITION_TICKS	(400)  	/*400*5 = 2s*/

/*��ֱ�ٶȱ仯����*/
#define CTRL_AUTO_GO_HOME_VERTICAL_SPEED_CHANGE_MAX_PERIOD	(8)		/*8*5 = 40ms*/

/*��������*/
typedef enum
{
	CTRL_GO_HOME_NULL_PROCESS   			    = 0,	/*��Чö��*/	
	CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME = 1,	/*Զ��HOME��,����������ȫ�߶�,����*/
	CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME 			= 2,	/*����HOME��,���ְ�ȫ�߶�,����*/	
	CTRL_LAND_ARRIVE_DESCEND_GO_HOME 			= 3,	/*HOME�㸽��,����*/
}CTRL_GO_HOME_PROCESS;

/*���������HOME�����궨*/
typedef enum
{
	CTRL_GO_HOME_NULL_DISTANCE    = 0,	/*��Чö��*/
	CTRL_GO_HOME_FARAWAY_DISTANCE = 1,	/*��������HOME��Զ����*/
	CTRL_GO_HOME_NEAR_DISTANCE 	  = 2,	/*��������HOME�������*/	
	CTRL_GO_HOME_ARRIVE_DISTANCE  = 2,	/*�����㿿��HOME��*/
}CTRL_GO_HOME_DISTANCE;

/*����ʱ�Ƿ��ֶ���Ԥ*/
typedef enum
{
	CTRL_GO_HOME_HAND_MEDDLE_TRUE  = 0,	/*���ֶ���Ԥ*/
	CTRL_GO_HOME_HAND_MEDDLE_FALSE = 1,	/*���ֶ���Ԥ*/	
}CTRL_GO_HOME_HAND_MEDDLE_STATUS;

/*����ʱ��ֱ�߶Ⱥ�ˮƽλ�ƿ���ģʽ*/
typedef enum
{	
	CTRL_GO_HOME_NULL_CTRL = 0,	/*��Чö��*/
	CTRL_GO_HOME_AUTO_CTRL = 1,	/*�Զ�����ģʽ*/
	CTRL_GO_HOME_HAND_CTRL = 2,	/*�ֶ�����ģʽ*/
}CTRL_GO_HOME_CTRL_MODE;

/*�Ƿ�Ϊ��һ��(���ֶ�����/�����ֶ������)�з�������*/
typedef enum
{
	CTRL_GO_HOME_FIRST_SWITCH_TURE  = 1, /*��һ���з���*/
	CTRL_GO_HOME_FIRST_SWITCH_FALSE = 2, /*�ǵ�һ���з���*/
}CTRL_GO_HOME_FIRST_SWITCH_STATUS;

/*�����Ƿ�ﵽ��ȫ�߶�*/
typedef enum
{
	CTRL_GO_HOME_SAFE_HEIGHT_NULL	   = 0,	/*��ֵ��ȫ�߶��Ѵﵽ,�����������״̬,��ֹ��������*/
	CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE = 1,	/*δ�ﰲȫ�߶�*/	
	CTRL_GO_HOME_SAFE_HEIGHT_ARRIVE    = 2,	/*�Ѵﰲȫ�߶�*/
}CTRL_GO_HOME_SAFE_HEIGHT_STATUS;

/*����״̬����״̬*/
typedef enum
{
	CTRL_GO_GOME_TRANSITION_SET_NO = 1,	/*����״̬δ��������*/	
	CTRL_GO_GOME_TRANSITION_SET_OK = 2,	/*����״̬������*/
}CTRL_GO_GOME_TRANSITION_SET_STATUS;

/*��½����Ƿ�ʹ��*/
typedef enum
{
	CTRL_LAND_CHECK_ENABLE  = 0, /*��½���ʹ��*/
	CTRL_LAND_CHECK_DISABLE = 1, /*��½���ʧ��*/
}CTRL_LAND_CHECK_STATUS;

typedef struct
{
	vu16								  	    land_throttle_min_check;	    /*��½���ż��ֵ*/
	volatile CTRL_LAND_CHECK_STATUS             LAND_CHECK_STATUS;    	        /*�Ƿ�������½���*/	
	
	volatile CTRL_GO_HOME_DISTANCE 			    HOME_DISTANCE;					/*��HOME�����궨*/	
	volatile CTRL_GO_HOME_PROCESS  			    LAST_GO_HOME_PROCESS;			/*�ϴη�������*/
	volatile CTRL_GO_HOME_PROCESS  			    CUR_GO_HOME_PROCESS;			/*���η�������*/	
	volatile CTRL_GO_HOME_HAND_MEDDLE_STATUS    VERTICAL_HAND_MEDDLE_STATUS;	/*�߶��ֶ���Ԥ״̬*/
	volatile CTRL_GO_HOME_HAND_MEDDLE_STATUS    HORIZONTAL_HAND_MEDDLE_STATUS;	/*ˮƽ�ֶ���Ԥ״̬*/	
	volatile CTRL_GO_HOME_CTRL_MODE			    LAST_HORIZONTAL_CTRL_MODE;		/*�ϴ�ˮƽ����ģʽ*/
	volatile CTRL_GO_HOME_CTRL_MODE			    CUR_HORIZONTAL_CTRL_MODE;		/*����ˮƽ����ģʽ*/
	volatile CTRL_GO_HOME_FIRST_SWITCH_STATUS   FIRST_SWITCH_STATUS;			/*�Ƿ�Ϊ��һ���з���ģʽ*/	
	volatile CTRL_GO_HOME_SAFE_HEIGHT_STATUS    SAFE_HEIGHT_STATUS;				/*��ȫ�߶�״̬*/
	volatile CTRL_GO_GOME_TRANSITION_SET_STATUS TRANSITION_SET_STATUS;			/*����״̬����״̬*/
	Vector2s_Nav 							    LandHomePosition;				/*��λ��ˮƽ����*/
	Vector2s_Nav							  	LandCurPosition;				/*��ǰˮƽ����*/	
	Vector2s_Nav 							  	LandTargetPosition;				/*����ˮƽ����*/
	fp32 									  	heightExpect;					/*Ŀ����ֱ�߶�����*/
	fp32 									  	verticalSpeedExpect;			/*Ŀ����ֱ����/�½��ٶ�����*/
	vu8											verticalCalimbSpeedRate;		/*��ֱ�����ٶȱ仯��*/
	vs8											verticalDescendSpeedRate;		/*��ֱ�½��ٶȱ仯��*/	
	vu8										    verticalSpeedChangePeriod;		/*��ֱ�ٶȱ仯����*/
	u8											horizontalSpeedExpect;			/*ˮƽ�ٶ�����*/
	vu16									  	climb2CloseTransitionTicks;		/*����������ˮƽ��£����ʱ��*/
	Vector2f_Nav								RelativeHomeDistance;			/*��ǰ�����HOME��ˮƽ����*/
}ControlLand;


/*����״̬���*/
UAV_LAND_STATUS ctrl_Landing_Status_Check(Uav_Status *uavStatus);

/*��ⷵ��ʱ�ֶ���Ԥ�߶�*/
CTRL_GO_HOME_HAND_MEDDLE_STATUS ctrl_Go_Home_Vertical_Hand_Meddle(void);

/*��ⷵ��ʱ�ֶ���Ԥˮƽ*/
CTRL_GO_HOME_HAND_MEDDLE_STATUS ctrl_Go_Home_Horizontal_Hand_Meddle(void);

/*������״̬����*/
void ctrl_Go_Home_Status_Update(void);

/*��������*/
void ctrl_Go_Home_Control(fp32 controlDeltaT);

/*������ֱ�߶ȿ���*/
void ctrl_Go_Home_Vertical_Control(fp32 verticalSpeedExpect, fp32 heightExpect, fp32 controlDeltaT);

/*����ˮƽλ�ÿ���*/
void ctrl_Go_Home_Horizontal_Control(Vector2s_Nav targPos, Vector2s_Nav curPos, CTRL_GO_HOME_DISTANCE HOME_DISTANCE);

/*��������GPS*/
void ctrl_Go_Home_GPS_Control(fp32 controlDeltaT);

/*��ͨԭ����½����(NO GPS)*/
void ctrl_Land_Ground_Control(fp32 controlDeltaT, Uav_Status *uavStatus);

/*�õ����Ŀ������Pit��Rol����ƫ��*/
Vector2f_Nav land_Gps_Offset_Relative_To_Home(Vector2s_Nav targPos, Vector2s_Nav curPos);

/*��½��������*/
extern ControlLand g_sControlLand;

extern ControlLand *g_psControlLand;

#endif
