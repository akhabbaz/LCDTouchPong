
#include "lcd.h"

//#define FREQ 16000000
//#define BAUD 9600
//#define HIGH 1
//#define LOW 0
//#define BUFFER 1024
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
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	uint8_t  chloc = 0;
	uint8_t  line = 0;
	while (1)
	{
		drawchar(buff,8,0,displayChar);
		drawchar(buff,0,1,displayChar);
		write_buffer(buff);
		_delay_ms(2000);
		setpixel(buff,   0,  0, BLACK);
        setpixel(buff, 127, 63, BLACK);
		displayChar++;
	}
}

