#include "pid_System.h"
#include "filter_DataProcess.h"
#include "hci_oledshow.h"
#include "control_Config.h"   /*���ڿ���ϵͳ����*/

/*PID����ϵͳ*/
PidSystem g_sPidSystem =
{
	.PidSettingSystem.DO_STATUS            = PID_PARAMETER_DO_NULL, /*PID����ϵͳĬ��ʲô������*/
	.PidSettingSystem.AVA_STATUS           = PID_PARAMETER_DISAVA,
	.PidSettingSystem.PARA_DOWNLOAD_STATUS = PID_PARA_DOWNLOAD_FINISH,
};

PidSystem *g_psPidSystem = &g_sPidSystem;

/*
1ƫ���޷���־��   2�����޷���־��   3���ַ����־��   4������
5����             6ƫ�           7�ϴ�ƫ�       8ƫ���޷�ֵ��
9���ַ���ƫ��ֵ�� 10����ֵ          11�����޷�ֵ��    12���Ʋ���Kp��
13���Ʋ���Ki��    14���Ʋ���Kd��    15�������������  16�ϴο����������
17������޷���    18���Ʋ���Kp����  19���Ʋ���Ki����  20���Ʋ���Kd����
*/
const fp32 g_PidLinkInit[PID_PARAMETER_SETTING_NBR][20]=
{
	/*��̬ �ڻ�(���ٶ�)+�⻷(�Ƕ�)*/
/*                                       	   Kp        Ki        Kd                kPscale  kIscale kDscale*/
	/*1  2  3  4  5  6  7  8   9   10   11     12        13        14   15  16   17    18       19      20*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,500 ,0  ,0  ,200  ,0.75    ,5.5000    ,1.80  ,0  ,0  ,500   ,1       ,1      ,1}, /*Pitch_Gyro  ;�������ٶ�*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,500 ,0  ,0  ,200  ,0.75    ,5.5000    ,1.80  ,0  ,0  ,500   ,1       ,1      ,1}, /*Roll_Gyro   ;������ٶ�*/	
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,250 ,0  ,0  ,100  ,1.00    ,0.5000    ,0.00  ,0  ,0  ,300   ,1       ,1      ,1}, /*Yaw_Gyro    ;ƫ�����ٶ�*/	
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,30  ,0  ,0  ,80   ,4.00    ,0.0000    ,0.00  ,0  ,0  ,300   ,1       ,1      ,1}, /*Pitch_Angle ;������*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,30  ,0  ,0  ,80   ,4.00    ,0.0000    ,0.00  ,0  ,0  ,300   ,1       ,1      ,1}, /*Roll_Angle  ;�����*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,45  ,0  ,0  ,150  ,5.00    ,0.0000    ,0.00  ,0  ,0  ,300   ,1       ,1      ,1}, /*Yaw_Angle   ;ƫ����*/

	//���߲���
	//�߶ȵ���������ƣ���ƫ���޷����������Ϊ����������½��ٶ�400cm/s
	//Z���ٶȱ���+���ֿ��ƣ���ƫ���޷�		
#if (CTRL_HEIGHT_POS_CONTROL_ACC_STATUS == SYS_DISABLE) /*�Ƿ�����ֱ�߶�3������(λ��+�ٶ�+���ٶ�)*/		
/*                                       	   Kp        Ki        Kd                kPscale  kIscale kDscale*/
	/*1  2  3  4  5  6  7  8   9   10   11     12        13        14   15  16   17    18       19      20*/		
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,400 ,0  ,0  ,400  ,2.0     ,10.00     ,0.15  ,0  ,0  ,600   ,1       ,1      ,1}, /*High_Speed    ;���������ٶ�*/		
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,200 ,0  ,0  ,50   ,1.0     ,0.000     ,0     ,0  ,0  ,400   ,1       ,1      ,1}, /*High_Position ;���θ߶�λ��*/
#else
/*                                       	   Kp        Ki        Kd                kPscale  kIscale kDscale*/
	/*1  2  3  4  5  6  7  8   9   10   11     12        13        14   15  16   17    18       19      20*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,400 ,0  ,0  ,500  ,5.0     ,0.300     ,0.1   ,0  ,0  ,500   ,1       ,1      ,1}, /*High_Speed    ;���������ٶ�*/	
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,200 ,0  ,0  ,100  ,0.5     ,0.000     ,0     ,0  ,0  ,400   ,1       ,1      ,1}, /*High_Position ;���θ߶�λ��*/
#endif
 /*
1ƫ���޷���־��  2�����޷���־��3���ַ����־��   4������
5����            6ƫ�        7�ϴ�ƫ�       8ƫ���޷�ֵ��
9���ַ���ƫ��ֵ��10����ֵ       11�����޷�ֵ��    12���Ʋ���Kp��
13���Ʋ���Ki��   14���Ʋ���Kd�� 15�������������  16�ϴο����������
17������޷���
*/
	/*GPS(ˮƽ) �ڻ�(�ٶ�)+�⻷(λ��)*/
/*                                       	   Kp        Ki        Kd                kPscale  kIscale kDscale*/
	/*1  2  3  4  5  6  7  8   9   10   11     12        13        14   15  16   17    18       19      20*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,300 ,0  ,0  ,240  ,1.800   ,0.450     ,0     ,0  ,0  ,500   ,1       ,1      ,1}, /*Latitude_Speed     ;ˮƽγ���ٶ�*/	
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,180 ,0  ,0  ,8    ,0.200   ,0.000     ,0     ,0  ,0  ,150   ,1       ,1      ,1}, /*Latitude_Position  ;ˮƽγ��λ��*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,300 ,0  ,0  ,240  ,1.800   ,0.450     ,0     ,0  ,0  ,500   ,1       ,1      ,1}, /*Longitude_Speed    ;ˮƽ�����ٶ�*/		
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,180 ,0  ,0  ,8    ,0.200   ,0.000     ,0     ,0  ,0  ,150   ,1       ,1      ,1}, /*Longitude_Position ;ˮƽ����λ��*/
	
	/*Opticflow(ˮƽ) �ڻ�(�ٶ�)+�⻷(λ��)*/
