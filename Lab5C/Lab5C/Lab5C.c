/*
 * Lab5C.c
 *
 * Created: 12/5/2016 4:23:59 PM
 *  Author: 13104264d
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
unsigned char device=1;


ISR(ADC_vect){
	if(device==1){
		ADMUX=0B11100000;	//2.56V ADC 0
		//ADMUX = ADCmultiplexer selection register
		if(ADCH>0xEF){
			OCR0B=124;
		}
		else{
			OCR0B=0;
		}
		device++;
	}
	
	else if(device==2){
		ADMUX=0B11100001;	//2.56V ADC 0
		if(ADCH>0xEF){
			OCR1B=124;
		}
		else{
			OCR1B=0;
		}
		device++;
	}
	else if(device==3){
		ADMUX=0B11100010;	//2.56V ADC 0
		if(ADCH>0xEF){
			OCR2B=124;
		}
		else{
			OCR2B=0;
		}
		device=1;
	}
	ADCSRA|=(1<<ADSC);
}


int main(void)
{
	DDRD |= (1<<5)|(1<<3);
	DDRB |= (1<<2);//PD3, PD5, PB2: PWM output
	DDRC = 0;
	ADCSRA = 0x8F; //10001111? ADC enable, ADC interrupt enable, ck/128
	ADMUX = 0xE0;  //11100000? ADC0, left adjusted, 1.11 internal
	//3 timer
	TCCR0A = 0xA3; //10100011, clear mode, fast pwm, OCR0A as top
	TCCR0B = 0x0C; //00001100, prescaler 256
	OCR0A = 124;   // 16M/(500*256)
	TCCR1A = 0xA3; //10100011, clear mode, fast pwm, OCR1A as top
	TCCR1B = 0x1C; //00011100, prescaler 256
	OCR1A = 124;    // 16M/(500*256)
	
	TCCR2A = 0xA3; //10100011,  clear mode, fast pwm, OCR2A as top
	TCCR2B = 0x0E; //00001110, prescaler 256
	OCR2A = 124;   // 16M/(500*256)

	sei();          //enable interrupt
	ADCSRA |= (1<<ADSC);//start conversion
	while(1){};
	
	while(1)
	{
		
		//TODO:: Please write your application code
		
		
	}
	return 0;
}