#include "attitude_Aircraft.h"
#include "filter_DataProcess.h"
#include "sins_Strapdown.h"
#include "earth_Declination.h"

AttitudeAll g_sAttitudeAll = {0};

AttitudeAll *g_psAttitudeAll = &g_sAttitudeAll;

/*====== GPS���ݺ͹ߵ���ϵ ======*/
/*GPS�ź�������̬����*/
GPS_DATA_QUALITY_STATUS gps_data_quality_status(GPS_Data *gpsData)
{
	fp32 sateNbr, pDOP, fixType;
	static vu16 gpsSampleContinueTicks = 0;
	
	GPS_DATA_QUALITY_STATUS GPS_QUALITY_STATUS = GPS_DATA_QUALITY_BAD; /*Ĭ��ΪBAD,��ֹ��������֮������*/
	GPS_DATA_QUALITY_STATUS SATENBR_STATUS, PDOP_STATUS, FIXTYPE_STATUS;  /*��������: ������������, λ�þ�������, ��λ��������*/
	
	/*10hz,PVT����,1���Ӵ���(10������)*/
	/*�����Ǹ������ݽ��л���ƽ���˲�*/
	sateNbr = filter_Slider_Dp(&(g_sFilterTarg.GpsSateNbrSliderAverage), gpsData->satelliteNbr);
	
	/*������λ�þ����������ݽ��л���ƽ���˲�*/
	pDOP = filter_Slider_Dp(&(g_sFilterTarg.GpspDOPSliderAverage), gpsData->quality);
	
	/*�����Ƕ�λģʽ���ݽ��л���ƽ���˲�*/
	fixType = filter_Slider_Dp(&(g_sFilterTarg.GpsFixTypeSliderAverage), gpsData->POS_FIX_TYPE);
	
	/*1.�������������ź�����*/
	/*�źźܲ�ʱ���������ٸ��� sateNbr <= 7*/
	if (sateNbr <= SYS_GPS_M8N_DATA_QUALITY_BAD_SATENBR_MIN)
	{
		SATENBR_STATUS = GPS_DATA_SATENBR_BAD;	/*������Ǹ�����������Ϊ��*/
	}
	/*�ź��е�ʱ���������ٸ��� 7 < sateNbr < 9*/
	else if ((sateNbr > SYS_GPS_M8N_DATA_QUALITY_BAD_SATENBR_MIN) && \
			 (sateNbr < SYS_GPS_M8N_DATA_QUALITY_GOOD_SATENBR_MAX))
	{
		SATENBR_STATUS = GPS_DATA_SATENBR_MEDIUM;	/*������Ǹ�����������Ϊ�е�*/
	}	
	/*�źźܺ�ʱ���������ٸ��� sateNbr >= 9*/
	else if (sateNbr >= SYS_GPS_M8N_DATA_QUALITY_GOOD_SATENBR_MAX)
	{
		SATENBR_STATUS = GPS_DATA_SATENBR_GOOD;	/*������Ǹ�����������Ϊ��*/
	}	
		
	/*2.λ�þ��ȵ����ź�����*/	
	/*�źźܲ�ʱ��ˮƽλ�þ������� posQA >= 3.5f*/
	if (pDOP >= SYS_GPS_M8N_DATA_QUALITY_BAD_PDOP_QUALITY_MIN)
	{
		PDOP_STATUS = GPS_DATA_PDOP_BAD;	/*���λ�þ��ȵ�������Ϊ��*/
	}	
	/*�ź��е�ʱ��ˮƽλ�þ������� 3.0f < posQA < 3.5f*/	
	else if ((SYS_GPS_M8N_DATA_QUALITY_GOOD_PDOP_QUALITY_MAX < pDOP) && \
			 (pDOP < SYS_GPS_M8N_DATA_QUALITY_BAD_PDOP_QUALITY_MIN))
	{
		PDOP_STATUS = GPS_DATA_PDOP_MEDIUM;	/*���λ�þ��ȵ�������Ϊ�е�*/
	}	
	/*�źźܺ�ʱ��ˮƽλ�þ������� posQA <= 3.0f*/
	else if (pDOP <= SYS_GPS_M8N_DATA_QUALITY_GOOD_PDOP_QUALITY_MAX)
	{
		PDOP_STATUS = GPS_DATA_PDOP_GOOD;	/*���λ�þ��ȵ�������Ϊ��*/
	}
			
	/*3.��λ���͵����ź�����*/	
	/*�źźܲ�ʱ�Ķ�λ��ʽ:���10���г���7��3D(0x03)ģʽ,3��2D(0x02)ģʽ:(7*3 + 3*2)/10 = 2.7*/
	if (fixType <= SYS_GPS_M8N_DATA_QUALITY_BAD_FIX_MODE_MAX)
	{
		FIXTYPE_STATUS = GPS_DATA_FIXTYPE_BAD;	/*��Ƕ�λ���͵�������Ϊ��*/
	}	
	/*�ź��е�ʱ�Ķ�λ��ʽ:���10���г���8��3D(0x03)ģʽ,2��2D(0x02)ģʽ:(8*3 + 2*2)/10 = 2.8*/	
	else if ((SYS_GPS_M8N_DATA_QUALITY_BAD_FIX_MODE_MAX < fixType) && \
			 (fixType < SYS_GPS_M8N_DATA_QUALITY_GOOD_FIX_MODE_MIN))
	{
		FIXTYPE_STATUS = GPS_DATA_FIXTYPE_MEDIUM;	/*��Ƕ�λ���͵�������Ϊ�е�*/
	}		
	/*�źźܺ�ʱ�Ķ�λ��ʽ:���10���г���9��3D(0x03)ģʽ,1��2D(0x02)ģʽ:(9*3 + 1*2)/10 = 2.9*/
	else if (fixType >= SYS_GPS_M8N_DATA_QUALITY_GOOD_FIX_MODE_MIN)
	{
		FIXTYPE_STATUS = GPS_DATA_FIXTYPE_GOOD;	/*��Ƕ�λ���͵�������Ϊ��*/
	}	
	
	/*���ݵ�����ֽ���GPS�ź���������*/
	/*��*/
	if ((SATENBR_STATUS == GPS_DATA_SATENBR_BAD) || \
		(PDOP_STATUS == GPS_DATA_PDOP_BAD) || \
		(FIXTYPE_STATUS == GPS_DATA_FIXTYPE_BAD))
	{
		GPS_QUALITY_STATUS = GPS_DATA_QUALITY_BAD;	/*�ź������ܲ�*/
	}	
	
	/*�е�*/
	if ((SATENBR_STATUS == GPS_DATA_SATENBR_MEDIUM) && \
		(PDOP_STATUS == GPS_DATA_PDOP_MEDIUM) && \
		(FIXTYPE_STATUS == GPS_DATA_FIXTYPE_MEDIUM))
	{
		GPS_QUALITY_STATUS = GPS_DATA_QUALITY_MEDIUM;	/*�ź������е�*/	
	}		

	/*��*/
	if ((SATENBR_STATUS == GPS_DATA_SATENBR_GOOD) && \
		(PDOP_STATUS == GPS_DATA_PDOP_GOOD) && \
		(FIXTYPE_STATUS == GPS_DATA_FIXTYPE_GOOD))
	{
		GPS_QUALITY_STATUS = GPS_DATA_QUALITY_GOOD;	/*�ź������ܺ�*/
	}		
	
	/*��һ��ʹ��GPS����Ҫ�������ȶ���:GPS�ź���������100������Ϊ��*/
	if ((g_psAircraftStatus->GPS_DATA_FIRST_AVA_STATUS == GPS_DATA_FIRST_DISAVA) && \
		(GPS_QUALITY_STATUS == GPS_DATA_QUALITY_GOOD))
	{
		if (gpsSampleContinueTicks > 100)
		{
			g_psAircraftStatus->GPS_DATA_FIRST_AVA_STATUS = GPS_DATA_FIRST_AVA;
		}
		else
		{
			gpsSampleContinueTicks++;
		}
	}
	
	return GPS_QUALITY_STATUS;
}

