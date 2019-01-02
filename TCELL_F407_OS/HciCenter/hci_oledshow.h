#ifndef _HCI_AIRCRAFT_H_
#define _HCI_AIRCRAFT_H_

#include "sys_Platform.h"
#include "bsp_BoardLib.h"
#include "bsp_HCI_OLED0_96.h"
#include "status_Aircraft.h"
#include "calib_SensorData.h"
#include "pid_system.h"

#define HCI_PAGE_CHANGE_MIN_HOLD_TIME_MS	(1500) /*ҳ���л�������С����ʱ��(ms)*/

/*�����ֿ�*/
extern const u8 g_Ascii8X16[];
extern const u8 g_Ascii6X12[];
extern const u8 g_Ascii6X8[];
extern const u8 g_Hanzi16X16[][16];
extern const u8 g_Hanzi12X12[];
extern const u8 g_LogoFlashWolves128X64[];
extern const u8 g_ProgressBar96X24[];
extern const u8 g_ProgressBarFillBlock11X8[];
extern const u8 test64X64[];
extern const u8 g_EnableShowHint128X64[];

/*ACC & MAGУ׼��ʾ*/
/*���ٶ�У׼��ʾ:��һ��*/
extern const u8 g_AccCalib_1st_Side_48X128[];
/*���ٶ�У׼��ʾ:�ڶ���*/
extern const u8 g_AccCalib_2nd_Side_48X128[];
/*���ٶ�У׼��ʾ:������*/
extern const u8 g_AccCalib_3rd_Side_48X128[];
/*���ٶ�У׼��ʾ:������*/
extern const u8 g_AccCalib_4th_Side_48X128[];
/*���ٶ�У׼��ʾ:������*/
extern const u8 g_AccCalib_5th_Side_48X128[];
/*���ٶ�У׼��ʾ:������*/
extern const u8 g_AccCalib_6th_Side_48X128[];

/*������У׼��ʾ:��һ��*/
extern const u8 g_MagCalib_1st_Side_48X128[];
/*������У׼��ʾ:�ڶ���*/
extern const u8 g_MagCalib_2nd_Side_48X128[];
/*������У׼��ʾ:������*/
extern const u8 g_MagCalib_3rd_Side_48X128[];

/*��ʼ������*/
typedef enum
{
	HCI_SHOW_INIT_HCI_MODE    = 0, /*_0_�˻�����ģ��*/
	HCI_SHOW_INIT_BASE_MODE   = 1, /*_1_����ģ��*/
	HCI_SHOW_INIT_STOR_MODE   = 2, /*_2_���ݴ洢ģ��*/
	HCI_SHOW_INIT_AHRS_MODE   = 3, /*_3_AHRSģ��*/
	HCI_SHOW_INIT_BERO_MODE   = 4, /*_4_��ѹ��ģ��*/
	HCI_SHOW_INIT_ULTR_MODE   = 5, /*_5_������ģ��*/
	HCI_SHOW_INIT_GPS_MODE    = 6, /*_6_GPSģ��*/
	HCI_SHOW_INIT_OPFLOW_MODE = 7, /*_7_����ģ��*/
}HCI_SHOW_INIT_TARG;

/*�ܹ�ҳ��*/
#define HCI_TOTAL_SHOW_PAGE_NUMBER			(11)
/*��ʾҳ�����*/
typedef enum
{
	HCI_SHOW_PAGE_0  = 0,
	HCI_SHOW_PAGE_1  = 1,
	HCI_SHOW_PAGE_2  = 2,
	HCI_SHOW_PAGE_3  = 3,
	HCI_SHOW_PAGE_4  = 4,
	HCI_SHOW_PAGE_5  = 5,
	HCI_SHOW_PAGE_6  = 6,
	HCI_SHOW_PAGE_7  = 7,
	HCI_SHOW_PAGE_8  = 8,
	HCI_SHOW_PAGE_9  = 9,
	HCI_SHOW_PAGE_10 = 10,	
}HCI_SHOW_PAGE_INDEX;

/*ҳ���(�̶�λ��)��ʾ״̬*/
typedef enum
{
	HCI_SHOW_MOULD_FIRST    = 0, /*��һ����ʾ*/
	HCI_SHOW_MOULD_NOTFIRST = 1, /*�ǵ�һ����ʾ*/
}HCI_SHOW_MOULD_STATUS;

/*�˳���ʾ����״̬*/
typedef enum
{
	HCI_EXIT_SHOW_OP_FIRST    = 0, /*��һ���˳�*/
	HCI_EXIT_SHOW_OP_NOTFIRST = 1, /*�ǵ�һ���˳�*/
}HCI_EXIT_SHOW_OP_STATUS;

/*��ʾ����״̬*/
typedef enum
{
	HCI_SHOW_TASK_IDLE = 0, /*����(������ʾ����ҳ)*/
	HCI_SHOW_TASK_BUSY = 1, /*æµ(��������ʾ����ҳ)*/
}HCI_SHOW_TASK_STATUS;

/*========= ��ʼ���˻����� ========= */
/*��ʾ����������logo*/
void hci_Show_AircraftLogoHoldMs(u32 holdMs);

