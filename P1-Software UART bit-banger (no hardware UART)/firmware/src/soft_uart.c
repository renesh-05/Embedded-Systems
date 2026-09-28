#include<avr/io.h>
#include<stdint.h>
#include "soft_uart.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define custom_delay(us) __builtin_avr_delay_cycles((uint32t)(us*(F_CPU/1000000.0)))

#define BIT_DELAY 104
#define HALF_BIT_DELAY 52

void soft_uart_init(void)
{
    DDRD |= (1U<<TX_PIN);
    DDRD &= ~(1U<<RX_PIN);
    
    PORTD |= (1U<<TX_PIN);
    PORTD |= (1U<<RX_PIN);

}

void soft_uart_transmit(char data)
{
    PORTD &= ~(1U<<TX_PIN);
    custom_delay(BIT_DELAY);

    for(uint8_t i=0;i<8;i++)
    {
        if(data & 1U)
        {
            PORTD |= (1U<<TX_PIN);
        }
        else
        {
            PORTD &= ~(1U<<TX_PIN);
        }
        custom_delay(BIT_DELAY);
        data>>=1;
    }

    PORTD |= (1U<<TX_PIN);
    custom_delay(BIT_DELAY);
}

char soft_uart_receieve(void)
{
   char data=0;

   while(PIND & (1U<<RX_PIN));

   custom_delay(BIT_DELAY + HALF_BIT_DELAY);

   for(uint8_t i=0;i<8;i++)
   {
    data>>=1;

    if(PIND & (1U<<RX_PIN))
    {
        data |= 0x80;
    }
    custom_delay(BIT_DELAY);
   }

    custom_delay(BIT_DELAY);

   return data;
}