#include "filter_Kalman.h"

/*��ֱ�������۲���*/
Filter_Kalman_Vertical g_sFilterKalmanVertical = 
{
	/*R*/
	.R = 
	{
		5.0e-4f, 6.0e-4f,
	},
	
	/*quality factor*/
	.Q = 30,
	
	/*bias Gain*/
	.accBiasGain = 
	{
		0.0005f, 0.0005f, 0.0005f,
	},
	
	/*present covariance*/
	.pre_conv = 
	{
		0.18f, 0.1f, 0.1f, 0.18f,
	},
};

/*��ֱ�������۲����*/
void filter_Kalman_Estimate_Vertical(fp32 pos_observation,  /*λ�ù۲���*/
									 u16 estimateDelay, 	/*�۲⴫������ʱ*/
									 SINS *SinsKf, 	   		/*�ߵ��ṹ��*/
									 fp32 driveTarg,	    /*ϵͳԭʼ������*/
									 Filter_Kalman_Vertical *Kalman,
									 EARTH_FRAME_AXIS AXIS,
								     fp32 deltaT)
{
	fp32 conv_z = 0, z_cor = 0;
	fp32 temp_conv[4] = {0};	   /*����Э����*/
	fp32 k[2] = {0}; 			   /*�������*/
	fp32 c_temp = 0;
	
	/*����״̬*/
	SinsKf->curAcc[AXIS] = driveTarg;
	SinsKf->curAcc[AXIS] = SinsKf->accOffset[AXIS] + SinsKf->curAcc[AXIS];
	SinsKf->curPosition[AXIS] += SinsKf->curSpeed[AXIS] * deltaT + \
								 (SinsKf->curAcc[AXIS] * deltaT * deltaT) / 2.0f;
	SinsKf->curSpeed[AXIS] += SinsKf->curAcc[AXIS] * deltaT;
	
	/*����Э����*/
	c_temp = Kalman->pre_conv[1] + Kalman->pre_conv[3] * deltaT;
	temp_conv[0] = Kalman->pre_conv[0] + Kalman->pre_conv[2] * deltaT + c_temp * deltaT + Kalman->R[0];
	temp_conv[1] = c_temp;
	temp_conv[2] = Kalman->pre_conv[2] + Kalman->pre_conv[3] * deltaT;
	temp_conv[3] = Kalman->pre_conv[3] + Kalman->R[1];
	
	/*���㿨��������*/
	conv_z = temp_conv[0] + Kalman->Q;
	k[0] = temp_conv[0] / conv_z;
	k[1] = temp_conv[2] / conv_z;
	
	/*�����ں����*/
	z_cor = pos_observation - SinsKf->pos_History[AXIS][estimateDelay];
	SinsKf->curPosition[AXIS] += k[0] * z_cor;
	SinsKf->curSpeed[AXIS] += k[1] * z_cor;
	SinsKf->accOffset[AXIS] += Kalman->accBiasGain[AXIS] * z_cor;
	
	/*����״̬Э�������*/
	Kalman->pre_conv[0] = (1 - k[0]) * temp_conv[0];
	Kalman->pre_conv[1] = (1 - k[0]) * temp_conv[1];
	Kalman->pre_conv[2] = temp_conv[2] - k[1] * temp_conv[0];
	Kalman->pre_conv[3] = temp_conv[3] - k[1] * temp_conv[1];
} 



/*ˮƽGPS�������۲���*/
Filter_Kalman_Horizontal g_sFilter_Kalman_GPS_Horizontal = 
{	
	/*quality factor*/
	.Q = 
	{
		0.075f, 1.2f,
	},
	
	/*bias Gain*/
	.R_AccBias = 
	{
		0.0001f, 0,
	},
	
	/*present covariance*/
	.pre_conv = 
	{
		0.018f, 0.005f, 0.005f, 0.5f,
		0.018f, 0.005f, 0.005f, 0.5f,
	},
	
	/*k*/
	.K = 
	{
		0
	},
	
	/*accBiasGain*/
	.accBiasGain =
	{
		0, 0.001f,
	},
};

