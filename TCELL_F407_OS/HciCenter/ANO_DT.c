/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��data_transfer.c
 * ����    �����ݴ���
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/

#include "ANO_DT.h"
#include "attitude_Aircraft.h"
#include "bsp_BoardLib.h"
#include "status_Aircraft.h"
#include "pid_System.h"
#include "msp_UART.h"
#include "calib_SensorData.h"
#include "sys_Debug.h"
#include "sins_Strapdown.h"
#include "control_Aircraft.h"

dt_flag_t f = {0};		   		   		/*��Ҫ�������ݵı�־*/
#define ano_data_to_send g_DebugTxBuff	/*�������ݻ���*/
#define ano_rx_buff   	 g_DebugRxBuff  /*�������ݻ���*/

/////////////////////////////////////////////////////////////////////////////////////
//Data_Exchange��������������ݷ������󣬱�����ʵ��ÿ5ms����һ�δ�������������λ�������ڴ˺�����ʵ��
//�˺���Ӧ���û�ÿ1ms����һ��,ʱ��������ò�һ��
void ANO_DT_Data_Exchange(void)
{	
	static vu8 cnt          = 0;
	static vu8 senser_cnt 	= 2;   /*10msִ��һ��*/
	static vu8 status_cnt 	= 3;   /*15msִ��һ��*/
	static vu8 rcdata_cnt 	= 4;   /*20msִ��һ��*/
	static vu8 motopwm_cnt	= 5;   /*25msִ��һ��*/
	static vu8 power_cnt    = 10;  /*50msִ��һ��*/
	
	/*�û������ϴ�����*/
	static vu8 height_cnt   = 40;  /*200msִ��һ��*/	
	static vu8 gps_cnt      = 20;  /*100msִ��һ��*/
	static vu8 speed_cnt    = 21;  /*105msִ��һ��*/
	
	/*���͹ߵ�����������*/
	if((cnt % senser_cnt) == (senser_cnt-1))
		f.send_senser = 1;	
	
	/*������̬����*/
	if((cnt % status_cnt) == (status_cnt-1))
		f.send_status = 1;	
	
	/*����10ͨ��ң��ֵ:1000~2000*/
	if((cnt % rcdata_cnt) == (rcdata_cnt-1))
		f.send_rcdata = 1;	
	
	/*����pwm���ֵ*/
	if((cnt % motopwm_cnt) == (motopwm_cnt-1))
		f.send_motopwm = 1;	
	
	/*���͵�Դ��Ϣ*/
	if((cnt % power_cnt) == (power_cnt-1))
		f.send_power = 1;	

	/*�û������ϴ�����*/	
	/*��ѹ��+�������߶�*/
	if((cnt % height_cnt) == (height_cnt-1))
		f.send_height = 1;	
	
	/*gps*/
	if((cnt % gps_cnt) == (gps_cnt-1))
		f.send_gps = 1;	
	
	/*xyz �ٶ�*/
	if((cnt % speed_cnt) == (speed_cnt-1))
		f.send_xyzspeed = 1;		
	
	cnt++;
	
	
	
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_version)
	{
		f.send_version = 0;
		ANO_DT_Send_Version(PILOT_HARDWARE_TYPE, PILOT_HARDWARE_VERSION, PILOT_SOFTWARE_VERSION, PILOT_PROTOCOL_VERSION, PILOT_BOOTLOADER_VERSION);
	}
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_status) /*������ʱ�ϴ� ������̬����*/
	{
		f.send_status = 0;
		ANO_DT_Send_Status(g_psAttitudeAll->Ahrs.roll, g_psAttitudeAll->Ahrs.pitch, g_psAttitudeAll->Ahrs.yaw, \
					       g_psAttitudeAll->nowBeroAltitude, g_sUav_Status.UavFlyType.CURRENT, g_sUav_Status.LOCK_STATUS);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_senser) /*������ʱ�ϴ� ���͹ߵ�����������*/
	{
		f.send_senser = 0;
		ANO_DT_Send_Senser(g_psAccAttitude->x, g_psAccAttitude->y, g_psAccAttitude->z,
						   g_psGyroAttitude->x, g_psGyroAttitude->y, g_psGyroAttitude->z,
						   g_psMagFilter->x, g_psMagFilter->y, g_psMagFilter->z, (s32)g_psAttitudeAll->curPressure);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_rcdata) /*������ʱ�ϴ� ����10ͨ��ң��ֵ:1000~2000*/
	{
		f.send_rcdata = 0;
		ANO_DT_Send_RCData(g_psRemotData->AttThrottle, g_psRemotData->AttYaw, g_psRemotData->AttRoll, g_psRemotData->AttPitch, \
						   g_psRemotData->GimPitch, g_psRemotData->GimYaw, g_psRemotData->SWA, g_psRemotData->SWB, g_psRemotData->SWC, g_psRemotData->SWD);
	}	
