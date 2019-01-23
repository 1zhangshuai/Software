#include "attitude_Aircraft.h"
#include "filter_DataProcess.h"
#include "sins_Strapdown.h"
#include "earth_Declination.h"

AttitudeAll g_sAttitudeAll = {0};

AttitudeAll *g_psAttitudeAll = &g_sAttitudeAll;

/*====== GPS���ݺ͹ߵ���ϵ ======*/
/*gps�������ݻ�ȡ*/
void gps_fix_position_data_get(GpsM8nPvtData pvtData, GPS_Data *gpsData)
{
	static vu16 sampleContinueTicks = 0;
	fp32 deltaT;
	
	/*��ȡ��ȷ�ļ��ʱ��*/
	get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->GpsCtrlData));
		
	/*����ʱ����(s)*/
	deltaT = g_psSystemPeriodExecuteTime->GpsCtrlData.DeltaTime / 1000.0f;
	
	/*GPS���ݸ���ʧ��*/
	if (g_sGpsM8N.UPDATE_STATUS != GPS_DATA_UPDATE_SUCC)
	{
		/*��Ǳ���GPS�۲�ˮƽΪ��Чֵ*/
		g_psUav_Status->UavSenmodStatus.Horizontal.Gps.DATA_STATUS = UAV_SENMOD_DATA_NO;
		
		return;
	}
	
	/*Utc*/
	memcpy(&(gpsData->LocalTime), &pvtData.UtcTime, sizeof(GPS_Time));	/*������ȫ��Ӧ*/
	gpsData->LocalTime.hour = pvtData.UtcTime.hour + 8;			  	    /*����ʱ��: UTC+8*/
	
	/*��λ����*/
	gpsData->POS_FIX_TYPE = pvtData.POS_FIX_TYPE;
	
	/*��λ���Ǹ���*/
	gpsData->satelliteNbr = pvtData.satelliteNbr;
	
	/*��&γ��*10^7*/
	gpsData->Coordinate_s4.lon = pvtData.Coordinate.lon;
	gpsData->Coordinate_s4.lat = pvtData.Coordinate.lat;
	
	/*��&γ����ʵ��ȷֵ:deg*/
	gpsData->Coordinate_f8.lon = pvtData.Coordinate.lon * 0.0000001f;
	gpsData->Coordinate_f8.lat = pvtData.Coordinate.lat * 0.0000001f;
	
	/*GPS���θ߶�*/
	gpsData->hMSL *= 0.1f;	   /*cm*/
	
	/*ˮƽλ�ù��ƾ���*/
	gpsData->HV_Accuracy.hAcc = pvtData.HV_Accuracy.hAcc * 0.01f; /*m*/
	
	/*��ֱλ�ù��ƾ���*/
	gpsData->HV_Accuracy.vAcc = pvtData.HV_Accuracy.vAcc * 0.01f; /*m*/
	
	/*GPS��ȡ���ص���ϵ�����ٶ�(Y Axis)*/
	gpsData->NED_Velocity.velN = pvtData.NED_Velocity.velN * 0.1f; /*cm/s*/
	
	/*GPS��ȡ���ص���ϵ�������ٶ�(X Axis)*/
	gpsData->NED_Velocity.velE = pvtData.NED_Velocity.velE * 0.1f; /*cm/s*/
	
	/*GPS��ȡ���ص���ϵ�����ٶ�(Z Axis)*/
	gpsData->NED_Velocity.velD = pvtData.NED_Velocity.velD * 0.1f; /*cm/s*/
	
	/*����Ե��ٶ�*/
	gpsData->gSpeed = pvtData.gSpeed * 0.1f; /*cm/s*/
	
	/*�����˶������*/
	gpsData->headMot = pvtData.headMot * 0.00001f; /*deg*/
	
	/*�ٶȹ��ƾ���*/
	gpsData->sAcc = pvtData.sAcc * 0.1f; /*cm/s*/
	
	/*λ�ù��ƾ���*/
	gpsData->quality = pvtData.pDOP * 0.01f;
	
	
	/*��¼�ϴ��ٶ�*/
	gpsData->LastSpeed.east  = gpsData->CurSpeed.east;
	gpsData->LastSpeed.north = gpsData->CurSpeed.north;
	gpsData->LastSpeed.up    = gpsData->CurSpeed.up;
	
	/*��¼�����ٶ�*/	
	gpsData->CurSpeed.east  = gpsData->NED_Velocity.velE;
	gpsData->CurSpeed.north = gpsData->NED_Velocity.velN;
	gpsData->CurSpeed.up    = -gpsData->NED_Velocity.velD; /*down -> up*/
	
	/*�ٶ�����*/
	gpsData->DeltaSpeed.east  = (gpsData->CurSpeed.east - gpsData->LastSpeed.east) / deltaT;	 /*��λcm/s^2*/
	gpsData->DeltaSpeed.north = (gpsData->CurSpeed.north - gpsData->LastSpeed.north) / deltaT;   /*��λcm/s^2*/
	gpsData->DeltaSpeed.up    = (gpsData->CurSpeed.up - gpsData->LastSpeed.up) / deltaT;		 /*��λcm/s^2*/	
	
	/*�ж������ź��Ƿ����㶨λ����*/
	if ((gpsData->satelliteNbr >= 9) && \
		(gpsData->quality <= 3.0f))
	{
		/*���GPS���ݿ���*/
		g_psUav_Status->UavSenmodStatus.Horizontal.Gps.DATA_STATUS = UAV_SENMOD_DATA_OK;
		
		/*��ǿ��Կ�ʼ�ں�*/
		g_psUav_Status->UavSenmodStatus.Horizontal.Gps.FUSION_STATUS = UAV_SENMOD_FUSION_START;
	}
	else
	{
		/*���GPS���ݲ�����*/		
		g_psUav_Status->UavSenmodStatus.Horizontal.Gps.DATA_STATUS = UAV_SENMOD_DATA_NO;	
	}
	
	/*������Ч,�ҵ�һ��ʹ��״̬��Ч*/
	if ((g_psUav_Status->UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
		(g_psUav_Status->UavSenmodStatus.Horizontal.Gps.FIRST_USE_AVA_STATUS != UAV_SENMOD_FIRST_USE_AVA_OK))
	{
		/*�����Ч,�ȴ�GPS�ź��ȶ�*/
		if (sampleContinueTicks > 100)
		{
			g_psUav_Status->UavSenmodStatus.Horizontal.Gps.FIRST_USE_AVA_STATUS = UAV_SENMOD_FIRST_USE_AVA_OK;
		}
		else
		{
			sampleContinueTicks++;
		}
	}
	
	/*������������,���±��Ϊδ����*/
	g_sGpsM8N.UPDATE_STATUS = GPS_DATA_UPDATE_FAIL;
}

