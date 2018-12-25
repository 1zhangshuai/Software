#include "remot_DataAnaly.h"
#include "ahrs_Caculation.h"
#include "attitude_Aircraft.h"
#include "sys_McuInit.h"

#include "control_Config.h" /*ң���г���������*/
#include "control_Aircraft.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

/*ң�ؽ��ջ�PWM����*/
ReceiverAnaly g_sReceiverAnaly = 
{
	#if defined(REMOTE_DATA_RECV__PPM) /*PPM*/
	.PPM_Buff           = {0},
	.PPM_RECV_STATUS    = REMOT_PPM_RECV_FINISH,
	.PPM_SAMPLE_CHANNLE = REMOT_PPM_SAMPLE_1ST_CNL,
	#endif
};
ReceiverAnaly *g_psReceiverAnaly = &g_sReceiverAnaly;

/*ң������*/
RemotData g_sRemotData   = {0};
RemotData *g_psRemotData = &g_sRemotData;

/*=== ң�����ݽ��������������� ===*/
/*��ȡң�ظ�ͨ������*/
void remot_get_all_channel_data(RemotData *remotData, ReceiverAnaly *receiverAnaly)
{	
	/*�ж���̬ң�������Ƿ��������,�������������������ң��*/
	/*iA10B �ο���������ʧ��ʱ����ͨ�����ֵ���ڷ�Χ��,(1000/109%)���� */
	/*iA6 ʵ��ر�ң�غ�,���ջ��������*/	
	if ((receiverAnaly->Attitude.Throttle.UPDATE_STATUS == WAVE_STATUS_NEW) && \
		(receiverAnaly->Attitude.Throttle.avaTicks >= 980)) /*�˴�����980,ʵ��955(����0���������),��������ʱ,ֵ�������1000*/
	{
		/*�ڵ�ǰʱ��ι��100ms*/
		security_Feed_CMC_Succ_Dog_10MS(10, &g_sUavRemotCMCDog);
	}
	
	/*ң�����ݸ�ֵ*/
/*=== Attitude: CH1 - CH4 ===*/	
	/*Attitude CH1 Roll Data*/
	if (receiverAnaly->Attitude.Roll.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Attitude.Roll.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/
		
		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Attitude.Roll.avaTicks) && \
			(receiverAnaly->Attitude.Roll.avaTicks <= 2000))
		{
			remotData->AttRoll = receiverAnaly->Attitude.Roll.avaTicks;
		}
		else
		{
			remotData->AttRoll = 0;			
		}
		
		/*��ң������ת����ң��ROLL������*/
		if (remotData->AttRoll <= REMOT_ANGLE_DEADZONE_BUTTOM)
		{
			g_psControlAircraft->RemotExpectAngle.roll = (remotData->AttRoll - REMOT_ANGLE_DEADZONE_BUTTOM) * \
														  REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 
		}
		else if (remotData->AttRoll >= REMOT_ANGLE_DEADZONE_TOP)
		{
			g_psControlAircraft->RemotExpectAngle.roll = (remotData->AttRoll - REMOT_ANGLE_DEADZONE_TOP) * \
														  REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 			
		}
		else
		{
			g_psControlAircraft->RemotExpectAngle.roll = 0;
		}
		
		/*ң��ROLL�����Ƿ�Χ����*/
		if (g_psControlAircraft->RemotExpectAngle.roll >= REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.roll = REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX;
		}
		else if (g_psControlAircraft->RemotExpectAngle.roll <= -REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.roll = -REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX;		
		}		
		
		/*ң���������Ⱥ����*/
		g_psControlAircraft->RemotExpectAutoAngle.roll = g_psControlAircraft->RemotExpectAngle.roll;
	}
	
	/*Attitude CH2 Pitch Data*/	
	if (receiverAnaly->Attitude.Pitch.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Attitude.Pitch.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/
		
		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Attitude.Pitch.avaTicks) && \
			(receiverAnaly->Attitude.Pitch.avaTicks <= 2000))
		{
			remotData->AttPitch = receiverAnaly->Attitude.Pitch.avaTicks;
		}
		else
		{
			remotData->AttPitch = 0;			
		}		

		/*��ң������ת����ң��PITCH������*/
		if (remotData->AttPitch <= REMOT_ANGLE_DEADZONE_BUTTOM)
		{
			g_psControlAircraft->RemotExpectAngle.pitch = (REMOT_ANGLE_DEADZONE_BUTTOM - remotData->AttPitch) * \
														   REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 
		}
		else if (remotData->AttPitch >= REMOT_ANGLE_DEADZONE_TOP)
		{
			g_psControlAircraft->RemotExpectAngle.pitch = (REMOT_ANGLE_DEADZONE_TOP - remotData->AttPitch) * \
														   REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 			
		}
		else
		{
			g_psControlAircraft->RemotExpectAngle.pitch = 0;
		}
		
		/*ң��PITCH�����Ƿ�Χ����*/
		if (g_psControlAircraft->RemotExpectAngle.pitch >= REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.pitch = REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX;
		}
		else if (g_psControlAircraft->RemotExpectAngle.pitch <= -REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.pitch = -REMOT_PITCH_ROLL_ANGLE_EXPECT_MAX;		
		}		

		/*ң������ң���������ȸ�����*/
		g_psControlAircraft->RemotExpectAutoAngle.pitch = g_psControlAircraft->RemotExpectAngle.pitch;		
	}

	/*Attitude CH3 Throttle Data*/	
	if (receiverAnaly->Attitude.Throttle.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Attitude.Throttle.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/
		
		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Attitude.Throttle.avaTicks) && \
			(receiverAnaly->Attitude.Throttle.avaTicks <= 2000))
		{
			remotData->AttThrottle = receiverAnaly->Attitude.Throttle.avaTicks;
		}
		else
		{
			remotData->AttThrottle = 0;			
		}		
		
		/*Ϊ�˰�ȫ,���Ÿ˵�λ����Ϊ50*/
		s16 tempRc = remotData->AttThrottle - 1050;
		
		/*0 ~ 1000*/
		if (tempRc <= 0) /*1105*/
		{
 			g_psControlAircraft->RemotExpectAngle.throttle = 0;
		}
		else if (tempRc >= 1000) /*2100*/
		{
			g_psControlAircraft->RemotExpectAngle.throttle = 1000;		
		}
		else
		{
			g_psControlAircraft->RemotExpectAngle.throttle = tempRc;		
		}	
		
		/*���Ÿ����ƻ��������ж���(ң������ԭʼ�г���)*/
		g_psControlAircraft->throttleUpDownJudgeValue = g_psControlAircraft->RemotExpectAngle.throttle;
		
		/*����ֵ+���Ż���ֵ*/
		g_psControlAircraft->RemotExpectAngle.throttle += REMOT_THROTTLE_BASE_VALUE;
	}

	/*Attitude CH4 Yaw Data*/		
	if (receiverAnaly->Attitude.Yaw.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Attitude.Yaw.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/

		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Attitude.Yaw.avaTicks) && \
			(receiverAnaly->Attitude.Yaw.avaTicks <= 2000))
		{
			remotData->AttYaw = receiverAnaly->Attitude.Yaw.avaTicks;
		}
		else
		{
			remotData->AttYaw = 0;			
		}		
		
		/*��ң������ת����ң��YAW������*/
		if (remotData->AttYaw <= REMOT_ANGLE_DEADZONE_BUTTOM)
		{
			g_psControlAircraft->RemotExpectAngle.yaw = (REMOT_ANGLE_DEADZONE_BUTTOM - remotData->AttYaw) * \
														 REMOT_YAW_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 
		}
		else if (remotData->AttYaw >= REMOT_ANGLE_DEADZONE_TOP)
		{
			g_psControlAircraft->RemotExpectAngle.yaw = (REMOT_ANGLE_DEADZONE_TOP - remotData->AttYaw) * \
														 REMOT_YAW_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 			
		}
		else
		{
			g_psControlAircraft->RemotExpectAngle.yaw = 0;
		}

		/*ң��YAW�����Ƿ�Χ����*/
		if (g_psControlAircraft->RemotExpectAngle.yaw >= REMOT_YAW_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.yaw = REMOT_YAW_ANGLE_EXPECT_MAX;
		}
		else if (g_psControlAircraft->RemotExpectAngle.yaw <= -REMOT_YAW_ANGLE_EXPECT_MAX)
		{
			g_psControlAircraft->RemotExpectAngle.yaw = -REMOT_YAW_ANGLE_EXPECT_MAX;		
		}
	}		
	
