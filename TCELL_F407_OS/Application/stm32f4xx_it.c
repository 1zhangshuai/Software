#include "stm32f4xx_it.h"

#include "period_Execute.h"
#include "sys_McuInit.h"
#include "remot_DataAnaly.h"

#include "sys_OsTask.h"

void NMI_Handler(void)
{
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

/*ʹ��RTOSʱ,systick����Ϊϵͳ��������Դ*/
#ifndef PLATFORM_RTOS__RT_THREAD
void SysTick_Handler(void)
{
	if(g_vu32DelayCounter != 0)
	{
		g_vu32DelayCounter--;
	}	
}
#endif

/*EXTI*/
#if defined(REMOTE_DATA_RECV__PPM)
/*PPM����*/
void EXTI15_10_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	

	if (EXTI_GetITStatus(g_sPPMRecvExti.EXTI_Line) != RESET)
	{	
		remot_PPM_AllChannel_Analy();
		
		EXTI_ClearITPendingBit(g_sPPMRecvExti.EXTI_Line);
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}
#endif

/*=== Tim alarm ===*/
/*TIM2:�̶߳�ʱ����*/
void TIM2_IRQHandler(void)				
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	if (TIM_GetITStatus(g_sTimAlarmTaskAssist.Tim, TIM_IT_Update) != RESET)
	{			
		#if defined(PLATFORM_RTOS__RT_THREAD)
		/*��������*/
		if (gs_rtos_thread_wake_up.base_module < (RTOS_WAKE_UP_BASE_MODULE_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.base_module++;
		}
		else 
		{
			gs_rtos_thread_wake_up.base_module = 1;
			rt_sem_release(&base_module_sem);
		}

		/*У׼ϵͳ*/
		/*acc, mag*/
		if (gs_rtos_thread_wake_up.uav_calib < (RTOS_WAKE_UP_UAV_CALIB_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.uav_calib++;
		}
		else 
		{
			gs_rtos_thread_wake_up.uav_calib = 1;
			rt_sem_release(&uav_calib_sem);
		}	
		
		/*�������*/
		/*fly log write and read_send*/
		if (gs_rtos_thread_wake_up.tfsd_fly_log < (RTOS_WAKE_UP_TFSD_FLY_LOG_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.tfsd_fly_log++;
		}
		else 
		{
			gs_rtos_thread_wake_up.tfsd_fly_log = 1;
			rt_sem_release(&tfsd_fly_log_sem);
		}	

		/*�˻�����*/
		/*hci: oled show*/
		if (gs_rtos_thread_wake_up.hci_oled_show < (RTOS_WAKE_UP_HCI_OLED_SHOW_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.hci_oled_show++;
		}
		else 
		{
			gs_rtos_thread_wake_up.hci_oled_show = 1;
			rt_sem_release(&hci_oled_show);
		}		
		
		/*hci:  send data to host*/
		if (gs_rtos_thread_wake_up.hci_slave_host < (RTOS_WAKE_UP_HCI_SLAVE_HOST_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.hci_slave_host++;
		}
		else 
		{
			gs_rtos_thread_wake_up.hci_slave_host = 1;
			rt_sem_release(&hci_host_slave_sem);
		}			
		#endif		
		
		TIM_ClearITPendingBit(g_sTimAlarmTaskAssist.Tim, TIM_IT_Update);
	}		
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}

/*TIM7:�̶߳�ʱ����*/
void TIM7_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();
	#endif
	
	if (TIM_GetITStatus(g_sTimAlarmTaskMain.Tim, TIM_IT_Update) != RESET)
	{	
		#if defined(PLATFORM_RTOS__RT_THREAD)
		/*���������ݻ�ȡ����̬�ں�*/	
		/*IMU, MAG, Euler*/
		if (gs_rtos_thread_wake_up.euler_angle < (RTOS_WAKE_UP_EULER_ANGLE_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.euler_angle++;
		}
		else 
		{
			gs_rtos_thread_wake_up.euler_angle = 1;
			rt_sem_release(&euler_angle_sem);
		}	

		/*BERO, ULTR, Vertical*/
		if (gs_rtos_thread_wake_up.ver_fusion < (RTOS_WAKE_UP_VER_FUSION_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.ver_fusion++;
		}
		else 
		{
			gs_rtos_thread_wake_up.ver_fusion = 1;
			rt_sem_release(&ver_fusion_sem);
		}

		/*GPS, Horizontal*/	
		if (gs_rtos_thread_wake_up.gps_hor_fusion < (RTOS_WAKE_UP_GPS_HOR_FUSION_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.gps_hor_fusion++;
		}
		else 
		{
			gs_rtos_thread_wake_up.gps_hor_fusion = 1;
			rt_sem_release(&gps_hor_fusion_sem);
		}	

		/*OPTICFLOW, Horizontal*/
		if (gs_rtos_thread_wake_up.opflow_hor_fusion < (RTOS_WAKE_UP_OPFLOW_HOR_FUSION_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.opflow_hor_fusion++;
		}
		else 
		{
			gs_rtos_thread_wake_up.opflow_hor_fusion = 1;
			rt_sem_release(&opflow_hor_fusion_sem);
		}		
			
		/*����ϵͳ*/
		/*remot, pid, output motor*/
		if (gs_rtos_thread_wake_up.uav_ctrl < (RTOS_WAKE_UP_UAV_CTRL_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.uav_ctrl++;
		}
		else 
		{
			gs_rtos_thread_wake_up.uav_ctrl = 1;
			rt_sem_release(&uav_ctrl_sem);
		}
		
		/*�������״̬*/
		if (gs_rtos_thread_wake_up.task_status_check < (RTOS_WAKE_UP_TASK_STATUS_CHECK_FOC_MS / PLATFORM_TASK_SCHEDULER_MIN_FOC_MS))
		{
			gs_rtos_thread_wake_up.task_status_check++;
		}
		else
		{
			gs_rtos_thread_wake_up.task_status_check = 1;
			rt_sem_release(&task_status_check_sem);			
		}	
		
		#endif		

		TIM_ClearITPendingBit(g_sTimAlarmTaskMain.Tim, TIM_IT_Update);
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif
}

/*TIM9:�߳�ִ�м����׼����*/
void TIM1_BRK_TIM9_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	if (TIM_GetITStatus(g_sTimExecutePeriod.Tim, TIM_IT_Update) != RESET)
	{	
		g_vu32_Time_Period_Cnt_Foc_Ms++;	/*10000us,10ms*/	
		my_Tick_Ins();						/*g_vu32TickCounterʱ��,ÿ10ms��1*/
	
		TIM_ClearITPendingBit(g_sTimExecutePeriod.Tim, TIM_IT_Update);	
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif
}

/*=== Tim Input capture ===*/
#if defined(REMOTE_DATA_RECV__PWM)
/*1.Tim1_5_8*/
void TIM1_CC_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	remot_Switch_Channel_Analy();	/*����ͨ�����ݽ���*/
	/*�����־�������溯����*/	
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif
}

/*2.Tim4_1_4*/
void TIM4_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	remot_Attitude_Channel_Analy();	/*��̬ͨ�����ݽ���*/
	/*�����־�������溯����*/	
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}
#endif

/*=== Uart dma + idle interrupt ===*/
/*UART1 DMA+IDLE*/
u8 g_uUart1DMARxCnt   = 0; 
u8 g_uHostRxDataIndex = 0;
void USART1_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	if (USART_GetITStatus(g_sDebugUart.Uart, USART_IT_IDLE) != RESET)
	{
		/*ʧ��DMA*/
		msp_Dma_Disable(&g_sDebugUart.RxDma);
		
		/*��ȡ���յ�������*/
		USART_ReceiveData(g_sDebugUart.Uart);                                
		
		 /*��ȡ��ǰ���յ�������*/
		g_uUart1DMARxCnt = DEBUG_RX_BUFF_SIZE - DMA_GetCurrDataCounter(g_sDebugUart.RxDma.Stream);
		
		/***********֡���ݽ���������************/
		/*��ֹ�����ϴ�*/
		g_psPidSystem->PidSettingSystem.PARA_DOWNLOAD_STATUS = PID_PARA_DOWNLOAD_START;
		
		for (g_uHostRxDataIndex = 0; g_uHostRxDataIndex < g_uUart1DMARxCnt; g_uHostRxDataIndex++)
		{
			user_Host_Data_Receive_Prepare(g_sDebugUart.pRxBuff[g_uHostRxDataIndex]);
		}
		
		/*���浱ǰ��λ���·��Ĳ�����EEPROM ���� ��Ĭ�ϲ���д��EEPROM*/
		if ((g_psPidSystem->PidSettingSystem.DO_STATUS == PID_PARAMETER_DO_SAVE) || \
			(g_psPidSystem->PidSettingSystem.DO_STATUS == PID_PARAMETER_DO_RESET))
		{
			#if defined(PLATFORM_RTOS__RT_THREAD)
			/*PID�����ı�*/
			rt_event_send(&para_rdwr_event, (1 << RTOS_EVENT_PID_PARA_UPDATE_BY_HOST));			
			#endif
		}
		
		/*����buff��0*/
		memset(g_sDebugUart.pRxBuff, 0, g_uUart1DMARxCnt);
		/*************************************/
		
		/*�������ô�����*/
		DMA_SetCurrDataCounter(g_sDebugUart.RxDma.Stream, DEBUG_RX_BUFF_SIZE);

		/*ʹ��Debug Uart Rx*/
		msp_uart_recv_data(&g_sDebugUart);

		/*����жϱ�־*/
		USART_ClearITPendingBit(g_sDebugUart.Uart, USART_IT_IDLE);         
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}

/*UART4 DMA+IDLE*/
#if defined(HW_CUT__USE_GPS)
u8 g_uUart4DMARxCnt  = 0; 
u8 g_uGpsRxDataIndex = 0;
void UART4_IRQHandler(void)	
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif	
	
	if (USART_GetITStatus(g_sGpsUart.Uart, USART_IT_IDLE) != RESET)
	{
		/*ʧ��DMA*/
		msp_Dma_Disable(&g_sGpsUart.RxDma);
		
		/*��ȡ���յ�������*/
		USART_ReceiveData(g_sGpsUart.Uart);                                
		
		 /*��ȡ��ǰ���յ�������*/
		g_uUart4DMARxCnt = GPS_M8N_RX_BUFF_LENTH - DMA_GetCurrDataCounter(g_sGpsUart.RxDma.Stream);
		
		/***********֡���ݽ���������************/
		/*��������GPS����*/
		bsp_GPS_M8N_PVT_Parse(&g_sGpsM8N, &g_sGpsM8nRxPvtFrame, g_GpsM8nRxBuff);
		
		/*���̻߳�ȡGPS��������*/
		#if defined(PLATFORM_RTOS__RT_THREAD)
		rt_sem_release(&gps_update_sem);
		#endif
		
		/*����buff��0*/
		memset(g_sGpsUart.pRxBuff, 0, g_uUart4DMARxCnt);
		/*************************************/
		
		/*�������ô�����*/
		DMA_SetCurrDataCounter(g_sGpsUart.RxDma.Stream, GPS_M8N_RX_BUFF_LENTH);

		/*ʹ��Debug Uart Rx*/
		msp_uart_recv_data(&g_sGpsUart);

		/*����жϱ�־*/
		USART_ClearITPendingBit(g_sGpsUart.Uart, USART_IT_IDLE);         
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}
#endif

/*UART5 DMA+IDLE*/
#if defined(HW_CUT__USE_OPTICFLOW)
u8 g_uUart5DMARxCnt        = 0; 
u8 g_uOpticFlowRxDataIndex = 0;
void UART5_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif
	
	if (USART_GetITStatus(g_sOpticalFlowUart.Uart, USART_IT_IDLE) != RESET)
	{
		/*ʧ��DMA*/
		msp_Dma_Disable(&g_sOpticalFlowUart.RxDma);
		
		/*��ȡ���յ�������*/
		USART_ReceiveData(g_sOpticalFlowUart.Uart);                                
		
		 /*��ȡ��ǰ���յ�������*/
		g_uUart5DMARxCnt = OPFLOW_UPIXELSLC306_RX_BUFF_LENTH - DMA_GetCurrDataCounter(g_sOpticalFlowUart.RxDma.Stream);
		
		/***********֡���ݽ���������************/
		/*��ʼ�����,���ܵ�����Ϊλ������֡*/
		if (g_sOpFlowUpixelsLC306.INIT_STATUS == OPFLOW_UPIXELSLC306_INIT_FINISH)
		{
			/*ֱ�ӶԽ�������ȫ�����ݽ���,���¹���λ������*/
			bsp_OPFLOW_UpixelsLC306_POS_Data_Parse(&g_sOpFlowUpixelsLC306, &g_sOpFlowUpixelsLC306RxFrame, g_OpFlowUpixelsLC306RxBuff);
			
			/*���̻߳�ȡOpticflow��������*/
			#if defined(PLATFORM_RTOS__RT_THREAD)
			rt_sem_release(&opflow_update_sem);
			#endif		
		}
		/*��ʼ��δ���,���ܵ�����ΪӦ������֡*/
		else if (g_sOpFlowUpixelsLC306.INIT_STATUS == OPFLOW_UPIXELSLC306_INIT_START)
		{
			/*Ӧ������ȶ�*/
			if (bsp_OPFLOW_UpixelsLC306_Response_Data_Parse(g_sOpticalFlowUart.pRxBuff) == SYS_RET_SUCC)
			{
				/*���Ӧ��*/
				g_sOpFlowUpixelsLC306.RESPONSE_STATUS = OPFLOW_UPIXELSLC306_RESPONSE_SUCC;
			}
			else
			{
				/*���δӦ��*/
				g_sOpFlowUpixelsLC306.RESPONSE_STATUS = OPFLOW_UPIXELSLC306_RESPONSE_FAIL;				
			}
		}
		
		/*����buff��0*/
		memset(g_sOpticalFlowUart.pRxBuff, 0, g_uUart5DMARxCnt);
		/*************************************/
		
		/*�������ô�����*/
		DMA_SetCurrDataCounter(g_sOpticalFlowUart.RxDma.Stream, OPFLOW_UPIXELSLC306_RX_BUFF_LENTH);

		/*ʹ��Debug Uart Rx*/
		msp_uart_recv_data(&g_sOpticalFlowUart);

		/*����жϱ�־*/
		USART_ClearITPendingBit(g_sOpticalFlowUart.Uart, USART_IT_IDLE);         
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}
#endif

/*UART6 DMA+IDLE*/
#if defined(HW_CUT__USE_ULTR)
u8 g_uUart6DMARxCnt   = 0; 
u8 g_uUltrRxDataIndex = 0;
void USART6_IRQHandler(void)
{
	#if defined(PLATFORM_RTOS__RT_THREAD)
	/* enter interrupt */
	rt_interrupt_enter();	
	#endif
	
	if (USART_GetITStatus(g_sUltrUart.Uart, USART_IT_IDLE) != RESET)
	{
		/*ʧ��DMA*/
		msp_Dma_Disable(&g_sUltrUart.RxDma);
		
		/*��ȡ���յ�������*/
		USART_ReceiveData(g_sUltrUart.Uart);                                
		
		 /*��ȡ��ǰ���յ�������*/
		g_uUart6DMARxCnt = ULTR_US100_RX_BUFF_LENTH - DMA_GetCurrDataCounter(g_sUltrUart.RxDma.Stream);
		
		/***********֡���ݽ���������************/
		bsp_US100_Data_Parse(&g_sUs100);
		
		/*����buff��0*/
		memset(g_sUltrUart.pRxBuff, 0, g_uUart6DMARxCnt);
		/*************************************/
		
		/*�������ô�����*/
		DMA_SetCurrDataCounter(g_sUltrUart.RxDma.Stream, ULTR_US100_RX_BUFF_LENTH);

		/*ʹ��Debug Uart Rx*/
		msp_uart_recv_data(&g_sUltrUart);

		/*����жϱ�־*/
		USART_ClearITPendingBit(g_sUltrUart.Uart, USART_IT_IDLE);         
	}
	
	#if defined(PLATFORM_RTOS__RT_THREAD)
    /* leave interrupt */
    rt_interrupt_leave();
	#endif	
}
#endif
