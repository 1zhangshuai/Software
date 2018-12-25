#include "bsp_OPFLOW_UpixelsLC306.h"

OpFlowUpixelsLC306 g_sOpFlowUpixelsLC306 = 
{
	.RESPONSE_STATUS = OPFLOW_UPIXELSLC306_RESPONSE_FAIL,
	.INIT_STATUS     = OPFLOW_UPIXELSLC306_INIT_NULL,	
};

/*OpFlowUpixelsLC306 ����Э��֡*/
OpFlowUpixelsLC306TxFrame g_sOpFlowUpixelsLC306TxFrame = {0};

/*OpFlowUpixelsLC306 ����Э��֡*/
OpFlowUpixelsLC306RxFrame g_sOpFlowUpixelsLC306RxFrame = {0};

/*���ڷ���Buff*/
u8 g_OpFlowUpixelsLC306TxBuff[OPFLOW_UPIXELSLC306_TX_BUFF_LENTH] = {0};

/*���ڽ���Buff*/
u8 g_OpFlowUpixelsLC306RxBuff[OPFLOW_UPIXELSLC306_RX_BUFF_LENTH] = {0};

/*��������buff*/
u8 g_OpFlowUpixelsLC306DataBuff[OPFLOW_UPIXELSLC306_DATA_BUFF_LENTH] = {0};

/*��������*/
OpFlowUpixelsLC306DataReform g_sOpFlowUpixelsLC306DataReform = 
{
	.backByteNbr  = 0,
	.frontByteNbr = 0,
};

OpFlowUpixelsLC306DataReform *g_psOpFlowUpixelsLC306DataReform = &g_sOpFlowUpixelsLC306DataReform;

/*��ʼ��Ӧ����*/
OpFlowUpixelsLC306ResponseCmpReform g_sOpFlowUpixelsLC306ResponseCmpReform = 
{
	.pStart    = g_OpFlowUpixelsLC306RxBuff,
	.pCmpData1 = g_OpFlowUpixelsLC306RxBuff,
	.pCmpData2 = g_OpFlowUpixelsLC306RxBuff + 1,	
	.pCmpData3 = g_OpFlowUpixelsLC306RxBuff + 2,		
};

OpFlowUpixelsLC306ResponseCmpReform *g_psOpFlowUpixelsLC306ResponseCmpReform = &g_sOpFlowUpixelsLC306ResponseCmpReform;

/*OPFLOW_UPIXELSLC306 MODULE_CFG ���ý���ɹ���־*/
const u8 OPFLOW_UPIXELSLC306_MODULE_CFG_SUCC[3] = 
{
	0xAB, 0x00, 0xAB,
};

/*OPFLOW_UPIXELSLC306 MODULE_CFG ���ý���ɹ���־*/
const u8 OPFLOW_UPIXELSLC306_SENSOR_CFG_SUCC[3] = 
{
	0xBB, 0x00, 0xBB,
};

/*0xAB(ģ���ڲ���������ָ��)*/
const u8 OPFLOW_UpixelsLC306_AB_Tab_Focus_Buff[4] = 
{
	0x96, 0x26, 0xbc, 0x50
};

