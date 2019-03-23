/*
 * Lab2A Normal.cpp
 *
 * Created: 2017/9/23 14:26:35
 * Author : 1310284D
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
void T0Delay(int delay){//generate 0.1s
	int i=0;
	for(i=0;i<delay;i++){
		TCNT0 = 0x00; //255-16000000/1024/60=0
		TCCR0A = 0x00; //Timer 0, Normal mode
		TCCR0B = 0x05; //clk/1024
		while((TIFR0&(1<<TOV0))==0); //wait for TOV0 to roll over
		TCCR0B = 0; //turn off Timer 0
		TIFR0 = (1<<TOV0); //clear TOV0
	}
}