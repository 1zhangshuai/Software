#include "sys_OsTask.h"

/*�̻߳��Ѽ�����*/
rtos_thread_wake_up gs_rtos_thread_wake_up = 
{
	/*��������*/
	.base_module       = 1,
	/*���������ݻ�ȡ����̬�ں�*/		
	.euler_angle       = 1,
	.ver_fusion        = 1,
	.gps_hor_fusion    = 1,
	.opflow_hor_fusion = 1,
	/*����ϵͳ*/	
	.uav_ctrl   	   = 1,
	/*У׼ϵͳ*/	
	.uav_calib 		   = 1,
	/*�������*/	
	.tfsd_fly_log 	   = 1,
	/*�˻�����*/	
	.hci_oled_show 	   = 1,
	.hci_slave_host    = 1,	
};

#define RTOS_UPDATE_TOTAL_THREAD_NUMBER		(01 + 01 + 04 + 01 + 01 + 01 + 02 = 11)
/*0********** ����ģ������̶߳�ջ����(01/01) ***********/
/*1*************** ������дϵͳ(01/01) ******************/
/*2****** ���������ݻ�ȡ�������̶߳�ջ����(04/04) *******/
/*3************ �ɿؿ����̶߳�ջ����(01/01) *************/
/*4************ ϵͳУ׼�̶߳�ջ����(01/01) *************/
/*5************ ������־�̶߳�ջ����(01/01) *************/
/*6************ �˻������̶߳�ջ����(02/02) *************/

/*==================== �̶߳�ջ���估���Ⱥ�����ʼ�� ====================*/
/*0********** ����ģ������̶߳�ջ����(01/01) ***********/
/*0.����ģ��*/
#define RTOS_THREAD_STACK_BASE_MODULE			  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_base_module_stack[RTOS_THREAD_STACK_BASE_MODULE];
static struct rt_thread base_module_thread;	

void rt_entry_thread_base_module(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&base_module_sem, RT_WAITING_FOREVER);

		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->BaseModule));		
		
		/*LED*/
		#ifdef HW_CUT__USE_LED
		thread_led_ctrl(parameter);
		#endif
		
		/*KEY*/
		#ifdef HW_CUT__USE_KEY  
		thread_key_check(parameter);
		#endif
		
		/*RGB*/
		#ifdef HW_CUT__USE_RGB  
		thread_rgb_ctrl(parameter);
		#endif		
	}
}                                     

/*1*************** ������дϵͳ(01/01) ******************/
/* 0.������д */
#define RTOS_THREAD_STACK_SIZE_STOR_PARA_RDWR 	  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_stor_para_rdwr_stack[RTOS_THREAD_STACK_SIZE_STOR_PARA_RDWR];
static struct rt_thread stor_para_rdwr_thread;

void rt_entry_thread_stor_para_rdwr(void* parameter)
{
	rt_uint32_t event;
	
	while(1)
	{	
		/*flash rdwr*/		
		#ifdef STOR_MCU__FLASH
		#endif
		
		/*eeprom rdwr*/
		#ifdef STOR_BOARD__AT24CXX
		/*PID�������������(����PID����)*/
		if (rt_event_recv(&para_rdwr_event, ((1 << RTOS_EVENT_PID_PARA_UPDATE_BY_HOST)),
						  RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          5, &event) == RT_EOK)
		{
			pid_parameter_save_or_reset(&g_sPidSystem);
		}
		#endif
	}
}	

/*2****** ���������ݻ�ȡ�������̶߳�ջ����(04/04) *******/
/* 0.���ٶȼƺ�������(IMU)+������(MAG)���ݻ�ȡ������ŷ���� */
#define RTOS_THREAD_STACK_SIZE_EULER_ANGLE_CALCULATE	      (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_euler_angle_calculate_stack[RTOS_THREAD_STACK_SIZE_EULER_ANGLE_CALCULATE];
static struct rt_thread euler_angle_calculate_thread;

