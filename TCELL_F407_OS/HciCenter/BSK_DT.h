#ifndef _BSK_DT_H_
#define _BSK_DT_H_

#include "sys_Platform.h"

typedef enum
{
	OFF_BSK_DT_SEND_BASE_FLY_DATA     = 0x01,	/*�����ɿ�����*/
	OFF_BSK_DT_SEND_FLY_STATUS	      = 0x02,	/*����״̬*/
	OFF_BSK_DT_SEND_SENSOR_DATA       = 0x03,	/*����������*/
	OFF_BSK_DT_SEND_SENSOR_CALIB_DATA = 0x04, 	/*������У׼����*/
	OFF_BSK_DT_SEND_SENSOR_CALIB_CMD  =  0x05,  /*������У׼����*/
	OFF_BSK_DT_SEND_REMOT_DATA		  = 0x08,	/*ң������*/
	OFF_BSK_DT_SEND_MOTOR_PWM_OUTPUT  = 0x09,   /*���PWM���*/
	OFF_BSK_DT_SEND_BATTERY_INFO	  = 0x0A,   /*�����Ϣ*/
	OFF_BSK_DT_SEND_ATTITUDE_PID_PARA = 0x10, 	/*��̬PID����*/
	OFF_BSK_DT_SEND_POSITION_PID_PARA = 0x11, 	/*λ��PID����*/
	OFF_BSK_DT_SEND_PID_RDWR_RESPONSE = 0x12,  	/*PID��д��Ӧ*/
	OFF_BSK_DT_SEND_GPS_DATA		  = 0x20, 	/*GPS����*/
	OFF_BSK_DT_SEND_ATT_EST_CTRL_DATA = 0x30, 	/*��̬�������������*/
	OFF_BSK_DT_SEND_SPD_EST_CTRL_DATA = 0x31, 	/*�ٶȹ������������*/
	OFF_BSK_DT_SEND_POS_EST_CTRL_DATA = 0x32, 	/*λ�ù������������*/
	OFF_BSK_DT_SEND_USER_DATA		  = 0x33, 	/*�û��Զ�������*/
	OFF_BSK_DT_SEND_HEART_PACKAGE     = 0x05, 	/*������*/
}OFF_BSK_DT_SEND_HOST_MSG_ID;

/**/

#endif
