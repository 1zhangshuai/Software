#include "filter_DataProcess.h"

FilterTarg g_sFilterTarg;
FilterTarg *g_psFilterTarg = &g_sFilterTarg;

/************** 1. �˲�����ʼ�� **************/
/*ë���˲�����ʼ��*/
void filter_Base_Init(FilterBase *filterBase, fp32 maxAbs)
{
	u8 i;
	
	filterBase->lastVal = 0.0f;
	filterBase->maxAbs  = maxAbs;
	filterBase->index   = 0.0f;
	filterBase->isUp    = 0.0f;
	
	for (i = 0; i < FILTER_BASE_BUFF_LEN; i++)
	{
		filterBase->wvBuff[i] = sin(((fp32)i / (fp32)FILTER_BASE_BUFF_LEN) * (3.1415926f / 200));
	}
}

/*�����˲�����ʼ��*/
void filter_Slider_Init(FilterSlider *filterSlider, u8 arrayLen)
{
	u8 i;
	
	filterSlider->arrayLen = arrayLen;
	
	for (i = 0; i < FILTER_SLIDE_BUFF_LEN; i++)
	{
		filterSlider->array[i] = 0.0f;
	}
}

/*������˹���׵�ͨ�˲�����ʼ��:MATLAB����*/
void filter_LpButterworth_Matlab_Init(void)
{
	/*1. ACC ���ٶȰ�����˹��ͨ�˲�*/
	g_sFilterTarg.AccLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;
	g_sFilterTarg.AccLpBwPara[0].FS    = 200;
	g_sFilterTarg.AccLpBwPara[0].FC    = 80;
	g_sFilterTarg.AccLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[0].A[1]  = 1.14298050254;
	g_sFilterTarg.AccLpBwPara[0].A[2]  = 0.4128015980962;
	g_sFilterTarg.AccLpBwPara[0].B[0]  = 0.638945525159;
	g_sFilterTarg.AccLpBwPara[0].B[1]  = 1.277891050318;
	g_sFilterTarg.AccLpBwPara[0].B[2]  = 0.638945525159;  
	
	g_sFilterTarg.AccLpBwPara[1].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.AccLpBwPara[1].FS    = 200;
	g_sFilterTarg.AccLpBwPara[1].FC    = 60;
	g_sFilterTarg.AccLpBwPara[1].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[1].A[1]  = 0.3695273773512;
	g_sFilterTarg.AccLpBwPara[1].A[2]  = 0.1958157126558;
	g_sFilterTarg.AccLpBwPara[1].B[0]  = 0.3913357725018;
	g_sFilterTarg.AccLpBwPara[1].B[1]  = 0.7826715450035;
	g_sFilterTarg.AccLpBwPara[1].B[2]  = 0.3913357725018;
                            
	g_sFilterTarg.AccLpBwPara[2].FTYPE = FILTER_TYPE_LOWPASS;							
	g_sFilterTarg.AccLpBwPara[2].FS    = 200;
	g_sFilterTarg.AccLpBwPara[2].FC    = 51;
	g_sFilterTarg.AccLpBwPara[2].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[2].A[1]  = 0.03680751639284;
	g_sFilterTarg.AccLpBwPara[2].A[2]  = 0.1718123812701;
	g_sFilterTarg.AccLpBwPara[2].B[0]  = 0.3021549744157;
	g_sFilterTarg.AccLpBwPara[2].B[1]  = 0.6043099488315;
	g_sFilterTarg.AccLpBwPara[2].B[2]  = 0.3021549744157;	
	                   
	g_sFilterTarg.AccLpBwPara[3].FTYPE = FILTER_TYPE_LOWPASS;					   
	g_sFilterTarg.AccLpBwPara[3].FS    = 200;
	g_sFilterTarg.AccLpBwPara[3].FC    = 30;
	g_sFilterTarg.AccLpBwPara[3].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[3].A[1]  = -0.7477891782585;
	g_sFilterTarg.AccLpBwPara[3].A[2]  = 0.272214937925;
	g_sFilterTarg.AccLpBwPara[3].B[0]  = 0.1311064399166;
	g_sFilterTarg.AccLpBwPara[3].B[1]  = 0.2622128798333;
	g_sFilterTarg.AccLpBwPara[3].B[2]  = 0.1311064399166;
                        
	g_sFilterTarg.AccLpBwPara[4].FTYPE = FILTER_TYPE_LOWPASS;						
	g_sFilterTarg.AccLpBwPara[4].FS    = 200;
	g_sFilterTarg.AccLpBwPara[4].FC    = 20;
	g_sFilterTarg.AccLpBwPara[4].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[4].A[1]  = -1.14298050254;
	g_sFilterTarg.AccLpBwPara[4].A[2]  = 0.4128015980962;
	g_sFilterTarg.AccLpBwPara[4].B[0]  = 0.06745527388907;
	g_sFilterTarg.AccLpBwPara[4].B[1]  = 0.1349105477781;
	g_sFilterTarg.AccLpBwPara[4].B[2]  = 0.06745527388907;
	                        
	g_sFilterTarg.AccLpBwPara[5].FTYPE = FILTER_TYPE_LOWPASS;							
	g_sFilterTarg.AccLpBwPara[5].FS    = 200;
	g_sFilterTarg.AccLpBwPara[5].FC    = 15;
	g_sFilterTarg.AccLpBwPara[5].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[5].A[1]  = -1.348967745253;
	g_sFilterTarg.AccLpBwPara[5].A[2]  = 0.5139818942197;
	g_sFilterTarg.AccLpBwPara[5].B[0]  = 0.04125353724172;
	g_sFilterTarg.AccLpBwPara[5].B[1]  = 0.08250707448344;
	g_sFilterTarg.AccLpBwPara[5].B[2]  = 0.04125353724172;	
	                     
	g_sFilterTarg.AccLpBwPara[6].FTYPE = FILTER_TYPE_LOWPASS;
	g_sFilterTarg.AccLpBwPara[6].FS    = 200;
	g_sFilterTarg.AccLpBwPara[6].FC    = 10;
	g_sFilterTarg.AccLpBwPara[6].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[6].A[1]  = -1.561018075801;
	g_sFilterTarg.AccLpBwPara[6].A[2]  = 0.6413515380576;
	g_sFilterTarg.AccLpBwPara[6].B[0]  = 0.02008336556421;
	g_sFilterTarg.AccLpBwPara[6].B[1]  = 0.04016673112842;
	g_sFilterTarg.AccLpBwPara[6].B[2]  = 0.02008336556421;
                      
	g_sFilterTarg.AccLpBwPara[7].FTYPE = FILTER_TYPE_LOWPASS;					  
	g_sFilterTarg.AccLpBwPara[7].FS    = 200;
	g_sFilterTarg.AccLpBwPara[7].FC    = 5;
	g_sFilterTarg.AccLpBwPara[7].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[7].A[1]  = -1.778631777825;
	g_sFilterTarg.AccLpBwPara[7].A[2]  = 0.8008026466657;
	g_sFilterTarg.AccLpBwPara[7].B[0]  = 0.005542717210281;
	g_sFilterTarg.AccLpBwPara[7].B[1]  = 0.01108543442056;
	g_sFilterTarg.AccLpBwPara[7].B[2]  = 0.005542717210281; 
	
	g_sFilterTarg.AccLpBwPara[8].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.AccLpBwPara[8].FS    = 200;
	g_sFilterTarg.AccLpBwPara[8].FC    = 2;
	g_sFilterTarg.AccLpBwPara[8].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[8].A[1]  = -1.911197067426;
	g_sFilterTarg.AccLpBwPara[8].A[2]  = 0.9149758348014;
	g_sFilterTarg.AccLpBwPara[8].B[0]  = 0.0009446918438402;
	g_sFilterTarg.AccLpBwPara[8].B[1]  = 0.00188938368768;
	g_sFilterTarg.AccLpBwPara[8].B[2]  = 0.0009446918438402;   
	
	g_sFilterTarg.AccLpBwPara[9].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.AccLpBwPara[9].FS    = 200;
	g_sFilterTarg.AccLpBwPara[9].FC    = 1;
	g_sFilterTarg.AccLpBwPara[9].A[0]  = 1;
	g_sFilterTarg.AccLpBwPara[9].A[1]  = -1.955578240315;
	g_sFilterTarg.AccLpBwPara[9].A[2]  = 0.9565436765112;
	g_sFilterTarg.AccLpBwPara[9].B[0]  = 0.000241359049042;
	g_sFilterTarg.AccLpBwPara[9].B[1]  = 0.000482718098084;
	g_sFilterTarg.AccLpBwPara[9].B[2]  = 0.000241359049042;
	
	/*1.1 ACC ���ٶȰ�����˹�����˲�*/
	g_sFilterTarg.AccBsBwPara[0].FTYPE    = FILTER_TYPE_BANDSTOP;
	g_sFilterTarg.AccBsBwPara[0].FS       = 200;
	g_sFilterTarg.AccBsBwPara[0].bandLow  = 30;
	g_sFilterTarg.AccBsBwPara[0].bandHigh = 98;
	g_sFilterTarg.AccBsBwPara[0].A[0]  	  = 1;
	g_sFilterTarg.AccBsBwPara[0].A[1]  	  = 0.6270403082828;
	g_sFilterTarg.AccBsBwPara[0].A[2]  	  = -0.2905268567319;
	g_sFilterTarg.AccBsBwPara[0].B[0]  	  = 0.354736571634;
	g_sFilterTarg.AccBsBwPara[0].B[1]  	  = 0.6270403082828;
	g_sFilterTarg.AccBsBwPara[0].B[2]  	  = 0.354736571634;	
	
	g_sFilterTarg.AccBsBwPara[1].FTYPE    = FILTER_TYPE_BANDSTOP;
	g_sFilterTarg.AccBsBwPara[1].FS       = 200;
	g_sFilterTarg.AccBsBwPara[1].bandLow  = 30;
	g_sFilterTarg.AccBsBwPara[1].bandHigh = 94;
	g_sFilterTarg.AccBsBwPara[1].A[0]     = 1;
	g_sFilterTarg.AccBsBwPara[1].A[1]     = 0.5334540355829;
	g_sFilterTarg.AccBsBwPara[1].A[2]     = -0.2235264828971;
	g_sFilterTarg.AccBsBwPara[1].B[0]     = 0.3882367585514;
	g_sFilterTarg.AccBsBwPara[1].B[1]     = 0.5334540355829;
	g_sFilterTarg.AccBsBwPara[1].B[2]     = 0.3882367585514;	
	
	/*2.GYRO ���ٶȰ�����˹�˲�*/
	g_sFilterTarg.GyroLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.GyroLpBwPara[0].FS    = 200;
	g_sFilterTarg.GyroLpBwPara[0].FC    = 51;
	g_sFilterTarg.GyroLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.GyroLpBwPara[0].A[1]  = 0.03680751639284;
	g_sFilterTarg.GyroLpBwPara[0].A[2]  = 0.1718123812701;
	g_sFilterTarg.GyroLpBwPara[0].B[0]  = 0.3021549744157;
	g_sFilterTarg.GyroLpBwPara[0].B[1]  = 0.6043099488315;
	g_sFilterTarg.GyroLpBwPara[0].B[2]  = 0.3021549744157;
	
	g_sFilterTarg.GyroLpBwPara[1].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.GyroLpBwPara[1].FS    = 200;
	g_sFilterTarg.GyroLpBwPara[1].FC    = 30;
	g_sFilterTarg.GyroLpBwPara[1].A[0]  = 1;
	g_sFilterTarg.GyroLpBwPara[1].A[1]  = -0.7477891782585;
	g_sFilterTarg.GyroLpBwPara[1].A[2]  = 0.272214937925;
	g_sFilterTarg.GyroLpBwPara[1].B[0]  = 0.1311064399166;
	g_sFilterTarg.GyroLpBwPara[1].B[1]  = 0.2622128798333;
	g_sFilterTarg.GyroLpBwPara[1].B[2]  = 0.1311064399166;
	
	/*Gyro ���ٶȰ�����˹�����˲�*/
	g_sFilterTarg.GyroBsBwPara[0].FTYPE    = FILTER_TYPE_BANDSTOP;
	g_sFilterTarg.GyroBsBwPara[0].FS       = 200;
	g_sFilterTarg.GyroBsBwPara[0].bandLow  = 30;
	g_sFilterTarg.GyroBsBwPara[0].bandHigh = 98;
	g_sFilterTarg.GyroBsBwPara[0].A[0]     = 1;
	g_sFilterTarg.GyroBsBwPara[0].A[1]     = 0.6270403082828;
	g_sFilterTarg.GyroBsBwPara[0].A[2]     = -0.2905268567319;
	g_sFilterTarg.GyroBsBwPara[0].B[0]     = 0.354736571634;
	g_sFilterTarg.GyroBsBwPara[0].B[1]     = 0.6270403082828;
	g_sFilterTarg.GyroBsBwPara[0].B[2]     = 0.354736571634;	
	
	g_sFilterTarg.GyroBsBwPara[1].FTYPE    = FILTER_TYPE_BANDSTOP;
	g_sFilterTarg.GyroBsBwPara[1].FS       = 200;
	g_sFilterTarg.GyroBsBwPara[1].bandLow  = 30;
	g_sFilterTarg.GyroBsBwPara[1].bandHigh = 94;
	g_sFilterTarg.GyroBsBwPara[1].A[0]     = 1;
	g_sFilterTarg.GyroBsBwPara[1].A[1]     = 0.5334540355829;
	g_sFilterTarg.GyroBsBwPara[1].A[2]     = -0.2235264828971;
	g_sFilterTarg.GyroBsBwPara[1].B[0]     = 0.3882367585514;
	g_sFilterTarg.GyroBsBwPara[1].B[1]     = 0.5334540355829;
	g_sFilterTarg.GyroBsBwPara[1].B[2]     = 0.3882367585514;	

	/*3.BARO ��ѹ�ư�����˹�˲�*/
	g_sFilterTarg.BaroAboveLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.BaroAboveLpBwPara[0].FS    = 9;
	g_sFilterTarg.BaroAboveLpBwPara[0].FC    = 3;
	g_sFilterTarg.BaroAboveLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.BaroAboveLpBwPara[0].A[1]  = 0.6202041028867;
	g_sFilterTarg.BaroAboveLpBwPara[0].A[2]  = 0.2404082057735;
	g_sFilterTarg.BaroAboveLpBwPara[0].B[0]  = 0.465153077165;
	g_sFilterTarg.BaroAboveLpBwPara[0].B[1]  = 0.9303061543301;
	g_sFilterTarg.BaroAboveLpBwPara[0].B[2]  = 0.465153077165;
	
	g_sFilterTarg.BaroAboveLpBwPara[1].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.BaroAboveLpBwPara[1].FS    = 9;
	g_sFilterTarg.BaroAboveLpBwPara[1].FC    = 2;
	g_sFilterTarg.BaroAboveLpBwPara[1].A[0]  = 1;
	g_sFilterTarg.BaroAboveLpBwPara[1].A[1]  = -0.2047298257527;
	g_sFilterTarg.BaroAboveLpBwPara[1].A[2]  = 0.1789920775639;
	g_sFilterTarg.BaroAboveLpBwPara[1].B[0]  = 0.2435655629528;
	g_sFilterTarg.BaroAboveLpBwPara[1].B[1]  = 0.4871311259056;
	g_sFilterTarg.BaroAboveLpBwPara[1].B[2]  = 0.2435655629528;
	
	/*4.���� ������˹�˲�*/
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].FS    = 50;
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].FC    = 20;	
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].A[1]  = 1.14298050254;
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].A[2]  = 0.4128015980962;	
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].B[0]  = 0.638945525159;
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].B[1]  = 1.277891050318;
	g_sFilterTarg.OpticFlowIntPixLpBwPara[0].B[2]  = 0.638945525159;	
	
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].FS    = 50;
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].FC    = 10;	
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].A[1]  = -0.3695273773512;  
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].A[2]  = 0.1958157126558;	
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].B[0]  = 0.2065720838261; 
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].B[1]  = 0.4131441676523;
	g_sFilterTarg.OpticFlowGyroLpBwPara[0].B[2]  = 0.2065720838261;	

	/*���ٶ�ģ���˲�*/
	g_sFilterTarg.GyroLenthLpBwPara[0].FTYPE = FILTER_TYPE_LOWPASS;
	g_sFilterTarg.GyroLenthLpBwPara[0].FS    = 200;
	g_sFilterTarg.GyroLenthLpBwPara[0].FC    = 5;
	g_sFilterTarg.GyroLenthLpBwPara[0].A[0]  = 1;
	g_sFilterTarg.GyroLenthLpBwPara[0].A[1]  = -1.778631777825;
	g_sFilterTarg.GyroLenthLpBwPara[0].A[2]  = 0.8008026466657;
	g_sFilterTarg.GyroLenthLpBwPara[0].B[0]  = 0.005542717210281;
	g_sFilterTarg.GyroLenthLpBwPara[0].B[1]  = 0.01108543442056;
	g_sFilterTarg.GyroLenthLpBwPara[0].B[2]  = 0.005542717210281; 	
	
	/*5.PID���� ������˹�˲�*/
	g_sFilterTarg.PidControlErrPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.PidControlErrPara[0].FS    = 200;
	g_sFilterTarg.PidControlErrPara[0].FC    = 5;
	g_sFilterTarg.PidControlErrPara[0].A[0]  = 1;
	g_sFilterTarg.PidControlErrPara[0].A[1]  = -1.778631777825;
	g_sFilterTarg.PidControlErrPara[0].A[2]  = 0.8008026466657;
	g_sFilterTarg.PidControlErrPara[0].B[0]  = 0.005542717210281;
	g_sFilterTarg.PidControlErrPara[0].B[1]  = 0.01108543442056;
	g_sFilterTarg.PidControlErrPara[0].B[2]  = 0.005542717210281;
	
	g_sFilterTarg.PidControlDivPara[0].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.PidControlDivPara[0].FS    = 200;
	g_sFilterTarg.PidControlDivPara[0].FC    = 20;
	g_sFilterTarg.PidControlDivPara[0].A[0]  = 1;
	g_sFilterTarg.PidControlDivPara[0].A[1]  = -1.14298050254;
	g_sFilterTarg.PidControlDivPara[0].A[2]  = 0.4128015980962;
	g_sFilterTarg.PidControlDivPara[0].B[0]  = 0.06745527388907;
	g_sFilterTarg.PidControlDivPara[0].B[1]  = 0.1349105477781;
	g_sFilterTarg.PidControlDivPara[0].B[2]  = 0.06745527388907;
	
	g_sFilterTarg.PidControlDivPara[1].FTYPE = FILTER_TYPE_LOWPASS;	
	g_sFilterTarg.PidControlDivPara[1].FS    = 200;
	g_sFilterTarg.PidControlDivPara[1].FC    = 30;
	g_sFilterTarg.PidControlDivPara[1].A[0]  = 1;
	g_sFilterTarg.PidControlDivPara[1].A[1]  = -0.7477891782585;
	g_sFilterTarg.PidControlDivPara[1].A[2]  = 0.272214937925;
	g_sFilterTarg.PidControlDivPara[1].B[0]  = 0.1311064399166;
	g_sFilterTarg.PidControlDivPara[1].B[1]  = 0.2622128798333;
	g_sFilterTarg.PidControlDivPara[1].B[2]  = 0.1311064399166;	
}

