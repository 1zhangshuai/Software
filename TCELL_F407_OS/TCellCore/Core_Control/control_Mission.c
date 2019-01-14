#include "control_Mission.h"

/*ι�����⹷*/
void feed_control_fly_mission_check_dog(u8 _10msFoc, SimulateWatchDog *mission_check_dog)
{
	u32 feedCheckDogTimeX10Ms = 0;
	
	/*ң���������������:5000ms*/
	if (_10msFoc > 250)
	{
		_10msFoc = 250;
	}
	
	/*x X 10ms*/
	feedCheckDogTimeX10Ms = _10msFoc * 10;
	mission_check_dog->nextProcessTicks = my_GetTick();	     	  /*��ȡ��ǰtick, sys_GetTick 10ms��ȡһ��*/
	mission_check_dog->nextProcessTicks += feedCheckDogTimeX10Ms; /*��ʱ*/	
}

/*��ȡ�����⹷���״̬*/
SIM_WATCH_DOG_STATUS get_control_fly_mission_dog_status(SimulateWatchDog *mission_check_dog)
{
	/*��ȡ��ǰtick _10ms*/
	mission_check_dog->curTicks = my_GetTick();
	
	/*���curTicks < nextProcessTicks,˵��ʱ��δ��*/
	if (mission_check_dog->curTicks < mission_check_dog->nextProcessTicks)
	{
		return SIM_WATCH_DOG_ALIVE;
	}
	else
	{
		return SIM_WATCH_DOG_DIED;
	}
}

