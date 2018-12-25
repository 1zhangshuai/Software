#ifndef _BSP_OPFLOW_UPIXELSLC306_CMD_H_
#define _BSP_OPFLOW_UPIXELSLC306_CMD_H_

#define OPFLOW_UPIXELSLC306_SENSOR_IIC_ADDR		(0xDC)
#define OPFLOW_UPIXELSLC306_RESOLUTION_CM		(0.005263157f)	/*1m�߶��� 1�����ض�Ӧ��λ��,��λcm*/
												 

/*OPFLOW_UPIXELSLC306 ����ָ��*/
typedef enum
{
	OPFLOW_UPIXELSLC306_STATR_CFG  = 0xAA,	/*��������ָ��*/
	OPFLOW_UPIXELSLC306_MODULE_CFG = 0xAB,	/*ģ���ڲ���������ָ��*/
	OPFLOW_UPIXELSLC306_SENSOR_CFG = 0xBB,	/*��������������ָ��*/
	OPFLOW_UPIXELSLC306_FINISH_CFG = 0xDD,	/*�ر�����ָ��*/
}OPFLOW_UPIXELSLC306_CFG_CMD;

/*OPFLOW_UPIXELSLC306 ����Ӧ��*/
typedef enum
{
	OPFLOW_UPIXELSLC306_ACK_SUCC     = 0x00,
	OPFLOW_UPIXELSLC306_ACK_IIC_FAIL = 0x01,
	OPFLOW_UPIXELSLC306_ACK_XOR_FAIL = 0x02,
}OPFLOW_UPIXELSLC306_CFG_ACK;

/*1.OPFLOW_UPIXELSLC306_STATR_CFG*/
/*Ӧ��: ��
  ���:	��*/

/*2.OPFLOW_UPIXELSLC306_MODULE_CFG*/
/*Ӧ��: ״ֵ̬( 0x00: �ɹ�, 0x01: IIC ��Ӧ��, 0x02: XOR У�����)
  ���:	���óɹ���ģ�鷵��: 0x00, 0xAB*/

/*3.OPFLOW_UPIXELSLC306_SENSOR_CFG*/
/*Ӧ��: ״ֵ̬( 0x00: �ɹ�, 0x01: IIC ��Ӧ��, 0x02: XOR У�����)
  ���:	���óɹ���ģ�鷵��: 0x00, 0xBB*/

/*4.OPFLOW_UPIXELSLC306_FINISH_CFG*/
/*Ӧ��: ��
  ���:	��*/

#endif