/*������˹���׵�ͨ�˲�����ʼ��:��ʽ����*/
void filter_LpButterworth_Formulary_Init(u16 freqSample, u16 freqCut, FilterLpButterworthPara *para)
{
	u16 Fs, Fc;
	fp32 Wc, C1, G;
	
	Fs = freqSample;			/*����Ƶ��*/
	Fc = freqCut;				/*��ֹƵ��*/
	Wc = (2 * PI * Fc) / Fs;	/*��Ƶ��*/
	C1 = tan(Wc / 2);
	G  = 1 / (C1 * C1 + 1.41421356f * C1 + 1);
	
	para->A[0] = 1;
	para->A[1] = G * (2 * C1 * C1 - 2);
	para->A[2] = G * (C1 * C1 - 1.41421356f * C1 + 1);
	para->B[0] = G * C1 * C1;
	para->B[1] = 2 * para->B[0];
	para->B[2] = para->B[0];
}

/*һ�׻����˲���ʼ��*/
void filter_YijieHubu_Init(FilterYijieHubu *filterYijieHubu)
{
	filterYijieHubu->K     = 0.05f;
	filterYijieHubu->Angle = 0.0f;
}

/*���׻����˲���ʼ��*/
void filter_ErjieHubu_Init(FilterErjieHubu *filterErjieHubu)
{
	filterErjieHubu->K     = 0.05f;
	filterErjieHubu->Angle = 0.0f;
	filterErjieHubu->x1    = 0.0f;
	filterErjieHubu->x2    = 0.0f;
	filterErjieHubu->y1    = 0.0f;
}