/*=== ���úͻ�ȡ���� ===*/
/*����״̬����ʼ��*/
void control_fly_mission_machine_reset(Uav_Status *uavStatus, UAV_FLY_MISSION TARG_MISSION)
{
	switch(TARG_MISSION)
	{
		/*һ�����*/
		case UAV_FLY_MISSION_ONEKEY_FLY:
		{
			/*Ŀ��δ����*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS   = UAV_MISSION_TARG_SET_NO;
			
			/*����״̬:��*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_NO;
			
			/*�������δִ��*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.EXECUTE_STATUS = UAV_MISSION_EXECUTE_NOT;
		}break;
		
		/*һ������/����*/
		case UAV_FLY_MISSION_ONEKEY_LAND_HOME:
		{
			/*Ŀ��δ����*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.TARG_SET_STATUS   = UAV_MISSION_TARG_SET_NO;
			
			/*����״̬:��*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_NO;	

			/*�������δִ��*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.EXECUTE_STATUS = UAV_MISSION_EXECUTE_NOT;			
		}break;		
		
		/*����׷��*/
		case UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT:
		{
		
		}break;

		/*����Ѳ��*/
		case UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE:
		{
		
		}break;

		/*GPSд�뵱ǰ��λ*/
		case UAV_FLY_MISSION_GPS_WRITE_POS:
		{
		
		}break;	

		/*GPSѲ��*/
		case UAV_FLY_MISSION_GPS_PATROL_SKY:
		{
		
		}break;		

		default:break;
	}
}

/*�������*/
void control_fly_mission_clear(Uav_Status *uavStatus, UAV_FLY_MISSION TARG_MISSION)
{
	/*��λ��ǰ�����״̬��*/
	control_fly_mission_machine_reset(uavStatus, TARG_MISSION);
	
	/*�ָ������ϵͳ�ĸı�*/
	switch(TARG_MISSION)
	{
		/*һ�����*/
		case UAV_FLY_MISSION_ONEKEY_FLY:
		{
			g_psPidSystem->HighAcc.PidScale.kP      = 1.0f;
			g_psPidSystem->HighAcc.PidScale.kI      = 1.0f;		
			g_psPidSystem->HighSpeed.PidScale.kP    = 1.0f;
			g_psPidSystem->HighSpeed.PidScale.kI    = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kP = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kI = 1.0f;
		}break;
		
		/*һ������/����*/
		case UAV_FLY_MISSION_ONEKEY_LAND_HOME:
		{
			
		}break;		
		
		/*����׷��*/
		case UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT:
		{
		
		}break;

		/*����Ѳ��*/
		case UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE:
		{
		
		}break;

		/*GPSд�뵱ǰ��λ*/
		case UAV_FLY_MISSION_GPS_WRITE_POS:
		{
		
		}break;	

		/*GPSѲ��*/
		case UAV_FLY_MISSION_GPS_PATROL_SKY:
		{
		
		}break;		

		default:break;
	}	
	
	/*��ǵ�ǰ����ΪNULL*/
	uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION = UAV_FLY_MISSION_NULL;
}

/*�����һ������,���õ�ǰ����*/
void control_fly_mission_set(Uav_Status *uavStatus, UAV_FLY_MISSION SET_MISSION, UAV_MISSION_ATTITUDE_CTRL_STATUS ATTITUDE_CTRL_STATUS)
{
	/*����ϸ������״̬*/
	control_fly_mission_clear(uavStatus, uavStatus->UavCurrentFlyMission.LAST_FLY_MISSION);	
	
	/*���õ�ǰ����*/
	uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION = SET_MISSION;
	
	/*�����Ƿ�����ͬʱִ����̬����*/
	uavStatus->UavCurrentFlyMission.ATTITUDE_CTRL_STATUS = ATTITUDE_CTRL_STATUS;
	
	/*������״̬��ʼ��*/
	control_fly_mission_machine_reset(uavStatus, SET_MISSION);
}

/*��ȡ��ǰ����*/
UAV_FLY_MISSION control_fly_mission_get(void)
{
	return (g_psUav_Status->UavCurrentFlyMission.CURRENT_FLY_MISSION);
}

/*=== �������� ===*/
/*һ�����*/
void control_fly_mission_onekey_fly(Uav_Status *uavStatus)
{
	/*��ֱ���ߴ�������Ч,�Ҹ߶�Ŀ���δ����*/
	if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE != UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
	    (uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_NO))
	{
		/*����һ�����������ͣ�߶�*/
		g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z] + CTRL_HEIGHT_ONEKEY_TAKEOFF_HEIGHT;
		
		/*���һ������ο���������*/
		uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS = UAV_MISSION_TARG_SET_OK;
		
		/*����ģʽ���Ϊ����*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT; /*= 2->����*/
		
		/*��ֱ����ģʽΪ:���߿���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_FIX_HEIGHT;
	}
	/*�ж������ʱ:��ֱ���ߴ�������Ч,�Ҹ߶�Ŀ���δ����*/
	else if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
			 (uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_NO))
	{
		/*����ģʽ���Ϊ��̬ģʽ*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			
		/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;	

		/*һ���������ʧ��,�������*/
		control_fly_mission_clear(uavStatus, UAV_FLY_MISSION_ONEKEY_FLY);
	}
	/*�ж������״̬��:��ֱ���ߴ�������Ч,�������ø߶�Ŀ���(����������)*/
	else if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
			 (uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_OK))
	{
		/*����ģʽ���Ϊ��̬ģʽ*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/				
			
		/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;
		
		/*һ���������ʧ��,�������*/
		control_fly_mission_clear(uavStatus, UAV_FLY_MISSION_ONEKEY_FLY);
	}
	
	/*����ģʽΪ���߿���(���ڶ����������) & Ŀ��������� & ��δ����Ŀ���*/
	if ((uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) && \
		(uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_OK) && \
		(uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS == UAV_MISSION_TARG_REACH_NO))
	{
		/*��ʱ������ֱ���Ƶ�PID����,ʵ�ֿ������*/
		g_psPidSystem->HighAcc.PidScale.kP      = 1.0f;
		g_psPidSystem->HighAcc.PidScale.kI      = 1.5f;		
		g_psPidSystem->HighSpeed.PidScale.kP    = 1.2f;
		g_psPidSystem->HighSpeed.PidScale.kI    = 1.2f;
		g_psPidSystem->HighPosition.PidScale.kP = 1.5f;
		g_psPidSystem->HighPosition.PidScale.kI = 1.0f;		
		
		/*��ǰ�߶ȴ��������߶�,�������ָ�����*/
		if (g_psPidSystem->HighPosition.expect <= g_psSinsReal->curPosition[EARTH_FRAME_Z])
		{
			/*����Ѵ�Ŀ���*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_OK;
			
			/*�ָ��������Ʊ�������*/
			g_psPidSystem->HighAcc.PidScale.kP      = 1.0f;
			g_psPidSystem->HighAcc.PidScale.kI      = 1.0f;		
			g_psPidSystem->HighSpeed.PidScale.kP    = 1.0f;
			g_psPidSystem->HighSpeed.PidScale.kI    = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kP = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kI = 1.0f;				
			
			/*���������*/
			control_fly_mission_clear(uavStatus, UAV_FLY_MISSION_ONEKEY_FLY);
		}
	}
}

/*һ��/ʧ������/����*/
void control_fly_mission_gohome(void)
{

}

