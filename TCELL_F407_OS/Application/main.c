#include "sys_OsTask.h"

/*ע:��Ӳ�����ⲿ������8Mhz,���ٷ��̼���׼��25Mhz;
	 ���ʹ�ñ��׳���ʱ,��Ҫ�����������޸�:
  1.HSE����Ƶ���޸�:
  stm32f4xx.h�ļ���,��144��:
  #define HSE_VALUE    ((uint32_t)8000000) �޸�Ϊ
  #define HSE_VALUE    ((uint32_t)25000000) ��ȷ���ⲿ����(25Mhz)
  
  2.ʱ��Դ��Ƶ��Ƶ�޸�:
	system_stm32f4xx.c�ļ���

	��371�� PLL_M (4)   �޸�Ϊ   PLL_M (25) 
	��384�� PLL_Q (4)   �޸�Ϊ   PLL_Q (7)
	��401�� PLL_N (168) �޸�Ϊ   PLL_N (336)
	��403�� PLL_P (2)   �޸�Ϊ   PLL_P (2)   
	
   3.������I2C����ģ��I2C,��ʱ����__NOP(),
   ����Ȳ��while(1--){__NOP();}��ʱ��
   �� mcu_driver_init() �е� nop_delay_init()
   ���ǰȡ��ע��,��ú����ע��
  */

int main(void)
{
	/*MCU�жϹ����ʼ��*/
	mcu_nvic_init();
	
	/*MCU������ʼ��*/
	mcu_driver_init();
	
	/*RTOS �����ʼ��(�������Ӳ����ʼ��ǰ��)*/
	rtos_unit_init();
	
	/*Ӳ����ʼ��*/
	hardware_init();
	
	/*ϵͳ������ʼ��/��ȡ*/
	uav_system_init();
	
	/*�����������߳�*/
	rtos_thread_create();
	
	/*����CPUʹ���ʼ���߳�(�����̵߳Ĺ��Ӻ�����ִ��)*/
	cpu_usage_init();
	
	/*ϵͳ��ʼ������*/
	work_after_system_init();
	
	return 0;
}