/************** 2. �˲����������� **************/
/*�����ݽ���ë���˲�*/
fp32 filter_Base_Dp(FilterBase *filterBase, fp32 newVal)
{
	fp32 n = 0;
	
	if (math_Abs(newVal - filterBase->lastVal) > filterBase->maxAbs)
	{	
		if (newVal >= 0)
		{
			if (filterBase->isUp == 0x00)
			{
				filterBase->index = 0;
			}
			
			filterBase->isUp = 0x01;
			
			n = filterBase->lastVal + math_Abs(filterBase->lastVal - newVal) * \
				filterBase->wvBuff[filterBase->index];
		}
		else
		{
			if (filterBase->isUp == 0x01)
			{
				filterBase->index = 0;
			}
			
			filterBase->isUp = 0x00;
			
			n = filterBase->lastVal - math_Abs(newVal - filterBase->lastVal) * \
				filterBase->wvBuff[filterBase->index];
		}
		
		filterBase->index++;
		
		if (filterBase->index >= FILTER_BASE_BUFF_LEN)
		{
			filterBase->index   = 0;
			filterBase->lastVal = newVal;
			
			return newVal;
		}
		
		return n;
	}
	else
	{
		filterBase->lastVal = newVal;
		filterBase->index   = 0;
		
		return newVal;
	}
}

