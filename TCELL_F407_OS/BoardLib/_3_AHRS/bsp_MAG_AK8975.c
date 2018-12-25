#include "bsp_MAG_AK8975.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

BSP_AK8975 g_sAk8975;

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
/**
  * @brief  Write a byte to the specified device address through SIM_I2C bus.
  * @param  SimI2CMaster:
  * @param  DevAddr: The address byte of the slave device
  * @param  RegAddr: The address byte of  register of the slave device
  * @param  Data: the data would be writen to the specified device address
  * @return  None
  */
static SYS_RETSTATUS Ak8975_SimI2c_WriteOneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t Data)
{
#ifdef PLATFORM_RTOS__RT_THREAD
	/*I2C��������ȡ*/
//	rt_mutex_take(&i2c_mutex, 5);
#endif		
	
	FunctionalState state;
	
	ssp_SimI2C_Start(g_sAk8975.SimI2cMaster);
	
	state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, (DevAddr << 1) | 0);
	
	if (state == ENABLE)
	{
		state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, RegAddr);
		
		if (state == ENABLE)
		{
			state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, Data);
			
			if (state == ENABLE)
			{
				ssp_SimI2C_Stop(g_sAk8975.SimI2cMaster);
				return SYS_RET_SUCC;
			}
		}
	}
	
	ssp_SimI2C_Stop(g_sAk8975.SimI2cMaster);
	
#ifdef PLATFORM_RTOS__RT_THREAD
	/*I2C�������ͷ�*/
//	rt_mutex_release(&i2c_mutex);
#endif
	
	return SYS_RET_FAIL;
}

/**
  * @brief  Read an byte from the specified device address through SIM_I2C bus.
  * @param  SimI2CMaster:
  * @param  DevAddr: The address byte of the slave device
  * @param  RegAddr: The address byte of  register of the slave device
  * @return  the byte read from SIM_I2C bus
  */
static SYS_RETSTATUS Ak8975_SimI2c_ReadSomeByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t len, uint8_t *RdBuff)
{
#ifdef PLATFORM_RTOS__RT_THREAD
	/*I2C��������ȡ*/
//	rt_mutex_take(&i2c_mutex, 5);
#endif	
	
	FunctionalState state;
	
	ssp_SimI2C_Start(g_sAk8975.SimI2cMaster);
	
	state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, (DevAddr << 1) | 0);
	
	if (state == ENABLE)
	{
		state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, RegAddr);
		
		if (state == ENABLE)
		{
			ssp_SimI2C_Start(g_sAk8975.SimI2cMaster);	
			
			state = ssp_SimI2C_WriteByte(g_sAk8975.SimI2cMaster, (DevAddr << 1) | 1);
			
			if (state == ENABLE)
			{
				while(len)
				{
					if (len == 1)
					{
						*RdBuff = ssp_SimI2C_ReadByte(g_sAk8975.SimI2cMaster, RdBuff, DISABLE);
					}
					else
					{
						*RdBuff = ssp_SimI2C_ReadByte(g_sAk8975.SimI2cMaster, RdBuff, ENABLE);
					}
					
					len--;
					RdBuff++;
				}
				
				ssp_SimI2C_Stop(g_sAk8975.SimI2cMaster);
				return SYS_RET_SUCC;
			}
		}
	}

	ssp_SimI2C_Stop(g_sAk8975.SimI2cMaster);
	
#ifdef PLATFORM_RTOS__RT_THREAD
	/*I2C�������ͷ�*/
//	rt_mutex_release(&i2c_mutex);
#endif
	
	return SYS_RET_FAIL;	
}
#endif

#if defined(STD_PROTOCOL_HARDWARE_I2C)
/**
  * @brief  Write a byte to the specified device address through SIM_I2C bus.
  * @param  SimI2CMaster:
  * @param  DevAddr: The address byte of the slave device
  * @param  RegAddr: The address byte of  register of the slave device
  * @param  Data: the data would be writen to the specified device address
  * @return  None
  */
