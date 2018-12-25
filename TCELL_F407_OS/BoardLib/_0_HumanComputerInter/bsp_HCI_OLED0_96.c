#include "bsp_HCI_OLED0_96.h"
#include "hci_FontLib.h" /*�ֿ��ļ�*/

BSP_OLED0_96 g_sOled0_96;

/*OLED��ʼ��*/
#if defined(HW_CUT__USE_HCI_OLED)

SYS_RETSTATUS bsp_OLED0_96_Init(BSP_OLED0_96 *oled0_96)
{	
	SYS_RETSTATUS retStatus;
	
	/*SimSPI init*/
	oled0_96->SimSpiMaster = &g_sOledSimSPI;
	
	/*OLED ��ʼ��*/
	SIM_SPI_RST_SET(oled0_96->SimSpiMaster->RST_GPIO, oled0_96->SimSpiMaster->RST_Pin);
	sys_DelayMs(100);
	SIM_SPI_RST_RESET(oled0_96->SimSpiMaster->RST_GPIO, oled0_96->SimSpiMaster->RST_Pin);
	sys_DelayMs(100);
	SIM_SPI_RST_SET(oled0_96->SimSpiMaster->RST_GPIO, oled0_96->SimSpiMaster->RST_Pin); 
	
    bsp_OLED0_96_Write_Command(oled0_96, 0xAE);/*turn off oled panel*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x02);/*set low column address*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x10);/*set high column address*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x40);/*set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xFF);/*set contrast control register*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xFF);/*Set SEG Output Current Brightness*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xA1);/*Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xC8);/*Set COM/Row Scan Direction   0xc0���·��� 0xc8����*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xA6);/*set normal display*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xA8);/*set multiplex ratio(1 to 64)*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x3f);/*1/64 duty*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xD3);/*set display offset	Shift Mapping RAM Counter (0x00~0x3F)*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x00);/*not offset*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xd5);/*set display clock divide ratio/oscillator frequency*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x80);/*set divide ratio, Set Clock as 100 Frames/Sec*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xD9);/*set pre-charge period*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xF1);/*Set Pre-Charge as 15 Clocks & Discharge as 1 Clock*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xDA);/*set com pins hardware configuration*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x12);
    bsp_OLED0_96_Write_Command(oled0_96, 0xDB);/*set vcomh*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x40);/*Set VCOM Deselect Level*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x20);/*Set Page Addressing Mode (0x00/0x01/0x02)*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x02);
    bsp_OLED0_96_Write_Command(oled0_96, 0x8D);/*set Charge Pump enable/disable*/
    bsp_OLED0_96_Write_Command(oled0_96, 0x14);/*set(0x10) disable*/
    bsp_OLED0_96_Write_Command(oled0_96, 0xA4);/*Disable Entire Display On (0xa4/0xa5)*/
	bsp_OLED0_96_Write_Command(oled0_96, 0xA6);/*Disable Inverse Display On (0xa6/a7)*/
	bsp_OLED0_96_Write_Command(oled0_96, 0xAF);/*turn on oled panel*/
	bsp_OLED0_96_Write_Command(oled0_96, 0xAF);/*display ON*/ 																
	
	bsp_OLED0_96_Clear(oled0_96);
    bsp_OLED0_96_Set_Pos(oled0_96, 0, 0);	
	
	retStatus = SYS_RET_SUCC;
	
	return retStatus;
}
#endif

/*д����*/	
void bsp_OLED0_96_Write_Command(BSP_OLED0_96 *oled0_96, u8 cmd)
{
	ssp_SimSPI_WriteByte(oled0_96->SimSpiMaster, cmd, OLED_CMD);
}

/*д����*/	
void bsp_OLED0_96_Write_Data(BSP_OLED0_96 *oled0_96, u8 dat)
{
	ssp_SimSPI_WriteByte(oled0_96->SimSpiMaster, dat, OLED_DATA);
}