/*=== Switch: CH5 - CH8 ===*/
	/*Switch CH1 SWA Data*/
	if (receiverAnaly->Switch.SWA.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Switch.SWA.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/
	
		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Switch.SWA.avaTicks) && \
			(receiverAnaly->Switch.SWA.avaTicks <= 2000))
		{
			remotData->SWA = receiverAnaly->Switch.SWA.avaTicks;
		}
		else
		{
			remotData->SWA = 0;
		}		
	}
	
	/*Switch CH2 SWB Data*/
	if (receiverAnaly->Switch.SWB.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Switch.SWB.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/

		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Switch.SWB.avaTicks) && \
			(receiverAnaly->Switch.SWB.avaTicks <= 2000))
		{
			remotData->SWB = receiverAnaly->Switch.SWB.avaTicks;
		}
		else
		{
			remotData->SWB = 0;
		}		
	}

	/*Switch CH3 SWC Data*/
	if (receiverAnaly->Switch.SWC.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Switch.SWC.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/

		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Switch.SWC.avaTicks) && \
			(receiverAnaly->Switch.SWC.avaTicks <= 2000))
		{
			remotData->SWC = receiverAnaly->Switch.SWC.avaTicks;
		}
		else
		{
			remotData->SWC = 0;
		}		
	}

	/*Switch CH4 SWD Data*/		
	if (receiverAnaly->Switch.SWD.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Switch.SWD.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/
		
		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Switch.SWD.avaTicks) && \
			(receiverAnaly->Switch.SWD.avaTicks <= 2000))
		{
			remotData->SWD = receiverAnaly->Switch.SWD.avaTicks;
		}
		else
		{
			remotData->SWD = 0;
		}	
	}	
	