void rt_entry_thread_euler_angle_calculate(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&euler_angle_sem, RT_WAITING_FOREVER);
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->EulerAngle));
		
		/*IMU���ݻ�ȡ������*/
		#ifdef HW_CUT__USE_MD_IMU	/*MD IMU*/	
		ahrs_imu_data_get_and_dp();
		#endif
		
		#ifdef HW_CUT__USE_BD_IMU	/*BD IMU*/	
		ahrs_imu_data_get_and_dp();
		#endif
		
		/*MAG���ݻ�ȡ������*/
		#ifdef HW_CUT__USE_MD_MAG	/*MD MAG*/
		#ifdef HW_CUT__USE_GPS_MAG	/*GPS MAG*/
		ahrs_mag_data_get_and_dp(); /*GPS MAG�滻MD MAG*/
		#else
		ahrs_mag_data_get_and_dp(); /*MD MAG*/
		#endif
		#endif
		
		/*ŷ���Ǽ���: 96us*/
		ahrs_grades_calculat_attitude(g_psAhrsQuater, \
								  	  g_psAccAttData, \
								      g_psGyroAttData, \
								      g_psGyroFilter, \
								      g_psMagAttData->magYaw, \
								      g_psAhrsAttitude);																		 
			
		/*���·������Ҿ���*/
		ahrs_compute_rotation_matrix();
	}
}

/* 1.��ֱ���������ݻ�ȡ�������ں� */
#define RTOS_THREAD_STACK_SIZE_VERTICAL_DATA_FUSION   (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_vertical_fusion_stack[RTOS_THREAD_STACK_SIZE_VERTICAL_DATA_FUSION];
static struct rt_thread vertical_fusion_thread;

void rt_entry_thread_vertical_fusion(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&ver_fusion_sem, RT_WAITING_FOREVER);
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->VerFusion));
		
		/*��ѹ�����ݸ��¼�����*/
		#ifdef HW_CUT__USE_MD_BERO
		#ifdef HW_CUT__USE_DB_BERO
		bero_altitude_data_get_and_dp(g_psUav_Status);	
		#else
		bero_altitude_data_get_and_dp(g_psUav_Status);
		#endif
		#endif
		
		/*���������ݸ��¼�����*/		
		#if defined(HW_CUT__USE_ULTR)		
		ultr_altitude_data_get_and_dp(g_psUav_Status);
		#endif
		
		/*����ϵ->����ϵ ���ٶ�*/
		sins_get_body_relative_earth_acc(g_psAccSINS);
		
		/*��ֱ��ο����Ѿ��趨,����ֱ����������,���ɽ��������ں�*/
		if ((g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_OK) || \
			(g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_OK))
		{
			#ifdef SINS_DATA_FUSION__VER_THIRDORDER
			/*���׻�������ֱ�����ϵļ��ٶȡ��ٶȡ�λ��(Z��ֱ)*/
			sins_thirdorder_complement_vertical();
			#endif
			
			#ifdef SINS_DATA_FUSION__VER_KALMAN
			/*�������˲�����ֱ�����ϵļ��ٶȡ��ٶȡ�λ��(Z��ֱ)*/
			sins_kalman_estimate_vertical();
			#endif
		}	
	}
}

/* 2.��ȡGPS��������*/
#define RTOS_THREAD_STACK_SIZE_GPS_CTRL_DATA_GET	   	(2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_gps_ctrl_data_get_stack[RTOS_THREAD_STACK_SIZE_GPS_CTRL_DATA_GET];
static struct rt_thread gps_ctrl_data_get_thread;	

void rt_entry_gps_ctrl_data_get(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&gps_update_sem, RT_WAITING_FOREVER);	

		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->GpsDataGet));
		
		/*��ȡgps��������*/
		gps_fix_position_data_get(g_sGpsM8N.PvtData, &g_sAttitudeAll.GpsData);	
	}
}

