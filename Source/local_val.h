

#include "msp430x44x.h"


char Rec_Buf[100], Buf_Max;
char Host_Num[20];


U8 TIM_BUFFER[17];

U16 T_H, T_M, T_S, T_mS;


void Init_Device(void);


void M590_Intial(void);

void Sent_Message(void);


void RTC_TIMadd(void);               //RTCê±??×???oˉêy

void UTC_LOCAL(void);

void get_M590_TIM(void);

void GET_TandRH(void);