/*OLED������ʾ*/   
void bsp_OLED0_96_Display_On(BSP_OLED0_96 *oled0_96)
{
	bsp_OLED0_96_Write_Command(oled0_96, 0X8D);  //SET DCDC����
	bsp_OLED0_96_Write_Command(oled0_96, 0X14);  //DCDC ON
	bsp_OLED0_96_Write_Command(oled0_96, 0XAF);  //DISPLAY ON
}

/*OLED�ر���ʾ*/
void bsp_OLED0_96_Display_Off(BSP_OLED0_96 *oled0_96)
{
	bsp_OLED0_96_Write_Command(oled0_96, 0X8D);  //SET DCDC����
	bsp_OLED0_96_Write_Command(oled0_96, 0X10);  //DCDC ON
	bsp_OLED0_96_Write_Command(oled0_96, 0XAE);  //DISPLAY ON
}

/*�ֲ�����*/
void bsp_OLED0_96_Display_Part(BSP_OLED0_96 *oled0_96, u8 x0, u8 y0, u8 x1, u8 y1, u8 dat)
{
	u8 i, n;
	
	for (i = y0; i < y1; i++)  
	{  
		bsp_OLED0_96_Write_Command(oled0_96, 0xb0+i);    //����ҳ��ַ��0~7��
		bsp_OLED0_96_Write_Command(oled0_96, 0x02);      //������ʾλ�èD�е͵�ַ
		bsp_OLED0_96_Write_Command(oled0_96, 0x10);      //������ʾλ�èD�иߵ�ַ   
		
		for ( n = x0; n < x1; n++ )
		{
			bsp_OLED0_96_Write_Data(oled0_96, dat); 
		}
		
	} //������ʾ
}

/*����*/   							   		    
void bsp_OLED0_96_Clear(BSP_OLED0_96 *oled0_96)
{
	bsp_OLED0_96_Display_Part(oled0_96, 0, 0, 128, 8, 0x00);
}

/*��ʾ�ַ�*/
void bsp_OLED0_96_ShowChar(BSP_OLED0_96 *oled0_96, u8 x, u8 y, u8 chr, OLED096_ACSII_FONT ACSII_FONT)
{
	u8 c = 0, i = 0;
	
	c = chr - ' ';  /*�õ�ƫ�ƺ��ֵ*/
	
	if (x > OLED0_96_MAX_COLUMN - 1)
	{
		x = 0;
		y = y + 2;
	}
	
	if (ACSII_FONT == OLED096_ACSII_8X16)
	{
		bsp_OLED0_96_Set_Pos(oled0_96, x, y);	
		
		for (i = 0; i < 8; i++)
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Ascii8X16[c*16 + i]);
		}
		
		bsp_OLED0_96_Set_Pos(oled0_96, x, y + 1);
		
		for (i = 0; i < 8; i++)
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Ascii8X16[c*16 + i + 8]);
		}
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		bsp_OLED0_96_Set_Pos(oled0_96, x, y);
		
		for (i = 0; i < 6; i++)
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Ascii6X12[c*12 + i]);
		}
		
		bsp_OLED0_96_Set_Pos(oled0_96, x, y + 1);
		
		for (i = 0; i < 6; i++)
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Ascii6X12[c*12 + i + 6]);
		}
	}
	else if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		bsp_OLED0_96_Set_Pos(oled0_96, x, y);
		
		for (i = 0; i < 6; i++)
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Ascii6X8[c*6 + i]);
		}
	}
}

//m^n����
uint32_t oled_pow(u8 m, u8 n)
{
	uint32_t result = 1;	
	
	while(n--)
	{
		result *= m;    
	}
	
	return result;
}