/* 3.��ȡOPTIC FLOW��������*/
#define RTOS_THREAD_STACK_SIZE_OPTICFLOW_CTRL_DATA_GET   	(2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_opticflow_ctrl_data_get_stack[RTOS_THREAD_STACK_SIZE_OPTICFLOW_CTRL_DATA_GET];
static struct rt_thread opticflow_ctrl_data_get_thread;	

void rt_entry_opticflow_ctrl_data_get(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&opflow_update_sem, RT_WAITING_FOREVER);	

		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->OpFlowDataGet));		
		
		/*��ȡopticflow��������*/
		opflow_Offset_Relative_To_Home(g_sOpFlowUpixelsLC306.OpFlowData, \
									   g_psSinsReal->curPosition[EARTH_FRAME_Z], \
									   &g_sAttitudeAll);
	}
}

/* 4.GPSˮƽ���ݻ�ȡ�������ں� */
#define RTOS_THREAD_STACK_SIZE_GPS_DATA_HORIZONTAL_FUSION   	(2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_gps_data_horizontal_fusion_stack[RTOS_THREAD_STACK_SIZE_GPS_DATA_HORIZONTAL_FUSION];
static struct rt_thread gps_data_horizontal_fusion_thread;	

void rt_entry_gps_data_horizontal_fusion(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&gps_hor_fusion_sem, RT_WAITING_FOREVER);	

		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->GpsHorFusion));
		
		#ifdef HW_CUT__USE_GPS	/*GPS*/
		/*�ж�HOME�Ƿ��Ѿ��趨(ÿ���ϵ���GPS����ֻ�趨һ��)*/
		gps_home_location_set();
		#endif
		
		/*����ϵ->����ϵ ���ٶ�*/
		sins_get_body_relative_earth_acc(g_psAccSINS);
		
		/*HOME�Ѿ��趨,���ɽ��������ں�*/
		if (g_sUav_Status.HOME_SET_STATUS == UAV_HOME_SET_YES)
		{
			#ifdef SINS_DATA_FUSION__HOR_THIRDORDER
			/*ˮƽX��Y����:�����ߵ�(���׻���)�ں�����*/
			sins_thirdorder_complement_horizontal();
			#endif
			
			#ifdef SINS_DATA_FUSION__HOR_KALMAN
			/*�������˲���ˮƽ�����ϵļ��ٶȡ��ٶȡ�λ��(X,Yˮƽ)*/
			sins_kalman_estimate_horizontal();
			#endif
		}		
	}
}

/* 5.OPTIC FLOWˮƽ���ݻ�ȡ�������ں� */
#define RTOS_THREAD_STACK_SIZE_OPTICFLOW_DATA_HORIZONTAL_FUSION   	(2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_opticflow_data_horizontal_fusion_stack[RTOS_THREAD_STACK_SIZE_OPTICFLOW_DATA_HORIZONTAL_FUSION];
static struct rt_thread opticflow_data_horizontal_fusion_thread;	

void rt_entry_opticflow_data_horizontal_fusion(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&opflow_hor_fusion_sem, RT_WAITING_FOREVER);

		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->OpflowHorFusion));		
		
		/*�Թ������ݽ��д���,�������Ҫ���ٶ�,λ����Ϣ*/
		if ((g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
			(g_sOpFlowUpixelsLC306.UPDATE_STATUS == OPFLOW_UPIXELSLC306_UPDATE_SUCC))
		{

		}	
	}
}

/*3************ �ɿؿ����̶߳�ջ����(01/01) *************/
#define RTOS_THREAD_STACK_SIZE_UAV_CTRL_SYSTEM			  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_uav_ctrl_system_stack[RTOS_THREAD_STACK_SIZE_UAV_CTRL_SYSTEM];
static struct rt_thread uav_ctrl_system_thread;

