#ifndef UART_H
#define UART_H

#define BAUD 9600
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>
static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
void uart_init(void);

static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);


#endif // UART_H
