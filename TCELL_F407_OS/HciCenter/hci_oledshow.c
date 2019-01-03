#include "hci_oledshow.h"
#include "period_Execute.h"
#include "bsp_BoardLib.h"
#include "ahrs_Caculation.h"
#include "sins_Strapdown.h"
#include "status_Aircraft.h"
#include "attitude_Aircraft.h"
#include "remot_DataAnaly.h"
#include "control_Aircraft.h"
#include "safe_Operation.h"

#ifdef PLATFORM_RTOS__RT_THREAD
#include "sys_OsTask.h"
#endif

/*========= ��ʼ���˻����� ========= */
/*��ʾ����������logo*/
void hci_Show_AircraftLogoHoldMs(u32 holdMs)
{
	/*��ʾLOGO*/
	bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 0, 128, 8, (u8 *)&g_LogoFlashWolves128X64);
	
	/*����ʱ��*/
	sys_DelayMs(holdMs);
	
	/*����*/
	bsp_OLED0_96_Clear(&g_sOled0_96);
}

/*��ʾ��ʼ������*/
void hci_Show_InitRateOfProgress(u8 totalModuleNbr, HCI_SHOW_INIT_TARG INIT_TARG, SYS_RETERR INIT_STATUS)
{
	static u8 moduleInitSuccCnt = 0;
	static u8 progressBarPer = 0;
	static u8 showOnceFlag = 0;
	static u8 xHanziStart = 5;
	static u8 xLogStart = 0;
	u8 xNumberStart = 0;
	static u8 xProgressBarBlock = 20;
	u8 numberLen = 0;
	
	/*��ʼ��ģ��ɹ���1*/
	if (INIT_STATUS == SYS_RETERR_0ZR)
	{
		moduleInitSuccCnt++;
	}
	
	/*����ٷֱ�:0~100*/
	progressBarPer = (u8)((fp32)moduleInitSuccCnt / (fp32)totalModuleNbr * 100);
	
	/*================== 1.����+���Ȱٷ�����ʾ ==================*/
	/*��ʾ"ϵͳ��ʼ����"*/
	if (showOnceFlag == 0)
	{ 
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 0, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;
		
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 1, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;		
		
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 2, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;
		
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 3, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;
		
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 4, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;
		
		bsp_OLED0_96_ShowCHinese(&g_sOled0_96, xHanziStart, 0, 5, OLED096_HANZI_12X12);
		xHanziStart += OLED096_HANZI_12X12;
		
		/*��ʾ���������*/
		bsp_OLED0_96_DrawBMP(&g_sOled0_96, 16, 2, 112, 5, (u8 *)&g_ProgressBar96X24);

		/*��ʾ��������Ȱٷ����ķָ���*/
		xHanziStart	+= 8; /*����һ���ո�(' ')�ַ�*/
		bsp_OLED0_96_ShowChar(&g_sOled0_96, xHanziStart, 0, '-', OLED096_ACSII_6X12);

		/*��ʾ�ٷֱȺ�,����һ���ָ�('-')�ַ� + һ���ո�(' ')�ַ� + 3������λ*/	
		bsp_OLED0_96_ShowChar(&g_sOled0_96, (xHanziStart + 6 + 8 + 18), 0, '%', OLED096_ACSII_6X12);
		
		/*��ʾ��ʼ����¼LOG*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"LOG : ", OLED096_ACSII_6X8);
		xLogStart += 6 * 6;
		
		/*ֻ��ʾһ��*/
		showOnceFlag = 1;
	}
	
	/*�ж����ֵ�λ����*/
	if (progressBarPer <= 99)
	{
		numberLen = 2;
	}
	else
	{
		numberLen = 3;	
	}
	
	/*��ʾ�ٷֱ���*/
	xNumberStart += xHanziStart	+ 14; /*����һ���ָ���('-')6+�ո�(' ')8�ַ�*/
	if (progressBarPer <= 99) 	/*| |x|x|%|*/
	{
		xNumberStart += 8;
		
		bsp_OLED0_96_ShowNum(&g_sOled0_96, xNumberStart, 0, progressBarPer, numberLen, OLED096_ACSII_6X12);
	}
	else /*|1|0|0|%|*/
	{
		bsp_OLED0_96_ShowNum(&g_sOled0_96, xNumberStart, 0, progressBarPer, numberLen, OLED096_ACSII_6X12);
	}		
	
	/*================== 2.���ȿ���ʾ+���ȸ��� ==================*/
	/*��ʼ���ɹ��ż�һ�����ȿ�*/
	if (INIT_STATUS == SYS_RETERR_0ZR)
	{	
		/*��ʾ���������ȿ�*/
		bsp_OLED0_96_DrawBMP(&g_sOled0_96, xProgressBarBlock, 3, xProgressBarBlock + 11, 4, (u8 *)&g_ProgressBarFillBlock11X8);	
		xProgressBarBlock += 11; /*1���� = 11����λ*/
	}
	
	/*================== 3.��ʼ�����󼰳�ʼ������ַ�����ʾ ==================*/	
	/*��ʼ�������ַ�����ʾ*/
	switch(INIT_TARG)
	{	 
		/*_0_�˻�����ģ��*/
		case HCI_SHOW_INIT_HCI_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_0_HCI    = SUCC", OLED096_ACSII_6X8);	
				
				/*��ʾ��ʼ����¼ �˻�����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;				
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_0_HCI    = FAIL", OLED096_ACSII_6X8);
				
				/*��ʾ��ʼ����¼ �˻�����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}
		}break;

		/*_1_����ģ��*/
		case HCI_SHOW_INIT_BASE_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_1_Base   = SUCC", OLED096_ACSII_6X8);	

				/*��ʾ��ʼ����¼ ����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;					
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_1_Base   = FAIL", OLED096_ACSII_6X8);	
				
				/*��ʾ��ʼ����¼ ����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;				
			}		
		}break;

		/*_2_���ݴ洢ģ��*/
		case HCI_SHOW_INIT_STOR_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_2_Stor   = SUCC", OLED096_ACSII_6X8);	 

				/*��ʾ��ʼ����¼ ���ݴ洢ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;				
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_2_Stor   = FAIL", OLED096_ACSII_6X8);	
				
				/*��ʾ��ʼ����¼ ���ݴ洢ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;					
			}		
		}break;

		/*_3_AHRSģ��*/
		case HCI_SHOW_INIT_AHRS_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_3_AHRS   = SUCC", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ AHRSģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_3_AHRS   = FAIL", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ AHRSģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}		
		}break;

		/*_4_��ѹ��ģ��*/
		case HCI_SHOW_INIT_BERO_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_4_BERO   = SUCC", OLED096_ACSII_6X8);	

				/*��ʾ��ʼ����¼ ��ѹ��ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;					
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_4_BERO   = FAIL", OLED096_ACSII_6X8);	
				
				/*��ʾ��ʼ����¼ ��ѹ��ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}		
		}break;

		/*_5_������ģ��*/
		case HCI_SHOW_INIT_ULTR_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_5_ULTR   = SUCC", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ ������ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_5_ULTR   = FAIL", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ ������ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}		
		}break;

		/*_6_GPSģ��*/
		case HCI_SHOW_INIT_GPS_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_6_GPS    = SUCC", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ GPSģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_6_GPS    = FAIL", OLED096_ACSII_6X8);

				/*��ʾ��ʼ����¼ GPSģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1-", OLED096_ACSII_6X8);
				xLogStart += 2 * 6;	
			}		
		}break;
		
		/*_7_����ģ��*/
		case HCI_SHOW_INIT_OPFLOW_MODE:
		{
			if (INIT_STATUS == SYS_RETERR_0ZR)
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_7_OPFLOW = SUCC", OLED096_ACSII_6X8);	

				/*��ʾ��ʼ����¼ ����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"0", OLED096_ACSII_6X8);
				xLogStart += 1 * 6;
			}
			else
			{
				bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"_7_OPFLOW = FAIL", OLED096_ACSII_6X8);	

				/*��ʾ��ʼ����¼ ����ģ����*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, xLogStart, 7, (u8*)&"1", OLED096_ACSII_6X8);
				xLogStart += 1 * 6;	
			}		
		}break;		

		default:break;
	}	
	
	/*���һ����ʼ����Ϻ�,��ʱ1.5s������*/
	if (INIT_TARG == HCI_SHOW_INIT_OPFLOW_MODE)
	{
		sys_DelayMs(1000);
		
		/*����*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
	}
	else
	{
		/*��ʱ,���ڲ鿴*/
		sys_DelayMs(1);
	}
}

