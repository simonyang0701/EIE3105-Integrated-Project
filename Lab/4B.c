/*
 * Lab4B.c
 *
 * Created: 10/9/2017 2:51:32 PM
 * Author : 13102841d
 */ 

#include <avr/io.h>

#define FOSC 16000000
#define BAUD 4800
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr) {
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0)|(1 << TXEN0);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0 << USBS0)|(3 << UCSZ00);
}

void USART_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

int main(void)
{
	const unsigned char READY = 13, TERM = 2, BYE = 4;
	unsigned char ready_string[13] = "We are ready!";
	unsigned char term_string[2] = "Hi";
	unsigned char bye_string[4] = "Bye!";
	unsigned char ri, bi, input_index = 0;
	USART_Init(MYUBRR);
	unsigned char input;
	
	// Send out ready string
	for(ri = 0; ri < READY; ri++) {
		USART_Transmit(ready_string[ri]);
	}
	while(1) {
		input = USART_Receive();
		
		// Check input char
		if(input != term_string[input_index]) {
			input_index = 0;
			continue;
			} else {
			input_index++;
		}
		
		// input valid, send out bye string
		if(input_index == TERM) {
			input_index = 0;
			
			for(bi = 0; bi < BYE; bi++) {
				USART_Transmit(bye_string[bi]);
			}
		}
		// USART_Transmit(input);
	}
}

