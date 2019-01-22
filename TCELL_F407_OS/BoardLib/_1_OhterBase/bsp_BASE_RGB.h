#ifndef _BSP_BASE_RGB_H_
#define _BSP_BASE_RGB_H_

#include "sys_Platform.h"
#include "sys_BoardMap.h"
#include "sys_McuInit.h"
#include "msp_MutGPIO.h"

#define RGB_SHOW_CONTINUE_TICK_FOC_5MS	(5)  /*rgb��ʾ����ʱ��5msʱ��*/
#define RGB_WORK_LIST_MAX_NBR			(10) /*RGB��������󳤶�(һ������һ����ʾ����)*/
#define RGB_DELAY_MS(ms)				sys_DelayMs(ms)			

/*��ʾ��ɫ*/
typedef enum
{
	RGB_COLOR_NULL   = 0, /*��*/
	RGB_COLOR_RED    = 1, /*��*/
	RGB_COLOR_GREEN  = 2, /*��*/
	RGB_COLOR_BLUE   = 3, /*��*/
	RGB_COLOR_CYAN   = 4, /*��*/
	RGB_COLOR_PINK   = 5, /*��*/
	RGB_COLOR_YELLOW = 6, /*��*/
	RGB_COLOR_WHITE  = 7, /*��*/
}RGB_COLOR_TYPE;

/*������ʾ��ʽ*/
typedef enum
{
	RGB_SIG_SHOW_NULL   = 0, /*��*/
	RGB_SIG_SHOW_DARK   = 1, /*��*/
	RGB_SIG_SHOW_LIGHT  = 2, /*��*/
	RGB_SIG_SHOW_TOGGLE = 3, /*��*/
}RGB_SIG_SHOW_TYPE;

/*��������ʾ��ʽ*/
typedef enum
{
	RGB_LST_SHOW_NULL   = 0, /*��*/
	RGB_LST_SHOW_ONCE   = 1, /*һ��*/
	RGB_LST_SHOW_CIRCLE = 2, /*ѭ��*/
	RGB_LST_SHOW_RANDOM = 3, /*���*/	
}RGB_LST_SHOW_TYPE;

typedef struct
{
	/*������ʾ*/
	RGB_COLOR_TYPE    COLOR_TYPE; 	  /*��ɫ*/
	RGB_SIG_SHOW_TYPE SHOW_TYPE;  /*��ʽ*/
	u32			      conTick5MS;     /*����ʱ��_5ms*/
}Rgb_Work;

/*��ƽ��RGB*/
#ifdef RGB_LEVEL_TYPE 
typedef struct
{
	MSP_TricolorGpio *TricolorGpio;
	Rgb_Work 		  WorkList[RGB_WORK_LIST_MAX_NBR];   /*RGB������*/
	Rgb_Work 		  UserList[RGB_WORK_LIST_MAX_NBR];   /*�û������ռ�*/
	RGB_LST_SHOW_TYPE LIST_SHOW_TYPE;			/*��������ʾ��ʽ*/
}BSP_RGB;
#endif

/*ʱ����RGB*/
#ifdef RGB_CLOCK_TYPE 
typedef struct
{
	
}BSP_RGB;
#endif

/*��ʼ��*/
SYS_RETSTATUS bsp_RGB_Init(BSP_RGB *rgb);

/*����������*/
void bsp_rgb_work_list_set(BSP_RGB *rgb, const Rgb_Work *userList, RGB_LST_SHOW_TYPE LIST_SHOW_TYPE);

/*���������*/
void bsp_rgb_work_list_clear(BSP_RGB *rgb);

/*��������ʾ*/
void bsp_rgb_work_list_show(BSP_RGB *rgb);

/*��ɫ��ʾ*/
void bsp_rgb_show_color(BSP_RGB *rgb, RGB_COLOR_TYPE COLOR_TYPE, RGB_SIG_SHOW_TYPE SHOW_TYPE);

extern BSP_RGB g_sRgb;

#endif