/*0xAB(ģ���ڲ���������ָ��)*/
const u8 OPFLOW_UpixelsLC306_BB_Sensor_Cfg_Buff[161][2] = 
{
	/*��ַ, ����*/
	0x12, 0x80,
	0x11, 0x30,
	0x1b, 0x06,
	0x6b, 0x43,
	0x12, 0x20,
	0x3a, 0x00,
	0x15, 0x02,
	0x62, 0x81,
	0x08, 0xa0,
	0x06, 0x68,
	0x2b, 0x20,
	0x92, 0x25,
	0x27, 0x97,
	0x17, 0x01,
	0x18, 0x79,
	0x19, 0x00,
	0x1a, 0xa0,
	0x03, 0x00,
	0x13, 0x00,
	0x01, 0x13,
	0x02, 0x20,
	0x87, 0x16,
	0x8c, 0x01,
	0x8d, 0xcc,
	0x13, 0x07,
	0x33, 0x10,
	0x34, 0x1d,
	0x35, 0x46,
	0x36, 0x40,
	0x37, 0xa4,
	0x38, 0x7c,
	0x65, 0x46,
	0x66, 0x46,
	0x6e, 0x20,
	0x9b, 0xa4,
	0x9c, 0x7c,
	0xbc, 0x0c,
	0xbd, 0xa4,
	0xbe, 0x7c,
	0x20, 0x09,
	0x09, 0x03,
	0x72, 0x2f,
	0x73, 0x2f,
	0x74, 0xa7,
	0x75, 0x12,
	0x79, 0x8d,
	0x7a, 0x00,
	0x7e, 0xfa,
	0x70, 0x0f,
	0x7c, 0x84,
	0x7d, 0xba,
	0x5b, 0xc2,
	0x76, 0x90,
	0x7b, 0x55,
	0x71, 0x46,
	0x77, 0xdd,
	0x13, 0x0f,
	0x8a, 0x10,
	0x8b, 0x20,
	0x8e, 0x21,
	0x8f, 0x40,
	0x94, 0x41,
	0x95, 0x7e,
	0x96, 0x7f,
	0x97, 0xf3,
	0x13, 0x07,
	0x24, 0x58,
	0x97, 0x48,
	0x25, 0x08,
	0x94, 0xb5,
	0x95, 0xc0,
	0x80, 0xf4,
	0x81, 0xe0,
	0x82, 0x1b,
	0x83, 0x37,
	0x84, 0x39,
	0x85, 0x58,
	0x86, 0xff,
	0x89, 0x15,
	0x8a, 0xb8,
	0x8b, 0x99,
	0x39, 0x98,
	0x3f, 0x98,
	0x90, 0xa0,
	0x91, 0xe0,
	0x40, 0x20,
	0x41, 0x28,
	0x42, 0x26,
	0x43, 0x25,
	0x44, 0x1f,
	0x45, 0x1a,
	0x46, 0x16,
	0x47, 0x12,
	0x48, 0x0f,
	0x49, 0x0d,
	0x4b, 0x0b,
	0x4c, 0x0a,
	0x4e, 0x08,
	0x4f, 0x06,
	0x50, 0x06,
	0x5a, 0x56,
	0x51, 0x1b,
	0x52, 0x04,
	0x53, 0x4a,
	0x54, 0x26,
	0x57, 0x75,
	0x58, 0x2b,
	0x5a, 0xd6,
	0x51, 0x28,
	0x52, 0x1e,
	0x53, 0x9e,
	0x54, 0x70,
	0x57, 0x50,
	0x58, 0x07,
	0x5c, 0x28,
	0xb0, 0xe0,
	0xb1, 0xc0,
	0xb2, 0xb0,
	0xb3, 0x4f,
	0xb4, 0x63,
	0xb4, 0xe3,
	0xb1, 0xf0,
	0xb2, 0xa0,
	0x55, 0x00,
	0x56, 0x40,
	0x96, 0x50,
	0x9a, 0x30,
	0x6a, 0x81,
	0x23, 0x33,
	0xa0, 0xd0,
	0xa1, 0x31,
	0xa6, 0x04,
	0xa2, 0x0f,
	0xa3, 0x2b,
	0xa4, 0x0f,
	0xa5, 0x2b,
	0xa7, 0x9a,
	0xa8, 0x1c,
	0xa9, 0x11,
	0xaa, 0x16,
	0xab, 0x16,
	0xac, 0x3c,
	0xad, 0xf0,
	0xae, 0x57,
	0xc6, 0xaa,
	0xd2, 0x78,
	0xd0, 0xb4,
	0xd1, 0x00,
	0xc8, 0x10,
	0xc9, 0x12,
	0xd3, 0x09,
	0xd4, 0x2a,
	0xee, 0x4c,
	0x7e, 0xfa,
	0x74, 0xa7,
	0x78, 0x4e,
	0x60, 0xe7,
	0x61, 0xc8,
	0x6d, 0x70,
	0x1e, 0x39,
	0x98, 0x1a,
};

/*Upixels LC306������ʼ��*/
#if defined(HW_CUT__USE_OPTICFLOW)

SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Init(OpFlowUpixelsLC306 *opFlowUpixelsLC306)
{
	SYS_RETSTATUS statusRet = SYS_RET_SUCC;	
	u8 i, txLenth = 0;
	vu8 reSendTimes = 0;
	
	/*���ڳ�ʼ��*/
	opFlowUpixelsLC306->UartMaster = &g_sOpticalFlowUart;
	
	sys_DelayMs(1);
	
	/*1.��ʼ����:0xAA*/
	/*����ָ��*/
	txLenth = bsp_OPFLOW_UpixelsLC306_Make_Command(&g_sOpFlowUpixelsLC306TxFrame, OPFLOW_UPIXELSLC306_STATR_CFG, NULL, 0, g_OpFlowUpixelsLC306TxBuff);
	
	/*����ָ��*/
	msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);	
	
	/*��ǳ�ʼ����ʼ*/
	opFlowUpixelsLC306->INIT_STATUS = OPFLOW_UPIXELSLC306_INIT_START;
	
	/*��ʱ�ȴ�*/
	sys_DelayMs(OPFLOW_UPIXELSLC306_INIT_CMC_PERIOD_TICKS_MS);

	/*2.ģ���ڲ���������ָ��:0xAB*/
	/*����ָ��*/
	txLenth = bsp_OPFLOW_UpixelsLC306_Make_Command(&g_sOpFlowUpixelsLC306TxFrame, OPFLOW_UPIXELSLC306_MODULE_CFG, (u8 *)&OPFLOW_UpixelsLC306_AB_Tab_Focus_Buff, 4, g_OpFlowUpixelsLC306TxBuff);
	
	/*����ָ��*/
	msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);

/*�ж�ָ���Ƿ���Ч(�Ƿ�Ӧ��),��Ӧ���ظ�����,��෢��5��*/
	while((opFlowUpixelsLC306->RESPONSE_STATUS != OPFLOW_UPIXELSLC306_RESPONSE_SUCC) && (reSendTimes < OPFLOW_UPIXELSLC306_RESEND_CMD_MAX_TIMES))
	{
		/*����ָ��*/
		msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);	
		
		reSendTimes++;
		
		sys_DelayMs(OPFLOW_UPIXELSLC306_INIT_CMC_PERIOD_TICKS_MS);		
	}
	
	/*�ط��ʹ���������������ط�����,��ʼ��ʧ��*/
	if (reSendTimes >= OPFLOW_UPIXELSLC306_RESEND_CMD_MAX_TIMES)
	{
		return SYS_RET_FAIL;	/*���Ϊ��ʼ��ʧ��*/
	}
	else
	{
		reSendTimes = 0;		
	}
	
	/*Ӧ���Ǹ�λ*/
	opFlowUpixelsLC306->RESPONSE_STATUS = OPFLOW_UPIXELSLC306_RESPONSE_FAIL;
	
	/*3.��������������ָ��:0xBB*/	
	for (i = 0; i < 161; i++)
	{
		/*����ָ��*/
		txLenth = bsp_OPFLOW_UpixelsLC306_Make_Command(&g_sOpFlowUpixelsLC306TxFrame, OPFLOW_UPIXELSLC306_SENSOR_CFG, (u8 *)&OPFLOW_UpixelsLC306_BB_Sensor_Cfg_Buff[i], 2, g_OpFlowUpixelsLC306TxBuff);
	
		/*����ָ��*/
		msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);
	
		/*�ж�ָ���Ƿ���Ч(�Ƿ�Ӧ��)*/
		while((opFlowUpixelsLC306->RESPONSE_STATUS != OPFLOW_UPIXELSLC306_RESPONSE_SUCC) && (reSendTimes <= OPFLOW_UPIXELSLC306_RESEND_CMD_MAX_TIMES))
		{
			/*����ָ��*/
			msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);	
			
			reSendTimes++;
			
			sys_DelayMs(OPFLOW_UPIXELSLC306_INIT_CMC_PERIOD_TICKS_MS);		
		}
		
		/*�ط��ʹ���������������ط�����,��ʼ��ʧ��*/
		if (reSendTimes >= OPFLOW_UPIXELSLC306_RESEND_CMD_MAX_TIMES)
		{
			return SYS_RET_FAIL;	/*���Ϊ��ʼ��ʧ��*/
		}
		else
		{
			reSendTimes = 0;		
		}
	}

	/*4.�������:0xDD*/
	/*����ָ��*/
	txLenth = bsp_OPFLOW_UpixelsLC306_Make_Command(&g_sOpFlowUpixelsLC306TxFrame, OPFLOW_UPIXELSLC306_FINISH_CFG, NULL, 0, g_OpFlowUpixelsLC306TxBuff);
	
	/*����ָ��*/
	msp_Uart_Send_Data(opFlowUpixelsLC306->UartMaster, g_OpFlowUpixelsLC306TxBuff, txLenth, MSP_UART_POLL);	

	/*��ǳ�ʼ������*/
	opFlowUpixelsLC306->INIT_STATUS = OPFLOW_UPIXELSLC306_INIT_FINISH;	
	
	return statusRet;		
}
#endif

