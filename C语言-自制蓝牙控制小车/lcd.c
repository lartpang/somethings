
/*************  功能说明    **************

ST7920的基本驱动程序

******************************************/
#include 	<STC15W4K58S4.h>
#include    "intrins.h"
#include 	"delay.h"
#include    "lcd.h"

#define 	LCD_Data P0
#define 	Busy    0x80 //用于检测LCD状态字中的Busy标识

sfr TH2  = 0xD6;
sfr TL2  = 0xD7;

sbit    P_HC595_SER   = P4^0;   //pin 14    SER     data input
sbit    P_HC595_RCLK  = P5^4;   //pin 12    RCLk    store (latch) clock
sbit    P_HC595_SRCLK = P4^3;   //pin 11    SRCLK   Shift data clock

/**************************************/
sbit    LCD_RS 		= 	P1^3;  //定义引脚
sbit    LCD_RW 		=	P1^4;
sbit    LCD_E  		= 	P1^5;
sbit    PSB    		= 	P1^6;      //PSB脚为12864-12系列的串、并通讯功能切换，我们使用8位并行接口，PSB=1
sbit    LCD_RESET   =   P1^7;   //  17---RESET  L-->Enable
#define LCD_Data 		P0
/*************************************/


#define Busy    0x80 //用于检测LCD状态字中的Busy标识

//========================================================================
// 函数: void  Delay_Nms(u8 ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================

void DisableHC595(void)
{       
    u8  i;
    P_HC595_SER   = 1;
    for(i=0; i<20; i++)
    {
        P_HC595_SRCLK = 1;
        P_HC595_SRCLK = 0;
    }
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;                           //锁存输出数据
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;                           //锁存输出数据
}

//******************************************
void    LCD_delay(void)
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}


//写数据
void WriteDataLCD(u8 WDLCD)
{
    ReadStatusLCD(); //检测忙 
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_Data = WDLCD;
        LCD_delay();
    LCD_E = 1;
    LCD_delay();
    LCD_E = 0;
}

//写指令
void WriteCommandLCD(u8 WCLCD,BuysC) //BuysC为0时忽略忙检测
{
    if (BuysC) ReadStatusLCD(); //根据需要检测忙 
    LCD_RS = 0;
    LCD_RW = 0; 
    LCD_Data = WCLCD;
        LCD_delay();
    LCD_E = 1; 
    LCD_delay();
    LCD_E = 0;  
}

//读状态
u8 ReadStatusLCD(void)
{
    LCD_Data = 0xFF; 
    LCD_RS = 0;
    LCD_RW = 1; 
        LCD_delay();
    LCD_E = 1;
        LCD_delay();
    while (LCD_Data & Busy); //检测忙信号
    LCD_E = 0;
    return(LCD_Data);
}

void LCDInit(void) //LCM初始化
{
    PSB = 1;    //并口
//  PSB = 0;    //SPI口
    Delay_Nms(10);
    LCD_RESET = 0;
    Delay_Nms(10);
    LCD_RESET = 1;
    Delay_Nms(100);
    
    WriteCommandLCD(0x30,1); //显示模式设置,开始要求每次检测忙信号
    WriteCommandLCD(0x01,1); //显示清屏
    WriteCommandLCD(0x06,1); // 显示光标移动设置
    WriteCommandLCD(0x0C,1); // 显示开及光标设置
}

void LCDClear(void) //清屏
{
    WriteCommandLCD(0x01,1); //显示清屏
    WriteCommandLCD(0x34,1); // 显示光标移动设置
    WriteCommandLCD(0x30,1); // 显示开及光标设置
}


//按指定位置显示一串字符
void DisplayListChar(u8 X, u8 Y, u8 code *DData)
{
    u8 ListLength,X2;
    ListLength = 0;
    X2 = X;
    if(Y < 1)   Y=1;
    if(Y > 4)   Y=4;
    X &= 0x0F; //限制X不能大于16，Y在1-4之内
    switch(Y)
    {
        case 1: X2 |= 0X80; break;  //根据行数来选择相应地址
        case 2: X2 |= 0X90; break;
        case 3: X2 |= 0X88; break;
        case 4: X2 |= 0X98; break;
    }
    WriteCommandLCD(X2, 1); //发送地址码
    while (DData[ListLength] >= 0x20) //若到达字串尾则退出
    {
        if (X <= 0x0F) //X坐标应小于0xF
        {
            WriteDataLCD(DData[ListLength]); //
            ListLength++;
            X++;
        }
    }
}

//图形显示122*32
void DisplayImage (u8 code *DData)
{
    u8 x,y,i;
    unsigned int tmp=0;
    for(i=0;i<9;)       //分两屏，上半屏和下半屏，因为起始地址不同，需要分开
    {
        for(x=0;x<32;x++)   //32行
        {
            WriteCommandLCD(0x34,1);
            WriteCommandLCD((0x80+x),1);//列地址
            WriteCommandLCD((0x80+i),1);    //行地址，下半屏，即第三行地址0X88
            WriteCommandLCD(0x30,1);        
            for(y=0;y<16;y++)   
                WriteDataLCD(DData[tmp+y]);//读取数据写入LCD
            tmp+=16;        
        }
        i+=8;
    }
    WriteCommandLCD(0x36,1);    //扩充功能设定
    WriteCommandLCD(0x30,1);
}


//显示数字
void DisplayNum (u8 X, u8 Y, u8 num){
	if(num < 10) {
		DisplayListChar(X, Y, "  ");
		WriteDataLCD(num+48);
	} else {
		DisplayListChar(X, Y, " ");
		WriteDataLCD(num/10+48);
		Delay_Nms(10);
		WriteDataLCD(num%10+48);
	}
}
