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
	.GO_HOME_STATUS     = CTRL_AIRCRAFT_GO_HOME_DISABLE,	/*�ϵ�������Զ�����*/
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
		
		ctrl_Control_Mode_Select(&g_sAircraftStatus);
	}
	
	/************* 1.����������: ����+���ˮƽ������ *************/
	ctrl_MainLeading_Control_Dp();
	
	/************* 2.��̬������: �Ƕ��⻷+���ٶ��ڻ� *************/
	ctrl_Attitude_Control_Dp();
}

/*�߶�,ˮƽ����ģʽѡ��*/
void ctrl_Control_Mode_Select(AircraftStatus *aircraftStatus)
{
	/*�ϴ�ң�������߶ȿ���ģʽ*/
	aircraftStatus->LAST_HEIGHT_CONTROL_MODE = aircraftStatus->CUR_HEIGHT_CONTROL_MODE;

	/*�ϴ�ң������ˮƽλ�ÿ���ģʽ*/
	aircraftStatus->LAST_HORIZONTAL_CONTROL_MODE = aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE;
	
	/*============ �ж�ң����ģʽѡ���Ӧ������ֵ,������ģʽ�л� ============*/
	/*SWITCH A Ĭ���ϲ�Ϊ����̬����,�²�Ϊ����*/
	if (remot_Data_Range(g_sRemotData.SWA, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		aircraftStatus->CUR_HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;	/*��ֱ��̬����ģʽģʽ*/		
	}
	else if (remot_Data_Range(g_sRemotData.SWA, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{
		aircraftStatus->CUR_HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR;	/*����������ģʽ*/
	}

	/*SWITCH B Ĭ���ϲ�Ϊ����̬����,�²�Ϊˮƽ����*/
	if (remot_Data_Range(g_sRemotData.SWB, REMOT_DATA_MIN) == REMOT_DATA_MIN)
	{
		aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;	/*ˮƽ��̬����ģʽģʽ*/	
	}
	else if (remot_Data_Range(g_sRemotData.SWB, REMOT_DATA_MAX) == REMOT_DATA_MAX)
	{
		aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR;  /*������ˮƽ����ģʽ*/
	}
	
	/*============ ����&����ң���л����� ============*/
	/*�жϸ߶Ⱥ�ˮƽ����ģʽ�Ƿ��л�*/
	/*�߶�:���ο���ģʽ���ϴο���ģʽ��ͬ*/
	if (aircraftStatus->CUR_HEIGHT_CONTROL_MODE != aircraftStatus->LAST_HEIGHT_CONTROL_MODE)
	{
		/*�����ж���*/
		if (aircraftStatus->CUR_HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR)
		{
			/*��Ǳ���ң������Ϊ�����ж���*/
			aircraftStatus->HEIGHT_REMOT_EXPECT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO_TO_SENSOR;
			
			/*�����ο���δ����(�л�����Ҫ���¼�¼��ǰ�߶�)*/
			aircraftStatus->HEIGHT_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_NO;
		}
		/*����������*/
		else if (aircraftStatus->CUR_HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO)
		{
			/*��Ǳ���ң������Ϊ����������*/
			aircraftStatus->HEIGHT_REMOT_EXPECT_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR_TO_AUTO;
		}
	}

	/*ˮƽ:���ο���ģʽ���ϴο���ģʽ��ͬ*/
	if (aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE != aircraftStatus->LAST_HORIZONTAL_CONTROL_MODE) 
	{
		/*������ˮƽ����*/
		if (aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR)
		{
			/*��Ǳ���Ϊ������ˮƽ����*/
			aircraftStatus->HORIZONTAL_REMOT_EXPECT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO_TO_SENSOR;
			
			/*�����ο���δ����(�л�����Ҫ���¼�¼��ǰˮƽλ��)*/
			aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_NO;
		}
		/*ˮƽ����������*/
		else if (aircraftStatus->CUR_HORIZONTAL_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO)
		{
			/*��Ǳ���Ϊ����������*/
			aircraftStatus->HORIZONTAL_REMOT_EXPECT_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR_TO_AUTO;						
		}
	}

	/*������δʧ��,�Ž���ң�ظı䵱ǰ����״̬*/
	if (aircraftStatus->CMC_STATUS == AIRCRAFT_CMC_SUCC)
	{
		/*============ ���������жϼ�����ģʽ�л� ============*/
		/*��ֱ�����ж���*/
		if (aircraftStatus->HEIGHT_REMOT_EXPECT_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO_TO_SENSOR)
		{
			/*ֻҪ��ѹ����Ч�Ϳɶ���(��������Чʱ,�͸߶�������Դ������,����������Դ��ѹ��)*/
			if ((aircraftStatus->BERO_ESTIMATE_ALTITUDE == HEIGHT_DATA_STATUS_OK) && \
				(aircraftStatus->HEIGHT_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_NO))
			{
				/*ÿ���л�ֻ����һ�ε�ǰ����ֵ*/
				g_psControlAircraft->heightHoldThrottle = g_psControlAircraft->RemotExpectAngle.throttle;
		
				/*��Switch_SWA�������²���˲��Ĺߵ���ֱλ�ù�����ΪĿ��߶�*/
				g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
		
				/*���óɹ��� �����ο���������(��ֹ���ߺ�,���¸��������߶�)*/
				aircraftStatus->HEIGHT_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_OK;

				/*����ģʽ���Ϊ����*/
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_FIX_HEIGHT;	/*= 2->����*/
		
				/*��ֱ����ģʽΪ:���߿���ģʽ*/
				aircraftStatus->HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR;
			}
			/*�ж���ʱ:��ѹ����Ч,��δ���õ�λ*/
			else if ((aircraftStatus->BERO_ESTIMATE_ALTITUDE == HEIGHT_DATA_STATUS_NO) && \
					 (aircraftStatus->HEIGHT_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_NO))
			{
				/*����ģʽ���Ϊ��̬ģʽ*/
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			
				/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
				aircraftStatus->HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;		
			}
			/*����״̬��:��ѹ����Ч,�������õ�λ(����������)*/
			else if ((aircraftStatus->BERO_ESTIMATE_ALTITUDE == HEIGHT_DATA_STATUS_NO) && \
					 (aircraftStatus->HEIGHT_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_OK))
			{
				/*����ģʽ���Ϊ��̬ģʽ*/
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/

				/*�˳����ߺ�,��ǲο���δ����,������һֱcheck*/
				aircraftStatus->HEIGHT_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_NO;				
			
				/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
				aircraftStatus->HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;				
			}
		}
		else if (aircraftStatus->HEIGHT_REMOT_EXPECT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR_TO_AUTO)	/*����������*/
		{
			/*����ģʽ���Ϊ��̬*/
			aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/	
		
			/*��ֱ����ģʽΪ:���ȿ���ģʽ*/
			aircraftStatus->HEIGHT_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;			
		}
		
		/*============ ���������жϼ�����ģʽ�л� ============*/
		/*ˮƽ�����ж���,ˮƽ������Դ: ���� / GPS*/
		if (aircraftStatus->HORIZONTAL_REMOT_EXPECT_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO_TO_SENSOR)
		{
			/*(GPS����OK && HOME������ && �ںϳɹ�) && �ο���δ����*/
			if((status_GPS_Fix_Ava_Check(&g_sAircraftStatus) == GPS_POS_FIX_AVA_TRUE) && \
			   (aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_NO))
			{
				/*����ǰ�ߵ�ˮƽλ�ù�����ΪĿ����ͣ��*/
				g_psPidSystem->LatitudePosition.expect  = g_psSinsReal->curPosition[EARTH_FRAME_Y]; /*N*/
				g_psPidSystem->LongitudePosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_X];	/*E*/
						
				/*PIDˮƽ���ƻ��ָ�λ*/
				pid_Horizontal_GPS_Ctrl_Integrate_Reset();
			
				/*���óɹ��� �����ο���������(��ֹ�����,���¸�������λ��)*/
				aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_OK;	
			
				/*����ģʽ���Ϊ����*/
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_FIX_POS;	/*= 3->����*/		
			
				/*ˮƽ����ģʽΪ:�������ģʽ*/
				aircraftStatus->HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_GPS_SENSOR;
			}
			/*�ж���ʱ��,����������,�Զ��л�ˮƽ����ģʽ,������check,ֱ���л�������ģʽ�������㶨������*/
			else if ((status_GPS_Fix_Ava_Check(&g_sAircraftStatus) == GPS_POS_FIX_AVA_FALSE) && \
					(aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_NO))
			{
				/*����ģʽ���Ϊ�Ƕ���ģʽ*/
				if (aircraftStatus->CUR_FLY_TYPE == AIRCRAFT_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
				{
					aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_FIX_HEIGHT;
				}
				else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
				{
					aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
				}	
				
				/*PIDˮƽ���ƻ��ָ�λ*/
				pid_Horizontal_GPS_Ctrl_Integrate_Reset();				
			
				/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
				aircraftStatus->HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;
			}
			/*���붨��ģʽһ�κ�,�����źŲ����㶨������,�л�������ģʽ(������check,ֱ���л�������ģʽ�������㶨������),��������OK��,Ӧ�л����Զ�����ģʽ*/
			else if ((status_GPS_Fix_Ava_Check(&g_sAircraftStatus) == GPS_POS_FIX_AVA_FALSE) && \
					(aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS == AIRCRAFT_REFERENCE_SET_OK))
			{	
				/*����ģʽ���Ϊ�Ƕ���ģʽ*/
				if (aircraftStatus->CUR_FLY_TYPE == AIRCRAFT_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
				{
					aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_FIX_HEIGHT;
				}
				else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
				{
					aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/
				}
	
				/*PIDˮƽ���ƻ��ָ�λ*/
				pid_Horizontal_GPS_Ctrl_Integrate_Reset();
				
				/*�Զ��˳�����ģʽ �����ο���δ����(һֱCHECK,ֱ�����㶨��ģʽ/�ֶ�������ģʽ)*/
				aircraftStatus->HORIZONTAL_REFERENCE_SET_STATUS = AIRCRAFT_REFERENCE_SET_NO;
			
				/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
				aircraftStatus->HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;
			}			
		}
		else if(aircraftStatus->HORIZONTAL_REMOT_EXPECT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR_TO_AUTO) /*ˮƽ����������*/
		{
			/*����ģʽ���Ϊ�Ƕ���ģʽ*/
			if (aircraftStatus->CUR_FLY_TYPE == AIRCRAFT_FLY_TYPE_FIX_HEIGHT) /*����Դ��ڶ���ģʽ,��Ϊ����ģʽ*/
			{
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_FIX_HEIGHT;
			}
			else /*���û���ڶ���,Ҳû���ڶ���,��Ϊ����̬ģʽ*/
			{
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_ATTITUDE;	/*= 1->��̬*/			
			}		
			
			/*PIDˮƽ���ƻ��ָ�λ*/
			pid_Horizontal_GPS_Ctrl_Integrate_Reset();
			
			/*ˮƽ����ģʽΪ:���ȿ���ģʽ*/
			aircraftStatus->HORIZONTAL_CONTROL_MODE = AIRCRAFT_CONTROL_AUTO;			
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
			(g_psAircraftStatus->CUR_FLY_STATUS == AIRCRAFT_LANDING))
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
	
	/*�ж��Ƿ�Ϊʧ����ң��һ�� ����*/
	if (g_psAircraftStatus->CUR_FLY_TYPE != AIRCRAFT_FLY_TYPE_GO_HOME)
	{
		/*����ң�����л���λ,�ɿؽ��벻ͬģʽ*/
		/*================== 1.�߶�����,ˮƽ���� ģʽ ==================*/
		if ((g_psAircraftStatus->HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO) && \
			(g_psAircraftStatus->HORIZONTAL_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO))
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
		else if ((g_psAircraftStatus->HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR) && \
			     (g_psAircraftStatus->HORIZONTAL_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO))
		{
			/*����ˮƽ���ȿ����µ�ROLL(���)��PITCH(����)������ (ң�������������������ֵ��Χ)*/
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch; /*ң���ڴ����Ƚ�pitch*/
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;	/*ң���ڴ����Ƚ�roll*/
			
			/*��ֱ�߶�(����)������*/
			vertical_Control_AltHold(mainControlDeltaT);	
		}
		/*================== 3.�߶ȶ���,ˮƽ����(GPS/����) ģʽ ==================*/		
		else if ((g_psAircraftStatus->HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR) && \
				(g_psAircraftStatus->HORIZONTAL_CONTROL_MODE != AIRCRAFT_CONTROL_AUTO))
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
	/*ʧ����ң����Ϊң��һ������*/
	else if (g_psAircraftStatus->CUR_FLY_TYPE == AIRCRAFT_FLY_TYPE_GO_HOME) 
	{
		/*�������Զ���������*/
		ctrl_Go_Home_Control(mainControlDeltaT);
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
	
	/*��½�������*/
	g_psAircraftStatus->CUR_FLY_STATUS = ctrl_Landing_Check_And_Idling();
	
	/*=== �����������ڽ���״̬ ===*/
	if (g_psAircraftStatus->LOCK_STATUS == AIRCRAFT_UNLOCK)
	{
		/*��⵽����״̬Ϊ��½״̬,�������*/
		if (g_psAircraftStatus->CUR_FLY_STATUS == AIRCRAFT_LANDING)
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
			else if (g_psAircraftStatus->LAST_FLY_STATUS == AIRCRAFT_FLYING)
			{
				g_psAircraftStatus->LOCK_STATUS = AIRCRAFT_LOCKING;
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
		else if (g_psAircraftStatus->CUR_FLY_STATUS == AIRCRAFT_FLYING)
		{
			/*=== 1.��ֱ����,��̬����ģʽ ===*/
			if (g_psAircraftStatus->HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_AUTO)
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
			else if (g_psAircraftStatus->HEIGHT_CONTROL_MODE == AIRCRAFT_CONTROL_SENSOR)	
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
	else if (g_psAircraftStatus->LOCK_STATUS == AIRCRAFT_LOCKING)
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
		g_sAircraftStatus.LOCK_STATUS = AIRCRAFT_LOCKING;
	}
			
	#if (CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING == SYS_DISABLE)			
	}
	#endif
}
