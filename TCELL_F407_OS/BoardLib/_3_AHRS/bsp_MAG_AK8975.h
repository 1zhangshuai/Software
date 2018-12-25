#ifndef _BSP_MAG_AK8975_H_
#define _BSP_MAG_AK8975_H_

#include "bsp_MAG_AK8975_CMD.h"
#include "sys_Platform.h"
#include "sys_McuInit.h"
#include "ssp_SimI2C.h"
#include "msp_I2C.h"

/*Sensitivity Adjustment values*/
typedef struct
{
	u8 x;
	u8 y;
	u8 z;
}Ak8975_ASA;

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
typedef struct
{
	SSP_SimI2C *SimI2cMaster;
	Mag3s      Mag;
	Ak8975_ASA Asa;
	fp32       TestMagYaw;
}BSP_AK8975;
#endif

#if defined(STD_PROTOCOL_HARDWARE_I2C)
typedef struct
{
	MSP_I2c    *I2cMaster;
	Mag3s      Mag;
	Ak8975_ASA Asa;
	fp32       TestMagYaw;
}BSP_AK8975;
#endif

/*��ʼ��*/
SYS_RETSTATUS bsp_AK8975_Init(BSP_AK8975 *ak8975);

/*��ȡ�ӻ���ַʶ����*/
u8 bsp_AK8975_Get_Id(BSP_AK8975 *ak8975);

/*��ȡ�����ȵ���ֵ*/
SYS_RETSTATUS bsp_AK8975_Get_AsaValue(BSP_AK8975 *ak8975);

/*��ȡ3���������*/
Mag3s *bsp_AK8975_Get_Mag_Data(BSP_AK8975 *ak8975);

/*�����ݽ��������ȵ���*/
Mag3s *bsp_AK8975_Asa_Dp(BSP_AK8975 *ak8975);

/*����:��ȡ�����Ƶ�Yaw(ˮƽλ�ò���ƫ����)*/
fp32 bsp_AK8975_Get_Mag_Yaw(BSP_AK8975 *ak8975);

extern BSP_AK8975 g_sAk8975;

#endif 
