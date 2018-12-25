#ifndef _AND_DT_H_
#define	_AND_DT_H_

#include "sys_Platform.h"
#include "msp_Uart.h"

/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��fp32�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

typedef struct 
{
	vu8 send_version;
	vu8 send_status;
	vu8 send_senser;
	vu8 send_pid1;
	vu8 send_pid2;
	vu8 send_pid3;
	vu8 send_pid4;
	vu8 send_pid5;
	vu8 send_pid6;
	vu8 send_rcdata;
	vu8 send_offset;
	vu8 send_motopwm;
	vu8 send_power;
	
	/*����Ϊ�û�����*/
	vu8 send_height;
	vu8 send_gps;
	vu8 send_xyzspeed;
}dt_flag_t;

/*=== ������λ���ٷ�Э�� ===*/
/*����:1.Euler��,���ٶȼ�,������,������ֵ; 2.����ң�ؽ���ֵ(1000~2000); 3.���PWMֵ; 4.��ص�ѹֵ; 5.��λ��PIDֵ*/
void ANO_DT_Data_Exchange(void);

/*�ϴ�Ӳ���汾��Ϣ,����汾��Ϣ,Э��汾��Ϣ,����������Ϣ,*/
void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver);

/*�ϴ�ŷ���ǵ���̬��Ϣ*/
void ANO_DT_Send_Status(fp32 angle_rol, fp32 angle_pit, fp32 angle_yaw, s32 alt, u8 fly_model, u8 armed);

/*�ϴ��ߵ�����������*/
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar);

/*�ϴ�ң�ؽ���ԭʼֵ����*/
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);

/*�ϴ�������Ϣ*/
void ANO_DT_Send_Power(u16 votage, u16 current);

/*�ϴ����PWM��Ϣ*/
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);

/*�ϴ�PID����*/
void ANO_DT_Send_PID(u8 group,fp32 p1_p,fp32 p1_i,fp32 p1_d,fp32 p2_p,fp32 p2_i,fp32 p2_d,fp32 p3_p,fp32 p3_i,fp32 p3_d);

/*��λ��Ӧ��*/
void ANO_DT_Send_Check(u8 head, u8 check_sum);

/*=== ������λ���û�����Э�� ===*/
/*�ϴ���ѹ�ƺͳ����������߶�*/
void ANO_DT_Send_Height(s32 altBaro, u16 altUltr);

/*�ϴ�GPS��Ϣ*/
void ANO_DT_Send_GPS(u8 FIX_TYPE, u8 SateNbr, s32  longitudeE7, s32  latitudeE7, s16 GpsYaw);

/*�ϴ�X,Y,Z�����ٶ���Ϣ*/
void ANO_DT_Send_XYZ_Speed(s16 speedRoll, s16 speedPitch, s16 speedYaw);


/*=== ��λ�������ϴ����� ===*/
/*��λ���ϴ���λ��*/
void ANO_DT_Send_Data(u8 *dataToSend , u8 length, MSP_UART_WORK_MODE UART_TX_MODE);

extern dt_flag_t f;

#endif