/*�����ݽ��л���ƽ���˲�*/
fp32 filter_Slider_Dp(FilterSlider *filterSlider, fp32 newVal)
{
	fp32 sum = 0;
	u8 i;
	
	/*���黬��,ȥ��������*/
	for (i = 0; i < filterSlider->arrayLen-1; i++)
	{
		filterSlider->array[i] = filterSlider->array[i+1];
	}
	
	/*����������*/
	filterSlider->array[filterSlider->arrayLen-1] = newVal;
	
	/*�����������*/
	for (i = 0; i < filterSlider->arrayLen; i++)
	{
		sum += filterSlider->array[i];
	}
	
	/*��ƽ��,����ƽ��ֵ*/
	return (sum / filterSlider->arrayLen);
}

/*�����ݽ��л�����ֵ�˲�*/
fp32 filter_Slider_Average_Dp(FilterSlider *filterSlider, fp32 newVal)
{
	u8 i;
	fp32 max, min, sum;
	
	max = newVal;
	min = newVal;
	sum = newVal;
	
	/*�������ҳ����ֵ����������*/
	for (i = 0; i < filterSlider->arrayLen - 1; i++)
	{
		if (filterSlider->array[i] > max)
		{
			max = filterSlider->array[i];
		}
		else if (filterSlider->array[i] < min)
		{
			min = filterSlider->array[i];
		}
		
		sum += filterSlider->array[i];
		
		/*���黬��*/
		filterSlider->array[i] = filterSlider->array[i+1];
	}
	
	/*����������*/
	filterSlider->array[filterSlider->arrayLen - 1] = newVal;
	
	i   = filterSlider->arrayLen - 2;
	sum = (sum - max - min) / i;
	
	return sum;
}

