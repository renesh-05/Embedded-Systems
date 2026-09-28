#ifndef SOFT_UART_H
#define SOFT_UART_H

#include<stdint.h>

#define TX_PIN PD2
#define RX_PIN PD3

void soft_uart_init(void);
void soft_uart_transmit(uint8_t data);
uint8_t soft_uart_receive(void);

#endif