/*��ʾ������У׼���������*/
void hci_Show_Sensor_Calib_Parameter(RESULT_CALIB_STATUS ACC_CALIB_STATUS, RESULT_CALIB_STATUS MAG_CALIB_STATUS, u32 holdMs)
{	
/*1.��ʾ����*/
/*	         	
		                  |-x
			      |-Scale |-y
	              |       |-z
		    |-Acc |
	        |     |        |-x 
	        |	  |-Offset |-y
	        |              |-z
SensorCalib |      
            |              |-x
			|-Mag |-Offset |-y
	                       |-z
*/  
/*2.���Ű�
SENSOR_CALIB_RESULT
Acc:Scale&Offset SUCC
SX:��0.9999 SY:��0.9999 
SZ:��0.9999 OX:��0.9999 
OY:��0.9999 OZ:��0.9999 
MAG:Offset       SUCC
OX:��0.9999 OY:��0.9999 
OZ:��0.9999 
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾҳ����*/
	/*��0��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"SENSOR_CALIB_RESULT", OLED096_ACSII_6X8);
	
	/*��1��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"Acc:Scale&Offset", OLED096_ACSII_6X8);
	
	/*��2��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"SX:", OLED096_ACSII_6X8);
	bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SY:", OLED096_ACSII_6X8);
	
	/*��3��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"SZ:", OLED096_ACSII_6X8);
	bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"OX:", OLED096_ACSII_6X8);
	
	/*��4��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"OY:", OLED096_ACSII_6X8);	
	bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"OZ:", OLED096_ACSII_6X8);	

	/*��5��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"MAG:Offset", OLED096_ACSII_6X8);
	
	/*��6��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"OX:", OLED096_ACSII_6X8);
	bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"OY", OLED096_ACSII_6X8);	

	/*��7��*/
	bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"OZ:", OLED096_ACSII_6X8);
	
	/*��ʾ���*/
	/*��ʾ���ٶȼ�У׼���*/
	if (ACC_CALIB_STATUS == RESULT_CALIB_SUCC)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"SUCC", OLED096_ACSII_6X8);		
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"FAIL", OLED096_ACSII_6X8);	
	}
	
	/*��ʾ������У׼���*/
	if (MAG_CALIB_STATUS == RESULT_CALIB_SUCC)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"SUCC", OLED096_ACSII_6X8);		
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"FAIL", OLED096_ACSII_6X8);	
	}

	/*1.��ʾ�Ӵ洢����ȡ�ļ��ٶ�У׼ϵ��ֵ*/
	/*����ϵ��*/
	/*1.1��ʾSensorCalib -> Acc -> Scale -> x*/
	math_Floater_Number_Analy(g_fpAccScaleX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.2��ʾSensorCalib -> Acc -> Scale -> y*/
	math_Floater_Number_Analy(g_fpAccScaleY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*1.3��ʾSensorCalib -> Acc -> Scale -> z*/
	math_Floater_Number_Analy(g_fpAccScaleZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		

	/*��ƫϵ��*/
	/*1.4��ʾSensorCalib -> Acc -> Offset -> x*/
	math_Floater_Number_Analy(g_fpAccOffsetX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.5��ʾSensorCalib -> Acc -> Offset -> y*/
	math_Floater_Number_Analy(g_fpAccOffsetY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*1.6��ʾSensorCalib -> Acc -> Offset -> z*/
	math_Floater_Number_Analy(g_fpAccOffsetZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	


	/*2.��ʾ�Ӵ洢����ȡ�Ĵ�����У׼ϵ��ֵ*/
	/*��ƫϵ��*/
	/*2.1��ʾSensorCalib -> Mag -> Offset -> x*/
	math_Floater_Number_Analy(g_fpMagOffsetX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*2.2��ʾSensorCalib -> Mag -> Offset -> y*/
	math_Floater_Number_Analy(g_fpMagOffsetY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*2.3��ʾSensorCalib -> Mag -> Offset -> z*/
	math_Floater_Number_Analy(g_fpMagOffsetZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	/*����ʱ��*/
	sys_DelayMs(holdMs);
	
	/*����*/
	bsp_OLED0_96_Clear(&g_sOled0_96);
}

/*��ʾ����ϵͳ��ʼ������(PID����)*/
void hci_Show_Control_System_Parameter(SYS_RETSTATUS READ_STATUS, fp32 kP, fp32 kI, fp32 kD, PID_CONTROLER_LINK LINK_TARG, u32 holdMs)
{
	static u8 showOnceFlag  = 0;
	static u8 logStartXPos1 = 0;
	static u8 logStartXPos2 = 0;	
	u8 *pstr;
/*1.��ʾ����*/
/*	         	
		            |-kP
		|-PitchGyro |-kI
	    |           |-kD
		|
		|          |-kP
	    |-RollGyro |-kI
 	    |          |-kD
        |
        |	      |-kP
	    |-YawGyro |-kI
		|         |-kD
		|
		|            |-kP
		|-PitchAngle |-kI
	    |            |-kD
		|
		|           |-kP
	    |-RollAngle |-kI
 	    |           |-kD
        |
        |	       |-kP
	    |-YawAngle |-kI
		|          |-kD
pidPara	|
		|             |-kP
		|-HeightSpeed |-kI
	    |             |-kD
		|
		|           |-kP
	    |-HeightPos |-kI
 	    |           |-kD
        |
		|                |-kP
        |	       |-Lat |-kI
		|		   |     |-kD
	    |-GpsSpeed |
		|		   |	 |-kP
		|          |-Lon |-kI
        |                |-kD
		|                 
		|              |-kP
		|        |-Lat |-kI
		|		 |     |-kD
		|-GpsPos |      
		|        |	   |-kP
		|        |-Lon |-kI
		|              |-kD
		|
		|
		|              	  |-kP
        |	       	  |-x |-kI
		|		   	  |   |-kD
	    |-OpFlowSpeed |
		|		   	  |	  |-kP
		|          	  |-y |-kI
        |                 |-kD
		|                 
		|               |-kP
		|           |-x |-kI
		|		    |   |-kD
		|-OpFlowPos |      
		|           |	|-kP
		|           |-y |-kI
		|               |-kD
		|
		|		
		|           |-kP
	    |-HeightAcc |-kI
 	    |           |-kD
		|
        |
		|              |-kP
        |	     |-Lat |-kI
		|		 |     |-kD
	    |-GpsAcc |
		|		 |	   |-kP
		|        |-Lon |-kI
        |              |-kD
		|            
		|
		|                 |-kP
        |	        |-Lat |-kI
		|		    |     |-kD
	    |-OpFlowAcc |
	 		        |	  |-kP
	                |-Lon |-kI
                          |-kD
*/  
/*2.���Ű�
PID_SYSTEM_PARAMETER 
READ LINK "01" - SUCC
Link: PitchGyro      
kP: ��123.50
kI: ��123.50
kD: ��123.50
0-0-0-0-0-0-0-0-0-0-0
0-0-0-0-0-0-0-0-0-0-0
*/	
	/*��ʾҳ����*/
	if (showOnceFlag == 0)
	{
		/*��ֹ��ʾ��ʾ����*/
		g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_DISABLE;
		
		/*�����ʾ����æµ*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_BUSY;
		
		/*ҳ������*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
		
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"PID_SYSTEM_PARAMETER", OLED096_ACSII_6X8);
	
		/*��1��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"READ LINK "  " -", OLED096_ACSII_6X8);
	
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"Link: ", OLED096_ACSII_6X8);
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"kP: ", OLED096_ACSII_6X8);
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"kI: ", OLED096_ACSII_6X8);	

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"kD: ", OLED096_ACSII_6X8);	
			
		/*���ֻ��ʾһ��*/
		showOnceFlag = 1;
	}
	
	/*��ʾlink���*/
	math_Integer_Number_Analy(LINK_TARG, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 66, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*��ʾlinkTarg*/
	switch (LINK_TARG)
	{
		case PID_CONTROLER_PITCH_GYRO:
		{
			pstr = (u8*)&"PitchGyro      ";
		}break;
		
		case PID_CONTROLER_ROLL_GYRO:
		{
			pstr = (u8*)&"RollGyro       ";
		}break;	
		
		case PID_CONTROLER_YAW_GYRO:
		{
			pstr = (u8*)&"YawGyro        ";
		}break;	

		case PID_CONTROLER_PITCH_ANGLE:
		{
			pstr = (u8*)&"PitchAngle     ";
		}break;	

		case PID_CONTROLER_ROLL_ANGLE:
		{
			pstr = (u8*)&"RollAngle      ";
		}break;	
		
		case PID_CONTROLER_YAW_ANGLE:
		{
			pstr = (u8*)&"YawAngle       ";
		}break;	

		case PID_CONTROLER_HIGH_SPEED:
		{
			pstr = (u8*)&"HighSpeed      ";
		}break;	

		case PID_CONTROLER_HIGH_POSITION:
		{
			pstr = (u8*)&"HighPosition   ";
		}break;	
		
		case PID_CONTROLER_LATITUDE_SPEED:
		{
			pstr = (u8*)&"LatitudeSpeed  ";
		}break;	

		case PID_CONTROLER_LATITUDE_POSITION:
		{
			pstr = (u8*)&"LatitudePos    ";
		}break;	

		case PID_CONTROLER_LONGITUDE_SPEED:
		{
			pstr = (u8*)&"LongitudeSpeed ";
		}break;		
		
		case PID_CONTROLER_LONGITUDE_POSITION:
		{
			pstr = (u8*)&"LongitudePos   ";
		}break;	
		
		case PID_CONTROLER_OPTICFLOW_X_SPEED:
		{
			pstr = (u8*)&"OpFlowXSpeed   ";
		}break;		
		
		case PID_CONTROLER_OPTICFLOW_X_POSITION:
		{
			pstr = (u8*)&"OpFlowXPosSpeed";
		}break;	

		case PID_CONTROLER_OPTICFLOW_Y_SPEED:
		{
			pstr = (u8*)&"OpFlowYSpeed   ";
		}break;	

		case PID_CONTROLER_OPTICFLOW_Y_POSITION:
		{
			pstr = (u8*)&"OpFlowYPosSpeed";
		}break;	

		case PID_CONTROLER_HIGH_ACC:
		{
			pstr = (u8*)&"HighAcc        ";
		}break;	

		case PID_CONTROLER_LONGITUDE_ACC:
		{
			pstr = (u8*)&"LongitudeAcc   ";
		}break;		

		case PID_CONTROLER_LATITUDE_ACC:
		{
			pstr = (u8*)&"LatitudeAcc    ";
		}break;	

		case PID_CONTROLER_OPTICFLOW_X_ACC:
		{
			pstr = (u8*)&"OpFlowXAcc     ";
		}break;	

		case PID_CONTROLER_OPTICFLOW_Y_ACC:
		{
			pstr = (u8*)&"OpFlowYAcc     ";
		}break;			
		
		default:break;
	}		
	
	bsp_OLED0_96_ShowString(&g_sOled0_96, 36, 2, pstr, OLED096_ACSII_6X8);
	
	/*��ʾPID����*/
	math_Floater_Number_Analy(kP, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 24, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	math_Floater_Number_Analy(kI, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 24, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	math_Floater_Number_Analy(kD, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 24, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*��ʾ����PID������ʼ��״̬*/
	if (READ_STATUS == SYS_RET_SUCC)
	{
		/*��ǰ��״̬Ϊ�ɹ�*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"SUCC", OLED096_ACSII_6X8);
		
		/*��ʷLOG��ǳɹ�*/		
		if (LINK_TARG <= 10) /*δ����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos1, 6, (u8*)&"0", OLED096_ACSII_6X8);
			logStartXPos1 += 6;		
		}
		else /*����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos2, 7, (u8*)&"0", OLED096_ACSII_6X8);
			logStartXPos2 += 6;			
		}			
		
		/*�ָ���*/
		if (LINK_TARG <= 9) /*δ����*/
		{			
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos1, 6, (u8*)&"-", OLED096_ACSII_6X8);				
			logStartXPos1 += 6;
		}
		else if ((11 <= LINK_TARG) && (LINK_TARG <= 20)) /*����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos2, 7, (u8*)&"-", OLED096_ACSII_6X8);				
			logStartXPos2 += 6;
		}
	}
	else if (READ_STATUS == SYS_RET_FAIL)
	{
		/*��ǰ��״̬Ϊʧ��*/		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"FAIL", OLED096_ACSII_6X8);		
			
		/*��ʷLOG���ʧ��*/
		if (LINK_TARG <= 10) /*δ����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos1, 6, (u8*)&"1", OLED096_ACSII_6X8);
			logStartXPos1 += 6;		
		}
		else /*����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos2, 7, (u8*)&"1", OLED096_ACSII_6X8);
			logStartXPos2 += 6;			
		}
		
		/*�ָ���*/		
		if (LINK_TARG <= 9) /*δ����*/
		{			
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos1, 6, (u8*)&"-", OLED096_ACSII_6X8);				
			logStartXPos1 += 6;
		}
		else if ((11 <= LINK_TARG) && (LINK_TARG <= 20)) /*����*/
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, logStartXPos2, 7, (u8*)&"-", OLED096_ACSII_6X8);				
			logStartXPos2 += 6;
		}			
	}	
	
	/*���п��ƻ�������ȡ���*/
	if (LINK_TARG >= (PID_PARAMETER_SETTING_NBR - 1))
	{
		/*״̬����λ*/
		showOnceFlag  = 0;
		
		/*���긴λ*/
	    logStartXPos1 = 90;
	    logStartXPos2 = 0;			
		
		sys_DelayMs(holdMs);
		
		/*�жϽ�������ʾ����*/
		if (g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) /*�Ѿ�������ʾ,���л���Ĭ����ҳ*/
		{				
			/*�ָ�Ĭ����ʾҳ���*/
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		}
		else		
		{
			/*������ʾ��ʾ����*/
			g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE;
			
			/*������ʾ����˵���ʾ��ʾҳ��*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;
		}		
		
		/*�����ʾ�������*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE;			
	}
	else 
	{
		sys_DelayMs(300);		
	}
}

/*ʹ��OLED��ʾ��ʾ*/
void hci_Show_Enable_Hint(void)
{
	/*��ʾ������ʾ*/
	bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 0, 128, 8, (u8 *)&g_EnableShowHint128X64);	
}

/*=========  �������й�������ʾ =========*/
void hci_show_on_run_progress(void)
{
	/*HCI(OLED):��ң������ȡ��ǰ��ʾҳ�����*/
	hci_remot_switch_show_status(&g_sHciShowPage);
	
	/*�����ж��Ƿ�������ʾ,��ʾ�������(����������ʾ����ռ��)*/
	if ((g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) && \
		(g_sHciShowPage.SHOW_TASK_STATUS != HCI_SHOW_TASK_BUSY))
	{
		/*������ʾ��,�˳���ʾ������λ*/
		g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;
		
		/*�ж��Ƿ�����ҳ*/
		if (g_sHciShowPage.curPageIndex != g_sHciShowPage.lastPageIndex)
		{
			g_sHciShowPage.lastPageIndex = g_sHciShowPage.curPageIndex;
			
			/*����*/
			bsp_OLED0_96_Clear(&g_sOled0_96);
			
			/*ҳ��ģ����ʾ״̬��λ*/
			g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_FIRST;
		}
		
		/*��ʾ���*/
		switch(g_sHciShowPage.curPageIndex)
		{
			case HCI_SHOW_PAGE_0:
			{
				/*NO0.AHRS������ʾ*/
				hci_Show_Cur_Ahrs_Data();
			}break;

			case HCI_SHOW_PAGE_1:
			{
				/*NO1.�߶ȴ�����(������+��ѹ��)������ʾ*/
				hci_Show_High_Sensor_Data();			
			}break;

			case HCI_SHOW_PAGE_2:
			{
				/*NO2.GPS������ʾ*/
				hci_Show_Gps_Data();
			}break;

			case HCI_SHOW_PAGE_3:
			{
				/*NO3.����������ʾ*/
				hci_Show_Opticflow_Data();			
			}break;

			case HCI_SHOW_PAGE_4:
			{
				/*NO4.��ֱ�߶ȹߵ��ں���ʾ*/
				hci_Show_Height_SINS_Data();			
			}break;

			case HCI_SHOW_PAGE_5:
			{
				/*NO5.ˮƽ��PITCH����ߵ��ں���ʾ*/
				hci_Show_HorizontalPitch_SINS_Data();			
			}break;

			case HCI_SHOW_PAGE_6:
			{
				/*NO6.ˮƽ��ROLL����ߵ��ں���ʾ*/
				hci_Show_HorizontalRoll_SINS_Data();			
			}break;

			case HCI_SHOW_PAGE_7:
			{
				/*NO7.����ִ��������ʾ*/
				hci_Show_Execute_Period();				
			}break;

			case HCI_SHOW_PAGE_8:
			{
				/*NO8.����������״̬*/
				hci_Show_Aircraft_Status();			
			}break;

			case HCI_SHOW_PAGE_9:
			{
				/*NO9.ң��״̬��ʾ*/
				hci_Show_Remot_Status();
			}break;	

			case HCI_SHOW_PAGE_10:
			{
				/*NO10.������У׼���*/
				hci_Show_Sensor_Calib_Result();
			}break;

			default:break;
		}
	}
	else if (g_sHciShowPage.SHOW_HINT_STATUS == UAV_HCI_SHOW_ENABLE)
	{
		if (g_sHciShowPage.EXIT_SHOW_STATUS == HCI_EXIT_SHOW_OP_FIRST)
		{
			/*��ʾҳ�������0*/
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			
			/*�ϴ�ҳ��ٵ�ǰҳ��,��Ȼʹ����ʾ��������*/
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		
			/*ҳ������*/
			bsp_OLED0_96_Clear(&g_sOled0_96);
			
			/*������ʾ��ʾ*/
			hci_Show_Enable_Hint();
			
			/*����Ѿ��˳��������*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_NOTFIRST;
		}
		
		/*�ڴ���ҳ��,��ʾCPU������*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 38, 2, (u8*)&"CPU:", OLED096_ACSII_6X8);
		math_Integer_Number_Analy(g_psUav_Status->UavProgrameStatus.CpuUse.major, 2, &g_sMathIntegerAnaly);				
		bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 66, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 78, 2, '%', OLED096_ACSII_6X8);
		
		/*�ڴ���ҳ��,��ʾRTOS����״̬*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 38, 3, (u8*)&"TASK:", OLED096_ACSII_6X8);
		if (gps_SafeOperation->Task_Check_Status.TASK_CHECK_STATUS == SAFE_TASK_CHECK_NORMAL)
		{
			/*�����������*/
			bsp_OLED0_96_ShowString(&g_sOled0_96, 68, 3, (u8*)&"OK", OLED096_ACSII_6X8);
		}
		else
		{
			/*��������쳣*/
			bsp_OLED0_96_ShowString(&g_sOled0_96, 68, 3, (u8*)&"NO", OLED096_ACSII_6X8);
		}
	}
}

/*==== 1.ʵʱ������ʾ ====*/
/*NO0.AHRS������ʾ*/
void hci_Show_Cur_Ahrs_Data(void)
{
	u8 xNbr1Pos = 0;
	u8 xNbr2Pos = 0;	
	xNbr1Pos    = 18;	
	xNbr2Pos    = 84;

/*1.��ʾ����*/
/*	         				 |- x
			   	      |- ACC |- y
		              |      |- z
	           |- IMU |
			   |      |       |- x
			   |	  |- GYRO |- y
			   |			  |- z
	 |- Sensor |
     |         |	  |- x
	 |	   	   |- MAG |- y
AHRS |                |- z
     |	
	 |        |- pitch       
	 |- Euler |- roll
	          |- yaw
*/  
/*2.���Ű�
NO0: AHRS_SENSOR    *
	
ax:��0.9999 ay:��0.9999
az:��0.9999 gx:��0.9999
gy:��0.9999 gz:��0.9999
mx:��0.9999 my:��0.9999
mz:��0.9999 Er:��1.0011
Ep:��11.411 Ey:��111.11
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO0: AHRS_SENSOR", OLED096_ACSII_6X8);		
		
		/*��1�зָ��ʾ*/
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"ax:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"ay:", OLED096_ACSII_6X8);
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"az:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"gx:", OLED096_ACSII_6X8);

		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"gy:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"gz:", OLED096_ACSII_6X8);	

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"mx:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"my:", OLED096_ACSII_6X8);		
		
		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"mz:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"Er:", OLED096_ACSII_6X8);

		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"Ep:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 7, (u8*)&"Ey:", OLED096_ACSII_6X8);

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.ACC ===*/
	/*1.1��ʾAHRS -> Sensor -> IMU -> Acc -> x*/
	math_Floater_Number_Analy(g_psAccOrigion->x, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.2��ʾAHRS -> Sensor -> IMU -> Acc -> y*/
	math_Floater_Number_Analy(g_psAccOrigion->y, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.3��ʾAHRS -> Sensor -> IMU -> Acc -> z*/
	math_Floater_Number_Analy(g_psAccOrigion->z, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 2.GYRO ===*/	
	/*2.1��ʾAHRS -> Sensor -> IMU -> Gyro -> x*/
	math_Floater_Number_Analy(g_psGyroAttData->x, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*2.2��ʾAHRS -> Sensor -> IMU -> Gyro -> y*/
	math_Floater_Number_Analy(g_psGyroAttData->y, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*2.3��ʾAHRS -> Sensor -> IMU -> Gyro -> z*/
	math_Floater_Number_Analy(g_psGyroAttData->z, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 3.MAG ===*/	
	/*3.1��ʾAHRS -> Sensor -> Mag -> x*/
	math_Floater_Number_Analy(g_psMagFilter->x, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*3.2��ʾAHRS -> Sensor -> Mag -> y*/
	math_Floater_Number_Analy(g_psMagFilter->y, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*3.3��ʾAHRS -> Sensor -> Mag -> z*/
	math_Floater_Number_Analy(g_psMagFilter->z, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 4.Euler ===*/	
	/*4.1��ʾAHRS -> Euler -> roll*/
	math_Floater_Number_Analy(g_psAhrsAttitude->roll, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*4.1��ʾAHRS -> Euler -> pitch*/
	math_Floater_Number_Analy(g_psAhrsAttitude->pitch, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*4.3��ʾAHRS -> Euler -> yaw*/
	math_Floater_Number_Analy(g_psAhrsAttitude->yaw, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
}

/*NO1.�߶ȴ�����(������+��ѹ��)������ʾ*/
void hci_Show_High_Sensor_Data(void)
{
	u8 xNbr1Pos1    = 12;	
	u8 xNbr2Pos1    = 78;	
	u8 xNbr1Pos2    = 24;	
	u8 xNbr2Pos2    = 90;	
	
/*1.��ʾ����*/
/*	         		     |- zeroPressure
	             |- BERO |
			     |       |- curPressure
			     |	
			     |		
	   |- Sensor |
       |         |	
	   |	   	 |- ULTR 
Height |                
       |			     |- zeroHeight
	   |         |- BERO |
	   |- Height |       |- curHeight
			     |
			     |		 |- zeroHeight
				 |- ULTR |
					     |- curHeight
*/  				
/*2.���Ű�
NO1: HEIGHT_SENSOR  *

BeroData:
z:��99999.0 c:��99999.0
BeroHeight(cm):
PzH:��6666  PcH:��6666
UltrHeight(cm):
UzH:��6666  UcH:��6666
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}
	
	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO1: HEIGHT_SENSOR", OLED096_ACSII_6X8);		
		
		/*��1�зָ��ʾ*/
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"BeroData:", OLED096_ACSII_6X8);	
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"z:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"c:", OLED096_ACSII_6X8);

		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"BeroHeight(cm):", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"PzH:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"PcH:", OLED096_ACSII_6X8);
		
		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"UltrHeight(cm):", OLED096_ACSII_6X8);

		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"UzH:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 7, (u8*)&"UcH:", OLED096_ACSII_6X8);

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.BeroData ===*/
	/*1.1��ʾHigh_Sensor -> Sensor -> BERO -> zeroPressure*/
	math_Floater_Number_Analy(g_psAttitudeAll->zeroPressure, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr1Pos1, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	/*1.2��ʾHigh_Sensor -> Sensor -> BERO -> curPressure*/
	math_Floater_Number_Analy(g_psAttitudeAll->curPressure, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, xNbr2Pos1, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	
	/*=== 2.BeroHeight ===*/
	/*2.1��ʾBeroHeight -> Height -> BERO -> zeroHeight*/
	math_Integer_Number_Analy(g_psAttitudeAll->zeroBeroHeight, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, xNbr1Pos2, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*2.1��ʾBeroHeight -> Height -> BERO -> curHeight*/
	math_Integer_Number_Analy(g_psAttitudeAll->nowBeroAltitude, 4, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, xNbr2Pos2, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	
	/*=== 3.UltrHeight ===*/
	/*2.1��ʾBeroHeight -> Height -> ULTR -> zeroHeight*/
	math_Integer_Number_Analy(g_psAttitudeAll->zeroUltrHeight, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, xNbr1Pos2, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*2.1��ʾBeroHeight -> Height -> ULTR -> curHeight*/
	math_Integer_Number_Analy(g_psAttitudeAll->nowUltrAltitude, 4, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, xNbr2Pos2, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
}

/*NO2.GPS������ʾ*/
void hci_Show_Gps_Data(void)
{
/*1.��ʾ����*/
/*
			   |-year
			   |-month
		|-Date |-day
		|      |-hour
        |      |-minute
		|	   |-second
	    |
	    |     |-fixType
	    |-Fix |
	    |     |-sateNbr
	    |
	    |                |-latitude
GpsDate	|-_3_dimensional |-longitude
	    |                |-height
	    |
	    |- yaw
	    |
	    |          |-north
	    |-NavSpeed |-east
		|          |-up
		|
		|          |-pos
		|-Accuracy |
		           |-speed
*/  				
/*2.���Ű�
NO2: GPS_DATA	    *
D:2018/09/25 11:13:00
Fix: 2D   SateNbr: 13//Fix: 3D    SateNbr: 9//Fix: NA    SateNbr: 9
Lat:��101.1681770
Lon:��101.1681770
Hei��9999.0 Yaw��206.70//m ��
E:��0.99999 N:��9999.99
U:��0.99999 PDOP��999.0
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO2: GPS_DATA", OLED096_ACSII_6X8);		
		
		/*��1��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"D:1970/01/01 00:00:00", OLED096_ACSII_6X8);		
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"Fix: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 2, (u8*)&"SateNbr: ", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"Lat: ", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"Lon: ", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"Hei: ", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"Yaw: ", OLED096_ACSII_6X8);		

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"E:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"N:", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"U:", OLED096_ACSII_6X8);			
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 7, (u8*)&"PDOP:", OLED096_ACSII_6X8);		
		
		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.Date ===*/
	/*1.1��ʾGpsDate -> Date -> year*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.year, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 12, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*1.1��ʾGpsDate -> Date -> month*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.month, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 42, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*1.1��ʾGpsDate -> Date -> day*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.day, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 60, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*1.1��ʾGpsDate -> Date -> hour*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.hour, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 78, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*1.1��ʾGpsDate -> Date -> minute*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.minute, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 96, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*1.1��ʾGpsDate -> Date -> second*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.LocalTime.second, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	
	/*=== 2.Fix ===*/
	/*2.1��ʾGpsDate -> Fix -> fixType*/
	if (g_psAttitudeAll->GpsData.POS_FIX_TYPE == M8N_POS_FIX_3D)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 30, 2, (u8*)&"3D", OLED096_ACSII_6X8);		
	}
	else if (g_psAttitudeAll->GpsData.POS_FIX_TYPE == M8N_POS_FIX_2D)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 30, 2, (u8*)&"2D", OLED096_ACSII_6X8);		
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 30, 2, (u8*)&"NA", OLED096_ACSII_6X8);			
	}

	/*2.2��ʾGpsDate -> Fix -> sateNbr*/
	math_Integer_Number_Analy(g_psAttitudeAll->GpsData.satelliteNbr, 2, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 108, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	
	/*=== 3._3_dimensional===*/
	/*3.1��ʾGpsDate -> _3_dimensional -> latitude*/
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.Coordinate_f8.lat, 12, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 24, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*3.2��ʾGpsDate -> _3_dimensional -> longitude*/
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.Coordinate_f8.lon, 12, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 24, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	/*3.3��ʾGpsDate -> _3_dimensional -> height*/
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.hMSL, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	
	/*=== 4.yaw===*/
	/*4.1��ʾGpsDate -> yaw*/
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.headMot, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 5.NavSpeed ===*/
	/*5.1��ʾGpsDate -> NavSpeed -> east*/
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.CurSpeed.east, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 12, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*5.2��ʾGpsDate -> NavSpeed -> north*/	
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.CurSpeed.north, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 78, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		
		
	/*5.3��ʾGpsDate -> NavSpeed -> up*/			
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.CurSpeed.up, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 12, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	/*=== 6.�������� ===*/
	/*6.1�ٶȾ�������*/
	
	/*6.2λ�þ�������*/	
	math_Floater_Number_Analy(g_psAttitudeAll->GpsData.quality, 6, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 90, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
}

/*NO3.����������ʾ*/
void hci_Show_Opticflow_Data(void)
{
/*1.��ʾ����*/
/*
			   |-year
			   |-month
		|-Date |-day
		|      |-hour
        |      |-minute
		|	   |-second
	    |
	    |     |-fixType
	    |-Fix |
	    |     |-sateNbr
	    |
	    |                |-latitude
GpsDate	|-_3_dimensional |-longitude
	    |                |-height
	    |
	    |- yaw
	    |
	    |          |-north
	    |-NavSpeed |-east
		|          |-up
		|
		|          |-pos
		|-Accuracy |
		           |-speed
*/ 

/*2.���Ű�
NO3: OPFLOW_DATA    *
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}					
	
	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO3: OPFLOW_DATA", OLED096_ACSII_6X8);			

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;		
	}
}

/*==== 2.�ߵ��ں�������ʾ ====*/
/*NO4.��ֱ�߶ȹߵ��ں���ʾ*/
void hci_Show_Height_SINS_Data(void)
{
/*1.��ʾ����*/
/*
	             |-origionAcc
	       |-ACC |
	       |     |-sinsAcc
           |
	       |       |-origionSpeed
	       |-Speed |
	       |       |-sinsSpeed
HeightSINS |  
	       |     |-estimateHeight
	       |-Pos |
	       |     |-sinsHeight
		   |
		   |            |-acc
		   |-TOCCorrect |-speed
	                    |-pos
*/  				
/*2.���Ű�
NO4: VER_SINS_DATA  *

OA:��0.9999 SA:��0.9999
OS:��0.9999 SS:��0.9999
OP:��0.9999 SP:��0.9999
TOC_CA: ��0.99999
TOC_CS: ��0.99999
TOC_CP: ��0.99999
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO4: VER_SINS_DATA", OLED096_ACSII_6X8);		
		
		/*��1�и���*/	
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"OA:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SA", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"OS:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"SS", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"OP:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"SP", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"TOC_CA: ", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"TOC_CS: ", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"TOC_CP: ", OLED096_ACSII_6X8);	

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.ACC===*/
	/*1.1��ʾHeightSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsOrigion->curAcc[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*1.2��ʾHeightSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsReal->curAcc[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 2.Speed===*/
	/*2.1��ʾHeightSINS -> Speed -> origionSpeed*/
	math_Floater_Number_Analy(g_psSinsOrigion->curSpeed[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*2.2��ʾHeightSINS -> Speed -> sinsSpeed*/
	math_Floater_Number_Analy(g_psSinsReal->curSpeed[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		
	
	
	/*=== 3.Pos===*/
	/*3.1��ʾHeightSINS -> Pos -> estimateHeight*/
	math_Floater_Number_Analy(g_psSinsReal->estimatePos[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*3.2��ʾHeightSINS -> Pos -> estimateHeight*/
	math_Floater_Number_Analy(g_psSinsReal->curPosition[EARTH_FRAME_Z], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 4.TOCCorrect ===*/
	/*4.1��ʾTOCCorrect -> acc*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Z].acc, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*4.2��ʾTOCCorrect -> speed*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Z].speed, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*4.1��ʾTOCCorrect -> pos*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Z].pos, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
}

/*NO5.ˮƽ��PITCH����ߵ��ں���ʾ*/
void hci_Show_HorizontalPitch_SINS_Data(void)
{
/*1.��ʾ����*/
/*
					      |-origionAcc
					|-ACC |
					|     |-sinsAcc
					|
					|       |-origionSpeed
					|-Speed |
	                |       |-sinsSpeed
HorizontalPitchSINS |  
					|     |-estimateHorizontalPitch//y��
			        |-Pos |
					|     |-sinsHorizontalPitch//y��
				    |
				    |            |-acc
					|-TOCCorrect |-speed
							     |-pos
*/  				
/*2.���Ű�
NO5: PIH_SINS_DATA  *

OA:��0.9999 SA:��0.9999
OS:��0.9999 SS:��0.9999
OP:��0.9999 SP:��0.9999
TOC_CA: ��0.99999
TOC_CS: ��0.99999
TOC_CP: ��0.99999
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO5: PIH_SINS_DATA", OLED096_ACSII_6X8);		
		
		/*��1�и���*/	
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"OA:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SA", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"OS:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"SS", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"OP:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"SP", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"TOC_CA: ", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"TOC_CS: ", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"TOC_CP: ", OLED096_ACSII_6X8);	

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.ACC===*/
	/*1.1��ʾHorizontalPitchSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsOrigion->curAcc[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*1.2��ʾHorizontalPitchSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsReal->curAcc[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 2.Speed===*/
	/*2.1��ʾHorizontalPitchSINS -> Speed -> origionSpeed*/
	math_Floater_Number_Analy(g_psSinsOrigion->curSpeed[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*2.2��ʾHorizontalPitchSINS -> Speed -> sinsSpeed*/
	math_Floater_Number_Analy(g_psSinsReal->curSpeed[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		
	
	
	/*=== 3.Pos===*/
	/*3.1��ʾHorizontalPitchSINS -> Pos -> estimateHorizontalPitch*/
	math_Floater_Number_Analy(g_psSinsReal->estimatePos[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*3.2��ʾHorizontalPitchSINS -> Pos -> sinsHorizontalPitch*/
	math_Floater_Number_Analy(g_psSinsReal->curPosition[EARTH_FRAME_X], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 4.TOCCorrect ===*/
	/*4.1��ʾTOCCorrect -> acc*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_X].acc, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*4.2��ʾTOCCorrect -> speed*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_X].speed, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*4.1��ʾTOCCorrect -> pos*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_X].pos, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
}

/*NO6.ˮƽ��ROLL����ߵ��ں���ʾ*/
void hci_Show_HorizontalRoll_SINS_Data(void)
{
/*1.��ʾ����*/
/*
					     |-origionAcc
				   |-ACC |
				   |     |-sinsAcc
				   |
				   |       |-origionSpeed
				   |-Speed |
	               |       |-sinsSpeed
HorizontalRollSINS |  
				   |     |-estimateHorizontalRoll//y��
			       |-Pos |
				   |     |-sinsHorizontalRoll//y��
				   |
				   |            |-acc
				   |-TOCCorrect |-speed
							    |-pos
*/  				
/*6.���Ű�
NO6: ROL_SINS_DATA  *

OA:��0.9999 SA:��0.9999
OS:��0.9999 SS:��0.9999
OP:��0.9999 SP:��0.9999
TOC_CA: ��0.99999
TOC_CS: ��0.99999
TOC_CP: ��0.99999
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO6: ROL_SINS_DATA", OLED096_ACSII_6X8);		
		
		/*��1�и���*/	
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"OA:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SA", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"OS:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"SS", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"OP:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"SP", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"TOC_CA: ", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"TOC_CS: ", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"TOC_CP: ", OLED096_ACSII_6X8);	

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.ACC===*/
	/*1.1��ʾHorizontalRollSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsOrigion->curAcc[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*1.2��ʾHorizontalRollSINS -> ACC -> origionAcc*/
	math_Floater_Number_Analy(g_psSinsReal->curAcc[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 2.Speed===*/
	/*2.1��ʾHorizontalRollSINS -> Speed -> origionSpeed*/
	math_Floater_Number_Analy(g_psSinsOrigion->curSpeed[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*2.2��ʾHorizontalRollSINS -> Speed -> sinsSpeed*/
	math_Floater_Number_Analy(g_psSinsReal->curSpeed[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		
	
	
	/*=== 3.Pos===*/
	/*3.1��ʾHorizontalRollSINS -> Pos -> estimateHorizontalRoll*/
	math_Floater_Number_Analy(g_psSinsReal->estimatePos[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*3.2��ʾHorizontalRollSINS -> Pos -> sinsHorizontalRoll*/
	math_Floater_Number_Analy(g_psSinsReal->curPosition[EARTH_FRAME_Y], 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
	
	
	/*=== 4.TOCCorrect ===*/
	/*4.1��ʾTOCCorrect -> acc*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Y].acc, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);

	/*4.2��ʾTOCCorrect -> speed*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Y].speed, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*4.1��ʾTOCCorrect -> pos*/
	math_Floater_Number_Analy(g_psTOCSystem->BackCorrect[EARTH_FRAME_Y].pos, 8, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 48, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
}

/*==== 3.��������ǰ״̬��ʾ ====*/
/*NO7.����ִ��������ʾ*/
void hci_Show_Execute_Period(void)
{
/*1.��ʾ����*/
/*
			  |-Process |-task
			  |         
			  |
			  |      |-height
		      |-SINS |
	          |      |-horizontal
              | 
              |        |-beroAboveAltitude	
	          |-Sensor |-berobeneathAltitude
	          |        |-ultrAltitude
			  |
ExecutePeriod |-ahrsAttitude
	          |
			  |
	          |							 |-ctrlMainLeading(����latitudePos,longitudePos)
			  |        			         |-heightPos
			  |		   |-CtrlMainLeading |-heightAcc
			  |		   |			     |-heightSpeed
			  |		   |			     |-latitudeSpeed
			  |		   |				 |-longitudeSpeed
			  |        |
			  |		   |						 |-pitch
			  |Control |		         |-Angle |-roll
	                   |                 |       |-yaw
					   |-AttitudeControl |
					                     |      |-pitch
 				                         |-Gyro |-roll
										        |-yaw			 
*/ 									 
/*2.���Ű�
NO7: EXE_PERIOD_MS  *
task999 Sih999 Sio999  //task(task) Sh(SINS_height) Sho(SINS_horizontal)
SsBerA:999 SsBerB:999  //SiberA(beroAboveAltitude) SiberB(berobeneathAltitude)
SsUltr:999   Ahrs:999  //SsUltr(ultrAltitude) Ahrs(ahrsAttitude)
CMa999 Clp999 Cls999 //CMa(ctrlMainLeading(latitudePos,longitudePos)) Clp(latitudePos,longitudePos) Cls(latitudeSpeed,longitudeSpeed)
Cha999 Chs999 Chp999   //Cha(heightAcc)   Chs(heightSpeed) Chp(heightPos)
CAp999 CAr999 CAy999   //CAp(Angle_pitch) CAr(Angle_roll)  CAy(Angle_yaw)
CGp999 CGr999 CGy999   //CGp(Angle_pitch) CGr(Angle_roll)  CGy(Angle_yaw)
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO7: EXE_PERIOD_MS", OLED096_ACSII_6X8);		
		
		/*��1�и���*/	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"task", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 1, (u8*)&"Sih", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 90, 1, (u8*)&"Sio", OLED096_ACSII_6X8);
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"SsBerA:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SsBerB:", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"SsUltr:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 78, 3, (u8*)&"Ahrs:", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"CMa", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 4, (u8*)&"Clp", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 4, (u8*)&"Cls", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"Cha", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 5, (u8*)&"Chs", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 5, (u8*)&"Chp", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"CAp", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 6, (u8*)&"CAr", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 6, (u8*)&"CAy", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"CGp", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 7, (u8*)&"CGr", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 7, (u8*)&"CGy", OLED096_ACSII_6X8);	

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.Process ===*/
	/*1.1��ʾExecutePeriod -> Process -> task*/
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->UavCtrl.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 24, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	
	/*=== 2.SINS ===*/
	/*2.1��ʾExecutePeriod -> SINS -> height*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->SINS_High.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 66, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);

	/*2.2��ʾExecutePeriod -> SINS -> horizontal*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->SINS_Horizontal.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	
	/*=== 3.Sensor ===*/
	/*3.1��ʾExecutePeriod -> Sensor -> beroAboveAltitude*/
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->BeroAboveAltitude.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 42, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*3.2��ʾExecutePeriod -> Sensor -> berobeneathAltitude*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->BeroBeneathAltitude.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);

	/*3.3��ʾExecutePeriod -> Sensor -> ultrAltitude*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->UltrAltitude.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 42, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	
	/*=== 4.ahrsAttitude ===*/
	/*4.1��ʾExecutePeriod -> ahrsAttitude*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->AhrsAttitude.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	
	/*=== 5.Control ===*/	
	/*5.1��ʾExecutePeriod -> Control -> CtrlMainLeading -> ctrlMainLeading*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->CTRL_MainLeading.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 4, OLED096_ACSII_6X8, g_sMathIntegerAnaly);

	/*5.2��ʾExecutePeriod -> Control -> CtrlMainLeading -> latitudePos & lontitudePos*/	
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->CTRL_MainLeading.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 60, 4, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.3��ʾExecutePeriod -> Control -> CtrlMainLeading -> latitudeSpeed & lontitudeSpeed*/		
	math_Integer_Number_Analy(g_psSystemPeriodExecuteTime->CTRL_MainLeading.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 102, 4, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.4��ʾExecutePeriod -> Control -> CtrlMainLeading -> heightAcc*/	
	math_Integer_Number_Analy(g_psPidSystem->HighAcc.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.5��ʾExecutePeriod -> Control -> CtrlMainLeading -> heightSpeed*/	
	math_Integer_Number_Analy(g_psPidSystem->HighSpeed.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 60, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.6��ʾExecutePeriod -> Control -> CtrlMainLeading -> heightPos*/	
	math_Integer_Number_Analy(g_psPidSystem->HighPosition.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*5.7��ʾExecutePeriod -> Control -> AttitudeControl -> Angle -> pitch*/	
	math_Integer_Number_Analy(g_psPidSystem->PitchAngle.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 6, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.8��ʾExecutePeriod -> Control -> AttitudeControl -> Angle -> roll*/	
	math_Integer_Number_Analy(g_psPidSystem->PitchGyro.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 60, 6, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.9��ʾExecutePeriod -> Control -> AttitudeControl -> Angle -> yaw*/	
	math_Integer_Number_Analy(g_psPidSystem->YawAngle.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 6, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*5.10��ʾExecutePeriod -> Control -> AttitudeControl -> Gyro -> pitch*/	
	math_Integer_Number_Analy(g_psPidSystem->PitchGyro.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.11��ʾExecutePeriod -> Control -> AttitudeControl -> Gyro -> roll*/	
	math_Integer_Number_Analy(g_psPidSystem->RollGyro.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 60, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*5.12��ʾExecutePeriod -> Control -> AttitudeControl -> Gyro -> yaw*/	
	math_Integer_Number_Analy(g_psPidSystem->YawGyro.PidControlDT.DeltaTime, 3, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 108, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
}

/*NO8.����������״̬*/
void hci_Show_Aircraft_Status(void)
{
/*1.��ʾ����*/
/*
					  |-lock
					  |-flyStatus
	           |-Self |-gpshomeSet        
		       |	  |-cmcStatus
			   |
	           |		              |-lastHeightControlMode
	           |             |-Height |-curHeightControlMode
			   |-ControlType |        |-heightControlMode 
			   |             |
			   |             |			  |-lastHorizontalControlMode
			   |			 |-Horizontal |-curHorizontalControlMode
AircraftStatus |                          |-HorizontalControlMode			 
			   |
			   |		             |-beroEstimateAltitude
               |          |-Altitude |-ultrEstimateAltitude 
			   |-Estimate |         
			   |          |-Horizontal |-gpsEstimateHorizontal
			   |                       |-opflowEstimateHorizontal
			   |
	           |-Sensor |-estimateAltitude
			            |-estimateHorizontal
*/
/*2.���Ű�
NO8: UAV_STATUS     *

locS: UNL  flyS: LAD
gpsH: SET  cmcS: SUC
LV: SA CV: FX RV: FX //LV(lastHeightControlMode) CV(curHeightControlMode) RV(heightControlMode)
LH: SA CH: FX RH: FX //LH(lastHorizontalControlMode) CH(curHorizontalControlMode) RH(HorizontalControlMode)
bE: OK uE: OK gE: OK //bE(beroEstimateAltitude) uE(ultrEstimateAltitude) gE(gpsEstimateHorizontal)
oE: OK eV:B+U eH:G+O //oE(opflowEstimateHorizontal) eV(estimateAltitude) eH(estimateHorizontal)
*/	
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO8: UAV_STATUS", OLED096_ACSII_6X8);		
		
		/*��1�и���*/
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"locS: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"flyS: ", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"gpsH: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"cmcS: ", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"LV: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 4, (u8*)&"CV: ", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 4, (u8*)&"RV:", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"LH: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 5, (u8*)&"CH: ", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 5, (u8*)&"RH:", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"bE: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 6, (u8*)&"uE: ", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 6, (u8*)&"gE: ", OLED096_ACSII_6X8);					
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"oE: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 7, (u8*)&"eV:", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 7, (u8*)&"eH:", OLED096_ACSII_6X8);	

		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.����������״̬��ʾ ===*/
	/*1.1��ʾAircraftStatus -> Self -> lock*/
	if (g_sUav_Status.LOCK_STATUS == UAV_LOCK_NOT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 36, 2, (u8*)&"UNL", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 36, 2, (u8*)&"LOC", OLED096_ACSII_6X8);			
	}
	
	/*1.2��ʾAircraftStatus -> Self -> flyStatus*/
	if (g_sUav_Status.UavLandStatus.ThisTime == UAV_LAND_NOT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 2, (u8*)&"FLY", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavLandStatus.ThisTime == UAV_LAND_YES)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 2, (u8*)&"LAD", OLED096_ACSII_6X8);			
	}	
	
	/*1.3��ʾAircraftStatus -> Self -> gpshomeSet*/
	if (g_sUav_Status.HOME_SET_STATUS == UAV_HOME_SET_YES)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 36, 3, (u8*)&"SET", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.HOME_SET_STATUS == UAV_HOME_SET_NOT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 36, 3, (u8*)&"NST", OLED096_ACSII_6X8);			
	}	

	/*1.4��ʾAircraftStatus -> Self -> cmcStatus*/
	if (g_sUav_Status.WIRELESS_CMC_STATUS == UAV_WIRELESS_CMC_SUCC)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 3, (u8*)&"SUC", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.WIRELESS_CMC_STATUS == UAV_WIRELESS_CMC_FAIL)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 3, (u8*)&"FAL", OLED096_ACSII_6X8);			
	}

	/*=== 2.����ģʽ ===*/
	/*2.1��ʾAircraftStatus -> ControlType -> Height -> lastHeightControlMode*/
	if (g_sUav_Status.UavControlMode.Vertical.Mode_Switch.LastTime == UAV_VERTICAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"SA", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Vertical.Mode_Switch.LastTime == UAV_VERTICAL_CONTROL_FIX_HEIGHT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"FX", OLED096_ACSII_6X8);			
	}	
	
	/*2.2��ʾAircraftStatus -> ControlType -> Height -> curHeightControlMode*/
	if (g_sUav_Status.UavControlMode.Vertical.Mode_Switch.ThisTime == UAV_VERTICAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"SA", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Vertical.Mode_Switch.ThisTime == UAV_VERTICAL_CONTROL_FIX_HEIGHT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"FX", OLED096_ACSII_6X8);			
	}	
	
	/*2.3��ʾAircraftStatus -> ControlType -> Height -> heightControlMode*/
	if (g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 4, (u8*)&"F2S", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Vertical.CONTROL_MODE == UAV_VERTICAL_CONTROL_FIX_HEIGHT)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 4, (u8*)&"S2F", OLED096_ACSII_6X8);			
	}	

	/*2.4��ʾAircraftStatus -> ControlType -> Horizontal -> lastHorizontalControlMode*/
	if (g_sUav_Status.UavControlMode.Horizontal.Mode_Switch.LastTime == UAV_HORIZONTAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 5, (u8*)&"SA", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Horizontal.Mode_Switch.LastTime == UAV_HORIZONTAL_CONTROL_FIX_POS)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 5, (u8*)&"FX", OLED096_ACSII_6X8);			
	}	
	
	/*2.5��ʾAircraftStatus -> ControlType -> Horizontal -> curHorizontalControlMode*/
	if (g_sUav_Status.UavControlMode.Horizontal.Mode_Switch.ThisTime == UAV_HORIZONTAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"SA", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Horizontal.Mode_Switch.ThisTime == UAV_HORIZONTAL_CONTROL_FIX_POS)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"FX", OLED096_ACSII_6X8);			
	}	
	
	/*2.6��ʾAircraftStatus -> ControlType -> Horizontal -> HorizontalControlMode*/
	if (g_sUav_Status.UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_SELFAUTO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"F2S", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavControlMode.Horizontal.CONTROL_MODE == UAV_HORIZONTAL_CONTROL_FIX_POS)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"S2F", OLED096_ACSII_6X8);			
	}
	
	/*=== 3.�۲���Ч״̬ ===*/
	/*3.1��ʾAircraftStatus -> Estimate -> Altitude -> beroEstimateAltitude*/
	if (g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_OK)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 6, (u8*)&"OK", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_NO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 6, (u8*)&"NO", OLED096_ACSII_6X8);			
	}
	
	/*3.2��ʾAircraftStatus -> Estimate -> Altitude -> ultrEstimateAltitude*/
	if (g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_OK)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"OK", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_NO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"NO", OLED096_ACSII_6X8);			
	}
	
	/*3.3��ʾAircraftStatus -> Estimate -> Horizontal -> gpsEstimateHorizontal*/
	if (g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_OK)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 108, 6, (u8*)&"OK", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_NO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 108, 6, (u8*)&"NO", OLED096_ACSII_6X8);			
	}	
	
	/*3.4��ʾAircraftStatus -> Estimate -> Horizontal -> opflowEstimateHorizontal*/
	if (g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_OK)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 7, (u8*)&"OK", OLED096_ACSII_6X8);			
	}	
	else if (g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_NO)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 7, (u8*)&"NO", OLED096_ACSII_6X8);			
	}
	
	/*=== 4.��ֱ��ˮƽ�������۲�״̬ ===*/
	/*4.1��ʾAircraftStatus -> Sensor -> estimateAltitude*/
	if ((g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
		(g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_NO))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 7, (u8*)&"X+X", OLED096_ACSII_6X8);		
	}
	else if ((g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
			 (g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_NO))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 7, (u8*)&"B+X", OLED096_ACSII_6X8);			
	}
	else if ((g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
			 (g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_OK))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 7, (u8*)&"X+U", OLED096_ACSII_6X8);			
	}
	else if ((g_sUav_Status.UavSenmodStatus.Vertical.Bero.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
			 (g_sUav_Status.UavSenmodStatus.Vertical.Ultr.DATA_STATUS == UAV_SENMOD_DATA_OK))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 7, (u8*)&"B+U", OLED096_ACSII_6X8);			
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 60, 7, (u8*)&"N+N", OLED096_ACSII_6X8);
	}	

	/*4.2��ʾAircraftStatus -> Sensor -> estimateHorizontal*/
	if ((g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
		(g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_NO))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 7, (u8*)&"X+X", OLED096_ACSII_6X8);			
	}	
	if ((g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
		(g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_NO))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 7, (u8*)&"G+X", OLED096_ACSII_6X8);			
	}	
	if ((g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_NO) && \
		(g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_OK))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 7, (u8*)&"X+O", OLED096_ACSII_6X8);			
	}
	if ((g_sUav_Status.UavSenmodStatus.Horizontal.Gps.DATA_STATUS == UAV_SENMOD_DATA_OK) && \
		(g_sUav_Status.UavSenmodStatus.Horizontal.Opticflow.DATA_STATUS == UAV_SENMOD_DATA_OK))
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 7, (u8*)&"G+O", OLED096_ACSII_6X8);			
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 7, (u8*)&"N+N", OLED096_ACSII_6X8);			
	}
}

/*NO9.ң��״̬��ʾ*/
void hci_Show_Remot_Status(void)
{
/*1.��ʾ����*/
/*    
	                           |-roll
	                           |-pitch
	                |-Attitude |-throttle
	                |          |-yaw
	  |-ChannleData |        
	  |				|       |-vra
	  |             |Gimbal |-vrb
	  |             |
	  |             |       |-swa
	  |             |Switch |-swb
      |                     |-swc
	  |                     |-swd
      |
	  |                        |-roll
Romot |             |-Attitude |-pitch
	  |             |          |-throttle
	  |-ExpectAngle |          |-yaw
	  |             |
	  |             |       |-pitch
      |	            |Gimbal |
      |                     |-roll
      |
	  |                 |-roll
	  |-ExpectAutoAngle |
	                    |-pitch
*/
/*2.���Ű�
NO9: REMOT_DATA     *
1h2000 2h2000 3h2000
4h2000 5h2000 6h2000
7h2000 8h2000 9h2000
EArol: ��90 EApit: ��90	//pitch & roll�����Ƕȷ�Χ-45��~+45��
EAthr:1000 EAyaw:��123	//��������1000~1900, yaw�����Ƕȷ�Χ-150��~+150��
EGpit:��123 EGyaw:��123
EAArol:��90 EAApit:��90
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NO9: REMOT_DATA", OLED096_ACSII_6X8);		
		
		/*��1��*/	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"1h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 1, (u8*)&"2h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 1, (u8*)&"3h", OLED096_ACSII_6X8);
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"4h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 2, (u8*)&"5h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 2, (u8*)&"6h", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"7h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 3, (u8*)&"8h", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 3, (u8*)&"9h", OLED096_ACSII_6X8);	
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"EArol: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"EApit: ", OLED096_ACSII_6X8);

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"EAthr:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"EAyaw:", OLED096_ACSII_6X8);

		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"EGpit:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"EGyaw:", OLED096_ACSII_6X8);		
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"EAArol:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 7, (u8*)&"EAApit:", OLED096_ACSII_6X8);	
		
		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;
	}
	
	/*=== 1.ChannleData ===*/
	/*1.1��ʾRomot -> ChannleData -> Attitude -> roll*/
	math_Integer_Number_Analy(g_psRemotData->AttRoll, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.2��ʾRomot -> ChannleData -> Attitude -> pitch*/
	math_Integer_Number_Analy(g_psRemotData->AttPitch, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 54, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.3��ʾRomot -> ChannleData -> Attitude -> throttle*/
	math_Integer_Number_Analy(g_psRemotData->AttThrottle, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 96, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.4��ʾRomot -> ChannleData -> Attitude -> yaw*/
	math_Integer_Number_Analy(g_psRemotData->AttYaw, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.5��ʾRomot -> ChannleData -> Switch -> swa*/
	math_Integer_Number_Analy(g_psRemotData->SWA, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 54, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.6��ʾRomot -> ChannleData -> Switch -> swb*/
	math_Integer_Number_Analy(g_psRemotData->SWB, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 96, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	/*1.7��ʾRomot -> ChannleData -> Switch -> swc*/
	math_Integer_Number_Analy(g_psRemotData->SWC, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 12, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.8��ʾRomot -> ChannleData -> Switch -> swd*/
	math_Integer_Number_Analy(g_psRemotData->SWD, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 54, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*1.9��ʾRomot -> ChannleData -> gimbal -> vra*/
	math_Integer_Number_Analy(g_psRemotData->GimPitch, 4, &g_sMathIntegerAnaly);
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 96, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	
	/*=== 2.ExpectAngle ===*/
	/*2.1��ʾRomot -> ExpectAngle -> Attitude -> roll*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAngle.roll, 2, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 42, 4, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*2.2��ʾRomot -> ExpectAngle -> Attitude -> pitch*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAngle.pitch, 2, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 108, 4, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*2.3��ʾRomot -> ExpectAngle -> Attitude -> throttle*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAngle.throttle, 4, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer_No_Sign(&g_sOled0_96, 36, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	/*2.4��ʾRomot -> ExpectAngle -> Attitude -> yaw*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAngle.yaw, 3, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 102, 5, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

	/*2.5��ʾRomot -> ExpectAngle -> Gimbal -> pitch*/
	math_Integer_Number_Analy(g_psControlAircraft->GimbalExpectAngle.pitch, 3, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 36, 6, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
	/*2.6��ʾRomot -> ExpectAngle -> Gimbal -> yaw*/
	math_Integer_Number_Analy(g_psControlAircraft->GimbalExpectAngle.yaw, 3, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 102, 6, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	
	/*=== 3.ExpectAutoAngle ===*/
	/*3.1��ʾRomot -> ExpectAutoAngle -> roll*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAutoAngle.roll, 2, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 42, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	
	/*3.2��ʾRomot -> ExpectAutoAngle -> roll*/
	math_Integer_Number_Analy(g_psControlAircraft->RemotExpectAutoAngle.pitch, 2, &g_sMathIntegerAnaly);	
	bsp_OLED0_96_Show_Integer(&g_sOled0_96, 108, 7, OLED096_ACSII_6X8, g_sMathIntegerAnaly);		
}

/*NO10.������У׼���*/
void hci_Show_Sensor_Calib_Result(void)
{
/*1.��ʾ����*/
/*	         	
		                  |-x
			      |-Scale |-y
	              |       |-z
		    |-Acc |
	        |     |        |-x 
	        |	  |-Offset |-y
	        |              |-z
SensorCalib |      
            |              |-x
			|-Mag |-Offset |-y
	                       |-z
*/  
/*2.���Ű�
NOA: SS_CALIB_RESULT *
Acc:Scale&Offset SUCC
SX:��0.9999 SY:��0.9999 
SZ:��0.9999 OX:��0.9999 
OY:��0.9999 OZ:��0.9999 
MAG:Offset       SUCC
OX:��0.9999 OY:��0.9999 
OZ:��0.9999 
*/
	/*�ж��Ƿ�������ǰҳ*/
	if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, '*', OLED096_ACSII_6X8);
	}
	else if (g_sHciShowPage.PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
	{
		bsp_OLED0_96_ShowChar(&g_sOled0_96, 120, 0, ' ', OLED096_ACSII_6X8);	
	}

	/*��ʾ���ֿ��,Ȼ����������*/
	if (g_sHciShowPage.MOULD_STATUS == HCI_SHOW_MOULD_FIRST)
	{
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 0, (u8*)&"NOA: SS_CALIB_RESULT", OLED096_ACSII_6X8);
	
		/*��1��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"Acc:Scale&Offset", OLED096_ACSII_6X8);
	
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"SX:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 2, (u8*)&"SY:", OLED096_ACSII_6X8);
	
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"SZ:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"OX:", OLED096_ACSII_6X8);
	
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"OY:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 4, (u8*)&"OZ:", OLED096_ACSII_6X8);	

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"MAG:Offset", OLED096_ACSII_6X8);
	
		/*��6��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"OX:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"OY", OLED096_ACSII_6X8);	

		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"OZ:", OLED096_ACSII_6X8);
		
		/*���ģ�����ʾ����*/
		g_sHciShowPage.MOULD_STATUS = HCI_SHOW_MOULD_NOTFIRST;		
	}
	
	/*��ʾ���*/
	/*��ʾ���ٶȼ�У׼���*/
	if (g_psAccCalibSystem->RESULT_STATUS == RESULT_CALIB_SUCC)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"SUCC", OLED096_ACSII_6X8);		
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 1, (u8*)&"FAIL", OLED096_ACSII_6X8);	
	}
	
	/*��ʾ������У׼���*/
	if (g_psMagCalibSystem->RESULT_STATUS == RESULT_CALIB_SUCC)
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"SUCC", OLED096_ACSII_6X8);		
	}
	else
	{
		bsp_OLED0_96_ShowString(&g_sOled0_96, 102, 5, (u8*)&"FAIL", OLED096_ACSII_6X8);	
	}

	/*1.��ʾ�Ӵ洢����ȡ�ļ��ٶ�У׼ϵ��ֵ*/
	/*����ϵ��*/
	/*1.1��ʾSensorCalib -> Acc -> Scale -> x*/
	math_Floater_Number_Analy(g_fpAccScaleX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.2��ʾSensorCalib -> Acc -> Scale -> y*/
	math_Floater_Number_Analy(g_fpAccScaleY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*1.3��ʾSensorCalib -> Acc -> Scale -> z*/
	math_Floater_Number_Analy(g_fpAccScaleZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		

	/*��ƫϵ��*/
	/*1.4��ʾSensorCalib -> Acc -> Offset -> x*/
	math_Floater_Number_Analy(g_fpAccOffsetX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 3, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*1.5��ʾSensorCalib -> Acc -> Offset -> y*/
	math_Floater_Number_Analy(g_fpAccOffsetY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*1.6��ʾSensorCalib -> Acc -> Offset -> z*/
	math_Floater_Number_Analy(g_fpAccOffsetZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	


	/*2.��ʾ�Ӵ洢����ȡ�Ĵ�����У׼ϵ��ֵ*/
	/*��ƫϵ��*/
	/*2.1��ʾSensorCalib -> Mag -> Offset -> x*/
	math_Floater_Number_Analy(g_fpMagOffsetX, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
	/*2.2��ʾSensorCalib -> Mag -> Offset -> y*/
	math_Floater_Number_Analy(g_fpMagOffsetY, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

	/*2.3��ʾSensorCalib -> Mag -> Offset -> z*/
	math_Floater_Number_Analy(g_fpMagOffsetZ, 7, &g_sMathFloaterAnaly);
	bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 7, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	
}

/*==== 4.������У׼���� ====*/
/*1.����/�˳����ٶȼ�У׼��ʾ*/
void hci_Show_Acc_Calib_Status(ENTRY_CALIB_STATUS ENTRY_STATUS, CALIB_SIDE_INDEX GONNA_CALIB_SIDE)
{ 
	static U8_Bit_Flag accShowBitFlag = {BIT_FLAG_RESET};
/*2.���Ű�
      ACC_CALIB
	                *
 **** ****  **** ****
 *  * *  *  *  * * **
 ** * *  *  * ** ** *
 * ** *  *  ** * *  *
 **** ****  **** ****
	 *     *
   START       EXIT
*/	
	/*������ٶȼ�У׼��ʾ*/
	if (ENTRY_STATUS == ENTRY_CALIB_INTO)
	{
		/*��ֹ��ʾ��ʾ����*/
		g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_DISABLE;
		
		/*�����ʾ����æµ*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_BUSY;
			
		/*��ʾ������ʾ*/
		switch (GONNA_CALIB_SIDE)
		{
			case SIDE_INDEX_1ST:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit0 == BIT_FLAG_RESET)
				{
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);					
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_1st_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:1ST", OLED096_ACSII_6X8);
					
					/*�������ʾ*/
					accShowBitFlag.bit0 = BIT_FLAG_SET;
				}
			}break;

			case SIDE_INDEX_2ND:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit1 == BIT_FLAG_RESET)
				{				
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_2nd_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:2ND", OLED096_ACSII_6X8);		
					
					/*�������ʾ*/
					accShowBitFlag.bit1 = BIT_FLAG_SET;
				}					
			}break;

			case SIDE_INDEX_3RD:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit2 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_3rd_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:3RD", OLED096_ACSII_6X8);	
					
					/*�������ʾ*/
					accShowBitFlag.bit2 = BIT_FLAG_SET;
				}						
			}break;

			case SIDE_INDEX_4TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit3 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_4th_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:4TH", OLED096_ACSII_6X8);		
					
					/*�������ʾ*/
					accShowBitFlag.bit3 = BIT_FLAG_SET;
				}						
			}break;

			case SIDE_INDEX_5TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit4 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_5th_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:5TH", OLED096_ACSII_6X8);	
					
					/*�������ʾ*/
					accShowBitFlag.bit4 = BIT_FLAG_SET;
				}						
			}break;		
			
			case SIDE_INDEX_6TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit5 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_AccCalib_6th_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:6TH", OLED096_ACSII_6X8);	
					
					/*�������ʾ*/
					accShowBitFlag.bit5 = BIT_FLAG_SET;
				}						
			}break;		

			default:break;
		}
		
		/*��ʾ����*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 37, 0, (u8*)&"ACC_CALIB", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 7, (u8*)&"EXIT", OLED096_ACSII_6X8);	
	}
	/*�˳����ٶȼ�У׼*/	
	else if (ENTRY_STATUS == ENTRY_CALIB_EXIT)
	{
		/*״̬����λ*/
		accShowBitFlag.bit0 = BIT_FLAG_RESET;
		accShowBitFlag.bit1 = BIT_FLAG_RESET;
		accShowBitFlag.bit2 = BIT_FLAG_RESET;
		accShowBitFlag.bit3 = BIT_FLAG_RESET;
		accShowBitFlag.bit4 = BIT_FLAG_RESET;		
		accShowBitFlag.bit5 = BIT_FLAG_RESET;
		
		/*�жϽ�������ʾ����*/
		if (g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) /*�Ѿ�������ʾ,���л���Ĭ����ҳ*/
		{				
			/*�ָ�Ĭ����ʾҳ���*/
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		}	
		else		
		{
			/*������ʾ��ʾ����*/
			g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE;
			
			/*������ʾ����˵���ʾ��ʾҳ��*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;
		}
		
		/*�����ʾ�������*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE;			
	}
}

/*2.����/�˳�������У׼��ʾ*/
void hci_Show_Mag_Calib_Status(ENTRY_CALIB_STATUS ENTRY_STATUS, CALIB_SIDE_INDEX GONNA_CALIB_SIDE)
{
	static U8_Bit_Flag magShowBitFlag = {BIT_FLAG_RESET};		
/*2.���Ű�
      MAG_CALIB
	                *
 **** ****  **** ****
 *  * *  *  *  * * **
 ** * *  *  * ** ** *
 * ** *  *  ** * *  *
 **** ****  **** ****
	 *     *
   START       EXIT
*/	
	/*���������У׼��ʾ*/
	if (ENTRY_STATUS == ENTRY_CALIB_INTO)
	{
		/*��ֹ��ʾ��ʾ����*/
		g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_DISABLE;
		
		/*�����ʾ����æµ*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_BUSY;
			
		/*��ʾ������ʾ*/
		switch (GONNA_CALIB_SIDE)
		{
			case SIDE_INDEX_1ST:
			{
				/*ֻ��ʾһ��*/
				if (magShowBitFlag.bit0 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);					
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_MagCalib_1st_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:1ST", OLED096_ACSII_6X8);
					
					/*�������ʾ*/
					magShowBitFlag.bit0 = BIT_FLAG_SET;
				}
			}break;

			case SIDE_INDEX_2ND:
			{
				/*ֻ��ʾһ��*/
				if (magShowBitFlag.bit1 == BIT_FLAG_RESET)
				{					
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_MagCalib_2nd_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:2ND", OLED096_ACSII_6X8);	

					/*�������ʾ*/
					magShowBitFlag.bit1 = BIT_FLAG_SET;
				}
			}break;

			case SIDE_INDEX_3RD:
			{
				/*ֻ��ʾһ��*/
				if (magShowBitFlag.bit2 == BIT_FLAG_RESET)
				{				
					/*����*/
					bsp_OLED0_96_Clear(&g_sOled0_96);
				
					/*��ʾ��ʾ����*/
					bsp_OLED0_96_DrawBMP(&g_sOled0_96, 0, 1, 128, 6, (u8 *)&g_MagCalib_3rd_Side_48X128);
	
					/*��ʾ��ʾ��*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 17, 7, (u8*)&"S:3RD", OLED096_ACSII_6X8);		

					/*�������ʾ*/
					magShowBitFlag.bit2 = BIT_FLAG_SET;					
				}
			}break;	

			default:break;
		}			
			
		/*��ʾ����*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 37, 0, (u8*)&"MAG_CALIB", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 84, 7, (u8*)&"EXIT", OLED096_ACSII_6X8);			
	}
	/*�˳�������У׼*/	
	else if (ENTRY_STATUS == ENTRY_CALIB_EXIT)
	{
		/*״̬����λ*/
		magShowBitFlag.bit0 = BIT_FLAG_RESET;
		magShowBitFlag.bit1 = BIT_FLAG_RESET;
		magShowBitFlag.bit2 = BIT_FLAG_RESET;		
		
		/*�жϽ�������ʾ����*/
		if (g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) /*�Ѿ�������ʾ,���л���Ĭ����ҳ*/
		{				
			/*�ָ�Ĭ����ʾҳ���*/			
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		}	
		else		
		{
			/*������ʾ��ʾ����*/
			g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE;
			
			/*������ʾ����˵���ʾ��ʾҳ��*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;
		}
		
		/*�����ʾ�������*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE;
	}	
}

/*3.���ٶȼ�У׼��ʾ*/
void hci_Show_Acc_Calib_Process(CALIB_SIDE_INDEX CUR_SIDE_INDEX, SAMPLE_PROCESS_TYPE PROCESS_TYPE, SINGLE_SAMPLE_STATUS SIGSAMPLE_STATUS, fp32 gyroLenth, fp32 xg, fp32 yg, fp32 zg, u32 holdMs)
{
	static volatile CALIB_SIDE_INDEX LAST_SIDE_INDEX = SIDE_INDEX_NULL;
	static U8_Bit_Flag accShowBitFlag = {BIT_FLAG_RESET};
	static u8 showLogXPos  = 30;
	u8 *pstr1, *pstr2;
/*1.��ʾ����*/
/*

*/ 
/*2.���Ű�
    ACC_CALIB_SHOW	
	
SideNbr:04 PitchFront
nextNbr:05 PitchBack
GyroLenth: ��20.000
xg:��0.999  yg:��0.999 
zg:��0.999  RST: SUCC
Log: -0-0-0-0-0-0
*/	
	/*��ʾ���ֿ��,Ȼ����������*/
	if (CUR_SIDE_INDEX != LAST_SIDE_INDEX)
	{
		/*���ģ�����ʾ����*/
		LAST_SIDE_INDEX = CUR_SIDE_INDEX;		
		
		/*��ֹ��ʾ��ʾ����*/
		g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_DISABLE;
		
		/*�����ʾ����æµ*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_BUSY;
		
		/*����*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
		
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 37, 0, (u8*)&"ACC_CALIB", OLED096_ACSII_6X8);
		
		/*��1��*/	
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"SideNbr:", OLED096_ACSII_6X8);				
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"nextNbr:", OLED096_ACSII_6X8);		
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"GyroLenth: ", OLED096_ACSII_6X8);	

		/*��5��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 5, (u8*)&"xg:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 5, (u8*)&"yg:", OLED096_ACSII_6X8);

		/*��6��*/	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 6, (u8*)&"zg:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 6, (u8*)&"RST:", OLED096_ACSII_6X8);		
		
		/*��7��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 7, (u8*)&"Log: ", OLED096_ACSII_6X8);	
	}
	
	/*��ʾ���β��������еĶ�̬����*/
	if (PROCESS_TYPE == SAMPLE_PROCESS_SAMPLEING)
	{
		/*��ʾ����ҳ���*/
		switch(CUR_SIDE_INDEX)
		{
			case SIDE_INDEX_1ST:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit0 == BIT_FLAG_RESET)
				{
					pstr1 = (u8*)&"01 Top       ";
					pstr2 = (u8*)&"02 RollLeft  ";

					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);
		
					/*��ʾ��һҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, pstr2, OLED096_ACSII_6X8);			
					
					/*�������ʾ*/
					accShowBitFlag.bit0 = BIT_FLAG_SET;
				}
			}break;

			case SIDE_INDEX_2ND:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit1 == BIT_FLAG_RESET)
				{				
					pstr1 = (u8*)&"02 RollLeft  ";
					pstr2 = (u8*)&"03 RollRight ";	
			
					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);
		
					/*��ʾ��һҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, pstr2, OLED096_ACSII_6X8);

					/*�������ʾ*/
					accShowBitFlag.bit1 = BIT_FLAG_SET;
				}
			}break;

			case SIDE_INDEX_3RD:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit2 == BIT_FLAG_RESET)
				{				
					pstr1 = (u8*)&"03 RollRight ";
					pstr2 = (u8*)&"04 PitchFront";

					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);
		
					/*��ʾ��һҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, pstr2, OLED096_ACSII_6X8);	
					
					/*�������ʾ*/
					accShowBitFlag.bit2 = BIT_FLAG_SET;					
				}
			}break;

			case SIDE_INDEX_4TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit3 == BIT_FLAG_RESET)
				{				
					pstr1 = (u8*)&"04 PitchFront";
					pstr2 = (u8*)&"05 PitchBack ";	

					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);
		
					/*��ʾ��һҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, pstr2, OLED096_ACSII_6X8);	
					
					/*�������ʾ*/
					accShowBitFlag.bit3 = BIT_FLAG_SET;					
				}					
			}break;

			case SIDE_INDEX_5TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit4 == BIT_FLAG_RESET)
				{					
					pstr1 = (u8*)&"05 PitchBack ";
					pstr2 = (u8*)&"06 Under     ";	

					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);
		
					/*��ʾ��һҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, pstr2, OLED096_ACSII_6X8);
					
					/*�������ʾ*/
					accShowBitFlag.bit4 = BIT_FLAG_SET;											
				}
			}break;

			case SIDE_INDEX_6TH:
			{
				/*ֻ��ʾһ��*/
				if (accShowBitFlag.bit5 == BIT_FLAG_RESET)
				{					
					pstr1 = (u8*)&"06 Under     ";
			
					/*��ʾ��ǰҳ���*/
					bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 2, pstr1, OLED096_ACSII_6X8);			
					
					/*�������ʾ*/
					accShowBitFlag.bit5 = BIT_FLAG_SET;						
				}
			}break;

			default:break;
		}
	
		/*��ʾ�����ǲ���(�жϾ�ֹ)*/
		math_Floater_Number_Analy(gyroLenth, 7, &g_sMathFloaterAnaly);
		bsp_OLED0_96_Show_Floater(&g_sOled0_96, 66, 4, OLED096_ACSII_6X8, g_sMathFloaterAnaly);
	
		/*��ʾx y z���ٶ�ֵ*/
		math_Floater_Number_Analy(xg, 6, &g_sMathFloaterAnaly);
		bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);	

		math_Floater_Number_Analy(yg, 6, &g_sMathFloaterAnaly);
		bsp_OLED0_96_Show_Floater(&g_sOled0_96, 84, 5, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		

		math_Floater_Number_Analy(zg, 6, &g_sMathFloaterAnaly);
		bsp_OLED0_96_Show_Floater(&g_sOled0_96, 18, 6, OLED096_ACSII_6X8, g_sMathFloaterAnaly);		
	}
	/*��ʾ���β����Ľ��*/
	else if (PROCESS_TYPE == SAMPLE_PROCESS_RESULT)
	{
		/*��ʾ���*/
		if (SIGSAMPLE_STATUS == SINGLE_SAMPLE_SUCC)
		{
			/*��ʾ���ν��*/
			bsp_OLED0_96_ShowString(&g_sOled0_96, 96, 6, (u8*)&"SUCC", OLED096_ACSII_6X8);	
		
			/*��ʾ��ʷ���*/		
			bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos, 7, (u8*)&"-0", OLED096_ACSII_6X8);
			showLogXPos += 12;
		}
		else
		{
			/*��ʾ���ν��*/
			bsp_OLED0_96_ShowString(&g_sOled0_96, 96, 6, (u8*)&"FAIL", OLED096_ACSII_6X8);		
		
			/*��ʾ��ʷ���*/	
			bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos, 7, (u8*)&"-1", OLED096_ACSII_6X8);
			showLogXPos += 12;		
		}
	}
	
	/*��ʱ*/
	sys_DelayMs(holdMs);
	
	/*����������*/
	if ((CUR_SIDE_INDEX == SIDE_INDEX_6TH) && \
		(PROCESS_TYPE == SAMPLE_PROCESS_RESULT))
	{
		/*״̬����λ*/
		LAST_SIDE_INDEX     = SIDE_INDEX_NULL;
		accShowBitFlag.bit0 = BIT_FLAG_RESET;
		accShowBitFlag.bit1 = BIT_FLAG_RESET;
		accShowBitFlag.bit2 = BIT_FLAG_RESET;
		accShowBitFlag.bit3 = BIT_FLAG_RESET;
		accShowBitFlag.bit4 = BIT_FLAG_RESET;		
		accShowBitFlag.bit5 = BIT_FLAG_RESET;
		
		/*����㸴λ*/
		showLogXPos = 30;
		
		/*����*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
		
		/*�жϽ�������ʾ����*/
		if (g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) /*�Ѿ�������ʾ,���л���Ĭ����ҳ*/
		{
			/*�ָ�Ĭ����ʾҳ���*/			
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		}
		else		
		{		
			/*������ʾ��ʾ����*/
			g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE;
			
			/*������ʾ����˵���ʾ��ʾҳ��*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;		
		}
		
		/*�����ʾ�������*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE;		
	}
}

/*4.������У׼��ʾ*/
void hci_Show_Mag_Calib_Process(CALIB_SIDE_INDEX CUR_SIDE_INDEX, CALIB_POSITION_INDEX POSITION_INDEX, POSITION_SAMPLE_STATUS POS_SAMP_STATUS, SAMPLE_PROCESS_TYPE PROCESS_TYPE, u16 curYaw, u16 targetYaw, u8 errorMaxYaw, s16 targAccAxis, u32 holdMs)
{
	static volatile CALIB_SIDE_INDEX LAST_SIDE_INDEX = SIDE_INDEX_NULL;
	static U8_Bit_Flag magShowBitFlag = {BIT_FLAG_RESET};
	static u8 showLogXPos1 = 48;
	static u8 showLogXPos2 = 0;
	static u8 showLogXPos3 = 0;
	static u8 showLogXPos4 = 0;
/*1.��ʾ����*/
/*

*/ 	
/*2.���Ű�
	MAG_CALIB_SHOW
cPos: 1-03 nPos: 1-04
cYaw:360 tYaw:360 r10
AccAxis:+x Val: ��8192 
LOG:SUCC-0-0-0-0-0-0
-0-0-0-0-0-0-0-0-0-0
-0-0-0-0-0-0-0-0-0-0
-0-0-0-0-0-0-0-0-0-0

*/
	/*��ʾ���ֿ��,Ȼ����������*/
	if (CUR_SIDE_INDEX != LAST_SIDE_INDEX)
	{
		/*���ģ�����ʾ����*/			
		LAST_SIDE_INDEX = CUR_SIDE_INDEX;
		
		/*��ֹ��ʾ��ʾ����*/
		g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_DISABLE;
		
		/*�����ʾ����æµ*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_BUSY;
		
		/*����*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
		
		/*��0��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 37, 0, (u8*)&"MAG_CALIB", OLED096_ACSII_6X8);				

		/*��1��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 1, (u8*)&"cPos: ", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 42, 1, (u8*)&"-", OLED096_ACSII_6X8);		
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 1, (u8*)&"nPos: ", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 108, 1, (u8*)&"-", OLED096_ACSII_6X8);			
		
		/*��2��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 2, (u8*)&"cYaw:", OLED096_ACSII_6X8);
		bsp_OLED0_96_ShowString(&g_sOled0_96, 54, 2, (u8*)&"tYaw:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 108, 2, (u8*)&"r", OLED096_ACSII_6X8);			
		
		/*��3��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 3, (u8*)&"AccAxis:", OLED096_ACSII_6X8);	
		bsp_OLED0_96_ShowString(&g_sOled0_96, 66, 3, (u8*)&"Val: ", OLED096_ACSII_6X8);
		
		/*��4��*/
		bsp_OLED0_96_ShowString(&g_sOled0_96, 0, 4, (u8*)&"LOG:", OLED096_ACSII_6X8);				
	}
	
	/*��ʾ���β��������еĶ�̬����*/
	if (PROCESS_TYPE == SAMPLE_PROCESS_SAMPLEING)
	{
		/*��ʾ����ҳ���*/
		switch(CUR_SIDE_INDEX)
		{
			case SIDE_INDEX_1ST:
			{
				/*ֻ��ʾһ��*/				
				if (magShowBitFlag.bit0 == BIT_FLAG_RESET)
				{
					/*���������*/
					bsp_OLED0_96_ShowNum(&g_sOled0_96, 36, 1, SIDE_INDEX_1ST + 1, 1, OLED096_ACSII_6X8);
			
					/*���νǵ����*/
					math_Integer_Number_Analy(POSITION_INDEX + 1, 2, &g_sMathIntegerAnaly);
					bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 48, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);			
			
					/*���νǵ������1~35,���´���2~36*/
					if (POSITION_INDEX <= POSITION_INDEX_36TH)
					{
						/*�´������*/
						bsp_OLED0_96_ShowNum(&g_sOled0_96, 102, 1, SIDE_INDEX_1ST + 1, 1, OLED096_ACSII_6X8);
			
						/*�´νǵ����*/
						math_Integer_Number_Analy(POSITION_INDEX + 1 + 1, 2, &g_sMathIntegerAnaly);				
						bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);				
					}
			
					/*������1-36,�´���2-1*/
					else if (POSITION_INDEX >= POSITION_INDEX_36TH)
					{
						/*�´������*/
						bsp_OLED0_96_ShowNum(&g_sOled0_96, 102, 1, SIDE_INDEX_2ND + 1, 1, OLED096_ACSII_6X8);
			
						/*�´νǵ����*/
						math_Integer_Number_Analy(POSITION_INDEX_1ST + 1, 2, &g_sMathIntegerAnaly);				
						bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);				
					}
					
					/*�������ʾ*/
					magShowBitFlag.bit0 = BIT_FLAG_SET;		
				}
			}break;

			case SIDE_INDEX_2ND:
			{
				/*ֻ��ʾһ��*/				
				if (magShowBitFlag.bit1 == BIT_FLAG_RESET)
				{				
					/*���������*/
					bsp_OLED0_96_ShowNum(&g_sOled0_96, 36, 1, SIDE_INDEX_2ND + 1, 1, OLED096_ACSII_6X8);
					
					/*���νǵ����*/
					math_Integer_Number_Analy(POSITION_INDEX + 1, 2, &g_sMathIntegerAnaly);
					bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 48, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
					
					/*���νǵ������1~35,���´���2~36*/
					if (POSITION_INDEX <= POSITION_INDEX_35TH)
					{
						/*�´������*/
						bsp_OLED0_96_ShowNum(&g_sOled0_96, 102, 1, SIDE_INDEX_2ND + 1, 1, OLED096_ACSII_6X8);
					
						/*�´νǵ����*/
						math_Integer_Number_Analy(POSITION_INDEX + 1 + 1, 2, &g_sMathIntegerAnaly);				
						bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);				
					}
					/*������2-36,�´���3-1*/
					else if (POSITION_INDEX >= POSITION_INDEX_36TH)
					{			
						/*�´������*/
						bsp_OLED0_96_ShowNum(&g_sOled0_96, 102, 1, SIDE_INDEX_3RD + 1, 1, OLED096_ACSII_6X8);
				
						/*�´νǵ����*/
						math_Integer_Number_Analy(POSITION_INDEX_1ST + 1, 2, &g_sMathIntegerAnaly);				
						bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);						
					}
					
					/*�������ʾ*/
					magShowBitFlag.bit1 = BIT_FLAG_SET;						
				}
			}break;
			
			case SIDE_INDEX_3RD:
			{
				/*ֻ��ʾһ��*/				
				if (magShowBitFlag.bit2 == BIT_FLAG_RESET)
				{					
					/*���������*/
					bsp_OLED0_96_ShowNum(&g_sOled0_96, 36, 1, SIDE_INDEX_3RD + 1, 1, OLED096_ACSII_6X8);
				
					/*���νǵ����*/
					math_Integer_Number_Analy(POSITION_INDEX + 1, 2, &g_sMathIntegerAnaly);
					bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 48, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);
	
					/*���νǵ������1~35,���´���2~36*/
					if (POSITION_INDEX <= POSITION_INDEX_35TH)
					{
						/*�´������*/
						bsp_OLED0_96_ShowNum(&g_sOled0_96, 102, 1, SIDE_INDEX_3RD + 1, 1, OLED096_ACSII_6X8);
				
						/*�´νǵ����*/
						math_Integer_Number_Analy(POSITION_INDEX + 1 + 1, 2, &g_sMathIntegerAnaly);				
						bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 1, OLED096_ACSII_6X8, g_sMathIntegerAnaly);				
					}
					/*������3-36,�´���N-N*/
					else if (POSITION_INDEX >= POSITION_INDEX_36TH)
					{
						/*�´������*/
						bsp_OLED0_96_ShowChar(&g_sOled0_96, 102, 1, 'N', OLED096_ACSII_6X8);				
					
						/*�´νǵ����*/
						bsp_OLED0_96_ShowChar(&g_sOled0_96, 114, 1, 'N', OLED096_ACSII_6X8);					
					}
					
					/*�������ʾ*/
					magShowBitFlag.bit2 = BIT_FLAG_SET;						
				}
			}break;			
	
			default:break;
		}
	
		/*��ǰYAW��,Ŀ��Yaw��,��Ч��Χr*/
		/*��ǰYAW��*/
		math_Integer_Number_Analy(curYaw, 3, &g_sMathIntegerAnaly);
		bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 30, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	

		/*Ŀ��YAW��*/
		math_Integer_Number_Analy(targetYaw, 3, &g_sMathIntegerAnaly);
		bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 84, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);		
	
		/*��Ч��Χ*/
		math_Integer_Number_Analy(errorMaxYaw, 2, &g_sMathIntegerAnaly);
		bsp_OLED0_96_Show_Calendar(&g_sOled0_96, 114, 2, OLED096_ACSII_6X8, g_sMathIntegerAnaly);		
	
		/*У׼ʱ��ֱ���ϵ���*/
		if (CUR_SIDE_INDEX == SIDE_INDEX_1ST)
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, (u8*)&"+Z", OLED096_ACSII_6X8);	
		}
		else if (CUR_SIDE_INDEX == SIDE_INDEX_2ND)
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, (u8*)&"+Y", OLED096_ACSII_6X8);			
		}
		else if (CUR_SIDE_INDEX == SIDE_INDEX_3RD)
		{
			bsp_OLED0_96_ShowString(&g_sOled0_96, 48, 3, (u8*)&"+X", OLED096_ACSII_6X8);			
		}		
	
		/*������ٶ�ֵ*/
		math_Integer_Number_Analy(targAccAxis, 4, &g_sMathIntegerAnaly);
		bsp_OLED0_96_Show_Integer(&g_sOled0_96, 96, 3, OLED096_ACSII_6X8, g_sMathIntegerAnaly);	
	}
	/*��ʾ���β����Ľ��*/
	else if (PROCESS_TYPE == SAMPLE_PROCESS_RESULT)
	{
		/*����(���汾�ǵ�������)*/
		if (CUR_SIDE_INDEX == SIDE_INDEX_1ST)
		{
			if (POS_SAMP_STATUS == POSITION_SAMPLE_SUCC)
			{
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"SUCC", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos4 += 12;					
				}
			}
			else if (POS_SAMP_STATUS == POSITION_SAMPLE_FAIL)
			{
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"FAIL", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/		
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos4 += 12;
				}			
			}
		}
		else if (CUR_SIDE_INDEX == SIDE_INDEX_2ND)
		{
			if (POS_SAMP_STATUS == POSITION_SAMPLE_SUCC)
			{	
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"SUCC", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/		
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos4 += 12;					
				}
			}
			else if (POS_SAMP_STATUS == POSITION_SAMPLE_FAIL)
			{
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"FAIL", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/		
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos4 += 12;
				}
			}		
		}
		else if (CUR_SIDE_INDEX == SIDE_INDEX_3RD)
		{
			if (POS_SAMP_STATUS == POSITION_SAMPLE_SUCC)
			{
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"SUCC", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/		
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-0", OLED096_ACSII_6X8);
					showLogXPos4 += 12;					
				}	
			}
			else if (POS_SAMP_STATUS == POSITION_SAMPLE_FAIL)
			{
				/*��ʾ���ν��*/
				bsp_OLED0_96_ShowString(&g_sOled0_96, 24, 4, (u8*)&"FAIL", OLED096_ACSII_6X8);	
		
				/*��ʾ��ʷ���*/		
				if (POSITION_INDEX <= POSITION_INDEX_6TH)
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos1, 4, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos1 += 12;					
				}
				else if ((POSITION_INDEX_7TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_16TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos2, 5, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos2 += 12;					
				}
				else if ((POSITION_INDEX_17TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_26TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos3, 6, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos3 += 12;					
				}
				else if ((POSITION_INDEX_27TH <= POSITION_INDEX) && \
						 (POSITION_INDEX <= POSITION_INDEX_36TH))
				{
					bsp_OLED0_96_ShowString(&g_sOled0_96, showLogXPos4, 7, (u8*)&"-1", OLED096_ACSII_6X8);
					showLogXPos4 += 12;
				}
			}
		}
	}
	
	/*����У׼���,���긴λ*/
	if (PROCESS_TYPE == SAMPLE_PROCESS_RESULT)
	{
		showLogXPos1 = 48;
		showLogXPos2 = 0;	
		showLogXPos3 = 0;
		showLogXPos4 = 0;
	}
		
	/*��ʱ*/
	sys_DelayMs(holdMs);
		
	/*У׼�������*/
	if ((CUR_SIDE_INDEX == SIDE_INDEX_3RD) && \
		(POSITION_INDEX == POSITION_INDEX_36TH) && \
		(PROCESS_TYPE == SAMPLE_PROCESS_RESULT))
	{
		/*״̬����λ*/
		LAST_SIDE_INDEX     = SIDE_INDEX_NULL;
		magShowBitFlag.bit0 = BIT_FLAG_RESET;		
		magShowBitFlag.bit1 = BIT_FLAG_RESET;
		magShowBitFlag.bit2 = BIT_FLAG_RESET;			
		
		/*����*/
		bsp_OLED0_96_Clear(&g_sOled0_96);
		
		/*�жϽ�������ʾ����*/
		if (g_sHciShowPage.SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE) /*�Ѿ�������ʾ,���л���Ĭ����ҳ*/
		{							
			/*�ָ�Ĭ����ʾҳ���*/			
			g_sHciShowPage.curPageIndex  = HCI_SHOW_PAGE_0;
			g_sHciShowPage.lastPageIndex = (HCI_SHOW_PAGE_INDEX)(g_sHciShowPage.curPageIndex + 1);
		}
		else
		{
			/*������ʾ��ʾ����*/
			g_sHciShowPage.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE;	
			
			/*������ʾ����˵���ʾ��ʾҳ��*/
			g_sHciShowPage.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST;
		}
		
		/*�����ʾ�������*/
		g_sHciShowPage.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE;		
	}	
}

