/*
 * Lab1B2.cpp
 *
 * Created: 2017/9/23 13:44:53
 * Author : 13102841D
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	unsigned char state = 0x00;
	unsigned char flag = 0x00;
    while (1) 
    {
		while(flag == (PINB & 0x01));
		flag = PINB & 0x01;
		if(flag){
			if(PINB & 1<<0){
				if(state == 0){
					state = 1;
				}
				else{
					state = 0;
				}
			}
			if(state == 1){
				PORTD = 0xFF;
			}
			else{
				PORTD = 0x00;
			}
		}		
		_delay_ms(2000);
    }
}

