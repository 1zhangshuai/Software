#include "control_AltHold.h"
#include "control_Aircraft.h"

/*��ȡ���������λ�����ƶ����*/
CONTROL_THROTTLE_PUSH altitude_Get_Throttle_Relative_MidDead(ControlThrottlePush *controlThrottlePush)
{
	/*��¼�ϴ�����״̬*/
	controlThrottlePush->LAST_STATUS = controlThrottlePush->CUR_STATUS;
	
	/*��������״̬�ж�*/
	if (g_psControlAircraft->throttleUpDownJudgeValue <= REMOT_THROTTLE_DOWN_CROSSOVER)	/*����ֵ������֮��*/
	{
		controlThrottlePush->CUR_STATUS = CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD;
	}
	else if (g_psControlAircraft->throttleUpDownJudgeValue >= REMOT_THROTTLE_UP_CROSSOVER) /*����ֵ������֮��*/
	{
		controlThrottlePush->CUR_STATUS = CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD;		
	}
	else	/*����ֵ������֮��*/
	{
		controlThrottlePush->CUR_STATUS = CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD;		
	}
	
	/*�����ϴ�����״̬�ж�*/	
	if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD) && \
		(controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD)) 	 	/*���Ϲ���*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_UP_OVER_MIDDEAD;
	}
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD)) /*���¹���*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_DOWM_OVER_MIDDEAD;	
	}
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD)) /*���ϵ���*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_UP_TO_MIDDEAD;	
	}
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD)) /*���µ���*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_DOWM_TO_MIDDEAD;	
	}
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_UP_MIDDEAD)) /*��λ����λ��*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_UPDOWN_UPBAND;	
	}
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_DOWN_MIDDEAD)) /*��λ����λ��*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_UPDOWN_DOWMBAND;	
	}	
	else if ((controlThrottlePush->LAST_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD) && \
			 (controlThrottlePush->CUR_STATUS == CONTROL_THROTTLE_PUSH_LOCATE_MIDDEAD)) /*��λ����λ��*/
	{
		controlThrottlePush->REAL_STATUS = CONTROL_THROTTLE_PUSH_UPDOWN_DEADBAND;	
	}	
	
	return (controlThrottlePush->REAL_STATUS);
}

vu16 g_vu16HighPosControlTicks         = 0; /*�߶�λ�ÿ��Ƽ�����*/
vu16 g_vu16HighSpeedControlTicks       = 0; /*�߶��ٶȿ��Ƽ�����*/

fp32 g_HeightYawSpeedFeedforwardOutput = 0.0f;   /*��ֱ�ٶ�ǰ�����������*/
fp32 g_HeightYawSpeedFeedforwardRate   = 1.0f;   /*��ֱ�ٶ�ǰ��������,APM����Ϊ1��0.45*/
fp32 g_HeightYawSpeedFeedforwardDelta  = 0.0f;   /*��ֱ�����ٶȱ仯��*/
fp32 g_CurHeightYawSpeedExpect         = 0.0f;   /*������ֱ�ٶ�����*/
fp32 g_LastHeightYawSpeedExpect        = 0.0f;	 /*�ϴ���ֱ�ٶ�����*/

