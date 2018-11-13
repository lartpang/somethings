#include <STC15W4K58S4.H>
#include "motor.h"
/* 这个文件和书上的P194程序基本一致
*/

unsigned char curr_speed=0;
unsigned char run_flag=1;
unsigned char run_direct=1;

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

void MT_Init(void) {
	P2M0=0;
	P2M1=0;
	P_SW2|=BIT7;
	PWMCKS=0;
	PWMCFG=0;
	PWMC=CYCLE;

	PWM5CR=0;
	PWM5T1=0;
	PWM5T2=CYCLE*50/100;
	PWM4CR=0;
	PWM4T1=0;
	PWM4T2=CYCLE*50/100;

	PWMCR=BIT3+BIT2;
	PWMCR|=ENPWM;
	P_SW2&=~BIT7;

	MT_Forward();
	MT_SetSpeed(0);
	curr_speed=0;
	run_flag=1;
}

void MT_SetSpeed(unsigned char wide) {
	if(run_flag) {
		curr_speed=wide;
		
		if(!run_direct) {
			wide=100-wide;
		}
		
		if(wide) {
			P_SW2|=BIT7;
			PWM5T2=(u16)(CYCLE/(100.0/wide));
			PWM4T2=(u16)(CYCLE/(100.0/wide));
			P_SW2 &=~BIT7;
			PWMCR|=BIT3+BIT2;
		}
		else {
			PWMCR &=~(BIT3+BIT2);
			IN1=0;
			IN2=0;
			IN3=0;
			IN4=0;
		}
	}
}

void MT_Forward(void) {
	IN1=1;
	IN2=0;
	IN3=1;
	IN4=0;
	run_direct=1;
	MT_SetSpeed(curr_speed);
}

void MT_Backward(void) {
	IN1=0;
	IN2=1;
	IN3=0;
	IN4=1;
	run_direct=0;
	MT_SetSpeed(curr_speed);
}

void MT_Stop(void) {
	PWMCR &=~(BIT3+BIT2);
	IN1=0;
	IN2=0;
	IN3=0;
	IN4=0;
	MT_Forward();
	curr_speed=0;
	MT_SetSpeed(curr_speed);
	run_flag=0;
}

void MT_Start(void) {
	run_flag=1;
}

/**********************************************/

void SetSpeed_Left(u8 wide) {
	if(run_flag) {
		P_SW2 |= BIT7;
		PWM4T2 = (u16)(CYCLE/(100.0/wide));
		PWM5T2 = (u16)(CYCLE/(100.0/1));
		P_SW2 &= ~BIT7;
		PWMCR |= BIT2;
		PWMCR &= ~BIT3;
	}
}

void SetSpeed_Right(u8 wide) {
	if(run_flag) {
		P_SW2 |= BIT7;
		PWM5T2 = (u16)(CYCLE/(100.0/wide));
		PWM4T2 = (u16)(CYCLE/(100.0/1));
		P_SW2 &= ~BIT7;
		PWMCR &= ~BIT2;
		PWMCR |= BIT3;
	}
}

void MT_Left(void) {
	IN1=1;
	IN2=0;
	IN3=0;
	IN4=1;
	//SetSpeed_Left(1);	
	SetSpeed_Right(curr_speed);
}

void MT_Right(void) {
	IN1=0;
	IN2=1;
	IN3=1;
	IN4=0;
	SetSpeed_Left(curr_speed);
 	//SetSpeed_Right(1);
}

