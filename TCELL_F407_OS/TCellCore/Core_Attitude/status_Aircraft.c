#include "status_Aircraft.h"
#include "remot_DataAnaly.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

Uav_Status g_sUav_Status =
{
	/*= 1.�����������״̬ =*/	               
	.LOCK_STATUS = UAV_LOCK_YES,			/*�ɿ�����״̬*/
	
	.UavLandStatus = 
	{
		.ThisTime = UAV_LAND_YES,
		.LastTime = UAV_LAND_YES,
	},										/*��������½״̬*/
	
	.UavFlyType =
	{
		.CURRENT 			  = UAV_FLY_TYPE_ATTITUDE,
		.BEFORE_WIRELESS_MISS = UAV_FLY_TYPE_ATTITUDE,
	},									    /*����ģʽ*/
	
	.AIRSTOP_TYPE = UAV_AIRSTOP_NOT, 		/*������ͣ״̬*/
	
	.HOME_SET_STATUS     = UAV_HOME_SET_NOT,		    /*GPS HOME ������״̬*/
	.WIRELESS_CMC_STATUS = UAV_WIRELESS_CMC_FAIL,	    /*ң�غͷ�����ͨ��״̬*/
	.HCI_SHOW_STATUS     = UAV_HCI_SHOW_DISABLE,		/*HCI SHOW*/
										   
	/*= 2.������������ʽ״̬ =*/													  /*��ǰ��������*/
	.UavControlMode = 
	{
		.Vertical = 
		{
			.Mode_Switch = 
			{
				.ThisTime = UAV_VERTICAL_CONTROL_SELFAUTO,
				.LastTime = UAV_VERTICAL_CONTROL_SELFAUTO,
			},		   /*ģʽ�л�*/		
			
			.EXPECT_CONTROL_MODE  = UAV_VERTICAL_CONTROL_SELFAUTO,  /*��������ģʽ*/			
			.CONTROL_MODE         = UAV_VERTICAL_CONTROL_SELFAUTO,  /*ʵ�ʿ���ģʽ*/			
			.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO, 	/*�ο�������״̬*/			
		},
		
		.Horizontal = 
		{
			.Mode_Switch = 
			{
				.ThisTime = UAV_HORIZONTAL_CONTROL_SELFAUTO,
				.LastTime = UAV_HORIZONTAL_CONTROL_SELFAUTO,
			},		   /*ģʽ�л�*/		
			
			.EXPECT_CONTROL_MODE  = UAV_HORIZONTAL_CONTROL_SELFAUTO,  /*��������ģʽ*/			
			.CONTROL_MODE         = UAV_HORIZONTAL_CONTROL_SELFAUTO,  /*ʵ�ʿ���ģʽ*/			
			.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO, 	/*�ο�������״̬*/			
		},
	},			/*����������ģʽ*/
	
	.UavSenmodStatus = 
	{
		.Vertical = 
		{
			.WORK_STATUS = UAV_SENMOD_WORK_LIMITLESS, 		 /*Ĭ��ʹ�������ƴ�����*/
			
			.CURRENT_USE = UAV_VERTICAL_SENMOD_CURRENT_BERO, /*Ĭ����ѹ��*/
			
			.Bero = 
			{
				.EXIST_STATUS              = UAV_SENMOD_EXIST_NO,
				.DATA_STATUS               = UAV_SENMOD_DATA_NO,
				.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_NO,
				.FIRST_USE_AVA_STATUS      = UAV_SENMOD_FIRST_USE_AVA_NO,
				.USE_CONTROL_STATUS        = UAV_SENMOD_USE_CONTROL_DISALLOW,
				.FUSION_STATUS 			   = UAV_SENMOD_FUSION_FINISH,					
			},
			
			.Ultr = 
			{
				.EXIST_STATUS              = UAV_SENMOD_EXIST_NO,
				.DATA_STATUS               = UAV_SENMOD_DATA_NO,
				.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_NO,
				.FIRST_USE_AVA_STATUS      = UAV_SENMOD_FIRST_USE_AVA_NO,	
				.USE_CONTROL_STATUS        = UAV_SENMOD_USE_CONTROL_DISALLOW,	
				.FUSION_STATUS 			   = UAV_SENMOD_FUSION_FINISH,					
			},
		},
		
		.Horizontal = 
		{
			.WORK_STATUS = UAV_SENMOD_WORK_LIMITLESS, 		 /*Ĭ��ʹ�������ƴ�����*/
			
			.CURRENT_USE = UAV_HORIZONTAL_SENMOD_CURRENT_GPS, /*Ĭ��GPS*/
			
			.Opticflow = 
			{
				.EXIST_STATUS              = UAV_SENMOD_EXIST_NO,
				.DATA_STATUS               = UAV_SENMOD_DATA_NO,
				.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_NO,
				.FIRST_USE_AVA_STATUS      = UAV_SENMOD_FIRST_USE_AVA_NO,	
				.USE_CONTROL_STATUS        = UAV_SENMOD_USE_CONTROL_DISALLOW,				
			},
			
			.Gps = 
			{
				.EXIST_STATUS              = UAV_SENMOD_EXIST_NO,
				.DATA_STATUS               = UAV_SENMOD_DATA_NO,
				.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_NO,
				.FIRST_USE_AVA_STATUS      = UAV_SENMOD_FIRST_USE_AVA_NO,			
				.USE_CONTROL_STATUS        = UAV_SENMOD_USE_CONTROL_DISALLOW,				
				.FUSION_STATUS 			   = UAV_SENMOD_FUSION_FINISH,				
			},
		},
	},	/*������/ģ��״̬*/
	
	/*= 3.ң�ز���״̬ =*/
	.REMOT_OPERATE_STATUS = UAV_REMOT_OPERATE_NOT_UNLOCK,		/*ң�ز���״̬*/	
	
	/*= 4.��������״̬ =*/
	.UavProgrameStatus =
	{
		.INIT_STATUS = UAV_PROGRAME_INIT_START,
		.CpuUse = 
		{
			.major = 0,
			.minor = 0,
		},
	},			/*��������״̬*/	
};