/*GPS Home������*/
#if defined(HW_CUT__USE_GPS)

void gps_home_location_set(void)
{	
	/*home��ֻ����һ��,�������źų���100�μ��Ϊ����ʱ������ʹ�����������趨HOME��*/
	if ((g_sUav_Status.HOME_SET_STATUS != UAV_HOME_SET_YES) && \
		(g_sUav_Status.UavSenmodStatus.Horizontal.Gps.FIRST_USE_AVA_STATUS == UAV_SENMOD_FIRST_USE_AVA_OK))
	{
		g_psAttitudeAll->HomePos.Coordinate_s4.lat = g_psAttitudeAll->GpsData.Coordinate_s4.lat;	/*γ��^7*/
		g_psAttitudeAll->HomePos.Coordinate_s4.lon = g_psAttitudeAll->GpsData.Coordinate_s4.lon;   	/*����^7*/

		g_psAttitudeAll->HomePos.Coordinate_f8.lat = g_psAttitudeAll->GpsData.Coordinate_f8.lat;	/*γ��*/
		g_psAttitudeAll->HomePos.Coordinate_f8.lon = g_psAttitudeAll->GpsData.Coordinate_f8.lon;   	/*����*/		
		
		/*������趨HOME��*/
		g_sUav_Status.HOME_SET_STATUS = UAV_HOME_SET_YES;
		
		/*��λ�ߵ��ں�:����ϵX��(����)*/
		strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_X, g_psAttitudeAll->EarthFrameRelativeHome.east, 0);

		/*��λ�ߵ��ں�:����ϵY��(����)*/
		strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_Y, g_psAttitudeAll->EarthFrameRelativeHome.north, 0);			
		
		/*��ȡGPS��λ��ĵش�ƫ��*/
		g_psAttitudeAll->declination = get_earth_local_declination(g_psAttitudeAll->HomePos.Coordinate_f8.lat, \
														   	       g_psAttitudeAll->HomePos.Coordinate_f8.lon);
	}	
}