/*=== Switch: CH9 - CH10 ===*/
	/*Switch CH1 Pitch Data*/	
	if (receiverAnaly->Gimbal.VRA.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Gimbal.VRA.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/

		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Gimbal.VRA.avaTicks) && \
			(receiverAnaly->Gimbal.VRA.avaTicks <= 2000))
		{
			remotData->GimPitch = receiverAnaly->Gimbal.VRA.avaTicks;
		}
		else
		{
			remotData->GimPitch = 0;			
		}		
		
		/*��ң������ת���������Pitch������*/
		if (remotData->GimPitch <= REMOT_ANGLE_DEADZONE_BUTTOM)
		{
			g_psControlAircraft->GimbalExpectAngle.pitch = (REMOT_ANGLE_DEADZONE_BUTTOM - remotData->GimPitch) * \
															GIMBAL_PITCH_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 
		}
		else if (remotData->GimPitch >= REMOT_ANGLE_DEADZONE_TOP)
		{
			g_psControlAircraft->GimbalExpectAngle.pitch = (REMOT_ANGLE_DEADZONE_TOP - remotData->GimPitch) * \
															GIMBAL_PITCH_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 			
		}
		else
		{
			g_psControlAircraft->GimbalExpectAngle.pitch = 0;
		}		
	}

	/*Switch CH2 Yaw Data*/		
	if (receiverAnaly->Gimbal.VRB.UPDATE_STATUS == WAVE_STATUS_NEW)	/*�ж��Ƿ�Ϊ������*/
	{
		receiverAnaly->Gimbal.VRB.UPDATE_STATUS = WAVE_STATUS_OLD; /*�ù������ݱ��Ϊ������*/

		/*��Ч��ֵ*/
		if ((1000 <= receiverAnaly->Gimbal.VRB.avaTicks) && \
			(receiverAnaly->Gimbal.VRB.avaTicks <= 2000))
		{
			remotData->GimYaw = receiverAnaly->Gimbal.VRB.avaTicks;
		}
		else
		{
			remotData->GimYaw = 0;			
		}		

		/*��ң������ת���������Yaw������*/
		if (remotData->GimYaw <= REMOT_ANGLE_DEADZONE_BUTTOM)
		{
			g_psControlAircraft->GimbalExpectAngle.yaw = (REMOT_ANGLE_DEADZONE_BUTTOM - remotData->GimYaw) * \
														  GIMBAL_YAW_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 
		}
		else if (remotData->GimPitch >= REMOT_ANGLE_DEADZONE_TOP)
		{
			g_psControlAircraft->GimbalExpectAngle.yaw = (REMOT_ANGLE_DEADZONE_TOP - remotData->GimYaw) * \
														  GIMBAL_YAW_ANGLE_EXPECT_MAX / REMOT_ANGLE_DEADZONE_EXCEPT; 			
		}
		else
		{
			g_psControlAircraft->GimbalExpectAngle.yaw = 0;
		}			
	}	
}

vu16 g_sUnlockContinueTicks   = 0;	/*������������ʱ��*/
vu16 g_sLockContinueTicks     = 0;	/*������������ʱ��*/
vu16 g_sAutoLockContinueTicks = 0;	/*�Զ���������ʱ��*/

