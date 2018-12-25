#include "status_Aircraft.h"
#include "remot_DataAnaly.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

/*��ΪĳЩ״ֵ̬��ʼ����������0xff,
  ���ѡ��0xf0��0xf1��ʼ������״ֵ̬*/

AircraftStatus g_sAircraftStatus = 
{
	/*= 1.�����������״̬ =*/
	/*�ɿ�����״̬*/
	.LOCK_STATUS   				        = AIRCRAFT_LOCKING,    /*Ĭ������*/
	                                    
	/*����������״̬*/                  
	.CUR_FLY_STATUS   				        = AIRCRAFT_LANDING,	/*��ʼ��Ĭ����½��*/
	.LAST_FLY_STATUS                    = AIRCRAFT_LANDING,	/*�ϴη���������״̬*/	
	                                    
	/*����������ģʽ*/                  
	.CUR_FLY_TYPE                       = AIRCRAFT_FLY_TYPE_ATTITUDE, /*Ĭ�ϴ���̬����ģʽ*/
	.LAST_FLY_TYPE					    = AIRCRAFT_FLY_TYPE_ATTITUDE, /*Ĭ�ϴ���̬����ģʽ*/
		                                
	/*GPS HOME ������״̬*/             
	.HOME_STATUS			            = AIRCRAFT_HOME_NOTSET,	/*Ĭ��δ����GPS HOME��*/
	                                    
	/*ң�غͷ�����ͨ��״̬*/            
	.CMC_STATUS 				        = AIRCRAFT_CMC_FAIL,   /*Ĭ��ͨ��ʧ��*/
	                                    
	/*�˻�������ʾҳ��*/                
	.HCI_SHOW_PAGE                      = AIRCRAFT_HCI_SHOW_DISABLE, /*Ĭ�ϲ�����OLED��ʾ*/	
	                                    
	/*= 2.������������ʽ״̬ =*/	    
	.CUR_HEIGHT_CONTROL_MODE              = AIRCRAFT_CONTROL_AUTO,     	    /*����̬����*/
	.LAST_HEIGHT_CONTROL_MODE	          = AIRCRAFT_CONTROL_AUTO,     	    /*����̬����*/	
	.CUR_HORIZONTAL_CONTROL_MODE          = AIRCRAFT_CONTROL_AUTO,     	    /*����̬����*/
	.LAST_HORIZONTAL_CONTROL_MODE         = AIRCRAFT_CONTROL_AUTO,     	    /*����̬����*/
	.HEIGHT_REMOT_EXPECT_CONTROL_MODE     = AIRCRAFT_CONTROL_SENSOR_TO_AUTO, /*ң�������߶ȿ���ģʽ*/
	.HORIZONTAL_REMOT_EXPECT_CONTROL_MODE = AIRCRAFT_CONTROL_SENSOR_TO_AUTO, /*ң������ˮƽ����ģʽ*/		
    .HEIGHT_CONTROL_MODE                  = AIRCRAFT_CONTROL_AUTO,			 /*Ĭ��ʵ�ʸ߶ȿ���ģʽΪ����*/
	.HORIZONTAL_CONTROL_MODE              = AIRCRAFT_CONTROL_AUTO,			 /*Ĭ��ʵ��ˮƽ����ģʽΪ����*/	
	.HEIGHT_REFERENCE_SET_STATUS          = AIRCRAFT_REFERENCE_SET_NO,		/*�߶ȿ��ƹ����вο�������״̬*/
	.HORIZONTAL_REFERENCE_SET_STATUS      = AIRCRAFT_REFERENCE_SET_NO,		/*ˮƽ���ƹ����вο�������״̬*/	
	.ONEKEY_TAKEOFF_CONTROL_STATUS        = AIRCRAFT_ONEKEY_CTRL_DISABLE,		/*һ����ɿ���״̬*/
	.ONEKEY_TAKEOFF_TARGET_SET_STATUS     = AIRCRAFT_ONEKEY_TARGET_SET_NO,	/*һ�����Ŀ�������״̬*/
	.ONEKEY_TAKEOFF_TARGET_REACH_STATUS   = AIRCRAFT_ONEKEY_TARGET_REACH_NO,	/*һ�����Ŀ��㵽��״̬*/
	.ONEKEY_LAND_CONTROL_STATUS           = AIRCRAFT_ONEKEY_CTRL_DISABLE,		/*һ���������״̬*/	
	.ONEKEY_CRUISE_CONTROL_STATUS         = AIRCRAFT_ONEKEY_CTRL_DISABLE, 	/*һ��Ѳ������״̬*/
								        
	/*= 3.��̬����״̬ =*/              
	/*�߶�*/                            
	/*��ѹ����ο�����ѹֵ״̬*/        
	.BERO_ZERO_PRESSURE 		        = HEIGHT_DATA_STATUS_NO,  /*Ĭ����Ч*/
								        
	/*��ѹ�Ƶ�ǰ��ѹֵ״̬*/            
	.BERO_PRESSURE 				        = HEIGHT_DATA_STATUS_NO,  /*Ĭ����Ч*/
								        
	/*��ѹ�ƹ۲�߶�ֵ״̬*/	        
	.BERO_ESTIMATE_ALTITUDE 	        = HEIGHT_DATA_STATUS_NO,  /*Ĭ����Ч*/
								        
	/*��������ο���߶�ֵ״̬*/        
	.ULTR_ZERO_ALTITUDE 		        = HEIGHT_DATA_STATUS_NO,  /*Ĭ����Ч*/
								        
	/*�������۲�߶�ֵ״̬*/            
	.ULTR_ESTIMATE_ALTITUDE 	        = HEIGHT_DATA_STATUS_NO,  /*Ĭ����Ч*/
	
	/*�߶�(�ۺ����и߶ȴ���)�ο��������״̬*/
	.ZERO_ALTITUDE				        = HEIGHT_BERO_ULTR_ZERO_DISAVA,   /*Ĭ��Ϊ0,����״̬������*/
								        
	/*ˮƽ*/                            
	/*GPS�۲�ˮƽֵ״̬*/	            
	.GPS_ESTIMATE_HORIZONTAL 	        = HORIZONTAL_DATA_STATUS_NO, /*Ĭ����Ч*/
								        
	/*�����۲�ˮƽֵ״̬*/	            
	.OPFLOW_ESTIMATE_HORIZONTAL         = HORIZONTAL_DATA_STATUS_NO, /*Ĭ����Ч*/
	
	/*ˮƽ(�ۺ�����ˮƽ����)�ο��������״̬*/	
	.ZERO_HORIZONTAL                    = HORIZONTAL_GPS_OPFLOW_ZERO_DISAVA, /*Ĭ��Ϊ0,����״̬������*/

	/*=== 4.�߶�&ˮƽ��������Ч��״̬ ===*/
	/*�������۲�߶�ֵ״̬*/	
	.ESTIMATE_ALTITUDE     		        = HEIGHT_BERO_ULTR_DISAVA,  /*Ĭ��Ϊ0,����״̬������*/

	/*�������۲�ˮƽֵ״̬*/
	.ESTIMATE_HORIZONTAL 		        = HORIZONTAL_GPS_OPFLOW_DISAVA,  /*Ĭ��Ϊ0,����״̬������*/	
	
	/*=== 5.��������� ===*/
	.GPS_DATA_FIRST_AVA_STATUS          = GPS_DATA_FIRST_DISAVA,		 /*��һ��ʹ��(HOME������)GPS������Ч��*/	
	.GPS_QUALITY_STATUS					= GPS_DATA_QUALITY_BAD, 		 /*Ĭ��GPS�ź�������*/
	.CUR_VERTICAL_SENSOR                = VERTICAL_SENSOR_USE_BERO,		 /*������ֱ���򴫸���*/
	.LAST_VERTICAL_SENSOR				= VERTICAL_SENSOR_USE_BERO,		 /*�ϴ���ֱ���򴫸���*/
	
	/*=== 6.��������״̬ ==*/
	.PLATFORM_INIT_STATUS			    = PROGRAME_INIT_START,			 /*Ĭ�ϳ����ʼ����ʼ*/	
};