/*��ȡGPS HOME������״̬*/
UAV_HOME_SET_STATUS get_gps_home_set_status(Uav_Status *uavStatus)
{
	return (uavStatus->HOME_SET_STATUS);
}
#endif

/*�����Ķ�ά����*/
Vector2f_Nav gps_Two_Pos_XY_Offset(GPS_Coordinate_s4 loc1, GPS_Coordinate_s4 loc2)
{
	Vector2f_Nav twoPosDelta;

	twoPosDelta.north = (loc2.lat - loc1.lat) * GPS_LOCATION_SCALING_FACTOR; 							     /*�������� m*/
	twoPosDelta.east  = (loc2.lon - loc1.lon) * GPS_LOCATION_SCALING_FACTOR * gps_Longitude_Scale(loc2);   /*�������� m*/	
	return twoPosDelta;
}

/*���ȱ���*/
fp32 gps_Longitude_Scale(GPS_Coordinate_s4 loc)
{
	static s32 lastLat = 0;
	static fp32 scale = 1.0f;
	
	/*�Ƚ�����γ�����ֵ�������ظ��������Һ���*/
	if (math_Abs(lastLat - loc.lat) < 100000)
	{
		// we are within 0.01 degrees (about 1km) of the
		// same latitude. We can avoid the cos() and return
		// the same scale factor.
        return scale;		
	}

    scale = cosf(loc.lat * 1.0e-7f * DEG2RAD);
    scale = math_Constrain(scale, 1.0f, 0.01f); 
    lastLat = loc.lat;
    return scale;	
}

/*������ֱ�߾���*/
fp32 gps_Two_Pos_Segment_Distance(GPS_Coordinate_s4 loc1, GPS_Coordinate_s4 loc2)
{
	fp32 disLat = (fp32)(loc2.lat - loc1.lat);
	fp32 disLon = (fp32)(loc2.lon - loc1.lon) * gps_Longitude_Scale(loc2); /*m*/
	
	/*����ֱ�߾���*/
	return (pythagorous2(disLat, disLon) * GPS_LOCATION_SCALING_FACTOR);
}