/*=== ��Ҫ������+������������� ===*/
Control_Fly_Mission_Switch_Motion gs_mission_check_ultr_opflow = 
{
	.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO,
	.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO,
	.COUNTDOWN_STATUS     = CONTROL_MISSION_COUNTDOWN_FINISH,
	.CHECK_STATUS         = CONTROL_MISSION_START_CHECK_DISABLE, /*Ĭ��ʧ�ܼ��*/
	.edge_cnt     	 	  = 0,
	.count_down      	  = {0},
};

/*������*/
UAV_FLY_MISSION control_fly_mission_check_ultr_opflow_pos(Uav_Status *uavStatus, u32 checkPeriodMS)
{
	/*�������л��쳣,Ĭ�Ϸ��ص�ǰ����ִ�е�����*/
	UAV_FLY_MISSION FLY_MISSION_RET = uavStatus->UavCurrentFlyMission.LAST_FLY_MISSION;
	
	/*���SWD����λ��*/
	if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		if (gs_mission_check_ultr_opflow.CHECK_STATUS == CONTROL_MISSION_START_CHECK_ENABLE)
		{
			gs_mission_check_ultr_opflow.LOW_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_OK;
		}
	}
	else if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{	
		/*�ж��Ƿ�����һ�ּ��*/
		if (gs_mission_check_ultr_opflow.COUNTDOWN_STATUS == CONTROL_MISSION_COUNTDOWN_FINISH)
		{
			/*ʹ�ܼ��*/
			gs_mission_check_ultr_opflow.CHECK_STATUS = CONTROL_MISSION_START_CHECK_ENABLE;
			
			/*��ʼ����ʱ250 * 20ms = 5000ms*/
			feed_control_fly_mission_check_dog(checkPeriodMS / 20 / 10, &gs_mission_check_ultr_opflow.count_down);
			
			/*����ѿ�ʼ����ʱ*/
			gs_mission_check_ultr_opflow.COUNTDOWN_STATUS = CONTROL_MISSION_COUNTDOWN_START;
		}
		
		if (gs_mission_check_ultr_opflow.CHECK_STATUS == CONTROL_MISSION_START_CHECK_ENABLE)
		{
			gs_mission_check_ultr_opflow.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_OK;
		}
	}
	
	if ((gs_mission_check_ultr_opflow.LOW_EDGE_AVA_STATUS == REMOT_SWITCH_EDGE_AVA_OK) && \
		(gs_mission_check_ultr_opflow.HIGH_EDGE_AVA_STATUS == REMOT_SWITCH_EDGE_AVA_OK))
	{
		/*��������++*/
		gs_mission_check_ultr_opflow.edge_cnt++;
		
		gs_mission_check_ultr_opflow.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO;
		gs_mission_check_ultr_opflow.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO;		
	}
	
	/*���ڵ���ʱ״̬,�ҵ���ʱ�ѵ�ʱ��*/
	if ((get_control_fly_mission_dog_status(&gs_mission_check_ultr_opflow.count_down) == SIM_WATCH_DOG_DIED) && \
		(gs_mission_check_ultr_opflow.COUNTDOWN_STATUS == CONTROL_MISSION_COUNTDOWN_START))
	{
		/*���ݲ���������������������*/
		if (gs_mission_check_ultr_opflow.edge_cnt == MISSION_OPFLOW_FOLLOW_POINT)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT;	/*����׷��*/
		}
		else if (gs_mission_check_ultr_opflow.edge_cnt == MISSION_OPFLOW_FOLLOW_LINE)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE;	/*����ѭ��*/
		}
		
		/*���ֵ���ʱ����*/
		gs_mission_check_ultr_opflow.COUNTDOWN_STATUS = CONTROL_MISSION_COUNTDOWN_FINISH;
		
		/*ʧ�ܼ��*/
		gs_mission_check_ultr_opflow.CHECK_STATUS = CONTROL_MISSION_START_CHECK_DISABLE;
		
		/*cnt��0*/
		gs_mission_check_ultr_opflow.edge_cnt = 0;
	}
	
	return FLY_MISSION_RET;
}

/*����׷��*/
void control_fly_mission_ultr_opflow_follow_point(void)
{
	
}

/*����ѭ��*/
void control_fly_mission_ultr_opflow_follow_line(void)
{

}

