#include <avr/pgmspace.h>
#include "lcd.h"

#define SID_DDR DDRD
#define SID_PIN PIND
#define SID_PORT PORTD
#define SID 6

#define SCLK_DDR DDRD
#define SCLK_PIN PIND
#define SCLK_PORT PORTD
#define SCLK 5

#define A0_DDR DDRD
#define A0_PIN PIND
#define A0_PORT PORTD
#define A0 4

#define RST_DDR DDRD
#define RST_PIN PIND
#define RST_PORT PORTD
#define RST 3

#define CS_DDR DDRD
#define CS_PIN PIND
#define CS_PORT PORTD
#define CS 2

uint8_t is_reversed = 0;

int pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };

// buffer input which prints initial splash
uint8_t buff[128*64/8] = {
	0x7F,0x7F,0x7F,0x7F,0x71,0x7F,0x71,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7F,0x7A,0x7B,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x71,0x7F,0x7F,0x71,0x7F,0x7F,0x7F,
	0x00,0x00,0x60,0x60,0x60,0x78,0x7F,0x7F,0x7F,0x7F,0x7F,0x60,0x60,0x60,0x60,0x60,
	0x60,0x70,0x70,0x78,0x78,0x3E,0x3F,0x1F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF1,0xF3,0xF7,0xFF,
	0xFF,0xFF,0xF7,0xF1,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x00,0x01,0x01,
	0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,
	0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x00,0x00,
	0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,

	0xE0,0xFF,0x03,0x00,0x01,0x03,0x07,0x0F,0x1E,0x3E,0x7F,0xFF,0xFF,0xFF,0x9F,0x0F,
	0x0F,0x8F,0xFF,0xFF,0xFF,0x7F,0x3E,0x1E,0x0F,0x07,0x03,0x01,0x00,0x01,0x3F,0xFC,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,
	0x1C,0x1C,0x1C,0x3C,0x78,0xF8,0xF8,0xF8,0xF1,0xEF,0xDF,0xFF,0x7F,0xFF,0xE1,0xC1,
	0x81,0x81,0x81,0x81,0x81,0xC1,0xE1,0xF9,0xFF,0xFF,0x7F,0x1F,0x87,0x80,0x80,0xC0,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x78,0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xFF,0xFF,
	0xFF,0xFF,0x3F,0x07,0x00,0x00,0x80,0x80,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x78,
	0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xE0,0xF0,0xFF,0xFF,0xFF,0x7F,0x1F,0x00,0x00,0x00,

	0x00,0x80,0xE0,0xF8,0xFC,0xFF,0xFF,0x3F,0x1F,0x1E,0x3C,0xF8,0xF0,0xE0,0xC0,0x80,
	0x00,0xC0,0xE0,0xF0,0xF8,0x3C,0x1E,0x1F,0x3F,0xFF,0xFF,0xFC,0xF8,0xE0,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xBF,0x81,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x30,0x18,0x0C,0x0C,0x06,0x06,
	0x06,0x0C,0x0C,0x18,0x18,0x30,0x60,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x06,0x0E,0x0E,0x3E,0xFE,0xFE,0xFE,0xFE,0xFE,0x7E,0x0E,0x0E,0x06,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xF8,0xFC,0xFC,0x7E,
	0x3E,0x1E,0x0E,0x0E,0x0E,0x06,0x0E,0x0E,0x0E,0x1C,0x38,0x78,0xE6,0x06,0x0E,0x1E,
	0xFE,0xFE,0xFE,0xFE,0xFE,0x0E,0x0E,0x06,0x00,0x00,0x00,0x06,0x06,0x0E,0xFE,0xFE,
	0xFE,0xFE,0xFE,0xFE,0x0E,0x0E,0x06,0x06,0x0E,0xFE,0xFE,0xFE,0xFE,0xFE,0x7E,0x0E,
	0x0E,0x06,0x00,0x00,0x00,0x06,0x0E,0x0E,0xFE,0xFE,0xFE,0xFE,0xFE,0x1E,0x0E,0x06,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFF,0xFF,0xCE,0xCE,0xCE,0xCE,0x00,0x1F,0x1F,0x1F,0x38,0x38,0x1F,0x1F,
	0x1C,0x38,0x3C,0x1F,0x0F,0x00,0xFF,0xFF,0x1C,0x38,0x38,0x1F,0x1F,0x07,0x07,0x1F,
	0x1F,0x3B,0x3B,0x1F,0x1F,0x07,0x07,0x1F,0x1F,0x38,0x38,0xFF,0xFF,0xFF,0x00,0x0F,
	0x1F,0x38,0x38,0x1C,0xFF,0xFF,0x00,0x0F,0x1F,0x1F,0x3B,0x3B,0x1F,0x1F,0x07,0x0F,
	0x1F,0x1F,0x38,0x38,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x78,0xFC,0xFC,0xCE,0xCF,
	0xC7,0xC3,0x18,0x1F,0x1F,0x03,0x03,0x1F,0x1F,0x18,0x1E,0x1F,0x3F,0x3B,0x1B,0x01,
	0x18,0xFF,0xFF,0x18,0x18,0x00,0x0F,0x1F,0x3B,0x3B,0x3B,0x1F,0x0F,0x00,0x1F,0x1F,
	0x1C,0x38,0x3C,0x1F,0x1F,0x18,0x38,0x3F,0x1F,0x07,0x00,0x1F,0x3F,0x3B,0x3B,0x19,

	0x00,0x00,0xF0,0xF0,0x70,0x70,0x70,0x70,0x00,0xF0,0xF0,0xF0,0x00,0x00,0xF0,0xF0,
	0x00,0x00,0x00,0xF0,0xF0,0x00,0xF0,0xF0,0xF0,0x70,0x70,0xF0,0xE0,0x80,0xC0,0xE0,
	0xF0,0x70,0x30,0x70,0x70,0x00,0xC0,0xE0,0xF0,0x70,0x70,0xE0,0xF0,0xF0,0x00,0xE0,
	0xF0,0x70,0x70,0x70,0xF0,0xF0,0x00,0xC0,0xE0,0xF0,0x70,0x30,0x70,0x70,0x00,0xC0,
	0xE0,0xF0,0x70,0x70,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x60,0x70,0x70,0x30,0x70,
	0xF0,0xE0,0x00,0x00,0xEE,0xFE,0xFC,0xE0,0x00,0x00,0x70,0x70,0x30,0xF0,0xF0,0xE0,
	0x00,0xE0,0xF0,0x70,0x70,0x00,0xE0,0xF0,0x70,0x30,0x30,0x70,0x60,0x00,0xF0,0xF0,
	0x00,0x00,0x00,0xF0,0xF0,0x00,0x00,0xF0,0xF0,0xF0,0x60,0x70,0x30,0xB0,0xF0,0xE0
};

