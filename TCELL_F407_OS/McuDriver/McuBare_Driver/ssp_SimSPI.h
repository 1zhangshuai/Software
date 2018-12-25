#ifndef _SSP_SIMSPI_H_
#define _SSP_SIMSPI_H_

#include "sys_Platform.h"
#include "msp_GPIO.h"

#define NO_CMD				0
#define NO_DAT				0

#define OLED_CMD  			0	//д����
#define OLED_DATA 			1	//д���� 	

#define SIM_SPI_CLK_SET(GPIO, PIN)		GPIO_SetBits(GPIO, PIN)
#define SIM_SPI_CLK_RESET(GPIO, PIN)	GPIO_ResetBits(GPIO, PIN)

#define SIM_SPI_SDIN_SET(GPIO, PIN)		GPIO_SetBits(GPIO, PIN)
#define SIM_SPI_SDIN_RESET(GPIO, PIN)	GPIO_ResetBits(GPIO, PIN)

#define SIM_SPI_RST_SET(GPIO, PIN)		GPIO_SetBits(GPIO, PIN)
#define SIM_SPI_RST_RESET(GPIO, PIN)	GPIO_ResetBits(GPIO, PIN)

#define SIM_SPI_DC_SET(GPIO, PIN)		GPIO_SetBits(GPIO, PIN)
#define SIM_SPI_DC_RESET(GPIO, PIN)		GPIO_ResetBits(GPIO, PIN)

#define SIM_SPI_CS_SET(GPIO, PIN)		GPIO_SetBits(GPIO, PIN)
#define SIM_SPI_CS_RESET(GPIO, PIN)		GPIO_ResetBits(GPIO, PIN)

typedef struct
{
	/*����,����:D1*/
	GPIO_TypeDef *MOSI_GPIO;
	u16 		 MOSI_Pin;
	u32 		 RCC_Periph_MOSI_GPIO;
	
	/*����,�ӳ�*/	
	GPIO_TypeDef *MISO_GPIO;
	u16 		 MISO_Pin;	
	u32 		 RCC_Periph_MISO_GPIO;	

	/*ʱ��:D0*/
	GPIO_TypeDef *CLK_GPIO;
	u16 		 CLK_Pin;	
	u32 		 RCC_Periph_CLK_GPIO;		

	/*Ƭѡ:CS*/
	GPIO_TypeDef *CS_GPIO;
	u16 		 CS_Pin;	
	u32 		 RCC_Periph_CS_GPIO;	
	
	/*��λ:RES*/
	GPIO_TypeDef *RST_GPIO;
	u16 		 RST_Pin;	
	u32 		 RCC_Periph_RST_GPIO;		

	/*DC:DC*/
	GPIO_TypeDef *DC_GPIO;
	u16 		 DC_Pin;	
	u32 		 RCC_Periph_DC_GPIO;	
}SSP_SimSPI;

void ssp_SimSPI_Init(SSP_SimSPI *SimSPI);
void ssp_SimSPI_WriteByte(SSP_SimSPI *SimSPI, u8 data, uint8_t command);

#endif