void rt_entry_uav_ctrl_system(void* parameter)
{
	while(1)
	{	
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&uav_ctrl_sem, RT_WAITING_FOREVER);		
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->UavCtrl));
		
		/*����ң������(������"���ң�غͷ�����ͨ���Ƿ�����"ǰ��ִ��)*/
		remot_get_all_channel_data(g_psRemotData, g_psReceiverAnaly);
	
		/*���ң�غͷ�����ͨ���Ƿ�����(������ "����ң������"����ִ��)*/
		status_check_aircraft_remot_communication(&g_sUavRemotCMCDog, g_psUav_Status);
	
		/*���ң������״̬*/
		remot_aircraft_lock_and_unlock();
		
		/*����ϵͳ����*/
		ctrl_auto_control_system_dp();
		
		/*����ϵͳ�����ʩ��PWM������������*/
		ctrl_auto_control_system_output();
	}
}

/*4************ ϵͳУ׼�̶߳�ջ����(01/01) *************/
#define RTOS_THREAD_STACK_SIZE_UAV_CALIB_SYSTEM	 	  	  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_uav_calib_system_stack[RTOS_THREAD_STACK_SIZE_UAV_CALIB_SYSTEM];
static struct rt_thread uav_calib_system_thread;

void rt_entry_tuav_calib_system(void* parameter)
{
	while(1)
	{	
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&uav_calib_sem, RT_WAITING_FOREVER);		
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->UavCalib));		
		
		/*���ٶȼ�У׼*/
		if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
			(g_sHciShowPage.SHOW_DATA_STATUS != UAV_HCI_SHOW_ENABLE))
		{
			/*��ȡң��ָ��У׼�����*/
			g_psAccCalibSystem->CUR_SIDE_INDEX = calib_acc_sensor_check();
			
			/*��Ч����ſ�ʼУ׼*/
			if ((g_psAccCalibSystem->CUR_SIDE_INDEX != SIDE_INDEX_NULL) || \
				(g_psAccCalibSystem->ENTRY_STATUS == ENTRY_CALIB_INTO) || \
				(g_psAccCalibSystem->ENTRY_STATUS == ENTRY_CALIB_EXIT))
			{
				calib_acc_sensor_dp(g_psAccCalibSystem->CUR_SIDE_INDEX);
			}
		}
		
		/*������У׼*/
		if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
			(g_sHciShowPage.SHOW_DATA_STATUS != UAV_HCI_SHOW_ENABLE))
		{
			/*��ȡң��ָ��У׼�����*/
			g_psMagCalibSystem->CUR_SIDE_INDEX = calib_mag_sensor_check();
			
			/*��Ч����ſ�ʼУ׼*/
			if ((g_psMagCalibSystem->CUR_SIDE_INDEX != SIDE_INDEX_NULL) || \
				(g_psMagCalibSystem->ENTRY_STATUS == ENTRY_CALIB_INTO) || \
				(g_psMagCalibSystem->ENTRY_STATUS == ENTRY_CALIB_EXIT))				
			{
				calib_mag_sensor_dp(g_psMagCalibSystem->CUR_SIDE_INDEX);
			}
		}
		
		/*����г�У׼,�������Ҫ�ε���غ����*/
		if (1){}
		
		/*ˮƽ�Ƕ�У׼*/
		if (1){}
	}
}

/*5************ ������־�̶߳�ջ����(01/01) *************/
#define RTOS_THREAD_STACK_SIZE_TFSD_FLY_LOG	 	  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_tfsd_fly_log_stack[RTOS_THREAD_STACK_SIZE_TFSD_FLY_LOG];
static struct rt_thread tfsd_fly_log_thread;

void rt_entry_thread_tfsd_fly_log(void* parameter)
{	
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&tfsd_fly_log_sem, RT_WAITING_FOREVER);
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->FlyLog));		
		
		/*����CPUʹ����*/
		g_sUav_Status.UavProgrameStatus.CpuUse.major = cpu_usage_major; /*CPU ʹ���� ����*/
		g_sUav_Status.UavProgrameStatus.CpuUse.minor = cpu_usage_minor; /*CPU ʹ���� С��*/	
	}
}

