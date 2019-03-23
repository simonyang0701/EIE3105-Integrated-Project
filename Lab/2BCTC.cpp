/*
 * Lab2B CTC.cpp
 *
 * Created: 2017/9/24 19:41:31
 * Author : 13102841d
 */ 

#include <avr/io.h>

void T0Delay(int delay);
void T0DelayBlinking(int delay);
int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		PORTC = 0x10;
		PORTB = 0x21;
		T0Delay(61*5);
		
		PORTC = 0x10;
		PORTB = 0x11;
		T0Delay(61);
		
		PORTC = 0x10;
		PORTB = 0x09;
		T0Delay(61);
		
		PORTC = 0x10;
		PORTB = 0x0B;
		T0Delay(61);
		
		PORTC = 0x20;
		PORTB = 0x0C;
		T0Delay(61*5);
		
		PORTC = 0x20;
		PORTB = 0x0A;
		T0DelayBlinking(3);
		
		PORTC = 0x10;
		PORTB = 0x09;
		T0Delay(61);
		
		PORTC = 0x10;
		PORTB = 0x19;
		T0Delay(61);
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
}//0.1s
void T0DelayBlinking(int delay){
	for(int j=0;j<20;j++){
		for(int i=0;i<delay;i++){
			OCR0A = 255;
			TCCR0A = 0x02;
			TCCR0B = 0x05;
			while((TIFR0&(1<<OCR0A))==0);
			TCCR0B = 0;
			TIFR0 = (1<<TOV0);
		}
		PORTC ^=0x20;//PORTC1 blinking
	}
}
