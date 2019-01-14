#include "control_Landing.h"
#include "ahrs_Caculation.h"
#include "control_Aircraft.h"

/*��½��������*/
ControlLand	g_sControlLand = 
{
	.verticalSpeedChangePeriod = 0,
};

ControlLand *g_psControlLand = &g_sControlLand;


vu16 g_vu16LandCheckContinueTicks = 0; /*�����������½����ticks*/

/*�Լ촥�ؽ��뵡��ģʽ*/
UAV_LAND_STATUS ctrl_Landing_Status_Check(Uav_Status *uavStatus)
{
	/*���ſ��ƴ��ڽϵ��г�:
	  1.��̬ģʽ��,���Ÿ˴��ڵ�λ
      2.����ģʽ��,�����ٶ�����,�����ٶȻ�����Ϊ��Сֵ��
      3.���ٶȿ���������ڳ�ʱ�����,�����Ľϴ�ֵ,ʹ�����ſ��ƽϵ�*/
	
	/*��¼�ϴη���״̬*/
	uavStatus->UavLandStatus.LastTime = uavStatus->UavLandStatus.ThisTime;
	
	/*1.��ǰ�������,2.���غ�����ת(�Ͻ��ٶ�С��20deg/s),3.�ߵ���ֱ�����ٶ�<��20cm/s,4.��ˮƽ�ֶ�����*/
	if ((g_psControlAircraft->throttleOutput <= CTRL_LAND_CHECK_THROTTLE_THRESHOLD_MIN) && \
		(g_GyroLenth <= 20.0f) && \
		(math_Abs(g_psSinsReal->curSpeed[EARTH_FRAME_Z]) <= 20.0f) && \
		(ctrl_Go_Home_Horizontal_Hand_Meddle() == CTRL_GO_HOME_HAND_MEDDLE_FALSE))
	{
		g_vu16LandCheckContinueTicks++; /*5msִ��һ��*/	
	}
	else
	{
		g_vu16LandCheckContinueTicks /= 2; /*��������*/		
	}
	
	/*���Ticks����״̬*/
	if (g_vu16LandCheckContinueTicks >= 5000)
	{
		g_vu16LandCheckContinueTicks = 5000; /*��ֹ���*/
	}
	
	if (g_vu16LandCheckContinueTicks >= 300) /*����1.5S���������½���*/
	{
		/*��Ƿ���������״̬Ϊ��½״̬*/		
		uavStatus->UavLandStatus.ThisTime = UAV_LAND_YES;
		
		/*����һ�����,��ֹһ����½*/
//		uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.ENABLE_STATUS = UAV_MISSION_ENABLE; 
//		uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.ENABLE_STATUS       = UAV_MISSION_DISABLE;
		
		/*��½���ǽ�ֹ ʧ���Զ�����*/
		g_psControlAircraft->GO_HOME_STATUS = CTRL_AIRCRAFT_GO_HOME_DISABLE;
		
		/*��½״̬,��ֹ����������*/
		if ((uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION != UAV_FLY_MISSION_ONEKEY_FLY) && \
			(uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION != UAV_FLY_MISSION_NULL))
		{
			control_fly_mission_clear(uavStatus, uavStatus->UavCurrentFlyMission.CURRENT_FLY_MISSION);
		}
	}
	else
	{
		/*��Ƿ���������״̬Ϊ����״̬*/		
		uavStatus->UavLandStatus.ThisTime = UAV_LAND_NOT;	

		/*����һ������,��ֹһ�����*/
//		uavStatus->UavCurrentFlyMission.Onekey_Mission.LandHome.ENABLE_STATUS       = UAV_MISSION_ENABLE;		
//		uavStatus->UavCurrentFlyMission.Onekey_Mission.FixedHeightFly.ENABLE_STATUS = UAV_MISSION_DISABLE; 

		/*��ɺ���ʹ�� ʧ���Զ�����*/
		g_psControlAircraft->GO_HOME_STATUS = CTRL_AIRCRAFT_GO_HOME_ENABLE;
	}
	
	return (uavStatus->UavLandStatus.ThisTime);	/*���ط���������״̬*/
}


/*������ֱ�߶ȿ���*/
vu16 g_vu16LandVerticalPosControlTicks   = 0;	/*����λ�ÿ��Ƽ�����*/
vu16 g_vu16LandVerticalSpeedControlTicks = 0;	/*�������ƿ��Ƽ�����*/