/*gps�������ݻ�ȡ*/
void gps_fix_position_data_get(GpsM8nPvtData pvtData, GPS_Data *gpsData)
{
	fp32 deltaT;
	
	/*��ȡ��ȷ�ļ��ʱ��*/
	get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->GpsCtrlData));
		
	/*����ʱ����(s)*/
	deltaT = g_psSystemPeriodExecuteTime->GpsCtrlData.DeltaTime / 1000.0f;
	
	/*GPS���ݸ���ʧ��*/
	if (g_sGpsM8N.UPDATE_STATUS != GPS_DATA_UPDATE_SUCC)
	{
		/*��Ǳ���GPS�۲�ˮƽΪ��Чֵ*/
		g_psAircraftStatus->GPS_ESTIMATE_HORIZONTAL = HORIZONTAL_DATA_STATUS_NO;
		
		/*ˮƽ������״̬���GPS�۲�������Ч*/
		g_psAircraftStatus->ESTIMATE_HORIZONTAL &= HORIZONTAL_GPS_SENSOR_DISAVA;

		return;
	}
	
	/*Utc*/
	memcpy(&(gpsData->LocalTime), &pvtData.UtcTime, sizeof(GPS_Time));	/*������ȫ��Ӧ*/
	gpsData->LocalTime.hour = pvtData.UtcTime.hour + 8;	/*����ʱ��: UTC+8*/
	
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
	gpsData->hMSL /= 100.0f;	/*cm*/
	
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
	
	/*��ȡ��ǰ������������״̬����*/
	g_psAircraftStatus->GPS_QUALITY_STATUS = gps_data_quality_status(gpsData);
	
	/*���������ź�����(��ʱ����)*/
	if (g_psAircraftStatus->GPS_QUALITY_STATUS == GPS_DATA_QUALITY_GOOD)
	{
		/*��Ǳ���GPS�۲�ˮƽΪ��Чֵ*/
		g_psAircraftStatus->GPS_ESTIMATE_HORIZONTAL = HORIZONTAL_DATA_STATUS_OK;
		
		/*ˮƽ������״̬���GPS�۲�������Ч*/
		g_psAircraftStatus->ESTIMATE_HORIZONTAL |= HORIZONTAL_GPS_SENSOR_AVA;
	}
	else
	{
		/*��Ǳ���GPS�۲�ˮƽΪ��Чֵ*/
		g_psAircraftStatus->GPS_ESTIMATE_HORIZONTAL = HORIZONTAL_DATA_STATUS_NO;
		
		/*ˮƽ������״̬���GPS�۲�������Ч*/
		g_psAircraftStatus->ESTIMATE_HORIZONTAL &= HORIZONTAL_GPS_SENSOR_DISAVA;	
		
		return;
	}
	
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
	
	/*������������,���±��Ϊδ����*/
	g_sGpsM8N.UPDATE_STATUS = GPS_DATA_UPDATE_FAIL;
}

