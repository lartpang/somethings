#ifndef _MOTOR_H
#define _MOTOR_H

/*按需更改
  电机驱动控制
*/

sbit IN1=P2^2;
sbit IN2=P2^1;
sbit IN3=P2^3;
sbit IN4=P2^4;

#define PWM4 IN1
#define PWM5 IN3;
#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)
#define CYCLE 0x7fffl

#define ENPWM 0x80

void MT_Init(void);
void MT_SetSpeed(unsigned char wide);
void MT_Forward(void);
void MT_Backward(void);
void MT_Left(void);
void MT_Right(void);
void MT_Stop(void);
void MT_Start(void);

#endif