/*                                       	   Kp        Ki        Kd                kPscale  kIscale kDscale*/
	/*1  2  3  4  5  6  7  8   9   10   11     12        13        14   15  16   17    18       19      20*/		
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,30  ,15 ,0  ,200  ,4.50    ,0.10      ,0.0   ,0  ,0  ,450   ,1       ,1      ,1}, /*����(ˮƽX����)�ٶ�*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,100 ,15 ,0  ,15   ,0.25    ,0.00      ,0     ,0  ,0  ,30    ,1       ,1      ,1}, /*����(ˮƽX����)λ��*/	
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,30  ,15 ,0  ,200  ,4.50    ,0.10      ,0.0   ,0  ,0  ,450   ,1       ,1      ,1}, /*����(ˮƽY����)�ٶ�*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,100 ,15 ,0  ,15   ,0.25    ,0.00      ,0     ,0  ,0  ,30    ,1       ,1      ,1}, /*����(ˮƽY����)λ��*/	
	
	/*���ٶȿ�����*/
	//�����ٶ�200cm/s^2
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,600 ,0  ,0  ,500  ,0.12    ,1.5000    ,0     ,0  ,0  ,600   ,1       ,1      ,1}, /*��ֱ���ٶȿ�����*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,100 ,0  ,0  ,3    ,0.32    ,0.0000    ,0     ,0  ,0  ,150   ,1       ,1      ,1}, /*ˮƽ���ȷ�����ٶȿ�����*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,100 ,0  ,0  ,15   ,0.45    ,0.0000    ,0     ,0  ,0  ,25    ,1       ,1      ,1}, /*ˮƽά�ȷ�����ٶȿ�����*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,0   ,0  ,0  ,0    ,0.00    ,0.0000    ,0     ,0  ,0  ,0     ,1       ,1      ,1}, /*����(ˮƽX����)���ٶ�*/
	{1  ,1 ,0 ,0 ,0 ,0 ,0 ,0   ,0  ,0  ,0    ,0.00    ,0.0000    ,0     ,0  ,0  ,0     ,1       ,1      ,1}, /*����(ˮƽY����)���ٶ�*/		
};

/*PID����ϵͳ��ʼ��*/
void pid_System_Init(PidSystem *pidSystem)
{ 
	/*1.�ǶȺͽ��ٶȿ��� ��ʼ��*/
	pid_Link_Init(&(pidSystem->PitchGyro), PID_CONTROLER_PITCH_GYRO);
	pid_Link_Init(&(pidSystem->RollGyro), PID_CONTROLER_ROLL_GYRO);	
	pid_Link_Init(&(pidSystem->YawGyro), PID_CONTROLER_YAW_GYRO);	
	pid_Link_Init(&(pidSystem->PitchAngle), PID_CONTROLER_PITCH_ANGLE);	
	pid_Link_Init(&(pidSystem->RollAngle), PID_CONTROLER_ROLL_ANGLE);
	pid_Link_Init(&(pidSystem->YawAngle), PID_CONTROLER_YAW_ANGLE);

	
	/*2.��ֱ�߶ȿ��� ��ʼ��*/
	pid_Link_Init(&(pidSystem->HighSpeed), PID_CONTROLER_HIGH_SPEED);	
	pid_Link_Init(&(pidSystem->HighPosition), PID_CONTROLER_HIGH_POSITION);	
	
	/*3.(GPS)λ�ú��ٶȿ��� ��ʼ��*/
	pid_Link_Init(&(pidSystem->LatitudeSpeed), PID_CONTROLER_LATITUDE_SPEED);	
	pid_Link_Init(&(pidSystem->LatitudePosition), PID_CONTROLER_LATITUDE_POSITION);	
	pid_Link_Init(&(pidSystem->LongitudeSpeed), PID_CONTROLER_LONGITUDE_SPEED);	
	pid_Link_Init(&(pidSystem->LongitudePosition), PID_CONTROLER_LONGITUDE_POSITION);

	/*4.(����) λ�ú��ٶȿ��� ��ʼ��*/
	pid_Link_Init(&(pidSystem->OpticFlowXSpeed), PID_CONTROLER_OPTICFLOW_X_SPEED);	
	pid_Link_Init(&(pidSystem->OpticFlowXPosition), PID_CONTROLER_OPTICFLOW_X_POSITION);
	pid_Link_Init(&(pidSystem->OpticFlowYSpeed), PID_CONTROLER_OPTICFLOW_Y_SPEED);
	pid_Link_Init(&(pidSystem->OpticFlowYPosition), PID_CONTROLER_OPTICFLOW_Y_POSITION);	
	
	
	/*5.���ٶȿ��� ��ʼ��*/
	pid_Link_Init(&(pidSystem->HighAcc), PID_CONTROLER_HIGH_ACC);
	pid_Link_Init(&(pidSystem->LongitudeAcc), PID_CONTROLER_LONGITUDE_ACC);
	pid_Link_Init(&(pidSystem->LatitudeAcc), PID_CONTROLER_LATITUDE_ACC);	
	pid_Link_Init(&(pidSystem->OpticFlowXAcc), PID_CONTROLER_OPTICFLOW_X_ACC);
	pid_Link_Init(&(pidSystem->OpticFlowYAcc), PID_CONTROLER_OPTICFLOW_Y_ACC);		
}

/*PID���Ƶ�����ʼ��*/
void pid_Link_Init(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK)
{
	/*1.ƫ���޷���־*/
	pidLink->ERROR_LIMIT_FLAG = (PID_LIMIT_FLAG)(g_PidLinkInit[CONTROLER_LINK][0]);
	/*2.�����޷���־*/
	pidLink->INTEGRATE_LIMIT_FLAG = (PID_LIMIT_FLAG)(g_PidLinkInit[CONTROLER_LINK][1]);
	/*3.���ַ����־*/
	pidLink->INTEGRATE_SEPARATION_FLAG = (PID_LIMIT_FLAG)(g_PidLinkInit[CONTROLER_LINK][2]);
	/*4.����*/
	pidLink->expect = g_PidLinkInit[CONTROLER_LINK][3];
	/*5.����ֵ*/
	pidLink->feedback = g_PidLinkInit[CONTROLER_LINK][4];
	/*6.ƫ��*/
	pidLink->error = g_PidLinkInit[CONTROLER_LINK][5];
	/*7.�ϴ�ƫ��*/
	pidLink->lastError = g_PidLinkInit[CONTROLER_LINK][6];
	/*8.ƫ���޷�ֵ*/
	pidLink->errorMax = g_PidLinkInit[CONTROLER_LINK][7];
	/*9.���ַ���ƫ��ֵ*/
	pidLink->integrateSeparationError = g_PidLinkInit[CONTROLER_LINK][8];
	/*10.����ֵ*/
	pidLink->integrate = g_PidLinkInit[CONTROLER_LINK][9];
	/*11.�����޷�ֵ*/
	pidLink->integrateMax = g_PidLinkInit[CONTROLER_LINK][10];
	/*12.���Ʋ���kP*/
	pidLink->PID.kP = g_PidLinkInit[CONTROLER_LINK][11];
	/*13.���Ʋ���kI*/
	pidLink->PID.kI = g_PidLinkInit[CONTROLER_LINK][12];	
	/*14.���Ʋ���kD*/
	pidLink->PID.kD = g_PidLinkInit[CONTROLER_LINK][13];		
	/*15.�����������*/
	pidLink->controlOutput = g_PidLinkInit[CONTROLER_LINK][14];
	/*16.�ϴο����������*/
	pidLink->lastControlOutPut = g_PidLinkInit[CONTROLER_LINK][15];
	/*17.������������޷�*/
	pidLink->controlOutPutLimit = g_PidLinkInit[CONTROLER_LINK][16];
	
	/*PID ScaleĬ��1.0��*/
	pidLink->PidScale.kP = g_PidLinkInit[CONTROLER_LINK][17];
	pidLink->PidScale.kI = g_PidLinkInit[CONTROLER_LINK][18];
	pidLink->PidScale.kD = g_PidLinkInit[CONTROLER_LINK][19];
}

