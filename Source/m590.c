#include "global_val.h"
#include "local_val.h"

const U8 tab1[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

U8 GSM_Sta;
U8 AD_Time;
UTC UTC_LOCALTIM;
rtc RTC_TIM;

void Delay_mS(U16 number)
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

U8 Hand(char *a)
{
    if (strstr(Rec_Buf, a) != NULL)
    { return 1; }
    else
    { return 0; }
}

void CLR_Buf(void)
{
    U8 k;
    for (k = 0; k < 100; k++)
    {
        Rec_Buf[k] = 0;
    }

    Rec_Buf[0] = 0;
    Buf_Max = 0;
}


void Send_ASCII(char *ch)
{

    for (; *ch != '\0'; ch++)
    {

        TXBUF0 = *ch;
        while (!(IFG1 & UTXIFG0))
        {}


    }
}


void Send_Hex(char c)
{

    TXBUF0 = c;
    while (!(IFG1 & UTXIFG0))
    {}


}


//====================================================================================================================


void Handle_Message()
{

    char *Str1;
    char Sent_Str[3], i, j, k;
    i = 0;
    CLR_Buf();
    while (!Hand("OK") && i < 5)
    {
        Send_ASCII("AT+CMGF=1");
        Send_Hex(0x0d);
        Delay_mS(200);
        i++;
    }
    CLR_Buf();

    i = 0;
    while (!Hand("OK") && i < 5)
    {
        Send_ASCII("AT+CSCS=");
        Send_Hex(0x22);
        Send_ASCII("GSM");
        Send_Hex(0x22);
        Send_Hex(0x0d);
        Delay_mS(200);
        i++;
    }
    CLR_Buf();
    i = 0;
    while (!Hand("OK") &&
           i < 6)
    {
        Send_ASCII("AT+CPMS?");
        Send_Hex(0x0d);
        Delay_mS(500);
        i++;
    }

    Str1 = strstr(Rec_Buf, "SM");                    //"GSM",1,60,"SM",1,60,"SM",1,60
    if (Str1 != NULL)
    {
        i = *(Str1 + 4);
        k = *(Str1 + 5);

        if (i > 47 && i < 58)
        {
            if (i == 48)
            {
                j = 0;            //Ascii To Interg
            }
            else
            {
                if (k == 0x2c)
                { j = i - 48; }
                else
                {
                    j = k - 48;
                    j += (i - 48) * 10;
                }
            }

            if (j > 60)
            { j = 60; }

            CLR_Buf();

            for (k = 0; k < j; k++)
            {
                i = k + 1;
                sprintf(Sent_Str, "%d", (U16) i);
                i = 0;
                while (!Hand("OK") &&
                       i < 5)
                {
                    Send_ASCII("AT+CMGR=");
                    Send_ASCII(Sent_Str);
                    Send_Hex(0x0d);
                    Delay_mS(1000);
                    i++;
                }

                if (Hand("Host Number"))
                {
                    Str1 = strstr(Rec_Buf, "Number");
                    for (i = 0; i < 11; i++)
                    { Host_Num[i] = *(Str1 + 7 + i); }
                    Host_Num[11] = 0;
                }


                else
                {
                    CLR_Buf();
                    i = 0;
                    while (!Hand("OK") && i < 5)
                    {
                        Send_ASCII("AT+CMGD=");
                        Send_ASCII(Sent_Str);
                        Send_Hex(0x0d);
                        Delay_mS(200);
                        i++;
                    }
                }
                CLR_Buf();
            }
        }
    }
    CLR_Buf();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void get_M590_TIM()
{
    U8 TEMP_BUF[25];
    U8 i = 0;
    U8 j = 0;

    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+CGDCONT=1,\"IP\",\"CMNET\"");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;

    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+XISP=0");
//		Send_Hex(0x0A); 
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;
    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+XIIC=0");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;

    while ((!Hand("ECV")) && i < 5)
    {
        Send_ASCII("AT+TCPCLOSE=0");
        Send_Hex(0x0d);
        Delay_mS(200);
        CLR_Buf();
        Send_ASCII("AT+TCPSETUP=0,24.56.178.140,13");
        Send_Hex(0x0d);
        CLR_Buf();
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    i = 0;
    for (i = 0; i < 23; i++)
    {
        TEMP_BUF[i] = *(Rec_Buf + 73 + i);
    }


    if (TEMP_BUF[0] == '-')
    {}
    else if (TEMP_BUF[1] == '-')
    {}
    else if (TEMP_BUF[2] == '-')
    { i = 0; }
    else if (TEMP_BUF[3] == '-')
    { i = 1; }
    else if (TEMP_BUF[4] == '-')
    { i = 2; }
    else if (TEMP_BUF[5] == '-')
    { i = 3; }
    else if (TEMP_BUF[6] == '-')
    { i = 4; }
    else if (TEMP_BUF[7] == '-')
    { i = 5; }
    else if (TEMP_BUF[8] == '-')
    { i = 6; }
    else if (TEMP_BUF[9] == '-')
    { i = 7; }
    else if (TEMP_BUF[10] == '-')
    { i = 8; }
    else if (TEMP_BUF[11] == '-')
    { i = 9; }


    for (j = 0; j < 17; j++)
    {
        TIM_BUFFER[j] = TEMP_BUF[i];
        i++;
    }
    //	Send_ASCII(TIM_BUFFER);
    CLR_Buf();
    i = 0;


    Send_ASCII("AT+TCPCLOSE=0");
    //	Send_Hex(0x0A);
    Send_Hex(0x0D);
    Delay_mS(200);
    Delay_mS(500);
    Delay_mS(500);
    i++;

    CLR_Buf();
    i = 0;

}

void UTC_LOCAL()
{
    U16 year;
    U8 mon;
    U8 day;
    U8 H, M, S;


    U8 flag = 0;
    year = (TIM_BUFFER[0] - 48) * 10 + (TIM_BUFFER[1] - 48);
    mon = (TIM_BUFFER[3] - 48) * 10 + (TIM_BUFFER[4] - 48);
    day = (TIM_BUFFER[6] - 48) * 10 + (TIM_BUFFER[7] - 48);
    H = (TIM_BUFFER[9] - 48) * 10 + (TIM_BUFFER[10] - 48);
    M = (TIM_BUFFER[12] - 48) * 10 + (TIM_BUFFER[13] - 48);
    S = (TIM_BUFFER[15] - 48) * 10 + (TIM_BUFFER[16] - 48);


    if (year % 100 == 0)
    {
        if (year % 400 == 0)
        {
            flag = 1;
        }
        else if (year % 4 == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    }

    H += 8;
    if (H > 23)
    {
        H -= 24;
        day++;
    }
    if (!flag)
    {
        if (day > tab1[mon])
        {
            day = 1;
            mon++;
            if (mon > 12)
            {
                year++;
                mon = 1;
            }
        }
    }
    else
    {
        if (mon == 2)
        {
            if (day > (tab1[mon] + 1))
            {
                day = 1;
                mon++;
            }
        }
        else
        {
            if (day > tab1[mon])
            {
                day = 1;
                mon++;
                if (mon > 12)
                {
                    year++;
                    mon = 1;
                }
            }

        }
    }

    UTC_LOCALTIM.year = year;
    UTC_LOCALTIM.mon = mon;
    UTC_LOCALTIM.day = day;
    UTC_LOCALTIM.H = H;
    UTC_LOCALTIM.M = M;
    UTC_LOCALTIM.S = S;
}


//====================================================================================================================================================
void M590_Intial()
{
//	U32 tem;
    U8 i, j, k;
    char *Str1;
    GSM_Sta = 0;                            //Intial GSM status




    SHDN_ON;                            //Turn on M590 Power
    Delay_mS(500);
    Delay_mS(500);
    Delay_mS(500);
    CLR_Buf();


    POW_ON;
    Delay_mS(500);                                                 // Power on M590
    Delay_mS(500);
    Delay_mS(500);
    Delay_mS(500);
    Delay_mS(310);
    POW_OFF;                            // Power on M590

    Delay_mS(900);
    Delay_mS(900);
    Delay_mS(900);
//        
    j = 0;


    i = 0;


    i = 0;


    while (!Hand("+PBREADY") && i < 50)
    {
        Delay_mS(200);

        i++;
    }
    CLR_Buf();                                            //Clear Buffer;



    i = 0;
    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+CPIN?");
        Send_Hex(0x0d);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    if (i > 4)
    { GSM_Sta = 1; }
    CLR_Buf();

    k = 0;
    while (!GSM_Sta && k < 5)
    {
        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("AT+CSQ");
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);//?óê±
            i++;
        }
        if (i > 10)
        { GSM_Sta = 1; }
        else
        {
            Str1 = strstr(Rec_Buf, "CSQ:");
            if (Str1 != NULL)
            {
                i = *(Str1 + 6);
                if (i == 0x2c)
                { GSM_Sta = 1; }
                else
                {
                    if (i == '9')
                    {
                        j = *(Str1 + 5);
                        if (j == '9')
                        {
                            k++;
                            if (k > 4)
                            {
                                GSM_Sta = 1;
                            }
                            else
                            { Delay_mS(200); }
                        }
                        else
                        { k = 5; }
                    }
                    else
                    { k = 5; }
                }

            }

        }
        CLR_Buf();
    }

    k = 0;
    while (!GSM_Sta && k < 5)
    {
        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("AT+CREG?");
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);
            Delay_mS(500);
            i++;
        }
        if (i > 4)
        { GSM_Sta = 1; }
        else
        {
            Str1 = strstr(Rec_Buf, "CREG:");
            if (Str1 != NULL)
            {
                i = *(Str1 + 8);
                if (i == '3')
                {
                    Delay_mS(500);
                    Delay_mS(500);
                    Delay_mS(500);
                    Delay_mS(500);
                    k++;
                }
                else if (i == '0' || i == '2' || i == '4')
                { GSM_Sta = 1; }
                else if (i == '1' || i == '5')
                { k = 5; }
            }

        }
        CLR_Buf();
    }


    if (!GSM_Sta)
    {
        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("ATE1");
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);
            Delay_mS(500);
            i++;
        }
        CLR_Buf();


        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("AT+CMGF=1");
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);
            Delay_mS(500);
            i++;
        }
        CLR_Buf();

        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("AT+CSCS=");
            Send_Hex(0x22);
            Send_ASCII("GSM");
            Send_Hex(0x22);
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);
            Delay_mS(500);
            i++;
        }
        CLR_Buf();

        i = 0;
        while (!Hand("OK") && i < 5)
        {
            Send_ASCII("AT+CNMI=2,1,0,0,0");
            Send_Hex(0x0d);
            Delay_mS(200);
            Delay_mS(500);
            Delay_mS(500);
            Delay_mS(500);
            i++;
        }
        CLR_Buf();

        Handle_Message();                                   //Handle Message
    }

    i = 0;
    CLR_Buf();

