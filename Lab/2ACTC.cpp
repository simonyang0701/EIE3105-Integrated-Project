/*
 * Lab2A CTC.cpp
 *
 * Created: 2017/9/24 0:02:16
 * Author : 13102841d
  * T = 1/16 MHz = 0.0625 us
  * TCNT0 = 0
  * 256 * 0.0625 us = 16 us
  * 1s / 16 us = 62500
  * 62500 / 1024 = 61;
 */ 

#include <avr/io.h>

void T0Delay(int delay);
int main(void)
{
    /* Replace with your application code */
		DDRB = 0xFF;
		PORTB = 0x01;
    while (1) 
    {
		T0Delay(61);
		PORTB ^= 0x01;
    }
}
void T0Delay(int delay){
	int i=0;
	for(i=0;i<delay;i++){
		OCR0A = 255;
		TCCR0A = 0x02;
		TCCR0B = 0x05;
		while((TIFR0&(1<<OCR0A))==0);
		TCCR0B = 0;
		TIFR0 = (1<<TOV0);
	}
}