/*6************ �˻������̶߳�ջ����(02/02) *************/
/* OLED����վ��ʾ */
#define RTOS_THREAD_STACK_SIZE_HCI_OLED_SHOW 	  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_hci_oled_show_stack[RTOS_THREAD_STACK_SIZE_HCI_OLED_SHOW];
static struct rt_thread hci_oled_show_thread;

void rt_entry_thread_hci_oled_show(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&hci_oled_show, RT_WAITING_FOREVER);		
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->HciOledShow));
		
		/*��ȡң�ؿ���OLED��ʾ*/
		hci_remot_switch_show_status(&g_sHciShowPage);
		
		/*OLED��ʾ����*/
		hci_show_on_run_progress();
	}
}

/* ��λ������λ������ */
#define RTOS_THREAD_STACK_SIZE_HCI_SLAVE_AND_HOST 	  (2048)
ALIGN(RT_ALIGN_SIZE)

static u8 thread_hci_slave_and_host_stack[RTOS_THREAD_STACK_SIZE_HCI_SLAVE_AND_HOST];
static struct rt_thread hci_slave_and_host_thread;

void rt_entry_thread_hci_slave_and_host(void* parameter)
{
	while(1)
	{
		/*�ȴ��ź������ͷ�*/
		rt_sem_take(&hci_host_slave_sem, RT_WAITING_FOREVER);		
		
		/*�߳�ִ�����ڼ���*/
		get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->HciHostSlave));
		
		/*�ϴ���������λ��*/
		if (g_psPidSystem->PidSettingSystem.PARA_DOWNLOAD_STATUS != PID_PARA_DOWNLOAD_START)
		{
			if ((g_sUserSendHostSystem.HOST_TARG == USER_HOST_CHOOSE_ANO) && \
				(g_sUserSendHostSystem.MSG_ID == USER_ANO_MSG_EXCHANGE))									 
			{
				ANO_DT_Data_Exchange();																		 /*������λ��: ���ݽ���*/
			}
			else if ((g_sUserSendHostSystem.HOST_TARG == USER_HOST_CHOOSE_ANO) && \
					 (g_sUserSendHostSystem.MSG_ID != USER_ANO_MSG_EXCHANGE))
			{
				user_ANO_Send_Host_Wave_Data(g_sUserSendHostSystem.MSG_ID, g_sUserSendHostSystem.period_5MS);   /*������λ��: �ϴ���������*/
			}
			else if ((g_sUserSendHostSystem.HOST_TARG == USER_HOST_CHOOSE_VCAN) && \
					 (g_sUserSendHostSystem.MSG_ID != USER_ANO_MSG_EXCHANGE))
			{
				user_VCAN_Send_Host_Wave_Data(g_sUserSendHostSystem.MSG_ID, g_sUserSendHostSystem.period_5MS);  /*ɽ����λ��: �ϴ���������*/			
			}
		}	
	}
}

/*======================= �߳����񴴽�����ʼ���� =======================*/
/*�����߳�*/
void rtos_thread_create(void)
{
	rt_err_t err;
	
/*0********** ����ģ������̶߳�ջ����(01/01) ***********/
/*0.����ģ��*/
	err = rt_thread_init(&base_module_thread,
                         "rt_base_module",
						 rt_entry_thread_base_module,
                         RT_NULL,
                         thread_base_module_stack,
                         sizeof(thread_base_module_stack),
						 RTOS_PR_BASE_MODULE,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&base_module_thread);
    }                                                                                           										                                                    												                  										     

/*1*************** ������дϵͳ(01/01) ******************/
/* 0.������д */
	err = rt_thread_init(&stor_para_rdwr_thread,
                         "rt_stor_para_rdwr",
						 rt_entry_thread_stor_para_rdwr,
                         RT_NULL,
                         thread_stor_para_rdwr_stack,
                         sizeof(thread_stor_para_rdwr_stack),
						 RTOS_PR_STOR_PARA_RDWR,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&stor_para_rdwr_thread);
    }	
	
