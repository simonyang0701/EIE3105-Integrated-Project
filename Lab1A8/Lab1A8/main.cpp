/*
 * Lab1A8.cpp
 *
 * Created: 2017/9/23 0:34:30
 * Author : 13102841d
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF;
    while (1) 
    {
		PORTD = 0x55;
		_delay_ms(20000);
		PORTD = 0xAA;
		_delay_ms(20000);
    }
}