/*��ȡ�������home��ˮƽƫ��*/
#if defined(HW_CUT__USE_GPS)
/*����(����)����ϵ������+Lat(γ��)+Y������+Lon(����)+X ����λ��ƫ��*/
/*����(����)����ϵ��������+x��+roll�����帩��+(y��)+pitch ����λ��ƫ��*/
void gps_Offset_Relative_To_Home(void)
{
	Vector2f_Nav locationDelta = {0};
	
	/*���ݵ�ǰGPS��λ��Ϣ��Home��λ����Ϣ����[��������ϵ]��������������λ��ƫ��*/
	locationDelta = gps_Two_Pos_XY_Offset(g_psAttitudeAll->HomePos.Coordinate_s4, g_psAttitudeAll->GpsData.Coordinate_s4);
   /***********************************************************************************
   ��ȷ�µ���ϵ������������������Ϊ������:
   ����������γ������,�����˻����home�㣬���������ƶ�ʱ����ʱ g_psAttitudeAll->GpsData.Coordinate_s4.lat  > g_psAttitudeAll->HomePos.Coordinate_s4.lat, 
   �õ���locationDelta.x����0;	
	
   ������������������,�����˻����home�㣬���������ƶ�ʱ����ʱ g_psAttitudeAll->GpsData.Coordinate_s4.lon > g_psAttitudeAll->HomePos.Coordinate_s4.lon,
   �õ���locationDelta.y����0;
   ******************************************************************************/	
	
   locationDelta.north *= 100;    /*�ص�������ϵ������(lat,y)����λ��ƫ��,��λΪCM*/
   locationDelta.east  *= 100;    /*�ص�������ϵ������(lon,x)����λ��ƫ��,��λΪCM*/

   g_psAttitudeAll->EarthFrameRelativeHome.north = locationDelta.north; /*����ϵ�����Home������λ��ƫ��,CM*/	
   g_psAttitudeAll->EarthFrameRelativeHome.east  = locationDelta.east; /*����ϵ�����Home������λ��ƫ��,CM*/

   /*�����˻��ڵ�������ϵ�µ��������������������λ��ƫ����ת����ǰ�����λ��ƫ��:��ͷ(����)+���*/
   g_psAttitudeAll->BodyFrameRelativeHome.x = locationDelta.east * COS_YAW + locationDelta.north * SIN_YAW;   /*�������λ��ƫ��,X������*/	
   g_psAttitudeAll->BodyFrameRelativeHome.y = -locationDelta.east * SIN_YAW + locationDelta.north * COS_YAW;  /*��ͷ����λ��ƫ��,Y������*/     
}
#endif

/*====== OpticFlow���� ======*/
/*�Թ������ݽ��д���,�������Ҫ���ٶ�,λ����Ϣ*/
#if defined(HW_CUT__USE_OPTICFLOW)

