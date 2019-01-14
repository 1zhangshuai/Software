#ifndef _SYS_BOARDMAP_H_
#define _SYS_BOARDMAP_H_

#include "sys_Platform.h"
#include "bsp_BoardLib.h"

#define BSP_TOTAL_MODULE_TYPE_NUMBER		(8) /*��������������*/

/*ͨ��Э��ѡ��*/
/*I2C*/
#undef  STD_PROTOCOL_HARDWARE_I2C	/*Ӳ��I2C*/
#define STD_PROTOCOL_SOFTWARE_I2C	/*���(ģ��)I2C*/

/*SPI*/
#undef  STD_PROTOCOL_HARDWARE_SPI	/*Ӳ��SPI*/
#define STD_PROTOCOL_SOFTWARE_SPI	/*���(ģ��)SPI*/

/*SPI�� ���滻*/
#define STD_PROTOCOL_SPI_PORT_REPLACE		(SYS_DISABLE)

/*===================== Ӳ���ü� =====================*/
/*=== 0.����ģ�� ===*/
#if 0
	/*LED*/
	#define HW_CUT__USE_LED
	#include "bsp_BASE_LED.h"
#endif

#if 0 
	/*KEY*/
	#define HW_CUT__USE_KEY
	#include "bsp_BASE_KEY.h"
#endif

#if 0
	/*RGB*/
	#define HW_CUT__USE_RGB
	#include "bsp_BASE_RGB.h"
#endif

/*=== 1.���ݴ洢 ===*/
#if 0
	#define HW_CUT__USE_FLASH_STOR
#endif

#if 1
	#define HW_CUT__USE_EEPROM_STOR
#endif

#if 0
	#define HW_CUT__USE_TFSD_STOR
#endif

/*=== 2.IMU + ������ ===*/
#if 1
	#define HW_CUT__USE_MD_IMU
#endif

#if 1
	#define HW_CUT__USE_MD_MAG
#endif

#if 0
	#define HW_CUT__USE_BD_IMU
#endif

#if 1
	#define HW_CUT__USE_GPS_MAG
#endif

/*=== 3.��ѹ�� ===*/
#if 1
	#define HW_CUT__USE_MD_BERO
#endif

#if 0
	#define HW_CUT__USE_DB_BERO /*˫��ѹ��*/
#endif

/*=== 4.������ ===*/
#if 1
	#define HW_CUT__USE_ULTR
#endif

/*=== 5.GPS ===*/
#if 1
	#define HW_CUT__USE_GPS
#endif

/*=== 6.���� ===*/
#if 1
	/*UPIXELS*/
	#define HW_CUT__USE_OPTICFLOW
#endif

/*=== 7.�˻����� ===*/
#if 1
	#define HW_CUT__USE_HCI_OLED
#endif



/*===================== ����ѡ�� =====================*/
/*=== 0.���ݴ洢 ===*/
/*MCU FLASH*/
#if defined(HW_CUT__USE_FLASH_STOR)
	#if 1
		/*FLASH*/
		#define STOR_MCU__FLASH	
	#endif
#endif

/*EEPROM*/
#if defined(HW_CUT__USE_EEPROM_STOR)
	#if 1
		/*EEPROM_AT24CXX*/	
		#define STOR_BOARD__AT24CXX
		#include "bsp_EEPROM_AT24CXX.h"
	#endif
#endif

/*SD CARD*/
#if defined(HW_CUT__USE_TFSD_STOR)
	#if 1
		/*SD CARD*/	
		#define STOR_BOARD__TFSD
	#endif
#endif

/*=== 1.IMU + ������ ===*/
#if defined(HW_CUT__USE_MD_IMU)	/*ģ�� IMU*/
	#if 1
		/*MPU6050*/
		#define MD_IMU__MPU6050
		#include "bsp_IMU_MPU6050.h"		
	#endif
	
	#if 0
		/*MPU6000*/
		#define MD_IMU__MPU6000
		#include "bsp_IMU_MPU6000.h"
	#endif	
#endif

