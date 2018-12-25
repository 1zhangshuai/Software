#ifndef _MSP_DMA_H_
#define _MSP_DMA_H_

#include "sys_Platform.h"

typedef enum
{
	DMA_IT_DISABLE = 0,	
	DMA_IT_ENABLE  = 1,
}DMA_IT_ENABLE_STATUS;

typedef struct
{
	DMA_Stream_TypeDef*  	 Stream;
	u32					 	 Channel;
	u32 				 	 PerBaseAddr;
	u32 				 	 MemBaseAddr;
	u32					 	 Direction;
	u32 			 	 	 PerAlignByte;
	u32 			 	 	 MemAlignByte;	
	u32 				 	 Mode;	
	u32 				 	 Priority;
	u32 				 	 BuffSize;
	u32					 	 RCC_Periph_DMA;
	u32					     dmaFlag;	/*DMA�����־*/
	DMA_IT_ENABLE_STATUS     dmaIT;	    /*DMA�Ƿ�ʹ���ж�*/
	volatile FunctionalState dmaStatus;	/*DMAʹ��״̬,��һ��ʹ�����ڽ�������ʱ�ж���ʹ��,main����while(1)ǰ����ʹ��*/
	u32 				 	 ITType;    /*DMA�ж�����*/
	u8 				     	 NVIC_IRQChannel; /*�ж�ͨ��*/
}Msp_Dma;

/*DMA��ʼ��*/
void msp_Dma_Init(Msp_Dma * msp_Dma);

/*DMAʹ��*/
void msp_Dma_Enable(Msp_Dma * msp_Dma);

/*DMAʧ��*/
void msp_Dma_Disable(Msp_Dma * msp_Dma);
	
#endif