//font to print ascii characters
const uint8_t font[] PROGMEM = {
	0x0, 0x0, 0x0, 0x0, 0x0,       // Ascii 0
	0x7C, 0xDA, 0xF2, 0xDA, 0x7C,  //ASC(01)
	0x7C, 0xD6, 0xF2, 0xD6, 0x7C,  //ASC(02)
	0x38, 0x7C, 0x3E, 0x7C, 0x38,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x38, 0xEA, 0xBE, 0xEA, 0x38,
	0x38, 0x7A, 0xFE, 0x7A, 0x38,
	0x0, 0x18, 0x3C, 0x18, 0x0,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x0, 0x18, 0x24, 0x18, 0x0,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0xC, 0x12, 0x5C, 0x60, 0x70,
	0x64, 0x94, 0x9E, 0x94, 0x64,
	0x2, 0xFE, 0xA0, 0xA0, 0xE0,
	0x2, 0xFE, 0xA0, 0xA4, 0xFC,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0xFE, 0x7C, 0x38, 0x38, 0x10,
	0x10, 0x38, 0x38, 0x7C, 0xFE,
	0x28, 0x44, 0xFE, 0x44, 0x28,
	0xFA, 0xFA, 0x0, 0xFA, 0xFA,
	0x60, 0x90, 0xFE, 0x80, 0xFE,
	0x0, 0x66, 0x91, 0xA9, 0x56,
	0x6, 0x6, 0x6, 0x6, 0x6,
	0x29, 0x45, 0xFF, 0x45, 0x29,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x8, 0x4, 0x7E, 0x4, 0x8,
	0x10, 0x10, 0x54, 0x38, 0x10,
	0x10, 0x38, 0x54, 0x10, 0x10,
	0x78, 0x8, 0x8, 0x8, 0x8,
	0x30, 0x78, 0x30, 0x78, 0x30,
	0xC, 0x1C, 0x7C, 0x1C, 0xC,
	0x60, 0x70, 0x7C, 0x70, 0x60,
	0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0xFA, 0x0, 0x0,
	0x0, 0xE0, 0x0, 0xE0, 0x0,
	0x28, 0xFE, 0x28, 0xFE, 0x28,
	0x24, 0x54, 0xFE, 0x54, 0x48,
	0xC4, 0xC8, 0x10, 0x26, 0x46,
	0x6C, 0x92, 0x6A, 0x4, 0xA,
	0x0, 0x10, 0xE0, 0xC0, 0x0,
	0x0, 0x38, 0x44, 0x82, 0x0,
	0x0, 0x82, 0x44, 0x38, 0x0,
	0x54, 0x38, 0xFE, 0x38, 0x54,
	0x10, 0x10, 0x7C, 0x10, 0x10,
	0x0, 0x1, 0xE, 0xC, 0x0,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x0, 0x0, 0x6, 0x6, 0x0,
	0x4, 0x8, 0x10, 0x20, 0x40,
	0x7C, 0x8A, 0x92, 0xA2, 0x7C,
	0x0, 0x42, 0xFE, 0x2, 0x0,
	0x4E, 0x92, 0x92, 0x92, 0x62,
	0x84, 0x82, 0x92, 0xB2, 0xCC,
	0x18, 0x28, 0x48, 0xFE, 0x8,
	0xE4, 0xA2, 0xA2, 0xA2, 0x9C,
	0x3C, 0x52, 0x92, 0x92, 0x8C,
	0x82, 0x84, 0x88, 0x90, 0xE0,
	0x6C, 0x92, 0x92, 0x92, 0x6C,
	0x62, 0x92, 0x92, 0x94, 0x78,
	0x0, 0x0, 0x28, 0x0, 0x0,
	0x0, 0x2, 0x2C, 0x0, 0x0,
	0x0, 0x10, 0x28, 0x44, 0x82,
	0x28, 0x28, 0x28, 0x28, 0x28,
	0x0, 0x82, 0x44, 0x28, 0x10,
	0x40, 0x80, 0x9A, 0x90, 0x60,
	0x7C, 0x82, 0xBA, 0x9A, 0x72,
	0x3E, 0x48, 0x88, 0x48, 0x3E,
	0xFE, 0x92, 0x92, 0x92, 0x6C,
	0x7C, 0x82, 0x82, 0x82, 0x44,
	0xFE, 0x82, 0x82, 0x82, 0x7C,
	0xFE, 0x92, 0x92, 0x92, 0x82,
	0xFE, 0x90, 0x90, 0x90, 0x80,
	0x7C, 0x82, 0x82, 0x8A, 0xCE,
	0xFE, 0x10, 0x10, 0x10, 0xFE,
	0x0, 0x82, 0xFE, 0x82, 0x0,
	0x4, 0x2, 0x82, 0xFC, 0x80,
	0xFE, 0x10, 0x28, 0x44, 0x82,
	0xFE, 0x2, 0x2, 0x2, 0x2,
	0xFE, 0x40, 0x38, 0x40, 0xFE,
	0xFE, 0x20, 0x10, 0x8, 0xFE,
	0x7C, 0x82, 0x82, 0x82, 0x7C,
	0xFE, 0x90, 0x90, 0x90, 0x60,
	0x7C, 0x82, 0x8A, 0x84, 0x7A,
	0xFE, 0x90, 0x98, 0x94, 0x62,
	0x64, 0x92, 0x92, 0x92, 0x4C,
	0xC0, 0x80, 0xFE, 0x80, 0xC0,
	0xFC, 0x2, 0x2, 0x2, 0xFC,
	0xF8, 0x4, 0x2, 0x4, 0xF8,
	0xFC, 0x2, 0x1C, 0x2, 0xFC,
	0xC6, 0x28, 0x10, 0x28, 0xC6,
	0xC0, 0x20, 0x1E, 0x20, 0xC0,
	0x86, 0x9A, 0x92, 0xB2, 0xC2,
	0x0, 0xFE, 0x82, 0x82, 0x82,
	0x40, 0x20, 0x10, 0x8, 0x4,
	0x0, 0x82, 0x82, 0x82, 0xFE,
	0x20, 0x40, 0x80, 0x40, 0x20,
	0x2, 0x2, 0x2, 0x2, 0x2,
	0x0, 0xC0, 0xE0, 0x10, 0x0,
	0x4, 0x2A, 0x2A, 0x1E, 0x2,
	0xFE, 0x14, 0x22, 0x22, 0x1C,
	0x1C, 0x22, 0x22, 0x22, 0x14,
	0x1C, 0x22, 0x22, 0x14, 0xFE,
	0x1C, 0x2A, 0x2A, 0x2A, 0x18,
	0x0, 0x10, 0x7E, 0x90, 0x40,
	0x18, 0x25, 0x25, 0x39, 0x1E,
	0xFE, 0x10, 0x20, 0x20, 0x1E,
	0x0, 0x22, 0xBE, 0x2, 0x0,
	0x4, 0x2, 0x2, 0xBC, 0x0,
	0xFE, 0x8, 0x14, 0x22, 0x0,
	0x0, 0x82, 0xFE, 0x2, 0x0,
	0x3E, 0x20, 0x1E, 0x20, 0x1E,
	0x3E, 0x10, 0x20, 0x20, 0x1E,
	0x1C, 0x22, 0x22, 0x22, 0x1C,
	0x3F, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0x3F,
	0x3E, 0x10, 0x20, 0x20, 0x10,
	0x12, 0x2A, 0x2A, 0x2A, 0x24,
	0x20, 0x20, 0xFC, 0x22, 0x24,
	0x3C, 0x2, 0x2, 0x4, 0x3E,
	0x38, 0x4, 0x2, 0x4, 0x38,
	0x3C, 0x2, 0xC, 0x2, 0x3C,
	0x22, 0x14, 0x8, 0x14, 0x22,
	0x32, 0x9, 0x9, 0x9, 0x3E,
	0x22, 0x26, 0x2A, 0x32, 0x22,
	0x0, 0x10, 0x6C, 0x82, 0x0,
	0x0, 0x0, 0xEE, 0x0, 0x0,
	0x0, 0x82, 0x6C, 0x10, 0x0,
	0x40, 0x80, 0x40, 0x20, 0x40,
	0x3C, 0x64, 0xC4, 0x64, 0x3C,
	0x78, 0x85, 0x85, 0x86, 0x48,
	0x5C, 0x2, 0x2, 0x4, 0x5E,
	0x1C, 0x2A, 0x2A, 0xAA, 0x9A,
	0x84, 0xAA, 0xAA, 0x9E, 0x82,
	0x84, 0x2A, 0x2A, 0x1E, 0x82,
	0x84, 0xAA, 0x2A, 0x1E, 0x2,
	0x4, 0x2A, 0xAA, 0x9E, 0x2,
	0x30, 0x78, 0x4A, 0x4E, 0x48,
	0x9C, 0xAA, 0xAA, 0xAA, 0x9A,
	0x9C, 0x2A, 0x2A, 0x2A, 0x9A,
	0x9C, 0xAA, 0x2A, 0x2A, 0x1A,
	0x0, 0x0, 0xA2, 0x3E, 0x82,
	0x0, 0x40, 0xA2, 0xBE, 0x42,
	0x0, 0x80, 0xA2, 0x3E, 0x2,
	0xF, 0x94, 0x24, 0x94, 0xF,
	0xF, 0x14, 0xA4, 0x14, 0xF,
	0x3E, 0x2A, 0xAA, 0xA2, 0x0,
	0x4, 0x2A, 0x2A, 0x3E, 0x2A,
	0x3E, 0x50, 0x90, 0xFE, 0x92,
	0x4C, 0x92, 0x92, 0x92, 0x4C,
	0x4C, 0x12, 0x12, 0x12, 0x4C,
	0x4C, 0x52, 0x12, 0x12, 0xC,
	0x5C, 0x82, 0x82, 0x84, 0x5E,
	0x5C, 0x42, 0x2, 0x4, 0x1E,
	0x0, 0xB9, 0x5, 0x5, 0xBE,
	0x9C, 0x22, 0x22, 0x22, 0x9C,
	0xBC, 0x2, 0x2, 0x2, 0xBC,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x12, 0x7E, 0x92, 0xC2, 0x66,
	0xD4, 0xF4, 0x3F, 0xF4, 0xD4,
	0xFF, 0x90, 0x94, 0x6F, 0x4,
	0x3, 0x11, 0x7E, 0x90, 0xC0,
	0x4, 0x2A, 0x2A, 0x9E, 0x82,
	0x0, 0x0, 0x22, 0xBE, 0x82,
	0xC, 0x12, 0x12, 0x52, 0x4C,
	0x1C, 0x2, 0x2, 0x44, 0x5E,
	0x0, 0x5E, 0x50, 0x50, 0x4E,
	0xBE, 0xB0, 0x98, 0x8C, 0xBE,
	0x64, 0x94, 0x94, 0xF4, 0x14,
	0x64, 0x94, 0x94, 0x94, 0x64,
	0xC, 0x12, 0xB2, 0x2, 0x4,
	0x1C, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x1C,
	0xF4, 0x8, 0x13, 0x35, 0x5D,
	0xF4, 0x8, 0x14, 0x2C, 0x5F,
	0x0, 0x0, 0xDE, 0x0, 0x0,
	0x10, 0x28, 0x54, 0x28, 0x44,
	0x44, 0x28, 0x54, 0x28, 0x10,
	0x55, 0x0, 0xAA, 0x0, 0x55,
	0x55, 0xAA, 0x55, 0xAA, 0x55,
	0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x0, 0x0, 0x0, 0xFF, 0x0,
	0x8, 0x8, 0x8, 0xFF, 0x0,
	0x28, 0x28, 0x28, 0xFF, 0x0,
	0x8, 0x8, 0xFF, 0x0, 0xFF,
	0x8, 0x8, 0xF, 0x8, 0xF,
	0x28, 0x28, 0x28, 0x3F, 0x0,
	0x28, 0x28, 0xEF, 0x0, 0xFF,
	0x0, 0x0, 0xFF, 0x0, 0xFF,
	0x28, 0x28, 0x2F, 0x20, 0x3F,
	0x28, 0x28, 0xE8, 0x8, 0xF8,
	0x8, 0x8, 0xF8, 0x8, 0xF8,
	0x28, 0x28, 0x28, 0xF8, 0x0,
	0x8, 0x8, 0x8, 0xF, 0x0,
	0x0, 0x0, 0x0, 0xF8, 0x8,
	0x8, 0x8, 0x8, 0xF8, 0x8,
	0x8, 0x8, 0x8, 0xF, 0x8,
	0x0, 0x0, 0x0, 0xFF, 0x8,
	0x8, 0x8, 0x8, 0x8, 0x8,
	0x8, 0x8, 0x8, 0xFF, 0x8,
	0x0, 0x0, 0x0, 0xFF, 0x28,
	0x0, 0x0, 0xFF, 0x0, 0xFF,
	0x0, 0x0, 0xF8, 0x8, 0xE8,
	0x0, 0x0, 0x3F, 0x20, 0x2F,
	0x28, 0x28, 0xE8, 0x8, 0xE8,
	0x28, 0x28, 0x2F, 0x20, 0x2F,
	0x0, 0x0, 0xFF, 0x0, 0xEF,
	0x28, 0x28, 0x28, 0x28, 0x28,
	0x28, 0x28, 0xEF, 0x0, 0xEF,
	0x28, 0x28, 0x28, 0xE8, 0x28,
	0x8, 0x8, 0xF8, 0x8, 0xF8,
	0x28, 0x28, 0x28, 0x2F, 0x28,
	0x8, 0x8, 0xF, 0x8, 0xF,
	0x0, 0x0, 0xF8, 0x8, 0xF8,
	0x0, 0x0, 0x0, 0xF8, 0x28,
	0x0, 0x0, 0x0, 0x3F, 0x28,
	0x0, 0x0, 0xF, 0x8, 0xF,
	0x8, 0x8, 0xFF, 0x8, 0xFF,
	0x28, 0x28, 0x28, 0xFF, 0x28,
	0x8, 0x8, 0x8, 0xF8, 0x0,
	0x0, 0x0, 0x0, 0xF, 0x8,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF, 0xF, 0xF, 0xF, 0xF,
	0xFF, 0xFF, 0xFF, 0x0, 0x0,
	0x0, 0x0, 0x0, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x1C, 0x22, 0x22, 0x1C, 0x22,
	0x3E, 0x54, 0x54, 0x7C, 0x28,
	0x7E, 0x40, 0x40, 0x60, 0x60,
	0x40, 0x7E, 0x40, 0x7E, 0x40,
	0xC6, 0xAA, 0x92, 0x82, 0xC6,
	0x1C, 0x22, 0x22, 0x3C, 0x20,
	0x2, 0x7E, 0x4, 0x78, 0x4,
	0x60, 0x40, 0x7E, 0x40, 0x40,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x38, 0x54, 0x92, 0x54, 0x38,
	0x32, 0x4E, 0x80, 0x4E, 0x32,
	0xC, 0x52, 0xB2, 0xB2, 0xC,
	0xC, 0x12, 0x1E, 0x12, 0xC,
	0x3D, 0x46, 0x5A, 0x62, 0xBC,
	0x7C, 0x92, 0x92, 0x92, 0x0,
	0x7E, 0x80, 0x80, 0x80, 0x7E,
	0x54, 0x54, 0x54, 0x54, 0x54,
	0x22, 0x22, 0xFA, 0x22, 0x22,
	0x2, 0x8A, 0x52, 0x22, 0x2,
	0x2, 0x22, 0x52, 0x8A, 0x2,
	0x0, 0x0, 0xFF, 0x80, 0xC0,
	0x7, 0x1, 0xFF, 0x0, 0x0,
	0x10, 0x10, 0xD6, 0xD6, 0x10,
	0x6C, 0x48, 0x6C, 0x24, 0x6C,
	0x60, 0xF0, 0x90, 0xF0, 0x60,
	0x0, 0x0, 0x18, 0x18, 0x0,
	0x0, 0x0, 0x8, 0x8, 0x0,
	0xC, 0x2, 0xFF, 0x80, 0x80,
	0x0, 0xF8, 0x80, 0x80, 0x78,
	0x0, 0x98, 0xB8, 0xE8, 0x48,
    0x0, 0x3C, 0x3C, 0x3C, 0x3C,};