/*PIDͨ�ÿ��Ƽ���*/
fp32 pid_Control_General_Dp(PidLink *pidLink)
{
	fp32 controlDeltaT;
	
	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/
	
	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->error >= pidLink->errorMax)
		{
			pidLink->error = pidLink->errorMax;
		}
		
		if (pidLink->error <= -pidLink->errorMax)
		{
			pidLink->error = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->error) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->error + \
							  pidLink->integrate + \
							  pidLink->PID.kD * (pidLink->error - pidLink->lastError);

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);
}

/*PID Yaw�ǿ��Ƽ���*/
fp32 pid_Control_Yaw_Dp(PidLink *pidLink)
{
	fp32 controlDeltaT;
	
	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/	

	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	
	/*ƫ����ƫ�����180����*/
	if (pidLink->error < -180)
	{
		pidLink->error = pidLink->error + 360;
	}

	if (pidLink->error > 180)
	{
		pidLink->error = pidLink->error - 360;
	}	
	
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->error >= pidLink->errorMax)
		{
			pidLink->error = pidLink->errorMax;
		}
		
		if (pidLink->error <= -pidLink->errorMax)
		{
			pidLink->error = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->error) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT; /*kI����*/
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->error + \
							  pidLink->integrate + \
							  pidLink->PID.kD * (pidLink->error - pidLink->lastError);

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);	
}

/*PID DIV���Ƶ�ͨ�˲�*/
fp32 pid_Control_Div_LPF(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK)
{
	u8 i;
	fp32 controlDeltaT;

	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/	

	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	/*ԭʼ΢��*/
	pidLink->disErr = pidLink->error - pidLink->lastError;
	
	/******************************************/
	#if (PID_SYSTEM_CONFIG_USE_AVERAGE_FILTER == SYS_ENABLE)
	/*��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣*/		
	fp32 tempa, tempb, tempc, max, min;  /*���ڷ������˲�*/	

	tempa = pidLink->lastLastDisErrLPF; 
	tempb = pidLink->lastDisErrLPF;
	tempc = pidLink->disErr;
	max = tempa > tempb ? tempa : tempb;
	max = max > tempc ? max : tempc;
	min = tempa < tempb ? tempa : tempb;
	min = min < tempc ? min : tempc;
	if (tempa > min && tempa < max)    pidLink->disErr = tempa;
	if (tempb > min  && tempb < max )  pidLink->disErr = tempb;
	if (tempc > min  &&  tempc < max)  pidLink->disErr = tempc;
	pidLink->lastLastDisErrLPF = pidLink->lastDisErrLPF;
	pidLink->lastDisErrLPF     = pidLink->disErr;
	#endif
	/*****************************************/	
	
	/*���ֵ�ͨ��΢������Ƴ�������*/
	for (i = 4; i > 0; i--)
	{
		pidLink->disErrHistory[i] = pidLink->disErrHistory[i - 1];
	}
	
	/*����������*/
	pidLink->disErrHistory[0] = filter_Pid_Control_Device_LPF(pidLink->disErr, \
															  &(g_sFilterTarg.PidControlBuff[CONTROLER_LINK]), \
															  &(g_sFilterTarg.PidControlDivPara[0])); /*������˹��ͨ��õ���΢����,20hz*/
	
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->error >= pidLink->errorMax)
		{
			pidLink->error = pidLink->errorMax;
		}
		
		if (pidLink->error <= -pidLink->errorMax)
		{
			pidLink->error = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->error) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT; /*kI����*/
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->error + \
							  pidLink->integrate + \
							  pidLink->PID.kD * pidLink->disErrHistory[0]; /*΢������Դ�ڰ�����˹��ͨ�˲���*/

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);	
}

/*PID ERR���Ƶ�ͨ�˲�*/
fp32 pid_Control_Err_LPF(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK)
{
	fp32 controlDeltaT;

	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/	

	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	/*ԭʼ΢��*/
	pidLink->disErr = pidLink->error - pidLink->lastError;
	
	pidLink->lastErrLPF = pidLink->errLPF;
	pidLink->errLPF = filter_Pid_Control_Device_LPF(pidLink->error, \
													&(g_sFilterTarg.PidControlBuff[CONTROLER_LINK]), \
													&(g_sFilterTarg.PidControlErrPara[0])); /*������˹��ͨ��õ���΢����,5hz*/
	
	/*΢����*/
	pidLink->disErrLPF = pidLink->errLPF - pidLink->lastErrLPF;
	
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->errLPF >= pidLink->errorMax)
		{
			pidLink->errLPF = pidLink->errorMax;
		}
		
		if (pidLink->errLPF <= -pidLink->errorMax)
		{
			pidLink->errLPF = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->errLPF) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->errLPF * controlDeltaT; /*kI����*/
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->errLPF * controlDeltaT; /*kI����*/		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->errLPF + \
							  pidLink->integrate + \
							  pidLink->PID.kD * pidLink->disErrLPF; /*�Ѷ�ƫ���ͨ,�˴����ٶ�΢�������ͨ*/

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);		
}