/*2****** ���������ݻ�ȡ�������̶߳�ջ����(04/04) *******/
/* 0.���ٶȼƺ�������(IMU)+������(MAG)���ݻ�ȡ������ŷ���� */                                                
	err = rt_thread_init(&euler_angle_calculate_thread,
                         "rt_euler_angle",
						 rt_entry_thread_euler_angle_calculate,
                         RT_NULL,
                         thread_euler_angle_calculate_stack,
                         sizeof(thread_euler_angle_calculate_stack),
						 RTOS_PR_EULER_ANGLE_CALCULATE,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&euler_angle_calculate_thread);
    }	
	
/* 1.��ֱ���������ݻ�ȡ�������ں� */                                               
	err = rt_thread_init(&vertical_fusion_thread,
                         "rt_ver_fus",
						 rt_entry_thread_vertical_fusion,
                         RT_NULL,
                         thread_vertical_fusion_stack,
                         sizeof(thread_vertical_fusion_stack),
						 RTOS_PR_DATA_VERTICAL_FUSION,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&vertical_fusion_thread);
    }
	
/* 2.��ȡGPS��������*/	
	err = rt_thread_init(&gps_ctrl_data_get_thread,
                         "rt_gps_data",
						 rt_entry_gps_ctrl_data_get,
                         RT_NULL,
                         thread_gps_ctrl_data_get_stack,
                         sizeof(thread_gps_ctrl_data_get_stack),
						 RTOS_PR_GPS_CTRL_DATA_GET,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&gps_ctrl_data_get_thread);
    }	
	
/* 3.��ȡOPTIC FLOW��������*/
	err = rt_thread_init(&opticflow_ctrl_data_get_thread,
                         "rt_opflow_data",
						 rt_entry_opticflow_ctrl_data_get,
                         RT_NULL,
                         thread_opticflow_ctrl_data_get_stack,
                         sizeof(thread_opticflow_ctrl_data_get_stack),
						 RTOS_PR_OPFLOW_CTRL_DATA_GET,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&opticflow_ctrl_data_get_thread);
    }		
	
/* 4.GPSˮƽ���ݻ�ȡ�������ں� */                                                
	err = rt_thread_init(&gps_data_horizontal_fusion_thread,
                         "rt_gps_hor_fus",
						 rt_entry_gps_data_horizontal_fusion,
                         RT_NULL,
                         thread_gps_data_horizontal_fusion_stack,
                         sizeof(thread_gps_data_horizontal_fusion_stack),
						 RTOS_PR_GPS_HORIZONTAL_FUSION,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&gps_data_horizontal_fusion_thread);
    }
	
/* 5.OPTIC FLOWˮƽ���ݻ�ȡ�������ں� */                                                
	err = rt_thread_init(&opticflow_data_horizontal_fusion_thread,
                         "rt_opflow_hor_fus",
						 rt_entry_opticflow_data_horizontal_fusion,
                         RT_NULL,
                         thread_opticflow_data_horizontal_fusion_stack,
                         sizeof(thread_opticflow_data_horizontal_fusion_stack),
						 RTOS_PR_OPFLOW_HORIZONTAL_FUSION,
                         20);
	if (err == RT_EOK)
    {
        rt_thread_startup(&opticflow_data_horizontal_fusion_thread);
    }
	
/*3************ �ɿغ����̶߳�ջ����(01/01) *************/
/* 0.����ϵͳ */
	err = rt_thread_init(&uav_ctrl_system_thread,
						 "rt_uav_ctrl",
						 rt_entry_uav_ctrl_system,
						 RT_NULL,
						 thread_uav_ctrl_system_stack,
						 sizeof(thread_uav_ctrl_system_stack),
						 RTOS_PR_UAV_CTRL_SYSTEM,
						 20);
	if (err == RT_EOK)
    {
		rt_thread_startup(&uav_ctrl_system_thread);
	}

