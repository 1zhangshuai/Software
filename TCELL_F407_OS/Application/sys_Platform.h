#ifndef _SYS_PLATFORM_H_
#define _SYS_PLATFORM_H_

#include "stm32f4xx.h"
#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

#define PLATFORM_RTOS__RT_THREAD

/*RTOS*/
#ifdef PLATFORM_RTOS__RT_THREAD
/*RT-Thread*/
#include <rtthread.h>
#define sys_DelayMs(foc)	 rt_thread_delay(foc)	/*delay min ms*/
#endif

/*ϵͳ����ʱ����*/
#define PLATFORM_TASK_SCHEDULER_MIN_FOC_MS	(5)	/*ϵͳ������Сʱ��*/

/*Processors do multiplication faster than division!(���������˷�������������)*/

/********* MCU��Ϣ(�ص�,�������ʵ������) *********/
#define ST_STM32F407VE				(1)
#define ST_STM32F407VG				(2)
#define ST_STM32F427VG				(4)

#if 1
#define MCU_TARG				    (ST_STM32F407VE)	    		  /*MCU����*/
#define MCU_PAKEGE_IO				(100.82)						  /*��װ��IO����*/
#define MCU_INTERNAL_RAM_SIZE		(192)	    					  /*KByte*/
#define MCU_INTERNAL_FLASH_SIZE		(512)	    					  /*KByte*/
#define MCU_SYSTEME_CLOCK_MHZ		(168.0f) 						  /*ϵͳʱ��Ƶ��(Mhz)*/
#define MCU_ONE_NOP_SYCLE_NS		(1000.0f / MCU_SYSTEME_CLOCK_MHZ) /*1��nop��ʱ��*/	
#endif

#if 0
#define MCU_TARG				    (ST_STM32F407VG)	    		  /*MCU����*/
#define MCU_PAKEGE_IO				(100.82)						  /*��װ��IO����*/
#define MCU_INTERNAL_RAM_SIZE		(192)	    					  /*KByte*/
#define MCU_INTERNAL_FLASH_SIZE		(1024)	    					  /*KByte*/
#define MCU_SYSTEME_CLOCK_MHZ		(168.0f) 						  /*ϵͳʱ��Ƶ��(Mhz)*/
#define MCU_ONE_NOP_SYCLE_NS		(1000.0f / MCU_SYSTEME_CLOCK_MHZ) /*1��nop��ʱ��*/	
#endif

#if 0
#define MCU_TARG				    (ST_STM32F427VG)	    		  /*MCU����*/
#define MCU_PAKEGE_IO				(100.82)						  /*��װ��IO����*/
#define MCU_INTERNAL_RAM_SIZE		(256)	    					  /*KByte*/
#define MCU_INTERNAL_FLASH_SIZE		(1024)	    					  /*KByte*/
#define MCU_SYSTEME_CLOCK_MHZ		(180.0f) 						  /*ϵͳʱ��Ƶ��(Mhz)*/
#define MCU_ONE_NOP_SYCLE_NS		(1000.0f / MCU_SYSTEME_CLOCK_MHZ) /*1��nop��ʱ��*/	
#endif
/*********************************************/

/****** �ɿذ汾��(���������λ���鿴) *******/
#define PILOT_HARDWARE_TYPE			(1)
#define PILOT_HARDWARE_VERSION		(100)
#define PILOT_SOFTWARE_VERSION		(100)
#define PILOT_PROTOCOL_VERSION		(401)
#define PILOT_BOOTLOADER_VERSION	(100)
/*********************************************/

/*Hardware FPU DSP*/
#define FPU_ENABLE_DSP //FPU_DISABLE_DSP

/*DSP*/
#ifdef FPU_ENABLE_DSP

#endif

/*No DSP*/
#ifdef FPU_DISABLE_DSP

#endif

