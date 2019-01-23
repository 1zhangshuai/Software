#include "control_PosHold.h"
#include "control_Aircraft.h"

Vector2f_Nav g_sUavGpsStopPoint = {0}; 		  /*GPS����ģʽ����ϵ�е�ɲ��ֹͣ��*/
Vector2f g_sUavOpticFlowStopPoint = {0}; 		  /*��������ģʽ��ɲ��ֹͣ��*/

Vector2f g_sHorizontalExpectAcc   = {0}; /*ˮƽ�������ٶ�*/
Vector2f g_sHorizontalExpectAngle = {0}; /*ˮƽ�����Ƕ�*/

/*����ģʽ��,ң�˻��к�,����ˮƽ�ٶȿ���ɲ��,��ɲͣ���ٸ�ֵλ��ѡ��*/
SYS_RETSTATUS horizontal_GPS_Get_Stop_Point_XY(Vector2f_Nav *stopPoint)
{
	Vector2f_Nav curPos, curSpeed, curAcc;      /*����ϵ�µ�ǰλ��,�ٶ�,���ٶ�*/
	fp32 resultantSpeed = 0, resultantAcc = 0;	/*���ٶ�,�ϼ��ٶ�*/
	
	/*��ǰˮƽ����(x,y)��λ��,�ٶ�,���ٶ�*/
	curPos.north   = g_psSinsReal->curPosition[EARTH_FRAME_Y];
	curPos.east    = g_psSinsReal->curPosition[EARTH_FRAME_X];
	curSpeed.north = g_psSinsReal->curSpeed[EARTH_FRAME_Y];
	curSpeed.east  = g_psSinsReal->curSpeed[EARTH_FRAME_X];	
	curAcc.north   = g_psSinsReal->curAcc[EARTH_FRAME_Y];
	curAcc.east    = g_psSinsReal->curAcc[EARTH_FRAME_X];

	/*���ٶȺͺϼ��ٶ�*/
	resultantSpeed = pythagorous2(curSpeed.north, curSpeed.east);
	resultantAcc   = pythagorous2(curAcc.north, curAcc.east);
	
	/*��ˮƽ�ٶ�С�ڵ���20cm/s
	  ��ˮƽ���ٶ�С�ڵ���40cm/s^2
	  Cos_Pitch*Cos_Roll����������ˮƽ��̬ԼΪ15deg����������ˮƽ��̬��С��10deg(rMatrix[2][2] = COS_PITCH * COS_ROLL)*/
	if ((resultantSpeed <= 20.0f) && (resultantAcc <= 40.0f) && (rMatrix[2][2] >= 0.97f))
	{
		stopPoint->north = curPos.north;
		stopPoint->east  = curPos.east;
		
		return SYS_RET_SUCC;
	}
	
	return SYS_RET_FAIL;
}

/*����ģʽ��,ң�˻��к�,����ˮƽ�ٶȿ���ɲ��,��ɲͣ���ٸ�ֵ����λ��ѡ��*/
SYS_RETSTATUS horizontal_OpticFlow_Get_Stop_Point_XY(Vector2f *stopPoint)
{
	Vector2f curSpeed, curAcc; /*��ǰλ��,�ٶ�,���ٶ�*/
	fp32 resultantSpeed = 0, resultantAcc = 0;	/*���ٶ�,�ϼ��ٶ�*/
	
	/*��ǰˮƽ����(x,y)���ٶ�,���ٶ�*/
	curSpeed.x = g_psAttitudeAll->OpticFlowData.RealSpeed.x;				/*����ˮƽ�ٶ�*/
	curSpeed.y = g_psAttitudeAll->OpticFlowData.RealSpeed.y;		
	curAcc.x   = g_psSinsFilterFeedback->curAcc[EARTH_FRAME_X];				/*ˮƽ���ٶ�*/
	curAcc.y   = g_psSinsFilterFeedback->curAcc[EARTH_FRAME_Y];

	/*���ٶȺͺϼ��ٶ�*/
	resultantSpeed = pythagorous2(curSpeed.x, curSpeed.y);
	resultantAcc   = pythagorous2(curAcc.x, curAcc.y);
	
	/*��ˮƽ�ٶȵ�С�ڵ���20cm/s
	  ��ˮƽ���ٶȵ�С�ڵ���40cm/s^2
	  Cos_Pitch*Cos_Roll����������ˮƽ��̬ԼΪ15deg����������ˮƽ��̬��ԼΪ10deg*/
	if ((resultantSpeed <= 20.0f) && (resultantAcc <= 40.0f) && (rMatrix[2][2] >= 0.97f))
	{
		/*��������,ɲ����,����λ��Ϊ0*/
		stopPoint->x = 0;
		stopPoint->y = 0;
		
		return SYS_RET_SUCC;
	}
	
	return SYS_RET_FAIL;	
}

