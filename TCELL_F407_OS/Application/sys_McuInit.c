#include "sys_McuInit.h"

/*=== 0.���ݴ洢 ===*/
/*MCU FLASH*/
#if defined(HW_CUT__USE_FLASH_STOR)
#endif

/*EEPROM*/
#if defined(HW_CUT__USE_EEPROM_STOR)
#if defined(STD_PROTOCOL_HARDWARE_I2C)
MSP_I2c g_sEePromI2C = 
{
	.I2c 			 = I2C1,
	.I2C_CLOCK_SPEED = MSP_I2C_CLOCK_SPEED_100KHZ,
};
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
SSP_SimI2C g_sEePromSimI2C = {0};
#endif
#endif

/*SD CARD*/
#if defined(HW_CUT__USE_TFSD_STOR)
#endif

/*=== 1.IMU + ������ ===*/
/*IMU I2C*/
#if defined(HW_CUT__USE_MD_IMU)
#if defined(STD_PROTOCOL_HARDWARE_I2C)
MSP_I2c g_sImuI2C = 
{
	.I2c 			 = I2C1,
	.I2C_CLOCK_SPEED = MSP_I2C_CLOCK_SPEED_100KHZ,
};
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
SSP_SimI2C g_sImuSimI2C = {0};
#endif
#endif

/*MAG I2C*/
#if defined(HW_CUT__USE_MD_MAG)
#if defined(STD_PROTOCOL_HARDWARE_I2C)
MSP_I2c g_sMagI2C = 
{
	.I2c 			 = I2C1,
	.I2C_CLOCK_SPEED = MSP_I2C_CLOCK_SPEED_100KHZ,
};
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
SSP_SimI2C g_sMagSimI2C = {0};
#endif
#endif

/*GPS_MAG I2C*/
#if defined(HW_CUT__USE_GPS_MAG)
#if defined(STD_PROTOCOL_HARDWARE_I2C)
MSP_I2c g_sGpsMagI2C = 
{
	.I2c 			 = I2C1,
	.I2C_CLOCK_SPEED = MSP_I2C_CLOCK_SPEED_100KHZ,
};
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
SSP_SimI2C g_sGpsMagSimI2C = {0};
#endif
#endif

/*=== 2.��ѹ�� ===*/
/*BERO I2C*/
#if defined(HW_CUT__USE_MD_BERO)
#if defined(STD_PROTOCOL_HARDWARE_I2C)
MSP_I2c g_sBeroI2C = 
{
	.I2c 			 = I2C1,
	.I2C_CLOCK_SPEED = MSP_I2C_CLOCK_SPEED_100KHZ,
};
#endif

#if defined(STD_PROTOCOL_SOFTWARE_I2C)
SSP_SimI2C g_sBeroSimI2C = {0};
#endif
#endif

/*=== 3.������ ===*/
#if defined(HW_CUT__USE_ULTR)
/*ULTR UART*/
MSP_Uart g_sUltrUart = 
{
	.Uart 				   = USART6,
	.BaudRate 			   = 9600,	
	.NVIC_IRQChannel       = USART6_IRQn,
};
#endif

/*=== 4.GPS ===*/
#if defined(HW_CUT__USE_GPS)
/*GPS UART*/
MSP_Uart g_sGpsUart = 
{
	.Uart 				   = UART4,
	.BaudRate 			   = 115200,
	.NVIC_IRQChannel       = UART4_IRQn,
};
#endif

/*=== 5.���� ===*/
#if defined(HW_CUT__USE_OPTICFLOW)
/*OPTICALFLOW UART*/
MSP_Uart g_sOpticalFlowUart = 
{
	.Uart 				   = UART5,
	.BaudRate 			   = 19200,
	.NVIC_IRQChannel	   = UART5_IRQn,	
};
#endif

/*=== 6.�˻����� ===*/
#if defined(HW_CUT__USE_HCI_OLED)
/*OLED SIMSPI*/
SSP_SimSPI g_sOledSimSPI = {0};
#endif