/******************* ����ϵ�������� *******************
**************************************************************
***************************************************************
              X�Ͱ�װ��ʽ,����������̬�ǹ�ϵ
                             -
                           Pitch
                   3#                1#
                  ������            ������
                   ������          ������
                    ������        ������
                     ������      ������
                      ������    ������
       -   Roll            ������            Roll   +
                           ������
                      ������    ������
                     ������      ������
                    ������        ������
                   ������          ������
                  ������            ������
                   2#                4#
���ٶȴ���������������X��Y��Zͬ��,������ԭ�㿴����ʱ����ת�Ƕ�Ϊ+
                           Pitch
                             +
                            Y Aixs
                            *
                            *
                            *
                            *
                            *
                            *
                            * * * * * * * *   X Axis Roll+
                          (0)
***************************************************************
**************************************************************
Pitch(ŷ����,�ߵ�����,ŷ������x��) - x(�ߵ�Ԫ����x����) - y(Pitch����ת��y��) - Longitude(����) - E(east)
Roll(ŷ����,�ߵ�����,ŷ������y��)  - y(�ߵ�Ԫ����y����) - x(Roll����ת��x��)  - Latitude(��γ)  - N(north)
Yaw(ŷ���ǽ�,�ߵ�����,ŷ������z��) - z(�ߵ�Ԫ����z����) - xxxxxxxxxxxxxx      - xxxxxxxxxxxxxx  - U(up)
����(����)����ϵ������+Lat(γ��)+X������+Lon(����)+Y ����λ��ƫ��
����(����)����ϵ��������+x��+roll�����帩��+(y��)+pitch ����λ��ƫ��
******************************************************/

/**************** ң�ػ������� ***************/
/*���������*/
	/*1.ҡ���ڰ˽���*/
	/*
		*******       *******
		*     *       *     *
		*  *  *       *  *  * 
		*   * *       * *   *
		*******       *******
			  *       *   
	*/
	
	/*2.ҡ���������*/
	/*
		*******       *******
		*     *       *     *
		*  *  *       *  *  * 
		* *   *       *   * *
		*******       *******
		*                   *   
	*/

/*���ٶȼƺʹ�����У׼*/	
	/*1.���ٶȼƽ���/�˳�У׼�����ж�*/
	/*						*	
		*******       *******
		*     *       *   * *
		*  *  *       *  *  * 
		* *   *       *     *
		*******       *******
		*                     
	*/
	
	/*2.�����ƽ���/�˳�У׼�����ж�*/
	/*			      *	
		*******       *******
		*     *       * *   *
		*  *  *       *  *  * 
		*   * *       *     *
		*******       *******
		      *                    
	*/	

/*HCI_OLED��ʾ����*/
	/*1.����OLED��ʾ*/
	/*		  	      *	
		*******       *******
		*     *       * *   *
		*  *  *       *  *  * 
		*  *  *       *     *
		*******       *******		                        
		   *
	*/

	/*2.�ر�OLED��ʾ*/
	/*		  	      *	
		*******       *******
		*     *       * *   *
		*  *  *       *  *  * 
		*  *  *       *     *
		*******       *******		                        
		   *
	*/
/*********************************************/

/******************* ��� ********************/
#define SYS_NO_AVA_MARK					  (-255)	/*��Ч���*/
#define SYS_ENABLE						  (1)		/*ʹ��*/
#define SYS_DISABLE						  (0)		/*��ʹ��*/
/*********************************************/

/**************** ��Ȼ���ﳣ�� ***************/
#ifndef PI
#define PI						    (3.1415926f)		  	/*��ֵ*/
#endif

#define LOCAL_DECLINATION_YAW		(-3.61915f)				/*��ǰ�ش�ƫ��,��ѯ�����վ���*/

#define SEA_LEVEL_PRESSURE			(101325.0f)				/*��ƽ����ѹ(Pa)*/
#define EARTH_RADIUS				(6371004)				/*����뾶(m)*/
#define DEG2RAD            			(PI / 180.0f) 			/*��ת����*/
#define RAD2DEG 		   			(180.0f / PI) 			/*����ת��*/
#define GRAVITY_STD		   			(9.80665f)				/*��׼�������ٶ�m/s^2*/
/*********************************************/

/***** ��������������(�ص�,�������ʵ������) ******/
#define GPS_LOCATION_SCALING_FACTOR (0.011131884502145034f) /*1 ��/γ�� ʵ�ʾ��� 111ǧ��*/
#define MPU_ACC_RANGE	   			(4.0f / 32768.0f)		/*���ٶ�����:��4G*/
#define MPU_GYRO_RANGE	   			(1000.0f / 32768.0f) 	/*����������:(��1000��/s)*/
#define ACC_MAX_ONE_G	   			(8192.0f)			    /*����*/
#define ACC_TO_ONE_G	   			(GRAVITY_STD / ACC_MAX_ONE_G)
#define ONE_G_TO_ACC	   			(ACC_MAX_ONE_G / GRAVITY_STD)
/*********************************************/