/*ˮƽ����ļ��ٶȻ������*/
Vector2f *horizontal_Acc_Convert_To_Dip_Angle(Vector2f acc2f, Vector2f *angle2f)
{
	/*��ͷ�������ٶ�(ǰ���˶����ٶ�),���ͷ�����ᴹֱ���ٶ�(�����˶����ٶ�)*/
	fp32 accY, accX; 
	fp32 maxDipAngle = REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX;
	
	accY = acc2f.y; /*��ͷ������Ϊǰ���ٶ�*/
	accX = acc2f.x; /*���ͷ�����ᴹֱ��Ϊ�����ٶ�*/
	
	/*����ǰ�������˶����ٶ�(cm/s^2)�ʹ�ֱ���������ٶ�(cm/s^2)�ļн���,��������*/
	angle2f->y = math_Constrain(math_fast_atan((accY * COS_ROLL) / (GRAVITY_STD * 100)) * RAD2DEG, maxDipAngle, -maxDipAngle); /*pitch*/
	angle2f->x = math_Constrain(math_fast_atan(accX / (GRAVITY_STD * 100)) * RAD2DEG, maxDipAngle, -maxDipAngle);			   /*roll*/
	
	return (angle2f);
}

/*ˮƽλ�ÿ�����*/
void horizontal_Control_PosHold(fp32 controlDeltaT)
{
	/*GPS����ģʽ(�ж���ʱ�ж�)*/
	if (g_sUav_Status.UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_GPS)
	{
		horizontal_Control_GPS_PosHold(controlDeltaT);
	}
	/*��������ģʽ(�ж���ʱ�ж�)*/
	else if (g_sUav_Status.UavSenmodStatus.Horizontal.CURRENT_USE == UAV_HORIZONTAL_SENMOD_CURRENT_OPTICFLOW)
	{
//		horizontal_Control_OpticFlow_PosHold(controlDeltaT);
	}	
}

vu16 g_vu16GpsHorizontalPosControlTicks   = 0; /*GPSˮƽλ�ÿ��Ƽ�����*/
vu16 g_vu16GpsHorizontalSpeedControlTicks = 0; /*GPSˮƽ�ٶȿ��Ƽ�����*/

