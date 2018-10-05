#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "adc.h"


uint16_t touchX = 0;
uint16_t touchY = 0;

void adc_init(){
	ADMUX  |= ~(1<<REFS1) | (1<<REFS0); // should use external capacitor on ARef
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // enable ADC and set rate to 1/128
	ADCSRB |= 0x00; // free running
	DDRC = 0x0;  // initialize to read
}

uint16_t adc_read(uint8_t ADCchannel){	//1110
	
	ADCchannel &= 0b0000011; // zero out high bits
	ADMUX = (ADMUX & 0xF8) | ADCchannel; //00000011 & 00001111 => 00000011
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}

uint16_t readXPosition(){
	DDRC = (1 << DDC3) | (1 << DDC1) ;		//0b00001010 Setting X- & X+ as Outputs
	PORTC = ( 1 << PORTC3) ;		//0b00001101 Setting X- High and X+ Low. & Setting Y-  internal Pull up mode.
	return adc_read( MUX0);
	// DDRC &=  ~( ( 1 << DDC3) | (1 << DDC1));  // set output bits back to read for Y reading 
	//ADMUX &= 0b11110000;
	//ADCSRA |= (1<<ADSC);   // enable conversion
	//while (ADCSRA & (1<<ADSC));
	////return ADC;
	//touchX = ADC;
	//touchX = ADC;
	//return touchX;
}

uint16_t readYPosition(){
	DDRC = ( 1 << DDC2 ) | (1 << DDC0); //  enable y output 	
	PORTC =  1 << PORTC0;
	return adc_read( MUX3);  
//	ADMUX &= 0b11110011;
//	ADCSRA |= (1<<ADSC);
//	while (ADCSRA & (1<<ADSC));
//	//return ADC;
//	touchY = ADC;
//	touchY = ADC;
//	return touchY;
}
