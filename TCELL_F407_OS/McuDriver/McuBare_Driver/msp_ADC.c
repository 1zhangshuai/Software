#include "msp_ADC.h"
#include "msp_GPIO.h"

u16 g_u16Adc1Buff[MSP_ADC1_BUFF_SIZE] = {0};
u16* g_pu16Adc1Buff = g_u16Adc1Buff;

void msp_Adc_Init(MSP_ADC_RESOURCE mspAdcResource)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	/*RCC Enable*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	
	/*GPIO Init*/
	GPIO_InitStruct.GPIO_Pin  = ADC1_IN14_I_MOTA_Pin | ADC1_IN15_I_MOTB_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*ADC init*/
	ADC_InitStruct.ADC_Mode 			  =	MSP_ADC1_MODE;
	ADC_InitStruct.ADC_ScanConvMode       = MSP_ADC1_SCANCONVMODE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv   = MSP_ADC1_EXTERNALTRIGCONV;
	ADC_InitStruct.ADC_DataAlign 		  = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel       = MSP_ADC1_NBROFCHANNEL;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	/*ADC enable*/
	ADC_Cmd(ADC1, ENABLE);
	
#if (MSP_ADC1_DATA_MODE	== MSP_ADC1_DATA_DMA)	

#elif (MSP_ADC1_DATA_MODE == MSP_ADC1_NORMAL)

#endif	
	
	/*����ADC�����ο�ʱ�ӵ�Ԥ��Ƶֵ*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	/*����ADC�Ĺ���ͨ���Ĳ���˳��Ͳ���ʱ��*/
	ADC_RegularChannelConfig(ADC1, MSP_ADC_CH0, MSP_ADC_RANK1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, MSP_ADC_CH1, MSP_ADC_RANK2, ADC_SampleTime_71Cycles5);	
	
	/*����ADC����У׼������ֹ���ֽϴ�����*/
	ADC_ResetCalibration(ADC1);		
	/*�ȴ�У׼�ɹ�*/
	while(ADC_GetCalibrationStatus(ADC1));		
	/*����ADC����״̬*/
	ADC_StartCalibration(ADC1);	
	/*�ȵ������ɹ�*/
	while(ADC_GetCalibrationStatus(ADC1));			
}

/*ʹ��ָ����ADC1�����ת����������*/
void msp_Adc_Start(MSP_ADC_RESOURCE mspAdcResource)
{
	switch(mspAdcResource)
	{
		case MSP_SRC_ADC1:
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		}break;

		case MSP_SRC_ADC2:
		{
			ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		}break;		

		case MSP_SRC_ADC3:
		{
			ADC_SoftwareStartConvCmd(ADC3, ENABLE);
		}break;	

		default:break;
	}
}

void msp_Adc_Stop(MSP_ADC_RESOURCE mspAdcResource)
{
	
}