//lcd clear screen function
void clear_screen(void) {
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {
    
    lcd_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      lcd_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      lcd_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      lcd_data(0x0);
    }     
  }
}


//lcd initialization function
void lcd_init(void) {
  // set pin directions
  SID_DDR |= _BV(SID);
  SCLK_DDR |= _BV(SCLK);
  A0_DDR |= _BV(A0);
  RST_DDR |= _BV(RST);
  CS_DDR |= _BV(CS);
  
  // toggle RST low to reset; CS low so it'll listen to us
  CS_PORT &= ~_BV(CS);
  RST_PORT &= ~_BV(RST);
  _delay_ms(500);
  RST_PORT |= _BV(RST);

  // LCD bias select
  lcd_command(CMD_SET_BIAS_7);
  // ADC select
  lcd_command(CMD_SET_ADC_NORMAL);
  // SHL select
  lcd_command(CMD_SET_COM_NORMAL);
  // Initial display line
  lcd_command(CMD_SET_DISP_START_LINE);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  lcd_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  lcd_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  lcd_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  _delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  lcd_command(CMD_SET_RESISTOR_RATIO | 0x6);

}

inline void spiwrite(uint8_t c) {
  int8_t i;
  for (i=7; i>=0; i--) {
    SCLK_PORT &= ~_BV(SCLK);
    if (c & _BV(i))
      SID_PORT |= _BV(SID);
    else
      SID_PORT &= ~_BV(SID);
    SCLK_PORT |= _BV(SCLK);
  }

 
}
void lcd_command(uint8_t c) {
  A0_PORT &= ~_BV(A0);

  spiwrite(c);
}