/*4************ ϵͳУ׼�̶߳�ջ����(01/01) *************/
/* 0.������У׼ */
	err = rt_thread_init(&uav_calib_system_thread,
						 "rt_uav_calib",
						 rt_entry_tuav_calib_system,
						 RT_NULL,
						 thread_uav_calib_system_stack,
						 sizeof(thread_uav_calib_system_stack),
						 RTOS_PR_UAV_CALIB_SYSTEM,
						 20);
	if (err == RT_EOK)
    {
		rt_thread_startup(&uav_calib_system_thread);
	}
	
/*5************ ������־�̶߳�ջ����(01/01) *************/
/* 0.SD����¼��־ */
	err = rt_thread_init(&tfsd_fly_log_thread,
						 "rt_tfsd_fly_log",
						 rt_entry_thread_tfsd_fly_log,
						 RT_NULL,
						 thread_tfsd_fly_log_stack,
						 sizeof(thread_tfsd_fly_log_stack),
						 RTOS_PR_TFSD_FLY_LOG,
						 20);
	if (err == RT_EOK)
    {
		rt_thread_startup(&tfsd_fly_log_thread);
	}
	
/*6************ �˻������̶߳�ջ����(02/02) *************/
/* 0.OLED����վ��ʾ */
	err = rt_thread_init(&hci_oled_show_thread,
						 "rt_hci_oled",
						 rt_entry_thread_hci_oled_show,
						 RT_NULL,
						 thread_hci_oled_show_stack,
						 sizeof(thread_hci_oled_show_stack),
						 RTOS_PR_HCI_OLED_SHOW,
						 20);
	if (err == RT_EOK)
    {
		rt_thread_startup(&hci_oled_show_thread);
	}

/* 1.��λ������λ������ */	
	err = rt_thread_init(&hci_slave_and_host_thread,
						 "rt_hci_hs",
						 rt_entry_thread_hci_slave_and_host,
						 RT_NULL,
						 thread_hci_slave_and_host_stack,
						 sizeof(thread_hci_slave_and_host_stack),
						 RTOS_PR_HCI_SLAVE_AND_HOST,
						 20);
	if (err == RT_EOK)
    {
		rt_thread_startup(&hci_slave_and_host_thread);
	}	
}

/*NVIC��ʼ��*/
void mcu_nvic_init(void)
{
	msp_NVIC_Init(NVIC_PRIO_GROUP_2);
}

/*MCU��ʼ��*/
void mcu_driver_init(void)
{
//	nop_delay_init(); /*���ڲ��Ե�ǰMCU��while(1--){__NOP()}��ʱ*/
	
	sys_Mcu_Peripheral_Init();
}

/*Ӳ����ʼ��*/
void hardware_init(void)
{
	bsp_BoardLib_Init(&g_sBoardStatus);
}

/*ϵͳ������ȡ*/
void uav_system_init(void)
{
	/*�˲�����ʼ��*/
	filter_origin_data_filter_init(&g_sFilterTarg);	
	
	/*������У׼������ȡ*/
	calib_all_sensor_parameter_read();
	
	/*�����㷨������ʼ��*/
	ctrl_autocontrol_para_init(&g_sCtrlSysStatus);
	
	/*��ʼ��Ԫ����ʼ��*/
	ahrs_quaternion_init(&g_sAhrsQuater);
}

/*��ʼ������*/
void work_after_system_init(void)
{
	/*TIM��ʱ����ʼ��������*/
	sys_mcu_tim_init();
	
	/*MCU_EXTI��ʼ��*/
	sys_mcu_exti_init();
	
	/*ʹ��Debug Uart Rx*/
	msp_uart_recv_data(&g_sDebugUart);
	
	/*���ϵͳ��ʼ�����*/
	g_sUav_Status.UavProgrameStatus.INIT_STATUS = UAV_PROGRAME_INIT_FINISH;
}

