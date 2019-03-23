/*
 * LAB1c.c
 *
 * Created: 10/14/2016 8:12:28 PM
 *  Author: 12132054d
 */ 


#include <avr/io.h>

int main(void)
{
	DDRB &= 0x00;
	DDRD |= 0xFF;
	PORTD &= 0x00;
	unsigned char last, current;
	
    while(1) {
		current = (PINB & 0x01);
		if((last == 0x00) && (current == 0x01))
			PORTD ^= 0xFF;
		last = current;
    }
}