/*
 * Quiz4b_13104264d.c
 *
 * Created: 12/2/2016 3:31:22 PM
 *  Author: 13104264d sunzhichao
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char on=0;

void init()
{
	UCSR0B=(1<<TXEN0)|(1<<RXEN0);
	UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);
	UCSR0B|=(1<<RXCIE0)|(1<<UDRIE0);
	UBRR0H=0;
	UBRR0L=103;//9600  
	
	DDRB = 0xff;
	TIMSK1=0X02;
	PORTB=0x00;

	// interrupt setting
	EIMSK = 0x01;
	EICRA = 0x03;
	
	// Timer1 setting
	OCR1A = 0x4fff;
	TCCR1A=0x00;
	TCCR1B=0x0D;
	TIMSK1=(1<<OCIE1A);//enable Timer1 overflow interrupt

	
}



int main(void)
{
 
init();
sei();
while(1);

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
		OCR1A=0x8fff;//2s
	}
	else if(c=='2'){
		OCR1A=0x4fff;//1s
	}
	else if(c=='0'){
		OCR1A=0x2fff;//0.5s
	}
}
