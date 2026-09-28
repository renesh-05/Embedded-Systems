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
        char recived = soft_uart_receieve();

        soft_uart_transmit(recived);
    }
    return 0;
}