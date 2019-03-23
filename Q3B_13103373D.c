/*
 * Q3B_13103373D.c
 *
 * Created: 12/2/2016 3:49:06 PM
 *  Author: 13103373d
 */ 


#include <avr/io.h>
#include<avr/interrupt.h>
//int state=0;
int count=0;
int on=0;
void init()
{
	UCSR0B=(1<<TXEN0)|(1<<RXEN0);
	UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);
	UCSR0B|=(1<<RXCIE0)|(1<<UDRIE0);
	UBRR0H=0;
	UBRR0L=103;//9600
}
int main(void)
{
	init();
	
	OCR1A=0x1E84;//0.5s
	TCCR1A=0x00;
	TCCR1B=0x0D;
	TIMSK1=(1<<OCIE1A);//enable Timer1 overflow interrupt
	
	sei(); //enable interrupts
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}


ISR (TIMER1_COMPA_vect){
		on=1;	
	
}

ISR(USART_UDRE_vect)
{
	if((on==1)){
		UDR0='a';
		on=0;
	}	
}

ISR(USART_RX_vect){
	unsigned char c;
	c=UDR0;
	if(c=='3'){
		OCR1A=0x7A12;//2s	
	}
	else if(c=='2'){
		OCR1A=0x3D02;//1s
	}
	else if(c=='0'){
		OCR1A=0x1E84;//0.5s
	}
}

