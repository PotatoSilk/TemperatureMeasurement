#include "global_val.h"

U8 TEMP = 0;
U8 j = 0;
U8 flag_send = 0, flag_c = 0;
const U8 tab[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
秒自增函数   调用一次秒数加一
*/
void RTC_TIMadd()
{
    char flag = 0;
    if (RTC_TIM.year % 100 == 0)
    {
        if (RTC_TIM.year % 400 == 0)
        {
            flag = 1;
        }
        else if (RTC_TIM.year % 4 == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    }


    RTC_TIM.S++;
    if (RTC_TIM.S >= 60)
    {
        RTC_TIM.S = 0;
        RTC_TIM.M++;
        if (RTC_TIM.M >= 60)
        {
            RTC_TIM.H++;
            RTC_TIM.M = 0;
            if (RTC_TIM.H > 23)
            {
                RTC_TIM.H = 0;
                RTC_TIM.day++;


                if (!flag)
                {
                    if (RTC_TIM.day > tab[RTC_TIM.mon])
                    {
                        RTC_TIM.day = 1;
                        RTC_TIM.mon++;
                        if (RTC_TIM.mon > 12)
                        {
                            RTC_TIM.year++;
                            RTC_TIM.mon = 1;
                        }
                    }
                }
                else
                {
                    if (RTC_TIM.mon == 2)
                    {
                        if (RTC_TIM.day > (tab[RTC_TIM.mon] + 1))
                        {
                            RTC_TIM.day = 1;
                            RTC_TIM.mon++;
                        }
                    }
                    else
                    {
                        if (RTC_TIM.day > tab[RTC_TIM.mon])
                        {
                            RTC_TIM.day = 1;
                            RTC_TIM.mon++;
                            if (RTC_TIM.mon > 12)
                            {
                                RTC_TIM.year++;
                                RTC_TIM.mon = 1;
                            }
                        }

                    }
                }
            }
        }


    }


}


#pragma vector=USART0RX_VECTOR

__interrupt void usart0_rx(void)
{


    TEMP = RXBUF0;
    {
        Rec_Buf[Buf_Max] = TEMP;
        Buf_Max++;
        if (Buf_Max > 99)
        { Buf_Max = 99; }

    }


    IFG1 &= 0x00;
}


#pragma vector=TIMERA0_VECTOR

__interrupt void Timer_A(void)
{
    CCR0 += 32767;                            // 下次中断时间装入CCR0中
    RTC_TIMadd();

    WEN = get_temp();

    j++;
    flag_send++;
    if (j == 10)
    {
        if (x == 1)
        { x = 0; }
        else
        { x = 1; }

        j = 0;

    }
    if (flag_send >= 40)
    {
        flag_send = 0;
        flag_c++;
    }
    LCD_display(WEN, RTC_TIM.year, RTC_TIM.mon, RTC_TIM.day, RTC_TIM.H, RTC_TIM.M, RTC_TIM.S, 2, 4, 1, 0, 0, 1, 1, x);
}