/*RTOS �����ʼ��*/
/*********** �ź���(��ʱִ��) ***********/	
/*=== �������� ===*/
struct rt_semaphore base_module_sem;    
/*=== ���������ݻ�ȡ����̬�ں� ===*/
struct rt_semaphore euler_angle_sem;  
struct rt_semaphore ver_fusion_sem;
struct rt_semaphore gps_update_sem;
struct rt_semaphore opflow_update_sem;
struct rt_semaphore gps_hor_fusion_sem;  
struct rt_semaphore opflow_hor_fusion_sem;  
/*=== ����ϵͳ ===*/
struct rt_semaphore uav_ctrl_sem;

/*=== У׼ϵͳ ===*/
struct rt_semaphore uav_calib_sem;
/*=== ������� ===*/
struct rt_semaphore tfsd_fly_log_sem;
/*=== �˻����� ===*/
struct rt_semaphore hci_oled_show;
struct rt_semaphore hci_host_slave_sem;

/*********** �¼�(����ִ��) ***********/
/*=== ������д ===*/
struct rt_event para_rdwr_event;

/********** ������(��Դ����) ***********/
/*=== I2Cͨ�� ===*/
//struct rt_mutex i2c_mutex;
//struct rt_mutex gps_mag_i2c_mutex;
/*=== SPIͨ�� ===*/
//struct rt_mutex spi_mutex;

void rtos_unit_init(void)
{	
	/*******************************�ź���*****************************/	
	/*=== �������� ===*/
	rt_sem_init(&base_module_sem, "base_module_sem", 0, RT_IPC_FLAG_FIFO);		

	/*=== ���������ݻ�ȡ����̬�ں� ===*/  
	rt_sem_init(&euler_angle_sem, "euler_angle_sem", 0, RT_IPC_FLAG_FIFO);	  
	rt_sem_init(&ver_fusion_sem, "ver_fusion_sem", 0, RT_IPC_FLAG_FIFO);
	rt_sem_init(&gps_update_sem, "gps_update_sem", 0, RT_IPC_FLAG_FIFO);
	rt_sem_init(&opflow_update_sem, "opflow_update_sem", 0, RT_IPC_FLAG_FIFO);	
	rt_sem_init(&gps_hor_fusion_sem, "gps_hor_fusion_sem", 0, RT_IPC_FLAG_FIFO);	 
	rt_sem_init(&opflow_hor_fusion_sem, "opflow_hor_fusion_sem", 0, RT_IPC_FLAG_FIFO);	

	/*=== ����ϵͳ ===*/
	rt_sem_init(&uav_ctrl_sem, "uav_ctrl_sem", 0, RT_IPC_FLAG_FIFO);	

	/*=== У׼ϵͳ ===*/
	rt_sem_init(&uav_calib_sem, "uav_calib_sem", 0, RT_IPC_FLAG_FIFO);	

	/*=== ������� ===*/
	rt_sem_init(&tfsd_fly_log_sem, "tfsd_fly_log_sem", 0, RT_IPC_FLAG_FIFO);	

	/*=== �˻����� ===*/	
	rt_sem_init(&hci_oled_show, "hci_oled_sem", 0, RT_IPC_FLAG_FIFO);
	rt_sem_init(&hci_host_slave_sem, "hci_hs_sem", 0, RT_IPC_FLAG_FIFO);	
	
	/********************************�¼�******************************/
	/*=== ������д ===*/
	rt_event_init(&para_rdwr_event, "para_rdwr_event", RT_IPC_FLAG_FIFO);
	
	/********************************������*****************************/	
	/*=== I2Cͨ�� ===*/	
//	rt_mutex_init(&i2c_mutex, "i2c_mutex", RT_IPC_FLAG_FIFO);
//	rt_mutex_init(&gps_mag_i2c_mutex, "gmag_i2c_mutex", RT_IPC_FLAG_FIFO);	
	
	/*=== SPIͨ�� ===*/
//	rt_mutex_init(&spi_mutex, "spi_mutex", RT_IPC_FLAG_FIFO);	
}