#if defined(HW_CUT__USE_MD_MAG)	/*ģ�� MAG*/
	#if 1
		/*AK8975*/
		#define MD_MAG__AK8975
		#include "bsp_MAG_AK8975.h"
	#endif
	
	#if 0
		/*HMC5883L*/
		#define MD_MAG__HMC5883L
		#include "bsp_MAG_HMC5883L.h"
	#endif
	
	#if 0
		/*IST8310*/
		#define MD_MAG__IST8310
		#include "bsp_MAG_IST8310.h"		
	#endif	
#endif	

#if defined(HW_CUT__USE_BD_IMU)	/*���� IMU*/
	#if 0
		/*MPU6050*/
		#define BD_IMU__MPU6050
		#include "bsp_IMU_MPU6050.h"
	#endif
	
	#if 0
		/*MPU6000*/
		#define BD_IMU__MPU6000
		#include "bsp_IMU_MPU6000.h"		
	#endif	
#endif

#if defined(HW_CUT__USE_GPS_MAG)	/*GPS MAG*/
	#if 0
		/*AK8975*/
		#define GPS_MAG__AK8975
		#include "bsp_MAG_AK8975.h"		
	#endif
	
	#if 0
		/*HMC5883L*/
		#define GPS_MAG__HMC5883L
		#include "bsp_MAG_HMC5883L.h"
	#endif
	
	#if 0
		/*IST8310*/
		#define GPS_MAG__IST8310
		#include "bsp_MAG_IST8310.h"		
	#endif
	
	#if 1
		/*HMC5983*/
		#define GPS_MAG__HMC5983
		#include "bsp_MAG_HMC5983.h"
	#endif
#endif	

/*=== 2.��ѹ�� ===*/
#if defined(HW_CUT__USE_MD_BERO)
	#if 1
		/*SPL06-001*/	
		#define MD_BERO__SPL06
		#include "bsp_BERO_SPL06.h"		
	#endif
	
	#if 0
		/*BMP280*/	
		#define MD_BERO__BMP280
		#include "bsp_BERO_BMP280.h"
	#endif
	
	#if 0
		/*MS5611*/	
		#define MD_BERO__MS5611
		#include "bsp_BERO_MS5611.h"		
	#endif
#endif

#if defined(HW_CUT__USE_DB_BERO) /*˫��ѹ��*/
	#if 1
		/*SPL06-001*/	
		#define MD_BERO__SPL06
		#include "bsp_BERO_SPL06.h"		
	#endif
	
	#if 0
		/*BMP280*/	
		#define MD_BERO__BMP280
		#include "bsp_BERO_BMP280.h"
	#endif
	
	#if 1
		/*MS5611*/	
		#define MD_BERO__MS5611
		#include "bsp_BERO_MS5611.h"		
	#endif
#endif


/*=== 3.������ ===*/
#if defined(HW_CUT__USE_ULTR)
	#if 1
		/*US100*/	
		#define ULTR_BD__US100
		#include "bsp_ULTR_US100.h"		
	#endif
	
	#if 0
		/*RCWL1603*/	
		#define ULTR_BD__RCWL1603
		#include "bsp_ULTR_RCWL1603.h"			
	#endif
	
	#if 0
		/*HCSR04*/	
		#define ULTR_BD__HCSR04
		#include "bsp_ULTR_HCSR04.h"			
	#endif
#endif

/*=== 4.GPS ===*/
#if defined(HW_CUT__USE_GPS)
	#if 1
		/*MD M8N*/
		#define GPS_MD__M8N
		#include "bsp_GPS_M8N.h"		
	#endif
	
	#if 0
		/*BD M8N*/
		#define GPS_BD__M8N
		#include "bsp_GPS_M8N.h"		
	#endif
#endif

/*=== 5.���� ===*/
#if defined(HW_CUT__USE_OPTICFLOW)
	#if 1
		/*UPIXELS*/	
		#define OPTICALFLOW_MD__UPIXELSLC306
		#include "bsp_OPFLOW_UpixelsLC306.h"
	#endif

	#if 0
		/*UPIXELS*/	
		#define OPTICALFLOW_BD__UPIXELSLC306
		#include "bsp_OPFLOW_UpixelsLC306.h"
	#endif
#endif

/*=== 6.�˻����� ===*/
#if defined(HW_CUT__USE_HCI_OLED)
	#if 1
		/*0.96*/
		#define HCI_MD__OLED0_96
		#include "bsp_HCI_OLED0_96.h"
	#endif
#endif

#endif