/*�ɿ�:����/����/�Զ�����*/
AIRCRAFT_LOCK_STATUS remot_aircraft_lock_and_unlock(void)
{		
	/*1.ҡ���ڰ˽���*/
	/*
		*******       *******
		*     *       *     *
		*  *  *       *  *  * 
		*   * *       * *   *
		*******       *******
			  *       *   
	*/
	if ((remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MIN)     == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MIN)    == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttThrottle, REMOT_DATA_MIN) == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MAX)      == REMOT_DATA_MAX))
	{
		/*��������Ŀ��Ticks�ۼ�*/
		g_sUnlockContinueTicks++;
		
		/*��Ŀ��Ticks��0*/
		g_sLockContinueTicks     = 0;
		g_sAutoLockContinueTicks = 0;
		
		/*��������Ticks,����*/
		if (g_sUnlockContinueTicks >= (AIRCRAFT_UNLOCK_CONTINU_TIME_MS / RTOS_WAKE_UP_UAV_CTRL_FOC_MS))
		{
			/*���һ�μ��,Ŀ��Ticks��0*/
			g_sUnlockContinueTicks = 0;			
			
			/*���طɻ�״̬: δ����*/
			g_psAircraftStatus->LOCK_STATUS = AIRCRAFT_UNLOCK;
		}
	}
	
	/*2.ҡ���������*/
	/*
		*******       *******
		*     *       *     *
		*  *  *       *  *  * 
		* *   *       *   * *
		*******       *******
		*                   *   
	*/	
	if ((remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MAX)     == REMOT_DATA_MAX) && \
		(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MIN)    == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttThrottle, REMOT_DATA_MIN) == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MIN)      == REMOT_DATA_MIN))
	{
		/*��������Ŀ��Ticks�ۼ�*/
		g_sLockContinueTicks++;
		
		/*��Ŀ��Ticks��0*/
		g_sUnlockContinueTicks   = 0;
		g_sAutoLockContinueTicks = 0;
		
		/*��������Ticks,����*/
		if (g_sLockContinueTicks >= (AIRCRAFT_LOCK_CONTINU_TIME_MS / RTOS_WAKE_UP_UAV_CTRL_FOC_MS))
		{
			/*���һ�μ��,Ŀ��Ticks��0*/
			g_sLockContinueTicks = 0;
			
			g_psAircraftStatus->LOCK_STATUS = AIRCRAFT_LOCKING;
		}
	}	
	
	/*����״̬&&��½״̬,һ��ʱ�䲻����ң��(��������,����3��ҡ�˶�����λ),���Զ�����*/
	if ((g_psAircraftStatus->LOCK_STATUS == AIRCRAFT_UNLOCK) && \
	    (g_psAircraftStatus->CUR_FLY_STATUS == AIRCRAFT_LANDING) && \
		(remot_Data_Range(g_sRemotData.AttThrottle, REMOT_DATA_MIN)  == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MID)  == REMOT_DATA_MID) && \
		(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MID) == REMOT_DATA_MID) && \
		(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MID) == REMOT_DATA_MID))
	{
		/*��������Ŀ��Ticks�ۼ�*/
		g_sAutoLockContinueTicks++;
		
		/*��Ŀ��Ticks��0*/
		g_sUnlockContinueTicks = 0;
		g_sLockContinueTicks   = 0;
		
		/*������������Ticks,�Զ�����*/
		if (g_sAutoLockContinueTicks >= (AIRCRAFT_AUTOLOCK_CONTINU_TIME_MS / RTOS_WAKE_UP_UAV_CTRL_FOC_MS))
		{
			/*���һ�μ��,Ŀ��Ticks��0*/
			g_sAutoLockContinueTicks = 0;
			
			/*���Ϊ����*/
			g_psAircraftStatus->LOCK_STATUS = AIRCRAFT_LOCKING;
		}
	}
	
	/*����״̬&&��½״̬&&ң�غͷ������Ͽ�ͨ��*/
	if ((g_psAircraftStatus->LOCK_STATUS == AIRCRAFT_UNLOCK) && \
	    (g_psAircraftStatus->CUR_FLY_STATUS == AIRCRAFT_LANDING) && \
	    (g_psAircraftStatus->CMC_STATUS == AIRCRAFT_CMC_FAIL))
	{
		/*���Ϊ����*/
		g_psAircraftStatus->LOCK_STATUS = AIRCRAFT_LOCKING;		
	}
	
	return (g_psAircraftStatus->LOCK_STATUS);
}

