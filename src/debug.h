/* Author: Steve Gunn
Licence: Creative Commons Attribution License
   Notes: For AVR UART0 debugging
*/
#pragma once

#include <avr/io.h>
#include <stdio.h>

#define DEBUG_BAUD 9600

// Wrap functions in extern "C" for C++ compatibility
extern "C" {
	int uputchar0(char c, FILE *stream);
	int ugetchar0(FILE *stream);
}

// Transmit function with proper signature
int uputchar0(char c, FILE *stream) {
	if (c == '\n') uputchar0('\r', stream);
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
	return c;
}

// Receive function with proper signature
int ugetchar0(FILE *stream) {
	while (!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

// Initialize UART0 with C++ compatibility
void init_debug_uart0() {
	// Baud rate calculation
	UBRR0H = (F_CPU / (DEBUG_BAUD * 16UL) - 1) >> 8;
	UBRR0L = (F_CPU / (DEBUG_BAUD * 16UL) - 1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	// C++ compatible stream setup
	static FILE uout;
	static FILE uin;

	fdev_setup_stream(&uout, uputchar0, NULL, _FDEV_SETUP_WRITE);
	fdev_setup_stream(&uin, NULL, ugetchar0, _FDEV_SETUP_READ);

	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
}