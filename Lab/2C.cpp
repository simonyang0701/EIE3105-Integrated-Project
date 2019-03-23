/*
 * Lab2C.cpp
 *
 * Created: 2017/9/24 19:48:24
 * Author : 13102841d
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRD=0x00;
	DDRC=0x01;
	int state = 0;
    while (1) 
    {
		PORTC = (state)>>0;//reset portC0
		OCR0A = 2;//press 3 times
		TCCR0A = 0x02;//CTC 
		TCCR0B = 0x06;//External Clock
		while((TIFR0&(1<<OCF0A))==0);
		state = 1-state;
		TIFR0 = 0x02;//reset OFC0A
    }
}