void ctrl_Go_Home_Vertical_Control(fp32 verticalSpeedExpect, fp32 heightExpect, fp32 controlDeltaT)
{
	/*��ͣ��������ΪĬ����ͣ����*/
	#if (CTRL_THROTTLE_HOVER_ENABLE_STATUS == SYS_ENABLE)	
	g_psControlAircraft->heightHoldThrottle = CTRL_THROTTLE_HOVER_DEFAULT_VALUE;
	#endif	
	
	/*�߶ȿ�������1��*/
	/*  **
	   ***
	  ****
	    **
		**
		**
		**   
	  *******	
	  *******/
	/************************** ��ֱ�߶ȿ����� ��ʼ ********************************/
	/****** ���ߣ��߶�λ�û�+�ٶȻ�+���ٶȻ����������ڷֱ�Ϊ10ms��5ms��5ms ********/
	/*�߶�δ��Ŀ��߶�/�ֶ������߶�*/
	if (heightExpect == 0)
	{	
		/*һֱ���¸߶�����*/
		g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
	}
	else
	{	
		/*���¸߶�����*/
		g_psPidSystem->HighPosition.expect = heightExpect;
	}
	
	/************************** ��ֱλ�ÿ����� ��ʼ ********************************/
	/*����ҡ��λ���м�λ��������(�Ȳ�������,Ҳ��������)*/
	if ((g_psControlAircraft->throttleUpDownJudgeValue >= REMOT_THROTTLE_DOWN_CROSSOVER) && \
		(g_psControlAircraft->throttleUpDownJudgeValue <= REMOT_THROTTLE_UP_CROSSOVER))
	{
		/*ticks++*/
		g_vu16LandVerticalPosControlTicks++;	/*5msִ��һ��*/
		
		/*��ֱ�߶ȿ�������2*5=10ms*/
		if (g_vu16LandVerticalPosControlTicks >= CTRL_VERTICAL_POSITION_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16LandVerticalPosControlTicks = 0;
			
			/*���¸߶�λ�÷���*/
			g_psPidSystem->HighPosition.feedback = g_psSinsReal->curPosition[EARTH_FRAME_Z];
			
			/*�⻷ ��ֱ�߶�λ��PID���㼰���*/
			g_psPidSystem->HighPosition.controlOutput = pid_Control_General_Dp(&g_psPidSystem->HighPosition);
			
			/*�ж��ٶ�������Դ*/
			if (heightExpect == 0)
			{
				/*������ֱ�ٶ�������Դ���ⲿֱ�Ӹ���*/
				g_CurHeightYawSpeedExpect = verticalSpeedExpect;
			}
			else
			{
				 /*������ֱ�ٶ�������Դ��λ�û����*/
				g_CurHeightYawSpeedExpect = g_psPidSystem->HighPosition.controlOutput;
			}
			
			#ifdef CTRL_HEIGHT_YAW_SPEED_FEEDFORWARD /*�Ƿ������ٶ�ǰ��������*/
			g_HeightYawSpeedFeedforwardDelta  = (g_CurHeightYawSpeedExpect - g_LastHeightYawSpeedExpect) / (2 * controlDeltaT); /*�ٶ������仯��*/
			g_HeightYawSpeedFeedforwardOutput = g_HeightYawSpeedFeedforwardRate * g_HeightYawSpeedFeedforwardDelta; /*��ֱ�ٶ�ǰ�����������*/
			g_LastHeightYawSpeedExpect        = g_CurHeightYawSpeedExpect; /*��ֱ�ٶ������Ƶ�*/
			#endif
			
			/*���� �ڻ� ��ֱ�ٶ�����*/
			g_psPidSystem->HighSpeed.expect = g_CurHeightYawSpeedExpect;
		}
		
		/*��������ֱ�����˶����Ʊ�������*/
		g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND = CTRL_AIRCRAFT_MOVE_VER_HOLD;
	}
	/*����ҡ��λ���м�λ������֮��(����),���������ٶ�����*/
	else if (g_psControlAircraft->throttleUpDownJudgeValue > REMOT_THROTTLE_UP_CROSSOVER)
	{
		/*���Ÿ�����,ֱ�Ӹ�����ֱ�ٶ�����*/
		g_psPidSystem->HighSpeed.expect = CTRL_HEIGHT_CLIMB_UP_MAX_SPEED * \
										  (g_psControlAircraft->throttleUpDownJudgeValue - REMOT_THROTTLE_UP_CROSSOVER) / \
										  (CTRL_THROTTLE_TOP_VALUE - REMOT_THROTTLE_UP_CROSSOVER);
		
		/*��������ֱ�����˶���������*/
		g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND = CTRL_AIRCRAFT_MOVE_VER_UP;
	}
	/*����ҡ��λ���м�λ������֮��(����),�����½��ٶ�����*/
	else if (g_psControlAircraft->throttleUpDownJudgeValue < REMOT_THROTTLE_DOWN_CROSSOVER)
	{
		/*���Ÿ�����,ֱ�Ӹ�����ֱ�ٶ�����*/
		g_psPidSystem->HighSpeed.expect = CTRL_HEIGHT_CLIMB_DOWN_MAX_SPEED * \
										  (g_psControlAircraft->throttleUpDownJudgeValue - REMOT_THROTTLE_DOWN_CROSSOVER) / \
										  (REMOT_THROTTLE_DOWN_CROSSOVER - CTRL_THROTTLE_BOTTOM_VALUE);
		
		/*��������ֱ�����˶���������*/
		g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND = CTRL_AIRCRAFT_MOVE_VER_DOWN;		
	}
	/************************** ��ֱλ�ÿ����� ���� ********************************/		

	/*�߶ȿ�������2��*/
	/**********
	 **********
		     **
	 **********
	 **********
	 **
	 **********
	 ********** */		
	/************************** ��ֱ�ٶȿ����� ��ʼ ********************************/		
	/*ticks++*/
	g_vu16LandVerticalSpeedControlTicks++;
	
	/*��ֱ�߶ȿ�������1*5=5ms*/
	if (g_vu16LandVerticalSpeedControlTicks >= CTRL_VERTICAL_SPEED_CONTROL_PERIOD) /*5msִ��һ��*/
	{
		/*ticks��0*/
		g_vu16LandVerticalSpeedControlTicks = 0;
		
		/*������ֱ�ٶȷ���,����ֱ�ߵ��ٶ�*/
		g_psPidSystem->HighSpeed.feedback = g_psSinsReal->curSpeed[EARTH_FRAME_Z];
		
		/*��ֱ�߶��ٶȿ��Ƽ����*/
		g_psPidSystem->HighSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->HighSpeed, PID_CONTROLER_HIGH_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/
	}
	
	/*�����½��������������ٶ��޷���������*/
	if (g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND == CTRL_AIRCRAFT_MOVE_VER_UP)	/*����*/
	{  
		g_psPidSystem->HighSpeed.controlOutput = math_Constrain(g_psPidSystem->HighSpeed.controlOutput, \
															    CTRL_HEIGHT_CLIMB_UP_MAX_ACCELERATION, -CTRL_HEIGHT_CLIMB_UP_MAX_ACCELERATION);
	}
	else if (g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND == CTRL_AIRCRAFT_MOVE_VER_DOWN) /*�½�*/
	{
		g_psPidSystem->HighSpeed.controlOutput = math_Constrain(g_psPidSystem->HighSpeed.controlOutput, \
															    CTRL_HEIGHT_CLIMB_DOWN_MAX_ACCELERATION, -CTRL_HEIGHT_CLIMB_DOWN_MAX_ACCELERATION);		
	} 	
	
	/************************** ��ֱ�ٶȿ����� ���� ********************************/
	/*�߶ȿ�������3��*/
	/************
	 ************
              ***
             ***
              ***
               ***
        ***********
	 **************/
	/************************** ��ֱ���ٶȿ����� ��ʼ ********************************/
	/*������ֱ���ٶ�����*/
	g_psPidSystem->HighAcc.expect = g_psPidSystem->HighSpeed.controlOutput; /*�����⻷�������*/
	
	#ifdef CTRL_HEIGHT_YAW_SPEED_FEEDFORWARD /*�Ƿ������ٶ�ǰ��������*/
	g_psPidSystem->HighAcc.expect += g_HeightYawSpeedFeedforwardOutput; /*����ǰ�����������*/
	#endif
	
	/*������ֱ���ٶȷ���*/
	g_psPidSystem->HighAcc.feedback = g_psSinsFilterFeedback->curAcc[EARTH_FRAME_Z]; /*���ٶ�ԭʼ����У׼+������˹�˲����ֵ��Ϊ���ٶȷ���ֵ*/
	
	/*��ֱ���ٶȿ��Ƽ����*/
	g_psPidSystem->HighAcc.controlOutput = pid_Control_Err_LPF(&g_psPidSystem->HighAcc, PID_CONTROLER_HIGH_ACC);  /*PID ERR���Ƶ�ͨ�˲�*/
	/************************** ��ֱ���ٶȿ����� ���� ********************************/
	
	/*��ͣ���� = ���ٶȻ�����ֵ + ��׼��ͣ����
	  ��ʱ����� F = mg	(F����ͣ���Ų�����(��)��)
  	  ����Ҫ���a�ļ��ٶ�ʱ������� F1=m(g+a)
	  F1/F = 1 + a/g
	  ��˴�ʱӦ�������ͣ����*(1 + a/g)*/
	
	/*������Ҫ�ﵽ�ļ��ٶ���������Ҫ�����ӵ�������*/
	g_psPidSystem->HighAcc.controlOutput += (g_psControlAircraft->heightHoldThrottle + g_psPidSystem->HighAcc.integrate - CTRL_THROTTLE_START_TURN) * \
										    (g_psPidSystem->HighAcc.expect / 980);
	
	/*�Ƿ� ������������ģʽ: ����ֱ�߶�λ��+��ֱ�ٶ�+��ֱ���ٶ�:1����,0�ر�*/ 
	if (CTRL_HEIGHT_POS_CONTROL_ACC_STATUS == SYS_ENABLE)
	{
		/*������Դ�ڸ߶ȼ��ٶȿ��������*/
		g_psControlAircraft->ctrlThrottle = (u16)(g_psControlAircraft->heightHoldThrottle + \
												  g_psPidSystem->HighAcc.controlOutput);
	}
	else /*��������ģʽ:����ֱ�߶�λ��+��ֱ�ٶ�*/
	{
		/*������Դ�ڸ߶��ٶȿ��������*/
		g_psControlAircraft->ctrlThrottle = (u16)(g_psControlAircraft->heightHoldThrottle + \
												  g_psPidSystem->HighSpeed.controlOutput);		
	}
	
	/********************* �߶ȿ���������,�������ſ����� ���� ************************/		
}

