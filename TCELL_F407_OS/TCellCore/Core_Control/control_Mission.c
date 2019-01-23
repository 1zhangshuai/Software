#include "control_Mission.h"

Uav_Fly_Mission gs_Uav_Fly_Mission = 
{
	.CURRENT_FLY_MISSION  = UAV_FLY_MISSION_NULL,			   /*��ǰ��������*/
	.LAST_FLY_MISSION     = UAV_FLY_MISSION_NULL,			   /*�ϴη�������*/
	.ATTITUDE_CTRL_STATUS = UAV_MISSION_ATTITUDE_CTRL_DISABLE, /*�Ƿ�����ִ������ͬʱִ����̬����*/
	.CLEAR_SWITCH_STATUS  = MISSION_CLEAR_SWITCH_DISABLE,	   /*��Ϊң��Ĭ��SWC�ڵ�λ,����Ĭ��ʹ�������������*/
	
	/*һ������*/
	.Onekey_Mission =
	{
		.FixedHeightFly = 
		{
			.ENABLE_STATUS        = UAV_MISSION_DISABLE,
			.TARG_SET_STATUS      = UAV_MISSION_TARG_SET_NO,
			.TARG_REACH_STATUS    = UAV_MISSION_TARG_REACH_NO,
			.EXECUTE_STATUS       = UAV_MISSION_EXECUTE_NOT,
		},
		
		.LandHome = 
		{
			.ENABLE_STATUS        = UAV_MISSION_DISABLE,
			.TARG_SET_STATUS      = UAV_MISSION_TARG_SET_NO,
			.TARG_REACH_STATUS    = UAV_MISSION_TARG_REACH_NO,		
			.EXECUTE_STATUS       = UAV_MISSION_EXECUTE_NOT,				
		},
	},

	/*д��GPS������*/
	.Write_Gps_Mission = 
	{
		.index       = 0,
		.posTotalNbr = 0,
		
		.Limitless_Pos =
		{
			(UAV_MISSION_TARG_SET_STATUS)0, 	/*UAV_MISSION_TARG_SET_NO = 0, Ŀ���δ����*/
		}
	}
};

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
void control_fly_mission_machine_reset(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION TARG_MISSION)
{
	switch(TARG_MISSION)
	{
		/*һ�����*/
		case UAV_FLY_MISSION_ONEKEY_FLY:
		{
			/*Ŀ��δ����*/
			uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS   = UAV_MISSION_TARG_SET_NO;
			
			/*����״̬:��*/
			uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_NO;
			
			/*�������δִ��*/
			uav_fly_mission->Onekey_Mission.FixedHeightFly.EXECUTE_STATUS = UAV_MISSION_EXECUTE_NOT;
		}break;
		
		/*һ������/����*/
		case UAV_FLY_MISSION_ONEKEY_LAND_HOME:
		{
			/*Ŀ��δ����*/
			uav_fly_mission->Onekey_Mission.LandHome.TARG_SET_STATUS   = UAV_MISSION_TARG_SET_NO;
			
			/*����״̬:��*/
			uav_fly_mission->Onekey_Mission.LandHome.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_NO;	

			/*�������δִ��*/
			uav_fly_mission->Onekey_Mission.LandHome.EXECUTE_STATUS = UAV_MISSION_EXECUTE_NOT;			
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
void control_fly_mission_clear(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION TARG_MISSION)
{
	/*��λ��ǰ�����״̬��*/
	control_fly_mission_machine_reset(uav_fly_mission, TARG_MISSION);
	
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
	uav_fly_mission->CURRENT_FLY_MISSION = UAV_FLY_MISSION_NULL;
}

/*�����һ������,���õ�ǰ����*/
void control_fly_mission_set(Uav_Fly_Mission *uav_fly_mission, UAV_FLY_MISSION SET_MISSION, UAV_MISSION_ATTITUDE_CTRL_STATUS ATTITUDE_CTRL_STATUS)
{
	/*����ϸ������״̬*/
	control_fly_mission_clear(uav_fly_mission, uav_fly_mission->LAST_FLY_MISSION);	
	
	/*���õ�ǰ����*/
	uav_fly_mission->CURRENT_FLY_MISSION = SET_MISSION;
	
	/*�����Ƿ�����ͬʱִ����̬����*/
	uav_fly_mission->ATTITUDE_CTRL_STATUS = ATTITUDE_CTRL_STATUS;
	
	/*������״̬��ʼ��*/
	control_fly_mission_machine_reset(uav_fly_mission, SET_MISSION);
}

/*��ȡ��ǰ����*/
UAV_FLY_MISSION control_fly_mission_get(void)
{
	return (gs_Uav_Fly_Mission.CURRENT_FLY_MISSION);
}

/*======================== �������� ========================*/
/*1.һ�����*/
void control_fly_mission_onekey_fly(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{	
	/*��ֱ���ߴ�������Ч,�Ҹ߶�Ŀ���δ����*/
	if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE != UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
	    (uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_NO))
	{
		/*����һ�����������ͣ�߶�*/
		g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z] + CTRL_HEIGHT_ONEKEY_TAKEOFF_HEIGHT;
		
		/*���һ������ο���������*/
		uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS = UAV_MISSION_TARG_SET_OK;
		
		/*����ģʽ���Ϊ����*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT; /*= 2->����*/
		
		/*��ֱ����ģʽΪ:���߿���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_FIX_HEIGHT;
	}
	/*�ж������ʱ:��ֱ���ߴ�������Ч,�Ҹ߶�Ŀ���δ����*/
	else if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
			 (uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_NO))
	{
		/*����ģʽ���Ϊ��̬ģʽ*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			
		/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;	

		/*һ���������ʧ��,�������*/
		control_fly_mission_clear(uav_fly_mission, UAV_FLY_MISSION_ONEKEY_FLY);
	}
	/*�ж������״̬��:��ֱ���ߴ�������Ч,�������ø߶�Ŀ���(����������)*/
	else if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_NULL) && \
			 (uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_OK))
	{
		/*����ģʽ���Ϊ��̬ģʽ*/
		uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/				
			
		/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
		uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;
		
		/*һ���������ʧ��,�������*/
		control_fly_mission_clear(uav_fly_mission, UAV_FLY_MISSION_ONEKEY_FLY);
	}
	
	/*����ģʽΪ���߿���(���ڶ����������) & Ŀ��������� & ��δ����Ŀ���*/
	if ((uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) && \
		(uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_SET_STATUS == UAV_MISSION_TARG_SET_OK) && \
		(uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS == UAV_MISSION_TARG_REACH_NO))
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
			uav_fly_mission->Onekey_Mission.FixedHeightFly.TARG_REACH_STATUS = UAV_MISSION_TARG_REACH_OK;
			
			/*�ָ��������Ʊ�������*/
			g_psPidSystem->HighAcc.PidScale.kP      = 1.0f;
			g_psPidSystem->HighAcc.PidScale.kI      = 1.0f;		
			g_psPidSystem->HighSpeed.PidScale.kP    = 1.0f;
			g_psPidSystem->HighSpeed.PidScale.kI    = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kP = 1.0f;
			g_psPidSystem->HighPosition.PidScale.kI = 1.0f;				
			
			/*���������*/
			control_fly_mission_clear(uav_fly_mission, UAV_FLY_MISSION_ONEKEY_FLY);
		}
	}
}

