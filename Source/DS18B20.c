#include "global_val.h"


void delay(U16 y)
{


    while (y--) asm("nop");
}

//DS18B20初始化子程序，成功返回1，否则返回0
U8 DS18B20_init(void)
{
    U8 tem = 0;
    DQ_OUT;
    DQ_1;
    delay(50);
    DQ_0;
    delay(330);
    DQ_1;
    DQ_IN;                    //8
    delay(100);
    tem = DQ;
    delay(220);
    if (!tem)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//从DS18B20读一位数据
U8 tempreadbit(void)
{
    int dat = 0;
    DQ_OUT;
    DQ_0;
    delay(3);            //延时6us
    DQ_1;
    delay(4);            //延时9us
    DQ_IN;
    asm("nop");
    if (DQ)
    { dat = 1; }
    delay(22);           //延时45us
    DQ_OUT;
    DQ_1;
    delay(5);           //延时10us


    return dat;

}


//从DS18B20读一字节
U8 tempreadbyte(void)
{
    int i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++)
    {
        j = tempreadbit();
        asm("nop");
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}


//从DS18B20写一位数据
void tempwritebit(U8 instruc_data)
{
    DQ_OUT;
    DQ_1;
    if (instruc_data)
    {
        DQ_0;      //拉低控制线
        delay(8);
        DQ_1;      //拉高控制线
        delay(30);
    }
    else
    {
        DQ_OUT;
        DQ_0;      //拉低控制线
        delay(38);
    }
    DQ_1;      //拉高控制线
    asm("nop");


}


//向DS18B20写一字节数据
void tempwritebyte(U8 instru)
{
    int i;
    for (i = 1; i <= 8; i++)   //将1字节拆分8位，逐位写给DS18B20
    {
        tempwritebit(instru & 0X01);
        instru = instru >> 1;
    }
}


//读取寄存器中存储的温度值
int get_temp()
{
    U16 temp;
    U8 temp_L, temp_H, str;
    str = 0;
    float f_temp;
    DS18B20_init();
    DQ_OUT;
    DQ_1;
    tempwritebyte(0Xcc);        //写跳过ROM指令
    tempwritebyte(0X44);
    DS18B20_init();
    tempwritebyte(0Xcc);
    tempwritebyte(0Xbe);        //写入读暂存器指令
    temp_L = tempreadbyte();     //读温度低八度
    temp_H = tempreadbyte();     //读温度高八度
    temp = temp_H << 8 | temp_L;     //获取温度数据



    if ((temp & 0xf000))
    {
        temp = (~temp) + 1;         //小数求补码
        str = 1;
    }
    f_temp = temp * 0.0625;         //12位温度数据，分辨率为0.0625
    temp = (int) (f_temp * 100);
    if (str)
    {
        temp = -temp;
    }
    return temp;               //返回温度值
}
