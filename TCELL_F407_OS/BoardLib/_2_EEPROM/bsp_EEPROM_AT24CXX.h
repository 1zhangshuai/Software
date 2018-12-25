#ifndef _BSP_EEPROM_AT24CXX_H_
#define _BSP_EEPROM_AT24CXX_H_

#include "bsp_EEPROM_AT24CXX_CMD.h"
#include "sys_Platform.h"
#include "sys_McuInit.h"
#include "ssp_SimI2C.h"
#include "msp_I2C.h"

/*AT24CXX����*/
typedef enum
{
    BSP_FAM_AT24C01	 = 1,
    BSP_FAM_AT24C02  = 2,
    BSP_FAM_AT24C04  = 3,
    BSP_FAM_AT24C08  = 4,
    BSP_FAM_AT24C16  = 5, 
    BSP_FAM_AT24C32  = 6,
    BSP_FAM_AT24C64  = 7,
    BSP_FAM_AT24C128 = 8, 
    BSP_FAM_AT24C256 = 9,
}BSP_AT24CXX_TARG;

#ifdef STD_PROTOCOL_SOFTWARE_I2C /*ģ��I2C*/
typedef struct
{
	SSP_SimI2C       *SimI2cMaster; 
	BSP_AT24CXX_TARG At24CxxTarg;
	u32              MaxSize;
}BSP_AT24CXX;
#endif

#ifdef STD_PROTOCOL_HARDWARE_I2C /*Ӳ��I2C*/
typedef struct
{
	MSP_I2c       	 *I2cMaster; 
	BSP_AT24CXX_TARG At24CxxTarg;
	u32              MaxSize;
}BSP_AT24CXX;
#endif

/*=== AT24CXX������������ ===*/
SYS_RETSTATUS bsp_AT24CXX_Init(BSP_AT24CXX *AT24CXX);
u8 bsp_AT24CXX_Check(BSP_AT24CXX *AT24CXX);
void bsp_AT24CXX_WriteOneByte(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u8 Data);
u8 bsp_AT24CXX_ReadOneByte(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u8* pData);
void bsp_AT24CXX_WriteSomeByte(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u8 *pBuff, u16 WriteNbr);
u8* bsp_AT24CXX_ReadSomeByte(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u8 *pBuff, u16 ReadNbr);
void bsp_AT24CXX_WriteSizeByte(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u32 Data, u8 Size);
u32 bsp_AT24CXX_ReadSizeByte(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u32* pData, u8 Size);

/*=== AT24CXX��/д�� ===*/
/*д�з���32bit������*/
void bsp_AT24CXX_Write_1_S32Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, s32 s32Data);
/*���з���32bit������*/
SYS_RETSTATUS bsp_AT24CXX_Read_1_S32Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, s32 *s32Data);

/*д�޷���32bit������*/
void bsp_AT24CXX_Write_1_U32Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u32 u32Data);
/*���޷���32bit������*/
SYS_RETSTATUS bsp_AT24CXX_Read_1_U32Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u32 *u32Data);

/*д�з���16bit������*/
void bsp_AT24CXX_Write_1_S16Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, s16 s16Data);
/*���з���16bit������*/
SYS_RETSTATUS bsp_AT24CXX_Read_1_S16Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, s16 *s16Data);

/*д�޷���16bit������*/
void bsp_AT24CXX_Write_1_U16Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u16 u16Data);
/*���޷���16bit������*/
SYS_RETSTATUS bsp_AT24CXX_Read_1_U16Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u16 *u16Data);

/*д������*/
void bsp_AT24CXX_Write_1_FloatData(BSP_AT24CXX *AT24CXX, u16 WriteAddr, fp32 fpData);
/*��������*/
SYS_RETSTATUS bsp_AT24CXX_Read_1_FloatData(BSP_AT24CXX *AT24CXX, u16 ReadAddr, fp32 *fpData);

/*=== AT24CXX���ܲ������� ===*/
/*��AT24CXXд��3���з���32bit����*/
void bsp_AT24CXX_Write_3_S32Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, s32 s32Data1, s32 s32Data2, s32 s32Data3);
/*��AT24CXX����3���з���32bit����*/
SYS_RETSTATUS bsp_AT24CXX_Read_3_S32Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, s32 *s32Data1, s32 *s32Data2, s32 *s32Data3);

/*��AT24CXXд��3���޷���32bit����*/
void bsp_AT24CXX_Write_3_U32Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u32 u32Data1, u32 u32Data2, u32 u32Data3);
/*��AT24CXX����3���޷���32bit����*/
SYS_RETSTATUS bsp_AT24CXX_Read_3_U32Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u32 *u32Data1, u32 *u32Data2, u32 *u32Data3);

/*��AT24CXXд��3���з���16bit����*/
void bsp_AT24CXX_Write_3_S16Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, s16 s16Data1, s16 s16Data2, s16 s16Data3);
/*��AT24CXX����3���з���16bit����*/
SYS_RETSTATUS bsp_AT24CXX_Read_3_S16Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, s16 *s16Data1, s16 *s16Data2, s16 *s16Data3);

/*��AT24CXXд��3���޷���16bit����*/
void bsp_AT24CXX_Write_3_U16Data(BSP_AT24CXX *AT24CXX, u16 WriteAddr, u16 u32Data1, u16 u32Data2, u16 u32Data3);
/*��AT24CXX����3���޷���16bit����*/
SYS_RETSTATUS bsp_AT24CXX_Read_3_U16Data(BSP_AT24CXX *AT24CXX, u16 ReadAddr, u16 *u16Data1, u16 *u16Data2, u16 *u16Data3);

/*��AT24CXXд��3��������*/
void bsp_AT24CXX_Write_3_FloatData(BSP_AT24CXX *AT24CXX, u16 WriteAddr, fp32 fpData1, fp32 fpData2, fp32 fpData3);
/*��AT24CXX����3��������*/
SYS_RETSTATUS bsp_AT24CXX_Read_3_FloatData(BSP_AT24CXX *AT24CXX, u16 ReadAddr, fp32 *fpData1, fp32 *fpData2, fp32 *fpData3);


extern BSP_AT24CXX g_sAt24cxx;

#endif