static SYS_RETSTATUS Ak8975_I2c_WriteOneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t Data)
{
	SYS_RETSTATUS ret = SYS_RET_FAIL;
	
	u8 wrData = Data;
	
	ret = I2C_Write_SomeByte(g_sAk8975.I2cMaster, RegAddr, &wrData, 1);
	
	return ret;
}

/**
  * @brief  Read an byte from the specified device address through SIM_I2C bus.
  * @param  SimI2CMaster:
  * @param  DevAddr: The address byte of the slave device
  * @param  RegAddr: The address byte of  register of the slave device
  * @return  the byte read from SIM_I2C bus
  */
static SYS_RETSTATUS Ak8975_I2c_ReadSomeByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t len, uint8_t *RdBuff)
{
	SYS_RETSTATUS ret = SYS_RET_FAIL;
	
	ret = I2C_Read_SomeByte(g_sAk8975.I2cMaster, RegAddr, RdBuff, len);
	
	return ret;	
}
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
#define AK8975_I2C_WRITE_ONEBYTE	 Ak8975_SimI2c_WriteOneByte
#define AK8975_I2C_READ_SOMEBYTE	 Ak8975_SimI2c_ReadSomeByte
#endif

#if defined(STD_PROTOCOL_HARDWARE_I2C)
#define AK8975_I2C_WRITE_ONEBYTE	 Ak8975_I2c_WriteOneByte
#define AK8975_I2C_READ_SOMEBYTE 	 Ak8975_I2c_ReadSomeByte
#endif

/*��ʼ��*/
#if defined(HW_CUT__USE_MD_MAG)

SYS_RETSTATUS bsp_AK8975_Init(BSP_AK8975 *ak8975)
{
	SYS_RETSTATUS statusRet = SYS_RET_SUCC;
	
	/*ak8975 i2c Init*/
#ifdef HW_CUT__USE_MD_MAG
#ifdef HW_CUT__USE_GPS_MAG
	#if defined(STD_PROTOCOL_HARDWARE_I2C)
	ak8975->I2cMaster            = &g_sGpsMagI2C;		/*����GPS�ӻ�I2C��*/
	ak8975->I2cMaster->slaveAddr = (AK8975_SLAVEADDR << 1);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	ak8975->SimI2cMaster         = &g_sGpsMagSimI2C;
	#endif
#else
	#if defined(STD_PROTOCOL_HARDWARE_I2C)	
	ak8975->I2cMaster            = &g_sMagI2C;			/*���ڰ���I2C��*/
	ak8975->I2cMaster->slaveAddr = (AK8975_SLAVEADDR << 1);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	ak8975->SimI2cMaster         = &g_sMagSimI2C;
	#endif	
#endif
#endif	

	sys_DelayMs(1);
	
	/*ak8975 check*/
	if (bsp_AK8975_Get_Id(ak8975) != AK8975_WIA_VALUE)
	{
		statusRet = SYS_RET_FAIL;
	}
	
	/*read Fuse ROM(Sensitivity Adjustment values)*/
	if (bsp_AK8975_Get_AsaValue(ak8975) != SYS_RET_SUCC)
	{
		statusRet = SYS_RET_FAIL;
	}
	
	/*��ʼ��ʱ����һ�β��������´ζ�ȡ,��Ϊ��оƬ�����Զ�����*/
	AK8975_I2C_WRITE_ONEBYTE(AK8975_SLAVEADDR, AK8975_CNTL, AK8975_SINGLE_MEAS_MODE);
	
	return statusRet;
}
#endif

/*��ȡ�ӻ���ַʶ����*/
u8 bsp_AK8975_Get_Id(BSP_AK8975 *ak8975)
{
	u8 slaveId;
	
	AK8975_I2C_READ_SOMEBYTE(AK8975_SLAVEADDR, AK8975_WIA, 1, &slaveId);
	
	return slaveId;
}