/*MCU��ʼ��*/
void sys_Mcu_Peripheral_Init(void)
{
	SYS_RCC_TREE gpioRccTree;
	SYS_RCC_TREE periphRccTree;

/*======= SIMI2C =======*/
/*->1.IMU I2C Init: RCC + GPIO*/
#if defined(HW_CUT__USE_MD_IMU)
	#if defined(STD_PROTOCOL_HARDWARE_I2C)
	g_sImuI2C.SCL_GPIO            = GPIOB;
	g_sImuI2C.SCL_Pin             = GPIO_Pin_6;
	g_sImuI2C.SCL_PinSource 	  = GPIO_PinSource6;
	g_sImuI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sImuI2C.SDA_GPIO            = GPIOB;
	g_sImuI2C.SDA_Pin             = GPIO_Pin_7;
	g_sImuI2C.SDA_PinSource 	  = GPIO_PinSource7;	
	g_sImuI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sImuI2C.GPIO_AF			  = GPIO_AF_I2C1;			/*GPIO����*/
	g_sImuI2C.RCC_Periph_I2C      = RCC_APB1Periph_I2C1;	/*I2Cʱ��*/
	gpioRccTree	                  = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				  = SYS_RCC_APB1;			/*�趨I2Cʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sImuI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sImuI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sImuI2C.RCC_Periph_I2C, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_I2C, &g_sImuI2C);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	g_sImuSimI2C.SCL_GPIO            = GPIOB;
	g_sImuSimI2C.SCL_Pin             = GPIO_Pin_6;
	g_sImuSimI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	g_sImuSimI2C.SDA_GPIO            = GPIOB;
	g_sImuSimI2C.SDA_Pin             = GPIO_Pin_7;
	g_sImuSimI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	gpioRccTree	                     = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sImuSimI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sImuSimI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMI2C, &g_sImuSimI2C);
	#endif
#endif	
	
/*->2.Mag I2C Init: RCC + GPIO*/
#if defined(HW_CUT__USE_MD_MAG)
	#if defined(STD_PROTOCOL_HARDWARE_I2C)
	g_sMagI2C.SCL_GPIO            = GPIOB;
	g_sMagI2C.SCL_Pin             = GPIO_Pin_6;
	g_sMagI2C.SCL_PinSource 	  = GPIO_PinSource6;
	g_sMagI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sMagI2C.SDA_GPIO            = GPIOB;
	g_sMagI2C.SDA_Pin             = GPIO_Pin_7;
	g_sMagI2C.SDA_PinSource 	  = GPIO_PinSource7;	
	g_sMagI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sMagI2C.GPIO_AF			  = GPIO_AF_I2C1;			/*GPIO����*/
	g_sMagI2C.RCC_Periph_I2C      = RCC_APB1Periph_I2C1;	/*I2Cʱ��*/
	gpioRccTree	                  = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				  = SYS_RCC_APB1;			/*�趨I2Cʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sMagI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sMagI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sMagI2C.RCC_Periph_I2C, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_I2C, &g_sMagI2C);	
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	g_sMagSimI2C.SCL_GPIO            = GPIOB;
	g_sMagSimI2C.SCL_Pin             = GPIO_Pin_6;
	g_sMagSimI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	g_sMagSimI2C.SDA_GPIO            = GPIOB;
	g_sMagSimI2C.SDA_Pin             = GPIO_Pin_7;
	g_sMagSimI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	gpioRccTree	                     = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sMagSimI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sMagSimI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMI2C, &g_sMagSimI2C);	
	#endif
#endif	
	
/*->3.BERO SimI2C Init: RCC + GPIO*/
#if defined(HW_CUT__USE_MD_BERO)
	#if defined(STD_PROTOCOL_HARDWARE_I2C)
	g_sBeroI2C.SCL_GPIO            = GPIOB;
	g_sBeroI2C.SCL_Pin             = GPIO_Pin_6;
	g_sBeroI2C.SCL_PinSource 	   = GPIO_PinSource6;
	g_sBeroI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sBeroI2C.SDA_GPIO            = GPIOB;
	g_sBeroI2C.SDA_Pin             = GPIO_Pin_7;
	g_sBeroI2C.SDA_PinSource 	   = GPIO_PinSource7;	
	g_sBeroI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sBeroI2C.GPIO_AF			   = GPIO_AF_I2C1;			/*GPIO����*/
	g_sBeroI2C.RCC_Periph_I2C      = RCC_APB1Periph_I2C1;	/*I2Cʱ��*/
	gpioRccTree	                   = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				   = SYS_RCC_APB1;			/*�趨I2Cʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sBeroI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sBeroI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sBeroI2C.RCC_Periph_I2C, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_I2C, &g_sBeroI2C);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	g_sBeroSimI2C.SCL_GPIO            = GPIOB;
	g_sBeroSimI2C.SCL_Pin             = GPIO_Pin_6;
	g_sBeroSimI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	g_sBeroSimI2C.SDA_GPIO            = GPIOB;
	g_sBeroSimI2C.SDA_Pin             = GPIO_Pin_7;
	g_sBeroSimI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	gpioRccTree	                      = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sBeroSimI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sBeroSimI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMI2C, &g_sBeroSimI2C);	
	#endif	
#endif	
	
/*->4.EEPROM SimI2C Init: RCC + GPIO*/
#if defined(HW_CUT__USE_EEPROM_STOR)
	#if defined(STD_PROTOCOL_HARDWARE_I2C)	
	g_sEePromI2C.SCL_GPIO            = GPIOB;
	g_sEePromI2C.SCL_Pin             = GPIO_Pin_6;
	g_sEePromI2C.SCL_PinSource 	     = GPIO_PinSource6;
	g_sEePromI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sEePromI2C.SDA_GPIO            = GPIOB;
	g_sEePromI2C.SDA_Pin             = GPIO_Pin_7;
	g_sEePromI2C.SDA_PinSource 	     = GPIO_PinSource7;	
	g_sEePromI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sEePromI2C.GPIO_AF			 = GPIO_AF_I2C1;			/*GPIO����*/
	g_sEePromI2C.RCC_Periph_I2C      = RCC_APB1Periph_I2C1;		/*I2Cʱ��*/
	gpioRccTree	                     = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				     = SYS_RCC_APB1;			/*�趨I2Cʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sEePromI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sEePromI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sEePromI2C.RCC_Periph_I2C, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_I2C, &g_sEePromI2C);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	g_sEePromSimI2C.SCL_GPIO            = GPIOB;
	g_sEePromSimI2C.SCL_Pin             = GPIO_Pin_6;
	g_sEePromSimI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	g_sEePromSimI2C.SDA_GPIO            = GPIOB;
	g_sEePromSimI2C.SDA_Pin             = GPIO_Pin_7;
	g_sEePromSimI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	gpioRccTree	                        = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sEePromSimI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sEePromSimI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMI2C, &g_sEePromSimI2C);	
	#endif	
#endif	
	
/*->5.GPS_MAG SimI2C Init: RCC + GPIO*/
#if defined(HW_CUT__USE_GPS_MAG)
	#if defined(STD_PROTOCOL_HARDWARE_I2C)
	g_sGpsMagI2C.SCL_GPIO            = GPIOB;
	g_sGpsMagI2C.SCL_Pin             = GPIO_Pin_6;
	g_sGpsMagI2C.SCL_PinSource 	     = GPIO_PinSource6;
	g_sGpsMagI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sGpsMagI2C.SDA_GPIO            = GPIOB;
	g_sGpsMagI2C.SDA_Pin             = GPIO_Pin_7;
	g_sGpsMagI2C.SDA_PinSource 	     = GPIO_PinSource7;	
	g_sGpsMagI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOB;
	
	g_sGpsMagI2C.GPIO_AF			 = GPIO_AF_I2C1;			/*GPIO����*/
	g_sGpsMagI2C.RCC_Periph_I2C      = RCC_APB1Periph_I2C1;		/*I2Cʱ��*/
	gpioRccTree	                     = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				     = SYS_RCC_APB1;			/*�趨I2Cʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsMagI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsMagI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sGpsMagI2C.RCC_Periph_I2C, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_I2C, &g_sGpsMagI2C);
	#endif
	
	#if defined(STD_PROTOCOL_SOFTWARE_I2C)
	g_sGpsMagSimI2C.SCL_GPIO            = GPIOD;
	g_sGpsMagSimI2C.SCL_Pin             = GPIO_Pin_5;
	g_sGpsMagSimI2C.RCC_Periph_SCL_GPIO = RCC_AHB1Periph_GPIOD;
	g_sGpsMagSimI2C.SDA_GPIO            = GPIOD;
	g_sGpsMagSimI2C.SDA_Pin             = GPIO_Pin_6;
	g_sGpsMagSimI2C.RCC_Periph_SDA_GPIO = RCC_AHB1Periph_GPIOD;
	gpioRccTree	                        = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsMagSimI2C.RCC_Periph_SCL_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsMagSimI2C.RCC_Periph_SDA_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMI2C, &g_sGpsMagSimI2C);		
	#endif
#endif	

/*=== SIMSPI ===*/
/*->1.OLED SimSPI Init: RCC + GPIO*/
#if defined(HW_CUT__USE_HCI_OLED)
/*SPI����---�滻*/
#if (STD_PROTOCOL_SPI_PORT_REPLACE == SYS_ENABLE)
	g_sOledSimSPI.MOSI_GPIO			   = GPIOB;
	g_sOledSimSPI.MOSI_Pin 			   = GPIO_Pin_10;
	g_sOledSimSPI.RCC_Periph_MOSI_GPIO = RCC_AHB1Periph_GPIOB;	/*OLED_D1*/
	g_sOledSimSPI.CLK_GPIO 			   = GPIOB;
	g_sOledSimSPI.CLK_Pin              = GPIO_Pin_11;
	g_sOledSimSPI.RCC_Periph_CLK_GPIO  = RCC_AHB1Periph_GPIOB;	/*OLED_D0*/
	g_sOledSimSPI.DC_GPIO              = GPIOB;
	g_sOledSimSPI.DC_Pin               = GPIO_Pin_8;
	g_sOledSimSPI.RCC_Periph_DC_GPIO   = RCC_AHB1Periph_GPIOB;	/*OLED_DC*/
	g_sOledSimSPI.RST_GPIO 			   = GPIOB;
	g_sOledSimSPI.RST_Pin 			   = GPIO_Pin_9;
	g_sOledSimSPI.RCC_Periph_RST_GPIO  = RCC_AHB1Periph_GPIOB;	/*OLED_RES*/
	g_sOledSimSPI.CS_GPIO 			   = GPIOC;
	g_sOledSimSPI.CS_Pin 			   = GPIO_Pin_1;
	g_sOledSimSPI.RCC_Periph_CS_GPIO   = RCC_AHB1Periph_GPIOC;	/*OLED_CS*/
	
#else
	
	g_sOledSimSPI.MOSI_GPIO			   = GPIOB;
	g_sOledSimSPI.MOSI_Pin 			   = GPIO_Pin_15;
	g_sOledSimSPI.RCC_Periph_MOSI_GPIO = RCC_AHB1Periph_GPIOB;	/*OLED_D1*/
	g_sOledSimSPI.CLK_GPIO 			   = GPIOB;
	g_sOledSimSPI.CLK_Pin              = GPIO_Pin_13;
	g_sOledSimSPI.RCC_Periph_CLK_GPIO  = RCC_AHB1Periph_GPIOB;	/*OLED_D0*/
	g_sOledSimSPI.DC_GPIO              = GPIOD;
	g_sOledSimSPI.DC_Pin               = GPIO_Pin_8;
	g_sOledSimSPI.RCC_Periph_DC_GPIO   = RCC_AHB1Periph_GPIOD;	/*OLED_DC*/
	g_sOledSimSPI.RST_GPIO 			   = GPIOD;
	g_sOledSimSPI.RST_Pin 			   = GPIO_Pin_9;
	g_sOledSimSPI.RCC_Periph_RST_GPIO  = RCC_AHB1Periph_GPIOD;	/*OLED_RES*/
	g_sOledSimSPI.CS_GPIO 			   = GPIOB;
	g_sOledSimSPI.CS_Pin 			   = GPIO_Pin_12;
	g_sOledSimSPI.RCC_Periph_CS_GPIO   = RCC_AHB1Periph_GPIOB;	/*OLED_CS*/	
#endif	
	
	gpioRccTree	                       = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/	
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOledSimSPI.RCC_Periph_MOSI_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOledSimSPI.RCC_Periph_CLK_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOledSimSPI.RCC_Periph_DC_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOledSimSPI.RCC_Periph_RST_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOledSimSPI.RCC_Periph_CS_GPIO, ENABLE);	
	
	/*GPIO Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_SIMSPI, &g_sOledSimSPI);	
#endif	
	
/*======= UART =======*/
/*->1.GPS Uart Init: RCC + GPIO + UART + RxDMA*/
#if defined(HW_CUT__USE_GPS)
	g_sGpsUart.Tx_GPIO 			  = GPIOA;
	g_sGpsUart.Tx_Pin 			  = GPIO_Pin_0;
	g_sGpsUart.Tx_PinSource	      = GPIO_PinSource0;
	g_sGpsUart.RCC_Periph_Tx_GPIO = RCC_AHB1Periph_GPIOA;
	
	g_sGpsUart.Rx_GPIO 			  = GPIOA;
	g_sGpsUart.Rx_Pin 			  = GPIO_Pin_1;
	g_sGpsUart.Rx_PinSource	      = GPIO_PinSource1;
	g_sGpsUart.RCC_Periph_Rx_GPIO = RCC_AHB1Periph_GPIOA;	
	
	g_sGpsUart.GPIO_AF			  = GPIO_AF_UART4;			/*GPIO����*/
	g_sGpsUart.RCC_Periph_UART    = RCC_APB1Periph_UART4;	/*UARTʱ��*/
	gpioRccTree	                  = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				  = SYS_RCC_APB1;			/*�趨Uartʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsUart.RCC_Periph_Tx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sGpsUart.RCC_Periph_Rx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(periphRccTree, g_sGpsUart.RCC_Periph_UART, ENABLE);
	
	/*USART ����*/
