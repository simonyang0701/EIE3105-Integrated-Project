/*
 * Lab5B.c
 *
 * Created: 12/2/2016 4:59:36 PM
 *  Author: 13102841d
 */ 


#include "avr/io.h"
int main(void)
{
	

	unsigned char ch,count,c1,c2,t1,pulsewidth;
	
	DDRD = 0b00100000; //PD5 (OC0Ba)
	UCSR0B = 0x18;//(1<<RXEN)|(1<<TXEN);
	UCSR0C = 0x06;//(1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	UBRR0L = 107; //9600 baud rate
	count = c1 = 0;

	while(1)
	{
		while(!(UCSR0A & (1<<RXC0))); //new data received
		ch = UDR0;
		if(ch >= '0' && ch<='9') count++;//if input is a number between 0-9
		else count = 0;
		
		if(count == 1 && ch>'0') //range : 10-99 -> 0X (X=1-9 should be rejected)
		c1 = ch;
		if(count == 2)//two character
		{
			c2 = ch;
			//////debug
			for(unsigned char i=0;i<2;i++)
			{
				while (!(UCSR0A & (1<<UDRE0))); // is empty <-- what does it mean
				if(i==0) UDR0 = c1; 
				if(i==1) UDR0 = c2;
			}//output the input value
			count = 0;
			
			OCR0B = (c1 - '0')*10+(c2-'0');
			OCR0A = 127;//((c1 - '0')*10+(c2-'0'))*2; // 50Hz
			
			//timer 0
			TCCR0A = (1<< COM0A1)| (0<< COM0A0) | (1<< COM0B1) | (0<< COM0B0)| (1<<WGM01) | (1<< WGM00); //pre 1024, Fast PWM
			TCCR0B = (1<< WGM02)| (0<<CS01) | (1<< CS02) | (1<< CS00); //presc 1024, fast pwm	//101 here :o) !100
			PORTB = 0xFF;
			//timer 1
			TCCR1A = 0;
			TCCR1B = (1<<ICES1)|(1<<CS12)|(0<<CS11)|(0<<CS10);
			TIFR1 = (1<<ICF1);
			
			while((TIFR1&(1<<ICF1)) == 0);
			
			
			
			t1 = ICR1L;
			TIFR1 = (1<<ICF1);
			TCCR1B = (0<<ICES1)|(1<<CS12)|(0<<CS11)|(1<<CS10);
			while((TIFR1&(1<<ICF1)) ==0);
			pulsewidth = ICR1L - t1;//== get pulse width
			TIFR1=(1<<ICF1);
			c2=(pulsewidth%10);
			c1=((pulsewidth-c2)/10)+'0';//c1
			c2+='0';//c2
			for(unsigned char i=0;i<2;i++)
			{
				while (!(UCSR0A & (1<<UDRE0))); // is empty <-- what does it mean
				if(i==0) UDR0 = c1;
				if(i==1) UDR0 = c2;
			}
		}
		
		
	}

}