void opflow_Offset_Relative_To_Home(OpFlowUpixelsLC306DataFrame OpFlowData, fp32 sinsHeight_cm, AttitudeAll *attitudeAll)
{	
	/*�жϹ����Ƿ���³ɹ�,�����ݿ���*/
	if ((g_sOpFlowUpixelsLC306.UPDATE_STATUS != OPFLOW_UPIXELSLC306_UPDATE_SUCC) || \
		(OpFlowData.DATA_STATUS != OPFLOW_UPIXELSLC306_DATA_AVA))
	{
		/*��Ǳ���GPS�۲�ˮƽΪ��Чֵ*/
		g_psUav_Status->UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS = UAV_SENMOD_DATA_NO;
		
		return;
	}
	
	/*����δ����,��ǲ�����*/
	/*��Ǳ��ι����۲�ˮƽΪ��Чֵ*/
	g_psUav_Status->UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS = UAV_SENMOD_DATA_NO;
	
//	/*�����ۼ����ص��ͨ�˲�*/
//	attitudeAll->OpticFlowData.IntPixLPF.x = filter_OpFlowIntPixLpButterworth_Dp(OpFlowData.xIntegral, \
//																					 &(g_sFilterTarg.OpticFlowIntPixLpBwBuff[0]), \
//																					 &(g_sFilterTarg.OpticFlowIntPixLpBwPara[0])); /*x,50hz,20hz*/
//	
//	attitudeAll->OpticFlowData.IntPixLPF.y = filter_OpFlowIntPixLpButterworth_Dp(OpFlowData.yIntegral, \
//																					 &(g_sFilterTarg.OpticFlowIntPixLpBwBuff[0]), \
//																					 &(g_sFilterTarg.OpticFlowIntPixLpBwPara[1])); /*y,50hz,20hz*/
//	
//	/*�ǶȲ���λ��*/
//	attitudeAll->OpticFlowData.AngleCompensate.x += (600.0f * tanf(attitudeAll->Ahrs.roll * DEG2RAD) - \
//												         attitudeAll->OpticFlowData.AngleCompensate.x) * 0.2f;
//	
//	attitudeAll->OpticFlowData.AngleCompensate.y += (600.0f * tanf(attitudeAll->Ahrs.pitch * DEG2RAD) - \
//												         attitudeAll->OpticFlowData.AngleCompensate.y) * 0.2f;	
//	
//	/*���ص���ʵ���*/
//	attitudeAll->OpticFlowData.CurRawPosition.x = attitudeAll->OpticFlowData.IntPixLPF.x - \
//													  attitudeAll->OpticFlowData.AngleCompensate.x;
//	
//	attitudeAll->OpticFlowData.CurRawPosition.y = attitudeAll->OpticFlowData.IntPixLPF.y - \
//													  attitudeAll->OpticFlowData.AngleCompensate.y;
//													  
//	/*�������ٶ� rad/s*/
//	attitudeAll->OpticFlowData.GyroSpeed.x = attitudeAll->OpticFlowData.IntPixLPF.x / 200.0f;
//	attitudeAll->OpticFlowData.GyroSpeed.y = attitudeAll->OpticFlowData.IntPixLPF.y / 200.0f;													  
//													  
//	/*�߶�����*/
//	if (sinsHeight_cm < 200)
//	{
//		sinsHeight_cm = 100;
//	}
//	else if (sinsHeight_cm < 300)
//	{
//		sinsHeight_cm = 150;		
//	}
//	else if (sinsHeight_cm < 400)
//	{
//		sinsHeight_cm = 200;		
//	}
//	else if (sinsHeight_cm < 500)
//	{
//		sinsHeight_cm = 250;		
//	}
//	else if (sinsHeight_cm < 600)
//	{
//		sinsHeight_cm = 300;		
//	}	
//	else if (sinsHeight_cm < 1000)
//	{
//		sinsHeight_cm = 350;		
//	}
//	else	
//	{
//		sinsHeight_cm = 400;			
//	}													  
//	
//	/*������֡���������ʵλ��*/
//	attitudeAll->OpticFlowData.DealtRealPosition.x = (attitudeAll->OpticFlowData.CurRawPosition.x - \
//														  attitudeAll->OpticFlowData.LastRawPosition.x) * \
//														  sinsHeight_cm * 10 / 10000.0f; /*���ص�תmm*/
//	
//	attitudeAll->OpticFlowData.LastRawPosition.x = attitudeAll->OpticFlowData.CurRawPosition.x;
//	
//	attitudeAll->OpticFlowData.DealtRealPosition.y = (attitudeAll->OpticFlowData.CurRawPosition.y - \
//														  attitudeAll->OpticFlowData.LastRawPosition.y) * \
//														  sinsHeight_cm * 10 / 10000.0f; /*���ص�תmm*/
//	
//	attitudeAll->OpticFlowData.LastRawPosition.y = attitudeAll->OpticFlowData.CurRawPosition.y;	
//	
//	/*΢��ˮƽ�ٶ� m/s*/
//	attitudeAll->OpticFlowData.DiffSpeed.x = (attitudeAll->OpticFlowData.DealtRealPosition.x * 50.0f) / 10.0f; /*cm/s*/
//	attitudeAll->OpticFlowData.DiffSpeed.y = (attitudeAll->OpticFlowData.DealtRealPosition.y * 50.0f) / 10.0f; /*cm/s*/
//	
//	/*΢��ˮƽ�ٶȵ�ͨ�˲�*/
//	attitudeAll->OpticFlowData.DiffSpeedLPF.x += (attitudeAll->OpticFlowData.DiffSpeed.x - attitudeAll->OpticFlowData.DiffSpeedLPF.x) * 0.08f;
//	attitudeAll->OpticFlowData.DiffSpeedLPF.y += (attitudeAll->OpticFlowData.DiffSpeed.y - attitudeAll->OpticFlowData.DiffSpeedLPF.y) * 0.08f;	
//	
//	/*�ٶ� cm/s*/
//	attitudeAll->OpticFlowData.RealSpeed.x = attitudeAll->OpticFlowData.DiffSpeedLPF.x;
//	attitudeAll->OpticFlowData.RealSpeed.y = attitudeAll->OpticFlowData.DiffSpeedLPF.y;
//	
//	/*�ۻ�λ�� cm*/
//	attitudeAll->OpticFlowData.RealPosition.x += attitudeAll->OpticFlowData.DealtRealPosition.x / 10.0f; /*cm*/
//	attitudeAll->OpticFlowData.RealPosition.y += attitudeAll->OpticFlowData.DealtRealPosition.y / 10.0f; /*cm*/
	
	/*����������ʹ��,�������Ϊδ����״̬*/
	g_sOpFlowUpixelsLC306.UPDATE_STATUS = OPFLOW_UPIXELSLC306_UPDATE_FAIL;
}
#endif