/*=== ԭʼ���ݼ���/��С/��ֵ�ж� ===*/
REMOT_DATA_STATUS remot_Data_Range(u16 ChannelData, REMOT_DATA_STATUS testStatus)
{
	REMOT_DATA_STATUS retStatus;
	
	switch(testStatus)
	{
		case REMOT_DATA_ZERO:	/*�ж������ǲ��ǿյ�,����û��ң�ز���*/
		{
			if (ChannelData == 0)
			{
				retStatus = REMOT_DATA_ZERO;	  /*�����ǿյ�,��û��ң�ز���*/
			}
			else
			{
				retStatus = REMOT_DATA_ERROR;   /*�������ֵ����,���ݴ���Ƿ�*/	
			}
		}break;
		
		case REMOT_DATA_MIN:	/*�ж������ǲ�����Сֵ*/
		{
			if ((ChannelData >= (REMOT_DATA_MIN_VALUE - REMOT_DATA_PRECISION)) && \
				(ChannelData <= (REMOT_DATA_MIN_VALUE + REMOT_DATA_PRECISION)))
			{
				retStatus = REMOT_DATA_MIN;	  /*������Сֵ����*/
			}
			else
			{
				retStatus = REMOT_DATA_ERROR;  /*��������Сֵ����,���ݴ���Ƿ�*/	
			}
		}break;

		case REMOT_DATA_MID:	/*�ж������ǲ�����ֵ,ҡ�����в���ʱ,���ܲ������ϸ���м�,������Ҫ�����������*/
		{
			if ((ChannelData >= (REMOT_DATA_MID_VALUE - REMOT_DATA_PRECISION - REMOT_DATA_MID_ERROR)) && \
				(ChannelData <= (REMOT_DATA_MID_VALUE + REMOT_DATA_PRECISION + REMOT_DATA_MID_ERROR)))
			{
				retStatus = REMOT_DATA_MID;	  /*������ֵ����*/
			}
			else
			{
				retStatus = REMOT_DATA_ERROR;  /*��������ֵ����,���ݴ���Ƿ�*/	
			}		
		}break;

		case REMOT_DATA_MAX:	/*�ж������ǲ������ֵ*/
		{
			if ((ChannelData >= (REMOT_DATA_MAX_VALUE - REMOT_DATA_PRECISION)) && \
				(ChannelData <= (REMOT_DATA_MAX_VALUE + REMOT_DATA_PRECISION)))
			{
				retStatus = REMOT_DATA_MAX;	  /*�������ֵ����*/
			}
			else
			{
				retStatus = REMOT_DATA_ERROR;  /*���������ֵ����,���ݴ���Ƿ�*/	
			}		
		}break;	

		default:break;
	}
	
	return retStatus;
}


#if defined(REMOTE_DATA_RECV__PWM)
/*��̬ͨ��(TIM4)����+����*/
void remot_Attitude_Channel_Analy(void)
{	
	/*����CH1���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC1) != RESET)	
	{	
		if(g_psReceiverAnaly->Attitude.Roll.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Attitude.Roll.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Attitude.Roll.fallingEdgeTicks = TIM_GetCapture1(g_sTimPwmIn_Attitude.Tim);
			
			if (g_psReceiverAnaly->Attitude.Roll.fallingEdgeTicks > \
				g_psReceiverAnaly->Attitude.Roll.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵĸߵ�ƽ����*/
				g_psReceiverAnaly->Attitude.Roll.avaTicks = g_psReceiverAnaly->Attitude.Roll.fallingEdgeTicks - \
															g_psReceiverAnaly->Attitude.Roll.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Attitude.Roll.UPDATE_STATUS = WAVE_STATUS_NEW;
			}
			
	   		TIM_OC1PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Attitude.Roll.risingEdgeTicks = TIM_GetCapture1(g_sTimPwmIn_Attitude.Tim);
			
			g_psReceiverAnaly->Attitude.Roll.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC1PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC1);	/*����жϱ�־λ*/	  	    
	}	
	
	/*����CH2���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC2) != RESET)	
	{	
		if(g_psReceiverAnaly->Attitude.Pitch.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Attitude.Pitch.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Attitude.Pitch.fallingEdgeTicks = TIM_GetCapture2(g_sTimPwmIn_Attitude.Tim);
			
			if (g_psReceiverAnaly->Attitude.Pitch.fallingEdgeTicks > \
				g_psReceiverAnaly->Attitude.Pitch.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Attitude.Pitch.avaTicks = g_psReceiverAnaly->Attitude.Pitch.fallingEdgeTicks - \
															 g_psReceiverAnaly->Attitude.Pitch.risingEdgeTicks;

				/*pwm����״̬����*/
				g_psReceiverAnaly->Attitude.Pitch.UPDATE_STATUS = WAVE_STATUS_NEW;				
			}
			
	   		TIM_OC2PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Attitude.Pitch.risingEdgeTicks = TIM_GetCapture2(g_sTimPwmIn_Attitude.Tim);
			
			g_psReceiverAnaly->Attitude.Pitch.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC2PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC2);	/*����жϱ�־λ*/	    
	}	
	
	/*����CH3���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC3) != RESET)	
	{	
		if(g_psReceiverAnaly->Attitude.Throttle.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Attitude.Throttle.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Attitude.Throttle.fallingEdgeTicks = TIM_GetCapture3(g_sTimPwmIn_Attitude.Tim);
			
			if (g_psReceiverAnaly->Attitude.Throttle.fallingEdgeTicks > \
				g_psReceiverAnaly->Attitude.Throttle.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Attitude.Throttle.avaTicks = g_psReceiverAnaly->Attitude.Throttle.fallingEdgeTicks - \
																g_psReceiverAnaly->Attitude.Throttle.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Attitude.Throttle.UPDATE_STATUS = WAVE_STATUS_NEW;	
			}
			
	   		TIM_OC3PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Attitude.Throttle.risingEdgeTicks = TIM_GetCapture3(g_sTimPwmIn_Attitude.Tim);
			
			g_psReceiverAnaly->Attitude.Throttle.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC3PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC3);	/*����жϱ�־λ*/	  	    
	}	
	
	/*����CH4���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC4) != RESET)	
	{	
		if(g_psReceiverAnaly->Attitude.Yaw.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Attitude.Yaw.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Attitude.Yaw.fallingEdgeTicks = TIM_GetCapture4(g_sTimPwmIn_Attitude.Tim);
			
			if (g_psReceiverAnaly->Attitude.Yaw.fallingEdgeTicks > \
				g_psReceiverAnaly->Attitude.Yaw.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Attitude.Yaw.avaTicks = g_psReceiverAnaly->Attitude.Yaw.fallingEdgeTicks - \
														   g_psReceiverAnaly->Attitude.Yaw.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Attitude.Yaw.UPDATE_STATUS = WAVE_STATUS_NEW;					
			}
			
	   		TIM_OC4PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Attitude.Yaw.risingEdgeTicks = TIM_GetCapture4(g_sTimPwmIn_Attitude.Tim);
			
			g_psReceiverAnaly->Attitude.Yaw.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC4PolarityConfig(g_sTimPwmIn_Attitude.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Attitude.Tim, TIM_IT_CC4);	/*����жϱ�־λ*/	    
	}	

	/*��ʱ����������ж�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Attitude.Tim, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(g_sTimPwmIn_Attitude.Tim, TIM_IT_Update);	/*����жϱ�־λ*/	  	    
	}	
}


