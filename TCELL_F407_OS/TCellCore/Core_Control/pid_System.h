#ifndef _PID_SYSTEM_H_
#define _PID_SYSTEM_H_

#include "sys_Platform.h"
#include "filter_DataProcess.h"
#include "bsp_BoardLib.h"

#define PID_PARAMETER_SETTING_NBR	    (21)
#define PID_PARAMETER_STOR_BUFF_LENTH	(sizeof(Vector3s_PID) * PID_PARAMETER_SETTING_NBR / sizeof(StorS16Data))

/*PID�����ĵ���ִ��״̬*/
typedef enum
{
	
	PID_PARAMETER_DO_SAVE  = 0,		/*����������洢��*/
	PID_PARAMETER_DO_RESET = 1,		/*����ΪĬ�ϲ���*/		
	PID_PARAMETER_DO_NULL  = 0xFF,
}PID_PARAMETER_DO_STATUS;

/*PID��������Ч״̬*/
typedef enum
{
	PID_PARAMETER_DISAVA = 0,		/*PID������Ч*/	
	PID_PARAMETER_AVA    = 1,		/*PID������Ч*/
}PID_PARAMETER_AVA_STATUS;

/*PID��������״̬*/
typedef enum
{
	PID_PARA_DOWNLOAD_FINISH = 0, /*�����������*/
	PID_PARA_DOWNLOAD_START  = 1, /*�������ؿ�ʼ*/
}PID_PARA_DOWNLOAD_STATUS; 

/*PID��������ϵͳ*/
typedef struct
{
	StorS16Data 		      		  RWBuff[PID_PARAMETER_STOR_BUFF_LENTH]; /*���ڲ����Ķ�дBuff*/
	volatile PID_PARAMETER_DO_STATUS  DO_STATUS;							 /*PID�����ĵ���ִ��״̬*/
    volatile PID_PARAMETER_AVA_STATUS AVA_STATUS;						     /*PID��������Ч��״̬*/
	volatile PID_PARA_DOWNLOAD_STATUS PARA_DOWNLOAD_STATUS; 			     /*PID��������״̬*/
}PidParameterSettingSystem;

/*PID���Ʊ�־*/
typedef enum
{
	PID_LIMIT_DISABLE = 0,	/*��ֹ����*/
	PID_LIMIT_ENABLE  = 1,	/*��������*/
}PID_LIMIT_FLAG;

/*PID���ƻ������б�*/
typedef enum
{
	 /****** �ǶȺͽ��ٶȿ��� ******/	
     PID_CONTROLER_PITCH_GYRO           = 0,	
	 PID_CONTROLER_ROLL_GYRO            = 1,	
	 PID_CONTROLER_YAW_GYRO    		    = 2,
     PID_CONTROLER_PITCH_ANGLE          = 3,
     PID_CONTROLER_ROLL_ANGLE           = 4,
     PID_CONTROLER_YAW_ANGLE            = 5,
									    
	 /****** λ�ú��ٶȿ��� ******/	    
     PID_CONTROLER_HIGH_SPEED           = 6,		
     PID_CONTROLER_HIGH_POSITION        = 7,
     PID_CONTROLER_LATITUDE_SPEED       = 8,	
     PID_CONTROLER_LATITUDE_POSITION    = 9,	
     PID_CONTROLER_LONGITUDE_SPEED      = 10,	
     PID_CONTROLER_LONGITUDE_POSITION   = 11,	
	 /****** ����λ�ú��ٶȿ��� ******/
	 PID_CONTROLER_OPTICFLOW_X_SPEED    = 12,	
     PID_CONTROLER_OPTICFLOW_X_POSITION = 13,		
	 PID_CONTROLER_OPTICFLOW_Y_SPEED    = 14,	
     PID_CONTROLER_OPTICFLOW_Y_POSITION = 15,	

	 /****** ���ٶȿ��� ******/	
     PID_CONTROLER_HIGH_ACC			    = 16,
     PID_CONTROLER_LONGITUDE_ACC 	    = 17,
     PID_CONTROLER_LATITUDE_ACC         = 18,
     PID_CONTROLER_OPTICFLOW_X_ACC 	    = 19,
     PID_CONTROLER_OPTICFLOW_Y_ACC      = 20, 	 
}PID_CONTROLER_LINK;

/*PID���ƻ�*/
typedef struct
{
	PID_LIMIT_FLAG 			ERROR_LIMIT_FLAG;			 /*ƫ���޷���־*/
	PID_LIMIT_FLAG 			INTEGRATE_LIMIT_FLAG;		 /*�����޷���־*/
	PID_LIMIT_FLAG 			INTEGRATE_SEPARATION_FLAG;	 /*���ַ����־*/
	fp32		   			expect;						 /*����*/
	fp32		   			feedback;					 /*����*/
	fp32 		   			error;						 /*ƫ��*/
    fp32 		   			lastError;					 /*�ϴ�ƫ��*/
    fp32 		   			errorMax;					 /*ƫ���޷�ֵ*/
    fp32 		   			integrateSeparationError;	 /*���ַ���ƫ��ֵ*/
    fp32 		   			integrate;					 /*����ֵ*/
    fp32 		   			integrateMax;				 /*�����޷�ֵ*/
    Vector3f_PID   			PID;					     /*���Ʋ���kP,kI,kD*/
	Vector3f_PID			PidScale;					 /*������ʱ����PID��3��ϵ������*/
    fp32 		   			controlOutput;				 /*�����������*/
    fp32 		   			lastControlOutPut;			 /*�ϴο����������*/
    fp32		   			controlOutPutLimit;			 /*����޷�*/
    /***************************************/	
	fp32 					lastLastError;				 /*���ϴ�ƫ��*/
	fp32 				    adaptableKd;				 /*����Ӧ΢�ֲ���*/
    fp32 		  			lastFeedBack;				 /*�ϴη���ֵ*/
    fp32 		  		    disErr;				 		 /*���΢����*/
    fp32 		   			disErrHistory[5];		 	 /*��ʷ���΢����*/
    /***************************************/		
    fp32 		   			errLPF;						 /*����ͨ�˲���*/		 
    fp32 		   			lastErrLPF;					 /*�ϴ�����ͨ�˲���*/
    fp32 		   			disErrLPF;					 /*���΢�ֵ�ͨ�˲���*/
    fp32 		   			lastDisErrLPF;				 /*�ϴ����΢�ֵ�ͨ�˲���*/	
	fp32				    lastLastDisErrLPF;			 /*���ϴ����΢�ֵ�ͨ�˲���*/
    PeriodExecuteTime	    PidControlDT;				 /*PID���Ƽ��ʱ��*/
}PidLink;