/*PID DIV���Ƶ�ͨ�˲� Gyro*/
fp32 pid_Control_Div_LPF_Gyro(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK)
{
	u8 i;
	fp32 controlDeltaT;

	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/
	
	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�������ϴ�ƫ��*/
	pidLink->lastLastError = pidLink->lastError;
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	/*�����һ�β�����΢��*/
	pidLink->disErr = pidLink->error - pidLink->lastLastError;
	
	/******************************************/
	#if (PID_SYSTEM_CONFIG_USE_AVERAGE_FILTER == SYS_ENABLE)
	/*��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣*/		
	fp32 tempa, tempb, tempc, max, min;  /*���ڷ������˲�*/	
	
	tempa = pidLink->lastLastDisErrLPF; 
	tempb = pidLink->lastDisErrLPF;
	tempc = pidLink->disErr;
	max = tempa > tempb ? tempa : tempb;
	max = max > tempc ? max : tempc;
	min = tempa < tempb ? tempa : tempb;
	min = min < tempc ? min : tempc;
	if (tempa > min && tempa < max)    pidLink->disErr = tempa;
	if (tempb > min  && tempb < max )  pidLink->disErr = tempb;
	if (tempc > min  &&  tempc < max)  pidLink->disErr = tempc;
	pidLink->lastLastDisErrLPF = pidLink->lastDisErrLPF;
	pidLink->lastDisErrLPF     = pidLink->disErr;
	#endif
	/*****************************************/		
	
	/*���ֵ�ͨ��΢������Ƴ�������*/
	for (i = 4; i > 0; i--)
	{
		pidLink->disErrHistory[i] = pidLink->disErrHistory[i - 1];
	}
	
	/*����������*/
	pidLink->disErrHistory[0] = filter_Pid_Control_Device_LPF(pidLink->disErr, \
															  &(g_sFilterTarg.PidControlBuff[CONTROLER_LINK]), \
															  &(g_sFilterTarg.PidControlDivGyroPara[0])); /*������˹��ͨ��õ���΢����,30hz*/
	/*΢�����޷�*/
	pidLink->disErrHistory[0] = math_Constrain(pidLink->disErrHistory[0], 500, -500);
	
	/*����Ӧ΢�ֲ���*/
	pidLink->adaptableKd = pidLink->PID.kD * (1 + pidLink->disErrHistory[0] / 500);
	
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->error >= pidLink->errorMax)
		{
			pidLink->error = pidLink->errorMax;
		}
		
		if (pidLink->error <= -pidLink->errorMax)
		{
			pidLink->error = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->error) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT; /*kI����*/
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->error + \
							  pidLink->integrate + \
							  pidLink->adaptableKd * pidLink->disErrHistory[0]; /*΢������Դ�ڰ�����˹��ͨ�˲���*/

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);	
}

/*΢������PID������*/
fp32 pid_Control_Div_LPF_Differential_Forward(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK)
{
	u8 i;
	fp32 controlDeltaT;

	/*��ȡ��ȷִ�м��ʱ��*/
	get_Period_Execute_Time_Info(&(pidLink->PidControlDT));
	controlDeltaT = pidLink->PidControlDT.DeltaTime / 1000.0f; /*�����s*/	

	/*��С���Ƽ������*/
	if (controlDeltaT < 0.001f)
	{
		return 0;
	}
	
	/*1.ƫ�����*/
	/*�����ϴ�ƫ��*/
	pidLink->lastError = pidLink->error;	
	/*������ȥ�����õ�ƫ��*/
	pidLink->error = pidLink->expect - pidLink->feedback; 
	/*ֻ�Է����ź�΢��*/
	pidLink->disErr = -(pidLink->feedback - pidLink->lastFeedBack);
	/*��¼�ϴη���ֵ*/
	pidLink->lastFeedBack = pidLink->feedback;
	
	/******************************************/
	#if (PID_SYSTEM_CONFIG_USE_AVERAGE_FILTER == SYS_ENABLE)
	/*��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣*/		
	fp32 tempa, tempb, tempc, max, min;  /*���ڷ������˲�*/	
	
	tempa = pidLink->lastLastDisErrLPF; 
	tempb = pidLink->lastDisErrLPF;
	tempc = pidLink->disErr;
	max = tempa > tempb ? tempa : tempb;
	max = max > tempc ? max : tempc;
	min = tempa < tempb ? tempa : tempb;
	min = min < tempc ? min : tempc;
	if (tempa > min && tempa < max)    pidLink->disErr = tempa;
	if (tempb > min  && tempb < max )  pidLink->disErr = tempb;
	if (tempc > min  &&  tempc < max)  pidLink->disErr = tempc;
	pidLink->lastLastDisErrLPF = pidLink->lastDisErrLPF;
	pidLink->lastDisErrLPF     = pidLink->disErr;
	#endif
	/*****************************************/		
	
	/*���ֵ�ͨ��΢������Ƴ�������*/
	for (i = 4; i > 0; i--)
	{
		pidLink->disErrHistory[i] = pidLink->disErrHistory[i - 1];
	}
	
	/*����������*/
	pidLink->disErrHistory[0] = filter_Pid_Control_Device_LPF(pidLink->disErr, \
															  &(g_sFilterTarg.PidControlBuff[CONTROLER_LINK]), \
															  &(g_sFilterTarg.PidControlDivPara[0])); /*������˹��ͨ��õ���΢����,20hz*/
	
	/*ƫ���޷���־λ*/
	if (pidLink->ERROR_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->error >= pidLink->errorMax)
		{
			pidLink->error = pidLink->errorMax;
		}
		
		if (pidLink->error <= -pidLink->errorMax)
		{
			pidLink->error = -pidLink->errorMax;
		}
	}
	
	/*2.���ּ���*/
	/*���ַ����־λ*/
	if (pidLink->INTEGRATE_SEPARATION_FLAG == PID_LIMIT_ENABLE)
	{
		if (math_Abs(pidLink->error) <= pidLink->integrateSeparationError)
		{
			pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT; /*kI����*/
		}
	}
	else
	{
		pidLink->integrate += (pidLink->PidScale.kI * pidLink->PID.kI) * pidLink->error * controlDeltaT;		
	}
	
	/*3.�����޷�*/
	/*�������Ʒ��ȱ�־*/
	if (pidLink->INTEGRATE_LIMIT_FLAG == PID_LIMIT_ENABLE)
	{
		if (pidLink->integrate >= pidLink->integrateMax)
		{
			pidLink->integrate = pidLink->integrateMax;
		}
		
		if (pidLink->integrate <= -pidLink->integrateMax)
		{
			pidLink->integrate = -pidLink->integrateMax;
		}
	}
	
	/*4.���������*/
	/*���ֵ����*/
	pidLink->lastControlOutPut = pidLink->controlOutput;
	/*PID����*/
	pidLink->controlOutput = (pidLink->PidScale.kP * pidLink->PID.kP) * pidLink->error + \
							  pidLink->integrate + \
							  pidLink->PID.kD * pidLink->disErrHistory[0]; /*΢������Դ�ڰ�����˹��ͨ�˲���*/

	/*5.������޷�*/
	if (pidLink->controlOutput >= pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = pidLink->controlOutPutLimit;
	}
	
	if (pidLink->controlOutput <= -pidLink->controlOutPutLimit)
	{
		pidLink->controlOutput = -pidLink->controlOutPutLimit;
	}
	
	/*6.���������*/
	return (pidLink->controlOutput);	
}

