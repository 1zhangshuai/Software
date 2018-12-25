#include "bsp_ULTR_RCWL1603.h"

BSP_RCWL1603 g_sRCWL1603;

/*������ģ���ʼ��*/
SYS_RETSTATUS bSP_ULTR_RCWL1603_Init(BSP_RCWL1603 *rcwl1603)
{
	SYS_RETSTATUS retStatus;
	
	
	return retStatus;
}

///*������ģ���ʼ��*/
//SYS_RETSTATUS bSP_ULTR_US100_Init(BSP_US100 *us100)
//{
//	SYS_RETSTATUS statusRet = SYS_RET_SUCC;
//	u8 ackData;
//	
//	if ((ultrasonic->WorkMode == BSP_ULTRASONIC_UART) || \
//		(ultrasonic->WorkMode == BSP_ULTRASONIC_AUTO_UART))	/*����ͨ�ŷ�ʽ*/
//	{
//		/*����������ʼ��*/
//		ultrasonic->UartMaster.Uart               = USART6;
//		ultrasonic->UartMaster.Resource 		  = MSP_SRC_UART6;
//		ultrasonic->UartMaster.BaudRate           = 9600;
//		ultrasonic->UartMaster.Mode               = USART_Mode_Tx | USART_Mode_Rx;
//		
//		/*GPIO�ܽ�����*/
//		ultrasonic->UartMaster.Tx_GPIO            = GPIOC;
//		ultrasonic->UartMaster.Tx_Pin             = GPIO_Pin_6;
//		ultrasonic->UartMaster.Tx_PinSource		  = GPIO_PinSource6;
//		ultrasonic->UartMaster.RCC_Periph_Tx_GPIO = RCC_AHB1Periph_GPIOC;
//		ultrasonic->UartMaster.Rx_GPIO            = GPIOC;
//		ultrasonic->UartMaster.Rx_Pin   		  = GPIO_Pin_7;
//		ultrasonic->UartMaster.Tx_PinSource		  = GPIO_PinSource7;		
//		ultrasonic->UartMaster.RCC_Periph_Rx_GPIO = RCC_AHB1Periph_GPIOC;
//		
//		/*����Ŀ��*/
//		ultrasonic->UartMaster.GPIO_AF			  = GPIO_AF_USART6;
//		
//		/*�շ�buff*/
//		ultrasonic->UartMaster.pRxBuff            = ultrasonic->RxDataBuff;	/*���ݽ���*/
//		ultrasonic->UartMaster.pTxBuff            = &(ultrasonic->TxCmd);	/*ָ���*/
//		
//		/*���ô����շ�����ģʽ*/
//		ultrasonic->UartMaster.TxMode             = MSP_UART_POLL;
//		ultrasonic->UartMaster.RxMode 			  = MSP_UART_DMA;
//		
//		/*ѡ������DMAͨ��*/
//		ultrasonic->UartMaster.RxDma.Stream		  = DMA2_Stream1;
//		ultrasonic->UartMaster.RxDma.Channel	  = DMA_Channel_5;
//		ultrasonic->UartMaster.RxDma.PerAlignByte = DMA_PeripheralDataSize_Byte;		
//		ultrasonic->UartMaster.RxDma.MemAlignByte = DMA_MemoryDataSize_Byte;
//		ultrasonic->UartMaster.RxDma.BuffSize     = sizeof(ultrasonic->RxDataBuff) / sizeof(u8);
//		ultrasonic->UartMaster.RxDma.Mode         = DMA_Mode_Circular;	/*ѭ��ģʽ*/
//		ultrasonic->UartMaster.RxDma.Priority     = DMA_Priority_High;
//		
//		/*���ڳ�ʼ��*/
//		msp_Uart_Init(&(ultrasonic->UartMaster));		
//	}
//	
//	/*���ݳ�����ģ������ָ������ģʽ*/
//	if (ultrasonic->Targ == BSP_ULTRASONIC_RCWL_1603)	/*RCWL_1603*/
//	{	
//		/*RCWL_1603���ù���ģʽ*/
//		if (ultrasonic->WorkMode == BSP_ULTRASONIC_UART)
//		{
//			ultrasonic->TxCmd  = RCWL_1603_SET_SINGLE_UART_MODE_CMD;
//		}
//		else if (ultrasonic->WorkMode == BSP_ULTRASONIC_AUTO_UART)
//		{
//			ultrasonic->TxCmd  = RCWL_1603_SET_AUTO_UART_MODE_CMD;
//		}
//		else if (ultrasonic->WorkMode == BSP_ULTRASONIC_GPIO)	/*GPIO:Trig��Echoģʽ*/
//		{
//			ultrasonic->TxCmd = RCWL_1603_SET_GPIO_MODE_CMD;		
//		}
//		else if (ultrasonic->WorkMode == BSP_ULTRASONIC_PWM)	/*�ӻ�PWM���ģʽ*/
//		{
//			ultrasonic->TxCmd = RCWL_1603_SET_PWM_OUT_MODE_CMD;	
//		}