/*Upixels LC306Ӧ�����ݽ���*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Response_Data_Parse(u8 *rspBuff)
{
	SYS_RETSTATUS RET_STATUS;
	
	if ((rspBuff[0] == 0xAB) || (rspBuff[0] == 0xBB))
	{
		if (rspBuff[1] == 0x00)
		{
			if ((rspBuff[2] == 0xAB) || (rspBuff[2] == 0xBB))
			{
				RET_STATUS = SYS_RET_SUCC;
			}
			else
			{
				RET_STATUS = SYS_RET_FAIL;
			}
		}
		else
		{
			RET_STATUS = SYS_RET_FAIL;			
		}
	}
	else
	{
		RET_STATUS = SYS_RET_FAIL;	
	}
	
	return RET_STATUS;
}

/*Upixels LC306λ�����ݽ���*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_POS_Data_Parse(OpFlowUpixelsLC306 *opFlowUpixelsLC306, OpFlowUpixelsLC306RxFrame *rxFrame, u8 *rxBuff)
{
	u8 i, check = 0;

	/*0.�����ݽ������½�ȡƴװ��һ֡����*/
	if (bsp_OPFLOW_UpixelsLC306_Reform_Data(&g_sOpFlowUpixelsLC306DataReform, rxBuff) != SYS_RET_SUCC)
	{
		/*���OPTIC FLOW���ݸ���ʧ��*/
		opFlowUpixelsLC306->UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_FAIL;			
		
		return SYS_RET_FAIL;	/*δ�ҵ�1֡����*/
	}
	
	/*��ƴ�ս�ȡ�����������*/
	memcpy(g_OpFlowUpixelsLC306DataBuff, g_psOpFlowUpixelsLC306DataReform->pHead, g_psOpFlowUpixelsLC306DataReform->backByteNbr);

	/*��ƴ�ս�ȡ����ǰ������*/	
	memcpy((g_OpFlowUpixelsLC306DataBuff + g_psOpFlowUpixelsLC306DataReform->backByteNbr), g_psOpFlowUpixelsLC306DataReform->pStitch, g_psOpFlowUpixelsLC306DataReform->frontByteNbr);
	
	/*�����������ݸ�ֵ����������Э��֡*/
	memcpy((u8 *)&g_sOpFlowUpixelsLC306RxFrame, g_OpFlowUpixelsLC306DataBuff, OPFLOW_UPIXELSLC306_DATA_BUFF_LENTH);	
	
	/*OpFlowUpixelsLC306DataBuff ��0*/
	memset(g_OpFlowUpixelsLC306DataBuff, 0, OPFLOW_UPIXELSLC306_DATA_BUFF_LENTH);
	
	/*1.�Ա�֡������Ч�Խ����ж�*/
	/*��֡ͷ��֡β��Ч�Խ��м��*/
	if ((rxFrame->head != 0xFE) || \
		(rxFrame->tail != 0x55))
	{
		/*���OPTIC FLOW���ݸ���ʧ��*/
		opFlowUpixelsLC306->UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_FAIL;			
		
		return SYS_RET_FAIL;
	}

	/*�Թ̶�������Ч�Խ��м��*/
	if (rxFrame->lenth != 0x0A)
	{
		/*���OPTIC FLOW���ݸ���ʧ��*/
		opFlowUpixelsLC306->UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_FAIL;			
		
		return SYS_RET_FAIL;
	}		
	
	/*������У����Ч�Խ��м��*/
	/*У��λ����(payload���)*/
	check = rxFrame->payload[0];
	
	for (i = 1; i < OPFLOW_UPIXELSLC306_RX_PAYLOAD_LENTH; i++)
	{
		check ^= *(((u8 *)&g_sOpFlowUpixelsLC306RxFrame) + 2 + i);
	}
	
	if (rxFrame->XOR != check)
	{
		/*���OPTIC FLOW���ݸ���ʧ��*/
		opFlowUpixelsLC306->UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_FAIL;			
		
		return SYS_RET_FAIL;		
	}
	
	/*2.����Ϊ��Ч����,����ƴװ,��ע�ⰲװ�����������*/
	/*X ���ص��ۼ�ʱ���ڵ��ۼ�λ��,(radians*10000)[���� 10000 ���Ը߶�(mm)��Ϊʵ��λ��(mm)]*/
	opFlowUpixelsLC306->OpFlowData.xIntegral = (rxFrame->payload[1] << 8) + rxFrame->payload[0];

	/*Y ���ص��ۼ�ʱ���ڵ��ۼ�λ��,(radians*10000)[���� 10000 ���Ը߶�(mm)��Ϊʵ��λ��(mm)]*/
	opFlowUpixelsLC306->OpFlowData.yIntegral = (rxFrame->payload[3] << 8) + rxFrame->payload[2];	
	
	/*��һ�η��͹������ݵ����η��͹������ݵ��ۼ�ʱ��(s)*/
	opFlowUpixelsLC306->OpFlowData.integrationTimespan = (rxFrame->payload[5] << 8) + rxFrame->payload[4];

	/*Ԥ����ؾ���,Ĭ��Ϊ 999(0x03E7)*/
	opFlowUpixelsLC306->OpFlowData.groundDistance = (rxFrame->payload[7] << 8) + rxFrame->payload[6];
	
	 /*�������ݿ���״̬*/
	opFlowUpixelsLC306->OpFlowData.DATA_STATUS = (OPFLOW_UPIXELSLC306_DATA_STATUS)rxFrame->payload[8];
	
	/*�����汾��*/
	opFlowUpixelsLC306->OpFlowData.version = rxFrame->payload[9];
	
	/*��ǹ�������Ϊ����״̬*/
	opFlowUpixelsLC306->UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_SUCC;
	
	return SYS_RET_SUCC;	
}

