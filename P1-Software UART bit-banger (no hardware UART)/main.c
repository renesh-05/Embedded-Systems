#include<avr/io.h>
#include "sec.h"
int main(void)
{
    DDRB |= (1U<<DDB5);
    
    init_ms();

    while(1)
    {

    if((ms()/1000)%2==0)
    {
        PORTB |= (1U<<PORTB5);
    }
    else
    {
        PORTB &= ~(1U<<PORTB5);
    }

    }
}