/*=== ң�����HCI ===*/
HciShowPage g_sHciShowPage = 
{
	.curPageIndex     = HCI_SHOW_PAGE_0,	         /*�ӵ�0ҳ��ʼ��ʾ*/
	.lastPageIndex    = HCI_SHOW_PAGE_0,             /*�ϴ���ʾҳ��*/
	.MOULD_STATUS     = HCI_SHOW_MOULD_FIRST,        /*��һ����ʾ*/
	.SHOW_DATA_STATUS = UAV_HCI_SHOW_DISABLE,   /*Ĭ�ϲ���ʾ*/
	.PAGE_STATUS      = UAV_HCI_SHOW_FREE_PAGE, /*Ĭ�ϲ�����������ʾҳ*/
	.SHOW_HINT_STATUS = UAV_HCI_SHOW_ENABLE,    /*Ĭ����ʾ��ʾҳ*/
	.EXIT_SHOW_STATUS = HCI_EXIT_SHOW_OP_FIRST,      /*�˳�����״̬*/
	.SHOW_TASK_STATUS = HCI_SHOW_TASK_IDLE,          /*Ĭ�Ͽ���*/
};

vu16 g_vu16ShowEnableContinueTicks     = 0;
vu16 g_vu16ShowSwitchNextContinueTicks = 0;
vu16 g_vu16ShowSwitchLastContinueTicks = 0;
vu16 g_vu16ShowHoldContinueTicks       = 0;

