#ifndef _SYS_OSTASK_H_
#define _SYS_OSTASK_H_

#include "msp_NVIC.h"
#include "sys_Platform.h"
#include "bsp_BoardLib.h"

#include "ahrs_Caculation.h"
#include "attitude_Aircraft.h"

#include "sins_Strapdown.h"
#include "remot_DataAnaly.h"
#include "calib_SensorData.h"
#include "control_Aircraft.h"
#include "hci_oledshow.h"

/*�߳����ȼ�����*/
enum
{	
	/*��������*/
	RTOS_PR_BASE_MODULE				   = 12,
	
	/*������дϵͳ*/
	RTOS_PR_STOR_PARA_RDWR 			   = 16,
	
	/*���������ݻ�ȡ����̬�ں�*/
	RTOS_PR_EULER_ANGLE_CALCULATE	   = 6,	/*IMU, MAG, Euler*/
	RTOS_PR_DATA_VERTICAL_FUSION	   = 7, /*BERO, ULTR, Vertical*/
	RTOS_PR_GPS_CTRL_DATA_GET		   = 4,	/*GPS �������ݻ�ȡ*/
	RTOS_PR_OPFLOW_CTRL_DATA_GET	   = 5, /*OPFLOW �������ݻ�ȡ*/
	RTOS_PR_GPS_HORIZONTAL_FUSION	   = 8, /*GPS, Horizontal*/
	RTOS_PR_OPFLOW_HORIZONTAL_FUSION   = 9, /*OPTICFLOW, Horizontal*/
	
	/*����ϵͳ*/
	RTOS_PR_UAV_CTRL_SYSTEM 		   = 3, /*remot, pid, output motor*/
	
	/*ϵͳУ׼*/
	RTOS_PR_UAV_CALIB_SYSTEM		   = 13, /*ACC, MAG calib*/
	
	/*�������*/
	RTOS_PR_TFSD_FLY_LOG			   = 14, /*fly log write and read_send*/
	
	/*�˻�����*/
	RTOS_PR_HCI_OLED_SHOW              = 21, /*hci: oled show*/
	RTOS_PR_HCI_SLAVE_AND_HOST         = 22, /*hci: host salve exchange*/	
};

/*���ݸ����¼�*/
enum
{
	/*== ϵͳ(У׼)����д�� ===*/
	RTOS_EVENT_PID_PARA_UPDATE_BY_HOST = 1,	/*PID��������λ���´�ı�ָ��(����)*/

	/*== У׼ ===*/
};

/*RTOS �̻߳��Ѽ���������*/
/*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS*/
/*��������*/
#define RTOS_WAKE_UP_BASE_MODULE_FOC_MS		  (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
/*���������ݻ�ȡ����̬�ں�*/	
#define RTOS_WAKE_UP_EULER_ANGLE_FOC_MS		  (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
#define RTOS_WAKE_UP_VER_FUSION_FOC_MS	 	  (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
#define RTOS_WAKE_UP_GPS_HOR_FUSION_FOC_MS    (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
#define RTOS_WAKE_UP_OPFLOW_HOR_FUSION_FOC_MS (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
/*����ϵͳ*/
#define RTOS_WAKE_UP_UAV_CTRL_FOC_MS	      (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
/*У׼ϵͳ*/
#define RTOS_WAKE_UP_UAV_CALIB_FOC_MS		  (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
/*�������*/
#define RTOS_WAKE_UP_TFSD_FLY_LOG_FOC_MS	  (20 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
/*�˻�����*/
#define RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS	  (30 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)
#define RTOS_WAKE_UP_HCI_SLAVE_HOST_FOC_MS	  (01 * PLATFORM_TASK_SCHEDULER_MIN_FOC_MS)

typedef vu16 rt_wakeup_t;
typedef struct
{
	/*��������*/
	rt_wakeup_t base_module;	
	
	/*���������ݻ�ȡ����̬�ں�*/	
	rt_wakeup_t euler_angle;
	rt_wakeup_t ver_fusion;
	rt_wakeup_t gps_hor_fusion;
	rt_wakeup_t opflow_hor_fusion;
	
	/*����ϵͳ*/
	rt_wakeup_t uav_ctrl;	
	
	/*У׼ϵͳ*/
	rt_wakeup_t uav_calib;
	
	/*�������*/
	rt_wakeup_t tfsd_fly_log;
	
	/*�˻�����*/
	rt_wakeup_t hci_oled_show;
	rt_wakeup_t hci_slave_host;	
}rtos_thread_wake_up;

/*NVIC��ʼ��*/
void mcu_nvic_init(void);

/*MCU��ʼ��*/
void mcu_driver_init(void);

/*Ӳ����ʼ��*/
void hardware_init(void);

/*�ɿ�ϵͳ��ʼ��*/
void uav_system_init(void);

/*MCU ��ʼ������*/
void work_after_system_init(void);

/*RTOS �����ʼ��*/
void rtos_unit_init(void);

/*�����߳�*/
void rtos_thread_create(void);

extern rtos_thread_wake_up gs_rtos_thread_wake_up;

#if defined(PLATFORM_RTOS__RT_THREAD)
/*********** �ź���(��ʱִ��) ***********/	
/*=== �������� ===*/
extern struct rt_semaphore base_module_sem;    
/*=== ���������ݻ�ȡ����̬�ں� ===*/
extern struct rt_semaphore euler_angle_sem;
extern struct rt_semaphore ver_fusion_sem;
extern struct rt_semaphore gps_update_sem;
extern struct rt_semaphore opflow_update_sem;
extern struct rt_semaphore gps_hor_fusion_sem;  
extern struct rt_semaphore opflow_hor_fusion_sem; 
/*=== ����ϵͳ ===*/
extern struct rt_semaphore uav_ctrl_sem;

/*=== У׼ϵͳ ===*/
extern struct rt_semaphore uav_calib_sem;
/*=== ������� ===*/
extern struct rt_semaphore tfsd_fly_log_sem;
/*=== �˻����� ===*/
extern struct rt_semaphore hci_oled_show;
extern struct rt_semaphore hci_host_slave_sem;

/*********** �¼�(����ִ��) ***********/
/*=== ������д ===*/
extern struct rt_event para_rdwr_event;

/********** ������(��Դ����) ***********/
/*=== I2Cͨ�� ===*/
extern struct rt_mutex i2c_mutex;
extern struct rt_mutex gps_mag_i2c_mutex;
/*=== SPIͨ�� ===*/
extern struct rt_mutex spi_mutex;
#endif

#endif