/*PID�������ָ�λ*/
void pid_Link_Integrate_Reset(PidLink *pidLink)
{
	pidLink->integrate = 0.0f;
}

/*PIDˮƽ���ǰ���ָ�λ*/
void pid_Horizontal_Takeoff_Integrate_Reset(void)
{
	/*���ǰ������̬��/���ٶȻ�����*/
	pid_Link_Integrate_Reset(&(g_sPidSystem.PitchAngle));
	pid_Link_Integrate_Reset(&(g_sPidSystem.PitchGyro));	
	pid_Link_Integrate_Reset(&(g_sPidSystem.RollAngle));
	pid_Link_Integrate_Reset(&(g_sPidSystem.RollGyro));	
	pid_Link_Integrate_Reset(&(g_sPidSystem.YawAngle));
	pid_Link_Integrate_Reset(&(g_sPidSystem.YawGyro));

	/*λ�ÿ����ٶȻ�������*/
	pid_Link_Integrate_Reset(&(g_sPidSystem.LongitudeSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.LatitudeSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.OpticFlowYSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.OpticFlowXSpeed));	
}

/*PIDˮƽ���ƻ��ָ�λ*/
void pid_Horizontal_GPS_Ctrl_Integrate_Reset(void)
{
	/*λ�ÿ����ٶȻ�������*/
	/*gps*/
	pid_Link_Integrate_Reset(&(g_sPidSystem.LongitudeSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.LatitudeSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.LongitudePosition));
	pid_Link_Integrate_Reset(&(g_sPidSystem.LatitudePosition));		
}

/*PID��ֱ���ƻ��ָ�λ*/
void pid_Vertical_Ctrl_Integrate_Reset(void)
{
	pid_Link_Integrate_Reset(&(g_sPidSystem.HighAcc));	
	pid_Link_Integrate_Reset(&(g_sPidSystem.HighSpeed));
	pid_Link_Integrate_Reset(&(g_sPidSystem.HighPosition));
}	