/*��̨�������ťͨ��(TIM1)����+����*/
void remot_Switch_Channel_Analy(void)
{	
	/*����CH1���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Switch.Tim, TIM_IT_CC1) != RESET)	
	{	
		if(g_psReceiverAnaly->Switch.SWA.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Switch.SWA.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Switch.SWA.fallingEdgeTicks = TIM_GetCapture1(g_sTimPwmIn_Switch.Tim);
			
			if (g_psReceiverAnaly->Switch.SWA.fallingEdgeTicks > \
				g_psReceiverAnaly->Switch.SWA.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Switch.SWA.avaTicks = g_psReceiverAnaly->Switch.SWA.fallingEdgeTicks - \
														 g_psReceiverAnaly->Switch.SWA.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Switch.SWA.UPDATE_STATUS = WAVE_STATUS_NEW;					
			}
			
	   		TIM_OC1PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Switch.SWA.risingEdgeTicks = TIM_GetCapture1(g_sTimPwmIn_Switch.Tim);
			
			g_psReceiverAnaly->Switch.SWA.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC1PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Switch.Tim, TIM_IT_CC1);	/*����жϱ�־λ*/	  	    
	}	
	
	/*����CH2���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Switch.Tim, TIM_IT_CC2) != RESET)	
	{	
		if(g_psReceiverAnaly->Switch.SWB.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Switch.SWB.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Switch.SWB.fallingEdgeTicks = TIM_GetCapture2(g_sTimPwmIn_Switch.Tim);
			
			if (g_psReceiverAnaly->Switch.SWB.fallingEdgeTicks > \
				g_psReceiverAnaly->Switch.SWB.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Switch.SWB.avaTicks = g_psReceiverAnaly->Switch.SWB.fallingEdgeTicks - \
													     g_psReceiverAnaly->Switch.SWB.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Switch.SWB.UPDATE_STATUS = WAVE_STATUS_NEW;					
			}
			
	   		TIM_OC2PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Switch.SWB.risingEdgeTicks = TIM_GetCapture2(g_sTimPwmIn_Switch.Tim);
			
			g_psReceiverAnaly->Switch.SWB.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC2PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Switch.Tim, TIM_IT_CC2);	/*����жϱ�־λ*/	    
	}

	/*����CH3���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Switch.Tim, TIM_IT_CC3) != RESET)	
	{	
		if(g_psReceiverAnaly->Switch.SWC.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Switch.SWC.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Switch.SWC.fallingEdgeTicks = TIM_GetCapture3(g_sTimPwmIn_Switch.Tim);
			
			if (g_psReceiverAnaly->Switch.SWC.fallingEdgeTicks > \
				g_psReceiverAnaly->Switch.SWC.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵĸߵ�ƽ����*/
				g_psReceiverAnaly->Switch.SWC.avaTicks = g_psReceiverAnaly->Switch.SWC.fallingEdgeTicks - \
														 g_psReceiverAnaly->Switch.SWC.risingEdgeTicks;
				
				/*pwm����״̬����*/
				g_psReceiverAnaly->Switch.SWC.UPDATE_STATUS = WAVE_STATUS_NEW;
			}
			
	   		TIM_OC3PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Switch.SWC.risingEdgeTicks = TIM_GetCapture3(g_sTimPwmIn_Switch.Tim);
			
			g_psReceiverAnaly->Switch.SWC.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC3PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Switch.Tim, TIM_IT_CC3);	/*����жϱ�־λ*/	  	    
	}	
	
	/*����CH4���������¼�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Switch.Tim, TIM_IT_CC4) != RESET)	
	{	
		if(g_psReceiverAnaly->Switch.SWD.CAP_EDGE == CAP_EDGE_FALLING)	/*���β������½��� */
		{	  			
			g_psReceiverAnaly->Switch.SWD.CAP_EDGE = CAP_EDGE_RISING;	/*����´β��񵽵ı���״̬Ϊ������*/
					
			/*�����½���ʱ�̼���*/
			g_psReceiverAnaly->Switch.SWD.fallingEdgeTicks = TIM_GetCapture4(g_sTimPwmIn_Switch.Tim);
			
			if (g_psReceiverAnaly->Switch.SWD.fallingEdgeTicks > \
				g_psReceiverAnaly->Switch.SWD.risingEdgeTicks)
			{
				/*�õ�һ��PWM���ڵ���Ч����*/
				g_psReceiverAnaly->Switch.SWD.avaTicks = g_psReceiverAnaly->Switch.SWD.fallingEdgeTicks - \
													     g_psReceiverAnaly->Switch.SWD.risingEdgeTicks;

				/*pwm����״̬����*/
				g_psReceiverAnaly->Switch.SWD.UPDATE_STATUS = WAVE_STATUS_NEW;				
			}
			
	   		TIM_OC4PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Rising); /*����Ϊ�����ز���*/
		}
		else  						/*���β����������� */
		{
			/*����������ʱ�̼���*/
 			g_psReceiverAnaly->Switch.SWD.risingEdgeTicks = TIM_GetCapture4(g_sTimPwmIn_Switch.Tim);
			
			g_psReceiverAnaly->Switch.SWD.CAP_EDGE = CAP_EDGE_FALLING;	/*����´β��񵽵ı���״̬Ϊ�½���*/
			
	   		TIM_OC4PolarityConfig(g_sTimPwmIn_Switch.Tim, TIM_ICPolarity_Falling);		/*CC1P=1 ����Ϊ�½��ز���*/
		}
		
		TIM_ClearITPendingBit(g_sTimPwmIn_Switch.Tim, TIM_IT_CC4);	/*����жϱ�־λ*/	    
	}

	/*��ʱ����������ж�*/
	if (TIM_GetITStatus(g_sTimPwmIn_Switch.Tim, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(g_sTimPwmIn_Switch.Tim, TIM_IT_Update);	/*����жϱ�־λ*/	  	    
	}	
}

