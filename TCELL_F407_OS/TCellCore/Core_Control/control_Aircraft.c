#include "control_Aircraft.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

CTRL_SysStatus g_sCtrlSysStatus = 
{
	.pid  = CTRL_SYSTEM_PARA_INIT_FAIL,
	.adrc = CTRL_SYSTEM_PARA_INIT_FAIL,
};

CTRL_SysStatus *g_psCtrlSysStatus = &g_sCtrlSysStatus;

ControlAircraft g_sControlAircraft = 
{
	.heightHoldThrottle = 0,       	/*�߶ȱ�������(���ڶ���)*/
	.RemotExpectAngle   = {0},      /*ң��ֵת��Ϊ�����Ƕ�*/
	.CurMotorPwmOutput  = {0},		/*PWM��������*/
	.LastMotorPwmOutput = {0},
	.GO_HOME_STATUS     = CTRL_AIRCRAFT_GO_HOME_DISABLE,	/*�ϵ��(��½״̬)������ʧ���Զ�����*/
	.GO_HOME_SET        = CTRL_AIRCRAFT_GO_HOME_NOTSET,     /*�Զ���������״̬*/
};

ControlAircraft *g_psControlAircraft = &g_sControlAircraft;

/*�����㷨������ʼ��*/
SYS_RETERR ctrl_autocontrol_para_init(CTRL_SysStatus *sysStatus)
{
	SYS_RETERR errRet = SYS_RETERR_FF;
	
/*PID*/
#if (CONTROL_SYS__ONLY_PID == SYS_ENABLE)
	/*PID Init*/
	sysStatus->pid = (CTRL_SYSTEM_PARA_INIT_STATUS)pid_Parameter_Read_And_Init(&g_sPidSystem);
#endif	

/*ADRC*/
#if (CONTROL_SYS__ONLY_ADRC == SYS_ENABLE)
	/*ADRC Init*/
	sysStatus->adrc = (CTRL_SYSTEM_PARA_INIT_STATUS)adrc_Parameter_Read_And_Init(&g_sPidSystem);
#endif	
	
/*PID + ADRC*/
#if (CONTROL_SYS__PID_ADRC == SYS_ENABLE)
	/*PID Init*/
	sysStatus->pid = (CTRL_SYSTEM_PARA_INIT_STATUS)pid_Parameter_Read_And_Init(&g_sPidSystem);	
	
	/*ADRC Init*/	
	sysStatus->adrc = (CTRL_SYSTEM_PARA_INIT_STATUS)adrc_Parameter_Read_And_Init(&g_sPidSystem);
#endif
	
	if ((sysStatus->pid == CTRL_SYSTEM_PARA_INIT_SUCC) && (sysStatus->adrc == CTRL_SYSTEM_PARA_INIT_SUCC))
	{
		errRet = SYS_RETERR_0ZR;	/*�����㷨:PID+ADRC*/
	}
	else if ((sysStatus->pid == CTRL_SYSTEM_PARA_INIT_FAIL) && (sysStatus->adrc == CTRL_SYSTEM_PARA_INIT_SUCC))
	{
		errRet = SYS_RETERR_1ST;		
	}
	else if ((sysStatus->pid == CTRL_SYSTEM_PARA_INIT_SUCC) && (sysStatus->adrc == CTRL_SYSTEM_PARA_INIT_FAIL))
	{
		errRet = SYS_RETERR_2ND;	/*�����㷨:PID*/	
	}	
	else if ((sysStatus->pid == CTRL_SYSTEM_PARA_INIT_FAIL) && (sysStatus->adrc == CTRL_SYSTEM_PARA_INIT_FAIL))
	{
		errRet = SYS_RETERR_3RD;
	}	
	
	return errRet;
}

/*������ģʽ����Ƶ��*/
vu8 g_CtrlCheckControlModeTicks = 0;

/*�������Զ�����ϵͳ*/
void ctrl_auto_control_system_dp(void)
{	
	/*����ģʽѡ��*/
	g_CtrlCheckControlModeTicks++;
	
	if (g_CtrlCheckControlModeTicks >= (20 / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)) /*ÿ20ms���һ��*/
	{
		g_CtrlCheckControlModeTicks = 0;		
		
		/*����ģʽ����������ѡ��*/
		ctrl_Control_Mode_Select(g_psUav_Status);
	}
	
	/************* 1.����������: ����+���ˮƽ������ *************/
	ctrl_MainLeading_Control_Dp();
	
	/************* 2.��̬������: �Ƕ��⻷+���ٶ��ڻ� *************/
	ctrl_Attitude_Control_Dp();
}

