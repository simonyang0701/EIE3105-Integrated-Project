/*
 * Lab4Db.c
 *
 * Created: 10/9/2017 3:04:31 PM
 * Author : 13102841d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000
#define BAUD 4800
#define MYUBRR FOSC/16/BAUD-1

// const setting
const unsigned char READY = 13, TERM = 2, BYE = 4;
unsigned char ready_string[13] = "We are ready!";
unsigned char term_string[2] = "Hi";
unsigned char bye_string[4] = "Bye!";
unsigned char ri, bi, input_index = 0;

void USART_Init( unsigned int ubrr) {
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0)|(1 << TXEN0)|(1 << RXCIE0);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0 << USBS0)|(3 << UCSZ00);
}

void USART_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

ISR(USART_RX_vect) {
	unsigned char input = UDR0;
	// Check input char
	if(input != term_string[input_index]) {
		input_index = 0;
		} else {
		input_index++;
		
		// input valid, send out bye string
		if(input_index == TERM) {
			input_index = 0;
			
			for(bi = 0; bi < BYE; bi++) {
				USART_Transmit(bye_string[bi]);
			}
		}
	}
}

int main(void)
{
	USART_Init(MYUBRR);

	sei();
	
	// Send out ready string
	for(ri = 0; ri < READY; ri++) {
		USART_Transmit(ready_string[ri]);
	}
	
	while(1);
}