/*��ʾ����*/
void bsp_OLED0_96_ShowNum(BSP_OLED0_96 *oled0_96, u8 x, u8 y, uint32_t num, u8 len, OLED096_ACSII_FONT ACSII_FONT)
{	
	u8 t;
	u8 enshow = 0;	
	static u8 temp = 0;
	
	if (ACSII_FONT == OLED096_ACSII_8X16)
	{
		for( t = 0; t < len; t++ )
		{
			temp = (num / oled_pow(10, len - t - 1)) % 10;
			
			if( (enshow == 0) && (t < (len - 1)) )
			{
				if( temp == 0 )
				{
					bsp_OLED0_96_ShowChar(oled0_96, x + (ACSII_FONT / 2) * t, y, ' ', OLED096_ACSII_8X16);
					continue;
				}
				else 
				{
					enshow = 1; 
				} 
			}
			
			bsp_OLED0_96_ShowChar(oled0_96, x + (ACSII_FONT / 2) * t, y, temp + '0', OLED096_ACSII_8X16); 
		}	
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		for( t = 0; t < len; t++ )
		{	
			temp = (num / oled_pow(10, len - t - 1)) % 10;
			
			if( (enshow == 0) && (t < (len - 1)) )
			{
				if( temp == 0 )
				{
					bsp_OLED0_96_ShowChar(oled0_96, x + (ACSII_FONT / 2) * t, y, ' ', OLED096_ACSII_6X12);
					continue;
				}
				else 
				{
					enshow = 1; 
				} 
			}
			
			bsp_OLED0_96_ShowChar(oled0_96, x + (ACSII_FONT / 2) * t, y, temp + '0', OLED096_ACSII_6X12); 
		}	
	}
	else if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		for( t = 0; t < len; t++ )
		{		
			temp = (num / oled_pow(10, len - t - 1)) % 10;
			
			if( (enshow == 0) && (t < (len - 1)) )
			{
				if( temp == 0 )
				{
					bsp_OLED0_96_ShowChar(oled0_96, x + ((ACSII_FONT + 4) / 2) * t, y, ' ', OLED096_ACSII_6X8);
					continue;
				}
				else 
				{
					enshow = 1; 
				} 
			}
			
			bsp_OLED0_96_ShowChar(oled0_96, x + ((ACSII_FONT + 4) / 2) * t, y, temp + '0', OLED096_ACSII_6X8); 
		}	
	}
}

/*��ʾ�ַ���*/
void bsp_OLED0_96_ShowString(BSP_OLED0_96 *oled0_96, u8 x, u8 y, u8 *str, OLED096_ACSII_FONT ACSII_FONT)
{
	u8 j = 0;
	
	if (ACSII_FONT == OLED096_ACSII_8X16)
	{
		while (str[j] != '\0')
		{		
			bsp_OLED0_96_ShowChar(oled0_96, x, y, str[j], ACSII_FONT);
		
			x += 8;
		
			if( x > 120) /*û��ʾ���,��������*/
			{
				x = 0;
				y += 2;
			}
		
			j++;
		}
	}
	else if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		while ( str[j] != '\0' )
		{		
			bsp_OLED0_96_ShowChar(oled0_96, x, y, str[j], ACSII_FONT);
		
			x += 6;
		
			if( x > 122) /*û��ʾ���,��������*/
			{
				x = 0;
				y += 1;
			}
		
			j++;
		}
	}
}

/*��������*/
void bsp_OLED0_96_Set_Pos(BSP_OLED0_96 *oled0_96, u8 x, u8 y)
{
	bsp_OLED0_96_Write_Command(oled0_96, 0xb0+y);
	bsp_OLED0_96_Write_Command(oled0_96, (((x + 2) & 0xf0) >> 4) | 0x10);
	bsp_OLED0_96_Write_Command(oled0_96, (x + 2) & 0x0f); 
}

/*��ʾ����*/
void bsp_OLED0_96_ShowCHinese(BSP_OLED0_96 *oled0_96, u8 x, u8 y, u8 no,  OLED096_HANZI_FONT HANZI_FONT)
{
	u8 t, adder = 0;
	
	bsp_OLED0_96_Set_Pos(oled0_96, x, y);	
	
	if (HANZI_FONT == OLED096_HANZI_16X16)
	{
		for( t = 0; t < 16; t++ )
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Hanzi16X16[2*no][t]);
			adder += 1;
		}
		
		bsp_OLED0_96_Set_Pos(oled0_96, x, y + 1);	
		
		for( t = 0; t < 16; t++ )
		{	
			bsp_OLED0_96_Write_Data(oled0_96, g_Hanzi16X16[2*no+1][t]);
			adder += 1;
		}	
	}
	else if (HANZI_FONT == OLED096_HANZI_12X12)
	{
		for( t = 0; t < 12; t++ )
		{
			bsp_OLED0_96_Write_Data(oled0_96, g_Hanzi12X12[no*24 + t]);
			adder += 1;
		}
		
		bsp_OLED0_96_Set_Pos(oled0_96, x, y + 1);	
		
		for( t = 0; t < 12; t++ )
		{	
			bsp_OLED0_96_Write_Data(oled0_96, g_Hanzi12X12[no*24 + 12 + t]);
			adder += 1;
		}		
	}	
}