AircraftStatus *g_psAircraftStatus = &g_sAircraftStatus;


/*���GPS�Ƿ����(���㶨λ����+�����ںϳɹ�)*/
GPS_POS_FIX_AVA_STATUS status_GPS_Fix_Ava_Check(AircraftStatus *aircraftStatus)
{
	/*1.HOME�������� 
	  2.GPS�������㶨λ����
	  3.�����ںϳɹ�
	*/
	if ((g_psAircraftStatus->HOME_STATUS == AIRCRAFT_HOME_SET) && \
		(g_psAircraftStatus->GPS_ESTIMATE_HORIZONTAL == HORIZONTAL_DATA_STATUS_OK) && \
		(g_psSinsReal->FUSION_STATUS[SINS_FUSION_HORIZONTAL] == SINS_FUSION_SUCC))
	{
		aircraftStatus->GPS_AVA_STATUS = GPS_POS_FIX_AVA_TRUE;
	}
	else
	{
		aircraftStatus->GPS_AVA_STATUS = GPS_POS_FIX_AVA_FALSE;
	}
	
	return (aircraftStatus->GPS_AVA_STATUS);
}

SimulateWatchDog g_sUavRemotCMCDog = 
{
	.curTicks         = 0, /*��ʼֵ������0*/
	.nextProcessTicks = 0, /*��ʼֵ������0*/
};

