#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "adc.h"


uint16_t touchX = 0;
uint16_t touchY = 0;

void adc_init(){
	ADMUX = ~(1<<REFS1) | (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint16_t adc_read(uint8_t ADCchannel){	//1110
	
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); //00000011 & 00001111 => 00000011
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}

uint16_t readXPosition(){
	DDRC = 0b00001010;		//0b00001010 Setting X- & X+ as Outputs
	PORTC= 0b00001000;		//0b00001101 Setting X- High and X+ Low. & Setting Y- & Y+ in internal Pull up mode.
	ADMUX &= 0b11110000;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	//return ADC;
	touchX = ADC;
	touchX = ADC;
	return touchX;
}

uint16_t readYPosition(){
	DDRC = 0b00000101;	
	PORTC =0b00000001;	
	ADMUX &= 0b11110011;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	//return ADC;
	touchY = ADC;
	touchY = ADC;
	return touchY;
}