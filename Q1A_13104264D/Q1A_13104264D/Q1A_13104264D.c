/*
 * Q1A_13104264D.c
 *
 * Created: 11/11/2016 3:48:16 PM
 *  Author: 13104264d
 SUN Zhichao Q1A
 */ 


#include <avr/io.h>

void delay (unsigned long delayCTC){
	unsigned long i=0;
	while(i<delayCTC)
	{
		OCR1A=0X00F9;
		TCCR1A=0X02;
		TCCR1B=0X05;
		while ((TIFR1&(1<<OCR1A))==0);
		TCCR1A=0;
		TCCR1B=0;
		TIFR1 = 0X01;
		i++;
	}
	
}
int main(void)
{
	DDRB = 0xff;
	PORTB = 0x00;
	DDRD=0X00;
	
	
	while(1){
		
		//OCR0A=3;
		//TCCR0A=0X02;
		//TCCR0B=0X07;
		//while ((TIFR0&(1<<OCF0A))==0);
		//TCCR0A=0;
		//TCCR0B=0;
		//TIFR0 = 0X02;
		
		
		PORTB =0x00;
		delay (30);
		PORTB =0X03;
		delay (30);
		//OCF0A=0;
		
		
		
		
		
		PORTB= 0X01;
		delay (15);
		PORTB= 0X02;
		delay (15);
		
		
	}
	
}