/*====== Bero��Ultr�߶����ݻ�ȡ������(У׼���˲�) ======*/
/*��ȡ��ѹ����Թ۲�߶�*/
s32 baro_get_relative_altitude(fp32 currentPa, fp32 referencePa)
{
	fp32 A = 0, altM = 0;
	s32 alt_cm = 0;
	
	if (referencePa != 0)	/*��ʼ��Ϊ0,Υ����������*/
	{
		A = powf((currentPa / referencePa), 1 / 5.257f);
		altM = ((1 - A) * 6357000.0f) / (A + 161.1035f);	
	}

	alt_cm = (s32)(altM * 100.0f);	/*m -> cm*/
	
	return (alt_cm);
}

/*Bero Altitude���ݻ�ȡ�ʹ��� */
#if defined(HW_CUT__USE_MD_BERO)

vu16 g_BeroUpdateContinueTicks     = 0;
vu16 g_BeroZeroSampleContinueTicks = 0;

void bero_altitude_data_get_and_dp(Uav_Status *uavStatus)
{
	fp32 beroDeltaT;
	
	g_BeroUpdateContinueTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
	
	/*�ж��Ƿ�������ѹ���¶ȸ���ʱ��*/
	if (g_BeroUpdateContinueTicks == 1)	
	{		
		bsp_SPL06_Get_Temperature(&g_sSpl06);
	}
	/*�ж��Ƿ�������ѹ�Ƹ߶ȸ���ʱ��*/
	else if (g_BeroUpdateContinueTicks >= (SYS_BERO_MIN_MEAS_PERIOD_TICK_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))	/*110ms��ȡ1��*/
	{
		/*cntTicks��0*/
		g_BeroUpdateContinueTicks = 0;
		
		/*��ȡ��ѹ�Ƶ�ǰ��ѹֵ*/
		bsp_SPL06_Get_Pressure(&g_sSpl06);
		
		/*�жϱ�����ѹֵ�Ƿ��������ʵ��(�뺣ƽ����ѹֵ�Ƚ�,���������С�ڵ��ں�ƽ����ѹֵ)*/
		if (g_sSpl06.Pressure <= SEA_LEVEL_PRESSURE)
		{
			/*�ж��Ƿ����趨��ѹ����ο�����ѹ�Ͳο���߶�*/
			if (uavStatus->UavSenmodStatus.Vertical.Bero.ZERO_REFERENCE_SET_STATUS != UAV_SENMOD_ZERO_REFERENCE_SET_OK)				
			{	
				/*�ɼ�50�κ�,����ѹ�����ȶ���(��Ҫ����),�趨��ʼλ����ѹֵ*/
				if (g_BeroZeroSampleContinueTicks > 50)
				{
					/*�趨�ο������ѹֵ*/
					g_psAttitudeAll->BaroData.zeroPressure = g_sSpl06.Pressure;
				
					/*�����ѹ����ο�������������ȷ*/
					uavStatus->UavSenmodStatus.Vertical.Bero.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_OK;
					
					/*������ѹ���ι�ϵ,����õ���Ը߶�*/
					g_psAttitudeAll->BaroData.curAltitude = baro_get_relative_altitude(g_psAttitudeAll->BaroData.zeroPressure, g_psAttitudeAll->BaroData.zeroPressure);
					
					/*��¼��ο�����ѹ�Ƹ߶�ֵ*/
					g_psAttitudeAll->BaroData.zeroHeight = g_psAttitudeAll->BaroData.curAltitude;					
					
					/*��λ�ߵ��ں�:Z��(��)*/					
					strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_Z, g_psAttitudeAll->BaroData.curAltitude, 0);
				}
				else
				{
					g_BeroZeroSampleContinueTicks++; 
				}
			}
		
			/*��ʼλ����ѹֵ��Ч,�ű����ֱ����ߵ�������Ч*/
			if (uavStatus->UavSenmodStatus.Vertical.Bero.ZERO_REFERENCE_SET_STATUS == UAV_SENMOD_ZERO_REFERENCE_SET_OK)
			{	
				/*��Ǳ�����ѹֵΪ��Чֵ*/
				uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS = UAV_SENMOD_DATA_OK;
				
				/*��ȡ��ȷ�ļ��ʱ��*/
				get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->BeroAboveAltitude));
	
				beroDeltaT = (g_psSystemPeriodExecuteTime->BeroAboveAltitude.DeltaTime) / 1000.0f; /*ms�����s*/			

				/*ԭʼ��ѹֵ*/
				g_psAttitudeAll->BaroData.rawPressure = g_sSpl06.Pressure;			

				/*Bero Altitude: 2rd lpButterWorth FS:9HZ, FC:3HZ (��ѹ����ѹԭʼֵ������˹��ͨ�˲�)*/
				g_psAttitudeAll->BaroData.filterPressure = filter_BaroAltitudeLpButterworth_Dp(g_psAttitudeAll->BaroData.rawPressure, &(g_sFilterTarg.BaroAboveLpBwBuff[0]), \
																				               &(g_sFilterTarg.BaroAboveLpBwPara[FILTER_LPBW_BARO_9HZ_3HZ_IDX])); 				
				
				/*������ѹ���ι�ϵ,����õ�ԭʼ�߶�*/
				g_psAttitudeAll->BaroData.rawAltitude = baro_get_relative_altitude(g_psAttitudeAll->BaroData.filterPressure, g_psAttitudeAll->BaroData.zeroPressure);
			
				/*�жϵ�ǰ�Ƿ�������״̬,�����ù۲�߶�һֱΪ0*/
				if (uavStatus->LOCK_STATUS == UAV_LOCK_YES)
				{
					/*ÿ����������ƫ����*/
					g_psAttitudeAll->BaroData.obAltitudeOffset = 0 - g_psAttitudeAll->BaroData.rawAltitude;
				}
				
				/*��ǰ�۲�߶ȼ���ƫ����*/
				g_psAttitudeAll->BaroData.curAltitude = g_psAttitudeAll->BaroData.rawAltitude + \
														g_psAttitudeAll->BaroData.obAltitudeOffset;				
				
				/*������ѹ�����ݼ������Z�ᴹֱ���Ϸ����ϵ��ٶ�(cm/s)*/
				g_psAttitudeAll->BaroData.climbSpeed = (g_psAttitudeAll->BaroData.curAltitude - \
									                    g_psAttitudeAll->BaroData.lastAltitude) / beroDeltaT;
			
				/*���ι۲�߶�,��Ϊ�´μ�����ϴι۲�߶�*/
				g_psAttitudeAll->BaroData.lastAltitude = g_psAttitudeAll->BaroData.curAltitude;
			}
		}
		else /*���ݲ��Ϸ�,���β�����Ч*/
		{
			/*��Ǳ�����ѹֵΪ��Чֵ*/
			uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS = UAV_SENMOD_DATA_NO;			
			
			/*���θ߶��趨Ϊ��Чֵ*/
			g_psAttitudeAll->BaroData.curAltitude = SYS_NO_AVA_MARK;
		}
	}
}
#endif