/*ˮƽGPSλ�ÿ�����*/
void horizontal_Control_GPS_PosHold(fp32 controlDeltaT)
{
	/************************** ˮƽλ�ÿ����� ��ʼ ********************************/
	/*ң�ؾ���,ˮƽ������ң�ظ��������Ƕ�/�ٶ�*/
	if ((g_psControlAircraft->RemotExpectAngle.pitch == 0) && \
		(g_psControlAircraft->RemotExpectAngle.roll == 0))
	{
		/*ticks++*/
		g_vu16GpsHorizontalPosControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
		
		/************************** ˮƽλ�ÿ����� ��ʼ ********************************/
		if (g_vu16GpsHorizontalPosControlTicks >= CTRL_GPS_HORIZONTAL_POSITION_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16GpsHorizontalPosControlTicks = 0;
			
			/*λ������,��γ�������ٶȡ��߶�,����˻��к�,����Ϊ0,������������(ֻ����һ��)*/
			if ((g_psPidSystem->LatitudePosition.expect == 0) && \
				(g_psPidSystem->LongitudePosition.expect == 0))
			{
				/*��ɲ��ͣסʱ,�Ÿ���ˮƽλ������*/
				if (horizontal_GPS_Get_Stop_Point_XY(&g_sUavGpsStopPoint) == SYS_RET_SUCC)
				{
					g_psPidSystem->LatitudePosition.expect  = g_sUavGpsStopPoint.north;
					g_psPidSystem->LongitudePosition.expect = g_sUavGpsStopPoint.east;
					
					/*���ˮƽ��������ͣ*/
					g_psUav_Status->AIRSTOP_TYPE |= UAV_AIRSTOP_ONLY_HORIZONTAL;
				}
				else /*ֻ����ˮƽ�ٶ�ɲ��*/
				{
					/*ˮƽɲ���ٶ�����Ϊ0,������ͣס*/
					g_psPidSystem->LatitudeSpeed.expect  = 0;
					g_psPidSystem->LongitudeSpeed.expect = 0;
					
					/*���ˮƽ�������ͣ*/
					g_psUav_Status->AIRSTOP_TYPE &= UAV_AIRSTOP_ONLY_VERTICAL;					
				}	
			}
			else /*ˮƽλ�ÿ�ˮƽ�ٶȻ�ɲ����,����ˮƽλ������,Ȼ�����ˮƽλ�ÿ��ƻ�*/
			{
				/*����ˮƽλ�÷���,��Դ�ڵ�ǰ�ߵ���λ�ù���*/
				g_psPidSystem->LatitudePosition.feedback  = g_psSinsReal->curPosition[EARTH_FRAME_Y];					
				g_psPidSystem->LongitudePosition.feedback = g_psSinsReal->curPosition[EARTH_FRAME_X];
			
				/*��������ϵ��E��N������λ��ƫ��*/
				g_psAttitudeAll->EarthFramePosError.north = g_psPidSystem->LatitudePosition.expect - g_psPidSystem->LatitudePosition.feedback;
				g_psAttitudeAll->EarthFramePosError.east  = g_psPidSystem->LongitudePosition.expect - g_psPidSystem->LongitudePosition.feedback;
			
				/*��������ϵ ��ת�� ��������ϵ λ��ƫ��*/
				g_psAttitudeAll->BodyFramePosError.pitch = -g_psAttitudeAll->EarthFramePosError.east * SIN_YAW + \
															g_psAttitudeAll->EarthFramePosError.north * COS_YAW;
	
				g_psAttitudeAll->BodyFramePosError.roll  =  g_psAttitudeAll->EarthFramePosError.east * COS_YAW + \
															g_psAttitudeAll->EarthFramePosError.north * SIN_YAW;

				/*��������ϵ�·���������ɲ���ٶ�,����Ϊ���������㲻����PID���㺯��*/
				g_psAttitudeAll->BodyFramePosError.pitch = math_Constrain( g_psAttitudeAll->BodyFramePosError.pitch, \
																		   g_psPidSystem->LatitudePosition.errorMax, \
																		  -g_psPidSystem->LatitudePosition.errorMax); /*λ��ƫ���޷�,��λcm*/

				g_psAttitudeAll->BodyFramePosError.roll  = math_Constrain( g_psAttitudeAll->BodyFramePosError.roll, \
																		   g_psPidSystem->LongitudePosition.errorMax, \
																		  -g_psPidSystem->LongitudePosition.errorMax); /*λ��ƫ���޷�,��λcm*/
			
				/*����ˮƽ����ɲ���ٶ�:��������(ˮƽλ�ÿ������)*/
				g_psAttitudeAll->BodyFrameBrakeSpeed.pitch = g_psPidSystem->LatitudePosition.PID.kP * g_psAttitudeAll->BodyFramePosError.pitch;  /*y*/
				g_psAttitudeAll->BodyFrameBrakeSpeed.roll  = g_psPidSystem->LongitudePosition.PID.kP * g_psAttitudeAll->BodyFramePosError.roll;  /*x*/
			
				/*���¡�����ϵ��ˮƽ����Pitch��Roll�ٶȿ���������*/
				g_psPidSystem->LatitudeSpeed.expect  = g_psAttitudeAll->BodyFrameBrakeSpeed.pitch;  /*��γ-��y�᷽��(N��)*/
				g_psPidSystem->LongitudeSpeed.expect = g_psAttitudeAll->BodyFrameBrakeSpeed.roll;   /*����-��x�᷽��(E��)*/
				
				/******************** ˮƽλ�ÿ���������,��������ɲ���ٶ�(�����ٶ�) ���� *********************/
			}
		}				
	
		/************************** ˮƽ�ٶȿ�����(����) ��ʼ ********************************/				
	    /*����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������*/
		/*������Pitch��Roll����ˮƽ�ٶȿ���*/
		g_vu16GpsHorizontalSpeedControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
		
		if (g_vu16GpsHorizontalSpeedControlTicks >= CTRL_GPS_HORIZONTAL_SPEED_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16GpsHorizontalSpeedControlTicks = 0;
			
			/*����ϵ ת ����ϵ ˮƽ�����ٶȷ�����*/
			g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psSinsReal->curSpeed[EARTH_FRAME_X] * SIN_YAW + \
															 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * COS_YAW;
			
			g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psSinsReal->curSpeed[EARTH_FRAME_X] * COS_YAW + \
															 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * SIN_YAW;					
			
			/*���� ����ϵ ˮƽ�����ٶȷ�����*/
			g_psPidSystem->LatitudeSpeed.feedback  = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*y�᷽��*/
			g_psPidSystem->LongitudeSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*x�᷽��*/
			
			/*����ϵ ˮƽ�����ٶȿ��Ƽ����*/
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
		
		/************************** ˮƽ�ٶȿ�����(����) ���� ********************************/
	}
	/*����ҡ��,ֻ����ˮƽ�ٶȿ���,��ˮƽλ�ÿ���*/
	else if ((g_psControlAircraft->RemotExpectAngle.pitch != 0) || \
			 (g_psControlAircraft->RemotExpectAngle.roll != 0)) 
	{
		/*���������:
		1����������ϵ����̬�ǿ���;
		2����������ϵ�����ϵ��ٶȿ���;*/
		
		/*�ƶ������,��Ӧ�����Ƕ�*/
		if (CTRL_HORIZONTAL_SENSOR_MODE_REMOT_EXPECT_ANGLE == SYS_ENABLE)
		{
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch;
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;			
		}
		/*�ƶ������,��Ӧ������������ϵ����Pitch,Roll�����˶��ٶ�*/
		/************************** ˮƽ�ٶȿ�����(ҡ�˿���) ��ʼ ********************************/			
		else
		{
			/*ticks++*/
			g_vu16GpsHorizontalSpeedControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
			
			if (g_vu16GpsHorizontalSpeedControlTicks >= CTRL_GPS_HORIZONTAL_SPEED_CONTROL_PERIOD) 
			{
				/*ticks��0*/
				g_vu16GpsHorizontalSpeedControlTicks = 0;
				
				/*N��(��PITCH����)����ƶ��ٶ�*/
				g_psPidSystem->LatitudeSpeed.expect  = -(g_psControlAircraft->RemotExpectAutoAngle.pitch / REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX) * \
													     CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�,ǰspeed>0;��speed<0*/

				/*E��(��ROLL����)����ƶ��ٶ�*/
				g_psPidSystem->LongitudeSpeed.expect =  (g_psControlAircraft->RemotExpectAutoAngle.roll / REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX) * \
													     CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�,ǰspeed>0;��speed<0*/						
				
				/*����ϵ��ˮƽ�ٶȣ�ת������������ϵ������*/
				/*�ػ���Pitch��Roll����ˮƽ�ٶȿ���*/
				g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psSinsReal->curSpeed[EARTH_FRAME_X] * SIN_YAW + \
																 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * COS_YAW;

				g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psSinsReal->curSpeed[EARTH_FRAME_X] * COS_YAW + \
																 g_psSinsReal->curSpeed[EARTH_FRAME_Y] * SIN_YAW;
				
				/*����ˮƽ�ٶȷ���*/
				g_psPidSystem->LatitudeSpeed.feedback  = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*y�᷽��*/	
				g_psPidSystem->LongitudeSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*x�᷽��*/
				
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
		
		/************************** ˮƽ�ٶȿ�����(ҡ�˿���) ���� ********************************/
		/*�Ƹ˼�϶,ˮƽλ��������0,ң��ֱֵ�Ӹ�ˮƽ �ٶ�/�Ƕ� ����*/
		g_psPidSystem->LatitudePosition.expect  = 0;
		g_psPidSystem->LongitudePosition.expect = 0;				
	}
}

vu16 g_vu16OpflowHorizontalPosControlTicks   = 0; /*OpticFlowˮƽλ�ÿ��Ƽ�����*/
vu16 g_vu16OpflowHorizontalSpeedControlTicks = 0; /*OpticFlowˮƽ�ٶȿ��Ƽ�����*/

/*ˮƽ����λ�ÿ�����*/
void horizontal_Control_OpticFlow_PosHold(fp32 controlDeltaT)
{
	/************************** ˮƽλ�ÿ����� ��ʼ ********************************/	
	/*ң�ؾ���,ˮƽ������ң�ظ��������Ƕ�*/
	if ((g_psControlAircraft->RemotExpectAngle.pitch == 0) && \
		(g_psControlAircraft->RemotExpectAngle.roll == 0))
	{
		/*ticks++*/
		g_vu16OpflowHorizontalPosControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
		
		/************************** ˮƽλ�ÿ����� ��ʼ ********************************/
		if (g_vu16OpflowHorizontalPosControlTicks >= CTRL_OPFLOW_HORIZONTAL_POSITION_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16OpflowHorizontalPosControlTicks = 0;
			
			/*λ������,x,y,����˻��к�,����Ϊ0,������������(ֻ����һ��)*/
			if ((g_psPidSystem->OpticFlowXPosition.expect == 0) && \
				(g_psPidSystem->OpticFlowYPosition.expect == 0))
			{
				/*��ɲ��ͣסʱ,�Ÿ���ˮƽλ������*/
				if (horizontal_OpticFlow_Get_Stop_Point_XY(&g_sUavOpticFlowStopPoint) == SYS_RET_SUCC)
				{
					g_psPidSystem->OpticFlowXPosition.expect = g_sUavOpticFlowStopPoint.y;
					g_psPidSystem->OpticFlowYPosition.expect = g_sUavOpticFlowStopPoint.x;
				}
				else /*ֻ����ˮƽ�ٶ�ɲ��*/
				{
					/*ˮƽɲ���ٶ�����Ϊ0,������ͣס*/
					g_psPidSystem->OpticFlowXSpeed.expect = 0;
					g_psPidSystem->OpticFlowYSpeed.expect = 0;						
				}	
			}
			else /*ˮƽλ�ÿ�ˮƽ�ٶȻ�ɲ����,����ˮƽλ������,Ȼ�����ˮƽλ�ÿ��ƻ�*/
			{
				/*����ˮƽλ�÷���,��Դ�ڵ�ǰ�ߵ���λ�ù���*/
				g_psPidSystem->OpticFlowXPosition.feedback = g_psAttitudeAll->OpticFlowData.RealPosition.x;	
				g_psPidSystem->OpticFlowYPosition.feedback = g_psAttitudeAll->OpticFlowData.RealPosition.y;	
			
				/*��������ϵ��E��N������λ��ƫ��*/
				g_psAttitudeAll->EarthFramePosError.north = g_psPidSystem->OpticFlowYPosition.expect - g_psPidSystem->OpticFlowYPosition.feedback;
				g_psAttitudeAll->EarthFramePosError.east  = g_psPidSystem->OpticFlowXPosition.expect - g_psPidSystem->OpticFlowXPosition.feedback;
			
				/*��������ϵ�»���Pitch(��pitch��ת��������,��y�᷽��(N��))��Roll(��Roll��ת��������,��x�᷽��(E��))������λ��ƫ��*/
				g_psAttitudeAll->BodyFramePosError.pitch = -g_psAttitudeAll->EarthFramePosError.east * SIN_YAW + \
															g_psAttitudeAll->EarthFramePosError.north * COS_YAW;
	
				g_psAttitudeAll->BodyFramePosError.roll  =  g_psAttitudeAll->EarthFramePosError.east * COS_YAW + \
															g_psAttitudeAll->EarthFramePosError.north * SIN_YAW;		

				/*��������ϵ�»���Pitch(��pitch��ת��������,��y�᷽��(N��))��Roll(��Roll��ת��������,
				  ��x�᷽��(E��))����������ɲ���ٶ�,����Ϊ���������㲻����PID���㺯��*/
				g_psAttitudeAll->BodyFramePosError.pitch = math_Constrain(g_psAttitudeAll->BodyFramePosError.pitch, \
																		  g_psPidSystem->OpticFlowYPosition.errorMax, \
																		  -g_psPidSystem->OpticFlowYPosition.errorMax); /*(��pitch��ת��������,��y�᷽��)����,λ��ƫ���޷�,��λcm*/

				g_psAttitudeAll->BodyFramePosError.roll  = math_Constrain(g_psAttitudeAll->BodyFramePosError.roll, \
																		  g_psPidSystem->OpticFlowXPosition.errorMax, \
																		  -g_psPidSystem->OpticFlowXPosition.errorMax); /*(��Roll��ת��������,��x�᷽��)����,λ��ƫ���޷�,��λcm*/
			
				/*����ˮƽ����Pitch(��pitch��ת��������,��y�᷽��(N��))��Roll(��Roll��ת��������,��x�᷽��(E��))ɲ���ٶ�:��������(ˮƽλ�ÿ������)*/
				g_psAttitudeAll->BodyFrameBrakeSpeed.pitch = g_psPidSystem->OpticFlowXPosition.PID.kP * g_psAttitudeAll->BodyFramePosError.pitch;
				g_psAttitudeAll->BodyFrameBrakeSpeed.roll  = g_psPidSystem->OpticFlowYPosition.PID.kP * g_psAttitudeAll->BodyFramePosError.roll;  
			
				/*����ˮƽ����Pitch(��pitch��ת��������,��y�᷽��(N��))��Roll(��Roll��ת��������,��x�᷽��(E��))�ٶȿ���������*/
				g_psPidSystem->OpticFlowYSpeed.expect = g_psAttitudeAll->BodyFrameBrakeSpeed.pitch;  /*��γ-��y�᷽��(N��)(��pitch��ת��������)*/					
				g_psPidSystem->OpticFlowXSpeed.expect = g_psAttitudeAll->BodyFrameBrakeSpeed.roll;   /*����-��x�᷽��(E��)(��roll��ת��������)*/			
				
				/******************** ˮƽλ�ÿ���������,��������ɲ���ٶ�(�����ٶ�) ���� *********************/
			}
		}				
	
		/************************** ˮƽ�ٶȿ����� ��ʼ ********************************/				
	    /*����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������*/
		/*������Pitch��Roll����ˮƽ�ٶȿ���*/
		g_vu16OpflowHorizontalSpeedControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
		
		if (g_vu16OpflowHorizontalSpeedControlTicks >= CTRL_OPFLOW_HORIZONTAL_SPEED_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16OpflowHorizontalSpeedControlTicks = 0;
			
			/*����ϵˮƽ�����ٶȷ�����*/
			g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psAttitudeAll->OpticFlowData.RealSpeed.y * SIN_YAW + \
															 g_psAttitudeAll->OpticFlowData.RealSpeed.x * COS_YAW;
			
			g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psAttitudeAll->OpticFlowData.RealSpeed.y * COS_YAW + \
															 g_psAttitudeAll->OpticFlowData.RealSpeed.x * SIN_YAW;					
			
			/*����������ˮƽ�����ٶȷ�����*/
			g_psPidSystem->OpticFlowXSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*��γ-��y�᷽��(N��)(��pitch��ת��������)*/		
			g_psPidSystem->OpticFlowYSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*����-��x�᷽��(E��)(��roll��ת��������)*/
			
			/*������ˮƽ�����ٶȿ��Ƽ����*/
			g_psPidSystem->OpticFlowXSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->OpticFlowXSpeed, PID_CONTROLER_LATITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/		
			g_psPidSystem->OpticFlowYSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->OpticFlowYSpeed, PID_CONTROLER_LONGITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/	
			
			/*ˮƽ�����������ٶȼ�ˮƽ�ٶȿ��ƻ������*/
			g_sHorizontalExpectAcc.y = -g_psPidSystem->OpticFlowYSpeed.controlOutput;  /*��ͷpitchǰ����*/
			g_sHorizontalExpectAcc.x =  g_psPidSystem->OpticFlowXSpeed.controlOutput;  /*���ͷpitch��ֱroll���ҷ���*/
				
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
		/*���������:
		1����������ϵ�ĺ����ٶȿ���;
		2����������ϵ�����ϵ��ٶȿ���;*/
		
		/*��˺�,����ˮƽλ����0*/
		g_psAttitudeAll->OpticFlowData.RealPosition.x = 0;
		g_psAttitudeAll->OpticFlowData.RealPosition.y = 0;
		
		/*�ƶ������,��Ӧ�����Ƕ�*/
		if (CTRL_HORIZONTAL_SENSOR_MODE_REMOT_EXPECT_ANGLE == SYS_ENABLE)
		{
			g_psPidSystem->PitchAngle.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch;
			g_psPidSystem->RollAngle.expect  = g_psControlAircraft->RemotExpectAutoAngle.roll;					
		}
		/*�ƶ������,��Ӧ������������ϵ����Pitch,Roll�����˶��ٶ�*/
		/************************** ˮƽ�ٶȿ����� ��ʼ ********************************/			
		else
		{
			/*ticks++*/
			g_vu16OpflowHorizontalSpeedControlTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
			
			if (g_vu16OpflowHorizontalSpeedControlTicks >= CTRL_OPFLOW_HORIZONTAL_SPEED_CONTROL_PERIOD)
			{
				/*ticks��0*/
				g_vu16OpflowHorizontalSpeedControlTicks = 0;
				
				/*N��(��PITCH����)����ƶ��ٶ�*/
				g_psPidSystem->OpticFlowYSpeed.expect = g_psControlAircraft->RemotExpectAutoAngle.pitch * \
														CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�45*6/100=3.6m/s*/

				/*E��(��ROLL����)����ƶ��ٶ�*/
				g_psPidSystem->OpticFlowXSpeed.expect = g_psControlAircraft->RemotExpectAutoAngle.roll * \
														CTRL_HORIZONTAL_MAX_MOVE_SPEED; /*��������ٶ�45*6/100=3.6m/s*/						
				
				/*����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������*/
				/*������Pitch��Roll����ˮƽ�ٶȿ���*/
				g_psAttitudeAll->BodyFrameSpeedFeedback.pitch = -g_psAttitudeAll->OpticFlowData.RealSpeed.y * SIN_YAW + \
																 g_psAttitudeAll->OpticFlowData.RealSpeed.x * COS_YAW;

				g_psAttitudeAll->BodyFrameSpeedFeedback.roll  =  g_psAttitudeAll->OpticFlowData.RealSpeed.y * COS_YAW + \
																 g_psAttitudeAll->OpticFlowData.RealSpeed.x * SIN_YAW;
				
				/*����ˮƽ�ٶȷ���*/
				g_psPidSystem->OpticFlowXSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.pitch; /*��γ-��y�᷽��(N��)(��pitch��ת��������)*/	
				g_psPidSystem->OpticFlowYSpeed.feedback = g_psAttitudeAll->BodyFrameSpeedFeedback.roll;  /*����-��x�᷽��(E��)(��roll��ת��������)*/
				
				/*ˮƽ�ٶ�PID���㼰��� (ˮƽ����)*/
				g_psPidSystem->OpticFlowXSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->OpticFlowXSpeed, PID_CONTROLER_LATITUDE_SPEED);    /*PID DIV���Ƶ�ͨ�˲�*/
				g_psPidSystem->OpticFlowYSpeed.controlOutput = pid_Control_Div_LPF(&g_psPidSystem->OpticFlowYSpeed, PID_CONTROLER_LONGITUDE_SPEED);  /*PID DIV���Ƶ�ͨ�˲�*/	
				
				/*ˮƽ�����������ٶȼ�ˮƽ�ٶȿ��ƻ������*/
				g_sHorizontalExpectAcc.y = -g_psPidSystem->OpticFlowYSpeed.controlOutput;  /*��ͷpitchǰ����*/
				g_sHorizontalExpectAcc.x =  g_psPidSystem->OpticFlowXSpeed.controlOutput; /*���ͷpitch��ֱroll���ҷ���*/
				
				/*ˮƽ�������ٶ��봹ֱ���������ٶȼ�����������*/
				horizontal_Acc_Convert_To_Dip_Angle(g_sHorizontalExpectAcc, &g_sHorizontalExpectAngle);
				
				/*���½ǶȻ�������(��Դ����ˮƽ���ٶȺ��������ٶȻ�������������)*/
				g_psPidSystem->PitchAngle.expect = g_sHorizontalExpectAngle.y;	/*��ͷpitchǰ����*/
				g_psPidSystem->RollAngle.expect  = g_sHorizontalExpectAngle.x;	/*���ͷpitch��ֱroll���ҷ���*/				
			}
		}
		
		/************************** ˮƽ�ٶȿ����� ���� ********************************/
		
		/*�Ƹ˼�϶,ˮƽλ��������0,ң��ֱֵ�Ӹ�ˮƽ�ٶ�����*/
		g_psPidSystem->OpticFlowXPosition.expect = 0;
		g_psPidSystem->OpticFlowYPosition.expect = 0;				
	}
}