//	  get_M590_TIM();                    
//	  UTC_LOCAL();	                     
//	  set_localtime(0,UTC_LOCALTIM.year,UTC_LOCALTIM.mon,UTC_LOCALTIM.day,UTC_LOCALTIM.H,UTC_LOCALTIM.M,UTC_LOCALTIM.S);	                 
//	

//	while(!Hand("OK") && i<5) 						
//	{
//		Send_ASCII("AT+CPWROFF"); 					
//		Send_Hex(0x0d); 							
//		Delay_mS(1000); 							
//		i++;
//	}
//	CLR_Buf();
//
//	Delay_mS(800); 								
//	SHDN_OFF;							//Turn off M590 Power



}

//短信方式发送
//====================================================================================================================================
void Sent_Message()
{ //U32 tem ;

    U8 i;

    char Str1[10] = {0};

    sprintf(Str1, "%d", WEN);


    i = 0;
    while (!Hand(">") && i < 5)
    {
        Send_ASCII("AT+CMGS=");
        Send_Hex(0x22);
        Send_ASCII("17626021249");
        Send_Hex(0x22);
        Send_Hex(0x0d);
        Delay_mS(300);
        i++;
    }
    CLR_Buf();
    Delay_mS(500);
    Delay_mS(500);
    Delay_mS(500);
    Delay_mS(500);

    i = 0;
    while (!Hand("OK") && i < 5)
    {
        Send_ASCII(Str1);

        Delay_mS(500);
        Send_Hex(0x1A);
        Delay_mS(500);

        i++;
    }
    CLR_Buf();


}


