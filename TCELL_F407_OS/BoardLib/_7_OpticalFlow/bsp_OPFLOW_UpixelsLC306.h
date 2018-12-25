#ifndef _BSP_OPFLOW_UPIXELSLC306_H_
#define _BSP_OPFLOW_UPIXELSLC306_H_

#include "bsp_OPFLOW_UPIXELSLC306_CMD.h"
#include "sys_Platform.h"
#include "msp_UART.h"
#include "sys_McuInit.h"

#define OPFLOW_UPIXELSLC306_INIT_CMC_PERIOD_TICKS_MS    (10)

#define OPFLOW_UPIXELSLC306_TX_BUFF_LENTH				(6)
#define OPFLOW_UPIXELSLC306_RX_BUFF_LENTH				(14)
#define OPFLOW_UPIXELSLC306_TX_PAYLOAD_LENTH  			(4)
#define OPFLOW_UPIXELSLC306_RX_PAYLOAD_LENTH  			(10)
#define OPFLOW_UPIXELSLC306_DATA_BUFF_LENTH				(14)

/*��ʼ����Ӧ��ʱ,��������ָ�����*/
#define OPFLOW_UPIXELSLC306_RESEND_CMD_MAX_TIMES		(5)

/*�������ݿ�����*/
typedef enum
{
	OPFLOW_UPIXELSLC306_DATA_DISAVA = 0x00, /*�������ݲ�����*/
	OPFLOW_UPIXELSLC306_DATA_AVA    = 0xF5,	/*�������ݿ���*/
}OPFLOW_UPIXELSLC306_DATA_STATUS;

/*������ʼ��Ӧ��״̬*/
typedef enum
{
	OPFLOW_UPIXELSLC306_RESPONSE_FAIL = 0, /*����δӦ��*/	
	OPFLOW_UPIXELSLC306_RESPONSE_SUCC = 1, /*����Ӧ��*/	
}OPFLOW_UPIXELSLC306_RESPONSE_STATUS;

/*������ʼ��״̬*/
typedef enum
{
	OPFLOW_UPIXELSLC306_INIT_START  = 0,  /*������ʼ����ʼ*/	
	OPFLOW_UPIXELSLC306_INIT_FINISH = 1, /*������ʼ������*/		
	OPFLOW_UPIXELSLC306_INIT_NULL   = 0xff,
}OPFLOW_UPIXELSLC306_INIT_STATUS;

/*�������ݸ���״̬*/
typedef enum
{
	OPFLOW_UPIXELSLC306_UPDATE_FAIL = 0, /*δ����*/
	OPFLOW_UPIXELSLC306_UPDATE_SUCC  = 1, /*�Ѹ���*/
}OPFLOW_UPIXELSLC306_UPDATE_STATUS;

/*UpixelsLC306 ����ָ��֡*/
typedef struct
{
	u8 command;	/*ָ��*/
	u8 payload[OPFLOW_UPIXELSLC306_TX_PAYLOAD_LENTH]; /*ָ������buff*/
	u8 XOR;		/*buff�������*/
}OpFlowUpixelsLC306TxFrame;

/*UpixelsLC306 ��������֡*/
typedef struct
{
	u8 head;									      /*֡ͷ*/
	u8 lenth;										  /*���ݰ��ֽ���(�̶�ֵ 0x0A)*/
	u8 payload[OPFLOW_UPIXELSLC306_RX_PAYLOAD_LENTH]; /*ָ������buff*/
	u8 XOR;											  /*3-12�ֽ����У��λ*/
	u8 tail;										  /*֡β*/
}OpFlowUpixelsLC306RxFrame;


/*UpixelsLC306 ���ݽ���*/
typedef struct
{
	s16 							xIntegral;			 /*X ���ص��ۼ�ʱ���ڵ��ۼ�λ��,(radians*10000)[���� 10000 ���Ը߶�(mm)��Ϊʵ��λ��(mm)]*/
	s16 							yIntegral;			 /*Y ���ص��ۼ�ʱ���ڵ��ۼ�λ��,(radians*10000)[���� 10000 ���Ը߶�(mm)��Ϊʵ��λ��(mm)]*/
	u16 							integrationTimespan; /*��һ�η��͹������ݵ����η��͹������ݵ��ۼ�ʱ��(us)*/
	u16 							groundDistance;		 /*Ԥ����ؾ���,Ĭ��Ϊ 999(0x03E7)*/
	OPFLOW_UPIXELSLC306_DATA_STATUS DATA_STATUS;		 /*�������ݿ���״̬*/
	u8 							    version;			 /*����ģ��汾��*/
}OpFlowUpixelsLC306DataFrame;


