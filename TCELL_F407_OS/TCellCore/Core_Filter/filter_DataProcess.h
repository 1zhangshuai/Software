#ifndef _FILTER_ARITHMETIC_H_
#define _FILTER_ARITHMETIC_H_

#include "ahrs_Caculation.h"

/*=========================== ԭʼ�����˲��� ===========================*/
/*1.�޷��˲�(ë���˲���)*/
#define FILTER_BASE_BUFF_LEN (10)	/*�޷��˲������*/

typedef struct
{
	fp32 lastVal;						/*��һ�ε�ֵ*/
	fp32 maxAbs;						/*�����ϴα�����ֵ�����*/
	fp32 wvBuff[FILTER_BASE_BUFF_LEN];	/*Ȩֵ����*/
	u16	 index;							/*˳��*/
	u16	 isUp;							/*���*/
}FilterBase;

/*ë���˲�����ʼ��*/
void filter_Base_Init(FilterBase *filterBase, fp32 maxAbs);

/*�����ݽ���ë���˲�*/
fp32 filter_Base_Dp(FilterBase *filterBase, fp32 newVal);

/*2.�����˲�(���Ƹ�Ƶ��)*/
#define FILTER_SLIDE_BUFF_LEN (15)	/*����ƽ��������󳤶�*/

typedef struct
{
	fp32 array[FILTER_SLIDE_BUFF_LEN];
	u8   arrayLen;
}FilterSlider;

/*�����˲�����ʼ��*/
void filter_Slider_Init(FilterSlider *filterSlider, u8 arrayLen);

/*�����ݽ��л���ƽ���˲�*/
fp32 filter_Slider_Dp(FilterSlider *filterSlider, fp32 newVal);

/*�����ݽ��л�����ֵ�˲�*/
fp32 filter_Slider_Average_Dp(FilterSlider *filterSlider, fp32 newVal);

/*3.������˹���׵�ͨ�˲���*/
/*������˹�˲���ѡ��(����Ƶ��&��ֹƵ��)*/
/*ACC ���ٶ�*/
#define FILTER_ACC_200HZ_80HZ_IDX						(0)
#define FILTER_ACC_200HZ_60HZ_IDX						(1)
#define FILTER_ACC_200HZ_51HZ_IDX						(2)
#define FILTER_ACC_200HZ_30HZ_IDX						(3)
#define FILTER_ACC_200HZ_20HZ_IDX						(4)
#define FILTER_ACC_200HZ_15HZ_IDX						(5)
#define FILTER_ACC_200HZ_10HZ_IDX						(6)
#define FILTER_ACC_200HZ_5HZ_IDX						(7)
#define FILTER_ACC_200HZ_2HZ_IDX						(8)
#define FILTER_ACC_200HZ_1HZ_IDX						(9)
					
/*GYRO ���ٶ�*/					
#define FILTER_GYRO_200HZ_51HZ_IDX						(0)
#define FILTER_GYRO_200HZ_30HZ_IDX						(1)
					
/*BARO ��ѹ��*/					
#define FILTER_BARO_9HZ_3HZ_IDX							(0)
#define FILTER_BARO_9HZ_2HZ_IDX							(1)