/*PID�������������*/
SYS_RETSTATUS pid_parameter_save_or_reset(PidSystem *pidSystem)
{
	SYS_RETSTATUS retStatus = SYS_RET_SUCC;
	u8 i;
	u32 pidSaveAddr = 0;
	
	/*�ж��Ƿ����㽫����վ����PID����д��洢��*/
	if (pidSystem->PidSettingSystem.DO_STATUS == PID_PARAMETER_DO_SAVE)
	{
		/*1.���ٶȻ�������ֵ���洢BUFF*/ /*=== PID Group 1 ===*/
		pidSystem->PidSettingSystem.RWBuff[0].value  = pidSystem->PitchGyro.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[1].value  = pidSystem->PitchGyro.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[2].value  = pidSystem->PitchGyro.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[3].value  = pidSystem->RollGyro.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[4].value  = pidSystem->RollGyro.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[5].value  = pidSystem->RollGyro.PID.kD;

		pidSystem->PidSettingSystem.RWBuff[6].value  = pidSystem->YawGyro.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[7].value  = pidSystem->YawGyro.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[8].value  = pidSystem->YawGyro.PID.kD;

		/*2.�ǶȻ�������ֵ���洢BUFF*/ /*=== PID Group 2 ===*/
		pidSystem->PidSettingSystem.RWBuff[9].value  = pidSystem->PitchAngle.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[10].value = pidSystem->PitchAngle.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[11].value = pidSystem->PitchAngle.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[12].value = pidSystem->RollAngle.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[13].value = pidSystem->RollAngle.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[14].value = pidSystem->RollAngle.PID.kD;

		pidSystem->PidSettingSystem.RWBuff[15].value = pidSystem->YawAngle.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[16].value = pidSystem->YawAngle.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[17].value = pidSystem->YawAngle.PID.kD;

		/*3.�ٶȺ�λ�û�������ֵ���洢BUFF*/ /*=== PID Group 3 ===*/	
		pidSystem->PidSettingSystem.RWBuff[18].value = pidSystem->HighSpeed.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[19].value = pidSystem->HighSpeed.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[20].value = pidSystem->HighSpeed.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[21].value = pidSystem->HighPosition.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[22].value = pidSystem->HighPosition.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[23].value = pidSystem->HighPosition.PID.kD;
		
		/*GPS ˮƽλ�ú��ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[24].value = pidSystem->LatitudeSpeed.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[25].value = pidSystem->LatitudeSpeed.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[26].value = pidSystem->LatitudeSpeed.PID.kD;	
		
		pidSystem->PidSettingSystem.RWBuff[27].value = pidSystem->LatitudePosition.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[28].value = pidSystem->LatitudePosition.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[29].value = pidSystem->LatitudePosition.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[30].value = pidSystem->LongitudeSpeed.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[31].value = pidSystem->LongitudeSpeed.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[32].value = pidSystem->LongitudeSpeed.PID.kD;

		pidSystem->PidSettingSystem.RWBuff[33].value = pidSystem->LongitudePosition.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[34].value = pidSystem->LongitudePosition.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[35].value = pidSystem->LongitudePosition.PID.kD;

		/*���� ˮƽλ�ú��ٶ�*/		
		pidSystem->PidSettingSystem.RWBuff[36].value = pidSystem->OpticFlowXSpeed.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[37].value = pidSystem->OpticFlowXSpeed.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[38].value = pidSystem->OpticFlowXSpeed.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[39].value = pidSystem->OpticFlowXPosition.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[40].value = pidSystem->OpticFlowXPosition.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[41].value = pidSystem->OpticFlowXPosition.PID.kD;
		
		pidSystem->PidSettingSystem.RWBuff[42].value = pidSystem->OpticFlowYSpeed.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[43].value = pidSystem->OpticFlowYSpeed.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[44].value = pidSystem->OpticFlowYSpeed.PID.kD;

		pidSystem->PidSettingSystem.RWBuff[45].value = pidSystem->OpticFlowYPosition.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[46].value = pidSystem->OpticFlowYPosition.PID.kI;	
		pidSystem->PidSettingSystem.RWBuff[47].value = pidSystem->OpticFlowYPosition.PID.kD;
		
		/*4.���ٶȻ�������ֵ���洢BUFF*/	
		pidSystem->PidSettingSystem.RWBuff[48].value = pidSystem->HighAcc.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[49].value = pidSystem->HighAcc.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[50].value = pidSystem->HighAcc.PID.kD;
		
		/*GPS ˮƽ���ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[51].value = pidSystem->LongitudeAcc.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[52].value = pidSystem->LongitudeAcc.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[53].value = pidSystem->LongitudeAcc.PID.kD;	

		pidSystem->PidSettingSystem.RWBuff[54].value = pidSystem->LatitudeAcc.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[55].value = pidSystem->LatitudeAcc.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[56].value = pidSystem->LatitudeAcc.PID.kD;

		/*���� ˮƽ���ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[57].value = pidSystem->OpticFlowXAcc.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[58].value = pidSystem->OpticFlowXAcc.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[59].value = pidSystem->OpticFlowXAcc.PID.kD;

		pidSystem->PidSettingSystem.RWBuff[60].value = pidSystem->OpticFlowYAcc.PID.kP;
		pidSystem->PidSettingSystem.RWBuff[61].value = pidSystem->OpticFlowYAcc.PID.kI;
		pidSystem->PidSettingSystem.RWBuff[62].value = pidSystem->OpticFlowYAcc.PID.kD;
		
		
		/*�����²���BUFFд��洢��*/
		pidSaveAddr = AT24CXX_STOR_PID_PARA_ADDR;
		
		for (i = 0; i < PID_PARAMETER_STOR_BUFF_LENTH; i += 3) /*ÿ��дһ�����*/
		{
			bsp_AT24CXX_Write_3_S16Data(&g_sAt24cxx, pidSaveAddr, \
										 pidSystem->PidSettingSystem.RWBuff[i].value, \
										 pidSystem->PidSettingSystem.RWBuff[i + 1].value, \
										 pidSystem->PidSettingSystem.RWBuff[i + 2].value);
			
			/*�洢��ַƫ��:3 * 2Byte*/
			pidSaveAddr += sizeof(s16) * 3;
		}
		
		/*��Ǵ洢��������״̬Ϊ��Ч״̬*/
		pidSystem->PidSettingSystem.DO_STATUS = PID_PARAMETER_DO_NULL;
		
		/*���һ�ζ�д,�ű�ʾ����д������ɹ�*/
		retStatus = pid_Parameter_Read_And_Init(&g_sPidSystem);
		
		/*�²���д��ɹ��Զ�,�����λ����*/
		if (retStatus == SYS_RET_SUCC)
		{
			/*�ж�����*/
			__disable_irq();		
			
			/*ϵͳ��λ*/
			NVIC_SystemReset();
		}
	}
	else if (pidSystem->PidSettingSystem.DO_STATUS == PID_PARAMETER_DO_RESET)	/*����ΪĬ�ϲ���*/
	{
		/*PID����ϵͳ��ʼ��*/
		pid_System_Init(g_psPidSystem);	
		
		/*1.���ٶȻ�������ֵ���洢BUFF*/
		pidSystem->PidSettingSystem.RWBuff[0].value  = (s16)pidSystem->PitchGyro.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[1].value  = (s16)pidSystem->PitchGyro.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[2].value  = (s16)pidSystem->PitchGyro.PID.kD * 1000;	
		
		pidSystem->PidSettingSystem.RWBuff[3].value  = (s16)pidSystem->RollGyro.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[4].value  = (s16)pidSystem->RollGyro.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[5].value  = (s16)pidSystem->RollGyro.PID.kD * 1000;

		pidSystem->PidSettingSystem.RWBuff[6].value  = (s16)pidSystem->YawGyro.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[7].value  = (s16)pidSystem->YawGyro.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[8].value  = (s16)pidSystem->YawGyro.PID.kD * 1000;

		/*2.�ǶȻ�������ֵ���洢BUFF*/		
		pidSystem->PidSettingSystem.RWBuff[9].value  = (s16)pidSystem->PitchAngle.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[10].value = (s16)pidSystem->PitchAngle.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[11].value = (s16)pidSystem->PitchAngle.PID.kD * 1000;
		
		pidSystem->PidSettingSystem.RWBuff[12].value = (s16)pidSystem->RollAngle.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[13].value = (s16)pidSystem->RollAngle.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[14].value = (s16)pidSystem->RollAngle.PID.kD * 1000;

		pidSystem->PidSettingSystem.RWBuff[15].value = (s16)pidSystem->YawAngle.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[16].value = (s16)pidSystem->YawAngle.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[17].value = (s16)pidSystem->YawAngle.PID.kD * 1000;

		/*3.�ٶȺ�λ�û�������ֵ���洢BUFF*/	
		pidSystem->PidSettingSystem.RWBuff[18].value = (s16)pidSystem->HighSpeed.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[19].value = (s16)pidSystem->HighSpeed.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[20].value = (s16)pidSystem->HighSpeed.PID.kD * 1000;
		
		pidSystem->PidSettingSystem.RWBuff[21].value = (s16)pidSystem->HighPosition.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[22].value = (s16)pidSystem->HighPosition.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[23].value = (s16)pidSystem->HighPosition.PID.kD * 1000;

		/*GPS ˮƽλ��+�ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[24].value = (s16)pidSystem->LatitudeSpeed.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[25].value = (s16)pidSystem->LatitudeSpeed.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[26].value = (s16)pidSystem->LatitudeSpeed.PID.kD * 1000;	
		
		pidSystem->PidSettingSystem.RWBuff[27].value = (s16)pidSystem->LatitudePosition.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[28].value = (s16)pidSystem->LatitudePosition.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[29].value = (s16)pidSystem->LatitudePosition.PID.kD * 1000;
		
		pidSystem->PidSettingSystem.RWBuff[30].value = (s16)pidSystem->LongitudeSpeed.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[31].value = (s16)pidSystem->LongitudeSpeed.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[32].value = (s16)pidSystem->LongitudeSpeed.PID.kD * 1000;

		pidSystem->PidSettingSystem.RWBuff[33].value = (s16)pidSystem->LongitudePosition.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[34].value = (s16)pidSystem->LongitudePosition.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[35].value = (s16)pidSystem->LongitudePosition.PID.kD * 1000;
		
		/*����ˮƽλ��+�ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[36].value = (s16)pidSystem->OpticFlowXSpeed.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[37].value = (s16)pidSystem->OpticFlowXSpeed.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[38].value = (s16)pidSystem->OpticFlowXSpeed.PID.kD * 1000;
		
		pidSystem->PidSettingSystem.RWBuff[39].value = (s16)pidSystem->OpticFlowXPosition.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[40].value = (s16)pidSystem->OpticFlowXPosition.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[41].value = (s16)pidSystem->OpticFlowXPosition.PID.kD * 1000;
		
		pidSystem->PidSettingSystem.RWBuff[42].value = (s16)pidSystem->OpticFlowYSpeed.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[43].value = (s16)pidSystem->OpticFlowYSpeed.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[44].value = (s16)pidSystem->OpticFlowYSpeed.PID.kD * 1000;

		pidSystem->PidSettingSystem.RWBuff[45].value = (s16)pidSystem->OpticFlowYPosition.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[46].value = (s16)pidSystem->OpticFlowYPosition.PID.kI * 1000;	
		pidSystem->PidSettingSystem.RWBuff[47].value = (s16)pidSystem->OpticFlowYPosition.PID.kD * 1000;		
		
		/*4.���ٶȻ�������ֵ���洢BUFF*/	
		pidSystem->PidSettingSystem.RWBuff[48].value = (s16)pidSystem->HighAcc.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[49].value = (s16)pidSystem->HighAcc.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[50].value = (s16)pidSystem->HighAcc.PID.kD * 1000;

		/*GPS ˮƽ���ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[51].value = (s16)pidSystem->LongitudeAcc.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[52].value = (s16)pidSystem->LongitudeAcc.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[53].value = (s16)pidSystem->LongitudeAcc.PID.kD * 1000;	

		pidSystem->PidSettingSystem.RWBuff[54].value = (s16)pidSystem->LatitudeAcc.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[55].value = (s16)pidSystem->LatitudeAcc.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[56].value = (s16)pidSystem->LatitudeAcc.PID.kD * 1000;

		/*���� ˮƽ���ٶ�*/
		pidSystem->PidSettingSystem.RWBuff[57].value = (s16)pidSystem->OpticFlowXAcc.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[58].value = (s16)pidSystem->OpticFlowXAcc.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[59].value = (s16)pidSystem->OpticFlowXAcc.PID.kD * 1000;

		pidSystem->PidSettingSystem.RWBuff[60].value = (s16)pidSystem->OpticFlowYAcc.PID.kP * 1000;
		pidSystem->PidSettingSystem.RWBuff[61].value = (s16)pidSystem->OpticFlowYAcc.PID.kI * 1000;
		pidSystem->PidSettingSystem.RWBuff[62].value = (s16)pidSystem->OpticFlowYAcc.PID.kD * 1000;
		
		/*����ʼ������BUFFд��洢��*/
		pidSaveAddr = AT24CXX_STOR_PID_PARA_ADDR;
		
		for (i = 0; i < PID_PARAMETER_STOR_BUFF_LENTH; i += 3)
		{
			bsp_AT24CXX_Write_3_FloatData(&g_sAt24cxx, pidSaveAddr, \
										  pidSystem->PidSettingSystem.RWBuff[i].value, \
										  pidSystem->PidSettingSystem.RWBuff[i + 1].value, \
										  pidSystem->PidSettingSystem.RWBuff[i + 2].value);
			
			/*�洢��ַƫ��:3 * 2Byte*/
			pidSaveAddr += sizeof(s16) * 3;
		}
		
		/*��Ǵ洢����״̬��Ϊ��Ч״̬*/
		pidSystem->PidSettingSystem.DO_STATUS = PID_PARAMETER_DO_NULL;

		/*���һ�ζ�д,�ű�ʾ����д������ɹ�*/
		retStatus = pid_Parameter_Read_And_Init(&g_sPidSystem);		
	}
	
	return retStatus;
}

