#ifndef _SYS_DEBUG_H_
#define _SYS_DEBUG_H_

#include "sys_Platform.h"
#include "msp_UART.h"

#define DEBUG_TX_BUFF_SIZE	(100) /*�����Buff,��ֹ����Խ��*/
#define DEBUG_RX_BUFF_SIZE	(50)  /*������Buff,��ֹ����Խ��*/

/*���ڴ�ӡ��ʼ��*/
void rt_debug_uart_init(void);

extern u8 g_DebugTxBuff[DEBUG_TX_BUFF_SIZE];
extern u8 g_DebugRxBuff[DEBUG_RX_BUFF_SIZE];

/*Debug UART*/
extern MSP_Uart g_sDebugUart;

#endif