/////////////////////////////////////////////////////////////////////////////////////	
	if(f.send_motopwm) /*������ʱ�ϴ� ����Motor1, Motor2, Motor3, Motor4���ֵ*/
	{
		f.send_motopwm = 0;
		ANO_DT_Send_MotoPWM(g_psControlAircraft->CurMotorPwmOutput.channle4, g_psControlAircraft->CurMotorPwmOutput.channle2, \
			                g_psControlAircraft->CurMotorPwmOutput.channle3, g_psControlAircraft->CurMotorPwmOutput.channle1, \
							0, 0, 0, 0);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_power) /*������ʱ�ϴ� ���͵�Դ��Ϣ*/
	{
		f.send_power = 0;
		ANO_DT_Send_Power(126, 111);
	}
///////////////////////////////////////==== �û����� ====////////////////////////////
	if(f.send_height) /*������ʱ�ϴ� ��ѹ�ƺͳ����������߶�*/
	{
		f.send_height = 0;
		ANO_DT_Send_Height(g_psAttitudeAll->nowBeroAltitude, g_psAttitudeAll->nowUltrAltitude);
	}
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_gps) /*������ʱ�ϴ� GPS��Ϣ*/
	{
		f.send_gps = 0;
		ANO_DT_Send_GPS(g_psAttitudeAll->GpsData.POS_FIX_TYPE, g_psAttitudeAll->GpsData.satelliteNbr, \
					    g_psAttitudeAll->GpsData.Coordinate_s4.lon, g_psAttitudeAll->GpsData.Coordinate_s4.lat, \
						g_psAttitudeAll->GpsData.headMot);	
	}
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_xyzspeed) /*������ʱ�ϴ� X,Y,Z�����ٶ���Ϣ*/
	{
		f.send_xyzspeed = 0;
		ANO_DT_Send_XYZ_Speed(g_psSinsReal->curSpeed[EARTH_FRAME_Y], g_psSinsReal->curSpeed[EARTH_FRAME_X], g_psSinsReal->curSpeed[EARTH_FRAME_Z]);		
	}
//////////////////////////////////////==== ������� ====/////////////////////////////
	if(f.send_pid1) /*�����ϴ�: ���ٶȻ� PID����*/
	{
		f.send_pid1 = 0;
		ANO_DT_Send_PID(1, g_psPidSystem->RollGyro.PID.kP, g_psPidSystem->RollGyro.PID.kI, g_psPidSystem->RollGyro.PID.kD, \
						   g_psPidSystem->PitchGyro.PID.kP, g_psPidSystem->PitchGyro.PID.kI, g_psPidSystem->PitchGyro.PID.kD, \
						   g_psPidSystem->YawGyro.PID.kP, g_psPidSystem->YawGyro.PID.kI, g_psPidSystem->YawGyro.PID.kD);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_pid2) /*�����ϴ�: �ǶȻ� PID����*/
	{
		f.send_pid2 = 0;
		ANO_DT_Send_PID(2, g_psPidSystem->RollAngle.PID.kP, g_psPidSystem->RollAngle.PID.kI, g_psPidSystem->RollAngle.PID.kD, \
						   g_psPidSystem->PitchAngle.PID.kP, g_psPidSystem->PitchAngle.PID.kI, g_psPidSystem->PitchAngle.PID.kD, \
						   g_psPidSystem->YawAngle.PID.kP, g_psPidSystem->YawAngle.PID.kI, g_psPidSystem->YawAngle.PID.kD);
	}
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_pid3) /*�����ϴ�: ��ֱ�ٶ�&λ�� + ˮƽ�ٶȻ� PID����*/
	{
		f.send_pid3 = 0;
		ANO_DT_Send_PID(3, g_psPidSystem->HighSpeed.PID.kP, g_psPidSystem->HighSpeed.PID.kI, g_psPidSystem->HighSpeed.PID.kD, \
						   g_psPidSystem->HighPosition.PID.kP, g_psPidSystem->HighPosition.PID.kI, g_psPidSystem->HighPosition.PID.kD, \
						   g_psPidSystem->LatitudeSpeed.PID.kP, g_psPidSystem->LatitudeSpeed.PID.kI, g_psPidSystem->LatitudeSpeed.PID.kD);
	}
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_pid4) /*�����ϴ�: ˮƽλ�û� + ��ֱ���ٶȻ� PID����*/
	{
		f.send_pid4 = 0;
		ANO_DT_Send_PID(4, g_psPidSystem->LatitudePosition.PID.kP, g_psPidSystem->LatitudePosition.PID.kI, g_psPidSystem->LatitudePosition.PID.kD, \
						   g_psPidSystem->HighAcc.PID.kP, g_psPidSystem->HighAcc.PID.kI, g_psPidSystem->HighAcc.PID.kD, \
						   0, 0, 0);
	}	
}

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
//��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
void ANO_DT_Send_Data(u8 *dataToSend , u8 length, MSP_UART_WORK_MODE UART_TX_MODE)
{
	/*=== UART���ͷ�ʽ ===*/
	msp_Uart_Send_Data(&g_sDebugUart, g_sDebugUart.pTxBuff, DEBUG_TX_BUFF_SIZE, UART_TX_MODE);	
}

