#ifndef _SYS_CONTROL_MAP_H_
#define _SYS_CONTROL_MAP_H_

/*�����ں��㷨ѡ��*/
#if 0
	/*��ֱ����:���׻��������ں�*/
	#define SINS_DATA_FUSION__VER_THIRDORDER
#endif

#if 1
	/*��ֱ����:����������*/
	#define SINS_DATA_FUSION__VER_KALMAN
#endif

#if 0	
	/*ˮƽ����:���׻��������ں�*/
	#define SINS_DATA_FUSION__HOR_THIRDORDER
#endif

#if 1
	/*ˮƽ����:�������ߵ������ں�*/
	#define SINS_DATA_FUSION__HOR_KALMAN
#endif



/*�����㷨ѡ��*/
/*PID�㷨*/
#define CONTROL_SYS__ONLY_PID			    (1)
#if (CONTROL_SYS__ONLY_PID == SYS_ENABLE)
	#define CONTROL_SYS__PID
#endif

/*ADRC�㷨*/
#define CONTROL_SYS__ONLY_ADRC				(0)
#if (CONTROL_SYS__ONLY_ADRC == SYS_ENABLE)
	#define CONTROL_SYS__ADRC
#endif

/*PID + ADRC�㷨*/
#define CONTROL_SYS__PID_ADRC				(0)
#if (CONTROL_SYS__PID_ADRC == SYS_ENABLE)
	#define CONTROL_SYS__PID
	#define CONTROL_SYS__ADRC
#endif



/*ң�����ݽ��շ�ʽѡ��*/
#if 0
	/*PWM*/
	#define REMOTE_DATA_RECV__PWM
#endif

#if 1
	/*PPM*/
	#define REMOTE_DATA_RECV__PPM
#endif

#if 0
	/*SBUS*/
	#define REMOTE_DATA_RECV__SBUS
#endif



/*�����������ѡ��*/
/*debugging*/
#define CTRL_MOTOR_DRIVER_ON_FIXED_DEBUGGING  (0)  /*�̶����ܵ���ʱʹ��,��ʽ����ʱ��ֹʹ��*/


#define REMOT_THROTTLE_IS_AUTO_RETURN_TO_MID	(1)	/*1:ң�������Զ����� / 0:ң�����Ų����Զ�����*/

#endif