/************* ģ��������� *************/
/*ULTR������*/
#define SYS_ULTR_MAX_MEAS_DISTANCE		 	           (200)	/*��������������������:200cm*/
#define SYS_ULTR_MIN_MEAS_PERIOD_TICK_MS 	           (100)  	/*��������С��������:100ms*/

/*BERO��ѹ��*/
#define SYS_BERO_MIN_MEAS_PERIOD_TICK_MS 	           (110)  	/*SPL06��ѹ����С��������:110ms*/

/*OPFLOW����*/
#define SYS_OPFLOW_MAX_MEAS_DISTANCE		 	       (200)	/*�������������ֱ����:200cm*/

/*���ӵ�����(���)*/
#define ESC_MIN_PULSE_ZERO_SPEED_VALUE	               (1000) /*Electronic Speed Control��С������,���ٶ�,���ͣת״̬*/
#define ESC_MAX_PULSE_MAX_SPEED_VALUE	               (2000) /*Electronic Speed Control���������,����ٶ�,���ת�����״̬*/
										               
/*���ת����������*/                                   
#define MOTOR_TURN_DRIVE_TARGET_INDEX	               (0xFF) 		/*ȫ��ת������(��������0x00��ȫ��ͣת)*/ 
										               
/*ң��FLYSKY*/                                         
#define REMOT_DATA_REAL_MIN_VALUE		               (1000)		/*ң��������ʵ��Сֵ(1000us)*/
#define REMOT_DATA_REAL_MAX_VALUE		               (2000)		/*ң��������ʵ���ֵ(2000us)*/
#define REMOT_ANGLE_MID					               (1500)		/*ң�������Ƕ�(Roll,Pitch,Yaw)�м�ֵ*/
#define REMOT_ANGLE_MID_DEADZONE	                   (100)		/*ң�������Ƕ�(Roll,Pitch,Yaw)����ֵ*/
#define REMOT_THROTTLE_DEADZONE_MIN		               (1100)		/*ң������(Throttle)��Сֵ����*/
#define REMOT_THROTTLE_BASE_VALUE		               (1000)		/*ң������(Throttle)��ֵ(��Сֵ)*/
#define REMOT_THROTTLE_MID_DEADZONE		               (200)		/*ң������(Throttle)�м�����*/
/******************************************/

/********* EEPROM�������ݵĵ�ַ **********/
/*У׼�����洢��ַ*/
#define AT24CXX_STOR_ACC_SCALE_ADDR                    (45)										  /*Acc��������У׼�����洢λ��*/
#define AT24CXX_STOR_ACC_OFFSET_ADDR                   (AT24CXX_STOR_ACC_SCALE_ADDR  + 12 + 4)	  /*Acc��ƫУ׼�����洢λ��*/
#define AT24CXX_STOR_MAG_OFFSET_ADDR                   (AT24CXX_STOR_ACC_OFFSET_ADDR + 12 + 4)	  /*����������ƫִУ׼�����洢λ��*/
#define AT24CXX_STOR_GPS_MAG_OFFSET_ADDR               (AT24CXX_STOR_MAG_OFFSET_ADDR + 12 + 4)	  /*GPS_����������ƫִУ׼�����洢λ��*/
#define AT24CXX_STOR_PID_PARA_ADDR		               (AT24CXX_STOR_GPS_MAG_OFFSET_ADDR + 12 + 4)  /*PIDϵ���洢λ��*/
/*****************************************/

/*����ֵ״̬����*/
typedef enum
{
	SYS_RET_SUCC  = 0,
	SYS_RET_FAIL  = 1,
	SYS_RET_BUSY  = 2,
}SYS_RETSTATUS;

