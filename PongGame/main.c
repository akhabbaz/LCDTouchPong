


#include "lcd.h"


#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001




char displayChar = 0;

int main(void)
{
	//setting up the gpio for backlight
	DDRD |= 0x80;
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;
	
	//lcd initialisation
	lcd_init();
	
    uart_init();
	
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	adc_init();
	uint16_t xpos = readXPosition();

	while (1)
	{
		//drawchar(buff,0,0,displayChar);	
		//drawDiagonals(BLACK);
		//drawHomePage(BLACK)
		
		//uint16_t ypos = readYPosition();
		//uint16_t accel = adc_read(1 << MUX2);
		//printf(" X Position:%u, \t Y Position:%u \t accel: %u\n",xpos, ypos, accel);
		printf("adcval %u\n", adcval);
		drawcircle(buff, 64, 32, 10,BLACK);
		_delay_ms(1000);
	}
}

