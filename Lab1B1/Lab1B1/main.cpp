/*
 * Lab1B1.cpp
 *
 * Created: 2017/9/23 1:25:21
 * Author :13102841d
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF;
	DDRB = 0x00;
	PORTD = 0x00;
    while (1) 
    {
		if(PINB & (1<<0)){
			PORTD = 0x01;
		}
		else{
			PORTD = 0x00;
		}
    }
}