/*GPS Home������*/
#if defined(HW_CUT__USE_GPS)

void gps_home_location_set(void)
{
	/*home��ֻ����һ��,�������źų���100�μ��Ϊ��ʱ������ʹ�����������趨HOME��*/
	if ((g_psAircraftStatus->HOME_STATUS != AIRCRAFT_HOME_SET) && \
		(g_psAircraftStatus->GPS_DATA_FIRST_AVA_STATUS == GPS_DATA_FIRST_AVA))
	{
		g_psAttitudeAll->HomePos.Coordinate_s4.lat = g_psAttitudeAll->GpsData.Coordinate_s4.lat;	/*γ��*/
		g_psAttitudeAll->HomePos.Coordinate_s4.lon = g_psAttitudeAll->GpsData.Coordinate_s4.lon;   	/*����*/

		g_psAttitudeAll->HomePos.Coordinate_f8.lat = g_psAttitudeAll->GpsData.Coordinate_f8.lat;	/*γ��*/
		g_psAttitudeAll->HomePos.Coordinate_f8.lon = g_psAttitudeAll->GpsData.Coordinate_f8.lon;   	/*����*/		
		
		/*������趨HOME��*/
		g_psAircraftStatus->HOME_STATUS = AIRCRAFT_HOME_SET;
		
		/*��λ�ߵ��ں�:����ϵX��(����)*/
		strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_X, g_psAttitudeAll->EarthFrameRelativeHome.east, 0);

		/*��λ�ߵ��ں�:����ϵY��(����)*/
		strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_Y, g_psAttitudeAll->EarthFrameRelativeHome.north, 0);	
		
		/*����GPS��λ��ĵ�ǰλ�õĵش�ƫ��*/
		g_psAttitudeAll->declination = get_earth_declination(g_psAttitudeAll->HomePos.Coordinate_f8.lat, \
															 g_psAttitudeAll->HomePos.Coordinate_f8.lon);
	}	
}
#endif

