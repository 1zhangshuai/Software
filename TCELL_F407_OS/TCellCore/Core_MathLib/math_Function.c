#include "math_Function.h"

MATH_Integer g_sMathIntegerAnaly;
MATH_Integer *g_psMathIntegerAnaly = &g_sMathIntegerAnaly;

MATH_Floater g_sMathFloaterAnaly;
MATH_Floater *g_psMathFloaterAnaly = &g_sMathFloaterAnaly;

/*�����ֵ*/
fp32 math_Abs(fp32 val)
{
	if (val < 0)
	{
		return -val;
	}
	else 
	{
		return val;
	}
}

/*���ٿ�ƽ��ȡ����0x5f3759df/ 0x5f375a86*/
fp32 math_InvSqrt(fp32 val)
{
    fp32 halfx = 0.5f * val;
    s32 i = *(int*)&val;
        
	i = 0x5f3759df - (i >> 1);        // �����һ�����Ƹ�
    val = *(fp32*)&i;
    val = val*(1.5f - halfx*val*val);       // ţ�ٵ�����
    
	return val;
} 

/*���ٷ�����*/
fp32 math_fast_atan(fp32 v)
{
    fp32 v2 = v * v;
    return (v * (1.6867629106f + v2 * 0.4378497304f) / (1.6867633134f + v2));
}

/*���η�*/
fp32 power_x_2(fp32 x)
{
	return (x*x);
}

/*���η�*/
fp32 power_x_3(fp32 x)
{
	return (x*x*x);
}

/*2D vector length(��ά������ģ��)*/
fp32 pythagorous2(fp32 a, fp32 b)
{
	return (sqrtf(power_x_2(a) + power_x_2(b)));
}

/*3D vector length(��ά������ģ��)*/
fp32 pythagorous3(fp32 a, fp32 b, fp32 c)
{
	return (sqrtf(power_x_2(a) + power_x_2(b) + power_x_2(c)));	
}

/*����һ�����ķ�Χ(�޷�)*/
fp32 math_Constrain(fp32 targValue, fp32 maxValue, fp32 minValue)
{
	if (targValue >= maxValue)		/*����ֵ�޷�*/
	{
		targValue = maxValue;
	}
	else if (targValue <= minValue)	/*��Сֵ�޷�*/
	{
		targValue = minValue;	
	}
	
	return targValue;
}

/*��һ��unsigned / signed�����з���*/
void math_Integer_Number_Analy(s32 number, u8 totalWidth, MATH_Integer *integer)
{
	u32 pow_10 = 1;
	
	/*�жϸ�������*/
	if (number >= 0)
	{
		integer->NUMBER_SIGN = MATH_NUMBER_SIGN_PLUS; /*+*/
	}
	else
	{
		integer->NUMBER_SIGN = MATH_NUMBER_SIGN_MINUS; /*-*/
	}
	
	/*��������ֵ*/
	integer->numberABS = math_Abs(number);
	
	/*������λ��*/
	integer->avaBitNumber = 0; /*����0*/
	
	while((integer->numberABS / pow_10) != 0)
	{
		integer->avaBitNumber++;
		
		pow_10 *= 10;
	}
	
	/*��������0ʱ,����λΪ1λ*/	
	if (integer->numberABS == 0)
	{
		integer->avaBitNumber = 1;
	}
	
	/*��ʾ������λ��*/
	integer->totalBitNumber = totalWidth;
}

/*��һ��fp32�����з���*/
void math_Floater_Number_Analy(fp32 db_number, u8 totalWidth, MATH_Floater *floater)
{
	u32  powInt_10 = 1;	
	u32  powFloat_10 = 1;
	u8   i;
	fp64 decimal = 0.0f;
	/*�жϸ�������*/
	if (db_number >= 0)
	{
		floater->NUMBER_SIGN = MATH_NUMBER_SIGN_PLUS; /*+*/
	}
	else
	{
		floater->NUMBER_SIGN = MATH_NUMBER_SIGN_MINUS; /*-*/		
	}
	
	/*������ֳ�С������+����*/
	/*С�����ֽ��и߾��ȱ���*/
	decimal = db_number - (s32)db_number;				  /* < 1*/

	/*�������ֱ���*/
	floater->integerPartABS = (u32)(math_Abs(db_number));
	
	/*�����������������λ��*/
	floater->integerPartBitNumber = 0; /*����0*/
	
	while((floater->integerPartABS / powInt_10) != 0)
	{
		floater->integerPartBitNumber++;
		
		powInt_10 *= 10;
	}
	
	/*С������������0ʱ,����λΪ1λ*/
	if (floater->integerPartABS == 0)
	{
		floater->integerPartBitNumber = 1;		
	}
	
	/*С�������������ָ�λ���λ�����*/
	floater->decimalPointPos = floater->integerPartBitNumber + 1;
	
	/*������λ��,��ʾ����,����������ʾ��Ϻ�,ʣ��λ���С������*/
	floater->decimalPartTotalBitNumber = totalWidth - floater->integerPartBitNumber - 2; /*һ����������ʾ��λ�� - ����λ�� - ��������('+','-','.')*/
	
	/*����ʣ��λ��,��ȷ��С�����־���*/
	for (i = floater->decimalPartTotalBitNumber; i > 0; i--)
	{
		powFloat_10 *= 10;
	}
	
	/*��С�����������Ӧ10^x����,������������*/
	floater->decimalABS = (u32)(math_Abs(decimal * powFloat_10));
	
	/*��λ*/
	powFloat_10 = 1;
	
	/*����С��������Чλ��*/
	floater->decimalPartAvaBitNumber = 0; /*����0*/
	
	while((floater->decimalABS / powFloat_10) != 0)
	{
		floater->decimalPartAvaBitNumber++;
		
		powFloat_10 *= 10;
	}

	/*С������,��0����ʼǰ0�ĸ���*/
    floater->decimalNotZeroFrontZeroNbr = floater->decimalPartTotalBitNumber - floater->decimalPartAvaBitNumber;
}