/*����ģʽ����������ѡ��*/
void ctrl_Control_Mode_Select(Uav_Status *uavStatus)
{	
	/*�ϴ�ң�������߶ȿ���ģʽ*/
	uavStatus->UavControlMode.Vertical.Mode_Switch.LastTime = uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime;
	
	/*�ϴ�ң������ˮƽλ�ÿ���ģʽ*/
	uavStatus->UavControlMode.Horizontal.Mode_Switch.LastTime = uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime;
	
	/*�ϴη����������*/
	uavStatus->UavCurrentFlyMission.LAST_FLY_MISSION = uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION;
	
	/*============ �ж�ң����ģʽѡ���Ӧ������ֵ,������ģʽ�л� ============*/
	/*SWITCH A Ĭ���ϲ�Ϊ����̬����,�²�Ϊ����*/
	if (remot_Data_Range(g_sRemotData.SWA, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime = UAV_VERTICAL_CONTROL_SELFAUTO; /*��ֱ��̬����ģʽģʽ*/
	}
	else if (remot_Data_Range(g_sRemotData.SWA, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{
		uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime = UAV_VERTICAL_CONTROL_FIX_HEIGHT; /*����������ģʽ*/		
	}

	/*SWITCH B Ĭ���ϲ�Ϊ����̬����,�²�Ϊˮƽ����*/
	if (remot_Data_Range(g_sRemotData.SWB, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime = UAV_HORIZONTAL_CONTROL_SELFAUTO;	/*ˮƽ��̬����ģʽģʽ*/	
	}
	else if (remot_Data_Range(g_sRemotData.SWB, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{
		uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime = UAV_HORIZONTAL_CONTROL_FIX_POS;  /*������ˮƽ����ģʽ*/
	}
	
	/*SWITCH C �ϲ�Ϊ�����ǰ���������̬����ģʽ,�в�Ϊһ�����,�²�Ϊһ������*/
	if (remot_Data_Range(g_sRemotData.SWC, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		/*������ʱ,�������,�Զ��л���ִ������ǰ��ģʽ*/
		if (uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION != UAV_FLY_MISSION_NULL)
		{
			/*��������ΪNULL*/
			control_fly_mission_set(uavStatus, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
		}
	}
	else if (remot_Data_Range(g_sRemotData.SWC, REMOT_DATA_MID) == REMOT_DATA_MID)
	{
		/*����Ƿ�����һ���������:ʹ�� & ��δִ�� & ����״̬*/
		if ((uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.ENABLE_STATUS == UAV_MISSION_ENABLE) && \
			(uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.EXECUTE_STATUS == UAV_MISSION_EXECUTE_NOT) && \
			(uavStatus->UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT))
		{
			/*�����л���һ�����,ʹ����̬����*/
			control_fly_mission_set(uavStatus, UAV_FLY_MISSION_ONEKEY_FLY, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
			
			/*���������ִ��,��ֹ�ظ���������״̬*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.EXECUTE_STATUS = UAV_MISSION_EXECUTE_YES;
		}
	}
	else if (remot_Data_Range(g_sRemotData.SWC, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{
		/*����Ƿ�����һ����������:ʹ�� & ��δִ�� & ����״̬*/		
		if ((uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.ENABLE_STATUS == UAV_MISSION_ENABLE) && \
			(uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.EXECUTE_STATUS == UAV_MISSION_EXECUTE_NOT) && \
			(uavStatus->UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT))
		{
			/*�����л���һ������,��ֹ��̬����*/
			control_fly_mission_set(uavStatus, UAV_FLY_MISSION_ONEKEY_LAND_HOME, UAV_MISSION_ATTITUDE_CTRL_DISABLE);

			/*���������ִ��,��ֹ�ظ���������״̬*/
			uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.EXECUTE_STATUS = UAV_MISSION_EXECUTE_YES;
		}
	}
	
	/*============ ����&����ң���л����� ============*/
	/*�жϸ߶Ⱥ�ˮƽ����ģʽ�Ƿ��л�*/
	/*�߶�:���ο���ģʽ���ϴο���ģʽ��ͬ*/
	if (uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime != uavStatus->UavControlMode.Vertical.Mode_Switch.LastTime)
	{
		/*�����ж���*/
		if (uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime == UAV_VERTICAL_CONTROL_FIX_HEIGHT)
		{
			/*�ж�SWITCH D����״̬��ѡ����ֱ���򴫸���*/
			if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MIN) == REMOT_DATA_MIN)
			{
				uavStatus->UavSenmodStatus.Vertical.CURRENT_USE = UAV_VERTICAL_SENMOD_CURRENT_USE_BERO;	/*��ѹ�����ݲ��붨��*/
			}
			else if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MAX) == REMOT_DATA_MAX)
			{
				uavStatus->UavSenmodStatus.Vertical.CURRENT_USE = UAV_VERTICAL_SENMOD_CURRENT_USE_ULTR;	/*���������ݲ��붨��*/
			}
			
			/*��Ǳ���ң������Ϊ����ģʽ*/
			uavStatus->UavControlMode.Vertical.EXPECT_CONTROL_MODE = UAV_VERTICAL_CONTROL_FIX_HEIGHT;
			
			/*�����ο���δ����(�л�����Ҫ���¼�¼��ǰ�߶�)*/
			uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO;
		}
		/*����������*/
		else if (uavStatus->UavControlMode.Vertical.Mode_Switch.ThisTime == UAV_VERTICAL_CONTROL_SELFAUTO)
		{
			/*������ߴ���������*/
			uavStatus->UavSenmodStatus.Vertical.CURRENT_USE = UAV_VERTICAL_SENMOD_CURRENT_USE_NULL;
			
			/*��Ǳ���ң������Ϊ����*/
			uavStatus->UavControlMode.Vertical.EXPECT_CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;
		}
	}

	/*ˮƽ:���ο���ģʽ���ϴο���ģʽ��ͬ*/
	if (uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime != uavStatus->UavControlMode.Horizontal.Mode_Switch.LastTime) 
	{
		/*������ˮƽ����*/
		if (uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime == UAV_HORIZONTAL_CONTROL_FIX_POS)
		{
			/*�ж�SWITCH D����״̬��ѡ����ֱ���򴫸���*/
			if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MIN) == REMOT_DATA_MIN)
			{
				uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE = UAV_HORIZONTAL_SENMOD_CURRENT_USE_GPS;			/*GPS���ݲ��붨��*/
			}
			else if (remot_Data_Range(g_sRemotData.SWD, REMOT_DATA_MAX) == REMOT_DATA_MAX)
			{
				uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE = UAV_HORIZONTAL_SENMOD_CURRENT_USE_OPTICFLOW;	/*�������ݲ��붨��*/
			}
			
			/*��Ǳ���Ϊ������ˮƽ����*/
			uavStatus->UavControlMode.Horizontal.EXPECT_CONTROL_MODE = UAV_HORIZONTAL_CONTROL_FIX_POS;
			
			/*�����ο���δ����(�л�����Ҫ���¼�¼��ǰˮƽλ��)*/
			uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO;			
		}
		/*ˮƽ����������*/
		else if (uavStatus->UavControlMode.Horizontal.Mode_Switch.ThisTime == UAV_HORIZONTAL_CONTROL_SELFAUTO)
		{
			/*������ߴ���������*/
			uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE = UAV_HORIZONTAL_SENMOD_CURRENT_USE_NULL;
			
			/*��Ǳ���ң������Ϊ����*/
			uavStatus->UavControlMode.Horizontal.EXPECT_CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;			
		}
	}

	/*������δʧ��,�Ž���ң�ظı䵱ǰ����״̬*/
	if (uavStatus->WIRELESS_CMC_STATUS == UAV_WIRELESS_CMC_SUCC)
	{
		/*============ ���������жϼ�����ģʽ�л� ============*/
		/*��ֱ�����ж���*/
		if (uavStatus->UavControlMode.Vertical.EXPECT_CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT)
		{
			/*��ѹ�ƺͳ�����������һ����Ч,�Ҹ߶Ȳο���δ����*/
			if (((uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_OK) || \
				 (uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_OK)) && \
			    (uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
			{
				/*ÿ���л�ֻ����һ�ε�ǰ����ֵ*/
				g_psControlAircraft->heightHoldThrottle = g_psControlAircraft->RemotExpectAngle.throttle;
		
				/*��Switch_SWA�������²���˲��Ĺߵ���ֱλ�ù�����ΪĿ��߶�*/
				g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
		
				/*���óɹ��� �����ο���������(��ֹ���ߺ�,���¸��������߶�)*/
				uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_OK;

				/*����ģʽ���Ϊ����*/
				uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT; /*= 2->����*/
		
				/*��ֱ����ģʽΪ:���߿���ģʽ*/
				uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_FIX_HEIGHT;
			}
			/*�ж���ʱ:��ѹ�ƺͳ���������Ч,��δ���ø߶Ȳο���*/
			else if (((uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
					  (uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_NO)) && \
					 (uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
			{
				/*����ģʽ���Ϊ��̬ģʽ*/
				uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			
				/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
				uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;		
			}
			/*����״̬��:��ѹ�ƺͳ���������Ч,�������ø߶Ȳο���(����������)*/
			else if (((uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
					  (uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_NO)) && \
					 (uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_OK))
			{
				/*����ģʽ���Ϊ��̬ģʽ*/
				uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/				

				/*�˳����ߺ�,��ǲο���δ����,������һֱcheck*/
				uavStatus->UavControlMode.Vertical.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO;
			
				/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
				uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;				
			}
		}
		else if (uavStatus->UavControlMode.Vertical.EXPECT_CONTROL_MODE == UAV_VERTICAL_CONTROL_SELFAUTO)	/*����������*/
		{
			/*����ģʽ���Ϊ��̬*/
			uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/	
		
			/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
			uavStatus->UavControlMode.Vertical.CONTROL_MODE = UAV_VERTICAL_CONTROL_SELFAUTO;			
		}
		
		/*============ ���������жϼ�����ģʽ�л� ============*/
		/*ˮƽ�����ж���,ˮƽ������Դ: ���� / GPS*/
		if (uavStatus->UavControlMode.Horizontal.EXPECT_CONTROL_MODE == UAV_HORIZONTAL_CONTROL_FIX_POS)
		{
			/*ˮƽ���㴫������GPS*/
			if (uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_USE_GPS)
			{
				/*(GPS����OK && HOME������ && �ںϳɹ�) && �ο���δ����*/
				if((status_GPS_Fix_Ava_Check(g_psUav_Status) == UAV_SENMOD_USE_CONTROL_ALLOW) && \
				   (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
				{
					/*����ǰ�ߵ�ˮƽλ�ù�����ΪĿ����ͣ��*/
					g_psPidSystem->LatitudePosition.expect  = g_psSinsReal->curPosition[EARTH_FRAME_Y]; /*N*/
					g_psPidSystem->LongitudePosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_X];	/*E*/
						
					/*PIDˮƽ���ƻ��ָ�λ*/
					pid_Horizontal_GPS_Ctrl_Integrate_Reset();
			
					/*���óɹ��� �����ο���������(��ֹ�����,���¸�������λ��)*/
					uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_OK;	
			
					/*����ģʽ���Ϊ����*/
					uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_POS;	/*= 3->����*/		
			
					/*ˮƽ����ģʽΪ:�������ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_FIX_POS;
				}
				/*�ж���ʱ��,����������,�Զ��л�ˮƽ����ģʽ,������check,ֱ���л�������ģʽ�������㶨������*/
				else if ((status_GPS_Fix_Ava_Check(g_psUav_Status) == UAV_SENMOD_USE_CONTROL_DISALLOW) && \
						 (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
				{
					/*����ģʽ���Ϊ�Ƕ���ģʽ*/
					if (uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT;
					}
					else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
					}	
				
					/*PIDˮƽ���ƻ��ָ�λ*/
					pid_Horizontal_GPS_Ctrl_Integrate_Reset();				
			
					/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;
				}
				/*���붨��ģʽһ�κ�,�����źŲ����㶨������,�л�������ģʽ(������check,ֱ���л�������ģʽ�������㶨������),��������OK��,Ӧ�л����Զ�����ģʽ*/
				else if ((status_GPS_Fix_Ava_Check(g_psUav_Status) == UAV_SENMOD_USE_CONTROL_DISALLOW) && \
						 (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_OK))
				{	
					/*����ģʽ���Ϊ�Ƕ���ģʽ*/
					if (uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT;
					}
					else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
					}
	
					/*PIDˮƽ���ƻ��ָ�λ*/
					pid_Horizontal_GPS_Ctrl_Integrate_Reset();
				
					/*�Զ��˳�����ģʽ �����ο���δ����(һֱCHECK,ֱ�����㶨��ģʽ/�ֶ�������ģʽ)*/
					uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO;
			
					/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;
				}				
			}
			/*ˮƽ���㴫������OPTICFLOW*/
			else if (uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_USE_OPTICFLOW)
			{
				/*�������ݿ��� && �ο���δ����*/
				if((uavStatus->UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
				   (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
				{
					/*����ǰ�ߵ�ˮƽλ�ù�����ΪĿ����ͣ��*/
						
					/*PIDˮƽ���ƻ��ָ�λ*/
			
					/*���óɹ��� �����ο���������(��ֹ�����,���¸�������λ��)*/
					uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_OK;	
			
					/*����ģʽ���Ϊ����*/
					uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_POS;	/*= 3->����*/		
			
					/*ˮƽ����ģʽΪ:�������ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_FIX_POS;
				}
				/*�ж���ʱ��,����������,�Զ��л�ˮƽ����ģʽ,������check,ֱ���л�������ģʽ�������㶨������*/
				else if ((uavStatus->UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
						 (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_NO))
				{
					/*����ģʽ���Ϊ�Ƕ���ģʽ*/
					if (uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT;
					}
					else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
					}	
				
					/*PIDˮƽ���ƻ��ָ�λ*/			
			
					/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;
				}
				/*���붨��ģʽһ�κ�,�������ݲ����㶨������,�л�������ģʽ(������check,ֱ���л�������ģʽ�������㶨������)*/
				else if ((uavStatus->UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
						 (uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS == UAV_SWITCH_REFERENCE_SET_OK))
				{	
					/*����ģʽ���Ϊ�Ƕ���ģʽ*/
					if (uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT;
					}
					else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
					{
						uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
					}
	
					/*PIDˮƽ���ƻ��ָ�λ*/
				
					/*�Զ��˳�����ģʽ �����ο���δ����(һֱCHECK,ֱ�����㶨��ģʽ/�ֶ�������ģʽ)*/
					uavStatus->UavControlMode.Horizontal.REFERENCE_SET_STATUS = UAV_SWITCH_REFERENCE_SET_NO;
			
					/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
					uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;
				}				
			}
		}
		else if(uavStatus->UavControlMode.Horizontal.EXPECT_CONTROL_MODE == UAV_HORIZONTAL_CONTROL_SELFAUTO) /*ˮƽ����������*/
		{
			/*����ģʽ���Ϊ�Ƕ���ģʽ*/
			if (uavStatus->UavFlyType.CURRENT == UAV_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
			{
				uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_FIX_HEIGHT;
			}
			else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
			{
				uavStatus->UavFlyType.CURRENT = UAV_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			}		
			
			/*PIDˮƽ���ƻ��ָ�λ*/
			
			/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
			uavStatus->UavControlMode.Horizontal.CONTROL_MODE = UAV_HORIZONTAL_CONTROL_SELFAUTO;			
		}
		
		/*============ SWD�������ݷ�������ǰ״̬���÷������� ============*/		
		/*�жϵ�ǰ�Ƿ�����ֱ����+ˮƽ����ģʽ*/
		if ((uavStatus->UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT) && \
			(uavStatus->UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_FIX_POS))
		{
			/*�ڹ���+���������붨��Ŀ�����,��ȡ��ǰ��������*/
			if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_USE_ULTR) && \
				(uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_USE_OPTICFLOW))
			{
				/*5000ms��:1�ι���׷��/2�ι���ѭ��������*/		
				uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION = control_fly_mission_check_ultr_opflow_pos(uavStatus, 5000);
			}
			/*��GPS+��ѹ�Ʋ��붨��Ŀ�����,��ȡ��ǰ��������*/
			else if ((uavStatus->UavSenmodStatus.Vertical.CURRENT_USE == UAV_VERTICAL_SENMOD_CURRENT_USE_BERO) && \
					 (uavStatus->UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_USE_GPS))
			{
				/*5000ms��:1�μǵ�/2�ΰ��ǵ㿪ʼѲ��*/		
				uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION = control_fly_mission_check_bero_gps_pos(uavStatus, 5000);
			}
		}
	}
}

/*��̬������:�Ƕ��⻷+���ٶ��ڻ�*/
void ctrl_Attitude_Control_Dp(void)
{
	/*�ǶȻ�����*/
	ctrl_Attitude_Angle_Control_Dp();

	/*���ٶȻ�����*/	
	ctrl_Attitude_Gyro_Control_Dp();
}


vu16 g_WaitMagGyroFuseTicks = 0; /*�ȴ������ƺ��������ں�ʱ��*/

/*�Ƕ��⻷����*/
void ctrl_Attitude_Angle_Control_Dp(void)
{
	/*==== ROLL(�����),PITCH(������) �⻷�Ƕȷ�������� ====*/
	/***************�⻷ �Ƕ� ����****************/
	g_psPidSystem->PitchAngle.feedback = g_psAttitudeAll->Ahrs.pitch;
	g_psPidSystem->RollAngle.feedback  = g_psAttitudeAll->Ahrs.roll;
	
	/***************�⻷ �Ƕ� ����****************/
	g_psPidSystem->PitchAngle.controlOutput = pid_Control_General_Dp(&g_psPidSystem->PitchAngle); 	  /*PITCH��PID���㼰���*/	
	g_psPidSystem->RollAngle.controlOutput  = pid_Control_General_Dp(&g_psPidSystem->RollAngle);	  /*ROLL��PID���㼰���*/	
	
	/*=========== YAW(ƫ����) �⻷�Ƕȷ�������� =========*/	
	/*YAWҡ�������м�λ��*/
	if (g_psControlAircraft->RemotExpectAngle.yaw == 0)
	{
		/*��ͷģʽ���ɻ��ϵ��һ��ʱ������ƫ����,�����ơ��������ں���Ҫһ��ʱ��,����ȡ500��(2.5S)*/
		if (g_WaitMagGyroFuseTicks <= 500)
		{
			g_WaitMagGyroFuseTicks++;
		}
		
		/*YAWҡ�������м�λֵ������Ϊ0�������������,ƫ������������Ϊ��ǰƫ����*/
		if ((g_psPidSystem->YawAngle.expect == 0) || \
			(g_WaitMagGyroFuseTicks <= 500) || \
			(g_sUav_Status.UavLandStatus.ThisTime == UAV_LAND_YES))
		{
			/*ƫ������������Ϊ��ǰƫ����*/
			g_psPidSystem->YawAngle.expect = g_psAttitudeAll->Ahrs.yaw;
		}
		
		/*ƫ���Ƿ�������*/
		g_psPidSystem->YawAngle.feedback = g_psAttitudeAll->Ahrs.yaw;
		
		/*PID Yaw�ǿ��Ƽ����*/
		g_psPidSystem->YawAngle.controlOutput = pid_Control_Yaw_Dp(&(g_psPidSystem->YawAngle));  
		
		/*����ƫ�����ٶȻ�����,��Դ��ƫ���Ƕȿ��������*/
		g_psPidSystem->YawGyro.expect = g_psPidSystem->YawAngle.controlOutput;	
	}
	else /*����ƫ������˺�,ֻ�����ڻ����ٶȿ���(ң������ֱ�Ӹ��ڻ����ٶ�)*/
	{
		/*ƫ����������0,�����нǶȿ���*/
		g_psPidSystem->YawAngle.expect = 0;		
		
		/*ƫ�����ٶȻ�����,ֱ����Դ��ң���������*/
		g_psPidSystem->YawGyro.expect = g_psControlAircraft->RemotExpectAngle.yaw;
	}
}

#define CTRL_YAW_FEEDFORWARD 	(0.25f) /*ƫ����ǰ������*/
vu16 g_YawControlFaultHandleTicks = 0; /*�ȴ������ƺ��������ں�ʱ��*/

/*���ٶ��ڻ�����*/
void ctrl_Attitude_Gyro_Control_Dp(void)
{
	/*==== ROLL(�����),PITCH(������) �ڻ����ٶȷ�������� ====*/
	/***************�ڻ� ���ٶ� ����****************/
	g_psPidSystem->PitchGyro.expect = g_psPidSystem->PitchAngle.controlOutput; /*���ڽǶ��⻷���*/
	g_psPidSystem->RollGyro.expect  = g_psPidSystem->RollAngle.controlOutput;  /*���ڽǶ��⻷���*/
	
	/***************�ڻ� ���ٶ� ����****************/
	g_psPidSystem->PitchGyro.feedback = g_psAngleGyro->Pitch;
	g_psPidSystem->RollGyro.feedback  = g_psAngleGyro->Roll;	
	
	/***************�ڻ� ���ٶ� ����****************/
	g_psPidSystem->PitchGyro.controlOutput = pid_Control_Div_LPF(&(g_psPidSystem->PitchGyro), PID_CONTROLER_PITCH_GYRO);  /*PID DIV���Ƶ�ͨ�˲�*/
	g_psPidSystem->RollGyro.controlOutput  = pid_Control_Div_LPF(&(g_psPidSystem->RollGyro), PID_CONTROLER_ROLL_GYRO);    /*PID DIV���Ƶ�ͨ�˲�*/
	
	/*============ YAW(ƫ����) �ڻ����ٶȷ�������� ===========*/
	/***************�ڻ� ���ٶ� ����****************/
	g_psPidSystem->YawGyro.feedback = g_psAngleGyro->Yaw;

	/***************�ڻ� ���ٶ� ����****************/	
	g_psPidSystem->YawGyro.controlOutput = pid_Control_Div_LPF(&(g_psPidSystem->YawGyro), PID_CONTROLER_YAW_GYRO);    /*PID DIV���Ƶ�ͨ�˲�*/	
	
	/***************ƫ����ǰ������****************/
	g_psPidSystem->YawGyro.controlOutput += CTRL_YAW_FEEDFORWARD * g_psPidSystem->YawGyro.expect;
	
	/*�ڻ� ���ٶ� ����޷�*/
	if (g_psPidSystem->YawGyro.controlOutput >= g_psPidSystem->YawAngle.controlOutPutLimit)
	{
		g_psPidSystem->YawGyro.controlOutput = g_psPidSystem->YawAngle.controlOutPutLimit;
	}

	if (g_psPidSystem->YawGyro.controlOutput <= -g_psPidSystem->YawAngle.controlOutPutLimit)
	{
		g_psPidSystem->YawGyro.controlOutput = -g_psPidSystem->YawAngle.controlOutPutLimit;
	}

    /*=== ƫ�������쳣����ж�,��ƫ���������ܴ�ʱ,ƫ�����ٶȺ�С,���ʱΪǿ�������š����ŵص� ===*/	
	if ((math_Abs(g_psPidSystem->YawGyro.controlOutput) > ( g_psPidSystem->YawGyro.controlOutPutLimit / 2)) && \
	    (math_Abs(g_psAngleGyro->Yaw) <= 15.0f)) /*ƫ�����������Խϴ�,��ƫ�����ٶ���Ժ�С*/
	{
		g_YawControlFaultHandleTicks++;	/*5msִ��һ��*/
		
		if (g_YawControlFaultHandleTicks >= 500)
		{
			g_YawControlFaultHandleTicks = 500; /*�޷�*/
		}
	}
	else
	{
		g_YawControlFaultHandleTicks /= 2; /*�����㣬����������0*/
	}
	
	if (g_YawControlFaultHandleTicks == 400) /*2Sִ��һ��, ���⴦��*/
	{
		/*���ƫ���Ƕȿ��ƵĻ���*/
		pid_Link_Integrate_Reset(&(g_psPidSystem->YawAngle));		
		
		/*���ƫ�����ٶȿ��ƵĻ���*/
		pid_Link_Integrate_Reset(&(g_psPidSystem->YawGyro)); 
		
		/*ƫ�����ڴ�����Ϊ��ǰƫ����*/
		g_psPidSystem->YawAngle.expect = g_psAttitudeAll->Ahrs.yaw;
		
		/*Tick��0*/
		g_YawControlFaultHandleTicks = 0;
	}
}

/*����������: ����+����&����*/
void ctrl_MainLeading_Control_Dp(void)
{
	fp32 mainControlDeltaT;
	
	/*��ȡ����������ִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->CTRL_MainLeading));
	
	/*���ʱ�任�����*/
	mainControlDeltaT = g_psSystemPeriodExecuteTime->CTRL_MainLeading.DeltaTime / 1000.0f;
	
	/*���������� || ִ������ͬʱִ����̬����*/
	if ((g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_NULL) || \
		(g_sUav_Status.UavCurrentFlyMission.ATTITUDE_CTRL_STATUS == UAV_MISSION_ATTITUDE_CTRL_ENABLE))
	{
		/*����ң�����л���λ,�ɿؽ��벻ͬģʽ*/
		/*================== 1.�߶�����,ˮƽ���� ģʽ ==================*/
		if ((g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_SELFAUTO) && \
			(g_sUav_Status.UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_SELFAUTO))
		{
			/*�������ȿ����µ�ROLL(���)��PITCH(����)������ (ң�������������������ֵ��Χ)*/
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch; /*ң���ڴ����Ƚ�pitch*/
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;	/*ң���ڴ����Ƚ�roll*/
		
			/*�������ŷ�Χ��0~1000,����PWM���ڷ�Χ��1000~2000,������Ϊ1000*/
			if (g_psControlAircraft->RemotExpectAngle.throttle <= 1000)
			{
				g_psControlAircraft->ctrlThrottle = 1000;
			}
			else /*����ֱ����Դ��ң�������Ÿ���*/
			{
				g_psControlAircraft->ctrlThrottle = g_psControlAircraft->RemotExpectAngle.throttle;
			}
		
			/*����ֵ����*/
			g_psControlAircraft->lastCtrlThrottle = g_psControlAircraft->ctrlThrottle;
		}
		/*================== 2.�߶ȶ���,ˮƽ���� ģʽ ==================*/
		else if ((g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT) && \
			     (g_sUav_Status.UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_SELFAUTO))
		{
			/*����ˮƽ���ȿ����µ�ROLL(���)��PITCH(����)������ (ң�������������������ֵ��Χ)*/
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch; /*ң���ڴ����Ƚ�pitch*/
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;	/*ң���ڴ����Ƚ�roll*/
			
			/*��ֱ�߶�(����)������*/
			vertical_Control_AltHold(mainControlDeltaT);	
		}
		/*================== 3.�߶ȶ���,ˮƽ����(GPS/����) ģʽ ==================*/		
		else if ((g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT) && \
				 (g_sUav_Status.UavControlMode.Horizontal.CONTROL_MODE != UAV_HORIZONTAL_CONTROL_FIX_POS))
		{
			/*��ֱ�߶ȿ�����*/
			vertical_Control_AltHold(mainControlDeltaT);

			/*ˮƽλ�ÿ�����*/		
			horizontal_Control_PosHold(mainControlDeltaT);	
		}
		/*================== 4.�߶�����,ˮƽ���� ==================*/		
		else
		{	
			/*����ˮƽ�Ƕ�����,���¿�������*/
			/*����ˮƽ���ȿ����µ�ROLL(���)��PITCH(����)������ (ң�������������������ֵ��Χ)*/
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch;
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;		
		
			/*���ŷ�Χ0~1000*/
			if (g_psControlAircraft->RemotExpectAngle.throttle <= 1000)
			{
				g_psControlAircraft->ctrlThrottle = 1000;
			}
			else
			{
				/*����ֱ����Դ��ң�������Ÿ���*/
				g_psControlAircraft->ctrlThrottle = g_psControlAircraft->RemotExpectAngle.throttle;
			}
		
			/*���������Ƶ�*/
			g_psControlAircraft->lastCtrlThrottle = g_psControlAircraft->ctrlThrottle;
		}
	}
	
	/*��������һ�����*/
	if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_ONEKEY_FLY)
	{
		control_fly_mission_onekey_fly(g_psUav_Status);
	}
	/*��������ʧ����ң����Ϊң��һ������*/
	else if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_ONEKEY_LAND_HOME) 
	{
		/*�������Զ���������*/
		ctrl_Go_Home_Control(mainControlDeltaT);
	}
	/*�������񣺹���׷��*/
	else if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_OPFLOW_FOLLOW_POINT) 
	{
		/*δ��Ĭ��������*/		
		control_fly_mission_set(g_psUav_Status, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
	}
	/*�������񣺹���Ѳ��*/
	else if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_OPFLOW_FOLLOW_LINE) 
	{
		/*δ��Ĭ��������*/		
		control_fly_mission_set(g_psUav_Status, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
	}
	/*��������GPS��¼����*/
	else if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_GPS_WRITE_POS)
	{
		control_fly_mission_bero_gps_write_pos(g_psUav_Status);
	}
	/*��������GPSѲ��*/
	else if (g_sUav_Status.UavCurrentFlyMission.CURRENT_FLY_MISSION == UAV_FLY_MISSION_GPS_PATROL_SKY) 
	{
		/*δ��Ĭ��������*/
		control_fly_mission_set(g_psUav_Status, UAV_FLY_MISSION_NULL, UAV_MISSION_ATTITUDE_CTRL_ENABLE);
	}
}

/*������ǲ���*/
void ctrl_Throttle_DipAngle_Compensate(void)
{
	fp32 CosPitch_CosRoll = math_Abs(COS_PITCH * COS_ROLL);
	fp32 throttleMakeup = 0;
	fp32 temp = 0;
	
	if (CosPitch_CosRoll >= 0.999999f)
	{
		CosPitch_CosRoll = 0.999999f;
	}
	
	if (CosPitch_CosRoll <= 0.000001f)
	{
		CosPitch_CosRoll = 0.000001f;
	}
	
	if (CosPitch_CosRoll <= 0.50f)
	{
		CosPitch_CosRoll = 0.50f; /*Pitch,RollԼ����30��*/
	}
	
	/*������ת������*/
	if (g_psControlAircraft->ctrlThrottle >= CTRL_THROTTLE_START_TURN)
	{
		temp = (u16)MATH_MAX(MATH_ABS(100 * g_psAttitudeAll->Ahrs.pitch), MATH_ABS(100 * g_psAttitudeAll->Ahrs.roll));
		temp = math_Constrain(9000 - temp, 3000, 0) / (3000 * CosPitch_CosRoll);
		throttleMakeup = (g_psControlAircraft->ctrlThrottle - CTRL_THROTTLE_START_TURN) * temp; /*������ǲ���*/
		
		/*�������*/
		g_psControlAircraft->throttleOutput = (u16)(CTRL_THROTTLE_START_TURN + throttleMakeup);
		g_psControlAircraft->throttleOutput = (u16)(math_Constrain(g_psControlAircraft->throttleOutput, 2000, CTRL_THROTTLE_START_TURN));
	}
	else /*������ת������*/
	{
		g_psControlAircraft->throttleOutput = g_psControlAircraft->ctrlThrottle;
	}
}

vu16 g_vu16ThrottleIdleTransitionsTicks = 0;  		  /*���ٹ���ticks*/
vu16 g_vu16ThrottleIdleContinueTicks    = 0;  	      /*���ٳ���ticks*/

/*�Կ�ϵͳ���������*/
void ctrl_auto_control_system_output(void)
{		
	/*������ǲ���*/
	ctrl_Throttle_DipAngle_Compensate();
	
	/*��½���*/
	g_sUav_Status.UavLandStatus.ThisTime = ctrl_Landing_Status_Check(g_psUav_Status);
	
	/*=== �����������ڽ���״̬ ===*/
	if (g_sUav_Status.LOCK_STATUS == UAV_LOCK_NOT)
	{
		/*��⵽����״̬Ϊ��½״̬,�������*/
		if (g_sUav_Status.UavLandStatus.ThisTime == UAV_LAND_YES)
		{
			/*����ϴ��������ֵΪ���λ,���뵡��ʱ,���Ź��ɹ���*/
			if ((g_psControlAircraft->LastMotorPwmOutput.channle1 <= REMOT_THROTTLE_BASE_VALUE) && \
				(g_psControlAircraft->LastMotorPwmOutput.channle2 <= REMOT_THROTTLE_BASE_VALUE) && \
				(g_psControlAircraft->LastMotorPwmOutput.channle3 <= REMOT_THROTTLE_BASE_VALUE) && \
				(g_psControlAircraft->LastMotorPwmOutput.channle4 <= REMOT_THROTTLE_BASE_VALUE))
			{
				g_vu16ThrottleIdleTransitionsTicks = CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS;
			}
			/*�ϴ�Ϊ���״̬,����Ϊ��½״̬,�����һ������,��غ�,�������Զ�����*/
			else if (g_sUav_Status.UavLandStatus.LastTime == UAV_LAND_NOT)
			{
				g_sUav_Status.LOCK_STATUS = UAV_LOCK_YES;
			}
			
			g_vu16ThrottleIdleContinueTicks++;
			
			if (g_vu16ThrottleIdleContinueTicks >= CTRL_IDEL_TRANSITION_INC_PERIOD_TICKS)
			{
				if (g_vu16ThrottleIdleTransitionsTicks >= 1)
				{
					g_vu16ThrottleIdleTransitionsTicks--;
				}

				g_vu16ThrottleIdleContinueTicks = 0;				
			}
			
			/*���ŵ���*/
			g_psControlAircraft->CurMotorPwmOutput.channle1 = REMOT_THROTTLE_BASE_VALUE + \
															  (CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS - g_vu16ThrottleIdleTransitionsTicks) * \
															  (CTRL_IDEL_THROTTLE_VALUE - REMOT_THROTTLE_BASE_VALUE) / \
															   CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS;   
			
			g_psControlAircraft->CurMotorPwmOutput.channle2 = REMOT_THROTTLE_BASE_VALUE + \
															  (CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS - g_vu16ThrottleIdleTransitionsTicks) * \
															  (CTRL_IDEL_THROTTLE_VALUE - REMOT_THROTTLE_BASE_VALUE) / \
															   CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS;   
			
			g_psControlAircraft->CurMotorPwmOutput.channle3 = REMOT_THROTTLE_BASE_VALUE + \
															  (CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS - g_vu16ThrottleIdleTransitionsTicks) * \
															  (CTRL_IDEL_THROTTLE_VALUE - REMOT_THROTTLE_BASE_VALUE) / \
															   CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS;   
			
			g_psControlAircraft->CurMotorPwmOutput.channle4 = REMOT_THROTTLE_BASE_VALUE + \
															  (CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS - g_vu16ThrottleIdleTransitionsTicks) * \
															  (CTRL_IDEL_THROTTLE_VALUE - REMOT_THROTTLE_BASE_VALUE) / \
														       CTRL_IDEL_TRANSITION_CONTINUE_MAX_TICKS; 

			/*������������½״̬,���PID����ϵͳ������*/
			pid_Horizontal_Takeoff_Integrate_Reset();
		}
		/*������������½����,����⵽����״̬Ϊ����״̬*/
		else if (g_sUav_Status.UavLandStatus.ThisTime == UAV_LAND_NOT)
		{
			/*=== 1.��ֱ����,��̬����ģʽ ===*/
			if (g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_SELFAUTO)				
			{
				/*�������������*/
				if (g_psControlAircraft->ctrlThrottle >= CTRL_THROTTLE_START_FLY_VALUE)
				{
					/*Ĭ��ˮƽ��̬�����ٶȶ���������Դ��PID*/
					g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)(g_psControlAircraft->throttleOutput    - \
																		    g_psPidSystem->RollGyro.controlOutput  + \
																			g_psPidSystem->PitchGyro.controlOutput - \
																			g_psPidSystem->YawGyro.controlOutput);

					g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)(g_psControlAircraft->throttleOutput    + \
																			g_psPidSystem->RollGyro.controlOutput  - \
																			g_psPidSystem->PitchGyro.controlOutput - \
																			g_psPidSystem->YawGyro.controlOutput);

					g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)(g_psControlAircraft->throttleOutput    + \
																			g_psPidSystem->RollGyro.controlOutput  + \
																			g_psPidSystem->PitchGyro.controlOutput + \
																			g_psPidSystem->YawGyro.controlOutput);

					g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)(g_psControlAircraft->throttleOutput    - \
																			g_psPidSystem->RollGyro.controlOutput  - \
																			g_psPidSystem->PitchGyro.controlOutput + \
																			g_psPidSystem->YawGyro.controlOutput);
				}
				else /*С�����������*/
				{
					g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)(g_psControlAircraft->throttleOutput);
					g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)(g_psControlAircraft->throttleOutput);
					g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)(g_psControlAircraft->throttleOutput);
					g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)(g_psControlAircraft->throttleOutput);				
				
					/*������δ���,������½״̬,���PID����ϵͳ������*/
					pid_Horizontal_Takeoff_Integrate_Reset();
				}

				/*��·PWM������޷�*/
				g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle1, \
																					  2000, CTRL_IDEL_THROTTLE_VALUE);

			    g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle2, \
																					  2000, CTRL_IDEL_THROTTLE_VALUE);

				g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle3, \
																					  2000, CTRL_IDEL_THROTTLE_VALUE);

				g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle4, \
																					  2000, CTRL_IDEL_THROTTLE_VALUE);	
				
			}
			/*=== 2.��ֱ����,����������ģʽ,�����й� ===*/
			else if (g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT)	
			{
				/*Ĭ��ˮƽ��̬�����ٶȶ���������Դ��PID*/
				g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)(g_psControlAircraft->throttleOutput    - \
																		g_psPidSystem->RollGyro.controlOutput  + \
																		g_psPidSystem->PitchGyro.controlOutput - \
																		g_psPidSystem->YawGyro.controlOutput);

				g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)(g_psControlAircraft->throttleOutput    + \
																		g_psPidSystem->RollGyro.controlOutput  - \
																		g_psPidSystem->PitchGyro.controlOutput - \
																		g_psPidSystem->YawGyro.controlOutput);

				g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)(g_psControlAircraft->throttleOutput    + \
																		g_psPidSystem->RollGyro.controlOutput  + \
																		g_psPidSystem->PitchGyro.controlOutput + \
																		g_psPidSystem->YawGyro.controlOutput);
																		
				g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)(g_psControlAircraft->throttleOutput    - \
																		g_psPidSystem->RollGyro.controlOutput  - \
																		g_psPidSystem->PitchGyro.controlOutput + \
																		g_psPidSystem->YawGyro.controlOutput);				
			}	

			/*��·PWM������޷�*/
			g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle1, \
																				  2000, CTRL_IDEL_THROTTLE_VALUE);

			g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle2, \
																				  2000, CTRL_IDEL_THROTTLE_VALUE);

		    g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle3, \
																				  2000, CTRL_IDEL_THROTTLE_VALUE);

			g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle4, \
																				  2000, CTRL_IDEL_THROTTLE_VALUE);				
		}
	}
	/*=== ����������������״̬ ===*/
	else if (g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES)
	{ 
		/*�ĸ����PWMֵΪͣתֵ*/
		g_psControlAircraft->CurMotorPwmOutput.channle1 = REMOT_THROTTLE_BASE_VALUE;
		g_psControlAircraft->CurMotorPwmOutput.channle2 = REMOT_THROTTLE_BASE_VALUE;
		g_psControlAircraft->CurMotorPwmOutput.channle3 = REMOT_THROTTLE_BASE_VALUE;
		g_psControlAircraft->CurMotorPwmOutput.channle4 = REMOT_THROTTLE_BASE_VALUE;
		
		/*������������½״̬,���PID����ϵͳ������*/
		pid_Horizontal_Takeoff_Integrate_Reset();
		pid_Vertical_Ctrl_Integrate_Reset();
	}
	
	/*���PWM�������*/
	g_psControlAircraft->LastMotorPwmOutput.channle1 = g_psControlAircraft->CurMotorPwmOutput.channle1;
	g_psControlAircraft->LastMotorPwmOutput.channle2 = g_psControlAircraft->CurMotorPwmOutput.channle2;
	g_psControlAircraft->LastMotorPwmOutput.channle3 = g_psControlAircraft->CurMotorPwmOutput.channle3;
	g_psControlAircraft->LastMotorPwmOutput.channle4 = g_psControlAircraft->CurMotorPwmOutput.channle4;	
	
	/*��·PWM������޷�*/
	g_psControlAircraft->CurMotorPwmOutput.channle1 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle1, \
																		  2000, ESC_MIN_PULSE_ZERO_SPEED_VALUE);

	g_psControlAircraft->CurMotorPwmOutput.channle2 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle2, \
																		  2000, ESC_MIN_PULSE_ZERO_SPEED_VALUE);

	g_psControlAircraft->CurMotorPwmOutput.channle3 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle3, \
																		  2000, ESC_MIN_PULSE_ZERO_SPEED_VALUE);

	g_psControlAircraft->CurMotorPwmOutput.channle4 = (u16)math_Constrain(g_psControlAircraft->CurMotorPwmOutput.channle4, \
																		  2000, ESC_MIN_PULSE_ZERO_SPEED_VALUE);	
	
	/*���Ʋ��������ȡ�ɹ�,��������ת��,�̶����ܵ���ʱ����*/
	#if (CONTROL_SYS__ONLY_PID == SYS_ENABLE)		/*PID�㷨*/
	#if (CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING == SYS_DISABLE)	
	if (g_psCtrlSysStatus->pid == CTRL_SYSTEM_PARA_INIT_SUCC)
	{
	#endif
	#elif (CONTROL_SYS__ONLY_ADRC == SYS_ENABLE)	/*ADRC�㷨*/
	#if (CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING == SYS_DISABLE)		
	if (g_psCtrlSysStatus->adrc == CTRL_SYSTEM_PARA_INIT_SUCC)
	{		
	#endif
	#elif (CONTROL_SYS__PID_ADRC == SYS_ENABLE)		/*PID + ADRC�㷨*/
	#if (CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING == SYS_DISABLE)		
	if ((g_psCtrlSysStatus->pid == CTRL_SYSTEM_PARA_INIT_SUCC) && \
		(g_psCtrlSysStatus->adrc == CTRL_SYSTEM_PARA_INIT_SUCC))
	{		
	#endif
	#endif
	
	/*������ը��������*/
	if ((math_Abs(g_psAttitudeAll->Ahrs.pitch) <= CTRL_HORIZONTAL_ANGLE_SAFE_MAX) && \
		(math_Abs(g_psAttitudeAll->Ahrs.roll) <= CTRL_HORIZONTAL_ANGLE_SAFE_MAX)) /*��ȫ�Ƕȷ�Χ��,��������*/
	{
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH4, \
		   				       g_psControlAircraft->CurMotorPwmOutput.channle1);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH3, \
							   g_psControlAircraft->CurMotorPwmOutput.channle2);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH2, \
							   g_psControlAircraft->CurMotorPwmOutput.channle3);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH1, \
							   g_psControlAircraft->CurMotorPwmOutput.channle4);
	}
	else /*�ǰ�ȫ�Ƕȷ�Χ��,�رյ��,������*/
	{
		/*�رյ��*/
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH4, \
		   				       ESC_MIN_PULSE_ZERO_SPEED_VALUE);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH3, \
							   ESC_MIN_PULSE_ZERO_SPEED_VALUE);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH2, \
							   ESC_MIN_PULSE_ZERO_SPEED_VALUE);
		
		msp_TimPwmOut_SetPluse(&g_sTimPwmOut_Motor, MSP_TIM_CH1, \
							   ESC_MIN_PULSE_ZERO_SPEED_VALUE);
		
		/*����״̬*/
		g_sUav_Status.LOCK_STATUS = UAV_LOCK_YES;
	}
			
	#if (CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING == SYS_DISABLE)			
	}
	#endif
}
