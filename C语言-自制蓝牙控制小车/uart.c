#include "STC15W4K58S4.h"

//����������
void UART_Init(void) {
	SCON = 0x50;
	AUXR &= 0xBF;
	AUXR &= 0xFE;
	//AUXR1 |= 0x40;
	TMOD &= 0x0F;
	
	TL1 = 0xE6;	//���ö�ʱ��2��ʱ��ֵ
	TH1 = 0xFF;	//���ö�ʱ��2��ʱ��ֵ
	ET1 = 0;
	TR1 = 1;
	REN = 1;
	ES = 1;
}

//���ڷ�������
void UART1_SendData(unsigned char dat) {
	SBUF = dat;
	TI = 0;
	while(!TI);
}

//���ڷ����ַ���
void UART1_SendString(char *str){
	unsigned char idx = 0;
	while(*(str+idx) != '\0'){
		UART1_SendData(*(str + idx));
		idx++;
	}
}