/*�����(TIM1)����+����*/
void remot_Gimbal_Channel_Analy(void)
{
	
}
#endif

#if defined(REMOTE_DATA_RECV__PPM)
Remot_PPM_Recv_Tick g_sRemot_PPM_Recv_Tick = {0};
/*=== MCU PPM���� ===*/
void remot_PPM_AllChannel_Analy(void)
{
	/*ϵͳ����ʱ���ȡ,us*/
	g_sRemot_PPM_Recv_Tick.lastTick = g_sRemot_PPM_Recv_Tick.curTick;
	
	/*���㵱ǰʱ��us*/
	g_sRemot_PPM_Recv_Tick.curTick = (g_sTimExecutePeriod.Period * \
									  g_vu32_Time_Period_Cnt_Foc_Ms) + \
									  g_sTimExecutePeriod.Tim->CNT; /*us*/
	
	/*����ʱ���*/
	g_sRemot_PPM_Recv_Tick.deltaTick = g_sRemot_PPM_Recv_Tick.curTick - g_sRemot_PPM_Recv_Tick.lastTick;
	
	if (g_psReceiverAnaly->PPM_RECV_STATUS == REMOT_PPM_RECV_START)
	{
		/*��Ч��������*/
		if ((800 <= g_sRemot_PPM_Recv_Tick.deltaTick) && \
			(g_sRemot_PPM_Recv_Tick.deltaTick <= 2200))
		{
			g_psReceiverAnaly->PPM_Buff[g_psReceiverAnaly->PPM_SAMPLE_CHANNLE] = g_sRemot_PPM_Recv_Tick.deltaTick;			
			g_psReceiverAnaly->PPM_SAMPLE_CHANNLE++;
			
			if (g_psReceiverAnaly->PPM_SAMPLE_CHANNLE >= REMOT_PPM_RECV_MAX_CHANNEL_NBR)
			{
				/*��ͨ������*/	
				/*attitude*/
				g_psReceiverAnaly->Attitude.Roll.avaTicks     = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_1ST_CNL];
				g_psReceiverAnaly->Attitude.Pitch.avaTicks    = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_2ND_CNL];
				g_psReceiverAnaly->Attitude.Throttle.avaTicks = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_3RD_CNL];
				g_psReceiverAnaly->Attitude.Yaw.avaTicks      = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_4TH_CNL];	
				
				/*switch*/
				g_psReceiverAnaly->Switch.SWA.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_5TH_CNL];
				g_psReceiverAnaly->Switch.SWB.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_6TH_CNL];
				g_psReceiverAnaly->Switch.SWC.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_7TH_CNL];
				g_psReceiverAnaly->Switch.SWD.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_8TH_CNL];				
				
				/*������ݸ���״̬*/
				/*attitude*/
				g_psReceiverAnaly->Attitude.Roll.UPDATE_STATUS     = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Pitch.UPDATE_STATUS    = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Throttle.UPDATE_STATUS = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Yaw.UPDATE_STATUS      = WAVE_STATUS_NEW;

				/*switch*/
				g_psReceiverAnaly->Switch.SWA.UPDATE_STATUS		   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWB.UPDATE_STATUS 	   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWC.UPDATE_STATUS		   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWD.UPDATE_STATUS 	   = WAVE_STATUS_NEW;			
				
				/*���PPM�������*/
				g_psReceiverAnaly->PPM_RECV_STATUS = REMOT_PPM_RECV_FINISH;
			}
		}
		else
		{
			if (g_sRemot_PPM_Recv_Tick.deltaTick >= 2500) /*֡������ƽ����2ms = 2000us*/
			{
				/*��ͨ������*/	
				/*attitude*/
				g_psReceiverAnaly->Attitude.Roll.avaTicks     = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_1ST_CNL];
				g_psReceiverAnaly->Attitude.Pitch.avaTicks    = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_2ND_CNL];
				g_psReceiverAnaly->Attitude.Throttle.avaTicks = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_3RD_CNL];
				g_psReceiverAnaly->Attitude.Yaw.avaTicks      = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_4TH_CNL];	
				
				/*switch*/
				g_psReceiverAnaly->Switch.SWA.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_5TH_CNL];
				g_psReceiverAnaly->Switch.SWB.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_6TH_CNL];
				g_psReceiverAnaly->Switch.SWC.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_7TH_CNL];
				g_psReceiverAnaly->Switch.SWD.avaTicks 		  = g_psReceiverAnaly->PPM_Buff[REMOT_PPM_SAMPLE_8TH_CNL];
				
				/*������ݸ���״̬*/
				/*attitude*/
				g_psReceiverAnaly->Attitude.Roll.UPDATE_STATUS     = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Pitch.UPDATE_STATUS    = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Throttle.UPDATE_STATUS = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Attitude.Yaw.UPDATE_STATUS      = WAVE_STATUS_NEW;

				/*switch*/
				g_psReceiverAnaly->Switch.SWA.UPDATE_STATUS		   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWB.UPDATE_STATUS 	   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWC.UPDATE_STATUS		   = WAVE_STATUS_NEW;
				g_psReceiverAnaly->Switch.SWD.UPDATE_STATUS 	   = WAVE_STATUS_NEW;			
				
				/*���PPM�������*/
				g_psReceiverAnaly->PPM_RECV_STATUS    = REMOT_PPM_RECV_START;
				g_psReceiverAnaly->PPM_SAMPLE_CHANNLE = REMOT_PPM_SAMPLE_1ST_CNL;
			}
			else
			{
				g_psReceiverAnaly->PPM_RECV_STATUS = REMOT_PPM_RECV_FINISH;
			}
		}
	}
	else if (g_sRemot_PPM_Recv_Tick.deltaTick >= 2500) /*֡������ƽ����2ms = 2000us*/
	{
		g_psReceiverAnaly->PPM_RECV_STATUS    = REMOT_PPM_RECV_START;
		g_psReceiverAnaly->PPM_SAMPLE_CHANNLE = REMOT_PPM_SAMPLE_1ST_CNL;
	}
}
#endif