/*PID����ϵͳ*/
typedef struct
{	
	 /****** �ǶȺͽ��ٶȿ��� ******/
     PidLink 				   PitchGyro;			/*pitch���ٶȻ�*/	
     PidLink 				   RollGyro;            /*roll���ٶȻ�*/	
     PidLink 				   YawGyro;             /*yaw���ٶȻ�*/	
     PidLink 				   PitchAngle;			/*pitch�ǶȻ�*/
     PidLink 				   RollAngle;           /*roll�ǶȻ�*/
     PidLink 				   YawAngle;            /*yaw�ǶȻ�*/
		
	 /****** λ�ú��ٶȿ��� ******/	
     PidLink 				   HighSpeed;			/*high(��ֱ����)�ٶȻ�*/	
     PidLink 				   HighPosition;		/*high(��ֱ����)λ�û�*/
     PidLink 				   LatitudeSpeed;		/*γ��(ˮƽY����)�ٶȻ�*/
     PidLink 				   LatitudePosition;	/*γ��(ˮƽY����)λ�û�*/
     PidLink 				   LongitudeSpeed;		/*����(ˮƽX����)�ٶȻ�*/	
     PidLink 				   LongitudePosition;	/*����(ˮƽX����)λ�û�*/
     PidLink 				   OpticFlowXSpeed;		/*����(ˮƽX����)�ٶȻ�*/	
     PidLink 				   OpticFlowXPosition;	/*����(ˮƽX����)λ�û�*/
     PidLink 				   OpticFlowYSpeed;		/*����(ˮƽY����)�ٶȻ�*/		
     PidLink 				   OpticFlowYPosition;	/*����(ˮƽY����)λ�û�*/
	
	 /****** ���ٶȿ��� ******/	
     PidLink 				   HighAcc;				/*high(��ֱ����)���ٶȻ�*/
     PidLink 				   LongitudeAcc;		/*����(ˮƽX����)���ٶȻ�*/
     PidLink 				   LatitudeAcc;			/*γ��(ˮƽY����)���ٶȻ�*/
     PidLink 				   OpticFlowXAcc;		/*����(ˮƽX����)���ٶȻ�*/
     PidLink 				   OpticFlowYAcc;		/*����(ˮƽY����)���ٶȻ�*/	 
	
	 /****** PID���������洢 ******/
	 PidParameterSettingSystem PidSettingSystem;	/*PID��������ϵͳ*/
}PidSystem;

void pid_System_Init(PidSystem *pidSystem);			/*PID����ϵͳ��ʼ��*/
void pid_Link_Init(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK); /*PID���Ƶ�����ʼ��*/
fp32 pid_Control_General_Dp(PidLink *pidLink);		/*PIDͨ�ÿ��Ƽ���*/
fp32 pid_Control_Yaw_Dp(PidLink *pidLink);  		/*PID Yaw�ǿ��Ƽ���*/

fp32 pid_Control_Div_LPF(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK); 		 /*PID DIV���Ƶ�ͨ�˲�*/
fp32 pid_Control_Err_LPF(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK);  	 /*PID ERR���Ƶ�ͨ�˲�*/

fp32 pid_Control_Div_LPF_Gyro(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK);  /*PID DIV���Ƶ�ͨ�˲� Gyro*/
fp32 pid_Control_Div_LPF_Differential_Forward(PidLink *pidLink, PID_CONTROLER_LINK CONTROLER_LINK);  /*΢������PID������*/

void pid_Link_Integrate_Reset(PidLink *pidLink);	/*PID�������ָ�λ*/
void pid_Horizontal_Takeoff_Integrate_Reset(void);	/*PIDˮƽ���ǰ���ָ�λ*/
void pid_Horizontal_GPS_Ctrl_Integrate_Reset(void);	/*PIDˮƽ���ƻ��ָ�λ*/
void pid_Vertical_Ctrl_Integrate_Reset(void);		/*PID��ֱ���ƻ��ָ�λ*/

SYS_RETSTATUS pid_parameter_save_or_reset(PidSystem *pidSystem);/*PID�������������(���ߵ���)*/
SYS_RETSTATUS pid_Parameter_Read_And_Init(PidSystem *pidSystem);/*��ȡ�洢���ڲ�PID����(��ʼ��)*/

extern PidSystem g_sPidSystem; 
extern PidSystem *g_psPidSystem;

#endif
