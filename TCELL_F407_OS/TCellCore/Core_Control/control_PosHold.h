#ifndef _CONTROL_POSHOLD_H_
#define _CONTROL_POSHOLD_H_

#include "sys_Platform.h"
#include "control_Config.h"

/*����ģʽ��,ң�˻��к�,����ˮƽ�ٶȿ���ɲ��,��ɲͣ���ٸ�ֵGPSλ��ѡ��*/
SYS_RETSTATUS horizontal_GPS_Get_Stop_Point_XY(Vector2f_Nav *stopPoint);

/*����ģʽ��,ң�˻��к�,����ˮƽ�ٶȿ���ɲ��,��ɲͣ���ٸ�ֵ����λ��ѡ��*/
SYS_RETSTATUS horizontal_OpticFlow_Get_Stop_Point_XY(Vector2f *stopPoint);

/*ˮƽ����ļ��ٶȻ������*/
Vector2f *horizontal_Acc_Convert_To_Dip_Angle(Vector2f acc2f, Vector2f *angle2f);

/*ˮƽλ�ÿ�����*/
void horizontal_Control_PosHold(fp32 controlDeltaT);

/*ˮƽGPSλ�ÿ�����*/
void horizontal_Control_GPS_PosHold(fp32 controlDeltaT);

/*ˮƽ����λ�ÿ�����*/
void horizontal_Control_OpticFlow_PosHold(fp32 controlDeltaT);

extern Vector2f g_sHorizontalExpectAcc; /*ˮƽ�������ٶ�*/
extern Vector2f g_sHorizontalExpectAngle; /*ˮƽ�����Ƕ�*/

#endif