void lcd_data(uint8_t c) {
  A0_PORT |= _BV(A0);

  spiwrite(c);
}
void lcd_set_brightness(uint8_t val) {
    lcd_command(CMD_SET_VOLUME_FIRST);
    lcd_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}

//write to the lcd buffer
void write_buffer(uint8_t *buff) {
  uint8_t c, p;

  for(p = 0; p < 8; p++) {
    
    lcd_command(CMD_SET_PAGE | pagemap[p]);
    lcd_command(CMD_SET_COLUMN_LOWER | (0x0 & 0xf));
    lcd_command(CMD_SET_COLUMN_UPPER | ((0x0 >> 4) & 0xf));
    lcd_command(CMD_RMW);
    lcd_data(0xff);
    
    
    for(c = 0; c < 128; c++) {
      lcd_data(buff[(128*p)+c]);
    }
  }
}

// function to clear everything in the buffer
void clear_buffer(uint8_t *buff) {
	memset(buff, 0, 1024);
}

//*********Write the functions mentioned below ********//

//**********an example to get started************//

// this function writes a character on the lcd at a coordinate
void drawchar(uint8_t *buff, uint8_t x, uint8_t line, uint8_t c) {
	for (uint8_t i =0; i<5; i++ ) {
		buff[x + (line*128) ] = pgm_read_byte(font+(c*5)+i);
		x++;
	}
}
//*******************//


