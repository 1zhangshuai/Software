#include "msp_DMA.h"

/*DMA��ʼ��*/
void msp_Dma_Init(Msp_Dma * msp_Dma)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_DeInit(msp_Dma->Stream);	/*��DMA_Stream�Ĵ�������Ϊȱʡֵ*/
	
	while (DMA_GetCmdStatus(msp_Dma->Stream) != DISABLE){} /*�ȴ�DMA������*/
	
	DMA_InitStruct.DMA_Channel 		       = msp_Dma->Channel;
	DMA_InitStruct.DMA_PeripheralBaseAddr  = msp_Dma->PerBaseAddr;
	DMA_InitStruct.DMA_Memory0BaseAddr     = msp_Dma->MemBaseAddr;
	DMA_InitStruct.DMA_DIR                 = msp_Dma->Direction;
	DMA_InitStruct.DMA_BufferSize          = msp_Dma->BuffSize;
	DMA_InitStruct.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;	/*��ֹ�����ַ����*/
	DMA_InitStruct.DMA_MemoryInc           = DMA_MemoryInc_Enable;		/*ʹ���ڴ��ַ����*/
	DMA_InitStruct.DMA_PeripheralDataSize  = msp_Dma->PerAlignByte;
	DMA_InitStruct.DMA_MemoryDataSize      = msp_Dma->MemAlignByte;
	DMA_InitStruct.DMA_Mode                = msp_Dma->Mode;
	DMA_InitStruct.DMA_Priority            = msp_Dma->Priority;
	DMA_InitStruct.DMA_FIFOMode 		   = DMA_FIFOMode_Disable;       /*����Fifo*/  
	DMA_InitStruct.DMA_FIFOThreshold       = DMA_FIFOThreshold_Full;	 /*��*/
	DMA_InitStruct.DMA_MemoryBurst         = DMA_MemoryBurst_Single;     /*�洢��ͻ�����δ���*/
	DMA_InitStruct.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single; /*����ͻ�����δ���*/		
	
	DMA_Init(msp_Dma->Stream, &DMA_InitStruct);
		
	/*�ж��Ƿ�����DMA�ж�,�����ö�Ӧ��Ӧ�ж�*/
	if (msp_Dma->dmaIT == DMA_IT_ENABLE)
	{
		/*Ԥ����жϱ�־*/
		DMA_ClearFlag(msp_Dma->Stream, msp_Dma->ITType);
		
		/*����ʹ���ж�*/
		DMA_ITConfig(msp_Dma->Stream, msp_Dma->ITType, ENABLE);			
	}		
}

/*DMAʹ��*/
void msp_Dma_Enable(Msp_Dma * msp_Dma)
{
	/*�ر�DMAָʾͨ��*/
	DMA_Cmd(msp_Dma->Stream, DISABLE);
	
	/*ȷ��DMA���Ա�����*/
	while(DMA_GetCmdStatus(msp_Dma->Stream) != DISABLE){}	
	
	/*DMAͨ�������ݴ���������*/
	DMA_SetCurrDataCounter(msp_Dma->Stream, msp_Dma->BuffSize);
	
	/*ʹ��DMAָʾͨ��*/
	DMA_Cmd(msp_Dma->Stream, ENABLE);	
}

/*DMAʧ��*/
void msp_Dma_Disable(Msp_Dma * msp_Dma)
{
	/*�ر�DMAָʾͨ��*/
	DMA_Cmd(msp_Dma->Stream, DISABLE);
}