/*PID ����*/
#define FILTER_PID_CONTROLER_PITCH_ANGLE_IDX 			(0)
#define FILTER_PID_CONTROLER_PITCH_GYRO_IDX				(1)
#define FILTER_PID_CONTROLER_ROLL_ANGLE_IDX				(2)
#define FILTER_PID_CONTROLER_ROLL_GYRO_IDX				(3)
#define FILTER_PID_CONTROLER_YAW_ANGLE_IDX				(4)
#define FILTER_PID_CONTROLER_YAW_GYRO_IDX 				(5)
#define FILTER_PID_CONTROLER_HIGH_POSITION_IDX			(6)
#define FILTER_PID_CONTROLER_HIGH_SPEED_IDX				(7)
#define FILTER_PID_CONTROLER_LONGITUDE_POSITION_IDX		(8)
#define FILTER_PID_CONTROLER_LONGITUDE_SPEED_IDX		(9)
#define FILTER_PID_CONTROLER_LATITUDE_POSITION _IDX		(10)
#define FILTER_PID_CONTROLER_LATITUDE_SPEED_IDX			(11)
#define FILTER_PID_CONTROLER_OPTICFLOW_X_POSITION_IDX	(12)
#define FILTER_PID_CONTROLER_OPTICFLOW_X_SPEED_IDX		(13)
#define FILTER_PID_CONTROLER_OPTICFLOW_Y_POSITION_IDX	(14)
#define FILTER_PID_CONTROLER_OPTICFLOW_Y_SPEED_IDX		(15)
#define FILTER_PID_CONTROLER_HIGH_ACC_IDX 				(16)
#define FILTER_PID_CONTROLER_LONGITUDE_ACC_IDX 			(17)
#define FILTER_PID_CONTROLER_LATITUDE_ACC_IDX			(18)
#define FILTER_PID_CONTROLER_OPTICFLOW_X_ACC_IDX 		(19)
#define FILTER_PID_CONTROLER_OPTICFLOW_Y_ACC_IDX		(20)

/*�˲�������*/
typedef struct
{
	u16  FS;	/*����Ƶ��*/
	u16  FC;	/*��ֹƵ��*/
	fp32 A[3];	/*��ĸ*/
	fp32 B[3];	/*����*/
}FilterLpButterworthPara;

/*�˲��������������*/
typedef struct
{
	fp32 Input[3];		/*����*/
	fp32 Output[3];		/*���*/
}FilterLpButterworthBuff;

/*������˹���׵�ͨ�˲�����ʼ��:MATLAB����*/
void filter_LpButterworth_Matlab_Init(void);

/*������˹���׵�ͨ�˲�����ʼ��:��ʽ����*/
void filter_LpButterworth_Formulary_Init(u16 freqSample, u16 freqCut, FilterLpButterworthPara *para);