/*��ʾ��ʼ������*/
void hci_Show_InitRateOfProgress(u8 totalModuleNbr, HCI_SHOW_INIT_TARG INIT_TARG, SYS_RETERR INIT_STATUS);

/*��ʾ������У׼���������*/
void hci_Show_Sensor_Calib_Parameter(RESULT_CALIB_STATUS ACC_CALIB_STATUS, RESULT_CALIB_STATUS MAG_CALIB_STATUS, u32 holdMs);

/*��ʾ����ϵͳ��ʼ������(PID����)*/
void hci_Show_Control_System_Parameter(SYS_RETSTATUS READ_STATUS, fp32 kP, fp32 kI, fp32 kD, PID_CONTROLER_LINK LINK_TARG, u32 holdMs);

/*ʹ��OLED��ʾ��ʾ*/
void hci_Show_Enable_Hint(void);


/*=========  �������й�������ʾ =========*/
void hci_show_on_run_progress(void);

/*==== 1.ʵʱ������ʾ ====*/
/*NO0.AHRS������ʾ*/
void hci_Show_Cur_Ahrs_Data(void);

/*NO1.�߶ȴ�����(������+��ѹ��)������ʾ*/
void hci_Show_High_Sensor_Data(void);

/*NO2.GPS������ʾ*/
void hci_Show_Gps_Data(void);

/*NO3.����������ʾ*/
void hci_Show_Opticflow_Data(void);

/*==== 2.�ߵ��ں�������ʾ ====*/
/*NO4.��ֱ�߶ȹߵ��ں���ʾ*/
void hci_Show_Height_SINS_Data(void);

/*NO5.ˮƽ��PITCH����ߵ��ں���ʾ*/
void hci_Show_HorizontalPitch_SINS_Data(void);

/*NO6.ˮƽ��ROLL����ߵ��ں���ʾ*/
void hci_Show_HorizontalRoll_SINS_Data(void);

/*==== 3.��������ǰ״̬��ʾ ====*/
/*NO7.����ִ��������ʾ*/
void hci_Show_Execute_Period(void);

/*NO8.����������״̬*/
void hci_Show_Aircraft_Status(void);

/*NO9.ң��״̬��ʾ*/
void hci_Show_Remot_Status(void);

/*NO10.������У׼���*/
void hci_Show_Sensor_Calib_Result(void);


/*==== ������У׼���� ====*/
/*1.����/�˳����ٶȼ�У׼��ʾ*/
void hci_Show_Acc_Calib_Status(ENTRY_CALIB_STATUS ENTRY_STATUS, CALIB_SIDE_INDEX GONNA_CALIB_SIDE);

/*2.����/�˳�������У׼��ʾ*/
void hci_Show_Mag_Calib_Status(ENTRY_CALIB_STATUS ENTRY_STATUS, CALIB_SIDE_INDEX GONNA_CALIB_SIDE);

/*3.���ٶȼ�У׼������ʾ*/
void hci_Show_Acc_Calib_Process(CALIB_SIDE_INDEX CUR_SIDE_INDEX, SAMPLE_PROCESS_TYPE PROCESS_TYPE, SINGLE_SAMPLE_STATUS SIGSAMPLE_STATUS, fp32 gyroLenth, fp32 xg, fp32 yg, fp32 zg, u32 holdMs);

/*4.������У׼������ʾ*/
void hci_Show_Mag_Calib_Process(CALIB_SIDE_INDEX CUR_SIDE_INDEX, CALIB_POSITION_INDEX POSITION_INDEX, POSITION_SAMPLE_STATUS POS_SAMP_STATUS, SAMPLE_PROCESS_TYPE PROCESS_TYPE, u16 curYaw, u16 targetYaw, u8 errorMaxYaw, s16 targAccAxis, u32 holdMs);


typedef struct
{
	volatile HCI_SHOW_PAGE_INDEX 	  curPageIndex;         /*��ǰ��ʾҳ��*/
	volatile HCI_SHOW_PAGE_INDEX 	  lastPageIndex;        /*�ϴ���ʾҳ��*/
	volatile HCI_SHOW_MOULD_STATUS    MOULD_STATUS;         /*ÿ��ҳ���ģ�����ʾ״̬*/
	volatile UAV_HCI_SHOW_STATUS      PAGE_STATUS;          /*��ҳ����ʾ״̬*/
	volatile UAV_HCI_SHOW_STATUS      SHOW_DATA_STATUS;	    /*��ʾ����״̬*/
	volatile UAV_HCI_SHOW_STATUS 	  SHOW_HINT_STATUS;	    /*��ʾ��ʾ״̬*/
	volatile HCI_EXIT_SHOW_OP_STATUS  EXIT_SHOW_STATUS;     /*�˳���ʾ״̬*/
	volatile HCI_SHOW_TASK_STATUS     SHOW_TASK_STATUS;		/*��ʾ����״̬*/
}HciShowPage;

/*=== ң�����HCI ===*/
/*HCI(OLED):������ʾ/��ֹ��ʾ/��ʾ��һҳ/��ʾ��һҳ/������ǰҳ/��������ǰҳ*/
void hci_remot_switch_show_status(HciShowPage *hciShowPage);

extern HciShowPage g_sHciShowPage;

#endif