/*����ˮƽλ�ÿ���*/
vu16 g_vu16LandHorizontalPosControlTicks   = 0;	/*����λ�ÿ��Ƽ�����*/
vu16 g_vu16LandHorizontalSpeedControlTicks = 0;	/*�����ٶȿ��Ƽ�����*/

void ctrl_Go_Home_Horizontal_Control(Vector2s_Nav targPos, Vector2s_Nav curPos, CTRL_GO_HOME_DISTANCE HOME_DISTANCE)
{
	/*�õ����Ŀ���N��E����ƫ��,������λ��ƫ��*/
	g_psControlLand->RelativeHomeDistance = land_Gps_Offset_Relative_To_Home(targPos, curPos);
	
	if (g_psUav_Status->UavLandStatus.ThisTime == UAV_LAND_NOT)
	{
		if (status_GPS_Fix_Ava_Check(g_psUav_Status) == UAV_SENMOD_USE_CONTROL_ALLOW)
		{
			/************************** ˮƽλ�ÿ����� ��ʼ ********************************/
			/*ң�ؾ���,ˮƽ������ң�ظ��������Ƕ�*/
			if ((g_psControlAircraft->RemotExpectAngle.pitch == 0) && \
				(g_psControlAircraft->RemotExpectAngle.roll == 0))
			{
				/*ticks++*/
				g_vu16LandHorizontalPosControlTicks++; /*5msִ��һ��*/
				
				/************************** ˮƽλ�ÿ����� ��ʼ ********************************/
				if (g_vu16LandHorizontalPosControlTicks >= CTRL_GPS_HORIZONTAL_POSITION_CONTROL_PERIOD) /*5msִ��һ��,20ms*/
				{
					/*ticks��0*/
					g_vu16LandHorizontalPosControlTicks = 0;
					
					/*λ������,��γ�������ٶȡ��߶�*/
					g_psPidSystem->LatitudePosition.expect  = g_psSinsReal->curPosition[EARTH_FRAME_Y];
					g_psPidSystem->LongitudePosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_X];
					
					/*��������ϵ��E��N������λ��ƫ��*/
					g_psAttitudeAll->EarthFramePosError.north = g_psControlLand->RelativeHomeDistance.north - g_psSinsReal->curPosition[EARTH_FRAME_Y];
					g_psAttitudeAll->EarthFramePosError.east  = g_psControlLand->RelativeHomeDistance.east - g_psSinsReal->curPosition[EARTH_FRAME_X];
					
					/*��������ϵ�»���Pitch��Roll������λ��ƫ��*/
					g_psAttitudeAll->BodyFramePosError.pitch = -g_psAttitudeAll->EarthFramePosError.east * SIN_YAW + \
																g_psAttitudeAll->EarthFramePosError.north * COS_YAW;
			
					g_psAttitudeAll->BodyFramePosError.roll  =  g_psAttitudeAll->EarthFramePosError.east * COS_YAW + \
																g_psAttitudeAll->EarthFramePosError.north * SIN_YAW;		
					
					/*���ݵ�ǰ�����HOME��ˮƽ������������ǰˮƽ�ٶ�*/
					if (HOME_DISTANCE == CTRL_GO_HOME_FARAWAY_DISTANCE)
					{
						g_psControlLand->horizontalSpeedExpect = CTRL_AUTO_GO_HOME_A2B_FIRST_NAV_SPEED_CM_S;
					}
					else if (HOME_DISTANCE == CTRL_GO_HOME_FARAWAY_DISTANCE)
					{
						g_psControlLand->horizontalSpeedExpect = CTRL_AUTO_GO_HOME_B2C_SECOND_NAV_SPEED_CM_S;					
					}
					else if (HOME_DISTANCE == CTRL_GO_HOME_FARAWAY_DISTANCE)
					{
						g_psControlLand->horizontalSpeedExpect = CTRL_AUTO_GO_HOME_NEARC_THIRD_NAV_SPEED_CM_S;						
					}
					else 
					{
						g_psControlLand->horizontalSpeedExpect = CTRL_AUTO_GO_HOME_DEFAULT_NAV_SPEED_CM_S;						
					}
				
					/*��������ϵ�»���Pitch��Roll,����������ɲ���ٶ�,����Ϊ���������㲻����PID���㺯��*/
					g_psAttitudeAll->BodyFrameBrakeSpeed.pitch = math_Constrain(g_psPidSystem->LatitudePosition.PID.kP * g_psAttitudeAll->BodyFramePosError.pitch, \
																				g_psControlLand->horizontalSpeedExpect, \
																			   -g_psControlLand->horizontalSpeedExpect); /*λ��ƫ���޷�,��λcm*/
		
					g_psAttitudeAll->BodyFrameBrakeSpeed.roll  = math_Constrain(g_psPidSystem->LongitudePosition.PID.kP * g_psAttitudeAll->BodyFramePosError.roll, \
																				g_psControlLand->horizontalSpeedExpect, \
																			   -g_psControlLand->horizontalSpeedExpect); /*λ��ƫ���޷�,��λcm*/
					
					/*����ˮƽ����Pitch��Roll�ٶȿ���������*/
					g_psPidSystem->LatitudeSpeed.expect  = g_psAttitudeAll->BodyFrameBrakeSpeed.pitch;  /*��γ-��y�᷽��(N��)*/					
					g_psPidSystem->LongitudeSpeed.expect = g_psAttitudeAll->BodyFrameBrakeSpeed.roll;   /*����-��x�᷽��(E��)*/			
						
					/******************** ˮƽλ�ÿ���������,��������ɲ���ٶ�(�����ٶ�) ���� *********************/
				}			
		
				/************************** ˮƽ�ٶȿ����� ��ʼ ********************************/				
				/*����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������*/
				/*������Pitch��Roll����ˮƽ�ٶȿ���*/
				g_vu16LandHorizontalSpeedControlTicks++; /*5msִ��һ��*/
			
				if (g_vu16LandHorizontalSpeedControlTicks >= CTRL_GPS_HORIZONTAL_SPEED_CONTROL_PERIOD) /*10ms*/
				{
					/*ticks��0*/
					g_vu16LandHorizontalSpeedControlTicks = 0;
				
					/*����ϵˮƽ�����ٶȷ�����*/
					g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psSinsReal->curSpeed[EARTH_FRAME_X] * SIN_YAW + \
																	 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * COS_YAW;
				
					g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psSinsReal->curSpeed[EARTH_FRAME_X] * COS_YAW + \
																     g_psSinsReal->curSpeed[EARTH_FRAME_Y] * SIN_YAW;					
				
					/*����������ˮƽ�����ٶȷ�����*/
					g_psPidSystem->LatitudeSpeed.feedback  = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*��γ-��y�᷽��(N��)*/		
					g_psPidSystem->LongitudeSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*����-��x�᷽��(E��)*/
				
					/*������ˮƽ�����ٶȿ��Ƽ����*/
					g_psPidSystem->LatitudeSpeed.controlOutput  = pid_Control_Div_LPF(&g_psPidSystem->LatitudeSpeed, PID_CONTROLER_LATITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/		
					g_psPidSystem->LongitudeSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->LongitudeSpeed, PID_CONTROLER_LONGITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/	
				
					/*ˮƽ�����������ٶȼ�ˮƽ�ٶȿ��ƻ������*/
					g_sHorizontalExpectAcc.y = -g_psPidSystem->LatitudeSpeed.controlOutput;  /*��ͷpitchǰ����*/
					g_sHorizontalExpectAcc.x =  g_psPidSystem->LongitudeSpeed.controlOutput; /*���ͷpitch��ֱroll���ҷ���*/
					
					/*ˮƽ�������ٶ��봹ֱ���������ٶȼ�����������*/
					horizontal_Acc_Convert_To_Dip_Angle(g_sHorizontalExpectAcc, &g_sHorizontalExpectAngle);
					
					/*���½ǶȻ�������(��Դ����ˮƽ���ٶȺ��������ٶȻ�������������)*/
					g_psPidSystem->PitchAngle.expect = g_sHorizontalExpectAngle.y;	/*��ͷpitchǰ����*/
					g_psPidSystem->RollAngle.expect  = g_sHorizontalExpectAngle.x;	/*���ͷpitch��ֱroll���ҷ���*/			
				}
			
				/************************** ˮƽ�ٶȿ����� ���� ********************************/
			}
			/*����ҡ��,ֻ����ˮƽ�ٶȿ���,��ˮƽλ�ÿ���*/
			else if ((g_psControlAircraft->RemotExpectAngle.pitch != 0) || \
					 (g_psControlAircraft->RemotExpectAngle.roll != 0)) 
			{
				/*ticks++*/
				g_vu16LandHorizontalSpeedControlTicks++; /*5msִ��һ��*/
					
				if (g_vu16LandHorizontalSpeedControlTicks >= CTRL_GPS_HORIZONTAL_SPEED_CONTROL_PERIOD) /*10ms*/
				{
					/*ticks��0*/
					g_vu16LandHorizontalSpeedControlTicks = 0;
				
					/*N��(��PITCH����)����ƶ��ٶ�*/
					g_psPidSystem->LatitudeSpeed.expect  = g_psControlAircraft->RemotExpectAutoAngle.pitch * \
														   CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�45*6/100=3.6m/s*/

					/*E��(��ROLL����)����ƶ��ٶ�*/
					g_psPidSystem->LongitudeSpeed.expect = g_psControlAircraft->RemotExpectAutoAngle.roll * \
														   CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�45*6/100=3.6m/s*/						
				
					/*����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������*/
					/*������Pitch��Roll����ˮƽ�ٶȿ���*/
					g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psSinsReal->curSpeed[EARTH_FRAME_X] * SIN_YAW + \
																	 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * COS_YAW;

					g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psSinsReal->curSpeed[EARTH_FRAME_X] * COS_YAW + \
																	 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * SIN_YAW;
		
					/*����ˮƽ�ٶȷ���*/
					g_psPidSystem->LatitudeSpeed.feedback  = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*��γ-��y�᷽��(N��)(��pitch��ת��������)*/	
					g_psPidSystem->LongitudeSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*����-��x�᷽��(E��)(��roll��ת��������)*/
				
					/*ˮƽ�ٶ�PID���㼰��� (ˮƽ����)*/
					g_psPidSystem->LatitudeSpeed.controlOutput  = pid_Control_Div_LPF(&g_psPidSystem->LatitudeSpeed, PID_CONTROLER_LATITUDE_SPEED);    /*PID DIV���Ƶ�ͨ�˲�*/
					g_psPidSystem->LongitudeSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->LongitudeSpeed, PID_CONTROLER_LONGITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/	
				
					/*ˮƽ�����������ٶȼ�ˮƽ�ٶȿ��ƻ������*/
					g_sHorizontalExpectAcc.y = -g_psPidSystem->LatitudeSpeed.controlOutput;  /*��ͷpitchǰ����*/
					g_sHorizontalExpectAcc.x =  g_psPidSystem->LongitudeSpeed.controlOutput; /*���ͷpitch��ֱroll���ҷ���*/
				
					/*ˮƽ�������ٶ��봹ֱ���������ٶȼ�����������*/
					horizontal_Acc_Convert_To_Dip_Angle(g_sHorizontalExpectAcc, &g_sHorizontalExpectAngle);
				
					/*���½ǶȻ�������(��Դ����ˮƽ���ٶȺ��������ٶȻ�������������)*/
					g_psPidSystem->PitchAngle.expect = g_sHorizontalExpectAngle.y;	/*��ͷpitchǰ����*/
					g_psPidSystem->RollAngle.expect  = g_sHorizontalExpectAngle.x;	/*���ͷpitch��ֱroll���ҷ���*/				
				}
			}
		}
	}
	else if (g_psUav_Status->UavLandStatus.ThisTime == UAV_LAND_YES)		
	{
		/*ˮƽ������̬��Ϊ0*/
		g_psPidSystem->PitchAngle.expect = 0;
		g_psPidSystem->RollAngle.expect  = 0;
		
		/*���ǰ��ջ��ֿ�����*/
		pid_Horizontal_Takeoff_Integrate_Reset();		
	}
}