//		BSP_Ultrasonic_Send_Command(ultrasonic);
//		
//		sys_DelayMs(100);	/*�ȴ��ӻ�������Ӧ��*/
//		
//		/*���ճ�����Ӧ������*/
//		ackData = BSP_Ultrasonic_Recv_AckData(ultrasonic);
//		
//		if ((ackData == RCWL_1603_SET_SINGLE_UART_MODE_RCV) || \
//			(ackData == RCWL_1603_SET_GPIO_MODE_RCV)        || \
//			(ackData == RCWL_1603_SET_AUTO_UART_MODE_RCV)   || \
//			 ackData == RCWL_1603_SET_PWM_OUT_MODE_RCV)
//		{
//			statusRet = SYS_RET_SUCC;		/*RCWL_1603ģ�����óɹ�*/
//		}
//		
//		/*��ȡRCWL_1603�Ĺ̼��汾��Ϣ*/
//		ultrasonic->TxCmd = RCWL_1603_GET_MODULE_FIRMWARE_VERSION_CMD;
//		BSP_Ultrasonic_Send_Command(ultrasonic);
//		
//		ultrasonic->FirmwareVersion = BSP_Ultrasonic_Recv_AckData(ultrasonic);		
//		
//		/*���û�ȡ�¶�ֵ����*/
//		ultrasonic->GetTempCmd = RCWL_1603_GET_MODULE_TEMPERATURE_CMD;
//		
//		/*���û�ȡ��������ֵ����*/
//		ultrasonic->GetDistanceCmd = RCWL_1603_GET_MODULE_DISTANCE_CMD;
//	}
//	else if (ultrasonic->Targ == BSP_ULTRASONIC_US100)	/*US100*/
//	{
//		
//		/*���û�ȡ�¶�ֵ����*/
//		ultrasonic->GetTempCmd = US100_GET_MODULE_TEMPERATURE_CMD;
//		
//		/*���û�ȡ��������ֵ����*/
//		ultrasonic->GetDistanceCmd = US100_GET_MODULE_DISTANCE_CMD;
//	}
//	
//	return statusRet;
//}

///*������ģ�鷢������*/
//void BSP_Ultrasonic_Send_Command(BSP_Ultrasonic *ultrasonic)
//{
//	msp_Uart_Send_Data(&(ultrasonic->UartMaster));	/*ultrasonic->UartMaster.pTxBuff*/
//}

///*�ӳ�����ģ�����Ӧ������(DMA / interrupt / POLL����,����ֵ��������)*/
//u8 BSP_Ultrasonic_Recv_AckData(BSP_Ultrasonic *ultrasonic)
//{
//	u8 ackData;
//	
//	if (ultrasonic->RxDataBuff[0] != 0)
//	{
//		ackData = ultrasonic->RxDataBuff[0];
//	}
//	else if (ultrasonic->RxDataBuff[1] != 0)
//	{
//		ackData = ultrasonic->RxDataBuff[1];
//	}
//	else
//	{
//		ackData = 0xFF;
//	}
//	
//	/*����Buff��0,ȷ�����ݶ��Ǳ���ָ��������*/
//	memset(ultrasonic->RxDataBuff, 0, sizeof(ultrasonic->RxDataBuff) / sizeof(u8));
//	
//	return ackData;
//}

///*���������ʼ����ģ���¶�*/
//void BSP_Ultrasonic_Start_Meas_Temperature(BSP_Ultrasonic *ultrasonic)
//{
//	/*��ȡ�¶�����*/
//	ultrasonic->TxCmd = ultrasonic->GetTempCmd;
//	
//	/*��������*/
//	BSP_Ultrasonic_Send_Command(ultrasonic);
//}

///*���������ʼ��������*/
//void BSP_Ultrasonic_Start_Meas_Distance(BSP_Ultrasonic *ultrasonic)
//{
//	/*��ȡ��������*/
//	ultrasonic->TxCmd = ultrasonic->GetDistanceCmd;
//	
//	/*��������*/
//	BSP_Ultrasonic_Send_Command(ultrasonic);	
//}

///*��ȡ������ģ���¶�(DMA / interrupt / POLL����,����ֵ��������)*/
//u16 BSP_Ultrasonic_Get_Temperature(BSP_Ultrasonic *ultrasonic)
//{
//	u8 temperature;
//	
//	if (ultrasonic->RxDataBuff[0] != 0)
//	{
//		temperature = ultrasonic->RxDataBuff[0];
//	}
//	else if (ultrasonic->RxDataBuff[1] != 0)
//	{
//		temperature = ultrasonic->RxDataBuff[1];
//	}	
//	
//	/*����Buff��0,ȷ�����ݶ��Ǳ���ָ��������*/
//	memset(ultrasonic->RxDataBuff, 0, sizeof(ultrasonic->RxDataBuff) / sizeof(u8));
//	
//	if (ultrasonic->Targ == BSP_ULTRASONIC_US100)
//	{
//		/*TEMP = temperature - 45*/
//		ultrasonic->Temperature = temperature - US100_MODULE_TEMPRATURE_BASE;
//	}
//	else
//	{
//		ultrasonic->Temperature = temperature;
//	}
//	
//	return (ultrasonic->Temperature);
//}

///*��ȡ������ģ�����(DMA / interrupt / POLL����,����ֵ��������)*/
//u16 BSP_Ultrasonic_Get_Distance(BSP_Ultrasonic *ultrasonic)
//{	
//	ultrasonic->Distance = ((u16)ultrasonic->RxDataBuff[0] << 8) + ultrasonic->RxDataBuff[1];
//	
//	/*����Buff��0,ȷ�����ݶ��Ǳ���ָ��������*/
//	memset(ultrasonic->RxDataBuff, 0, sizeof(ultrasonic->RxDataBuff) / sizeof(u8));
//	
//	return (ultrasonic->Distance);
//}
