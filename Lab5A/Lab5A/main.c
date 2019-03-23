/*
 * Lab5A.c
 *
 * Created: 2016/12/6 4:42:41
 * Author : 13102
 Generate a wave using PWM mode.
 Timer0: Fast PWM mode
 frequency: 500Hz
 Duty cycle:50%
 Connect a LED to the pin which generates the wave
 Change the duty cycle to control the brightness of the LED
 */

 /*#include <avr/io.h>
 int main(void)
 {
	 TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) |(1 << WGM01)	| (1 << WGM00);
	 TCCR0B = (1 << WGM02) |(0 << CS02)	| (0 << CS01)	| (1 << CS00);

	 OCR0A = 249;	//64kHz
	 OCR0B = 49;	//20% duty cycle
	 DDRD = 0b00100000; // PD (OC0B)

	 while(1);
	 }*/


	 #include <avr/io.h>
	 int main(void)
	 {
	 TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) |(1 << WGM01)| (1 << WGM00);
	 TCCR0B = (0 << WGM02) | (1 << CS02)	| (0 << CS01)| (0 << CS00);//fast PWM mode, prescaler 256

	 OCR0A = 124;	//16M/(256*500)=125 cycles 125-1=124, 500Hz
	 OCR0B = 61;	//50% duty cycle
	 DDRD = 0b00100000; // PD5 (OC0B)

	 while(1);
	 }
