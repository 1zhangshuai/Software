#include "safe_Operation.h"

Safe_Operation gs_SafeOperation = 
{
	/*���ڰ�ȫ�Զ�����ǿ������*/	
	.SAFE_FORCE_MISSION = UAV_FLY_MISSION_NULL,		   
	
	/*������ȼ��*/
	.Task_Check_Status  =
	{
		.RETERR 		   = {SYS_RETERR_0ZR},			  /*Ĭ��ȫ�޴�*/
		.TASK_CHECK_STATUS = SAFE_TASK_CHECK_BREAKDOWN,   /*������ȼ��״̬*/
	},		
};

Safe_Operation *gps_SafeOperation = &gs_SafeOperation;

/*�������״̬���*/
SAFE_TASK_CHECK_STATUS safe_task_status_check(Safe_Operation *safe_Operation)
{
	static vu8 i;
	
	/*�ж�������ȵ��ź����Ƿ�����*/
	
	/*=== �������� ===*/
	if (base_module_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[0] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[0] = SYS_RETERR_1ST;	
	}

	/*=== ���������ݻ�ȡ����̬�ں� ===*/
	if (euler_angle_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[1] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[1] = SYS_RETERR_2ND;	
	}
	
	if (ver_fusion_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[2] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[2] = SYS_RETERR_3RD;	
	}
	
	if (gps_update_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[3] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[3] = SYS_RETERR_4TH;	
	}
	
	if (opflow_update_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[4] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[4] = SYS_RETERR_5TH;	
	}
	
	if (gps_hor_fusion_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[5] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[5] = SYS_RETERR_6TH;	
	}
	
	if (opflow_hor_fusion_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[6] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[6] = SYS_RETERR_7TH;	
	}
	
	/*=== ����ϵͳ ===*/
	if (uav_ctrl_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[7] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[7] = SYS_RETERR_8TH;		
	}	

	/*=== У׼ϵͳ ===*/
	if (uav_calib_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[8] = SYS_RETERR_0ZR;
	}	
	else
	{
		safe_Operation->Task_Check_Status.RETERR[8] = SYS_RETERR_9TH;
	}	
	
	/*=== ������� ===*/
	if (tfsd_fly_log_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[9] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[9] = SYS_RETERR_10TH;
	}	
	
	/*=== �˻����� ===*/
	if (hci_oled_show.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[10] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[10] = SYS_RETERR_11TH;
	}
	
	if (hci_host_slave_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[11] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[11] = SYS_RETERR_12TH;
	}	
	
	/*=== �������״̬ ===*/
	if (task_status_check_sem.value == 0)
	{
		safe_Operation->Task_Check_Status.RETERR[12] = SYS_RETERR_0ZR;
	}
	else
	{
		safe_Operation->Task_Check_Status.RETERR[12] = SYS_RETERR_13TH;
	}	
	
	/*�жϸ��߳���������Ƿ�����*/
	for (i = 0; i < 30; i++)
	{
		/*ĳһ���̵߳���������*/
		if (safe_Operation->Task_Check_Status.RETERR[i] != SYS_RETERR_0ZR)
		{
			/*���ϵͳ����*/
			safe_Operation->Task_Check_Status.TASK_CHECK_STATUS = SAFE_TASK_CHECK_BREAKDOWN;
			
			break;
		}
		else
		{
			/*���ϵͳ����*/
			safe_Operation->Task_Check_Status.TASK_CHECK_STATUS = SAFE_TASK_CHECK_NORMAL;			
		}
	}
	
	/*��������쳣*/
	if (safe_Operation->Task_Check_Status.TASK_CHECK_STATUS == SAFE_TASK_CHECK_BREAKDOWN)
	{
		/*���ð�ȫǿ������: �Զ�һ������*/
		safe_Operation->SAFE_FORCE_MISSION = UAV_FLY_MISSION_ONEKEY_LAND_HOME;
	}
	/*�����������*/
	else if (safe_Operation->Task_Check_Status.TASK_CHECK_STATUS == SAFE_TASK_CHECK_NORMAL)
	{
		/*��ȫǿ���������*/
		safe_Operation->SAFE_FORCE_MISSION = UAV_FLY_MISSION_NULL;
	}
	
	return (safe_Operation->Task_Check_Status.TASK_CHECK_STATUS);
}


/*��ȡ��ȫǿ�Ʒ�������*/
UAV_FLY_MISSION safe_force_mission_get(Safe_Operation *safe_Operation)
{
	return (safe_Operation->SAFE_FORCE_MISSION);
}

