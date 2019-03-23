/*
 * Lab4C.c
 *
 * Created: 10/9/2017 3:00:38 PM
 * Author : 13102841d
 */ 

#include <avr/io.h>

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

// State const
const unsigned char NORMAL = 0;
const unsigned char PAUSE = 1;
const unsigned char TENCHAR = 2;
const unsigned char DEFAULT_CHAR  = 'a';

// Global variable
unsigned char tmp_char = ' ';

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

unsigned char getNextState(unsigned char cur, unsigned char input_char) {
	if(cur ==  NORMAL) {
		if(input_char == DEFAULT_CHAR)
		return NORMAL;
		
		tmp_char = input_char;
		return TENCHAR;
	}
	
	if(cur == PAUSE) {
		if(input_char != tmp_char)
		return PAUSE;

		return NORMAL;
	}
	
	if(cur ==  TENCHAR)
	return PAUSE;
	
	return 255;
}


int main(void)
{
	USART_Init(MYUBRR);
	unsigned char i, input = 'a', state = NORMAL;

	while(1) {
		// USART_Transmit(DEFAULT_CHAR);
		// input = USART_Receive();
		input = 'a';
		if(UCSR0A & (1 << RXC0))
		input = UDR0;
		
		state = getNextState(state, input);
		
		if(state == NORMAL)
		USART_Transmit(DEFAULT_CHAR);
		else if (state == TENCHAR) {
			for(i = 0; i < 10; i++)
			USART_Transmit(tmp_char);
		}
	}
}