/*����ֵ��������(�����ڵڼ���)*/
typedef enum
{
	SYS_RETERR_0ZR  = 0,
	SYS_RETERR_1ST  = 1,
	SYS_RETERR_2ND  = 2,	
	SYS_RETERR_3RD  = 3,
	SYS_RETERR_4TH  = 4,	
	SYS_RETERR_5TH  = 5,	
	SYS_RETERR_6TH  = 6,
	SYS_RETERR_7TH  = 7,
	SYS_RETERR_8TH  = 8,
	SYS_RETERR_9TH  = 9,
	SYS_RETERR_10TH = 10,
	SYS_RETERR_11TH = 11,
	SYS_RETERR_12TH = 12,
	SYS_RETERR_13TH = 13,	
	SYS_RETERR_14TH = 14,
	SYS_RETERR_15TH = 15,
	SYS_RETERR_16TH = 16,
	SYS_RETERR_17TH = 17,
	SYS_RETERR_18TH = 18,	
	SYS_RETERR_19TH = 19,
	SYS_RETERR_20TH = 20,		
	SYS_RETERR_FF   = 0xff,	/*Ĭ�ϵ�״̬*/
}SYS_RETERR;

/*����׼��״̬*/
typedef enum
{
	DATA_READY_YES = 0,
	DATA_READY_NOT = 1,
}DATA_READY_STATUS;

/*ȫ���������Ͷ���*/
typedef short 			s16;
typedef int64_t 		s64;
typedef int32_t  		s32;
typedef int16_t 		s16;
typedef int8_t  		s8;

typedef const int32_t 	sc32;  
typedef const int16_t 	sc16;  
typedef const int8_t 	sc8;  

typedef __IO int32_t  	vs32;
typedef __IO int16_t  	vs16;
typedef __IO int8_t   	vs8;

typedef __I int32_t 	vsc32;  
typedef __I int16_t 	vsc16; 
typedef __I int8_t 		vsc8;   
	
typedef uint64_t  		u64;
typedef uint32_t  		u32;
typedef uint16_t 		u16;
typedef uint8_t  		u8;

typedef const uint32_t 	uc32;  
typedef const uint16_t 	uc16;  
typedef const uint8_t 	uc8; 

typedef __IO uint32_t  	vu32;
typedef __IO uint16_t 	vu16;
typedef __IO uint8_t  	vu8;

typedef __I uint32_t 	vuc32;  
typedef __I uint16_t 	vuc16; 
typedef __I uint8_t 	vuc8; 

typedef const float		fpc32;
typedef const double	dbc64;

typedef __IO float		vfp32;
typedef __IO double		vfp64;

typedef __I float		vfpc32;
typedef __I double		vdbc64;

typedef float 			fp32;
typedef double 		    fp64;

/*������������*/
#define FLOAT_BYTE_NUM 		(4)	/*fp32��������ռ���ֽ���*/
#define S32_BYTE_NUM 		(4)	/*s32��������ռ���ֽ���*/
#define U32_BYTE_NUM 		(4)	/*u32��������ռ���ֽ���*/

#define S16_BYTE_NUM 		(2)	/*s32��������ռ���ֽ���*/
#define U16_BYTE_NUM 		(2)	/*u32��������ռ���ֽ���*/

typedef union
{
	fp32 value;
	u8   byte[FLOAT_BYTE_NUM];
}StorFloatData;

typedef union
{
	s32 value;
	u8  byte[S32_BYTE_NUM];
}StorS32Data;

typedef union
{
	u32 value;
	u8  byte[U32_BYTE_NUM];
}StorU32Data;

typedef union
{
	s16 value;
	u8  byte[S16_BYTE_NUM];
}StorS16Data;

typedef union
{
	u16 value;
	u8  byte[U16_BYTE_NUM];
}StorU16Data;

/*״̬��*/
enum
{
	BIT_FLAG_RESET = 0,	
	BIT_FLAG_SET   = 1,
};

typedef struct
{
	vu8 bit0:1;
	vu8 bit1:1;
	vu8 bit2:1;
	vu8 bit3:1;
	vu8 bit4:1;
	vu8 bit5:1;
	vu8 bit6:1;
	vu8 bit7:1;	
}U8_Bit_Flag;