/*��ȡ�洢���ڲ�PID����(��ʼ��)*/
SYS_RETSTATUS pid_Parameter_Read_And_Init(PidSystem *pidSystem)
{
	SYS_RETSTATUS readStatus[PID_PARAMETER_SETTING_NBR] = {SYS_RET_SUCC};
	u8 i, j = 0;
	u16 pidSaveAddr = 0;
	
	/*PID�����ڴ洢���е���ʼ��ַ*/
	pidSaveAddr = AT24CXX_STOR_PID_PARA_ADDR;
	
	/*���PID�����Ķ�дBUFF*/
	memset(pidSystem->PidSettingSystem.RWBuff, 0, PID_PARAMETER_STOR_BUFF_LENTH);
	
	for (i = 0; i < PID_PARAMETER_STOR_BUFF_LENTH; i += 3)
	{
		readStatus[j++] = bsp_AT24CXX_Read_3_S16Data(&g_sAt24cxx, pidSaveAddr, 
													 &(pidSystem->PidSettingSystem.RWBuff[i].value), \
												     &(pidSystem->PidSettingSystem.RWBuff[i + 1].value), \
												     &(pidSystem->PidSettingSystem.RWBuff[i + 2]).value);

		/*��ȡ��ַƫ��:3 * 2Byte*/
		pidSaveAddr += sizeof(s16) * 3;		
		
		/*������ʾPID���Ʋ���*/
		#if (HCI_OLED_SHOW_PID_CONTROL_PARA_WHEN_BOOT == SYS_ENABLE)
		/*OLED����ʾPID��ȡ���*/
		hci_Show_Control_System_Parameter(readStatus[j - 1], (fp32)pidSystem->PidSettingSystem.RWBuff[i].value * 0.001f, \
															 (fp32)pidSystem->PidSettingSystem.RWBuff[i + 1].value * 0.001f, \
															 (fp32)pidSystem->PidSettingSystem.RWBuff[i + 2].value * 0.001f, \
														     (PID_CONTROLER_LINK)(j - 1), 2000);
		#endif
		
		/*��ʾPID���Ʋ���*/
		#if (HCI_OLED_SHOW_PID_CONTROL_PARA_WHEN_BOOT != SYS_ENABLE)
		if (g_psAircraftStatus->PLATFORM_INIT_STATUS == PROGRAME_INIT_FINISH) /*���й����в�����ʾ*/
		{
			/*OLED����ʾPID��ȡ���*/
			hci_Show_Control_System_Parameter(readStatus[j - 1], (fp32)pidSystem->PidSettingSystem.RWBuff[i].value * 0.001f, \
																 (fp32)pidSystem->PidSettingSystem.RWBuff[i + 1].value * 0.001f, \
																 (fp32)pidSystem->PidSettingSystem.RWBuff[i + 2].value * 0.001f, \
																 (PID_CONTROLER_LINK)(j - 1), 2000);
		}
		#endif
	}
	
	/*���ÿ��PID��������״̬*/
	for (i = 0; i < j; i++)
	{	
		if (readStatus[i] != SYS_RET_SUCC)
		{
			/*����ʧ�ܾ���Ĭ�ϲ������г�ʼ��*/
			pid_System_Init(pidSystem);
			
			/*PID������Ч�Ա��Ϊ��Ч*/
			pidSystem->PidSettingSystem.AVA_STATUS = PID_PARAMETER_DISAVA;
			
			return SYS_RET_FAIL;	/*���س�ʼ��ʧ��*/
		}
	}
	
	/*PID���������ɹ�,��ʼ��PID�������*/
	pid_System_Init(pidSystem);  /*����PID���������������������ʼ��*/
	
	/*1.���ٶȻ�������ֵ���洢BUFF*/
	pidSystem->PitchGyro.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[0].value * 0.001f; 
	pidSystem->PitchGyro.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[1].value * 0.001f; 
	pidSystem->PitchGyro.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[2].value * 0.001f; 
	                            
	pidSystem->RollGyro.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[3].value * 0.001f;
	pidSystem->RollGyro.PID.kI = (fp32) pidSystem->PidSettingSystem.RWBuff[4].value * 0.001f;
	pidSystem->RollGyro.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[5].value * 0.001f;
                                   
	pidSystem->YawGyro.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[6].value * 0.001f; 
	pidSystem->YawGyro.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[7].value * 0.001f; 
	pidSystem->YawGyro.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[8].value * 0.001f; 

	/*2.�ǶȻ�������ֵ���洢BUFF*/		
	pidSystem->PitchAngle.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[9].value * 0.001f; 
	pidSystem->PitchAngle.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[10].value * 0.001f; 	
	pidSystem->PitchAngle.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[11].value * 0.001f; 
                                           
	pidSystem->RollAngle.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[12].value * 0.001f; 
	pidSystem->RollAngle.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[13].value * 0.001f; 
	pidSystem->RollAngle.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[14].value * 0.001f; 
                                           
	pidSystem->YawAngle.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[15].value * 0.001f; 
	pidSystem->YawAngle.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[16].value * 0.001f; 
	pidSystem->YawAngle.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[17].value * 0.001f; 

	/*3.�ٶȺ�λ�û�������ֵ���洢BUFF*/	
	pidSystem->HighSpeed.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[18].value * 0.001f; 
	pidSystem->HighSpeed.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[19].value * 0.001f; 
	pidSystem->HighSpeed.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[20].value * 0.001f; 
	
	pidSystem->HighPosition.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[21].value * 0.001f; 
	pidSystem->HighPosition.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[22].value * 0.001f; 
	pidSystem->HighPosition.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[23].value * 0.001f; 
    
	/*GPS ˮƽ�ٶ�+λ��*/	
	pidSystem->LatitudeSpeed.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[24].value * 0.001f; 
	pidSystem->LatitudeSpeed.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[25].value * 0.001f; 
	pidSystem->LatitudeSpeed.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[26].value * 0.001f; 
	
	pidSystem->LatitudePosition.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[27].value * 0.001f; 
	pidSystem->LatitudePosition.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[28].value * 0.001f; 
	pidSystem->LatitudePosition.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[29].value * 0.001f; 
	                                   
	pidSystem->LongitudeSpeed.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[30].value * 0.001f; 
	pidSystem->LongitudeSpeed.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[31].value * 0.001f; 
	pidSystem->LongitudeSpeed.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[32].value * 0.001f; 
       
	pidSystem->LongitudePosition.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[33].value * 0.001f;
	pidSystem->LongitudePosition.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[34].value * 0.001f;
	pidSystem->LongitudePosition.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[35].value * 0.001f;
	
	/*���� ˮƽ�ٶ�+λ��*/
	pidSystem->OpticFlowXSpeed.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[36].value * 0.001f;
	pidSystem->OpticFlowXSpeed.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[37].value * 0.001f;
	pidSystem->OpticFlowXSpeed.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[38].value * 0.001f;
	
	pidSystem->OpticFlowXPosition.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[39].value * 0.001f;
	pidSystem->OpticFlowXPosition.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[40].value * 0.001f;
	pidSystem->OpticFlowXPosition.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[41].value * 0.001f;
	
	pidSystem->OpticFlowYSpeed.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[42].value * 0.001f;
	pidSystem->OpticFlowYSpeed.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[43].value * 0.001f;
	pidSystem->OpticFlowYSpeed.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[44].value * 0.001f;
	
	pidSystem->OpticFlowYPosition.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[45].value * 0.001f;
	pidSystem->OpticFlowYPosition.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[46].value * 0.001f;
	pidSystem->OpticFlowYPosition.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[47].value * 0.001f;

	/*4.���ٶȻ�������ֵ���洢BUFF*/	
	pidSystem->HighAcc.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[48].value * 0.001f;
	pidSystem->HighAcc.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[49].value * 0.001f; 
	pidSystem->HighAcc.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[50].value * 0.001f; 

	/*GPS ˮƽ���ٶ�*/
	pidSystem->LongitudeAcc.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[51].value * 0.001f;
	pidSystem->LongitudeAcc.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[52].value * 0.001f;
	pidSystem->LongitudeAcc.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[53].value * 0.001f;	

	pidSystem->LatitudeAcc.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[54].value * 0.001f;
	pidSystem->LatitudeAcc.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[55].value * 0.001f;
	pidSystem->LatitudeAcc.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[56].value * 0.001f;

	/*���� ˮƽ���ٶ�*/
	pidSystem->OpticFlowXAcc.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[57].value * 0.001f;
	pidSystem->OpticFlowXAcc.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[58].value * 0.001f;
	pidSystem->OpticFlowXAcc.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[59].value * 0.001f;

	pidSystem->OpticFlowYAcc.PID.kP = (fp32)pidSystem->PidSettingSystem.RWBuff[60].value * 0.001f;
	pidSystem->OpticFlowYAcc.PID.kI = (fp32)pidSystem->PidSettingSystem.RWBuff[61].value * 0.001f;
	pidSystem->OpticFlowYAcc.PID.kD = (fp32)pidSystem->PidSettingSystem.RWBuff[62].value * 0.001f;	
	
	/*PID������Ч�Ա��Ϊ��Ч*/
	pidSystem->PidSettingSystem.AVA_STATUS = PID_PARAMETER_AVA;	
	
	return SYS_RET_SUCC;	/*���س�ʼ���ɹ�*/
}