/*��������*/
void ctrl_Go_Home_Control(fp32 controlDeltaT)
{
	/*GPS������HOME������,�Ҵ��ڶ���ģʽ,ִ�з���*/
	if ((status_GPS_Fix_Ava_Check(g_psUav_Status) == UAV_SENMOD_USE_CONTROL_ALLOW) && \
		(g_psUav_Status->UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_FIX_POS))
	{
		/*GPS���뷵��*/
		ctrl_Go_Home_GPS_Control(controlDeltaT);
	}
	/*GPS��Чʱ,ִ�н���*/
	else
	{
		/*��ͨԭ����½����*/
		ctrl_Land_Ground_Control(controlDeltaT, g_psUav_Status);
	}
}

/*��ⷵ��ʱ�ֶ���Ԥ�߶�*/
CTRL_GO_HOME_HAND_MEDDLE_STATUS ctrl_Go_Home_Vertical_Hand_Meddle(void)
{
	CTRL_GO_HOME_HAND_MEDDLE_STATUS verticalStatus = CTRL_GO_HOME_HAND_MEDDLE_FALSE;
	
	/*�ж��Ƿ�����ֱ������*/
	if ((g_psControlAircraft->throttleUpDownJudgeValue <= REMOT_THROTTLE_DOWN_CROSSOVER) || \
		(g_psControlAircraft->throttleUpDownJudgeValue >= REMOT_THROTTLE_UP_CROSSOVER))
	{
		verticalStatus = CTRL_GO_HOME_HAND_MEDDLE_TRUE;
	}	
	
	return verticalStatus;	
}

