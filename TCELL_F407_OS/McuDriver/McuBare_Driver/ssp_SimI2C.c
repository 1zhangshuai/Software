#include "ssp_SimI2C.h"

void ssp_SimI2C_Init(SSP_SimI2C *SimI2C)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*SIM I2C SCL GPIO Init*/
	GPIO_InitStruct.GPIO_Pin   = SimI2C->SCL_Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(SimI2C->SCL_GPIO, &GPIO_InitStruct);
	
	/*SIM I2C SDA GPIO Init*/	
	GPIO_InitStruct.GPIO_Pin   = SimI2C->SDA_Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(SimI2C->SDA_GPIO, &GPIO_InitStruct);	
	
	/*I2C GPIO Default*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);
}

void ssp_SimI2C_IsBusy(SSP_SimI2C *SimI2C)
{
	/*��ȡSDA���ϵĵ�ƽ״̬,��Ϊ�͵�ƽ��˵�����߱��ӻ����ƣ���Ϊ�ߵ�ƽ��˵�����߿��У�����׼�����Ϳ�ʼ����*/
	while(SYS_GPIO_READ(SimI2C->SDA_GPIO, SimI2C->SDA_Pin) == Bit_RESET)
	{
		SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SYS_GPIO_RESET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
		SIM_I2C_DELAY(3);
		
		SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
		SIM_I2C_DELAY(3);
	}
}

/*������ʼ�ź�*/
void ssp_SimI2C_Start(SSP_SimI2C *SimI2C)
{
	/*�ж��������Ƿ��ڿ���״̬*/
	ssp_SimI2C_IsBusy(SimI2C);
	
	/*����SCL�еĵ�ƽΪ�ͣ���ֹ��ΪSCL���ڸߵ�ƽ��ʹ���潫SDA����ʱ�����ܻᴥ��һ��stop�ź�*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_RESET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);
	
	/*��SCL���ͣ�ǯסSCL�ߣ�׼�����͵�ַ����*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);	
}

/*���������ź�*/
void ssp_SimI2C_Stop(SSP_SimI2C *SimI2C)
{
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_RESET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);	
	
	SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);	
}

/*����Ӧ���źŻ��߷�Ӧ���ź�,ackState->ΪENABLEʱ�������Ӧ���ź�*/
void ssp_SimI2C_SetAck(SSP_SimI2C *SimI2C, FunctionalState ackState)
{
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	if (ackState == ENABLE)
	{
		SYS_GPIO_RESET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
		SIM_I2C_DELAY(1);
	}
	else if (ackState == DISABLE)
	{
		SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
		SIM_I2C_DELAY(1);	
	}
	
	SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(2);	
	
	/*����SCL�ߣ�ǯסSCL��׼����һ������*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);
}

/*���Ӧ���źţ�ENABLE�����߷�Ӧ���źţ�DISABLE��*/
FunctionalState ssp_SimI2C_GetAck(SSP_SimI2C *SimI2C)
{
	FunctionalState ask;
	
	SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);

	/*��ȡSDA���ϵĵ�ƽ״̬*/
	if (SYS_GPIO_READ(SimI2C->SDA_GPIO, SimI2C->SDA_Pin) == Bit_RESET)
	{
		ask = ENABLE;
	}
	else
	{
		ask = DISABLE;
	}
	
	/*����ȡ����Ӧ��Ϣ����ǯסSCL��׼����һ������*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);
	
	return ask;
}

/*д�����ݸ��ӻ���������Ӧ����߷�Ӧ���ź�*/
FunctionalState ssp_SimI2C_WriteByte(SSP_SimI2C *SimI2C, uint8_t data)
{
	uint8_t i;
	
	/*������λ�Ĵ����Ĺ��ܣ�������ͨ��I/O�ڷ��ͳ�ȥ*/
	for (i = 0; i < 8; i++)
	{
		SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SIM_I2C_DELAY(1);
		
		if ((data & 0x80) == 0x80)
		{
			SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);	
		}
		else if ((data & 0x80) == 0)
		{
			SYS_GPIO_RESET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
		}
		
		data = data << 1;
		SIM_I2C_DELAY(1);	
		
		SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SIM_I2C_DELAY(2);		
	}
	
	/*�����ͷ�SDA�ߣ�ʹ�����߿��У��Ա�ӻ��ܷ�����Ӧ��Ϣ,��ǯסSCL��*/
	SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
	SIM_I2C_DELAY(1);	
	
	SYS_GPIO_SET(SimI2C->SDA_GPIO, SimI2C->SDA_Pin);
	SIM_I2C_DELAY(1);

	return ssp_SimI2C_GetAck(SimI2C);
}

/*��ȡ�ӻ����͵�����,��������Ӧ���Ƿ�Ӧ��*/
uint8_t ssp_SimI2C_ReadByte(SSP_SimI2C *SimI2C, uint8_t *data, FunctionalState ackState)
{
	uint8_t i;
	
	*data = 0x00;
	
	/*������λ�Ĵ����Ĺ��ܣ������ݴ�I/O���ж�ȡ����*/
	for (i = 0; i < 8; i++)
	{
		SYS_GPIO_SET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SIM_I2C_DELAY(1);

		/*����λ����ֵ*/
		(*data) = (*data) << 1;
		
		if (SYS_GPIO_READ(SimI2C->SDA_GPIO, SimI2C->SDA_Pin) == 1)
		{
			(*data) = (*data) | 0x01;
		}
		else if (SYS_GPIO_READ(SimI2C->SDA_GPIO, SimI2C->SDA_Pin) == 0)
		{
			(*data) = (*data) | 0x00;
		}
		
		/*����SCL,�Ա�ӻ�׼������һ������*/
		SYS_GPIO_RESET(SimI2C->SCL_GPIO, SimI2C->SCL_Pin);
		SIM_I2C_DELAY(2);
	}
	
	ssp_SimI2C_SetAck(SimI2C, ackState);
	
	return *data;
}
