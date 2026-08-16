#include <avr/io.h>
#include <avr/interrupt.h>
#include "sec.h"

volatile uint32_t t=0;
volatile uint8_t t_f=0;

ISR(TIMER0_OVF_vect)
{
    t++;
    t_f++;
    if(t_f>=42)
    {
        t_f=0;
        t++;
    }
}

void init_ms(void)
{
    TCCR0B |= ((1U<<CS01) | (1U<<CS00)); 
    TIMSK0 |= (1U<<TOIE0);
    sei();
}

uint32_t ms(void)
{
 cli();
 uint32_t t_copy = t;
 sei();
 return t_copy;    
}