/*��ⷵ��ʱ�ֶ���Ԥˮƽ*/
CTRL_GO_HOME_HAND_MEDDLE_STATUS ctrl_Go_Home_Horizontal_Hand_Meddle(void)
{
	CTRL_GO_HOME_HAND_MEDDLE_STATUS horizontalStatus = CTRL_GO_HOME_HAND_MEDDLE_FALSE;
	
	/*�ж��Ƿ���ˮƽ������*/
	if ((g_psControlAircraft->RemotExpectAngle.roll != 0) || \
		(g_psControlAircraft->RemotExpectAngle.pitch != 0))
	{
		horizontalStatus = CTRL_GO_HOME_HAND_MEDDLE_TRUE;
	}	
	
	return horizontalStatus;
}

/*������״̬����*/
void ctrl_Go_Home_Status_Update(void)
{
	/*��ǰ�����HOME��ֱ�߾���*/
	g_psAttitudeAll->gpsRelativeHome = gps_Two_Pos_Segment_Distance(g_psAttitudeAll->HomePos.Coordinate_s4, g_psAttitudeAll->GpsData.Coordinate_s4) * 100.0f; /*cm*/
	
	/*�����ϴη�������*/
	g_psControlLand->LAST_GO_HOME_PROCESS = g_psControlLand->CUR_GO_HOME_PROCESS;
	
	/*��HOME��ˮƽ���볬�� FARAWAY_DISTANCE �뾶*/
	if (g_psAttitudeAll->gpsRelativeHome >= CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_CM)
	{
		/*Զ��HOME��,����������ȫ�߶�,����*/
		g_psControlLand->CUR_GO_HOME_PROCESS = CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME;
		
		/*���������궨:ңԶ*/
		g_psControlLand->HOME_DISTANCE = CTRL_GO_HOME_FARAWAY_DISTANCE;
	}
	/*��HOME��ˮƽ���볬�� NEAR �뾶*/
	else if (g_psAttitudeAll->gpsRelativeHome >= CTRL_AUTO_GO_HOME_NEAR_DISTANCE_CM)
	{
		/*����HOME��,���ְ�ȫ�߶�,����*/
		g_psControlLand->CUR_GO_HOME_PROCESS = CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME;			
		
		/*���������궨:����*/
		g_psControlLand->HOME_DISTANCE = CTRL_GO_HOME_NEAR_DISTANCE;		
	}
	/*��HOME��ˮƽ����С�� NEAR �뾶,����HOME��*/	
	else
	{
		/*HOME�㸽��(Ĭ�ϵ���HOME��),����*/
		g_psControlLand->CUR_GO_HOME_PROCESS = CTRL_LAND_ARRIVE_DESCEND_GO_HOME;

		/*���������궨:����*/
		g_psControlLand->HOME_DISTANCE = CTRL_GO_HOME_ARRIVE_DISTANCE;		
	}
	
	/*�ж�ˮƽλ�÷������޸�Ԥ*/
	g_psControlLand->HORIZONTAL_HAND_MEDDLE_STATUS = ctrl_Go_Home_Horizontal_Hand_Meddle();	
	
	/*�ж���ֱ�߶ȷ������޸�Ԥ*/
	g_psControlLand->VERTICAL_HAND_MEDDLE_STATUS = ctrl_Go_Home_Vertical_Hand_Meddle();
	
	/*ˮƽ����ģʽ����*/
	g_psControlLand->LAST_HORIZONTAL_CTRL_MODE = g_psControlLand->CUR_HORIZONTAL_CTRL_MODE;
	
	/*���ڷ���������,ˮƽλ�����ֶ���Ԥ*/
	if (g_psControlLand->HORIZONTAL_HAND_MEDDLE_STATUS == CTRL_GO_HOME_HAND_MEDDLE_TRUE)
	{
		/*���Զ�����������ΪNULL,��һ��ѭ���������ж�*/
		g_psControlLand->LAST_GO_HOME_PROCESS = CTRL_GO_HOME_NULL_PROCESS;
		
		/*�����������HOME����궨Ϊ��Ч,��һ��ѭ���������ж�*/
		g_psControlLand->HOME_DISTANCE = CTRL_GO_HOME_NULL_DISTANCE;
		
		/*��Ǳ���ˮƽ����Ϊ�ֶ�ģʽ*/
		g_psControlLand->CUR_HORIZONTAL_CTRL_MODE = CTRL_GO_HOME_HAND_CTRL;
		
		/*����Ϊ��һ��(���ֶ�����/�����ֶ������)�л�����ģʽ,*/
		g_psControlLand->FIRST_SWITCH_STATUS = CTRL_GO_HOME_FIRST_SWITCH_TURE;

		/*�ֶ���Ԥ��,�궨��ȫ�߶�δ����,�ȴ��������ð�ȫ�߶�*/
		g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE;		
	}
	else
	{
		/*��Ǳ���ˮƽ����Ϊ�Զ�ģʽ*/
		g_psControlLand->CUR_HORIZONTAL_CTRL_MODE = CTRL_GO_HOME_AUTO_CTRL;
	}
	
	/*���ڷ���������,��ֱ�߶����ֶ���Ԥ*/
	if (g_psControlLand->VERTICAL_HAND_MEDDLE_STATUS == CTRL_GO_HOME_HAND_MEDDLE_TRUE)
	{
		/*���Զ�����������ΪNULL,��һ��ѭ���������ж�*/		
		g_psControlLand->LAST_GO_HOME_PROCESS = CTRL_GO_HOME_NULL_PROCESS;	
		
		/*�����������HOME����궨Ϊ��Ч,��һ��ѭ���������ж�*/
		g_psControlLand->HOME_DISTANCE = CTRL_GO_HOME_NULL_DISTANCE;		
		
		/*����Ϊ��һ��(���ֶ�����/�����ֶ������)�л�����ģʽ,*/
		g_psControlLand->FIRST_SWITCH_STATUS = CTRL_GO_HOME_FIRST_SWITCH_TURE;
		
		/*�ֶ���Ԥ��,�궨��ȫ�߶�δ����,�ȴ��������ð�ȫ�߶�*/
		g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE;
	}	
}

