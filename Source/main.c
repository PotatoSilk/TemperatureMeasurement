#include "global_val.h"

int WEN = 0;
U8 x = 0;

int main(void)
{

    WDTCTL = WDTPW + WDTHOLD;
    device_init();
    WEN = get_temp();

    asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop");
    WEN = get_temp();
    asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop");
    WEN = get_temp();
    asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop");
    LCD_display(WEN, RTC_TIM.year, RTC_TIM.mon, RTC_TIM.day, RTC_TIM.H, RTC_TIM.M, RTC_TIM.S, 2, 4, 1, 1, 1, 1, 1, x);

    M590_Intial();
    get_M590_TIM();
    UTC_LOCAL();
    set_localtime(UTC_LOCALTIM.year, UTC_LOCALTIM.mon, UTC_LOCALTIM.day, UTC_LOCALTIM.H, UTC_LOCALTIM.M,
                  UTC_LOCALTIM.S);


    while (1)
    {
        if (flag_c)
        {
            flag_c = 0;
            tcp_send();
        }
    }
}