// the most basic function, set a single pixel
void setpixel(uint8_t *buff, uint8_t x, uint8_t y, uint8_t color) {

   uint8_t bte = x + y / 8 * 128;
   uint8_t shift = y % 8;
   shift = 8 - shift;
   buff[bte] = (1 << 7);
   buff[bte] |= (1 << 0);
   if (color)
   {
	//buff[bte] |= _BV(shift);
   }
   else {
	 //buff[bte] &= ~_BV(shift);
   }
}

// function to clear a single pixel
void clearpixel(uint8_t *buff, uint8_t x, uint8_t y) {
	
}

// function to write a string on the lcd
void drawstring(uint8_t *buff, uint8_t x, uint8_t line, uint8_t *c) {
	
}

// use bresenham's algorithm to write this function to draw a line
void drawline(uint8_t *buff,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint8_t color) {

}

// function to draw a filled rectangle
void fillrect(uint8_t *buff,uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t color) {
	
}


// function to draw a rectangle
void drawrect(uint8_t *buff,uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t color) {
	
}


// function to draw a circle
void drawcircle(uint8_t *buff,uint8_t x0, uint8_t y0, uint8_t r,uint8_t color) {
	
}


// function to draw a filled circle
void fillcircle(uint8_t *buff,uint8_t x0, uint8_t y0, uint8_t r,uint8_t color) {
	
}

