#ifndef _BSP_ULTR_RCWL1603_H_
#define _BSP_ULTR_RCWL1603_H_

#include "bsp_ULTR_RCWL1603_CMD.h"
#include "sys_Platform.h"
#include "msp_GPIO.h"
#include "msp_UART.h"
#include "sys_McuInit.h"

typedef struct
{
	u8 i;
}BSP_RCWL1603;

/*������ģ���ʼ��*/
SYS_RETSTATUS bSP_ULTR_RCWL1603_Init(BSP_RCWL1603 *rcwl1603);

extern BSP_RCWL1603 g_sRCWL1603;

#endif


//	MSP_Uart			*UartMaster;			/*����������ģʽ*/
//	u8 				    rxDataBuff[2];			/*����Buff*/
//	u8 				    txCmd;					/*����ָ��*/	
//	fp32				Distance;				/*��������ֵ(mm)(DisDataBuff[0] << 8 | DisDataBuff[1])*/
//	u8 					Temperature;	
//	u16					FirmwareVersion;		/*������ģ��汾��*/
//	u8					GetTempCmd;				/*��ȡ�¶�ֵ����*/
//	u8 					GetDistanceCmd;			/*��ȡ��������ֵ����*/
//	DATA_READY_STATUS	TemperatureDataStatus;	/*�¶����ݾ���״̬*/
//	DATA_READY_STATUS	DistanceDataStatus;		/*������ݾ���״̬*/

#define ULTR_US100_TX_BUFF_LENTH	(1)
#define ULTR_US100_RX_BUFF_LENTH	(2)

//typedef struct
//{
//	MSP_Uart			*UartMaster;			/*����������ģʽ*/
//	fp32				distance;				/*��������ֵ(mm)(DisDataBuff[0] << 8 | DisDataBuff[1])*/
//	u16 				temperature;			/*�¶�ֵ*/
//	DATA_READY_STATUS	temperatureDataStatus;	/*�¶����ݾ���״̬*/
//	DATA_READY_STATUS	distanceDataStatus;		/*������ݾ���״̬*/
//}BSP_US100;

///*������ģ���ʼ��*/
//SYS_RETSTATUS bSP_ULTR_US100_Init(BSP_US100 *us100);

///*������ģ�鷢������*/
//void BSP_Ultrasonic_Send_Command(BSP_US100 *us100);

///*�ӳ�����ģ�����Ӧ������*/
//u8 BSP_Ultrasonic_Recv_AckData(BSP_US100 *us100);

///*���������ʼ����ģ���¶�*/
//void BSP_Ultrasonic_Start_Meas_Temperature(BSP_US100 *us100);

///*���������ʼ��������*/
//void BSP_Ultrasonic_Start_Meas_Distance(BSP_US100 *us100);

///*��ȡ������ģ���¶�*/
//u16 BSP_Ultrasonic_Get_Temperature(BSP_US100 *us100);

///*��ȡ������ģ�����*/
//fp32 BSP_Ultrasonic_Get_Distance(BSP_US100 *us100);

//extern BSP_US100 g_sUs100;

///*���ڷ���Buff*/
//extern u8 g_Us100TxBuff[ULTR_US100_TX_BUFF_LENTH];

///*���ڽ���Buff*/
//extern u8 g_Us100RxBuff[ULTR_US100_RX_BUFF_LENTH];
