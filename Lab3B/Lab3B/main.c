/*
 * Lab3B.c
 *
 * Created: 10/9/2017 1:59:18 PM
 * Author : 13102841d
 */ 

#include "avr/io.h"
#include "avr/interrupt.h"

void T1Delay_ctc();
void T1blink_ctc();

unsigned char count = 0;

int main(void)
{
	// Serial.begin(9600);
	
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD &= 0x00;
	
	PORTB &= 0;
	PORTC &= 0;
	PORTD |= (1 << 2);	// pull up PORTD.2
	
	unsigned char i, j, k;
	unsigned char b_states[8] = {0x21, 0x11, 0x09, 0x0B, 0x0C, 0x0A, 0x09, 0x19};
	unsigned char c_states[8] = {0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x10, 0x10};
	unsigned char t_states[8] = {5, 1, 1, 1, 5, 1, 1, 1};

	// interrupt setting
	EIMSK = 0x01;
	EICRA = 0x03;
	sei();
	
	while(1)
	{
		for(i = 0; i < 8; i++) {
			if(i == 0) {
				PORTB |= b_states[i];
				PORTC |= c_states[i];
				} else {
				PORTB ^= (b_states[i - 1] ^ b_states[i]);
				PORTC ^= (c_states[i - 1] ^ c_states[i]);
			}
			if(i != 5) {
				for(j = 0; j < t_states[i]; j++)
				T1Delay_ctc();
				} else {
				for(k = 0; k < 5; k++) {
					PORTC ^= (0x1 << 5);
					T1blink_ctc();
				}
				PORTC ^= (0x1 << 5);
			}
		}
		PORTB &= 0;
		PORTC &= 0;
	}
}

void T1Delay_ctc() {
	TCNT1 = 0;
	OCR1A = 0x3D09;
	TCCR1A = 0x00;
	TCCR1B = 0x0D;
	while((TIFR1 & (1 << OCF1A)) == 0);
	TCCR1B = 0x00;
	TIFR1 = 0x1 << OCF1A;
}

void T1blink_ctc() {
	TCNT1 = 0;
	OCR1A = 0x0C35;
	TCCR1A = 0x00;
	TCCR1B = 0x0D;
	while((TIFR1 & (1 << OCF1A)) == 0);
	TCCR1B = 0x00;
	TIFR1 = 0x1 << OCF1A;
}

ISR (INT0_vect) {
	count ++;
	if(count > 2) {
		PORTC ^= 0x01;
		count = 0;
	}
}