/*���ٶ�(��̬) ������˹���׵�ͨ�˲����˲�*/
Acc3f* filter_AccAttLpButterworth_Dp(Acc3f *targAcc, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	u8 axis;
	
	/*��ȡ����x(n)*/
	buff[0].Input[2] = targAcc->x;
	buff[1].Input[2] = targAcc->y;
	buff[2].Input[2] = targAcc->z;
	
	/*butterworth�˲�*/
	for (axis = 0; axis < 3; axis++)
	{
		buff[axis].Output[2] = para->B[0] * buff[axis].Input[2] + \
							   para->B[1] * buff[axis].Input[1] + \
							   para->B[2] * buff[axis].Input[0] - \
							   para->A[1] * buff[axis].Output[1] - \
							   para->A[2] * buff[axis].Output[0];
	}
	
	for (axis = 0; axis < 3; axis++)
	{
		/*x(n)���б���*/
		buff[axis].Input[0] = buff[axis].Input[1];
		buff[axis].Input[1] = buff[axis].Input[2];
		
		/*y(n)���б���*/
		buff[axis].Output[0] = buff[axis].Output[1];
		buff[axis].Output[1] = buff[axis].Output[2];
	}
	
	/*�˲���ļ��ٶ�ֵ*/
	targAcc->x = buff[0].Output[2];
	targAcc->y = buff[1].Output[2];
	targAcc->z = buff[2].Output[2];	
	
	return targAcc;
}

