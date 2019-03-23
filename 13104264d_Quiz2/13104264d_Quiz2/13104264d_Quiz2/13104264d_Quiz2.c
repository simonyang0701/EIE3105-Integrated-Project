/*
 * _13104264d_Quiz2.c
 *
 * Created: 11/25/2016 3:38:59 PM
 *  Author: 13104264d
 */ 


#include <avr/io.h>
#include<avr/interrupt.h>


int count=0;
int state=0;

int main(void)
{
	DDRB = 0xff;
	TIMSK1=0X02;
	PORTB=0x01;
	PORTD |= (1 << 2);
	
	// interrupt setting
	EIMSK = 0x01;
	EICRA = 0x03;
	
	// Timer1 setting
	TCNT1 = 0;
	TCCR1A = 0x00;
	TCCR1B = 0x0D;
	OCR1A = 0x2fff;
	
	// Timer1 overflow interrupt
	TIMSK1 = (1 << OCIE1A);
	
	sei(); //enable interrupts
	
	while(1)
	{
	}
}


ISR (INT0_vect) {
	state=1-state;
	
	if(state==0){
		PORTB=0x01;
	}
	if(state==1){
		PORTB=0x00;
	}
	
}

ISR (TIMER1_COMPA_vect){
	if(state==0){
		if(count==2){
			PORTB^=0b00000011;//toggle
			count=0;
		}
		else count++;
	}
	if(state==1){
		PORTB^=0b00000011;//toggle
	}
	
}