/*��������GPS*/
void ctrl_Go_Home_GPS_Control(fp32 controlDeltaT)
{
	/*��¼��λ������*/
	g_psControlLand->LandHomePosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
	g_psControlLand->LandHomePosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;	
	
	/*���µ�ǰλ������*/
	g_psControlLand->LandCurPosition.lat = g_psAttitudeAll->GpsData.Coordinate_s4.lat;
	g_psControlLand->LandCurPosition.lon = g_psAttitudeAll->GpsData.Coordinate_s4.lon;
	
	/*�������µ�ǰ����HOME��λ�ù�ϵ*/
	ctrl_Go_Home_Status_Update();
	
	/*δ�ŵ�,����̬���Ʋ���*/
	if (g_psUav_Status->UavLandStatus.ThisTime != UAV_LAND_YES)
	{
		/*��ˮƽ��˶���*/
		if (g_psControlLand->HORIZONTAL_HAND_MEDDLE_STATUS == CTRL_GO_HOME_HAND_MEDDLE_FALSE)
		{
			/*�״����Զ�����,��Զ��HOME��ʱ,�����������õİ�ȫ�߶�*/
			if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME) && \
				(g_psControlLand->FIRST_SWITCH_STATUS == CTRL_GO_HOME_FIRST_SWITCH_TURE))
			{
				/*�����ǰ�߶ȵ���Զ���뷵���߶�,���������,ֱ��������ֱ��ȫ�߶�*/
				if (g_psSinsReal->curPosition[EARTH_FRAME_Z] < CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_SAFE_HEIGHT_CM)
				{
					/*ˮƽ����,��ֱ����*/
					g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->GpsData.Coordinate_s4.lat;
					g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->GpsData.Coordinate_s4.lon;
					
					/*�ٶȱ仯�����,Ȼ��ݼ�*/
					g_psControlLand->verticalCalimbSpeedRate = CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_CLIMB_CM_S;
					
					/*������ֱ�����ٶȱ仯����*/
					g_psControlLand->verticalSpeedChangePeriod = 0;
					
					/*�л�ʱ���ٶ�����Ϊ0*/
					g_psControlLand->verticalSpeedExpect = 0;
					
					/*Ŀ��߶�����Ϊ0*/
					g_psControlLand->heightExpect = 0;
					
					/*���δ�ﰲȫ�߶�*/
					g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE;
				}
				/*���з���[**˲��**]�ĸ߶ȴ��ڰ�ȫ�߶�ʱ,���ֵ�ǰ�߶�,ִ�з���*/
				else
				{
					/*ˮƽ��HOME�㿿£,�߶ȱ��ְ�ȫ�߶�*/
					g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
					g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;
					
					/*�л�ʱ���ٶ�����Ϊ0*/
					g_psControlLand->verticalSpeedExpect = 0;
					
					/*����Ŀ��߶�����Ϊ��ȫ�߶�*/
					g_psControlLand->heightExpect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
					
					/*����Ѵﰲȫ�߶�*/
					g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_ARRIVE;					
				}

				/*Զ��������ð�ȫ�߶�/�л�ʱ���ڰ�ȫ�߶ȼ������������,�´β�������*/
				g_psControlLand->FIRST_SWITCH_STATUS = CTRL_GO_HOME_FIRST_SWITCH_FALSE;
				
				/*ȷ���������һ�׶�,��ֹ�ٽ�λ���ж�ʧ��,�޷�������һ�׶�*/
				g_psControlLand->LAST_GO_HOME_PROCESS = CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME;
			}
			/*��������ңԶλ��*/
			else if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME) && \
					 (g_psControlLand->LAST_GO_HOME_PROCESS == CTRL_LAND_FARAWAY_CLIMB_SAFE_HEIGHT_GO_HOME))
			{
				/*��������ǰʱ���ѷ�����ȫ�߶�,��δ��������*/
				if ((g_psSinsReal->curPosition[EARTH_FRAME_Z] >= CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_SAFE_HEIGHT_CM) && \
					(g_psControlLand->SAFE_HEIGHT_STATUS == CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE))
				{
					/*����Ѵﰲȫ�߶�*/
					g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_ARRIVE;

					/*���ø߶�����������ˮƽλ�ÿ�£�Ĺ���ʱ��(2s)*/
					g_psControlLand->climb2CloseTransitionTicks = CTRL_AUTO_GO_HOME_CLIMB_THEN_CLOSE_TRANSITION_TICKS;
					
					/*��ǻ���ʱ���ڵķ���״̬δ����*/
					g_psControlLand->TRANSITION_SET_STATUS = CTRL_GO_GOME_TRANSITION_SET_NO;
				}
				/*��������HOME���Զ,δ�ﵽ��ȫ�߶�,��Ȼ���ڱ���ԭˮƽλ��,��������*/
				else if (g_psControlLand->SAFE_HEIGHT_STATUS == CTRL_GO_HOME_SAFE_HEIGHT_DISARRIVE)
				{	
					/*���ʱ仯����ticks++*/
					g_psControlLand->verticalSpeedChangePeriod++;
					
					/*8 * 5ms = 40ms�ٶȱ仯�ʸı�һ��*/
					if (g_psControlLand->verticalSpeedChangePeriod >= CTRL_AUTO_GO_HOME_VERTICAL_SPEED_CHANGE_MAX_PERIOD)
					{
						if (g_psControlLand->verticalCalimbSpeedRate >= 1)
						{
							g_psControlLand->verticalCalimbSpeedRate--;
						}
						else
						{
							g_psControlLand->verticalCalimbSpeedRate = 0;
						}
						
						g_psControlLand->verticalSpeedChangePeriod = 0;
					}
					
					/*�����ٶ����ε������������ſ�����ͻ��*/
					g_psControlLand->verticalSpeedExpect = CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_CLIMB_CM_S - \
														   g_psControlLand->verticalCalimbSpeedRate;
					
					/*Ŀ��߶�����Ϊ0*/
					g_psControlLand->heightExpect = 0;					
				}
				
				/*��������HOME���Զ,�Ѵﵽ��ȫ�߶�*/
				if (g_psControlLand->SAFE_HEIGHT_STATUS == CTRL_GO_HOME_SAFE_HEIGHT_ARRIVE)
				{
					/*��ֱ�߶ȵ��ﰲȫ�߶�,��Ŀ���ˮƽ�����ĵȴ�����ʱ��,5ms��һ��*/
					if (g_psControlLand->climb2CloseTransitionTicks >= 1)
					{
						g_psControlLand->climb2CloseTransitionTicks--;
					}
					else
					{
						g_psControlLand->climb2CloseTransitionTicks = 0;
					}
					
					/*����ʱ���ѽ���,���÷�����*/
					if (g_psControlLand->climb2CloseTransitionTicks == 0)
					{
						/*ˮƽ��HOME�㿿£,�߶ȱ��ְ�ȫ�߶�*/
						g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
						g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;
						
						/*������ֱ�����ٶ�Ϊ0*/
						g_psControlLand->verticalSpeedExpect = 0;
						
						/*���ֵ�ǰĿ��߶�*/
						g_psControlLand->heightExpect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
						
						/*���Ŀ��߶��ѵ���,���״̬������,����������*/
						g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_NULL;
					}
					/*��ֱ�߶ȴﵽ��ȫ�߶�,��ˮƽ��£�Ĺ���ʱ����,��Ҫ��ͣ�ڵ�ǰλ��*/
					else if (g_psControlLand->climb2CloseTransitionTicks != 0)
					{
						/*����ʱ���ڵĹ���״̬δ����,������(��ά����),���ȴ�����ʱ�����,����HOME�㿿��*/
						if (g_psControlLand->TRANSITION_SET_STATUS == CTRL_GO_GOME_TRANSITION_SET_NO)
						{
							/*��ǹ���ʱ���ڵ�״̬������*/
							g_psControlLand->TRANSITION_SET_STATUS = CTRL_GO_GOME_TRANSITION_SET_OK;
							
							/*������άλ������*/
							g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->GpsData.Coordinate_s4.lat;
							g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->GpsData.Coordinate_s4.lon;
							g_psControlLand->heightExpect           = g_psSinsReal->curPosition[EARTH_FRAME_Z];
							
							/*������ֱ�ٶ�����Ϊ0*/
							g_psControlLand->verticalSpeedExpect = 0;
						}
					}
				}
			}
			/*�״��з���ģʽ,����home�Ͻ�ʱ,���ֵ�ǰ�߶�,��Home����ΪĿ���,����home���Ϸ�*/
			else if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME) && \
					 (g_psControlLand->LAST_GO_HOME_PROCESS != CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME))
			{
				/*Ŀ������ΪHOMEλ��*/
				g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
				g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;
				
				/*������ֱ�ٶ�����Ϊ0*/
				g_psControlLand->verticalSpeedExpect = 0;
				
				/*Ŀ��߶�����Ϊ��ǰ�߶�*/
				g_psControlLand->heightExpect = g_psSinsReal->curPosition[EARTH_FRAME_Z];
				
				/*���Ŀ��߶��ѵ���,���״̬������,����������*/
				g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_NULL;		

				/*ȷ���������һ�׶�,��ֹ�ٽ�λ���ж�ʧ��,�޷�������һ�׶�*/
				g_psControlLand->LAST_GO_HOME_PROCESS = CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME;
			}
			/*������HOME�㸽��*/
			else if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME) && \
					 (g_psControlLand->LAST_GO_HOME_PROCESS == CTRL_LAND_NEAR_HOLD_HEIGHT_GO_HOME))
			{
				/*Ŀ������ΪHOMEλ��*/
				g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
				g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;	
				
				/*������ֱ�ٶ�����Ϊ0*/
				g_psControlLand->verticalSpeedExpect = 0;
				
				/*���Ŀ��߶��ѵ���,���״̬������,����������*/
				g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_NULL;		
			}
			/*�״ν���HOME�����Ϸ�*/
			else if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_ARRIVE_DESCEND_GO_HOME) && \
					 (g_psControlLand->LAST_GO_HOME_PROCESS != CTRL_LAND_ARRIVE_DESCEND_GO_HOME))
			{
				/*Ŀ������ΪHOMEλ��*/
				g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
				g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;
				
				/*�ٶȱ仯�����,Ȼ��ݼ�*/
				g_psControlLand->verticalDescendSpeedRate = CTRL_AUTO_GO_HOME_ARRIVE_DESCEND_CM_S;
					
				/*������ֱ�ٶ�����Ϊ0*/
				g_psControlLand->verticalSpeedExpect = 0;
				
				/*������ֱ�½��ٶȱ仯����*/
				g_psControlLand->verticalSpeedChangePeriod = 0;
				
				/*������ֱλ������Ϊ0*/
				g_psControlLand->heightExpect = 0;
				
				/*�½�����,�������������״̬*/
				g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_NULL;				
			}
			/*������HOME�����Ϸ�*/			
			else if ((g_psControlLand->CUR_GO_HOME_PROCESS == CTRL_LAND_ARRIVE_DESCEND_GO_HOME) && \
					 (g_psControlLand->LAST_GO_HOME_PROCESS == CTRL_LAND_ARRIVE_DESCEND_GO_HOME))
			{
				/*Ŀ������ΪHOMEλ��*/
				g_psControlLand->LandTargetPosition.lat = g_psAttitudeAll->HomePos.Coordinate_s4.lat;
				g_psControlLand->LandTargetPosition.lon = g_psAttitudeAll->HomePos.Coordinate_s4.lon;
				
				/*���ʱ仯����ticks++*/
				g_psControlLand->verticalSpeedChangePeriod++;
				
				/*8 * 5ms = 40ms�ٶȱ仯�ʸı�һ��*/
				if (g_psControlLand->verticalSpeedChangePeriod >= CTRL_AUTO_GO_HOME_VERTICAL_SPEED_CHANGE_MAX_PERIOD)
				{
					if (g_psControlLand->verticalDescendSpeedRate <= -1)
					{
						g_psControlLand->verticalDescendSpeedRate++;
					}
					else
					{
						g_psControlLand->verticalDescendSpeedRate = 0;
					}
						
					g_psControlLand->verticalSpeedChangePeriod = 0;
				}
					
				/*�½��ٶ����ε���,�������ſ�����ͻ��*/
				g_psControlLand->verticalSpeedExpect = CTRL_AUTO_GO_HOME_ARRIVE_DESCEND_CM_S - \
													   g_psControlLand->verticalDescendSpeedRate;				
				
				/*������ֱ�����߶�Ϊ0*/
				g_psControlLand->heightExpect = 0;

				/*�½�����,�������������״̬*/
				g_psControlLand->SAFE_HEIGHT_STATUS = CTRL_GO_HOME_SAFE_HEIGHT_NULL;
			}
			
			/*����ˮƽλ�ÿ���*/
			ctrl_Go_Home_Horizontal_Control(g_psControlLand->LandTargetPosition, g_psControlLand->LandCurPosition, g_psControlLand->HOME_DISTANCE); 
			
			/*������ֱ�߶ȿ���*/
			ctrl_Go_Home_Vertical_Control(g_psControlLand->verticalSpeedExpect, g_psControlLand->heightExpect, controlDeltaT); 
		}
		/*��ˮƽ��˶���*/
		else if (g_psControlLand->HORIZONTAL_HAND_MEDDLE_STATUS == CTRL_GO_HOME_HAND_MEDDLE_TRUE)
		{
			/*����ˮƽλ�ÿ���*/
			ctrl_Go_Home_Horizontal_Control(g_psControlLand->LandTargetPosition, g_psControlLand->LandCurPosition, g_psControlLand->HOME_DISTANCE); 			
			
			/*�ֶ�����*/
			if ((g_psControlLand->CUR_HORIZONTAL_CTRL_MODE == CTRL_GO_HOME_HAND_CTRL) && \
				(g_psControlLand->LAST_HORIZONTAL_CTRL_MODE == CTRL_GO_HOME_AUTO_CTRL))
			{
				/*������ֱ�ٶ�����Ϊ0*/
				g_psControlLand->verticalSpeedExpect = 0;
				
				/*Ŀ��߶�Ϊ�ֶ�������ĸ߶�*/
				g_psControlLand->heightExpect = g_psSinsReal->curPosition[EARTH_FRAME_Z];				
			}
			/*������ȫ���ֶ��ӹ�*/
			else if ((g_psControlLand->CUR_HORIZONTAL_CTRL_MODE == CTRL_GO_HOME_HAND_CTRL) && \
					 (g_psControlLand->LAST_HORIZONTAL_CTRL_MODE == CTRL_GO_HOME_HAND_CTRL))
			{
				/*������ֱ�ٶ�����Ϊ0*/
				g_psControlLand->verticalSpeedExpect = 0;			
			}
			
			/*������ֱ�߶ȿ���*/
			ctrl_Go_Home_Vertical_Control(g_psControlLand->verticalSpeedExpect, g_psControlLand->heightExpect, controlDeltaT); 
		}
	}
	/*���������ŵ�*/
	else if (g_psUav_Status->UavLandStatus.ThisTime == UAV_LAND_YES)
	{
		/*ˮƽ�����Ǹ�0*/
		g_psPidSystem->PitchAngle.expect = 0;
		g_psPidSystem->RollAngle.expect  = 0;

		/*���ǰ��ջ��ֿ�����*/
		pid_Horizontal_Takeoff_Integrate_Reset();
		
		/*��2*Nav_Rapid_Decline_rate�ٶ��½�,ʹ��������Ѹ�����㵡������*/
		g_psControlLand->verticalSpeedExpect = 2 * CTRL_AUTO_GO_HOME_RAPID_DESCEND_CM_S;
		
		/*��ֱ�߶�����Ϊ0*/
		g_psControlLand->heightExpect = 0;
		
		/*������ֱ�߶ȿ���*/
		ctrl_Go_Home_Vertical_Control(g_psControlLand->verticalSpeedExpect, g_psControlLand->heightExpect, controlDeltaT); 		
	}
}