typedef struct
{
	vu16 bit0:1;
	vu16 bit1:1;
	vu16 bit2:1;
	vu16 bit3:1;
	vu16 bit4:1;
	vu16 bit5:1;
	vu16 bit6:1;
	vu16 bit7:1;	
	vu16 bit8:1;
	vu16 bit9:1;
	vu16 bit10:1;
	vu16 bit11:1;
	vu16 bit12:1;
	vu16 bit13:1;
	vu16 bit14:1;
	vu16 bit15:1;	
}U16_Bit_Flag;

/*��������������(s: short / f: fp32)*/
/*IMU_Accԭʼ����*/
typedef struct
{
	s16 x;
	s16 y;
	s16 z;
}Acc3s;		

/*IMU_Acc�˲��������*/
typedef struct
{
	fp32 x;
	fp32 y;
	fp32 z;
}Acc3f;

/*IMU_Gyroԭʼ����*/
typedef struct
{
	s16 x;
	s16 y;
	s16 z;
}Gyro3s;		

/*IMU_Gyro�˲��������*/
typedef struct
{
	fp32 x;
	fp32 y;
	fp32 z;
}Gyro3f;

/*AHRS_Magԭʼ����*/
typedef struct
{
	s16 x;
	s16 y;
	s16 z;
}Mag3s;			

/*AHRS_Mag�˲��������*/
typedef struct
{
	fp32 x;
	fp32 y;
	fp32 z;
}Mag3f;

/*IMU(R&F)����*/
typedef struct
{	
	Acc3f  accRaw;		/*���ٶ�ԭʼ����*/
	Acc3f  accFilter;	/*���ٶ��˲�������*/
	Gyro3f gyroRaw;		/*���ٶ�ԭʼ����*/
	Gyro3f gyroFilter;	/*���ٶ��˲�������*/
}Imu3fRF;

/*Mag������(R&F)����*/
typedef struct
{
	Mag3f magRaw;		/*������ԭʼ����*/
	Mag3f magFilter;	/*�������˲�����*/
}Mag3fRF;

/****** �������Ҿ���(DCM) ******/
typedef struct
{
	s32 x;
	s32 y;
}Vector2i;

typedef struct
{
	fp32 x;
	fp32 y;
}Vector2f;

typedef struct
{
	s16 x;
	s16 y;
	s16 z;
}Vector3i;

typedef struct
{
	fp32 x;
	fp32 y;
	fp32 z;
}Vector3f;

typedef struct
{
	fp32 a;
	fp32 b;
	fp32 c;
	fp32 r;
}Vector4f;

typedef struct
{
	fp32 q0;
	fp32 q1;
	fp32 q2;
	fp32 q3;	
}Vector4q;

/****** ���嵼�� ******/
typedef struct
{
	fp32 east;
	fp32 north;
	fp32 up;
}Vector3f_Nav;

typedef struct
{
	s32 lon;
	s32 lat;
}Vector2s_Nav;

typedef struct
{
	fp32 east;
	fp32 north;
}Vector2f_Nav;

typedef struct
{
	fp32 pitch;
	fp32 roll;
}Vector2f_Ang;

typedef struct
{
	fp32 pitch;
	fp32 roll;
}Vector2f_Body;

typedef struct
{
	fp32 east;	
	fp32 north;
}Vector2f_Earth;

/****** PID�㷨���� ******/
typedef struct
{
	fp32 kP;
	fp32 kI;
	fp32 kD;
}Vector3f_PID;

typedef struct
{
	s16 kP;
	s16 kI;
	s16 kD;
}Vector3s_PID;

/****** ģ�⿴�Ź� ******/
typedef enum
{
	SIM_WATCH_DOG_ALIVE = 0, /*����*/
	SIM_WATCH_DOG_DIED  = 1, /*��ȥ*/
}SIM_WATCH_DOG_STATUS;

typedef struct
{	
	vu32 curTicks; 			/*��ǰticks*/
	vu32 nextProcessTicks;   /*�´��ж�ticks*/
}SimulateWatchDog;

#ifdef PLATFORM_RTOS__RT_THREAD
#include "rtos_cpuusage.h"
#endif

#include "period_Execute.h"	      /*ִ�����ڲ���: ���� my_GetTick()*/
#include "hci_show_send_config.h" /*�˻�����:OLED�ͷ��Ͳ�������*/
#include "sys_ControlMap.h"

#endif
