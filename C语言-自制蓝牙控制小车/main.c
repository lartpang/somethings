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

u8  code name[]    		= {"����������ΰ    "};
u8  code num_learn[]    = {"ѧ�ţ�201583135 "};
u8  code time[]		    = {"ʱ�䣺2018 1031 "};
u8  code name_sub[]     = {"  ����ң��С��  "};

void main() {
    P0M1 = 0;   P0M0 = 0;   //����Ϊ׼˫���
    P1M1 = 0;   P1M0 = 0;   //����Ϊ׼˫���
    P2M1 = 0;   P2M0 = 0;   //����Ϊ׼˫���
    P3M1 = 0;   P3M0 = 0;   //����Ϊ׼˫���
    P4M1 = 0;   P4M0 = 0;   //����Ϊ׼˫���
    P5M1 = 0;   P5M0 = 0;   //����Ϊ׼˫���
    P6M1 = 0;   P6M0 = 0;   //����Ϊ׼˫���
    P7M1 = 0;   P7M0 = 0;   //����Ϊ׼˫���	

    DisableHC595();     //��ֹ��ѧϰ���ϵ�HC595��ʾ��ʡ��
	
    Delay_Nms(100); //�����ȴ�����LCD���빤��״̬
    LCDInit(); //LCM��ʼ��
	MT_Init();
	UART_Init();
	EA = 1;

	LCDClear();
	DisplayListChar(0,1,name);    //��ʾ�ֿ��е���������
	DisplayListChar(0,2,num_learn);       //��ʾ�ֿ��е���������
	DisplayListChar(0,3,time);       //��ʾ�ֿ��е�����
	DisplayListChar(0,4,name_sub);        //��ʾ�ֿ��е���������
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
					// ������ǰ���qֹͣ������ֻ���ٰ�o���ܵ����ٶȣ������ٶȲ�������
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
					sys_state=0; // ȷ���ٶ�
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