/*��ͨԭ����½����(NO GPS)*/
void ctrl_Land_Ground_Control(fp32 controlDeltaT, Uav_Status *uavStatus)
{
	/*δ�ŵ�,����̬���Ʋ���*/
	if (uavStatus->UavLandStatus.ThisTime != UAV_LAND_YES)		
	{
		/*����ˮƽ���ȿ����µ�ROLL(���)��PITCH(����)������*/
		g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch; /*ң���ڴ����Ƚ�pitch*/
		g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;	/*ң���ڴ����Ƚ�roll*/
		
		/*���ݵ�ǰ�߶�,�����������ٶ�,��ȫ�߶������,��ȫ�߶�������*/
		if (g_psSinsReal->curPosition[EARTH_FRAME_Z] <= CTRL_AUTO_GO_HOME_FARAWAY_DISTANCE_SAFE_HEIGHT_CM)
		{
			g_psControlLand->verticalSpeedExpect = CTRL_AUTO_GO_HOME_ARRIVE_DESCEND_CM_S;	/*����*/
		}
		else 
		{
			g_psControlLand->verticalSpeedExpect = CTRL_AUTO_GO_HOME_RAPID_DESCEND_CM_S;	/*����*/
		}
		
		/*��ֱ�߶�����Ϊ0*/
		g_psControlLand->heightExpect = 0;
		
		ctrl_Go_Home_Vertical_Control(g_psControlLand->verticalSpeedExpect, g_psControlLand->heightExpect, controlDeltaT);
	}
	/*���������ŵ�*/
	else if (uavStatus->UavLandStatus.ThisTime == UAV_LAND_YES)
	{
		/*ˮƽ�����Ǹ�0*/
		g_psPidSystem->PitchAngle.expect = 0;
		g_psPidSystem->RollAngle.expect  = 0;

		/*���ǰ��ջ��ֿ�����*/
		pid_Horizontal_Takeoff_Integrate_Reset();		
		
		/*��2*Nav_Rapid_Decline_rate�ٶ��½�,ʹ��������Ѹ�����㵡������*/
		g_psControlLand->verticalSpeedExpect = 2 * CTRL_AUTO_GO_HOME_RAPID_DESCEND_CM_S;
		
		/*��ֱ�߶�����Ϊ0*/
		g_psControlLand->heightExpect = 0;
		
		ctrl_Go_Home_Vertical_Control(g_psControlLand->verticalSpeedExpect, g_psControlLand->heightExpect, controlDeltaT);		
	}
}