/*���ٶ�(����:������У׼���ߵ������Ʒ���) ������˹���׵�ͨ�˲����˲�*/
fp32 filter_AccFuncLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	static u8 accLpBwfilterCnt = 0;
	
	/*��ȡ����x(n)*/
	buff->Input[2] = currInput;
	
    if(accLpBwfilterCnt >= 100)
    {
		/*Butterworth�˲�*/
		buff->Output[2] = para->B[0] * buff->Input[2] + \
						  para->B[1] * buff->Input[1] + \
						  para->B[2] * buff->Input[0] - \
		                  para->A[1] * buff->Output[1] - \
						  para->A[2] * buff->Output[0];
	}
	else
	{
		buff->Output[2] = buff->Input[2];
		accLpBwfilterCnt++;
	}
	
	/*x(n)���б���*/	
	buff->Input[0] = buff->Input[1];
	buff->Input[1] = buff->Input[2];
	
	/*y(n)���б���*/
	buff->Output[0] = buff->Output[1];
	buff->Output[1] = buff->Output[2];
	
	return buff->Output[2];
}

/*���ٶ� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_GyroFuncLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	/*��ȡ����x(n)*/
	buff->Input[2] = currInput;
	
	/*Butterworth�˲�*/
	buff->Output[2] = para->B[0] * buff->Input[2] + \
					  para->B[1] * buff->Input[1] + \
					  para->B[2] * buff->Input[0] - \
					  para->A[1] * buff->Output[1] -\
	                  para->A[2] * buff->Output[0];
	
	/*x(n)���б���*/
	buff->Input[0] = buff->Input[1];
	buff->Input[1] = buff->Input[2];
	
	/*y(n)���б���*/
	buff->Output[0] = buff->Output[1];
	buff->Output[1] = buff->Output[2];

    return (buff->Output[2]);
}

