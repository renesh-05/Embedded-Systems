#ifndef SOFT_UART_H
#define SOFT_UART_H

#include<stdint.h>

#define TX PD2
#define RX PD3

void soft_uart_init(void);
void soft_uart_tranmit(char data);
char soft_uart_receieve(void);

#endif