/*�õ����Ŀ������Pit��Rol����ƫ��*/
Vector2f_Nav land_Gps_Offset_Relative_To_Home(Vector2s_Nav targPos, Vector2s_Nav curPos)
{
	Vector2f_Nav twoPosDealt;
	static s32 lastLandLat = 0;
	static fp32 landScale = 1.0f;
	
	/*�Ƚ�����γ�����ֵ�������ظ��������Һ���*/
	if (math_Abs(lastLandLat - targPos.lat) < 100000)
	{
		/*����scale*/
	}
	else
	{
		landScale = cosf(targPos.lat * 1.0e-7f * DEG2RAD);
		landScale = math_Constrain(landScale, 1.0f, 0.01f);
	}
	
	lastLandLat = targPos.lat;
	
	twoPosDealt.north = (targPos.lat - curPos.lat) * GPS_LOCATION_SCALING_FACTOR;				/*m*/
	twoPosDealt.east  = (targPos.lon - curPos.lon) * GPS_LOCATION_SCALING_FACTOR * landScale;	/*m*/
	
	/***********************************************************************************
	��ȷ�µ���ϵ������������������Ϊ������:
	������������������,�����˻����home�㣬���������ƶ�ʱ����ʱGPS_Present.lng>GPS_Home.lng���õ���location_delta.x����0;
	����������γ������,�����˻����home�㣬���������ƶ�ʱ����ʱGPS_Present.lat>GPS_Home.lat���õ���location_delta.y����0;
	******************************************************************************/
	twoPosDealt.north *= 100;//�ص�������ϵ����������λ��ƫ��,��λΪCM
	twoPosDealt.east  *= 100;//�ص�������ϵ����������λ��ƫ��,��λΪCM
	
	return twoPosDealt;	
}