/*ˮƽ(GPS)�������۲����*/
void filter_Kalman_Estimate_GPS_Horizontal(fp32 pos_observation,    /*λ�ù۲���*/
									       fp32 speed_observation,  /*�ٶȹ۲���*/
									       fp32 gpsQuality,			/*GPS��λ����*/
									       u16 estimateDelay, 	    /*λ�ù۲⴫������ʱ*/
									       SINS *SinsKf, 	   		/*�ߵ��ṹ��*/
										   fp32 driveTarg,	   		/*ϵͳԭʼ������*/
									       Filter_Kalman_Horizontal *Kalman,
									       EARTH_FRAME_AXIS AXIS,
								           fp32 deltaT)
{
	fp32 conv_z = 0;
	fp32 z_delta[2] = {0};
	fp32 conv_temp = 0;
	fp64 temp_conv[4] = {0};	/*����Э����*/
	
	/*���㶯̬��*/
	Kalman->R[KALMAN_POS] = 0.5f * KALMAN_GPS_PROCESS_NOISE_CONSTANT * KALMAN_GPS_DATA_UPDATE_PERIOD_S * KALMAN_GPS_DATA_UPDATE_PERIOD_S; /*POS Noise :0.005*/
	Kalman->R[KALMAN_SPD] = KALMAN_GPS_PROCESS_NOISE_CONSTANT * KALMAN_GPS_DATA_UPDATE_PERIOD_S;				 					      /*SPEED Noise :0.1*/
	
	/*����״̬*/
	SinsKf->curPosition[AXIS] += SinsKf->curSpeed[AXIS] * deltaT + \
								 ((driveTarg + Kalman->accBiasGain[AXIS]) * deltaT * deltaT) / 2.0f; /*�۲Ȿ�ε�λ������*/
	SinsKf->curSpeed[AXIS] += (driveTarg + Kalman->accBiasGain[AXIS]) * deltaT; 					 /*�۲Ȿ�ε��ٶ�����*/
	
	/*����Э����*/
	conv_temp = Kalman->pre_conv[AXIS][1] + Kalman->pre_conv[AXIS][3] * deltaT;
	temp_conv[0] = Kalman->pre_conv[AXIS][0] + Kalman->pre_conv[AXIS][2] * deltaT + conv_temp * deltaT + Kalman->R[0];
	temp_conv[1] = conv_temp;
	temp_conv[2] = Kalman->pre_conv[AXIS][2] + Kalman->pre_conv[AXIS][3] * deltaT;
	temp_conv[3] = Kalman->pre_conv[AXIS][3] + Kalman->R[1];
	
	/*���㿨��������*/
	conv_z = 1.0f / ((temp_conv[0] + Kalman->Q[0] * gpsQuality) * (temp_conv[3] + Kalman->Q[1] * gpsQuality) - temp_conv[1] * temp_conv[2]);
	
	/*��������*/
	Kalman->K[0][0] = ( temp_conv[0] * (temp_conv[3] + Kalman->Q[1] * gpsQuality) - temp_conv[1] * temp_conv[2]) * conv_z;
	Kalman->K[0][1] = ( temp_conv[1] * Kalman->Q[0] * gpsQuality) * conv_z;
	Kalman->K[1][0] = ( temp_conv[2] * Kalman->Q[1] * gpsQuality) * conv_z;
	Kalman->K[1][1] = (-temp_conv[1] * temp_conv[2] + temp_conv[3] * (temp_conv[0] + Kalman->Q[0] * gpsQuality)) * conv_z;	
	
	/*�ں��������*/
	z_delta[0] = pos_observation - SinsKf->pos_History[AXIS][estimateDelay];
	z_delta[1] = speed_observation - SinsKf->speed_History[AXIS][estimateDelay * 2];
	
	/*��ǰλ�ú��ٶȸ���*/
	SinsKf->curPosition[AXIS] += (Kalman->K[0][0] * z_delta[0]) + (Kalman->K[0][1] * z_delta[1]); /*�õ����ε�λ�����*/
	SinsKf->curSpeed[AXIS] += (Kalman->K[1][0] * z_delta[0]) + (Kalman->K[1][1] * z_delta[1]); /*�õ����ε��ٶ����*/
	
	Kalman->accBiasGain[AXIS] += (Kalman->R_AccBias[0] * z_delta[0]) + (Kalman->R_AccBias[1] * z_delta[1]); /*���ٶȻ���*/
	
	/*����״̬Э�������*/
	Kalman->pre_conv[AXIS][0] = (1 - Kalman->K[0][0]) * temp_conv[0] - Kalman->K[0][1] * temp_conv[2];
	Kalman->pre_conv[AXIS][1] = (1 - Kalman->K[0][0]) * temp_conv[1] - Kalman->K[0][1] * temp_conv[3];
	Kalman->pre_conv[AXIS][2] = (1 - Kalman->K[1][1]) * temp_conv[2] - Kalman->K[1][0] * temp_conv[0];
	Kalman->pre_conv[AXIS][3] = (1 - Kalman->K[1][1]) * temp_conv[3] - Kalman->K[1][0] * temp_conv[1];	
}