/*���ݴ�λ��ľ���*/
typedef struct
{
	u16 backByteNbr;  	/*��ȡλ��������ֽ���*/
	u16 frontByteNbr; 	/*��ȡλ����ǰ���ֽ���*/
	u8 *pStart;			/*�������ڴ��е���ʼ��ַ*/
	u8 *pStitch;		/*ƴ�ӵ�*/
	u8 *pHead;			/*������Head*/
	u8 *fixLenth;		/*�̶����ݳ���*/	
}OpFlowUpixelsLC306DataReform;

/*��ʼ��Ӧ��ȶ�*/
typedef struct
{
	u8 *pStart;			/*�������ڴ��е���ʼ��ַ*/
	u8 *pCmpData1;		/*��1���ȶ�����*/	
	u8 *pCmpData2;		/*��2���ȶ�����*/	
	u8 *pCmpData3;		/*��3���ȶ�����*/	
}OpFlowUpixelsLC306ResponseCmpReform;

typedef struct
{
	MSP_Uart 					            	 *UartMaster;  		/*����*/
	volatile OPFLOW_UPIXELSLC306_RESPONSE_STATUS RESPONSE_STATUS;	/*��ʼ��Ӧ��״̬*/
	volatile OPFLOW_UPIXELSLC306_INIT_STATUS 	 INIT_STATUS;		/*��ʼ����ʼ��״̬*/
	volatile OPFLOW_UPIXELSLC306_UPDATE_STATUS   UPDATE_STATUS;		/*�������ݸ���״̬*/
	OpFlowUpixelsLC306DataFrame 		         OpFlowData;	    /*��������*/
}OpFlowUpixelsLC306;

/*Upixels LC306������ʼ��:19200*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Init(OpFlowUpixelsLC306 *opFlowUpixelsLC306);

/*Upixels LC306Ӧ�����ݽ���*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Response_Data_Parse(u8 *rspBuff);

/*Upixels LC306λ�����ݽ���*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_POS_Data_Parse(OpFlowUpixelsLC306 *opFlowUpixelsLC306, OpFlowUpixelsLC306RxFrame *rxFrame, u8 *rxBuff);

/*Upixels LC306 ����ָ��*/
u8 bsp_OPFLOW_UpixelsLC306_Make_Command(OpFlowUpixelsLC306TxFrame *txFrame, OPFLOW_UPIXELSLC306_CFG_CMD CMD_TYPE, u8 *payload, u8 cmdLen, u8 *sendBuff);

/*����ƫ������*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Reform_Data(OpFlowUpixelsLC306DataReform *DataReform, u8 *rxBuff);

/*�ӻ�Ӧ���ѯ*/
SYS_RETSTATUS bsp_OPFLOW_UpixelsLC306_Slave_Response(OpFlowUpixelsLC306ResponseCmpReform *ResponseCmp, u8 cmpBuff[]);


extern OpFlowUpixelsLC306 g_sOpFlowUpixelsLC306;
extern OpFlowUpixelsLC306TxFrame g_sOpFlowUpixelsLC306TxFrame;				/*OpFlowUpixelsLC306 ����Э��֡*/
extern OpFlowUpixelsLC306RxFrame g_sOpFlowUpixelsLC306RxFrame;				/*OpFlowUpixelsLC306 ����Э��֡*/
extern u8 g_OpFlowUpixelsLC306TxBuff[OPFLOW_UPIXELSLC306_TX_BUFF_LENTH];	/*���ڷ���buff*/
extern u8 g_OpFlowUpixelsLC306RxBuff[OPFLOW_UPIXELSLC306_RX_BUFF_LENTH];	/*���ڽ���buff*/

#endif
