#include	<intrins.h>
#include 	"delay.h"

/* 和书上P196代码一致
*/

void NOP(int num)
{
	int i = num;
	do{
		Delay1ms();
	}while(--i);
}

void Delay_200ms(void)
{
	unsigned char i,j,k;
	_nop_();
	_nop_();
	i=10;
	j=31;
	k=147;
	do{
		do{
			while(--k);
		}while(--j);
	}while(--i);
}

void Delay_Nms(unsigned int n_ms)
{
	unsigned char i,j;
	while(n_ms--){
		i=12;
		j=169;
		do{
			while(--j);
		}while(--i);
	}
}

void Delay500us()
{
	unsigned char i,j;
	i=6;
	j=211;
	do{
		while(--j);
	}while(--i);
}

void Delay50us()
{
	unsigned char i,j;
	i=1;
	j=146;
	do{
		while(--j);
	}while(--i);
}

void Delay3us()
{
	unsigned char i;
	_nop_();
	_nop_();
	i=6;
	while(--i);
}

void Delay30us()
{
	unsigned char i;
	_nop_();
	_nop_();
	i=87;
	while(--i);
}

void Delay10us()
{
	unsigned char i;
	_nop_();
	_nop_();
	i=27;
	while(--i);
}

void Delay900us()
{
	unsigned char i,j;
	i=11;
	j=126;
	do{
		while(--j);
	}while(--i);
}

void Delay1ms()
{
	unsigned char i,j;
	i=12;
	j=169;
	do{
		while(--j);
	}while(--i);
}

void Delay4500us()
{
	unsigned char i,j;
	i=53;
	j=132;
	do{
		while(--j);
	}while(--i);
}

void Delay600us()
{
	unsigned char i,j;
	i=7;
	j=254;
	do{
		while(--j);
	}while(--i);
}
