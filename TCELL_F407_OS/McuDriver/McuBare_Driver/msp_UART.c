#include "msp_UART.h"

/*���ڳ�ʼ��*/
void msp_Uart_Init(MSP_Uart *uart)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*Tx GPIO*/
	GPIO_InitStruct.GPIO_Pin   = uart->Tx_Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(uart->Tx_GPIO, &GPIO_InitStruct);
	
	/*Rx GPIO*/
	GPIO_InitStruct.GPIO_Pin   = uart->Rx_Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(uart->Rx_GPIO, &GPIO_InitStruct);	
	
	/*Tx/Rx PinAF*/
	GPIO_PinAFConfig(uart->Tx_GPIO, uart->Tx_PinSource, uart->GPIO_AF);
	GPIO_PinAFConfig(uart->Rx_GPIO, uart->Rx_PinSource, uart->GPIO_AF);
	
	/*Uart Init*/
	USART_InitStruct.USART_BaudRate 		   = uart->BaudRate;
	USART_InitStruct.USART_WordLength 		   = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 		   = USART_StopBits_1;
	USART_InitStruct.USART_Parity      	       = USART_Parity_No;	
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_InitStruct.USART_Mode 			   = uart->Mode;
	USART_Init(uart->Uart, &USART_InitStruct); /*��ʼ������*/
	
	/*1.����UART���͹���ģʽ*/
	/*1.1���ô��ڷ����ж�*/
	if (uart->TxMode == MSP_UART_INTERRUPT) /*��������ж�*/
	{
		USART_ITConfig(uart->Uart, USART_IT_TXE, ENABLE); /*ʹ�ܽ����ж�*/		
	}
	
	/*1.2���ô��ڷ���DMA������ʽ*/
	if (uart->TxMode == MSP_UART_DMA)	/*����DMA*/
	{
		/*����DMA���ü���ʼ��*/
		uart->TxDma.PerBaseAddr = (u32)&(uart->Uart->DR);	    /*����Ŀ���ַ*/
		uart->TxDma.MemBaseAddr = (u32)(uart->pTxBuff);		    /*�ڴ�Ŀ���ַ*/	
		uart->TxDma.Direction   = DMA_DIR_MemoryToPeripheral;	/*�ڴ浽����*/		
		
		/*����DMA��ʼ��*/		
		msp_Dma_Init(&(uart->TxDma));
	}
	
	/*2.����UART���չ���ģʽ*/	
	/*2.1���ô��ڽ����ж�*/
	if ((uart->RxMode == MSP_UART_INTERRUPT) || (uart->RxMode == MSP_UART_DMA_IDLE)) /*�����ж� / ���տ����ж�*/
	{
		/*��ͨ�����ж�*/
		if (uart->RxMode == MSP_UART_INTERRUPT)
		{
			USART_ITConfig(uart->Uart, USART_IT_RXNE, ENABLE); /*ʹ�ܽ����ж�*/
		}
		/*�����ж�*/
		else if (uart->RxMode == MSP_UART_DMA_IDLE)
		{
			USART_ITConfig(uart->Uart, USART_IT_IDLE, ENABLE); /*ʹ�ܿ����ж�*/
		}
	}	
	
	/*2.2���ô��ڽ���DMA������ʽ*/
	if ((uart->RxMode == MSP_UART_DMA) || (uart->RxMode == MSP_UART_DMA_IDLE))	/*����DMA*/
	{
		/*����DMA���ü���ʼ��*/
		uart->RxDma.PerBaseAddr = (u32)&(uart->Uart->DR);		/*����Ŀ���ַ*/
		uart->RxDma.MemBaseAddr = (u32)(uart->pRxBuff);			/*�ڴ�Ŀ���ַ*/
		uart->RxDma.Direction   = DMA_DIR_PeripheralToMemory;	/*���赽�ڴ�*/
		
		/*����DMA��ʼ��*/
		msp_Dma_Init(&(uart->RxDma));
	}
	
	/*ʹ��UART*/
	USART_Cmd(uart->Uart, ENABLE);
}

/*���ڷ�������*/
void msp_Uart_Send_Data(MSP_Uart *uart, u8 *txBuff, u16 lenth, MSP_UART_WORK_MODE UART_TX_MODE)
{
	u8 i;
	
	/*��ѯ����ģʽ*/
	if (UART_TX_MODE == MSP_UART_POLL)	
	{
		for (i = 0; i < lenth; i++)
		{
			USART_SendData(uart->Uart, (u16)(txBuff[i]));
			
			while(USART_GetFlagStatus(uart->Uart, USART_FLAG_TXE) != SET);
		}
	}
	
	/*DMA����ģʽ*/
	if (UART_TX_MODE == MSP_UART_DMA)	
	{	
		/*�ж��Ƿ������,��ɺ����������ɱ�־,���ܿ����´δ���*/
		if (DMA_GetFlagStatus(uart->TxDma.Stream, uart->TxDma.dmaFlag) != RESET)
		{
			DMA_ClearFlag(uart->TxDma.Stream, uart->TxDma.dmaFlag);
		}
		
		/*�ж�DMA������ɱ�־λΪRESET,���ܿ�������*/
		if (DMA_GetFlagStatus(uart->TxDma.Stream, uart->TxDma.dmaFlag) == RESET)
		{
			/*ʹ�ܴ��ڷ���DMA*/
			USART_DMACmd(uart->Uart, USART_DMAReq_Tx, ENABLE);
		
			/*����DMA����*/
			msp_Dma_Enable(&(uart->TxDma));
		}
	}
}

/*���ڽ�������*/
void msp_uart_recv_data(MSP_Uart *uart)
{
//	u8 i;
	
	/*��ѯ����ģʽ*/
	if (uart->RxMode == MSP_UART_POLL)	
	{
		
	}
	
	/*DMA����ģʽ*/
	if (uart->RxMode == MSP_UART_DMA)	
	{
		/*�ж��Ƿ������,��ɺ����������ɱ�־,���ܿ����´δ���*/
		if (DMA_GetFlagStatus(uart->RxDma.Stream, uart->RxDma.dmaFlag) != RESET)
		{
			DMA_ClearFlag(uart->RxDma.Stream, uart->RxDma.dmaFlag);
		}
		
		/*�ж�DMA������ɱ�־λΪRESET,���ܿ�������*/
		if (DMA_GetFlagStatus(uart->RxDma.Stream, uart->RxDma.dmaFlag) == RESET)
		{
			uart->Uart->DR = 0;
			
			/*ʹ�ܴ��ڽ���DMA*/
			USART_DMACmd(uart->Uart, USART_DMAReq_Rx, ENABLE);
		
			/*����DMA����*/
			msp_Dma_Enable(&(uart->RxDma));
		}
	}

	/*DMA + IDLEģʽ*/	
	if (uart->RxMode == MSP_UART_DMA_IDLE)
	{
		/*ʹ�ܴ��ڽ���DMA*/
		USART_DMACmd(uart->Uart, USART_DMAReq_Rx, ENABLE);
		
		/*����DMA����*/
		msp_Dma_Enable(&(uart->RxDma));	
	}
}

#pragma import(__use_no_semihosting) 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;   

void _sys_exit(int x) 
{ 
	x = x; 
}

int fputc(int ch, FILE *f)
{	
	while((USART1->SR & 0X40) == 0){};//ѭ������,ֱ���������	
    USART1->DR = (u8)ch;
		
	return ch;
}