/*�����Ķ�ά����*/
Vector2s_Nav gps_Two_Pos_XY_Offset(GPS_Coordinate_s4 loc1, GPS_Coordinate_s4 loc2)
{
	Vector2s_Nav twoPosDelta;

	twoPosDelta.lat = (loc2.lat - loc1.lat) * GPS_LOCATION_SCALING_FACTOR; 							   /*�������� m*/
	twoPosDelta.lon = (loc2.lon - loc1.lon) * GPS_LOCATION_SCALING_FACTOR * gps_Longitude_Scale(loc2); /*�������� m*/	
	return twoPosDelta;
}

/*���ȱ���*/
fp32 gps_Longitude_Scale(GPS_Coordinate_s4 loc)
{
	static s32 lastLat;
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
	Vector2s_Nav locationDelta = {0};
	
	/*���ݵ�ǰGPS��λ��Ϣ��Home��λ����Ϣ����[��������ϵ]��������������λ��ƫ��*/
	locationDelta = gps_Two_Pos_XY_Offset(g_psAttitudeAll->HomePos.Coordinate_s4, g_psAttitudeAll->GpsData.Coordinate_s4);
   /***********************************************************************************
   ��ȷ�µ���ϵ������������������Ϊ������:
   ����������γ������,�����˻����home�㣬���������ƶ�ʱ����ʱ g_psAttitudeAll->GpsData.Coordinate_s4.lat  > g_psAttitudeAll->HomePos.Coordinate_s4.lat, 
   �õ���locationDelta.x����0;	
	
   ������������������,�����˻����home�㣬���������ƶ�ʱ����ʱ g_psAttitudeAll->GpsData.Coordinate_s4.lon > g_psAttitudeAll->HomePos.Coordinate_s4.lon,
   �õ���locationDelta.y����0;
   ******************************************************************************/	
	
   locationDelta.lat *= 100; 	/*�ص�������ϵ������(lat,y)����λ��ƫ��,��λΪCM*/
   locationDelta.lon *= 100;    /*�ص�������ϵ������(lon,x)����λ��ƫ��,��λΪCM*/

   g_psAttitudeAll->EarthFrameRelativeHome.north = locationDelta.lat; /*����ϵ�����Home������λ��ƫ��,CM*/	
   g_psAttitudeAll->EarthFrameRelativeHome.east  = locationDelta.lon; /*����ϵ�����Home������λ��ƫ��,CM*/

   /*�����˻��ڵ�������ϵ�µ��������������������λ��ƫ����ת����ǰ�����λ��ƫ��:��ͷ(����)+���*/
   g_psAttitudeAll->BodyFrameRelativeHome.x = locationDelta.lon * COS_YAW + locationDelta.lat * SIN_YAW;   /*�������λ��ƫ��,X������*/	
   g_psAttitudeAll->BodyFrameRelativeHome.y = -locationDelta.lon * SIN_YAW + locationDelta.lat * COS_YAW;  /*��ͷ����λ��ƫ��,Y������*/     
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
		g_psAircraftStatus->OPFLOW_ESTIMATE_HORIZONTAL = HORIZONTAL_DATA_STATUS_NO;
		
		/*ˮƽ������״̬���GPS�۲�������Ч*/
		g_psAircraftStatus->ESTIMATE_HORIZONTAL &= HORIZONTAL_OPFLOW_SENSOR_DISAVA;
		
		return;
	}
	
	/*����δ����,��ǲ�����*/
	/*��Ǳ��ι����۲�ˮƽΪ��Чֵ*/
	g_psAircraftStatus->OPFLOW_ESTIMATE_HORIZONTAL = HORIZONTAL_DATA_STATUS_NO;
	/*ˮƽ������״̬��ǹ����۲�������Ч*/
	g_psAircraftStatus->ESTIMATE_HORIZONTAL &= HORIZONTAL_OPFLOW_SENSOR_DISAVA;
	
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
/*Bero Altitude���ݻ�ȡ�ʹ��� */
#if defined(HW_CUT__USE_MD_BERO)