/*����ָ��*/
u8 bsp_OPFLOW_UpixelsLC306_Make_Command(OpFlowUpixelsLC306TxFrame *txFrame, OPFLOW_UPIXELSLC306_CFG_CMD CMD_TYPE, u8 *payload, u8 cmdLen, u8 *sendBuff)
{
	u8 i, txLenth = 0;	/*����buff����*/
	
	/*�ж�����ָ������*/
	switch(CMD_TYPE)
	{
		case OPFLOW_UPIXELSLC306_STATR_CFG:	 /*��������ָ��*/
		{
			/*��ֵtxFrame->command*/				
			txFrame->command = OPFLOW_UPIXELSLC306_STATR_CFG;	/*ָ��*/
			
			/*���㷢���ֽ�����*/	
			txLenth = 1 + cmdLen + 0;
			
			/*��ֵtxbuff*/
			memcpy(g_OpFlowUpixelsLC306TxBuff, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.command, 1);									
		}break;	

		case OPFLOW_UPIXELSLC306_MODULE_CFG: /*ģ���ڲ���������ָ��*/
		{
			/*��ֵtxFrame->command*/			
			txFrame->command = OPFLOW_UPIXELSLC306_MODULE_CFG;	/*ָ��*/
			
			/*��ֵtxFrame->payload*/
			for (i = 0; i < cmdLen; i++)
			{
				txFrame->payload[i] = payload[i];
			}	
		
			/*����У��λ*/			
			txFrame->XOR = (txFrame->payload[0] ^ txFrame->payload[1] ^ txFrame->payload[2] ^ txFrame->payload[3]);
			
			/*���㷢���ֽ�����*/	
			txLenth = 1 + cmdLen + 1;

			/*��ֵtxbuff*/
			memcpy(g_OpFlowUpixelsLC306TxBuff, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.command, 1);
			memcpy(g_OpFlowUpixelsLC306TxBuff + 1, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.payload, 4);	
			memcpy(g_OpFlowUpixelsLC306TxBuff + 5, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.XOR, 1);			
			
		}break;

		case OPFLOW_UPIXELSLC306_SENSOR_CFG: /*��������������ָ��*/
		{
			/*��ֵtxFrame->command*/			
			txFrame->command = OPFLOW_UPIXELSLC306_SENSOR_CFG;
			
			/*��ֵtxFrame->payload*/
			txFrame->payload[0] = OPFLOW_UPIXELSLC306_SENSOR_IIC_ADDR;	/*��������ַ*/
			
			for (i = 0 + 1; i < cmdLen + 1; i++)
			{
				txFrame->payload[i] = payload[i - 1];
			}		
		
			/*����У��λ*/			
			txFrame->XOR = (txFrame->payload[0] ^ txFrame->payload[1] ^ txFrame->payload[2]);
			
			/*���㷢���ֽ�����*/	
			txLenth = 1 + (1 + cmdLen) + 1;

			/*��ֵtxbuff*/
			memcpy(g_OpFlowUpixelsLC306TxBuff, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.command, 1);
			memcpy(g_OpFlowUpixelsLC306TxBuff + 1, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.payload, 3);	
			memcpy(g_OpFlowUpixelsLC306TxBuff + 4, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.XOR, 1);	
		}break;

		case OPFLOW_UPIXELSLC306_FINISH_CFG: /*�ر�����ָ��*/
		{
			/*��ֵtxFrame->command*/				
			txFrame->command = OPFLOW_UPIXELSLC306_FINISH_CFG;	/*ָ��*/
			
			/*���㷢���ֽ�����*/	
			txLenth = 1 + cmdLen + 0;
			
			/*��ֵtxbuff*/
			memcpy(g_OpFlowUpixelsLC306TxBuff, (u8 *)&g_sOpFlowUpixelsLC306TxFrame.command, 1);				
		}break;

		default:break;
	}
	
	return txLenth;
}