/*2.һ��/ʧ������/����*/
void control_fly_mission_auto_gohome(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus, fp32 controlDeltaT)
{	
	ctrl_Go_Home_Control(controlDeltaT);
}
/*==================================================================*/



/*======================== ��Ҫ������+������������� ========================*/
Control_Fly_Mission_Switch_Motion gs_mission_check_ultr_opflow = 
{
	.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO,
	.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO,
	.COUNTDOWN_STATUS     = CONTROL_MISSION_COUNTDOWN_FINISH,
	.CHECK_STATUS         = CONTROL_MISSION_START_CHECK_DISABLE, /*Ĭ��ʧ�ܼ��*/
	.edge_cnt     	 	  = 0,
	.count_down      	  = {0},
};

/*1.������*/
UAV_FLY_MISSION control_fly_mission_check_ultr_opflow_pos(Uav_Fly_Mission *uav_fly_mission, u32 checkPeriodMS)
{
	UAV_FLY_MISSION FLY_MISSION_RET = UAV_FLY_MISSION_NULL;
	
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

/*2.����׷��*/
void control_fly_mission_ultr_opflow_follow_point(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{
	/*�жϿ���ģʽ��ʹ�ô������Ƿ����������*/
	if (check_uav_ctrl_and_sensor_use(uavStatus, \
						              UAV_FLY_TYPE_FIX_POS, \
	                                  UAV_VERTICAL_SENMOD_CURRENT_ULTR, \
	                                  UAV_HORIZONTAL_SENMOD_CURRENT_OPTICFLOW) != SYS_BOOL_TRUE)
	{
		return;
	}
	
	control_fly_mission_set(uav_fly_mission, UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
}

/*3.����ѭ��*/
void control_fly_mission_ultr_opflow_follow_line(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{
	/*�жϿ���ģʽ��ʹ�ô������Ƿ����������*/
	if (check_uav_ctrl_and_sensor_use(uavStatus, \
						              UAV_FLY_TYPE_FIX_POS, \
	                                  UAV_VERTICAL_SENMOD_CURRENT_ULTR, \
	                                  UAV_HORIZONTAL_SENMOD_CURRENT_OPTICFLOW) != SYS_BOOL_TRUE)
	{
		return;
	}	
	
	control_fly_mission_set(uav_fly_mission, UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
}
/*==================================================================*/



/*======================== ��ҪGPS+��ѹ�ƶ�������� ========================*/
Control_Fly_Mission_Switch_Motion gs_mission_check_bero_gps = 
{
	.HIGH_EDGE_AVA_STATUS = REMOT_SWITCH_EDGE_AVA_NO,
	.LOW_EDGE_AVA_STATUS  = REMOT_SWITCH_EDGE_AVA_NO,
	.COUNTDOWN_STATUS     = CONTROL_MISSION_COUNTDOWN_FINISH,
	.CHECK_STATUS         = CONTROL_MISSION_START_CHECK_DISABLE, /*Ĭ��ʧ�ܼ��*/
	.edge_cnt     	 	  = 0,
	.count_down      	  = {0},
};

/*1.������*/
UAV_FLY_MISSION control_fly_mission_check_bero_gps_pos(Uav_Fly_Mission *uav_fly_mission, u32 checkPeriodMS)
{
	UAV_FLY_MISSION FLY_MISSION_RET = UAV_FLY_MISSION_NULL;
	
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
			feed_control_fly_mission_check_dog(checkPeriodMS / 20 / 10, &gs_mission_check_bero_gps.count_down);
			
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
		if (gs_mission_check_bero_gps.edge_cnt == MISSION_GPS_WRITE_LIMITLESS_POS)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_GPS_WRITE_POS;	/*GPSд�뵱ǰ����*/
		}
		else if (gs_mission_check_bero_gps.edge_cnt == MISSION_GPS_PATROL_SKY)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_GPS_PATROL_SKY;	/*��д���GPS�������Ѳ��*/
		}
		else if (gs_mission_check_bero_gps.edge_cnt == MISSION_GPS_CLEAR_ALL_LIMITLESS_POS)
		{
			FLY_MISSION_RET = UAV_FLY_MISSION_GPS_CLEAR_ALL_POS; /*������м�¼��*/
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

/*2.GPS��¼��ǰ����*/
void control_fly_mission_bero_gps_write_pos(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{
	/*�жϿ���ģʽ��ʹ�ô������Ƿ����������*/
	if (check_uav_ctrl_and_sensor_use(uavStatus, \
						              UAV_FLY_TYPE_FIX_POS, \
	                                  UAV_VERTICAL_SENMOD_CURRENT_BERO, \
	                                  UAV_HORIZONTAL_SENMOD_CURRENT_GPS) != SYS_BOOL_TRUE)
	{
		return;
	}	
	
	/*�ж��Ƿ񻹿�д��,�Ѵ�����,����д��*/
	if (uav_fly_mission->Write_Gps_Mission.posTotalNbr >= MISSION_LIMITLESS_POS_WRITE_MAX_NBR)
	{
		return;
	}
	
	/*�жϵ�ǰ�Ƿ���3ά��ͣ״̬*/
	if (uavStatus->AIRSTOP_TYPE == UAV_AIRSTOP_BOTH_V_H)
	{
		/*д�뵱ǰλ��*/
		uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.east  = g_psAttitudeAll->GpsData.Coordinate_s4.lon;
		uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.north = g_psAttitudeAll->GpsData.Coordinate_s4.lat;
		uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.up    = g_psSinsReal->curPosition[EARTH_FRAME_Z];
	
		/*��������������*/
		uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].SET_STATUS = UAV_MISSION_TARG_SET_OK;
		
		/*index++*/
		uav_fly_mission->Write_Gps_Mission.index++;
		
		/*posTotalNbr++*/
		uav_fly_mission->Write_Gps_Mission.posTotalNbr++;
	}
	
	/*�����ǰ����*/
	control_fly_mission_set(uav_fly_mission, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
}

/*GPS������м�¼������*/
void control_fly_mission_bero_gps_clear_all_pos(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{
	u8 i;	
	
	/*�ж��ܵ�λ�Ƿ�Ϊ0*/
	if (uav_fly_mission->Write_Gps_Mission.posTotalNbr > 0)
	{
		for (i = 0; i < MISSION_LIMITLESS_POS_WRITE_MAX_NBR; i++)
		{
			/*���λ�����ݼ�¼*/
			uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.east  = 0;
			uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.north = 0;
			uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].Pos.up    = 0;
	
			/*��������δ����*/
			uav_fly_mission->Write_Gps_Mission.Limitless_Pos[uav_fly_mission->Write_Gps_Mission.index].SET_STATUS = UAV_MISSION_TARG_SET_NO;			
		}
		
		/*index = 0*/
		uav_fly_mission->Write_Gps_Mission.index = 0;
		
		/*posTotalNbr = 0*/
		uav_fly_mission->Write_Gps_Mission.posTotalNbr = 0;		
	}

	/*�����ǰ����*/
	control_fly_mission_set(uav_fly_mission, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
}

/*3.GPSѲ��*/
void control_fly_mission_bero_gps_patrol_sky(Uav_Fly_Mission *uav_fly_mission, Uav_Status *uavStatus)
{
	/*�жϿ���ģʽ��ʹ�ô������Ƿ����������*/
	if (check_uav_ctrl_and_sensor_use(uavStatus, \
						              UAV_FLY_TYPE_FIX_POS, \
	                                  UAV_VERTICAL_SENMOD_CURRENT_BERO, \
	                                  UAV_HORIZONTAL_SENMOD_CURRENT_GPS) != SYS_BOOL_TRUE)
	{
		return;
	}
	
	/*��¼3����3�����ϵ�λ,���ܽ���Ѳ������*/
	if (uav_fly_mission->Write_Gps_Mission.posTotalNbr < 3)
	{
		return;
	}
		
	/*Ѳ������*/
	
	control_fly_mission_set(uav_fly_mission, UAV_FLY_MISSION_GPS_PATROL_SKY, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
	
}
/*==================================================================*/



/*======================== ��ҪOPENMV������ ========================*/
/*==================================================================*/
