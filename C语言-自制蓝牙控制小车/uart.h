#ifndef _UART_H_
#define _UART_H_

void UART_Init(void);
void UART1_SendData(unsigned char dat);
void UART1_SendString(char *str);

#endif