/*=== ��ҪGPS+��ѹ�ƶ�������� ===*/
Control_Fly_Mission_Switch_Motion gs_mission_check_bero_gps = 
{
	.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO,
	.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO,
	.COUNTDOWN_STATUS     = CONTROL_MISSION_COUNTDOWN_FINISH,
	.CHECK_STATUS         = CONTROL_MISSION_START_CHECK_DISABLE, /*Ĭ��ʧ�ܼ��*/
	.edge_cnt     	 	  = 0,
	.count_down      	  = {0},
};

/*������*/
UAV_FLY_MISSION control_fly_mission_check_bero_gps_pos(Uav_Status *uavStatus, u32 checkPeriodMS)
{
	/*�������л��쳣,Ĭ�Ϸ��ص�ǰ����ִ�е�����*/	
	UAV_FLY_MISSION FLY_MISSION_RET = uavStatus->UavCurrentFlyMission.LAST_FLY_MISSION;
	
	/*���SWD����λ��*/
	if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		if (gs_mission_check_bero_gps.CHECK_STATUS == CONTROL_MISSION_START_CHECK_ENABLE)
		{
			gs_mission_check_bero_gps.LOW_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_OK;
		}
	}
	else if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{	
		/*�ж��Ƿ�����һ�ּ��*/
		if (gs_mission_check_bero_gps.COUNTDOWN_STATUS == CONTROL_MISSION_COUNTDOWN_FINISH)
		{
			/*ʹ�ܼ��*/
			gs_mission_check_bero_gps.CHECK_STATUS = CONTROL_MISSION_START_CHECK_ENABLE;
			
			/*��ʼ����ʱ250 * 20ms = 5000ms*/
			feed_control_fly_mission_check_dog(checkPeriodMS / 20 / 10, &gs_mission_check_ultr_opflow.count_down);
			
			/*����ѿ�ʼ����ʱ*/
			gs_mission_check_bero_gps.COUNTDOWN_STATUS = CONTROL_MISSION_COUNTDOWN_START;
		}
		
		if (gs_mission_check_bero_gps.CHECK_STATUS == CONTROL_MISSION_START_CHECK_ENABLE)
		{
			gs_mission_check_bero_gps.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_OK;
		}
	}
	
	if ((gs_mission_check_bero_gps.LOW_EDGE_AVA_STATUS == REMOT_SWITCH_EDGE_AVA_OK) && \
		(gs_mission_check_bero_gps.HIGH_EDGE_AVA_STATUS == REMOT_SWITCH_EDGE_AVA_OK))
	{
		/*��������++*/
		gs_mission_check_bero_gps.edge_cnt++;
		
		gs_mission_check_bero_gps.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO;
		gs_mission_check_bero_gps.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO;		
	}
	
	/*���ڵ���ʱ״̬,�ҵ���ʱ�ѵ�ʱ��*/
	if ((get_control_fly_mission_dog_status(&gs_mission_check_bero_gps.count_down) == SIM_WATCH_DOG_DIED) && \
		(gs_mission_check_bero_gps.COUNTDOWN_STATUS == CONTROL_MISSION_COUNTDOWN_START))
	{
		/*���ݲ���������������������*/
		if (gs_mission_check_bero_gps.edge_cnt == MISSION_OPFLOW_FOLLOW_POINT)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_GPS_WRITE_POS;	/*GPSд�뵱ǰ����*/
		}
		else if (gs_mission_check_bero_gps.edge_cnt == MISSION_OPFLOW_FOLLOW_LINE)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_GPS_PATROL_SKY;	/*��д���GPS�������Ѳ��*/
		}
		
		/*���ֵ���ʱ����*/
		gs_mission_check_bero_gps.COUNTDOWN_STATUS = CONTROL_MISSION_COUNTDOWN_FINISH;
		
		/*ʧ�ܼ��*/
		gs_mission_check_bero_gps.CHECK_STATUS = CONTROL_MISSION_START_CHECK_DISABLE;
		
		/*cnt��0*/
		gs_mission_check_bero_gps.edge_cnt = 0;
	}
	
	return FLY_MISSION_RET;
}

/*GPS��¼��ǰ����*/
void control_fly_mission_bero_gps_write_pos(Uav_Status *uavStatus)
{
	/*�ж��Ƿ����д��*/
	
	/*��GPS��λд�����,�������һ������,�������*/
	control_fly_mission_set(uavStatus, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
}

/*GPSѲ��*/
void control_fly_mission_bero_gps_patrol_sky(Uav_Status *uavStatus)
{
	/**/
}

/*=== ��ҪOPENMV������ ===*/
