/*
 * Lab2B Normal.cpp
 *
 * Created: 2017/9/24 2:25:49
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
		/*
		DDRB = 0xFF;
		DDRD = 0xFF;
		DDRC = 0xFF;
		PORTD = 0xFF;
		PORTB = 0x3F;
		PORTC = 0x30;
		*/
		
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
		TCNT0 = 0x00; //load TCNT0
		TCCR0A = 0x00; //Timer 0, Normal mode
		TCCR0B = 0x05; //clk/1024
		while((TIFR0&(1<<TOV0))==0); //wait for TOV0 to roll over
		TCCR0B = 0; //turn off Timer 0
		TIFR0 = (1<<TOV0); //clear TOV0
	}
}
void T0DelayBlinking(int delay){
	for(int j=0;j<20;j++){
		for(int i=0;i<delay;i++){
			TCNT0 = 0x00; //load TCNT0
			TCCR0A = 0x00; //Timer 0, Normal mode
			TCCR0B = 0x05; //clk/1024
			while((TIFR0&(1<<TOV0))==0); //wait for TOV0 to roll over
			TCCR0B = 0; //turn off Timer 0
			TIFR0 = (1<<TOV0); //clear TOV0
		}
		PORTC ^=0x20;		
	}
}
