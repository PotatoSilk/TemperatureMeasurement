#include "msp430x44x.h"
#include <string.h>
#include <stdio.h>


#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int
#define S8 signed char
#define S16 signed short
#define S32 signed int


#define DQ_IN (P4DIR&=0XF7)
#define DQ_OUT (P4DIR|=BIT3)
#define DQ_1 (P4OUT|=BIT3)
#define DQ_0 (P4OUT&=~BIT3)
#define DQ (P4IN&BIT3)


#define    SHDN_ON            P3OUT|=BIT0
#define    SHDN_OFF          P3OUT&=~BIT0
#define    POW_ON            P2OUT&=~BIT6
#define    POW_OFF            P2OUT|=BIT6


extern char Rec_Buf[100], Buf_Max;
extern char Host_Num[20];
extern U8 GSM_Sta;
extern U8 TIM_BUFFER[17];
extern U16 T_H, T_M, T_S, T_mS;
extern U8 AD_Time;


extern U16 Tem;
extern int WEN;
extern U8 LV_POW, LV_CQS, str_wendu, str_sheding, str_bianhao, str_fa, str_zaixian;
extern U8 x;
extern U8 flag_send, flag_c;


void tcp_send(void);

void M590_Intial(void);

void Sent_Message(void);

void GET_TandRH(void);

void RTC_TIMadd(void);


void device_init(void);

void get_local_rtc(void);

void set_localtime(U16 Y, U16 MO, U16 D, U16 H, U16 MI, U16 S);

void
LCD_display(int Tem, U8 YEAR, U8 MON, U8 DAY, U8 H, U8 MIN, U8 SEC, U8 LV_POW, U8 LV_CQS, U8 str_wendu, U8 str_sheding,
            U8 str_bianhao, U8 str_fa, U8 str_zaixian, U8 str_shijian);

int get_temp(void);

void UTC_LOCAL(void);

void get_M590_TIM(void);


typedef struct
{
    U8 year;
    U8 mon;
    U8 day;
    U8 H;
    U8 M;
    U8 S;
} rtc;
extern rtc RTC_TIM;

typedef struct
{
    U8 year;
    U8 mon;
    U8 day;
    U8 H;
    U8 M;
    U8 S;
} UTC;
extern UTC UTC_LOCALTIM;