vu16 g_BeroUpdateContinueTicks     = 0;
vu16 g_BeroZeroSampleContinueTicks = 0;

void bero_altitude_data_get_and_dp(void)
{
	fp32 beroDeltaT;
	s32 rawAltitude;
	
	g_BeroUpdateContinueTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/
	
	/*�ж��Ƿ�������ѹ���¶ȸ���ʱ��*/
	if (g_BeroUpdateContinueTicks == 1)	
	{		
		bsp_SPL06_Get_Temperature(&g_sSpl06);
	}
	/*�ж��Ƿ�������ѹ�Ƹ߶ȸ���ʱ��*/
	else if (g_BeroUpdateContinueTicks >= (110 / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))	/*110ms��ȡ1��*/
	{
		/*cntTicks��0*/
		g_BeroUpdateContinueTicks = 0;
		
		/*��ȡ��ѹ�Ƶ�ǰ��ѹֵ*/
		bsp_SPL06_Get_Pressure(&g_sSpl06);
		
		/*�жϱ�����ѹֵ�Ƿ��������ʵ��(�뺣ƽ����ѹֵ�Ƚ�,���������С�ڵ��ں�ƽ����ѹֵ)*/
		if (g_sSpl06.Pressure <= SEA_LEVEL_PRESSURE)
		{
			/*�ж��Ƿ����趨��ѹ����ο�����ѹ�Ͳο���߶�*/
			if (g_psAircraftStatus->BERO_ZERO_PRESSURE != HEIGHT_DATA_STATUS_OK)
			{	
				/*�ɼ�10�κ�,����ѹ�����ȶ���(��Ҫ����),�趨��ʼλ����ѹֵ*/
				if (g_BeroZeroSampleContinueTicks > 10)
				{
					/*�趨�ο������ѹֵ*/
					g_psAttitudeAll->zeroPressure = g_sSpl06.Pressure;
				
					/*�����ѹ����ο�������������ȷ*/
					g_psAircraftStatus->BERO_ZERO_PRESSURE = HEIGHT_DATA_STATUS_OK;
					
					/*������ѹ���ι�ϵ,����õ���Ը߶�*/
					rawAltitude = bsp_SPL06_Get_Altitude(&g_sSpl06, g_psAttitudeAll->zeroPressure);					
					
					/*��¼��ο�����ѹ�Ƹ߶�ֵ*/
					g_psAttitudeAll->zeroBeroHeight = rawAltitude;

					/*�߶ȴ�������ο������ñ����ѹ����ο������óɹ�*/
					g_psAircraftStatus->ZERO_ALTITUDE |= HEIGHT_BERO_ZERO_AVA;					
					
					/*��λ�ߵ��ں�:Z��(��)*/					
					strapdown_ins_reset(&g_sSinsReal, &g_sTOCSystem, EARTH_FRAME_Z, rawAltitude, 0);
				}
				else
				{
					g_BeroZeroSampleContinueTicks++; 
				}
			}
		
			/*��ʼλ����ѹֵ��Ч,�ű����ֱ����ߵ�������Ч*/
			if (g_psAircraftStatus->BERO_ZERO_PRESSURE == HEIGHT_DATA_STATUS_OK)
			{
				/*��ǰ��ѹֵ*/
				g_psAttitudeAll->curPressure = g_sSpl06.Pressure;
				
				/*��Ǳ�����ѹֵΪ��Чֵ*/
				g_psAircraftStatus->BERO_PRESSURE = HEIGHT_DATA_STATUS_OK;
				
				/*��Ǳ�����ѹ�ƹ۲�߶�Ϊ��Чֵ*/
				g_psAircraftStatus->BERO_ESTIMATE_ALTITUDE = HEIGHT_DATA_STATUS_OK;
				
				/*�߶ȴ�����״̬�����ѹ�ƹ۲�������Ч*/
				g_psAircraftStatus->ESTIMATE_ALTITUDE |= HEIGHT_BERO_SENSOR_AVA;
			
				/*��ȡ��ȷ�ļ��ʱ��*/
				get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->BeroAboveAltitude));
	
				beroDeltaT = (g_psSystemPeriodExecuteTime->BeroAboveAltitude.DeltaTime) / 1000.0f; /*ms�����s*/			

				/*������ѹ���ι�ϵ,����õ���Ը߶�*/
				rawAltitude = bsp_SPL06_Get_Altitude(&g_sSpl06, g_psAttitudeAll->zeroPressure);
			
				/*Bero Altitude: 2rd lpButterWorth FS:9HZ, FC:3HZ (��ѹ�ƹ۲�߶Ȱ�����˹��ͨ�˲�)*/
				g_psAttitudeAll->nowBeroAltitude = (s32)filter_BaroAltitudeLpButterworth_Dp(rawAltitude, &(g_sFilterTarg.BaroAboveLpBwBuff[0]), \
																				            &(g_sFilterTarg.BaroAboveLpBwPara[FILTER_BARO_9HZ_3HZ_IDX]));	
			
				/*������ѹ�����ݼ������Z�ᴹֱ���Ϸ����ϵ��ٶ�(cm/s)*/
				g_psAttitudeAll->beroClimbSpeed = (g_psAttitudeAll->nowBeroAltitude - \
									               g_psAttitudeAll->lastBeroAltitude) / beroDeltaT;
			
				/*���ι۲�߶�,��Ϊ�´μ�����ϴι۲�߶�*/
				g_psAttitudeAll->lastBeroAltitude = g_psAttitudeAll->nowBeroAltitude;
			}
		}
		else /*���ݲ��Ϸ�,���β�����Ч*/
		{
			/*��Ǳ�����ѹֵΪ��Чֵ*/
			g_psAircraftStatus->BERO_PRESSURE = HEIGHT_DATA_STATUS_NO;

			/*��Ǳ�����ѹ�ƹ۲�߶�Ϊ��Чֵ*/
			g_psAircraftStatus->BERO_ESTIMATE_ALTITUDE = HEIGHT_DATA_STATUS_NO;		
			
			/*�߶ȴ�����״̬�����ѹ�ƹ۲�������Ч*/
			g_psAircraftStatus->ESTIMATE_ALTITUDE &= HEIGHT_BERO_SENSOR_DISAVA;			
			
			/*���θ߶��趨Ϊ��Чֵ*/
			g_psAttitudeAll->nowBeroAltitude = SYS_NO_AVA_MARK;
		}
	}
}
#endif