//	g_sGpsUart.Uart     = UART4;
//	g_sGpsUart.BaudRate = 9600;
	g_sGpsUart.Mode     = USART_Mode_Rx | USART_Mode_Tx;	/*�շ���ģʽ*/
	
	/*USART������ʽ, ����:DMA ����:POLL*/
	g_sGpsUart.RxMode = MSP_UART_DMA_IDLE;
	g_sGpsUart.TxMode = MSP_UART_POLL;
	
	/*DMA / DMA+IDLE*/	
	if ((g_sGpsUart.RxMode == MSP_UART_DMA) || (g_sGpsUart.RxMode == MSP_UART_DMA_IDLE))
	{
		/*���ý���DMA*/
		g_sGpsUart.RxDma.Stream         = DMA1_Stream2;	               /*DMA��*/
		g_sGpsUart.RxDma.Channel        = DMA_Channel_4;		       /*DMAͨ��*/
		g_sGpsUart.RxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA1;         /*DMAʱ��*/
		g_sGpsUart.RxDma.BuffSize 	    = GPS_M8N_RX_BUFF_LENTH;       /*buff(������)��С*/
		g_sGpsUart.RxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte; /*1�ֽڶ���*/
		g_sGpsUart.RxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;     /*1�ֽڶ���*/
		g_sGpsUart.RxDma.Mode           = DMA_Mode_Circular;		   /*���ν���*/
		g_sGpsUart.RxDma.Priority       = DMA_Priority_High;		   /*���ȼ�*/
		g_sGpsUart.RxDma.dmaFlag        = DMA_IT_TCIF2;			       /*Stream������ϱ�־*/
		g_sGpsUart.RxDma.dmaIT		    = DMA_IT_DISABLE;			   /*����DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sGpsUart.RxDma.dmaIT == DMA_IT_ENABLE)
		{
			g_sGpsUart.RxDma.ITType          = DMA_IT_TC;
			g_sGpsUart.RxDma.NVIC_IRQChannel = DMA1_Stream2_IRQn;				
		}
		
		/*ʹ�ܽ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1;  /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sGpsUart.RxDma.RCC_Periph_DMA, ENABLE);
	}
	
	/*DMA / DMA+IDLE*/	
	if ((g_sGpsUart.TxMode == MSP_UART_DMA) || (g_sGpsUart.TxMode == MSP_UART_DMA_IDLE))
	{	
		/*���÷���DMA*/ 
		g_sGpsUart.TxDma.Stream         = DMA1_Stream4;	                 /*DMA��*/
		g_sGpsUart.TxDma.Channel        = DMA_Channel_4;		         /*DMAͨ��*/
		g_sGpsUart.TxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA1;           /*DMAʱ��*/
		g_sGpsUart.TxDma.BuffSize 	    = GPS_M8N_TX_BUFF_LENTH; 		 /*buff(������)��С*/
		g_sGpsUart.TxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte;   /*1�ֽڶ���*/
		g_sGpsUart.TxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;       /*1�ֽڶ���*/
		g_sGpsUart.TxDma.Mode           = DMA_Mode_Normal;		         /*�����ݲŷ���*/
		g_sGpsUart.TxDma.Priority       = DMA_Priority_Medium;		     /*���ȼ�*/
		g_sGpsUart.TxDma.dmaFlag        = DMA_FLAG_TCIF4;				 /*Stream������ϱ�־*/
		g_sGpsUart.TxDma.dmaIT		    = DMA_IT_DISABLE;			     /*������DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sGpsUart.TxDma.dmaIT == DMA_IT_ENABLE)
		{
			g_sGpsUart.TxDma.ITType          = DMA_IT_TC;
			g_sGpsUart.TxDma.NVIC_IRQChannel = DMA1_Stream4_IRQn;			
		}		

		/*ʹ�ܷ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1; 	 /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sGpsUart.TxDma.RCC_Periph_DMA, ENABLE);		
	}		
	
	/*ָ������/�����ڴ�*/
	g_sGpsUart.pTxBuff = g_GpsM8nTxBuff;
	g_sGpsUart.pRxBuff = g_GpsM8nRxBuff;	
	
	/*GPIO & UART Init*/	
	msp_Peripheral_GPIO_Init(SYS_GPIO_UART, &g_sGpsUart);
#endif	

/*->2.ULTR Uart Init: RCC + GPIO + UART + RxDMA*/
#if defined(HW_CUT__USE_ULTR)	
	g_sUltrUart.Tx_GPIO 		   = GPIOC;
	g_sUltrUart.Tx_Pin 			   = GPIO_Pin_6;
	g_sUltrUart.Tx_PinSource	   = GPIO_PinSource6;
	g_sUltrUart.RCC_Periph_Tx_GPIO = RCC_AHB1Periph_GPIOC;
	
	g_sUltrUart.Rx_GPIO 		   = GPIOC;
	g_sUltrUart.Rx_Pin 			   = GPIO_Pin_7;
	g_sUltrUart.Rx_PinSource	   = GPIO_PinSource7;
	g_sUltrUart.RCC_Periph_Rx_GPIO = RCC_AHB1Periph_GPIOC;	
	
	g_sUltrUart.GPIO_AF			   = GPIO_AF_USART6;		/*GPIO����*/
	g_sUltrUart.RCC_Periph_UART    = RCC_APB2Periph_USART6;	/*UARTʱ��*/
	gpioRccTree	                   = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				   = SYS_RCC_APB2;			/*�趨Uartʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sUltrUart.RCC_Periph_Tx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sUltrUart.RCC_Periph_Rx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(periphRccTree, g_sUltrUart.RCC_Periph_UART, ENABLE);
	
	/*USART ����*/
//	g_sUltrUart.Uart     = USART6;
//	g_sUltrUart.BaudRate = 9600;
	g_sUltrUart.Mode     = USART_Mode_Rx | USART_Mode_Tx;	/*�շ���ģʽ*/
	
	/*USART������ʽ, ����:DMA ����:POLL*/
	g_sUltrUart.RxMode = MSP_UART_DMA_IDLE;
	g_sUltrUart.TxMode = MSP_UART_POLL;
	
	/*DMA / DMA+IDLE*/	
	if ((g_sUltrUart.RxMode == MSP_UART_DMA) || (g_sUltrUart.RxMode == MSP_UART_DMA_IDLE))
	{
		/*���ý���DMA*/
		g_sUltrUart.RxDma.Stream         = DMA2_Stream1;	            /*DMA��*/
		g_sUltrUart.RxDma.Channel        = DMA_Channel_5;		        /*DMAͨ��*/
		g_sUltrUart.RxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA2;         /*DMAʱ��*/
		g_sUltrUart.RxDma.BuffSize 	     = ULTR_US100_RX_BUFF_LENTH;    /*buff(������)��С*/
		g_sUltrUart.RxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte; /*1�ֽڶ���*/
		g_sUltrUart.RxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;     /*1�ֽڶ���*/
		g_sUltrUart.RxDma.Mode           = DMA_Mode_Circular;		   	/*����ģʽ*/
		g_sUltrUart.RxDma.Priority       = DMA_Priority_Medium;		    /*���ȼ�*/
		g_sUltrUart.RxDma.dmaFlag        = DMA_IT_TCIF1;			    /*Stream������ϱ�־*/
		g_sUltrUart.RxDma.dmaIT		     = DMA_IT_DISABLE;			    /*����DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sUltrUart.RxDma.dmaIT == DMA_IT_ENABLE)
		{
			g_sUltrUart.RxDma.ITType          = DMA_IT_TC;
			g_sUltrUart.RxDma.NVIC_IRQChannel = DMA2_Stream1_IRQn;				
		}
		
		/*ʹ�ܽ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1;  /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sUltrUart.RxDma.RCC_Periph_DMA, ENABLE);
	}
	
	/*DMA / DMA+IDLE*/	
	if ((g_sGpsUart.TxMode == MSP_UART_DMA) || (g_sGpsUart.TxMode == MSP_UART_DMA_IDLE))
	{	
		/*���÷���DMA*/ 
		g_sUltrUart.TxDma.Stream         = DMA2_Stream6;	             /*DMA��*/
		g_sUltrUart.TxDma.Channel        = DMA_Channel_5;		         /*DMAͨ��*/
		g_sUltrUart.TxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA2;          /*DMAʱ��*/
		g_sUltrUart.TxDma.BuffSize 	     = ULTR_US100_TX_BUFF_LENTH;     /*buff(������)��С*/
		g_sUltrUart.TxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte;  /*1�ֽڶ���*/
		g_sUltrUart.TxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;      /*1�ֽڶ���*/
		g_sUltrUart.TxDma.Mode           = DMA_Mode_Normal;		         /*�����ݲŷ���*/
		g_sUltrUart.TxDma.Priority       = DMA_Priority_Medium;		     /*���ȼ�*/
		g_sUltrUart.TxDma.dmaFlag        = DMA_FLAG_TCIF6;				 /*Stream������ϱ�־*/
		g_sUltrUart.TxDma.dmaIT		     = DMA_IT_DISABLE;			     /*������DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sUltrUart.TxDma.dmaIT == DMA_IT_ENABLE)
		{
			g_sUltrUart.TxDma.ITType          = DMA_IT_TC;
			g_sUltrUart.TxDma.NVIC_IRQChannel = DMA2_Stream6_IRQn;			
		}		

		/*ʹ�ܷ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1; 	 /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sUltrUart.TxDma.RCC_Periph_DMA, ENABLE);		
	}		
	
	/*ָ������/�����ڴ�*/
	g_sUltrUart.pTxBuff = g_Us100TxBuff;
	g_sUltrUart.pRxBuff = g_Us100RxBuff;	
	
	/*GPIO & UART Init*/	
	msp_Peripheral_GPIO_Init(SYS_GPIO_UART, &g_sUltrUart);
#endif	
	
/*->3.OPTICALFLOW Uart Init: RCC + GPIO + UART + RxDMA*/
#if defined(HW_CUT__USE_OPTICFLOW)
	g_sOpticalFlowUart.Tx_GPIO 			  = GPIOC;
	g_sOpticalFlowUart.Tx_Pin 			  = GPIO_Pin_12;
	g_sOpticalFlowUart.Tx_PinSource	      = GPIO_PinSource12;
	g_sOpticalFlowUart.RCC_Periph_Tx_GPIO = RCC_AHB1Periph_GPIOC;
	
	g_sOpticalFlowUart.Rx_GPIO 			  = GPIOD;
	g_sOpticalFlowUart.Rx_Pin 			  = GPIO_Pin_2;
	g_sOpticalFlowUart.Rx_PinSource	      = GPIO_PinSource2;
	g_sOpticalFlowUart.RCC_Periph_Rx_GPIO = RCC_AHB1Periph_GPIOD;	
	
	g_sOpticalFlowUart.GPIO_AF			  = GPIO_AF_UART5;			/*GPIO����*/
	g_sOpticalFlowUart.RCC_Periph_UART    = RCC_APB1Periph_UART5;	/*UARTʱ��*/
	gpioRccTree	                          = SYS_RCC_AHB1;			/*�趨GPIOʱ����*/
	periphRccTree				          = SYS_RCC_APB1;			/*�趨Uartʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOpticalFlowUart.RCC_Periph_Tx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sOpticalFlowUart.RCC_Periph_Rx_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(periphRccTree, g_sOpticalFlowUart.RCC_Periph_UART, ENABLE);
	
	/*USART ����*/
//	g_sOpticalFlowUart.Uart     = UART5;
//	g_sOpticalFlowUart.BaudRate = 19200;
	g_sOpticalFlowUart.Mode     = USART_Mode_Rx | USART_Mode_Tx;	/*�շ���ģʽ*/
	
	/*USART������ʽ, ����:DMA ����:POLL*/
	g_sOpticalFlowUart.RxMode = MSP_UART_DMA_IDLE;
	g_sOpticalFlowUart.TxMode = MSP_UART_POLL;
	
	/*DMA / DMA+IDLE*/	
	if ((g_sOpticalFlowUart.RxMode == MSP_UART_DMA) || (g_sOpticalFlowUart.RxMode == MSP_UART_DMA_IDLE))
	{
		/*���ý���DMA*/
		g_sOpticalFlowUart.RxDma.Stream         = DMA1_Stream0;	                     /*DMA��*/
		g_sOpticalFlowUart.RxDma.Channel        = DMA_Channel_4;		             /*DMAͨ��*/
		g_sOpticalFlowUart.RxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA1;               /*DMAʱ��*/
		g_sOpticalFlowUart.RxDma.BuffSize 	    = OPFLOW_UPIXELSLC306_RX_BUFF_LENTH; /*buff(������)��С*/
		g_sOpticalFlowUart.RxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte;   	 /*1�ֽڶ���*/
		g_sOpticalFlowUart.RxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;       	 /*1�ֽڶ���*/
		g_sOpticalFlowUart.RxDma.Mode           = DMA_Mode_Circular;		   	     /*ѭ������*/
		g_sOpticalFlowUart.RxDma.Priority       = DMA_Priority_Medium;		     	 /*���ȼ�*/
		g_sOpticalFlowUart.RxDma.dmaFlag        = DMA_IT_TCIF0;			         	 /*Stream������ϱ�־*/
		g_sOpticalFlowUart.RxDma.dmaIT		    = DMA_IT_DISABLE;			     	 /*����DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sOpticalFlowUart.RxDma.dmaIT == DMA_IT_ENABLE)
		{
		
			g_sOpticalFlowUart.RxDma.ITType          = DMA_IT_TC;
			g_sOpticalFlowUart.RxDma.NVIC_IRQChannel = DMA1_Stream0_IRQn;				
		}
		
		/*ʹ�ܽ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1;  /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sOpticalFlowUart.RxDma.RCC_Periph_DMA, ENABLE);
	}
	
	/*DMA / DMA+IDLE*/	
	if ((g_sOpticalFlowUart.TxMode == MSP_UART_DMA) || (g_sOpticalFlowUart.TxMode == MSP_UART_DMA_IDLE))
	{	
		/*���÷���DMA*/ 
		g_sOpticalFlowUart.TxDma.Stream         = DMA1_Stream7;	                 	 /*DMA��*/
		g_sOpticalFlowUart.TxDma.Channel        = DMA_Channel_4;		          	 /*DMAͨ��*/
		g_sOpticalFlowUart.TxDma.RCC_Periph_DMA = RCC_AHB1Periph_DMA1;          	 /*DMAʱ��*/
		g_sOpticalFlowUart.TxDma.BuffSize 	    = OPFLOW_UPIXELSLC306_TX_BUFF_LENTH; /*buff(������)��С*/
		g_sOpticalFlowUart.TxDma.PerAlignByte   = DMA_PeripheralDataSize_Byte;       /*1�ֽڶ���*/
		g_sOpticalFlowUart.TxDma.MemAlignByte   = DMA_MemoryDataSize_Byte;           /*1�ֽڶ���*/
		g_sOpticalFlowUart.TxDma.Mode           = DMA_Mode_Normal;		        	 /*�����ݲŷ���*/
		g_sOpticalFlowUart.TxDma.Priority       = DMA_Priority_Medium;		     	 /*���ȼ�*/
		g_sOpticalFlowUart.TxDma.dmaFlag        = DMA_FLAG_TCIF7;				 	 /*Stream������ϱ�־*/
		g_sOpticalFlowUart.TxDma.dmaIT		    = DMA_IT_DISABLE;			     	 /*������DMA�ж�*/
		
		/*�Ƿ�ʹ��DMA�����ж�,��ʹ�þ�ѡ���ж�����,�������ж�����ͨ��*/
		if (g_sOpticalFlowUart.TxDma.dmaIT == DMA_IT_ENABLE)
		{
			g_sOpticalFlowUart.TxDma.ITType          = DMA_IT_TC;
			g_sOpticalFlowUart.TxDma.NVIC_IRQChannel = DMA1_Stream7_IRQn;			
		}		

		/*ʹ�ܷ���DMAʱ��*/
		periphRccTree = SYS_RCC_AHB1; 	 /*�趨DMAʱ����*/
		sys_Peripheral_RCC_Init(periphRccTree, g_sOpticalFlowUart.TxDma.RCC_Periph_DMA, ENABLE);		
	}		
	
	/*ָ������/�����ڴ�*/
	g_sOpticalFlowUart.pTxBuff = g_OpFlowUpixelsLC306TxBuff;
	g_sOpticalFlowUart.pRxBuff = g_OpFlowUpixelsLC306RxBuff;	
	
	/*GPIO & UART Init*/	
	msp_Peripheral_GPIO_Init(SYS_GPIO_UART, &g_sOpticalFlowUart);
#endif	
}

/*=== TIM_Alarm ===*/
/*������ȶ�ʱ��_Main(Alarm)*/
TimAlarm g_sTimAlarmTaskMain = 
{
	.Tim             = TIM7,
	.Resource        = MSP_SRC_TIM7,
	.NVIC_IRQChannel = TIM7_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim  = RCC_APB1Periph_TIM7,
	.RCC_Tree_Mhz    = 84,
	.Prescaler       = 84 - 1,	/*1Mhz, 1us*/
	.Period          = PLATFORM_TASK_SCHEDULER_MIN_FOC_MS * 1000, /*us*/
	.ClockDivision   = TIM_CKD_DIV1,
};

/*������ȶ�ʱ��_Assist(Alarm)*/
TimAlarm g_sTimAlarmTaskAssist = 
{
	.Tim             = TIM2,
	.Resource        = MSP_SRC_TIM2,
	.NVIC_IRQChannel = TIM2_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim  = RCC_APB1Periph_TIM2,
	.RCC_Tree_Mhz    = 84,
	.Prescaler       = 84 - 1,	/*1Mhz, 1us*/
	.Period          = PLATFORM_TASK_SCHEDULER_MIN_FOC_MS * 1000, /*us*/
	.ClockDivision   = TIM_CKD_DIV1,
};

/*ִ�м��������(10ms)*/
TimAlarm g_sTimExecutePeriod = 
{
	.Tim             = TIM9,
	.Resource        = MSP_SRC_TIM9,
	.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim  = RCC_APB2Periph_TIM9,
	.RCC_Tree_Mhz    = 168,		
	.Prescaler       = 168 - 1,	/*1Mhz, 1us*/
	.Period          = 10000,	/*10000us = 10ms*/
	.ClockDivision   = TIM_CKD_DIV1,
};

TimAlarm *g_psTimExecutePeriod = &g_sTimExecutePeriod;

/*=== TIM_PWM_IN ===*/
#if defined(REMOTE_DATA_RECV__PWM)
/*PWM_Inͨ���е�Switch(TIM1)����*/
TimPwmIn g_sTimPwmIn_Switch = 
{
	.Tim             	 = TIM1,
	.Resource        	 = MSP_SRC_TIM1,
	.NVIC_IRQChannel 	 = TIM1_CC_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim  	 = RCC_APB2Periph_TIM1,
	.RCC_Tree_Mhz    	 = 168,
	.Prescaler       	 = 168 - 1,	/*1Mhz, 1us*/
	.Period          	 = 0xFFFF,	/*ң�ز���1000us~2000us / 20ms����*/
	.ClockDivision   	 = TIM_CKD_DIV1,
	
	/*Switch-SWA SWB*/
	/*Tim1_Ch1(PWMIN_8)*/
	.CH1_GPIO            = GPIOE,
	.CH1_Pin             = GPIO_Pin_9,
	.RCC_Periph_CH1_GPIO = RCC_AHB1Periph_GPIOE,
	.CH1_PinSource       = GPIO_PinSource9,

	/*Tim1_Ch2(PWMIN_7)*/	
	.CH2_GPIO            = GPIOE,
	.CH2_Pin             = GPIO_Pin_11,
	.RCC_Periph_CH2_GPIO = RCC_AHB1Periph_GPIOE,
	.CH2_PinSource       = GPIO_PinSource11,

	/*Gimbal-�����*/	
	/*Tim1_Ch3(PWMIN_6)*/
	.CH3_GPIO            = GPIOE,
	.CH3_Pin             = GPIO_Pin_13,
	.RCC_Periph_CH3_GPIO = RCC_AHB1Periph_GPIOE,
	.CH3_PinSource       = GPIO_PinSource13,

	/*Tim1_Ch4(PWMIN_5)*/
	.CH4_GPIO            = GPIOE,
	.CH4_Pin             = GPIO_Pin_14,
	.RCC_Periph_CH4_GPIO = RCC_AHB1Periph_GPIOE,
	.CH4_PinSource       = GPIO_PinSource14,
	
	/*pin Map*/
	.GPIO_AF			 = GPIO_AF_TIM1,
};

/*PWM_Inͨ���е�Attitude(TIM4)����*/
TimPwmIn g_sTimPwmIn_Attitude = 
{
	.Tim             	 = TIM4,
	.Resource        	 = MSP_SRC_TIM4,
	.NVIC_IRQChannel 	 = TIM4_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim  	 = RCC_APB1Periph_TIM4,
	.RCC_Tree_Mhz    	 = 84,
	.Prescaler       	 = 84 - 1,	/*1Mhz, 1us*/
	.Period          	 = 0xFFFF,	/*ң�ز���1000us~2000us / 2ms����*/
	.ClockDivision   	 = TIM_CKD_DIV1,
	
	/*Tim4_Ch1(PWMIN_1)*/
	.CH1_GPIO            = GPIOD,
	.CH1_Pin             = GPIO_Pin_12,
	.RCC_Periph_CH1_GPIO = RCC_AHB1Periph_GPIOD,
	.CH1_PinSource       = GPIO_PinSource12,

	/*Tim4_Ch2(PWMIN_2)*/	
	.CH2_GPIO            = GPIOD,
	.CH2_Pin             = GPIO_Pin_13,
	.RCC_Periph_CH2_GPIO = RCC_AHB1Periph_GPIOD,
	.CH2_PinSource       = GPIO_PinSource13,

	/*Tim4_Ch3(PWMIN_3)*/
	.CH3_GPIO            = GPIOD,
	.CH3_Pin             = GPIO_Pin_14,
	.RCC_Periph_CH3_GPIO = RCC_AHB1Periph_GPIOD,
	.CH3_PinSource       = GPIO_PinSource14,

	/*Tim4_Ch4(PWMIN_4)*/
	.CH4_GPIO            = GPIOD,
	.CH4_Pin             = GPIO_Pin_15,
	.RCC_Periph_CH4_GPIO = RCC_AHB1Periph_GPIOD,
	.CH4_PinSource       = GPIO_PinSource15,
	
	/*pin Map*/
	.GPIO_AF			 = GPIO_AF_TIM4,	
};
#endif

/*=== TIM_PWM_OUT ===*/
TimPwmOut g_sTimPwmOut_Motor =
{
	.Tim 				 = TIM3,
	.Resource 			 = MSP_SRC_TIM3,
	.NVIC_IRQChannel 	 = TIM3_IRQn,	/*�ж����ȫ�ֱ��������ʼ��*/
	.RCC_Periph_Tim 	 = RCC_APB1Periph_TIM3,
	.RCC_Tree_Mhz 		 = 84,
	.Prescaler 			 = 84 - 1,		/*1Mhz,1us*/
	.Period 			 = 2000 - 1,	/*2000us,2ms*/
	.ClockDivision		 = TIM_CKD_DIV1,


	/*Tim3_Ch1(PWMOUT_4)*/	
	.CH1_GPIO 			 = GPIOA,
	.CH1_Pin		     = GPIO_Pin_6,
	.RCC_Periph_CH1_GPIO = RCC_AHB1Periph_GPIOA,
	.CH1_PinSource 		 = GPIO_PinSource6,
	
	/*Tim3_Ch2(PWMOUT_3)*/	
	.CH2_GPIO 			 = GPIOA,
	.CH2_Pin 			 = GPIO_Pin_7,
	.RCC_Periph_CH2_GPIO = RCC_AHB1Periph_GPIOA,
	.CH2_PinSource 		 = GPIO_PinSource7,

	
	/*Tim3_Ch3(PWMOUT_2)*/	
	.CH3_GPIO 			 = GPIOB,
	.CH3_Pin 			 = GPIO_Pin_0,
	.RCC_Periph_CH3_GPIO = RCC_AHB1Periph_GPIOB,
	.CH3_PinSource 		 = GPIO_PinSource0,

	/*Tim3_Ch4(PWMOUT_1)*/	
	.CH4_GPIO 			 = GPIOB,
	.CH4_Pin 			 = GPIO_Pin_1,
	.RCC_Periph_CH4_GPIO = RCC_AHB1Periph_GPIOB,
	.CH4_PinSource 		 = GPIO_PinSource1,		

	/*pin Map*/	
	.GPIO_AF 			 = GPIO_AF_TIM3,		
};

/*MCU_TIM��ʼ��*/
void sys_mcu_tim_init(void)
{
	SYS_RCC_TREE gpioRccTree;
	SYS_RCC_TREE periphRccTree;
	
/*=== TIM_Alarm ===*/
	/*1.������ȶ�ʱ��_Main(Alarm)*/
	/*�趨TIMʱ����*/
	periphRccTree = SYS_RCC_APB1;	
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimAlarmTaskMain.RCC_Periph_Tim, ENABLE);	
	
	/*Tim Init*/
	msp_TimAlarm_Init(&g_sTimAlarmTaskMain);
	
	/*2.������ȶ�ʱ��_Main(Alarm)*/
	/*�趨TIMʱ����*/
	periphRccTree = SYS_RCC_APB1;	
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimAlarmTaskAssist.RCC_Periph_Tim, ENABLE);	
	
	/*Tim Init*/
	msp_TimAlarm_Init(&g_sTimAlarmTaskAssist);	
	
	/*3.ִ�м�����㶨ʱ����ʼ��(10000us,10ms)*/
	/*�趨TIMʱ����*/
	periphRccTree = SYS_RCC_APB2;	
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimExecutePeriod.RCC_Periph_Tim, ENABLE);	
	
	/*Tim Init*/	
	msp_TimAlarm_Init(&g_sTimExecutePeriod);

/*=== TIM_PWM_IN ===*/
	#if defined(REMOTE_DATA_RECV__PWM)
	/*1.PWM_Inͨ���е�Attitude(TIM4)����*/
	gpioRccTree   = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	periphRccTree = SYS_RCC_APB1;	/*�趨Timʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Attitude.RCC_Periph_CH1_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Attitude.RCC_Periph_CH2_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Attitude.RCC_Periph_CH3_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Attitude.RCC_Periph_CH4_GPIO, ENABLE);		
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimPwmIn_Attitude.RCC_Periph_Tim, ENABLE);	
	
	/*TimPwmIn Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_PWM_IN, &g_sTimPwmIn_Attitude);
	
	/*2.PWM_Inͨ���е�Switch(TIM1)����*/
	gpioRccTree   = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	periphRccTree = SYS_RCC_APB2;	/*�趨Timʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Switch.RCC_Periph_CH1_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Switch.RCC_Periph_CH2_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Switch.RCC_Periph_CH3_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmIn_Switch.RCC_Periph_CH4_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimPwmIn_Switch.RCC_Periph_Tim, ENABLE);
	
	/*TimPwmIn Init*/	
	msp_Peripheral_GPIO_Init(SYS_GPIO_PWM_IN, &g_sTimPwmIn_Switch);
	#endif
	
/*=== TIM_PWM_OUT ===*/	
	/*1.PWM_Outͨ���е�PWMOUT1~PWMOUT4����*/
	gpioRccTree   = SYS_RCC_AHB1;	/*�趨GPIOʱ����*/
	periphRccTree = SYS_RCC_APB1;	/*�趨Timʱ����*/
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmOut_Motor.RCC_Periph_CH1_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmOut_Motor.RCC_Periph_CH2_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmOut_Motor.RCC_Periph_CH3_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(gpioRccTree, g_sTimPwmOut_Motor.RCC_Periph_CH4_GPIO, ENABLE);
	sys_Peripheral_RCC_Init(periphRccTree, g_sTimPwmOut_Motor.RCC_Periph_Tim, ENABLE);	
	
	/*TimPwmOut Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_PWM_OUT, &g_sTimPwmOut_Motor);
}

/*=== EXTI Init ===*/
#if defined(REMOTE_DATA_RECV__PPM)
MSP_Exti g_sPPMRecvExti = 
{
	/*GPIO*/
	.Gpio.GPIO            = GPIOE,
	.Gpio.GPIO_Pin        = GPIO_Pin_12,
	
	/*rcc*/
	.Gpio.RCC_Periph_GPIO = RCC_AHB1Periph_GPIOE,
	.Periph_SYSCFG        = RCC_APB2Periph_SYSCFG,
	
	/*NVIC*/
	.NVIC_IRQChannel      = EXTI15_10_IRQn,
	
	/*EXTI*/
	.EXTI_PortSource      = EXTI_PortSourceGPIOE,
	.EXTI_PinSource       = EXTI_PinSource12,
	.EXTI_Line            = EXTI_Line12,
	.EXTI_Mode            = EXTI_Mode_Interrupt,
	.EXTI_Trigger         = EXTI_Trigger_Rising,
};

MSP_Exti *g_psPPMRecvExti = &g_sPPMRecvExti;
#endif

/*MCU_EXTI��ʼ��*/
void sys_mcu_exti_init(void)
{
	#if defined(REMOTE_DATA_RECV__PPM)	
	SYS_RCC_TREE gpioRccTree;		/*GPIOʱ����*/
	SYS_RCC_TREE periphRccTree;		/*�趨Timʱ����*/ 

	gpioRccTree   = SYS_RCC_AHB1;
	periphRccTree = SYS_RCC_APB2;
	
	/*RCC Init*/
	sys_Peripheral_RCC_Init(gpioRccTree, g_sPPMRecvExti.Gpio.RCC_Periph_GPIO, ENABLE);	
	sys_Peripheral_RCC_Init(periphRccTree, g_sPPMRecvExti.Periph_SYSCFG, ENABLE);	
	
	/*EXTI Init*/
	msp_Peripheral_GPIO_Init(SYS_GPIO_EXTI, &g_sPPMRecvExti);
	#endif
}

/*����ʱ�ӳ�ʼ��*/
void sys_Peripheral_RCC_Init(SYS_RCC_TREE rccTree, u32 rccPeriph, FunctionalState NewState)
{
	switch(rccTree)
	{
		case SYS_RCC_AHB1:
		{
			RCC_AHB1PeriphClockCmd(rccPeriph, NewState);
		}break;
		
		case SYS_RCC_AHB2:
		{
			RCC_AHB2PeriphClockCmd(rccPeriph, NewState);
		}break;		
		
		case SYS_RCC_AHB3:
		{
			RCC_AHB3PeriphClockCmd(rccPeriph, NewState);
		}break;

		case SYS_RCC_APB1:
		{
			RCC_APB1PeriphClockCmd(rccPeriph, NewState);
		}break;

		case SYS_RCC_APB2:
		{
			RCC_APB2PeriphClockCmd(rccPeriph, NewState);
		}break;

		default:break;
	}
}

/*����&GPIO��ʼ��*/
void msp_Peripheral_GPIO_Init(SYS_GPIO_PURPOSE gpioPurpose, void *gpioPeriphStruct)
{
	switch(gpioPurpose)
	{
		case SYS_GPIO_OUT:
		{
			
		}break;
		
		case SYS_GPIO_INP_D:
		{

		}break;		
		
		case SYS_GPIO_INP_U:
		{

		}break;

		case SYS_GPIO_EXTI:
		{
			MSP_Exti *pExti = gpioPeriphStruct;
			msp_EXTI_Init(pExti);
		}break;

		case SYS_GPIO_PWM_IN:
		{
			TimPwmIn *pTimPwmIn = gpioPeriphStruct;
			msp_TimPwmIn_Init(pTimPwmIn);
		}break;

		case SYS_GPIO_PWM_OUT:
		{
			TimPwmOut *pTimPwmOut = gpioPeriphStruct;
			msp_TimPwmOut_Init(pTimPwmOut);
		}break;		
		
		case SYS_GPIO_UART:
		{
			MSP_Uart *pUart = gpioPeriphStruct;
			msp_Uart_Init(pUart);
		}break;
		
		case SYS_GPIO_I2C:
		{
			MSP_I2c *pI2C = gpioPeriphStruct;		
			msp_I2c_Init(pI2C);			
		}break;

		case SYS_GPIO_SIMI2C:
		{
			SSP_SimI2C *pSimI2C = gpioPeriphStruct;
			ssp_SimI2C_Init(pSimI2C);
		}break;

		case SYS_GPIO_SIMSPI:
		{
			SSP_SimSPI *pSimSPI = gpioPeriphStruct;
			ssp_SimSPI_Init(pSimSPI);
		}break;		

		default:break;		
	}
}