/*���ٶ�3��(��̬) ������˹���׵�ͨ�˲����˲�*/
Acc3f* filter_AccAttLpButterworth_Dp(Acc3f *targAcc, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*���ٶ�(����:������У׼���ߵ������Ʒ���) ������˹���׵�ͨ�˲����˲�*/
fp32 filter_AccFuncLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*���ٶ� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_GyroFuncLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*��ѹ�Ƹ߶� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_BaroAltitudeLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*�����ۼ����ص� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_OpFlowIntPixLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*PIDƫ���� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_Pid_Control_Device_LPF(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para);

/*=========================== ��̬��������˲��� ===========================*/
/*1.һ�׻����˲�������̬��*/
typedef struct
{
	fp32 K;
	fp32 Angle;
}FilterYijieHubu;

/*һ�׻�����ʼ��*/
void filter_YijieHubu_Init(FilterYijieHubu *filterYijieHubu);

/*һ�׻����˲��˲�*/
fp32 filter_YijieHubu_Dp(FilterYijieHubu *filterYijieHubu, fp32 angle_m, fp32 gyro_m, fp32 dtS);	

/*2.���׻����˲�������̬��*/
typedef struct
{
	fp32 K;
	fp32 Angle;
	fp32 x1;
	fp32 x2;
	fp32 y1;
}FilterErjieHubu;

/*���׻�����ʼ��*/
void filter_ErjieHubu_Init(FilterErjieHubu *filterErjieHubu);

/*���׻����˲�*/
fp32 filter_ErjieHubu_Dp(FilterErjieHubu *filterErjieHubu, fp32 angle_m, fp32 gyro_m, fp32 dtS);	


/****** ���ݴ��� + ��̬���� ******/
typedef struct
{
	/*���ٶ� ���װ�����˹��ͨ�˲���*/
	FilterLpButterworthPara AccLpBwPara[10];    /*���ٶ�ԭʼ���ݰ�����˹���׵�ͨ�˲���_10��*/
	FilterLpButterworthBuff AccLpBwAttitude[3];	/*���ٶ�(��̬)������˹���׵�ͨ�˲��������������*/
	FilterLpButterworthBuff AccLpBwFeedback[3];	/*���ٶ�(����)������˹���׵�ͨ�˲��������������*/
	FilterLpButterworthBuff AccLpBwSINS[3];		/*���ٶ�(�ߵ��ں�)������˹���׵�ͨ�˲��������������*/
	FilterLpButterworthBuff AccLpBwCalib[3];	/*������(����)������˹���׵�ͨ�˲��������������*/	
	FilterLpButterworthBuff AccLpBwControl[3];	/*���ٶ�(����)������˹���׵�ͨ�˲��������������*/	

	/*���ٶ� ���װ�����˹��ͨ�˲���*/	
	FilterLpButterworthPara GyroLpBwPara[2];	/*���ٶ�ԭʼ���ݰ�����˹���׵�ͨ�˲���_2��*/
	FilterLpButterworthBuff GyroLpBwBuff[3];	/*���ٶ�ԭʼ���ݰ�����˹���׵�ͨ�˲��������������*/
	
	/*��ѹ�� ���װ�����˹��ͨ�˲���*/
	FilterLpButterworthPara BaroAboveLpBwPara[2];	/*��ѹ��(�Ϸ�)ԭʼ���ݰ�����˹���׵�ͨ�˲���_2��*/
	FilterLpButterworthBuff BaroAboveLpBwBuff[2];	/*��ѹ��(�Ϸ�)ԭʼ���ݰ�����˹���׵�ͨ�˲��������������*/

	/*���� ���װ�����˹��ͨ�˲���*/
	FilterLpButterworthPara OpticFlowIntPixLpBwPara[1];	/*�����ۼ����ص������˹��ͨ�˲�*/
	FilterLpButterworthPara OpticFlowGyroLpBwPara[1];	/*�������ٶȰ�����˹��ͨ�˲�*/
	FilterLpButterworthBuff OpticFlowIntPixLpBwBuff[2];	/*����������˹��ͨ�˲������������*/
	FilterLpButterworthBuff OpticFlowGyroLpBwBuff[2];	/*����������˹��ͨ�˲������������*/	
	
	/*������ ���ڻ���ƽ���˲���*/
	FilterSlider            MagxSliderAverage;  /*������(X��)ԭʼ���ݴ��ڻ���ƽ���˲�*/
	FilterSlider            MagySliderAverage;	/*������(Y��)ԭʼ���ݴ��ڻ���ƽ���˲�*/
	FilterSlider            MagzSliderAverage;	/*������(Z��)ԭʼ���ݴ��ڻ���ƽ���˲�*/
	
	/*������ ���ڻ���ƽ���˲���*/
	FilterSlider            UltrSliderAverage;  /*������ԭʼ���ݴ��ڻ���ƽ���˲�*/
	
	/*PID���� ���װ�����˹��ͨ�˲���*/
	FilterLpButterworthPara PidControlErrPara[1];
	FilterLpButterworthPara PidControlDivPara[1];
	FilterLpButterworthPara PidControlDivGyroPara[1];
	FilterLpButterworthBuff PidControlBuff[21]; 	   /*PID ��������ͨ�����������*/

    FilterYijieHubu         YijieHubuPitch;  		    /*Pitch��һ�׻����˲���*/
    FilterYijieHubu         YijieHubuRoll;    			/*Roll��һ�׻����˲���*/

	FilterErjieHubu 		ErjieHubuPitch;   		    /*Pitch�Ƕ��׻����˲���*/
	FilterErjieHubu 		ErjieHubuRoll;   		    /*Roll�Ƕ����˲���*/	   
}FilterTarg;


/*ԭʼ�����˲�����ʼ��*/
void filter_origin_data_filter_init(FilterTarg *filterTarg);

/*�˲����ۺ�*/
extern FilterTarg g_sFilterTarg;
extern FilterTarg *g_psFilterTarg;

#endif
