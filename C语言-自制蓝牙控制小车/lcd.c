
/*************  ����˵��    **************

ST7920�Ļ�����������

******************************************/
#include 	<STC15W4K58S4.h>
#include    "intrins.h"
#include 	"delay.h"
#include    "lcd.h"

#define 	LCD_Data P0
#define 	Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ

sfr TH2  = 0xD6;
sfr TL2  = 0xD7;

sbit    P_HC595_SER   = P4^0;   //pin 14    SER     data input
sbit    P_HC595_RCLK  = P5^4;   //pin 12    RCLk    store (latch) clock
sbit    P_HC595_SRCLK = P4^3;   //pin 11    SRCLK   Shift data clock

/**************************************/
sbit    LCD_RS 		= 	P1^3;  //��������
sbit    LCD_RW 		=	P1^4;
sbit    LCD_E  		= 	P1^5;
sbit    PSB    		= 	P1^6;      //PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1
sbit    LCD_RESET   =   P1^7;   //  17---RESET  L-->Enable
#define LCD_Data 		P0
/*************************************/


#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ

//========================================================================
// ����: void  Delay_Nms(u8 ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
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
    P_HC595_RCLK = 0;                           //�����������
    P_HC595_RCLK = 1;
    P_HC595_RCLK = 0;                           //�����������
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


//д����
void WriteDataLCD(u8 WDLCD)
{
    ReadStatusLCD(); //���æ 
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_Data = WDLCD;
        LCD_delay();
    LCD_E = 1;
    LCD_delay();
    LCD_E = 0;
}

//дָ��
void WriteCommandLCD(u8 WCLCD,BuysC) //BuysCΪ0ʱ����æ���
{
    if (BuysC) ReadStatusLCD(); //������Ҫ���æ 
    LCD_RS = 0;
    LCD_RW = 0; 
    LCD_Data = WCLCD;
        LCD_delay();
    LCD_E = 1; 
    LCD_delay();
    LCD_E = 0;  
}

//��״̬
u8 ReadStatusLCD(void)
{
    LCD_Data = 0xFF; 
    LCD_RS = 0;
    LCD_RW = 1; 
        LCD_delay();
    LCD_E = 1;
        LCD_delay();
    while (LCD_Data & Busy); //���æ�ź�
    LCD_E = 0;
    return(LCD_Data);
}

void LCDInit(void) //LCM��ʼ��
{
    PSB = 1;    //����
//  PSB = 0;    //SPI��
    Delay_Nms(10);
    LCD_RESET = 0;
    Delay_Nms(10);
    LCD_RESET = 1;
    Delay_Nms(100);
    
    WriteCommandLCD(0x30,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
    WriteCommandLCD(0x01,1); //��ʾ����
    WriteCommandLCD(0x06,1); // ��ʾ����ƶ�����
    WriteCommandLCD(0x0C,1); // ��ʾ�����������
}

void LCDClear(void) //����
{
    WriteCommandLCD(0x01,1); //��ʾ����
    WriteCommandLCD(0x34,1); // ��ʾ����ƶ�����
    WriteCommandLCD(0x30,1); // ��ʾ�����������
}


//��ָ��λ����ʾһ���ַ�
void DisplayListChar(u8 X, u8 Y, u8 code *DData)
{
    u8 ListLength,X2;
    ListLength = 0;
    X2 = X;
    if(Y < 1)   Y=1;
    if(Y > 4)   Y=4;
    X &= 0x0F; //����X���ܴ���16��Y��1-4֮��
    switch(Y)
    {
        case 1: X2 |= 0X80; break;  //����������ѡ����Ӧ��ַ
        case 2: X2 |= 0X90; break;
        case 3: X2 |= 0X88; break;
        case 4: X2 |= 0X98; break;
    }
    WriteCommandLCD(X2, 1); //���͵�ַ��
    while (DData[ListLength] >= 0x20) //�������ִ�β���˳�
    {
        if (X <= 0x0F) //X����ӦС��0xF
        {
            WriteDataLCD(DData[ListLength]); //
            ListLength++;
            X++;
        }
    }
}

//ͼ����ʾ122*32
void DisplayImage (u8 code *DData)
{
    u8 x,y,i;
    unsigned int tmp=0;
    for(i=0;i<9;)       //���������ϰ������°�������Ϊ��ʼ��ַ��ͬ����Ҫ�ֿ�
    {
        for(x=0;x<32;x++)   //32��
        {
            WriteCommandLCD(0x34,1);
            WriteCommandLCD((0x80+x),1);//�е�ַ
            WriteCommandLCD((0x80+i),1);    //�е�ַ���°������������е�ַ0X88
            WriteCommandLCD(0x30,1);        
            for(y=0;y<16;y++)   
                WriteDataLCD(DData[tmp+y]);//��ȡ����д��LCD
            tmp+=16;        
        }
        i+=8;
    }
    WriteCommandLCD(0x36,1);    //���书���趨
    WriteCommandLCD(0x30,1);
}


//��ʾ����
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
