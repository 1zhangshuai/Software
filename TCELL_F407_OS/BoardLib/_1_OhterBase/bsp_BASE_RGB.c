#include "bsp_BASE_RGB.h"

BSP_RGB g_sRgb = {0};

/*��ʼ��*/
SYS_RETSTATUS bsp_RGB_Init(BSP_RGB *rgb)
{
	SYS_RETSTATUS statusRet = SYS_RET_SUCC;
	
	/*rgb gpio init*/
	#if defined(RGB_LEVEL_TYPE) /*��ƽ��RGB*/
	rgb->TricolorGpio = &g_sLevelRgbGpio;
	#endif
	
	#if defined(RGB_CLOCK_TYPE)	/*ʱ����RGB*/
	rgb->Gpio = (Rgb_Gpio*)&g_sClockRgbGpio;
	#endif	
	
	RGB_DELAY_MS(1);
	
	return statusRet;	
}

/*����������*/
void bsp_rgb_work_list_set(BSP_RGB *rgb, const Rgb_Work *userList, RGB_LST_SHOW_TYPE LIST_SHOW_TYPE)
{
	u32 i;
	
	/*�������ʾ��*/
	bsp_rgb_work_list_clear(rgb);
	
	/*������ʾ��������ʾ����*/
	for (i = 0; i < RGB_WORK_LIST_MAX_NBR; i++)
	{
		if (userList[i].COLOR_TYPE == RGB_COLOR_NULL)
		{
			break;
		}
		
		rgb->WorkList[i].COLOR_TYPE = userList[i].COLOR_TYPE; 									 /*��ɫ����*/
		rgb->WorkList[i].SHOW_TYPE  = userList[i].SHOW_TYPE; 								     /*��ʽ����*/
		rgb->WorkList[i].conTick5MS = userList[i].conTick5MS * RGB_SHOW_CONTINUE_TICK_FOC_5MS;    /*����ʱ������*/
	}
	
	/*��ʾ����ʾ��ʽ*/
	rgb->LIST_SHOW_TYPE = LIST_SHOW_TYPE;
}

/*���������*/
void bsp_rgb_work_list_clear(BSP_RGB *rgb)
{
	u32 i;
	
	/*������ʾ��������ʾ����*/
	for (i = 0; i < RGB_WORK_LIST_MAX_NBR; i++)
	{
		rgb->WorkList[i].COLOR_TYPE = RGB_COLOR_NULL; 	 /*��ɫ����*/ 
		rgb->WorkList[i].SHOW_TYPE  = RGB_SIG_SHOW_NULL; /*��ʽ����*/		
		rgb->WorkList[i].conTick5MS = 0;    			 /*����ʱ������*/		
	}	
	
	/*�����ʾ������*/
	rgb->LIST_SHOW_TYPE = RGB_LST_SHOW_NULL;	
}

/*��������ʾ*/
void bsp_rgb_work_list_show(BSP_RGB *rgb)
{
	u32 i;
	
	/*ɨ����ʾ��*/
	for (i = 0; i < RGB_WORK_LIST_MAX_NBR; i++)
	{
		/*�ж�Ϊnull,��ʾ�������񵽴˽���*/
		if (rgb->WorkList[i].COLOR_TYPE == RGB_COLOR_NULL)
		{
			break;
		}		
		
		/*��ʾ*/
		bsp_rgb_show_color(rgb, rgb->WorkList[i].COLOR_TYPE, rgb->WorkList[i].SHOW_TYPE);
		
		/*��ʱ*/
		RGB_DELAY_MS(rgb->WorkList[i].conTick5MS);
	}
	
	/*��ʾ����ʾ��ʽΪ����,��ֱ�������ʾ��*/
	if (rgb->LIST_SHOW_TYPE == RGB_LST_SHOW_ONCE)
	{
		bsp_rgb_work_list_clear(rgb);
	}
}

/*��ɫ��ʾ*/
void bsp_rgb_show_color(BSP_RGB *rgb, RGB_COLOR_TYPE COLOR_TYPE, RGB_SIG_SHOW_TYPE SHOW_TYPE)
{	
	/*��ƽ��RGB*/
	#ifdef RGB_LEVEL_TYPE 
	static MSP_General_Gpio OpGpio;
	
	/*�ж���ʾ��ɫ*/
	switch(COLOR_TYPE)
	{
		case RGB_COLOR_RED:    /*��*/
		{
			OpGpio.GPIO     = rgb->TricolorGpio->Red.GPIO;
			OpGpio.GPIO_Pin = rgb->TricolorGpio->Red.GPIO_Pin;			
		}break;
		
		case RGB_COLOR_GREEN:  /*��*/
		{
			OpGpio.GPIO     = rgb->TricolorGpio->Green.GPIO;
			OpGpio.GPIO_Pin = rgb->TricolorGpio->Green.GPIO_Pin;			
		}break;

		case RGB_COLOR_BLUE:   /*��*/
		{
			OpGpio.GPIO     = rgb->TricolorGpio->Blue.GPIO;
			OpGpio.GPIO_Pin = rgb->TricolorGpio->Blue.GPIO_Pin;		
		}break;

		case RGB_COLOR_CYAN:   /*��*/
		{
		
		}break;

		case RGB_COLOR_PINK:   /*��*/
		{
		
		}break;

		case RGB_COLOR_YELLOW: /*��*/
		{
		
		}break;

		case RGB_COLOR_WHITE:  /*��*/
		{
		
		}break;		
		
		default:break;
	}
	
	/*�ж���ʾ��ʽ*/
	switch(SHOW_TYPE)
	{
		case RGB_SIG_SHOW_DARK:   /*��*/
		{
			SYS_GPIO_RESET(OpGpio.GPIO, OpGpio.GPIO_Pin);
		}break;

		case RGB_SIG_SHOW_LIGHT:  /*��*/
		{
			SYS_GPIO_SET(OpGpio.GPIO, OpGpio.GPIO_Pin);		
		}break;

		case RGB_SIG_SHOW_TOGGLE: /*��*/
		{
		
		}break;	

		default:break;		
	}	
	#endif

	/*ʱ����RGB*/
	#ifdef RGB_CLOCK_TYPE 

	#endif	
}