/*��ͼ*/
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void bsp_OLED0_96_DrawBMP(BSP_OLED0_96 *oled0_96, u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
{
	uint32_t j = 0;
	u8 x, y;
	
	if ((y1 % 8) == 0)
	{
		y = y1 / 8;
	}
	else
	{
		y = (y1 / 8) + 1;
	}
	
	for(y = y0; y < y1; y++)
	{
		bsp_OLED0_96_Set_Pos(oled0_96, x0, y);
		
		for(x = x0; x < x1; x++)
		{      
			bsp_OLED0_96_Write_Data(oled0_96, BMP[j++]);	    	
		}
	}	
}

/*=== ��չ��ʾ���� ===*/
/*��ʾ����*/
void bsp_OLED0_96_Show_Integer(BSP_OLED0_96 *oled0_96, u8 x, u8 y, OLED096_ACSII_FONT ACSII_FONT, MATH_Integer integer)
{	
	u8 xNbrStart = 0;
	
	/*1.��ʾ���Ų���*/
	if (integer.NUMBER_SIGN == MATH_NUMBER_SIGN_PLUS) /*'+' -> ' '*/
	{
		bsp_OLED0_96_ShowChar(oled0_96, x, y, ' ', ACSII_FONT);
	}
	else
	{
		bsp_OLED0_96_ShowChar(oled0_96, x, y, '-', ACSII_FONT);		
	}
	
	/*2.��ʾ���ݲ���*/
	/*����xNbrStart*/
	if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		xNbrStart = x + 6 + (integer.totalBitNumber - integer.avaBitNumber) * 6;
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		xNbrStart = x + 6 + (integer.totalBitNumber - integer.avaBitNumber) * 6;		
	}
	else
	{
		xNbrStart = x + 8 + (integer.totalBitNumber - integer.avaBitNumber) * 8;			
	}
	
	/*��ʾ����*/
	bsp_OLED0_96_ShowNum(oled0_96, xNbrStart, y, integer.numberABS, integer.avaBitNumber, ACSII_FONT);
}