SimulateWatchDog *g_psUavRemotCMCDog = &g_sUavRemotCMCDog;

/*ң����ɻ�ͨѶ״̬*/
AIRCRAFT_CMC_STATUS status_check_aircraft_remot_communication(SimulateWatchDog *uavRemotCMCDog, AircraftStatus *aircraftStatus)
{		
	uavRemotCMCDog->curTicks = my_GetTick();	/*��ȡ��ǰtick foc = 10ms*/
	 
	/*���curTicks < nextProcessTicks,˵��"ι��"��������,��������*/
	if (uavRemotCMCDog->curTicks <= uavRemotCMCDog->nextProcessTicks)
	{
		/*���ͨ�ųɹ�*/
		aircraftStatus->CMC_STATUS = AIRCRAFT_CMC_SUCC;
		
		/*ͨ�ųɹ���,���±���Զ�����δ���� �� �ָ����Զ�����ǰ����ģʽ*/
		if (g_psControlAircraft->GO_HOME_SET == CTRL_AIRCRAFT_GO_HOME_SET)
		{
			/*���η���ģʽΪ�ϴη���ģʽ(���ڴ�ʧ�����ָ�ͨѶ)*/
			aircraftStatus->CUR_FLY_TYPE = aircraftStatus->LAST_FLY_TYPE;
		
			/*����Զ�����δ�趨*/
			g_psControlAircraft->GO_HOME_SET = CTRL_AIRCRAFT_GO_HOME_NOTSET;
		}
	}
	/*����;��(�����Զ�����)ʧ�ط���,������ģʽ��,��ֹ�л����Զ�����ģʽ*/
	else
	{
		/*���ͨ��ʧ��*/
		aircraftStatus->CMC_STATUS = AIRCRAFT_CMC_FAIL;
	
		/*�ж��Ƿ�����ʧ������*/
		if (((remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_ZERO)     != REMOT_DATA_ZERO) || \
			  (remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_ZERO)    != REMOT_DATA_ZERO) || \
			  (remot_Data_Range(g_sRemotData.AttThrottle, REMOT_DATA_ZERO) != REMOT_DATA_ZERO) || \
			  (remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_ZERO)      != REMOT_DATA_ZERO)) && \
			  (g_psControlAircraft->GO_HOME_STATUS == CTRL_AIRCRAFT_GO_HOME_ENABLE) && \
			  (aircraftStatus->LOCK_STATUS == AIRCRAFT_UNLOCK))
		{	

		
			/*�Զ����� �ϴη���״̬���� �� ��ǰ����״̬���� ֻ����һ��*/
			if (g_psControlAircraft->GO_HOME_SET == CTRL_AIRCRAFT_GO_HOME_NOTSET)
			{	
				/*��¼ʧ��ǰ�ķ���ģʽ,�ָ�ͨ�ź�,�����л�֮ǰģʽ*/
				aircraftStatus->LAST_FLY_TYPE = aircraftStatus->CUR_FLY_TYPE;
		
				/*ʧ�����Ƿ���ģʽΪ�Զ�����*/
				aircraftStatus->CUR_FLY_TYPE = AIRCRAFT_FLY_TYPE_GO_HOME;
			
				/*����Զ��������趨*/
				g_psControlAircraft->GO_HOME_SET = CTRL_AIRCRAFT_GO_HOME_SET;
			}
		}
	}
	
	return (aircraftStatus->CMC_STATUS);
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