/*��λ��Ӧ��*/
void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
	ano_data_to_send[0]=0xAA;
	ano_data_to_send[1]=0xAA;
	ano_data_to_send[2]=0xEF;
	ano_data_to_send[3]=2;
	ano_data_to_send[4]=head;
	ano_data_to_send[5]=check_sum;
	
	u8 sum = 0;
	for(u8 i=0;i<6;i++)
		sum += ano_data_to_send[i];
	ano_data_to_send[6]=sum;

	ANO_DT_Send_Data(ano_data_to_send, 7, MSP_UART_POLL);
}

void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
	u8 _cnt=0;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x00;
	ano_data_to_send[_cnt++]=0;
	
	ano_data_to_send[_cnt++]=hardware_type;
	ano_data_to_send[_cnt++]=BYTE1(hardware_ver);
	ano_data_to_send[_cnt++]=BYTE0(hardware_ver);
	ano_data_to_send[_cnt++]=BYTE1(software_ver);
	ano_data_to_send[_cnt++]=BYTE0(software_ver);
	ano_data_to_send[_cnt++]=BYTE1(protocol_ver);
	ano_data_to_send[_cnt++]=BYTE0(protocol_ver);
	ano_data_to_send[_cnt++]=BYTE1(bootloader_ver);
	ano_data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_Status(fp32 angle_rol, fp32 angle_pit, fp32 angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = alt;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x01;
	ano_data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	
	ano_data_to_send[_cnt++]=BYTE3(_temp2);
	ano_data_to_send[_cnt++]=BYTE2(_temp2);
	ano_data_to_send[_cnt++]=BYTE1(_temp2);
	ano_data_to_send[_cnt++]=BYTE0(_temp2);
	
	ano_data_to_send[_cnt++] = fly_model;
	
	ano_data_to_send[_cnt++] = armed;
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar)
{
	u8 _cnt=0;
	vs16 _temp;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x02;
	ano_data_to_send[_cnt++]=0;
	
	_temp = a_x;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	ano_data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x03;
	ano_data_to_send[_cnt++]=0;
	ano_data_to_send[_cnt++]=BYTE1(thr);
	ano_data_to_send[_cnt++]=BYTE0(thr);
	ano_data_to_send[_cnt++]=BYTE1(yaw);
	ano_data_to_send[_cnt++]=BYTE0(yaw);
	ano_data_to_send[_cnt++]=BYTE1(rol);
	ano_data_to_send[_cnt++]=BYTE0(rol);
	ano_data_to_send[_cnt++]=BYTE1(pit);
	ano_data_to_send[_cnt++]=BYTE0(pit);
	ano_data_to_send[_cnt++]=BYTE1(aux1);
	ano_data_to_send[_cnt++]=BYTE0(aux1);
	ano_data_to_send[_cnt++]=BYTE1(aux2);
	ano_data_to_send[_cnt++]=BYTE0(aux2);
	ano_data_to_send[_cnt++]=BYTE1(aux3);
	ano_data_to_send[_cnt++]=BYTE0(aux3);
	ano_data_to_send[_cnt++]=BYTE1(aux4);
	ano_data_to_send[_cnt++]=BYTE0(aux4);
	ano_data_to_send[_cnt++]=BYTE1(aux5);
	ano_data_to_send[_cnt++]=BYTE0(aux5);
	ano_data_to_send[_cnt++]=BYTE1(aux6);
	ano_data_to_send[_cnt++]=BYTE0(aux6);

	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_Power(u16 votage, u16 current)
{
	u8 _cnt=0;
	u16 temp;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x05;
	ano_data_to_send[_cnt++]=0;
	
	temp = votage;
	ano_data_to_send[_cnt++]=BYTE1(temp);
	ano_data_to_send[_cnt++]=BYTE0(temp);
	temp = current;
	ano_data_to_send[_cnt++]=BYTE1(temp);
	ano_data_to_send[_cnt++]=BYTE0(temp);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
	u8 _cnt=0;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x06;
	ano_data_to_send[_cnt++]=0;
	
	ano_data_to_send[_cnt++]=BYTE1(m_1);
	ano_data_to_send[_cnt++]=BYTE0(m_1);
	ano_data_to_send[_cnt++]=BYTE1(m_2);
	ano_data_to_send[_cnt++]=BYTE0(m_2);
	ano_data_to_send[_cnt++]=BYTE1(m_3);
	ano_data_to_send[_cnt++]=BYTE0(m_3);
	ano_data_to_send[_cnt++]=BYTE1(m_4);
	ano_data_to_send[_cnt++]=BYTE0(m_4);
	ano_data_to_send[_cnt++]=BYTE1(m_5);
	ano_data_to_send[_cnt++]=BYTE0(m_5);
	ano_data_to_send[_cnt++]=BYTE1(m_6);
	ano_data_to_send[_cnt++]=BYTE0(m_6);
	ano_data_to_send[_cnt++]=BYTE1(m_7);
	ano_data_to_send[_cnt++]=BYTE0(m_7);
	ano_data_to_send[_cnt++]=BYTE1(m_8);
	ano_data_to_send[_cnt++]=BYTE0(m_8);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}
void ANO_DT_Send_PID(u8 group,fp32 p1_p,fp32 p1_i,fp32 p1_d,fp32 p2_p,fp32 p2_i,fp32 p2_d,fp32 p3_p,fp32 p3_i,fp32 p3_d)
{
	u8 _cnt=0;
	vs16 _temp;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x10+group-1;
	ano_data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	ano_data_to_send[_cnt++]=BYTE1(_temp);
	ano_data_to_send[_cnt++]=BYTE0(_temp);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);
}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