/*��ѹ�Ƹ߶� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_BaroAltitudeLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	/*��ȡ����x(n)*/
	buff->Input[2] = currInput;
	
	/*Butterworth�˲�*/
	buff->Output[2] = para->B[0] * buff->Input[2] + para->B[1] * buff->Input[1] + para->B[2] * buff->Input[0] - \
					  para->A[1] * buff->Output[1] - para->A[2] * buff->Output[0];
	
	/*x(n)���б���*/	
	buff->Input[0] = buff->Input[1];
	buff->Input[1] = buff->Input[2];
	
	/*y(n)���б���*/
	buff->Output[0] = buff->Output[1];
	buff->Output[1] = buff->Output[2];	
	
	return (buff->Output[2]); 
}

/*�����ۼ����ص� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_OpFlowIntPixLpButterworth_Dp(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	static u8 opFlowLpBwfilterCnt = 0;
	
	/*��ȡ����x(n)*/
	buff->Input[2] = currInput;
	
    if(opFlowLpBwfilterCnt >= 100)
    {
		/*Butterworth�˲�*/
		buff->Output[2] = para->B[0] * buff->Input[2] + \
						  para->B[1] * buff->Input[1] + \
						  para->B[2] * buff->Input[0] - \
		                  para->A[1] * buff->Output[1] - \
						  para->A[2] * buff->Output[0];
	}
	else
	{
		buff->Output[2] = buff->Input[2];
		opFlowLpBwfilterCnt++;
	}
	
	/*x(n)���б���*/	
	buff->Input[0] = buff->Input[1];
	buff->Input[1] = buff->Input[2];
	
	/*y(n)���б���*/
	buff->Output[0] = buff->Output[1];
	buff->Output[1] = buff->Output[2];
	
	return buff->Output[2];	
}