/*����ƫ������*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Reform_Data(OpFlowUpixelsLC306DataReform *DataReform, u8 *rxBuff)
{
	/*���������ڴ��е���ʼ��ַ*/
	DataReform->pStart = g_OpFlowUpixelsLC306RxBuff;
	
	/*ƴ�ӵ㼴��ʼ��*/
	DataReform->pStitch = DataReform->pStart;
	
	/*head��fixLenthĬ����ʼ��*/
	DataReform->pHead    = DataReform->pStart;
	DataReform->fixLenth = DataReform->pHead + 1;
	
	/*��������*/
	DataReform->frontByteNbr = 0;
	DataReform->backByteNbr  = 0;
	
	/*ɨ��ֱ����ȡ��Чλ(һ��֡ͷ,һ���̶�����ֵ)*/
	while((*(DataReform->pHead) != 0xFE) || (*(DataReform->fixLenth) != 0x0A))
	{	
		/*��ȡ��ǰ���byte++*/
		DataReform->frontByteNbr++;
		
		/*��Խ��ʱ��δ�ҵ�,������֡��û���ҵ�,��ֹͣ*/
		if (DataReform->frontByteNbr > OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - 2)
		{
			DataReform->pHead        = NULL;
			DataReform->fixLenth     = NULL;
			DataReform->frontByteNbr = 0;	
		    DataReform->backByteNbr  = 0;
			
			return SYS_RET_FAIL;
		}		
		
		/*headָ��ƫ��++*/
		DataReform->pHead++;

		/*fixLenthָ��ƫ��++*/
		DataReform->fixLenth++;
	}
	
	/*�����ȡ����滹�ж����ֽ�����*/
	DataReform->backByteNbr = OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - DataReform->frontByteNbr;
	
	return SYS_RET_SUCC;	
}