/*��ȡ�����ȵ���ֵ*/
SYS_RETSTATUS bsp_AK8975_Get_AsaValue(BSP_AK8975 *ak8975)
{
	SYS_RETSTATUS statusRet = SYS_RET_FAIL;
	u8 asaBuff[3] = {0};
	
	/*Fuse ROM access mode*/
	statusRet = AK8975_I2C_WRITE_ONEBYTE( AK8975_SLAVEADDR, AK8975_CNTL, AK8975_FUSE_ROM_ACCESS_MODE);
	
	if (statusRet == SYS_RET_SUCC)
	{
		statusRet = AK8975_I2C_READ_SOMEBYTE(AK8975_SLAVEADDR, AK8975_ASAX, 3, asaBuff);	

		if (statusRet == SYS_RET_SUCC)
		{
			/*�ж�ASAֵ�Ƿ���Ч*/
			if (((asaBuff[0] != 0xff) && (asaBuff[0] != 0x00)) && \
				((asaBuff[1] != 0xff) && (asaBuff[1] != 0x00)) && \
				((asaBuff[2] != 0xff) && (asaBuff[2] != 0x00)))
			{
				ak8975->Asa.x = asaBuff[0];
				ak8975->Asa.y = asaBuff[1];
				ak8975->Asa.z = asaBuff[2];		
		
				statusRet = SYS_RET_SUCC;
			}		
		}
	}
	
	return statusRet;
}

/*��ȡ3���������*/
Mag3s *bsp_AK8975_Get_Mag_Data(BSP_AK8975 *ak8975)
{
	u8 magBuff[6] = {0};
	
	if (AK8975_I2C_READ_SOMEBYTE(AK8975_SLAVEADDR, AK8975_HXL, 6, magBuff) == SYS_RET_SUCC)
	{
		ak8975->Mag.x = ((s16)(magBuff[1] << 8) | magBuff[0]);
		ak8975->Mag.y = ((s16)(magBuff[3] << 8) | magBuff[2]);
		ak8975->Mag.z = ((s16)(magBuff[5] << 8) | magBuff[4]);		
	}
	
	/*����һ�ε��β���,�����´ζ�ȡ,��Ϊ��оƬ�����Զ�����*/
	AK8975_I2C_WRITE_ONEBYTE( AK8975_SLAVEADDR, AK8975_CNTL, AK8975_SINGLE_MEAS_MODE);
	
	/*�����ݽ��������ȵ���(�ٷ��ֲ�)*/
	bsp_AK8975_Asa_Dp(ak8975);
	
	/*����ϵ����,Ϊ�˺�IMU����ϵһ��*/
	ak8975->Mag.x =  ak8975->Mag.x;
	ak8975->Mag.y = -ak8975->Mag.y;
	ak8975->Mag.z = -ak8975->Mag.z;
	
	return &(ak8975->Mag);
}

/*�����ݽ��������ȵ���*/
Mag3s *bsp_AK8975_Asa_Dp(BSP_AK8975 *ak8975)
{
	/*�����ֲ������ʽ��������*/
	ak8975->Mag.x = ak8975->Mag.x * ((((ak8975->Asa.x - 128) * 0.5) / 128) + 1);
	ak8975->Mag.y = ak8975->Mag.y * ((((ak8975->Asa.y - 128) * 0.5) / 128) + 1);
	ak8975->Mag.z = ak8975->Mag.z * ((((ak8975->Asa.z - 128) * 0.5) / 128) + 1);	
	
	return &(ak8975->Mag);
}

/*����:��ȡ�����Ƶ�Yaw(ˮƽλ�ò���ƫ����)*/
fp32 bsp_AK8975_Get_Mag_Yaw(BSP_AK8975 *ak8975)
{
	fp32 curentYaw;
	
	/*��ȡ������3���������*/
	bsp_AK8975_Get_Mag_Data(ak8975);
	
	/*����ˮƽYaw��*/
	curentYaw = (atan2(ak8975->Mag.y, ak8975->Mag.x) * (180 / PI) + 180);  

	ak8975->TestMagYaw = curentYaw;	
	
	return curentYaw;	
}