/*��ֱ�߶�(����)������*/
void vertical_Control_AltHold(fp32 controlDeltaT)
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
	/************************** ��ֱλ�ÿ����� ��ʼ ********************************/
	/*����ҡ��λ���м�λ��������(�Ȳ�������,Ҳ��������)*/
	if ((g_psControlAircraft->throttleUpDownJudgeValue >= REMOT_THROTTLE_DOWN_CROSSOVER) && \
		(g_psControlAircraft->throttleUpDownJudgeValue <= REMOT_THROTTLE_UP_CROSSOVER)) /*400 <= && 600 <=*/
	{
		/*�߶�λ�û���������ٶ�����*/
		if (g_psPidSystem->HighPosition.expect == 0) /*������/����ʱ,��ֱλ������Ϊ0,���Ż��к�,ֻ����һ����ֱλ������*/
		{
			g_psPidSystem->HighPosition.expect = g_psSinsReal->curPosition[EARTH_FRAME_Z]; /*���Ż��к�,����λ��(�߶�)����*/
		}
		
		/*ticks++*/
		g_vu16HighPosControlTicks++;	/*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
		
		/*��ֱ�߶ȿ�������2*5=10ms*/
		if (g_vu16HighPosControlTicks >= CTRL_VERTICAL_POSITION_CONTROL_PERIOD)
		{
			/*ticks��0*/
			g_vu16HighPosControlTicks = 0;
			
			/*���¸߶�λ�÷���*/
			g_psPidSystem->HighPosition.feedback = g_psSinsReal->curPosition[EARTH_FRAME_Z];
			
			/*�⻷ ��ֱ�߶�λ��PID���㼰���*/
			g_psPidSystem->HighPosition.controlOutput = pid_Control_General_Dp(&g_psPidSystem->HighPosition); 
	
			/*�Ƿ������ٶ�ǰ��������*/
			#ifdef CTRL_HEIGHT_YAW_SPEED_FEEDFORWARD 
			g_CurHeightYawSpeedExpect         = g_psPidSystem->HighPosition.controlOutput; /*������ֱ�ٶ�����*/
			g_HeightYawSpeedFeedforwardDelta  = (g_CurHeightYawSpeedExpect - g_LastHeightYawSpeedExpect) / (2 * controlDeltaT); /*�ٶ������仯��*/
			g_HeightYawSpeedFeedforwardOutput = g_HeightYawSpeedFeedforwardRate * g_HeightYawSpeedFeedforwardDelta; /*��ֱ�ٶ�ǰ�����������*/
			g_LastHeightYawSpeedExpect        = g_CurHeightYawSpeedExpect; /*��ֱ�ٶ������Ƶ�*/
			#endif
			
			/*���� �ڻ� ��ֱ�ٶ�����*/
			g_psPidSystem->HighSpeed.expect = g_psPidSystem->HighPosition.controlOutput; /*�����⻷�������*/
		}
		
		/*��������ֱ�����˶����Ʊ�������*/
		g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND = CTRL_AIRCRAFT_MOVE_VER_HOLD;
	}
	/*����ҡ��λ���м�λ������֮��(����),���������ٶ�����*/
	else if (g_psControlAircraft->throttleUpDownJudgeValue > REMOT_THROTTLE_UP_CROSSOVER) /*> 600*/
	{
		/*���Ÿ�����,ֱ�Ӹ�����ֱ�ٶ�����*/
		g_psPidSystem->HighSpeed.expect = CTRL_HEIGHT_CLIMB_UP_MAX_SPEED * \
										  (g_psControlAircraft->throttleUpDownJudgeValue - REMOT_THROTTLE_UP_CROSSOVER) / \
										  (CTRL_THROTTLE_TOP_VALUE - REMOT_THROTTLE_UP_CROSSOVER);
		
		g_psPidSystem->HighPosition.expect = 0; /*��ֱλ�û�������0*/
		
		/*��������ֱ�����˶���������*/
		g_psControlAircraft->AIRCRAFT_VER_MOVE_TREND = CTRL_AIRCRAFT_MOVE_VER_UP;
	}
	/*����ҡ��λ���м�λ������֮��(����),�����½��ٶ�����*/
	else if (g_psControlAircraft->throttleUpDownJudgeValue < REMOT_THROTTLE_DOWN_CROSSOVER) /*< 400*/
	{
		/*���Ÿ�����,ֱ�Ӹ�����ֱ�ٶ�����*/
		g_psPidSystem->HighSpeed.expect = CTRL_HEIGHT_CLIMB_DOWN_MAX_SPEED * \
										  (g_psControlAircraft->throttleUpDownJudgeValue - REMOT_THROTTLE_DOWN_CROSSOVER) / \
										  (REMOT_THROTTLE_DOWN_CROSSOVER - CTRL_THROTTLE_BOTTOM_VALUE);
		
		g_psPidSystem->HighPosition.expect = 0; /*��ֱλ�û�������0*/
		
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
	g_vu16HighSpeedControlTicks++;
	
	/*��ֱ�߶ȿ�������1*5=5ms*/
	if (g_vu16HighSpeedControlTicks >= CTRL_VERTICAL_SPEED_CONTROL_PERIOD) /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
	{
		/*ticks��0*/
		g_vu16HighSpeedControlTicks = 0;
		
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
	/**********
	 **********
              ***
             ***
              ***
               ***
        ***********
	 ***********/
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
