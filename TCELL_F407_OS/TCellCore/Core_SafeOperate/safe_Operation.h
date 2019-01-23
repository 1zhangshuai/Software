#ifndef _SAFE_OPERATION_H_
#define _SAFE_OPERATION_H_

#include "sys_Platform.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

#include "status_Aircraft.h"
#include "hci_oledshow.h"

typedef enum
{
	SAFE_TASK_CHECK_BREAKDOWN = 0,
	SAFE_TASK_CHECK_NORMAL    = 1,
}SAFE_TASK_CHECK_STATUS;

typedef struct
{
	volatile SYS_RETERR 			RETERR[30];
	volatile SAFE_TASK_CHECK_STATUS TASK_CHECK_STATUS;
}Safe_Task_Check_Status;

typedef struct
{
	volatile UAV_FLY_MISSION 	    SAFE_FORCE_MISSION;	/*���ڰ�ȫ�Զ�����ǿ������*/
	volatile Safe_Task_Check_Status Task_Check_Status;   /*������ȼ��״̬*/
}Safe_Operation;

/*�������״̬���*/
SAFE_TASK_CHECK_STATUS safe_task_status_check(Safe_Operation *safe_Operation);

/*��ȡ��ȫǿ�Ʒ�������*/
UAV_FLY_MISSION safe_force_mission_get(Safe_Operation *safe_Operation);

/*�ж�ִ�������Ƿ���ȷ*/
SYS_BOOLSTATUS safe_task_execute_period_is_true(fp32 realPeriodMs, fp32 expectPeriodMs);

/*�۲�OLED����ʱ,�ɻ�ǿ������*/
void safe_watch_oled_uav_auto_lock(Uav_Status *uavStatus, HciShowPage *hciShowPage);


extern Safe_Operation gs_SafeOperation;
extern Safe_Operation *gps_SafeOperation;

/*OLED��ʾ����ʱ��ֹ����*/
#define SAFE_FORBID_UNLOCK_WHEN_OLED_SHOW			(SYS_DISABLE)

#endif
