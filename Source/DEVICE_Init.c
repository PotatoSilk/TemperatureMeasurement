#include "global_val.h"

void Delay_m(U16 number)
{
    U16 a, b, c;
    b = 444;
    {
        for (c = 0; c < number; c++)
        {
            for (a = 0; a < 440; a++)
            { b *= b; }
        }
    }
}


void gpio_init()
{
    P1DIR |= 0X03;      //P1.0 1.1 OUT
    P4DIR |= BIT3;      //DQ
    P3DIR |= BIT0;
    P2DIR |= BIT6;
    P1DIR = 0x32;                             // Set P1.1,4,5 as outputs
    P1SEL = 0x32;                             // Select P1.1,4,5 as clocks
}


void uart_init()
{
    FLL_CTL0 |= XCAP18PF;                     // Configure load caps
    P2SEL |= 0x30;                            // P2.4,5 = USART0 TXD/RXD
    ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL1;                          // UCLK = SMCLK

    UBR00 = 0x9A;
    UBR10 = 0x02;
    UMCTL0 = 0x00;


    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;                            // Enable USART0 RX interrupt
    _EINT();
    TXBUF0 = 0x00;
    while (!(IFG1 & UTXIFG0))
    {}

}

void os_init()
{

//  FLL_CTL0 |= FLL_DIV1+DCOPLUS;
//  FLL_CTL0 |= XCAP18PF;                     // Set load capacitance for xtal
//  SCFI0 |= FN_4+FLLD_2;                            
//  SCFQCTL = 106;                             // (60+1) x 32768 = 2.02Mhz 
//  FLL_CTL1 |= SELM_DCO; 
//  Delay_m(20);

    FLL_CTL0 |= FLL_DIV1 + DCOPLUS;
    FLL_CTL0 |= XCAP18PF;                     // Set load capacitance for xtal
    SCFI0 |= FN_8;
    SCFQCTL = 60;                             // (60+1) x 32768 = 2.02Mhz
    FLL_CTL1 |= SELM_DCO;
}


void LCD_init(void)
{
    volatile unsigned int i;                  // Use volatile to prevent removal
    // by compiler optimization


    FLL_CTL0 |= XCAP18PF;                     // Configure load caps
    for (i = 0; i < 10000; i++)
    {}              // Delay for 32 kHz crystal to
    for (i = 0; i < 10000; i++);               // stabilize

    LCDCTL = LCDON + LCD4MUX + LCDSG0_2;      // LCD on, 4-Mux, segments S0-S19
    BTCTL = BT_fLCD_DIV32;                  // LCD clock freq is ACLK/32
    P5SEL = 0xFC;                             // Select P5.2-7 as Com and Rxx
//  LCDAVCTL0 = ;
//  LCDAVCTL1 = ;
}

void tim_init()
{
    // CCR0 中断使能
    CCR0 = 32767;
    TACTL = ID0 + MC_2 + ID_3;
    CCTL0 = CCIE;

}

void RTC_INIT()
{
//  RTCCTL = 0x3e;       //32位日历模式
//  set_localtime(0,0,0,0,0,0);
}

void val_init()
{
    Buf_Max = 0;
    RTC_TIM.year = 17;
    RTC_TIM.mon = 8;
    RTC_TIM.day = 2;
    RTC_TIM.H = 12;
    RTC_TIM.M = 20;
    RTC_TIM.S = 11;

    SHDN_OFF;
    sprintf(Host_Num, "%s", "17626021249");
}

void device_init()
{
    os_init();
    gpio_init();
    uart_init();
    LCD_init();
    tim_init();
    RTC_INIT();
    val_init();
}