/*=== ң�����HCI ===*/
/*HCI(OLED):������ʾ/��ֹ��ʾ/��ʾ��һҳ/��ʾ��һҳ*/
void hci_remot_switch_show_status(HciShowPage *hciShowPage)
{
	/*1.����/�ر�OLED��ʾ*/
	/*
	       *  	      *	
		*******       *******
		*  *  *       * *   *
		*  *  *       *  *  * 
		*  *  *       *     *
		*******       *******		                        
	       *
	*/
	/*����״̬ + ҡ��λ��(���ٹ�������ҡ��λ��)*/
	if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
		(remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MIN)     == REMOT_DATA_MIN) && \
		(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MAX)    == REMOT_DATA_MAX) && \
		(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MID)      == REMOT_DATA_MID))
	{
		g_vu16ShowEnableContinueTicks++;
		
		/*��Ŀ��Ticks��0*/
		g_vu16ShowSwitchNextContinueTicks = 0;
		g_vu16ShowSwitchLastContinueTicks = 0;
		g_vu16ShowHoldContinueTicks       = 0;
		
		if (g_vu16ShowEnableContinueTicks > (HCI_PAGE_CHANGE_MIN_HOLD_TIME_MS / RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS))
		{
			/*���һ�μ��,Ŀ��Ticks��0*/
			g_vu16ShowEnableContinueTicks = 0;
			
			/*��ʾʹ���벻ʹ�� ״̬�л�*/
			if (hciShowPage->SHOW_DATA_STATUS == UAV_HCI_SHOW_DISABLE)
			{
				hciShowPage->SHOW_DATA_STATUS = UAV_HCI_SHOW_ENABLE;
			}
			else if (hciShowPage->SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE)
			{
				hciShowPage->SHOW_DATA_STATUS = UAV_HCI_SHOW_DISABLE;
			}
		}
	}

	/*2.������ʾ��,������ʾҳ��*/	
	/*����״̬ + ҡ��λ��(���ٹ�������ҡ��λ��)*/
	/*1.�ж��л�����һҳ����*/
	/*	   
		   *
		*******       *******
		*  *  *       *     *
		*  *  *       *  ****** 
		*  *  *       *     *
		*******       *******
	       *
	*/		
	if (hciShowPage->SHOW_DATA_STATUS == UAV_HCI_SHOW_ENABLE)
	{
	
		if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
			(remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MAX)     == REMOT_DATA_MAX) && \
			(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MID)    == REMOT_DATA_MID) && \
			(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MID)      == REMOT_DATA_MID))
		{	
			g_vu16ShowSwitchNextContinueTicks++;
			
			/*����Tick��0*/
			g_vu16ShowEnableContinueTicks     = 0;
			g_vu16ShowSwitchLastContinueTicks = 0;
			g_vu16ShowHoldContinueTicks       = 0;
			
			if (g_vu16ShowSwitchNextContinueTicks >= (HCI_PAGE_CHANGE_MIN_HOLD_TIME_MS / RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS))
			{
				/*���һ�μ��,Ŀ��Ticks��0*/
				g_vu16ShowSwitchNextContinueTicks = 0;	
				
				/*�жϵ�ǰҳ�Ƿ�����*/
				if (hciShowPage->PAGE_STATUS != UAV_HCI_SHOW_HOLD_PAGE)
				{
					/*��ʾ��Խ�����*/
					if (hciShowPage->curPageIndex != (HCI_SHOW_PAGE_INDEX)(HCI_TOTAL_SHOW_PAGE_NUMBER - 1))
					{
						
						hciShowPage->curPageIndex++; /*û������������ҳ���������*/						
					}
					else
					{
						hciShowPage->curPageIndex = HCI_SHOW_PAGE_0; /*��ʾ�żӵ����޴�0ҳ��ʼ*/
					}	
				}
				else
				{
					hciShowPage->curPageIndex = hciShowPage->lastPageIndex; /*������ҳ��,������ʾ�ŵ����ϴ���ʾ��*/
				}
			}
		}	

		/*2.�ж��л�����һҳ����*/
		/*����״̬ + ҡ��λ��(���ٹ�������ҡ��λ��)*/		
		/*	   	
               *		
			*******       *******
			*  *  *       *     *
			*  *  *     ******  *
			*  *  *       *     *
			*******       *******
		       *
		*/		
		if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
			(remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MIN)     == REMOT_DATA_MIN) && \
			(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MID)    == REMOT_DATA_MID) && \
			(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MID)      == REMOT_DATA_MID))
		{	
			g_vu16ShowSwitchLastContinueTicks++;				
				
			/*����Tick��0*/
			g_vu16ShowEnableContinueTicks     = 0;
			g_vu16ShowSwitchNextContinueTicks = 0;
			g_vu16ShowHoldContinueTicks       = 0;
			
			if (g_vu16ShowSwitchLastContinueTicks >= (HCI_PAGE_CHANGE_MIN_HOLD_TIME_MS / RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS))
			{
				/*���һ�μ��,Ŀ��Ticks��0*/
				g_vu16ShowSwitchLastContinueTicks = 0;			
				
				/*�жϵ�ǰҳ�Ƿ�����*/
				if (hciShowPage->PAGE_STATUS != UAV_HCI_SHOW_HOLD_PAGE)
				{				
					/*��ʾ��Խ�����*/
					if (hciShowPage->curPageIndex != HCI_SHOW_PAGE_0)
					{

						hciShowPage->curPageIndex--; /*û������������ҳ����ż���*/
					}	
					else
					{
						hciShowPage->curPageIndex = (HCI_SHOW_PAGE_INDEX)(HCI_TOTAL_SHOW_PAGE_NUMBER - 1); /*��ʾ�ż������޴����һҳ��ʼ*/
					}					
				}
				else
				{
					hciShowPage->curPageIndex = hciShowPage->lastPageIndex; /*������ҳ��,������ʾ�ŵ����ϴ���ʾ��*/
				}
			}
		}		
		
		/*3.��ǰҳ�������ͽ���*/
		/*����״̬ + ҡ��λ��(���ٹ�������ҡ��λ��)*/			
		/*	   *  	 					
			*******       *******
			*  *  *       *     *
			*  *  *       *  *  *
			*  *  *       * *   *
			*******       *******
		       *          *
		*/		
		if ((g_sUav_Status.LOCK_STATUS == UAV_LOCK_YES) && \
			(remot_Data_Range(g_sRemotData.AttRoll, REMOT_DATA_MIN)     == REMOT_DATA_MIN) && \
			(remot_Data_Range(g_sRemotData.AttPitch, REMOT_DATA_MIN)    == REMOT_DATA_MIN) && \
			(remot_Data_Range(g_sRemotData.AttYaw, REMOT_DATA_MID)      == REMOT_DATA_MID))
		{	
			g_vu16ShowHoldContinueTicks++;
			
			/*����Tick��0*/
			g_vu16ShowEnableContinueTicks     = 0;
			g_vu16ShowSwitchNextContinueTicks = 0;
			g_vu16ShowSwitchLastContinueTicks = 0;
			
			if (g_vu16ShowHoldContinueTicks >= (HCI_PAGE_CHANGE_MIN_HOLD_TIME_MS / RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS))
			{
				/*���һ�μ��,Ŀ��Ticks��0*/
				g_vu16ShowHoldContinueTicks = 0;			

				/*��ʾʹ���벻ʹ�� ״̬�л�*/
				if (hciShowPage->PAGE_STATUS == UAV_HCI_SHOW_FREE_PAGE)
				{
					hciShowPage->PAGE_STATUS = UAV_HCI_SHOW_HOLD_PAGE;
				}
				else if (hciShowPage->PAGE_STATUS == UAV_HCI_SHOW_HOLD_PAGE)
				{
					hciShowPage->PAGE_STATUS = UAV_HCI_SHOW_FREE_PAGE;
				}				
			}
		}	
	}
}