/*��ȡ��ѹ�ƹ۲�����״̬*/
UAV_SENMOD_DATA_STATUS get_bero_estimate_data_status(Uav_Status *uavStatus)
{
	return (uavStatus->UavSenmodStatus.Vertical.Bero.DATA_STATUS);
}

/*ultr Altitude���ݻ�ȡ�ʹ��� */
#if defined(HW_CUT__USE_ULTR)

vu16 g_UltrUpdateContinueTicks     = 0;
vu16 g_UltrZeroSampleContinueTicks = 0;

void ultr_altitude_data_get_and_dp(Uav_Status *uavStatus)
{
	fp32 ultrDeltaT;
	
	g_UltrUpdateContinueTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/

	if (g_UltrUpdateContinueTicks >= (SYS_ULTR_MIN_MEAS_PERIOD_TICK_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))	/*5msִ��һ��,100ms��ȡ1��*/
	{	
		/*cntTicks��0*/
		g_UltrUpdateContinueTicks = 0;
		
		/*��ȡ��������ǰ�۲�ֵ,�������´β���ֵ*/
		g_psAttitudeAll->UltrData.rawAltitude = bsp_US100_Get_Distance(&g_sUs100);
		
		/*�жϲ��ֵ�Ƿ�Ϊ��Чֵ*/
		if (g_psAttitudeAll->UltrData.rawAltitude != SYS_NO_AVA_MARK)
		{
			/*�ж��Ƿ����趨��������ο���߶�*/
			if (uavStatus->UavSenmodStatus.Vertical.Ultr.ZERO_REFERENCE_SET_STATUS != UAV_SENMOD_ZERO_REFERENCE_SET_OK)				
			{
				/*�ɼ�10�κ�,�������������ȶ���(��Ҫ����),�趨��ʼλ�ó������߶�ֵ*/
				if (g_UltrZeroSampleContinueTicks > 50)
				{
					/*�趨��������ο���ĸ߶�*/
					g_psAttitudeAll->UltrData.zeroHeight = g_psAttitudeAll->UltrData.rawAltitude;
					
					/*��ǳ�������ο�������������ȷ*/
					uavStatus->UavSenmodStatus.Vertical.Ultr.ZERO_REFERENCE_SET_STATUS = UAV_SENMOD_ZERO_REFERENCE_SET_OK;
				}
				else
				{
					g_UltrZeroSampleContinueTicks++;
				}
			}
			
			/*��ʼλ�ó�����ֵ��Ч,�ű����ֱ����ߵ�������Ч*/
			if (uavStatus->UavSenmodStatus.Vertical.Ultr.ZERO_REFERENCE_SET_STATUS == UAV_SENMOD_ZERO_REFERENCE_SET_OK)
			{
				/*��������Ч�߶ȷ�Χ��*/
				if ((0 < g_psAttitudeAll->UltrData.rawAltitude) && (g_psAttitudeAll->UltrData.rawAltitude <= SYS_ULTR_MAX_MEAS_DISTANCE))
				{
					/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
					uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS = UAV_SENMOD_DATA_OK;

					/*��ȡ��ȷ�ļ��ʱ��*/
					get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->UltrAltitude));
	
					ultrDeltaT = (g_psSystemPeriodExecuteTime->UltrAltitude.DeltaTime) / 1000.0f; /*ms�����s*/

					/*���������� ���������˲�*/			
					g_psAttitudeAll->UltrData.filterAltitude = filter_Slider_Average_Dp(&(g_sFilterTarg.UltrSliderAverage), g_psAttitudeAll->UltrData.rawAltitude);

					g_psAttitudeAll->UltrData.curAltitude = g_psAttitudeAll->UltrData.filterAltitude;
					
					/*���㳬�������ݼ������Z�ᴹֱ���Ϸ����ϵ��ٶ�(cm/s)*/
					g_psAttitudeAll->UltrData.climbSpeed = (g_psAttitudeAll->UltrData.curAltitude - \
													        g_psAttitudeAll->UltrData.lastAltitude) / ultrDeltaT;
			
					/*���ι۲�߶�,��Ϊ�´μ�����ϴι۲�߶�*/
					g_psAttitudeAll->UltrData.lastAltitude = g_psAttitudeAll->UltrData.curAltitude;
				}
				else /*������Ч�߶ȷ�Χ��*/
				{
					/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
					uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS = UAV_SENMOD_DATA_NO;						
				}
			}				
		}
		else
		{
			/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
			uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS = UAV_SENMOD_DATA_NO;
		}
	}
}
#endif

/*��ȡ�������۲�����״̬*/
UAV_SENMOD_DATA_STATUS get_ultr_estimate_data_status(Uav_Status *uavStatus)
{
	return (uavStatus->UavSenmodStatus.Vertical.Ultr.DATA_STATUS);
}