/*ultr Altitude���ݻ�ȡ�ʹ��� */
#if defined(HW_CUT__USE_ULTR)

vu16 g_UltrUpdateContinueTicks     = 0;
vu16 g_UltrZeroSampleContinueTicks = 0;

void ultr_altitude_data_get_and_dp(void)
{
	fp32 ultrDeltaT;
	s16 rawAltitude;
	
	g_UltrUpdateContinueTicks++; /*PLATFORM_TASK_SCHEDULER_MIN_FOC_MS ִ��һ��*/

	if (g_UltrUpdateContinueTicks >= (75 / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))	/*5msִ��һ��,75ms��ȡ1��*/
	{	
		/*cntTicks��0*/
		g_UltrUpdateContinueTicks = 0;
		
		/*��ȡ��������ǰ�۲�ֵ,�������´β���ֵ*/
		rawAltitude = bsp_US100_Get_Distance(&g_sUs100);
		
		/*�жϲ��ֵ�Ƿ�Ϊ��Чֵ*/
		if (rawAltitude != SYS_NO_AVA_MARK)
		{
			/*�ж��Ƿ����趨��������ο���߶�*/
			if (g_psAircraftStatus->ULTR_ZERO_ALTITUDE != HEIGHT_DATA_STATUS_OK)
			{
				/*�ɼ�10�κ�,�������������ȶ���(��Ҫ����),�趨��ʼλ�ó������߶�ֵ*/
				if (g_UltrZeroSampleContinueTicks > 10)
				{
					/*�趨��������ο���ĸ߶�*/
					g_psAttitudeAll->zeroUltrHeight = rawAltitude;
					
					/*��ǳ�������ο�������������ȷ*/
					g_psAircraftStatus->ULTR_ZERO_ALTITUDE = HEIGHT_DATA_STATUS_OK;

					/*�߶ȴ�������ο������ñ�ǳ�������ο������óɹ�*/
					g_psAircraftStatus->ZERO_ALTITUDE |= HEIGHT_ULTR_ZERO_AVA;
				}
				else
				{
					g_UltrZeroSampleContinueTicks++;
				}
			}
			
			/*��ʼλ�ó�����ֵ��Ч,�ű����ֱ����ߵ�������Ч*/
			if (g_psAircraftStatus->ULTR_ZERO_ALTITUDE == HEIGHT_DATA_STATUS_OK)
			{
				/*��������Ч�߶ȷ�Χ��*/
				if ((0 < rawAltitude) && (rawAltitude <= SYS_ULTR_MAX_MEAS_DISTANCE))
				{
					/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
					g_psAircraftStatus->ULTR_ESTIMATE_ALTITUDE = HEIGHT_DATA_STATUS_OK;
			
					/*�߶ȴ�����״̬��ǳ������۲�������Ч*/
					g_psAircraftStatus->ESTIMATE_ALTITUDE |= HEIGHT_ULTR_SENSOR_AVA;

					/*��ȡ��ȷ�ļ��ʱ��*/
					get_Period_Execute_Time_Info(&(g_psSystemPeriodExecuteTime->UltrAltitude));
	
					ultrDeltaT = (g_psSystemPeriodExecuteTime->UltrAltitude.DeltaTime) / 1000.0f; /*ms�����s*/

					/*���������� ���������˲�*/			
					g_psAttitudeAll->nowUltrAltitude = filter_Slider_Average_Dp(&(g_sFilterTarg.UltrSliderAverage), rawAltitude);

					/*���㳬�������ݼ������Z�ᴹֱ���Ϸ����ϵ��ٶ�(cm/s)*/
					g_psAttitudeAll->beroClimbSpeed = (g_psAttitudeAll->nowUltrAltitude - \
													   g_psAttitudeAll->lastUltrAltitude) / ultrDeltaT;
			
					/*���ι۲�߶�,��Ϊ�´μ�����ϴι۲�߶�*/
					g_psAttitudeAll->lastUltrAltitude = g_psAttitudeAll->nowUltrAltitude;
				}
				else /*������Ч�߶ȷ�Χ��*/
				{
					/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
					g_psAircraftStatus->ULTR_ESTIMATE_ALTITUDE = HEIGHT_DATA_STATUS_NO;	
			
					/*�߶ȴ�����״̬��ǳ������۲�������Ч*/
					g_psAircraftStatus->ESTIMATE_ALTITUDE &= HEIGHT_ULTR_SENSOR_DISAVA;						
				}
			}				
		}
		else
		{
			/*��Ǳ��γ������۲�߶�Ϊ��Чֵ*/
			g_psAircraftStatus->ULTR_ESTIMATE_ALTITUDE = HEIGHT_DATA_STATUS_NO;
			
			/*�߶ȴ�����״̬��ǳ������۲�������Ч*/
			g_psAircraftStatus->ESTIMATE_ALTITUDE &= HEIGHT_ULTR_SENSOR_DISAVA;
		}
	}
}
#endif
