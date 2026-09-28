#include "soft_uart.h"

int main(void)
{
    soft_uart_init();

    char *msg = "Hello software UART is working.\r\nType to test recieving:\r\n";
    while(*msg)
    {
        soft_uart_transmit(*msg++);
    }

    while(1)
    {
        uint8_t recived = soft_uart_receive();

        soft_uart_transmit(recived);
    }
    return 0;
}