/*PIDƫ���� ������˹���׵�ͨ�˲����˲�*/
fp32 filter_Pid_Control_Device_LPF(fp32 currInput, FilterLpButterworthBuff *buff, FilterLpButterworthPara *para)
{
	/*��ȡ����x(n)*/
	buff->Input[2] = currInput;
	
	/*Butterworth�˲�*/
	buff->Output[2] = para->B[0] * buff->Input[2] + \
					  para->B[1] * buff->Input[1] + \
					  para->B[2] * buff->Input[0] - \
				      para->A[1] * buff->Output[1] - \
				      para->A[2] * buff->Output[0];
	
	/*x(n)���б���*/
	buff->Input[0] = buff->Input[1];
	buff->Input[1] = buff->Input[2];
	
	/*y(n)���б���*/	
	buff->Output[0] = buff->Output[1];
	buff->Output[1] = buff->Output[2];
	
	return (buff->Output[2]);
}

/************** 3. ��̬�����ں��㷨 **************/
/*һ�׻����˲�*/
fp32 filter_YijieHubu_Dp(FilterYijieHubu *filterYijieHubu, fp32 angle_m, fp32 gyro_m, fp32 dtS)
{
	filterYijieHubu->Angle = filterYijieHubu->K * angle_m + (1 - filterYijieHubu->K) * \
							 (filterYijieHubu->Angle + gyro_m * dtS);
	
	return filterYijieHubu->Angle;
}

/*���׻����˲�*/
fp32 filter_ErjieHubu_Dp(FilterErjieHubu *filterErjieHubu, fp32 angle_m, fp32 gyro_m, fp32 dtS)
{
	filterErjieHubu->x1 = (angle_m - filterErjieHubu->Angle) * (1 - filterErjieHubu->K) * (1 - filterErjieHubu->K);
	filterErjieHubu->y1 = filterErjieHubu->y1 + filterErjieHubu->x1 * dtS;
	filterErjieHubu->x2 = filterErjieHubu->y1 + 2 * (1 - filterErjieHubu->K) * (angle_m - filterErjieHubu->Angle) + gyro_m;
	
	filterErjieHubu->Angle = filterErjieHubu->Angle + filterErjieHubu->x2 * dtS;
	
	return filterErjieHubu->Angle;
}

/*ԭʼ�����˲�����ʼ��*/
void filter_origin_data_filter_init(FilterTarg *filterTarg)
{
	/*���װ�����˹��ͨ�˲�����ʼ��:MATLAB����*/
	filter_LpButterworth_Matlab_Init();
	
	/*������ ����ƽ���˲�����ʼ��*/
	filter_Slider_Init(&(filterTarg->MagxSliderAverage), 10);	/*Mag_X*/
	
	filter_Slider_Init(&(filterTarg->MagySliderAverage), 10);	/*Mag_Y*/
	
	filter_Slider_Init(&(filterTarg->MagzSliderAverage), 10);	/*Mag_Z*/

	/*������ ����ƽ���˲�����ʼ��*/
	filter_Slider_Init(&(filterTarg->UltrSliderAverage), 10);	/*������*/
}