U8 BCD_HEX(U8 TEM)
{
    U8 BCD, Buffer;
    BCD = TEM % 10;
    TEM /= 10;
    Buffer = TEM % 10;
    Buffer <<= 4;
    BCD = (BCD | Buffer);
    return BCD;

}

void tcp_send(void)
{
    U8 i = 0, j = 0;
    U16 TEM_BUF = 0;
    //    U16 X=0;
    U8 Str1[23] = {0};
    TEM_BUF = WEN;
    CLR_Buf();
    i = 0;


    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+CGDCONT=1,\"IP\",\"CMNET\"");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;

    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+XISP=0");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }


    CLR_Buf();
    i = 0;


    while ((!Hand("OK")) && i < 5)
    {
        Send_ASCII("AT+XIIC=0");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;

    Send_ASCII("AT+XIIC?");
    //	Send_Hex(0x0A);
    Send_Hex(0x0D);
    Delay_mS(200);
    Delay_mS(500);
    Delay_mS(500);
    CLR_Buf();
    i = 0;

    while ((!Hand("UP:0,O")) && i < 4)
    {
        Send_ASCII("AT+TCPSETUP=0,221.178.237.151,20000");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);
        Delay_mS(500);


        i++;
    }
    CLR_Buf();
    i = 0;
    //at+xiic?
    while ((!Hand(">")) && i < 5)
    {
        Send_ASCII("AT+TCPSEND=0,22");
        //	Send_Hex(0x0A);
        Send_Hex(0x0D);
        Delay_mS(200);
        Delay_mS(500);
        Delay_mS(500);
        i++;
    }
    CLR_Buf();
    i = 0;
    Str1[0] = 0X68;
    Str1[1] = 0X00;
    Str1[2] = 0X00;
    Str1[3] = 0X00;
    Str1[4] = 0X00;
    Str1[5] = 0X00;
    Str1[6] = 0X01;
    Str1[7] = 0X68;
    Str1[8] = 0X05;
    Str1[9] = 0X0a;
    Str1[10] = 0Xc3;
    Str1[11] = 0Xc3;

    Str1[12] = (BCD_HEX(TEM_BUF % 100) + 0X33) & 0XFF;
    TEM_BUF /= 100;
    Str1[13] = (BCD_HEX(TEM_BUF % 100) + 0X33) & 0XFF;
    Str1[14] = 0x33;

    Str1[15] = (BCD_HEX(RTC_TIM.M % 100) + 0X33) & 0XFF;
    Str1[16] = (BCD_HEX(RTC_TIM.H % 100) + 0X33) & 0XFF;
    Str1[17] = (BCD_HEX(RTC_TIM.day % 100) + 0X33) & 0XFF;
    Str1[18] = (BCD_HEX(RTC_TIM.mon % 100) + 0X33) & 0XFF;
    Str1[19] = (BCD_HEX(RTC_TIM.year % 100) + 0X33) & 0XFF;


    Str1[20] = 0;
    for (j = 0; j < 20; j++)
    {
        Str1[20] += Str1[j];
    }
    Str1[21] = 0x16;


    while (!Hand("OK") && i < 5)
    {
        for (j = 0; j < 22; j++)
        {
            Send_Hex(Str1[j]);
        }

        Delay_mS(500);
        Send_Hex(0x1A);
        Delay_mS(500);

        i++;
    }

    CLR_Buf();
    i = 0;


    Send_ASCII("AT+TCPCLOSE=0");
    //	Send_Hex(0x0A);
    Send_Hex(0x0D);
    Delay_mS(500);
    Delay_mS(500);
    CLR_Buf();
    i = 0;


}


void set_localtime(U16 Y, U16 MO, U16 D, U16 H, U16 MI, U16 S)
{
    RTC_TIM.year = Y;
    RTC_TIM.mon = MO;
    RTC_TIM.day = D;
    RTC_TIM.H = H;
    RTC_TIM.M = MI;
    RTC_TIM.S = S;
}



