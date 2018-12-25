#ifndef _BSP_IMU_MPU6050_H_
#define _BSP_IMU_MPU6050_H_

#include "bsp_IMU_MPU6050_CMD.h"
#include "sys_Platform.h"
#include "sys_McuInit.h"
#include "ssp_SimI2C.h"
#include "msp_I2C.h"

#define MPU6050_INT_STATUS		DISABLE //ENABLE

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
typedef struct
{
	SSP_SimI2C *SimI2cMaster;
	Gyro3s     Gyro;
	Gyro3s     GyroZero;
	Acc3s 	   Acc;
}BSP_MPU6050;
#endif

#if defined(STD_PROTOCOL_HARDWARE_I2C)
typedef struct
{
	MSP_I2c *I2cMaster;
	Gyro3s  Gyro;
	Gyro3f  GyroZero;
	Acc3s 	Acc;
}BSP_MPU6050;
#endif

/*��ʼ��*/
SYS_RETSTATUS bsp_MPU6050_Init(BSP_MPU6050* mpu6050);

/*��ȡ�ӻ���ַ*/
u8 bsp_MPU6050_GetId(BSP_MPU6050* mpu6050); 	

/*��ȡ�¶�ֵ*/
fp32 bsp_MPU6050_GetTemp(BSP_MPU6050* mpu6050); 	

/*��ȡ���ٶ�*/
Acc3s* bsp_MPU6050_GetAcc(BSP_MPU6050* mpu6050); 		

/*��ȡ���ٶ�*/
Gyro3s* bsp_MPU6050_GetGyro(BSP_MPU6050* mpu6050); 									

extern BSP_MPU6050 g_sMpu6050;

#endif