/*=== ������λ���û�����Э�� ===*/
/*�ϴ���ѹ�ƺͳ����������߶�*/
void ANO_DT_Send_Height(s32 altBaro, u16 altUltr)
{
	u8 _cnt=0;
	vs32 temp1;
	u16 temp2;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x07;
	ano_data_to_send[_cnt++]=0;
	
	temp1 = altBaro;
	ano_data_to_send[_cnt++]=BYTE3(temp1);
	ano_data_to_send[_cnt++]=BYTE2(temp1);
	ano_data_to_send[_cnt++]=BYTE1(temp1);
	ano_data_to_send[_cnt++]=BYTE0(temp1);
	temp2 = altUltr;
	ano_data_to_send[_cnt++]=BYTE1(temp2);
	ano_data_to_send[_cnt++]=BYTE0(temp2);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);	
}

/*�ϴ�GPS��Ϣ*/
void ANO_DT_Send_GPS(u8 FIX_TYPE, u8 SateNbr, s32  longitudeE7, s32  latitudeE7, s16 GpsYaw)
{
	u8 _cnt=0;
	vu8  temp1;
	vs32 temp2;
	vs16 temp3;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x04;
	ano_data_to_send[_cnt++]=0;
	
	temp1 = FIX_TYPE;
	ano_data_to_send[_cnt++]= temp1;
	temp1 = SateNbr;
	ano_data_to_send[_cnt++]= temp1;	
	
	temp2 = longitudeE7;
	ano_data_to_send[_cnt++]=BYTE3(temp2);
	ano_data_to_send[_cnt++]=BYTE2(temp2);
	ano_data_to_send[_cnt++]=BYTE1(temp2);
	ano_data_to_send[_cnt++]=BYTE0(temp2);
	temp2 = latitudeE7;
	ano_data_to_send[_cnt++]=BYTE3(temp2);
	ano_data_to_send[_cnt++]=BYTE2(temp2);
	ano_data_to_send[_cnt++]=BYTE1(temp2);
	ano_data_to_send[_cnt++]=BYTE0(temp2);
	
	temp3 = GpsYaw;
	ano_data_to_send[_cnt++]=BYTE1(temp3);
	ano_data_to_send[_cnt++]=BYTE0(temp3);	
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);		
}

/*�ϴ�X,Y,Z�����ٶ���Ϣ*/
void ANO_DT_Send_XYZ_Speed(s16 speedRoll, s16 speedPitch, s16 speedYaw)
{
	u8 _cnt=0;
	vs16 temp1;
	
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0xAA;
	ano_data_to_send[_cnt++]=0x0B;
	ano_data_to_send[_cnt++]=0;
	
	temp1 = speedRoll;
	ano_data_to_send[_cnt++]=BYTE1(temp1);
	ano_data_to_send[_cnt++]=BYTE0(temp1);	

	temp1 = speedPitch;
	ano_data_to_send[_cnt++]=BYTE1(temp1);
	ano_data_to_send[_cnt++]=BYTE0(temp1);	
	
	temp1 = speedYaw;
	ano_data_to_send[_cnt++]=BYTE1(temp1);
	ano_data_to_send[_cnt++]=BYTE0(temp1);
	
	ano_data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += ano_data_to_send[i];
	
	ano_data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(ano_data_to_send, _cnt, MSP_UART_DMA);		
}