/*��ʾ������*/
void bsp_OLED0_96_Show_Floater(BSP_OLED0_96 *oled0_96, u8 x, u8 y, OLED096_ACSII_FONT ACSII_FONT, MATH_Floater floater)
{
	u8 xIntStartPos = 0;
	u8 xPointPos = 0;
	u8 xFloaterPos = 0;	
	u8 i = 0;
	
	/*1.��ʾ���Ų���*/
	if (floater.NUMBER_SIGN == MATH_NUMBER_SIGN_PLUS) 
	{
		bsp_OLED0_96_ShowChar(oled0_96, x, y, ' ', ACSII_FONT); /*'+' -> ' '*/
	}
	else
	{
		bsp_OLED0_96_ShowChar(oled0_96, x, y, '-', ACSII_FONT);	 /*'-'*/	
	}
	
	/*2.��ʾ���ݲ���*/
	/*����xNbrStart*/
	if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		xIntStartPos = x + 6; /*��������λ*/
		xPointPos    = xIntStartPos + (6 * floater.integerPartBitNumber); /*����������λ*/
		xFloaterPos	 = xPointPos + 6; /*����С����λ*/

		/*����ʾС��λ��һ����������ǰ��'0'*/
		for (i = floater.decimalPartTotalBitNumber - floater.decimalPartAvaBitNumber; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xFloaterPos, y, '0', ACSII_FONT);	 /*'-'*/	
			
			xFloaterPos += 6;
		}		
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		xIntStartPos = x + 6;		 /*��������λ*/
		xPointPos    = xIntStartPos + (6 * floater.integerPartBitNumber); /*����������λ*/
		xFloaterPos	 = xPointPos + 6; /*����С����λ*/

		/*����ʾС��λ��һ����������ǰ��'0'*/
		for (i = floater.decimalPartTotalBitNumber - floater.decimalPartAvaBitNumber; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xFloaterPos, y, '0', ACSII_FONT);	 /*'-'*/	
			
			xFloaterPos += 6;
		}			
	}
	else
	{
		xIntStartPos = x + 8;		 /*��������λ*/
		xPointPos    = xIntStartPos + (8 * floater.integerPartBitNumber); /*����������λ*/
		xFloaterPos	 = xPointPos + 8; /*����С����λ*/

		/*����ʾС��λ��һ����������ǰ��'0'*/
		for (i = floater.decimalPartTotalBitNumber - floater.decimalPartAvaBitNumber; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xFloaterPos, y, '0', ACSII_FONT);	 /*'-'*/	
			
			xFloaterPos += 6;
		}			
	}
	
	/*��ʾ��������*/
	bsp_OLED0_96_ShowNum(oled0_96, xIntStartPos, y, floater.integerPartABS, floater.integerPartBitNumber, ACSII_FONT);
	
	/*��ʾС����*/
	bsp_OLED0_96_ShowChar(oled0_96, xPointPos, y, '.', ACSII_FONT);
	
	/*��ʾС����Ч����*/
	bsp_OLED0_96_ShowNum(oled0_96, xFloaterPos, y, floater.decimalABS, floater.decimalPartAvaBitNumber, ACSII_FONT);
}

/*��ʾ����*/
void bsp_OLED0_96_Show_Calendar(BSP_OLED0_96 *oled0_96, u8 x, u8 y, OLED096_ACSII_FONT ACSII_FONT, MATH_Integer integer)
{
	u8 xNbrStart = x;
	u8 repairZeroNbr = 0;
	u8 i;
	
	/*������Ҫ������0*/
	repairZeroNbr = integer.totalBitNumber - integer.avaBitNumber;
	
	if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		for (i = repairZeroNbr; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xNbrStart, y, '0', ACSII_FONT);
			
			xNbrStart += 6;
		}
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		for (i = repairZeroNbr; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xNbrStart, y, '0', ACSII_FONT);
			
			xNbrStart += 6;
		}	
	}
	else
	{
		for (i = repairZeroNbr; i > 0; i--)
		{
			bsp_OLED0_96_ShowChar(oled0_96, xNbrStart, y, '0', ACSII_FONT);
			
			xNbrStart += 8;
		}					
	}
	
	/*��ʾ����*/
	bsp_OLED0_96_ShowNum(oled0_96, xNbrStart, y, integer.numberABS, integer.avaBitNumber, ACSII_FONT);
}

/*��ʾ����(û�з���λ)*/
void bsp_OLED0_96_Show_Integer_No_Sign(BSP_OLED0_96 *oled0_96, u8 x, u8 y, OLED096_ACSII_FONT ACSII_FONT, MATH_Integer integer)
{
	u8 xNbrStart = 0;
	
	/*2.��ʾ���ݲ���*/
	/*����xNbrStart*/
	if (ACSII_FONT == OLED096_ACSII_6X8)
	{
		xNbrStart = x + (integer.totalBitNumber - integer.avaBitNumber) * 6;
	}
	else if (ACSII_FONT == OLED096_ACSII_6X12)
	{
		xNbrStart = x + (integer.totalBitNumber - integer.avaBitNumber) * 6;		
	}
	else
	{
		xNbrStart = x + (integer.totalBitNumber - integer.avaBitNumber) * 8;			
	}
	
	/*��ʾ����*/
	bsp_OLED0_96_ShowNum(oled0_96, xNbrStart, y, integer.numberABS, integer.avaBitNumber, ACSII_FONT);	
}