/*�ӻ�Ӧ���ѯ*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Slave_Response(OpFlowUpixelsLC306ResponseCmpReform *ResponseCmp, u8 cmpBuff[])
{	
	SYS_RETSTATUS CMP_RET_STATUS = SYS_RET_SUCC;
	
	/*���ʣ3���ռ�*/
	if ((ResponseCmp->pCmpData1 - g_OpFlowUpixelsLC306RxBuff) == OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - 3)
	{
		if ((*(ResponseCmp->pCmpData1) == cmpBuff[0]) && \
			(*(ResponseCmp->pCmpData2) == cmpBuff[1]) && \
			(*(ResponseCmp->pCmpData3) == cmpBuff[2]))
		{	
			CMP_RET_STATUS = SYS_RET_FAIL;
		}
		
		ResponseCmp->pCmpData1 = ResponseCmp->pStart + 0;
		ResponseCmp->pCmpData2 = ResponseCmp->pStart + 1;
		ResponseCmp->pCmpData3 = ResponseCmp->pStart + 2;
		
		return CMP_RET_STATUS;
	}
	/*���ʣ2���ռ�*/	
	else if ((ResponseCmp->pCmpData1 - g_OpFlowUpixelsLC306RxBuff) == OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - 2)
	{
		if ((*(ResponseCmp->pCmpData1) == cmpBuff[0]) && \
			(*(ResponseCmp->pCmpData2) == cmpBuff[1]) && \
			(*(ResponseCmp->pCmpData3) == cmpBuff[2]))
		{	
			CMP_RET_STATUS = SYS_RET_FAIL;
		}
		
		ResponseCmp->pCmpData1 = ResponseCmp->pStart + 1;
		ResponseCmp->pCmpData2 = ResponseCmp->pStart + 2;
		ResponseCmp->pCmpData3 = ResponseCmp->pStart + 3;

		return CMP_RET_STATUS;		
	}
	/*���ʣ1���ռ�*/
	else if ((ResponseCmp->pCmpData1 - g_OpFlowUpixelsLC306RxBuff) == OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - 1)
	{
		if ((*(ResponseCmp->pCmpData1) == cmpBuff[0]) && \
			(*(ResponseCmp->pCmpData2) == cmpBuff[1]) && \
			(*(ResponseCmp->pCmpData3) == cmpBuff[2]))
		{
			CMP_RET_STATUS = SYS_RET_FAIL;
		}

		ResponseCmp->pCmpData1 = ResponseCmp->pStart + 2;
		ResponseCmp->pCmpData2 = ResponseCmp->pStart + 3;
		ResponseCmp->pCmpData3 = ResponseCmp->pStart + 4;		
		
		return CMP_RET_STATUS;
	}
	/*λ�ڿ�ͷ*/
	else if ((ResponseCmp->pCmpData1 - g_OpFlowUpixelsLC306RxBuff) == 0)
	{
		if ((*(ResponseCmp->pCmpData1) == cmpBuff[0]) && \
			(*(ResponseCmp->pCmpData2) == cmpBuff[1]) && \
			(*(ResponseCmp->pCmpData3) == cmpBuff[2]))
		{	
			CMP_RET_STATUS = SYS_RET_FAIL;				
		}

		ResponseCmp->pCmpData1 = ResponseCmp->pStart + 3;
		ResponseCmp->pCmpData2 = ResponseCmp->pStart + 4;
		ResponseCmp->pCmpData3 = ResponseCmp->pStart + 5;
		
		return CMP_RET_STATUS;
	}
	/*λ���м�*/
	else
	{
		if ((*(ResponseCmp->pCmpData1) == cmpBuff[0]) && \
			(*(ResponseCmp->pCmpData2) == cmpBuff[1]) && \
			(*(ResponseCmp->pCmpData3) == cmpBuff[2]))
		{
			CMP_RET_STATUS = SYS_RET_FAIL;			
		}

		ResponseCmp->pCmpData1++;
		ResponseCmp->pCmpData2++;
		ResponseCmp->pCmpData3++;		
		
		return CMP_RET_STATUS;
	}
}
