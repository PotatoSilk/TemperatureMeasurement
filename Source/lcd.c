#include "global_val.h"
#include "stdlib.h"
#include "math.h"

U8 LV_POW, LV_CQS, str_wendu, str_sheding, str_bianhao, str_fa, str_zaixian;

const unsigned char code_num_float_1[10] = {0x5f, 0x06, 0x3d, 0x2f, 0x66, 0x6B, 0x7B, 0x0E, 0x7f, 0x6F};
const unsigned char code_num_float_2[10] = {0xf5, 0x60, 0xb6, 0xf2, 0x63, 0xd3, 0xd7, 0x70, 0xf7, 0xf3};

//电量等级范围  0--2    信号等级范围0-4

//参数说明 温度 年 月 日 时 分 秒    电量等级 信号强度 "温度" “设定” “编号” "阀开/关" “在线” "时间"
void
LCD_display(int Tem, U8 YEAR, U8 MON, U8 DAY, U8 H, U8 MIN, U8 SEC, U8 LV_POW, U8 LV_CQS, U8 str_wendu, U8 str_sheding,
            U8 str_bianhao, U8 str_fa, U8 str_zaixian, U8 str_shijian)
/*|---------------------------汉字显示状态--------------------------------------------|*/
{
    U8 str_char1 = 0;
    U8 str = 0;
    int ch = 0;
    Tem /= 10;
    str_char1 |= 0x08;
    if (LV_CQS > 0)
    { str_char1 |= 0x80; }
    if (LV_CQS > 1)
    { str_char1 |= 0x04; }
    if (LV_CQS > 2)
    { str_char1 |= 0x02; }
    if (LV_CQS > 3)
    { str_char1 |= 0x01; }
    str_char1 &= 0xbf;

    if (str_zaixian == 1)
    { str_char1 |= 0x10; }

    if (Tem < 0)
    { str = 1; }
    else
    { str = 0; }
    ch = abs(Tem);


    //===============================================================================================


////=================================================================================年-时   
    if (str_bianhao == 1)
    {
        if (str_shijian == 1)
        {
            LCDM1 = code_num_float_2[(YEAR % 100) / 10] | 0x08; //年
        }
        else
        {
            LCDM1 = code_num_float_2[(H % 100) / 10] | 0x08;
        }   //时
    }

    else
    {
        if (str_shijian == 1)
        {
            LCDM1 = code_num_float_2[YEAR % 100 / 10]; //年
        }
        else
        {
            LCDM1 = code_num_float_2[H % 100 / 10];
        }     //时
    }


    if (str_sheding == 1)
    {
        if (str_shijian == 1)
        {
            LCDM2 = code_num_float_2[YEAR % 10] | 0x08;
        }
        else
        {
            LCDM2 = code_num_float_2[H % 10] | 0x08;
        }
    }

    else
    {
        if (str_shijian == 1)
        {
            LCDM2 = code_num_float_2[YEAR % 10];
        }
        else
        {
            LCDM2 = code_num_float_2[H % 10];
        }
    }


    //=============================================================================月-分

    if (str_wendu == 1)
    {
        if (str_shijian == 1)
        {
            LCDM3 = code_num_float_2[MON % 100 / 10] | 0x08; //月
        }
        else
        {
            LCDM3 = code_num_float_2[MIN % 100 / 10] | 0x08;
        } //分
    }

    else
    {
        if (str_shijian == 1)
        {
            LCDM3 = code_num_float_2[MON % 100 / 10];
        }
        else
        {
            LCDM3 = code_num_float_2[MIN % 100 / 10];
        }
    }


    if (str_shijian == 1)
    {
        LCDM4 = code_num_float_2[MON % 10];
    }
    else
    { LCDM4 = code_num_float_2[MIN % 10] | 0x08; }
    //=============================================================================日-秒


    if (str_shijian == 0)
    {
        LCDM5 = code_num_float_2[SEC % 100 / 10]; //miao
    }
    else
    { LCDM5 = code_num_float_2[DAY % 100 / 10] | 0x08; }


    if (LV_POW > 1)
    {
        if (str_shijian == 1)
        {
            LCDM6 = code_num_float_2[DAY % 10] | 0x08;
        }
        else
        {
            LCDM6 = code_num_float_2[SEC % 10] | 0x08;
        }
    }

    else
    {
        if (str_shijian == 1)
        {
            LCDM6 = code_num_float_2[DAY % 10];
        }
        else
        {
            LCDM6 = code_num_float_2[SEC % 10];
        }
    }


    LCDM7 = str_char1;


    LCDM8 = code_num_float_1[ch % 10] | 0X80;                          //温度


    if (LV_POW > 0)
    {
        LCDM9 = code_num_float_1[(ch % 100) / 10] | 0x80;
    }
    else
    {
        LCDM9 = code_num_float_1[(ch % 100) / 10];
    }

    if (!str)
    {
        LCDM10 = code_num_float_1[(ch % 1000) / 100];
    }
    else
    {
        LCDM10 = code_num_float_1[(ch % 1000) / 100] | 0x80;
    }
}