Uav_Status *g_psUav_Status = &g_sUav_Status;

/*���GPS�Ƿ����(���㶨λ����+�����ںϳɹ�)*/
UAV_SENMOD_USE_CONTROL_STATUS status_GPS_Fix_Ava_Check(Uav_Status *uavStatus)
{
	/*1.HOME�������� 
	  2.GPS�������㶨λ����
	  3.�����ںϳɹ�
	*/
	if ((uavStatus->HOME_SET_STATUS == UAV_HOME_SET_YES) && \
		(uavStatus->UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
		(g_psSinsReal->FUSION_STATUS[SINS_FUSION_HORIZONTAL] == SINS_FUSION_SUCC))
	{
		uavStatus->UavSenmodStatus.Horizontal.Gps.USE_CONTROL_STATUS = UAV_SENMOD_USE_CONTROL_ALLOW; /*�������ڿ���*/
	}
	else
	{
		uavStatus->UavSenmodStatus.Horizontal.Gps.USE_CONTROL_STATUS = UAV_SENMOD_USE_CONTROL_DISALLOW; /*��ֹ���ڿ���*/		
	}
	
	return (uavStatus->UavSenmodStatus.Horizontal.Gps.USE_CONTROL_STATUS);
}

/*��ȡ��ǰ���˻�����̬����ģʽ�����ô�����*/
SYS_BOOLSTATUS check_uav_ctrl_and_sensor_use(Uav_Status *uavStatus, UAV_FLY_TYPE FLY_TYPE, UAV_VERTICAL_SENMOD_CURRENT_USE VER_SENMOD_USE, UAV_HORIZONTAL_SENMOD_CURRENT_USE HOR_SENMOD_USE)
{
	/*�жϺ�ָ��check�Ƿ�һ��*/
	if ((uavStatus->UavFlyType.CURRENT == FLY_TYPE) && \
		(uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == VER_SENMOD_USE) && \
		(uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE == HOR_SENMOD_USE))
	{
		/*������ȷ*/
		return SYS_BOOL_TRUE;
	}
	
	/*���ش���*/
	return SYS_BOOL_FALSE;
}

SimulateWatchDog g_sUavRemotCMCDog = 
{
	.curTicks         = 0, /*��ʼֵ������0*/
	.nextProcessTicks = 0, /*��ʼֵ������0*/
};

SimulateWatchDog *g_psUavRemotCMCDog = &g_sUavRemotCMCDog;

/*ң����ɻ�ͨѶ״̬*/
UAV_WIRELESS_CMC_STATUS status_check_uav_wireless(SimulateWatchDog *uavRemotCMCDog, Uav_Status *uavStatus)
{		
	uavRemotCMCDog->curTicks = my_GetTick();	/*��ȡ��ǰtick foc = 10ms*/
	 
	/*���curTicks < nextProcessTicks,˵��"ι��"��������,��������*/
	if (uavRemotCMCDog->curTicks <= uavRemotCMCDog->nextProcessTicks)
	{
		/*���ͨ�ųɹ�*/
		uavStatus->WIRELESS_CMC_STATUS = UAV_WIRELESS_CMC_SUCC;
		
		/*ͨ�ųɹ���,���±���Զ�����δ���� �� �ָ����Զ�����ǰ����ģʽ*/
		if (g_psControlAircraft->GO_HOME_SET == CTRL_AIRCRAFT_GO_HOME_SET)
		{
			/*���η���ģʽΪʧ��ǰģʽ(���ڴ�ʧ�����ָ�ͨѶ)*/
			uavStatus->UavFlyType.CURRENT = uavStatus->UavFlyType.BEFORE_WIRELESS_MISS;
			
			/*��յ�ǰ��������*/
			control_fly_mission_clear(&gs_Uav_Fly_Mission, gs_Uav_Fly_Mission.CURRENT_FLY_MISSION);
			
			/*����Զ�����δ�趨*/
			g_psControlAircraft->GO_HOME_SET = CTRL_AIRCRAFT_GO_HOME_NOTSET;
		}
	}
	/*����;��(�����Զ�����)ʧ�ط���,������ģʽ��,��ֹ�л����Զ�����ģʽ*/
	else
	{
		/*���ͨ��ʧ��*/
		uavStatus->WIRELESS_CMC_STATUS = UAV_WIRELESS_CMC_FAIL;		
	
		/*�ж��Ƿ�����ʧ������:ʧ�� / ���ջ�������������ɿذ�*/
		if ((g_psControlAircraft->GO_HOME_STATUS == CTRL_AIRCRAFT_GO_HOME_ENABLE) && \
			(uavStatus->LOCK_STATUS == UAV_LOCK_NOT))
		{	
			/*�Զ����� �ϴη���״̬���� �� ��ǰ����״̬���� ֻ����һ��*/
			if (g_psControlAircraft->GO_HOME_SET == CTRL_AIRCRAFT_GO_HOME_NOTSET)
			{	
				/*��¼ʧ��ǰ�ķ���ģʽ,�ָ�ͨ�ź�,�����л�֮ǰģʽ*/
				uavStatus->UavFlyType.BEFORE_WIRELESS_MISS = uavStatus->UavFlyType.CURRENT;
		
				/*ʧ����,�����һ������,�Զ������Զ���������*/
				control_fly_mission_set(&gs_Uav_Fly_Mission, UAV_FLY_MISSION_ONEKEY_LAND_HOME, UAV_MISSION_ATTITUDE_CTRL_DISABLE);				
			
				/*����Զ��������趨*/
				g_psControlAircraft->GO_HOME_SET = CTRL_AIRCRAFT_GO_HOME_SET;
			}
		}
	}
	
	return (uavStatus->WIRELESS_CMC_STATUS);
}

/*ң����ɻ�ͨѶ����ι��*/
void security_Feed_CMC_Succ_Dog_10MS(u8 _10msFoc, SimulateWatchDog *uavRemotCMCDog)
{
	u32 feedCmcDogTimeX10Ms = 0;
	
	/*ң���������ʧ��ʱ��:500ms*/
	if (_10msFoc > 50)
	{
		_10msFoc = 50;
	}
	
	/*x X 10ms*/
	feedCmcDogTimeX10Ms = _10msFoc * CMC_DOG_FEED_FACTOR_10MS;
	
	uavRemotCMCDog->nextProcessTicks = my_GetTick();	     /*��ȡ��ǰtick, sys_GetTick 10ms��ȡһ��*/
	
	uavRemotCMCDog->nextProcessTicks += feedCmcDogTimeX10Ms; /*��ʱ*/
}
