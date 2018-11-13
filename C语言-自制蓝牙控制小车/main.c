#include <STC15W4K58S4.h>
#include "lcd.h"
#include "delay.h"
#include "uart.h"
#include "motor.h"

unsigned char rxbuf = 0;
unsigned char rx_flag = 0;
unsigned char start_flag = 0;
unsigned char speed = 0;
unsigned char sys_state = 0;
unsigned char cnt = 0;

u8  code name[]    		= {"姓名：庞有伟    "};
u8  code num_learn[]    = {"学号：201583135 "};
u8  code time[]		    = {"时间：2018 1031 "};
u8  code name_sub[]     = {"  蓝牙遥控小车  "};

void main() {
    P0M1 = 0;   P0M0 = 0;   //设置为准双向口
    P1M1 = 0;   P1M0 = 0;   //设置为准双向口
    P2M1 = 0;   P2M0 = 0;   //设置为准双向口
    P3M1 = 0;   P3M0 = 0;   //设置为准双向口
    P4M1 = 0;   P4M0 = 0;   //设置为准双向口
    P5M1 = 0;   P5M0 = 0;   //设置为准双向口
    P6M1 = 0;   P6M0 = 0;   //设置为准双向口
    P7M1 = 0;   P7M0 = 0;   //设置为准双向口	

    DisableHC595();     //禁止掉学习板上的HC595显示，省电
	
    Delay_Nms(100); //启动等待，等LCD讲入工作状态
    LCDInit(); //LCM初始化
	MT_Init();
	UART_Init();
	EA = 1;

	LCDClear();
	DisplayListChar(0,1,name);    //显示字库中的中文数字
	DisplayListChar(0,2,num_learn);       //显示字库中的中文数字
	DisplayListChar(0,3,time);       //显示字库中的中文
	DisplayListChar(0,4,name_sub);        //显示字库中的中文数字
	Delay_Nms(5000);

	LCDClear();
	DisplayListChar(0, 1, "The Car         ");
	DisplayListChar(0, 2, "     is Working.");
	DisplayListChar(0, 3, "speed   "); 

    while(1) {
		if(rx_flag) {
			rx_flag=0;
			UART1_SendString("Bluetooth open successfully\n");
			switch(rxbuf) {
				case 'i':
					UART1_SendString("init\n");
					MT_Start();
					start_flag = 1;
					DisplayListChar(0,4,"init    ");
					break;
				case 'q':
					UART1_SendString("stop\n");
					sys_state=0; 
					// 这里的是按下q停止，并且只有再按o才能调节速度，但是速度不会置零
					MT_Stop();
					speed = 0;
					start_flag = 0;
					DisplayListChar(0, 4, "stop    ");
					break;
				case 'f':
					UART1_SendString("forward\n");
					MT_Forward();
					DisplayListChar(0,4,"forward ");
					break;
				case 'b':
					UART1_SendString("backward\n");
					MT_Backward();
					DisplayListChar(0,4,"backward");
					break;

 				case 'r':
					UART1_SendString("right\n");
					MT_Right();
					DisplayListChar(0,4,"right   ");
					break;
				case 'l':
					UART1_SendString("left\n");
					MT_Left();
					DisplayListChar(0,4,"left    ");
					break;

				case 'a':
					UART1_SendString("accelerate\n");
					sys_state=1;
					DisplayListChar(0,3,"speed ++");
					break;
				case 'd':
					UART1_SendString("decelerate\n");
					sys_state=2;
					DisplayListChar(0,3,"speed --");
					break;
				case 's':
					UART1_SendString("set speed \n");
					sys_state=0; // 确定速度
					DisplayListChar(0,3,"setspeed");
					break;
				default:
					break;
			}
		}	
		
		switch(sys_state){
			case 0x01:
				if(start_flag) {
					if(cnt++ > 10){
						cnt = 0;
						if(speed < 99) {
							speed++;
						}
						MT_SetSpeed(speed);
					}
				}
				break;
			case 0x02:
				if(start_flag) {
					if(cnt++ > 10){
						cnt = 0;
						if(speed > 1) {	
							speed--;
						}
						MT_SetSpeed(speed);
					}
				}
				break;
			default:
				break;
		}
		DisplayNum (5, 3, speed);
	}
}

void Uart_Handler() interrupt 4 {	
	if (RI) {
		RI = 0;
		rxbuf = SBUF;
		rx_flag = 1;
	}
	if(TI){
		TI=0;
	}
}
