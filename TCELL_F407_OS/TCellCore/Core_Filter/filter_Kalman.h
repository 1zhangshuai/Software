#ifndef _FILTER_KALMAN_H_
#define _FILTER_KALMAN_H_

#include "sys_Platform.h"
#include "sins_Strapdown.h"

#define KALMAN_GPS_PROCESS_NOISE_CONSTANT	(1.0f)
#define KALMAN_GPS_DATA_UPDATE_PERIOD_S		(0.1f) /*100ms*/

typedef enum
{
	KALMAN_POS = 0, /*λ�ö���*/
	KALMAN_SPD = 1, /*�ٶȶ���*/
}KALMAN_TARG;

/*��ֱ�������۲���*/
typedef struct
{
	fp32 R[2];
	fp32 Q;				/*quality factor(Ʒ������)*/
	fp32 accBiasGain[3];
	fp32 pre_conv[4];	/*present covariance*/
}Filter_Kalman_Vertical;

/*ˮƽ�������۲���*/
typedef struct
{
	fp32 R[2];			 /*��������*/
	fp32 Q[2];		     /*��������*/
	fp32 R_AccBias[2];   /*���ٶȹ�������*/
	fp64 pre_conv[2][4]; /*������Э�������*/
	fp64 K[2][2];		 /*�������*/
	fp32 accBiasGain[2]; /*���ٶȻ���ֵ*/
}Filter_Kalman_Horizontal;

/*��ֱ�������۲����*/
void filter_Kalman_Estimate_Vertical(fp32 pos_observation,  /*λ�ù۲���*/
									 u16 estimateDelay, 	/*�۲⴫������ʱ*/
									 SINS *SinsKf, 	   		/*�ߵ��ṹ��*/
									 fp32 driveTarg,	    /*ϵͳԭʼ������*/
									 Filter_Kalman_Vertical *Kalman,
									 EARTH_FRAME_AXIS AXIS,
								     fp32 deltaT);

/*ˮƽ(GPS)�������۲����*/
void filter_Kalman_Estimate_GPS_Horizontal(fp32 pos_observation,    /*λ�ù۲���*/
									       fp32 speed_observation, /*�ٶȹ۲���*/
									       fp32 gpsQuality,			/*GPS��λ����*/
									       u16 estimateDelay, 	    /*�۲⴫������ʱ*/
									       SINS *SinsKf, 	   		/*�ߵ��ṹ��*/
										   fp32 driveTarg,	    /*ϵͳԭʼ������*/
									       Filter_Kalman_Horizontal *Kalman,
									       EARTH_FRAME_AXIS AXIS,
								           fp32 deltaT);

/*��ֱ�������۲���*/
extern Filter_Kalman_Vertical g_sFilterKalmanVertical;

/*ˮƽGPS�������۲���*/
extern Filter_Kalman_Horizontal g_sFilter_Kalman_GPS_Horizontal;

#endif
